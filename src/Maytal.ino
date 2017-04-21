#define VERSION "0.01"


#include <Adafruit_GFX.h>     //  For OLED
#include <Adafruit_SSD1306.h> //  For OLED
//#include <RTClib.h>           //  For RTC
#include <SPI.h>              //  For OLED
#include <Wire.h>             //  For RTC



float pressure = 0.00;

#define OLED_RESET 4
#define PRESSURE_PIN A0

Adafruit_SSD1306 display(OLED_RESET);
//RTC_DS3231 rtc;

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif




void setup()
{
      Serial.begin(57600);
      display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

      /*if(rtc.begin() == false)
      {
              display.clearDisplay();
              display.setTextSize(2);
              display.setTextColor(WHITE);
              display.setCursor(0,0);
              display.println(F("Couldn't"));
              display.println(F("find RTC"));
              wait(3000);
      }*/
      
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print(F("Maytal v"));
      display.println(VERSION);
      display.println(__DATE__);    //  Compile data and time helps identify software uploads
      display.println(__TIME__);
      display.display();
      
      wait(3000);
}



void loop()
{
      int rawPressure = analogRead(PRESSURE_PIN);

      //  Validate that analogRead is between 0.5v-4.5v range of transducer
      if(rawPressure < 102 || rawPressure > 922)
      {
              pressure = 0;
      }
      else
      {
              pressure = ((rawPressure - 102.4) / 4.708);
      }
      
      display.clearDisplay();
      display.setTextSize(3);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
//      display.print(rawPressure);
      display.print(pressure, 1);
      display.setTextSize(2);
      display.print(F(" PSI"));
      display.display();
      
      wait(1000);      
}



void wait(unsigned int ms)  //  Non-blocking delay function
{
      unsigned long period = millis() + ms;
      while(millis() < period)
      {
            Serial.flush();
      }
}

