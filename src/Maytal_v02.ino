#define VERSION "0.02"
#define UTCOFFSET -5          //  Local standard time variance from UTC
#define INTERVAL 15           //  Number of minutes between readings
#define PUBLIC_KEY "XXXXXX"   //  Public key for phant stream
#define PRIVATE_KEY "XXXXXX"  //  Private key for phant stream
//- - - - - - - - - - - - - - - - - - - - - -

#include <Adafruit_FONA.h>    //  https://github.com/adafruit/Adafruit_FONA
#include <DS3232RTC.h>        //  https://github.com/JChristensen/DS3232RTC
#include <Sleep_n0m1.h>       //  https://github.com/n0m1/Sleep_n0m1
#include <SoftwareSerial.h>   //  Standard software serial library
#include <TimeLib.h>          //  Standard time library
#include <Wire.h>             //  Standard wire library for comms with RTC



#define PRESSURE_PIN A0       //  Pressure reading pin from transducer
#define RTC_INTERRUPT_PIN 2   //  Square wave pin from RTC
#define Q1 3                  //  Base pin for transducer switch
#define FONA_RST 5            //  FONA reset pin
#define FONA_RX 6             //  UART pin to FONA
#define FONA_TX 7             //  UART pin from FONA
#define FONA_KEY 8            //  FONA Key pin
#define FONA_PS 9             //  FONA power status pin

time_t currentTime;
int lastPressure = 0;
boolean sentData = false;



Adafruit_FONA fona = Adafruit_FONA (FONA_RST);
SoftwareSerial fonaSerial = SoftwareSerial (FONA_TX, FONA_RX);

Sleep sleep;




static void rtcIRQ()
{
      RTC.alarm(ALARM_1);
}



void setup()
{
      pinMode(RTC_INTERRUPT_PIN, INPUT_PULLUP);
      pinMode(FONA_KEY, OUTPUT);
      pinMode(FONA_RX, OUTPUT);
      pinMode(Q1, OUTPUT);
      
      Serial.begin(57600);
      Serial.println();
      Serial.print(F("Maytal v"));
      Serial.println(VERSION);
      Serial.print(__DATE__);   //  Compile data and time helps identify software uploads
      Serial.print(F(" "));
      Serial.println(__TIME__);

      digitalWrite(Q1, LOW);    //  Turn off transducer to save power

      fonaOn();
      clockSet();
        
      //  Delete any accumulated SMS messages to avoid interference from old commands
      //fona.sendCheckReply (F("AT+CMGF=1"), F("OK"));            //  Enter text mode
      //fona.sendCheckReply (F("AT+CMGDA=\"DEL ALL\""), F("OK")); //  Delete all SMS messages
        
      fonaOff();

      //  Set up the RTC interrupts on every minute at the 00 second mark
      RTC.squareWave(SQWAVE_NONE);
      RTC.setAlarm(ALM1_MATCH_SECONDS, 0, 0, 0, 1);
      RTC.alarm(ALARM_1);
      RTC.alarmInterrupt(ALARM_1, true);
      attachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN), rtcIRQ, FALLING);
      interrupts();
}



void loop()
{
      currentTime = RTC.get();
      
      Serial.print(hour(currentTime));
      Serial.print(F(":"));
      if(minute(currentTime) < 10)
      {
              Serial.print(F("0"));
      }
      Serial.println(minute(currentTime));
      
      int currentPressure = readPressure();

      Serial.println(currentPressure);
      
      //  If pressure has changed more than 10% since last upload, upload new reading
      if(((lastPressure*100)-(currentPressure*100))/currentPressure >= 10)
      {
              sendPressure(currentPressure);
      }

      //  If it is time to send a scheduled reading, send it
      if (minute(currentTime) % INTERVAL == 0 && sentData == false)
      {
              if(sendPressure(currentPressure) == true)
              {
                      sentData = true;
              }
      }
      else
      {
              sentData = false;
      }
      
      Serial.flush();         //  Flush any serial output before sleep
      RTC.setAlarm(ALM1_MATCH_SECONDS, 0, 0, 0, 1);
      RTC.alarm(ALARM_1);     //  Clear any outstanding RTC alarms
      RTC.alarmInterrupt(ALARM_1, true);
      sleep.pwrDownMode();
      wait(500);
      sleep.sleepInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN), FALLING); //  Sleep; wake on falling voltage on RTC pin
}



boolean fonaOn()
{
        if(digitalRead(FONA_PS) == LOW)             //  If the FONA is off...
        {
                Serial.print(F("Powering FONA on..."));
                while (digitalRead(FONA_PS) == LOW) 
                {
                        digitalWrite(FONA_KEY, LOW);  //  ...pulse the Key pin low...
                        wait(500);
                }
                digitalWrite(FONA_KEY, HIGH);        //  ...and then return it to high
                Serial.println(F(" done."));
        }
        
        Serial.println(F("Initializing FONA..."));
        
        fonaSerial.begin(4800);                      //  Open a serial interface to FONA
        
        if(fona.begin(fonaSerial) == false)         //  Start the FONA on serial interface
        {
                Serial.println(F("FONA not found. Check wiring and power."));
                return false;
        }
        else
        {
                Serial.print(F("FONA online. "));
                
                unsigned long gsmTimeout = millis() + 30000;
                boolean gsmTimedOut = false;

                Serial.print(F("Waiting for GSM network... "));
                while(1)
                {
                        byte network_status = fona.getNetworkStatus();
                        if(network_status == 1 || network_status == 5) break;
                        
                        if(millis() >= gsmTimeout)
                        {
                                gsmTimedOut = true;
                                break;
                        }
                        
                        wait(250);
                }

                if(gsmTimedOut == true)
                {
                        Serial.println(F("timed out. Check SIM card, antenna, and signal."));
                        return false;
                }
                else
                {
                        Serial.println(F("done."));
                }

                //  RSSI is a measure of signal strength -- higher is better; less than 10 is worrying
                byte rssi = fona.getRSSI();
                Serial.print(F("RSSI: "));
                Serial.println(rssi);

                wait(3000);    //  Give the network a moment

                //fona.setGPRSNetworkSettings(F("cellcard"));    //  Set APN to your local carrier

                if(rssi > 5)
                {
                        if(fona.enableGPRS(true) == false);
                        {
                              //  Sometimes enableGPRS() returns false even though it succeeded
                              if(fona.GPRSstate() != 1)
                              {
                                      for(byte GPRSattempts = 0; GPRSattempts < 5; GPRSattempts++)
                                      {
                                            Serial.println(F("Trying again..."));
                                            wait(2000);
                                            fona.enableGPRS(true);
                                            
                                            if(fona.GPRSstate() == 1)
                                            {
                                                    Serial.println(F("GPRS is on."));
                                                    break;
                                            }
                                            else
                                            {
                                                    Serial.print(F("Failed to turn GPRS on... "));
                                            }
                                      }
                              }
                        }
                }
                else
                {
                        Serial.println(F("Can't transmit, network signal strength is poor."));
                        gsmTimedOut = true;
                }
                
                return true;
        }
}



void clockSet()
{
        char method;
        
        wait(1000);    //  Give time for any trailing data to come in from FONA

        int netOffset;

        char theDate[17];

        Serial.println(F("Attempting to get time from GSM location service..."));

        flushFona();    //  Flush any trailing data

        fona.sendCheckReply(F("AT+CIPGSMLOC=2,1"), F("OK"));    //  Query GSM location service for time
        
        fona.parseInt();                    //  Ignore first int
        int secondInt = fona.parseInt();    //  Ignore second int -- necessary on some networks/towers
        int netYear = fona.parseInt();      //  Get the results -- GSMLOC year is 4-digit
        int netMonth = fona.parseInt();
        int netDay = fona.parseInt();
        int netHour = fona.parseInt();      //  GSMLOC is _supposed_ to get UTC; we will adjust
        int netMinute = fona.parseInt();
        int netSecond = fona.parseInt();    //  Our seconds may lag slightly, of course

        if(netYear < 2016 || netYear > 2050 || netHour > 23) //  If that obviously didn't work...
        {
                netSecond = netMinute;  //  ...shift everything up one to capture that second int
                netMinute = netHour;
                netHour = netDay;
                netDay = netMonth;
                netMonth = netYear;
                netYear = secondInt;

                Serial.println(F("Recombobulating..."));
        }

        if(netYear < 2016 || netYear > 2050 || netHour > 23)   // If that still didn't work...
        {
                Serial.println(F("GSM location service failed."));
                /*   ...the we'll get time from the NTP pool instead: 
                 *  (https://en.wikipedia.org/wiki/Network_Time_Protocol)
                 */
                fona.enableNTPTimeSync(true, F("0.daimakerlab.pool.ntp.org"));
                Serial.println(F("Attempting to enable NTP sync."));
                
                wait(15000);                 // Wait for NTP server response
                
                fona.println(F("AT+CCLK?")); // Query FONA's clock for resulting NTP time              
                netYear = fona.parseInt();    // Capture the results
                netMonth = fona.parseInt();
                netDay = fona.parseInt();
                netHour = fona.parseInt();    // We asked NTP for UTC and will adjust below
                netMinute = fona.parseInt();
                netSecond = fona.parseInt();  // Our seconds may lag slightly
        
                method = 'N';
        }
        else 
        {
                method = 'G';
        }

        if((netYear < 1000 && netYear >= 16 && netYear < 50) || (netYear > 1000 && netYear >= 2016 && netYear < 2050))
        //  If we got something that looks like a valid date...
        {
                //  Adjust UTC to local time
                if((netHour + UTCOFFSET) < 0)                   //  If our offset + the UTC hour < 0...
                {
                        netHour = (24 + netHour + UTCOFFSET);   //  ...add 24...
                        netDay = (netDay - 1);                  //  ...and adjust the date to UTC - 1
                }
                else
                {
                        if((netHour + UTCOFFSET) > 23)          //  If our offset + the UTC hour > 23...
                        {
                                netHour = (netHour + UTCOFFSET - 24); //  ...subtract 24...
                                netDay = (netDay + 1);                //  ...and adjust the date to UTC + 1
                        }
                        else
                        {
                                netHour = (netHour + UTCOFFSET);      //  Otherwise it's straight addition
                        }
                }

                Serial.print(F("Obtained current time: "));
                sprintf(theDate, "%d/%d/%d %d:%d", netDay, netMonth, netYear, netHour, netMinute);
                Serial.println(theDate);
                
                Serial.println(F("Adjusting RTC."));

                setTime(netHour, netMinute, netSecond, netDay, netMonth, netYear);   //set the system time to 23h31m30s on 13Feb2009
                RTC.set(now());                     //set the RTC from the system time
        }
        else
        {
                Serial.println(F("Didn't find reliable time. Will continue to use RTC's current time."));
                method = 'X';
        }

        wait(200);              //  Give FONA a moment to catch its breath
}



void flushFona()
{
        // Read all available serial input from FONA to flush any pending data.
        while(fona.available())
        {
                char c = fona.read();
                Serial.print(c);
        }
}



void fonaOff()
{
        wait(5000);        //  Shorter delays yield unpredictable results

        //  We'll turn GPRS off first, just to have an orderly shutdown
        if(fona.enableGPRS(false) == false)
        {
                if(fona.GPRSstate() == 1)
                {
                        Serial.println(F("Failed to turn GPRS off."));
                }
                else
                {
                        Serial.println(F("GPRS is off."));
                }
        }
      
        wait(500);
      
        // Power down the FONA if it needs it
        if(digitalRead(FONA_PS) == HIGH)      //  If the FONA is on...
        {
                fona.sendCheckReply(F("AT+CPOWD=1"), F("OK")); //  ...send shutdown command...
                digitalWrite(FONA_KEY, HIGH);                  //  ...and set Key high
        }
}



void wait(unsigned int ms)  //  Non-blocking delay function
{
      unsigned long period = millis() + ms;
      while(millis() < period)
      {
            Serial.flush();
      }
}



int readPressure()
{
      digitalWrite(Q1, HIGH);     //  Switch on transducer
      wait(3000);                 //  Let transducer settle
      
      float pressure = 0.00;
      int rawPressure = analogRead(PRESSURE_PIN);

      //  Validate that analogRead is between 0.5v-4.5v range of transducer
      if(rawPressure < 102 || rawPressure > 922)
      {
              pressure = 0;
      }
      else
      {
              pressure = ((rawPressure - 102.4) / 4.708);  //  Convert to PSI
      }

      digitalWrite(Q1, LOW);      //  Switch off transducer to save power

      return(int)pressure;
}



boolean sendPressure(int pressure)
{
      fonaOn();
      
      Serial.println(F("Sending pressure reading..."));

      char url[255];
      
      unsigned int voltage;
      fona.getBattVoltage(&voltage);   //  Read the battery voltage from FONA's ADC

      //  Generate the HTTP GET URL for the Phant server 
      sprintf(url, "data.sparkfun.com/input/%s?private_key=%s&pressure=%d&voltage=%d", PUBLIC_KEY, PRIVATE_KEY, pressure, voltage);
      Serial.print(F("Sending: "));
      Serial.println(url);

      unsigned int httpStatus;
      unsigned int datalen;

      boolean success = false;
      int attempts = 0;

      wait(7500);    //  A long delay here seems to improve reliability

      while(success == false && attempts < 5)   //  We'll attempt up to five times to upload data
      {
              fona.HTTP_GET_start(url, &httpStatus, &datalen);
              fona.HTTP_GET_end();
      
              if(httpStatus == 200)    //  If the HTTP GET request returned a 200, it succeeded
              {
                      Serial.println(F("Upload succeeded."));
                      success = true;
              }
              else
              {
                      Serial.println(F("Upload failed."));
                      success = false;
              }

              /*   Occasionally the FONA returns something other than a 200 even though the upload
               *   succeeded -- maybe that's coming from the server and maybe it isn't -- and so 
               *   you get a duplicate reading sent to Phant. It's not very common, but it does happen.
               */

              attempts++;

              wait(1000);
      }
      
      lastPressure = pressure;

      fonaOff();
      
      return success;
}

