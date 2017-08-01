EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:ArduProMiniTKB
LIBS:fona_808
LIBS:adafruit_ds3231
LIBS:mysensors_mcu
LIBS:mysensors_arduino
LIBS:Maytal-cache
EELAYER 25 0
EELAYER END
$Descr A 11000 8500
encoding utf-8
Sheet 1 1
Title "Maytal IoT Pressure Transducer"
Date "8 Mar 2017"
Rev "v0.2r0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L FONA_808 U1
U 1 1 58C049A8
P 5400 2450
F 0 "U1" H 5400 2400 60  0000 C CNN
F 1 "FONA_808" H 5400 2500 60  0000 C CNN
F 2 "DAI_Maker_Lab_footprints:FONA_808" H 5400 2450 60  0001 C CNN
F 3 "" H 5400 2450 60  0000 C CNN
	1    5400 2450
	0    -1   1    0   
$EndComp
$Comp
L CONN_01X03 P2
U 1 1 58C04F0E
P 6900 2800
F 0 "P2" H 6900 3000 50  0000 C CNN
F 1 "CONN_01X03" V 7000 2800 50  0000 C CNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_PT-3.5mm_3pol" H 6900 2800 50  0001 C CNN
F 3 "" H 6900 2800 50  0000 C CNN
	1    6900 2800
	0    -1   -1   0   
$EndComp
Text GLabel 7050 3100 2    60   Input ~ 0
GND
$Comp
L PN2222A Q1
U 1 1 58C050A2
P 7000 3350
F 0 "Q1" H 7200 3425 50  0000 L CNN
F 1 "PN2222A" H 7200 3350 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Inline_Narrow_Oval" H 7200 3275 50  0001 L CIN
F 3 "" H 7000 3350 50  0000 L CNN
	1    7000 3350
	-1   0    0    1   
$EndComp
Text GLabel 6900 3650 3    60   Input ~ 0
VCC
Text GLabel 7300 3350 2    60   Input ~ 0
BASE
$Comp
L Adafruit_DS3231 U2
U 1 1 58C053A0
P 3300 4250
F 0 "U2" H 3300 4250 60  0000 C CNN
F 1 "Adafruit_DS3231" H 3300 4400 60  0000 C CNN
F 2 "DAI_Maker_Lab_footprints:Adafruit_DS3231" H 3300 4250 60  0001 C CNN
F 3 "" H 3300 4250 60  0000 C CNN
	1    3300 4250
	0    -1   -1   0   
$EndComp
Text GLabel 3850 4700 3    60   Input ~ 0
VCC
Text GLabel 4000 4700 3    60   Input ~ 0
GND
Text GLabel 3800 4300 2    60   Input ~ 0
SDA
Text GLabel 4050 4400 2    60   Input ~ 0
SCL
Text GLabel 3500 2600 2    60   Input ~ 0
RAW
Text GLabel 3300 2600 0    60   Input ~ 0
GND
$Comp
L ArduinoProMini IC1
U 1 1 58C05D7C
P 5700 5150
F 0 "IC1" H 4950 6400 40  0000 L BNN
F 1 "ArduinoProMini" H 6100 3750 40  0000 L BNN
F 2 "DAI_Maker_Lab_footprints:Arduino_Pro_Mini" H 5700 5150 30  0001 C CIN
F 3 "" H 5700 5150 60  0000 C CNN
	1    5700 5150
	0    -1   -1   0   
$EndComp
Text GLabel 7000 6150 2    60   Input ~ 0
GND
Text GLabel 4600 6150 3    60   Input ~ 0
VCC
Text GLabel 4900 6150 3    60   Input ~ 0
RAW
Text GLabel 6600 4050 1    60   Input ~ 0
SDA
Text GLabel 6700 3800 1    60   Input ~ 0
SCL
Text GLabel 4650 3900 0    60   Input ~ 0
BASE
Text GLabel 4650 3400 0    60   Input ~ 0
RAW
Text GLabel 4650 3550 0    60   Input ~ 0
GND
Text GLabel 4650 3700 0    60   Input ~ 0
VCC
Wire Wire Line
	6800 3100 6800 3000
Wire Wire Line
	7050 3100 7000 3100
Wire Wire Line
	7000 3100 7000 3000
Wire Wire Line
	6900 3150 6900 3000
Wire Wire Line
	6900 3650 6900 3550
Wire Wire Line
	7300 3350 7200 3350
Wire Wire Line
	3750 4600 3850 4600
Wire Wire Line
	3850 4600 3850 4700
Wire Wire Line
	3750 4500 4000 4500
Wire Wire Line
	4000 4500 4000 4700
Wire Wire Line
	4050 4400 3750 4400
Wire Wire Line
	3800 4300 3750 4300
Wire Wire Line
	6700 6050 6700 6150
Wire Wire Line
	6700 6150 7000 6150
Wire Wire Line
	6800 6050 6800 6150
Wire Wire Line
	6900 6050 6900 6150
Wire Wire Line
	4600 6050 4600 6150
Wire Wire Line
	4900 6050 4900 6150
Wire Wire Line
	6600 4050 6600 4150
Wire Wire Line
	6700 3800 6700 4150
Wire Wire Line
	6200 4150 6200 3500
Wire Wire Line
	4650 3900 4900 3900
Wire Wire Line
	4900 3900 4900 4150
Wire Wire Line
	3750 4000 4800 4000
Wire Wire Line
	4800 4000 4800 4150
Wire Wire Line
	6200 3500 6400 3500
Wire Wire Line
	6400 3500 6400 3100
Wire Wire Line
	6400 3100 6800 3100
Wire Wire Line
	5100 3250 5100 4150
Wire Wire Line
	4650 3400 4800 3400
Wire Wire Line
	4800 3400 4800 3250
Wire Wire Line
	4650 3550 4900 3550
Wire Wire Line
	4900 3550 4900 3250
Wire Wire Line
	4650 3700 5000 3700
Wire Wire Line
	5000 3700 5000 3250
Wire Wire Line
	5200 3250 5200 4150
Wire Wire Line
	5300 3250 5300 4150
Wire Wire Line
	5500 3250 5500 3650
Wire Wire Line
	5500 3650 5400 3650
Wire Wire Line
	5400 3650 5400 4150
Wire Wire Line
	5700 3250 5700 3750
Wire Wire Line
	5700 3750 5500 3750
Wire Wire Line
	5500 3750 5500 4150
$Comp
L CONN_01X04 P1
U 1 1 59552F28
P 3500 3050
F 0 "P1" H 3500 3300 50  0000 C CNN
F 1 "LIPO_SOLAR" V 3600 3050 50  0000 C CNN
F 2 "DAI_Maker_Lab_footprints:Adafruit_LiPo_Solar" H 3500 3050 50  0001 C CNN
F 3 "" H 3500 3050 50  0000 C CNN
	1    3500 3050
	0    1    1    0   
$EndComp
Wire Wire Line
	3300 2600 3350 2600
Wire Wire Line
	3350 2600 3350 2850
Wire Wire Line
	3500 2600 3450 2600
Wire Wire Line
	3450 2600 3450 2850
$EndSCHEMATC
