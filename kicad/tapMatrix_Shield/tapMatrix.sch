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
LIBS:special
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
LIBS:tapMatrix
LIBS:tapMatrix-cache
EELAYER 24 0
EELAYER END
$Descr B 17000 11000
encoding utf-8
Sheet 1 1
Title "tapMatrix Shield"
Date "03 Sep 2014"
Rev "0.2"
Comp "Responsive Devices, Inc."
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 2550 3250 2    60   ~ 0
Arduino UNO Headers
$Comp
L TS_ZIF P5
U 1 1 53F23B9F
P 5000 2550
F 0 "P5" H 5050 2850 50  0000 C CNN
F 1 "TS_ZIF" V 5200 2550 50  0000 C CNN
F 2 "tapmatrix:SFW4R­5STE1LF" H 5000 2550 60  0001 C CNN
F 3 "" H 5000 2550 60  0000 C CNN
	1    5000 2550
	1    0    0    -1  
$EndComp
$Comp
L LED_MATRIX_HDR P6
U 1 1 53F2DDF1
P 7650 2100
F 0 "P6" H 7650 2850 60  0000 C CNN
F 1 "LED_MATRIX_HDR" H 7650 1400 50  0000 C CNN
F 2 "tapmatrix:5103310­-3" H 7650 2100 60  0001 C CNN
F 3 "http://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=5103310&DocType=Customer+Drawing&DocLang=English" H 7650 2100 60  0001 C CNN
	1    7650 2100
	1    0    0    -1  
$EndComp
NoConn ~ 7000 1700
NoConn ~ 7000 1850
NoConn ~ 7000 2150
NoConn ~ 8250 2150
NoConn ~ 8250 1700
$Comp
L GND #PWR01
U 1 1 53F2DE59
P 6750 2150
F 0 "#PWR01" H 6750 2150 30  0001 C CNN
F 1 "GND" H 6750 2080 30  0001 C CNN
F 2 "" H 6750 2150 60  0000 C CNN
F 3 "" H 6750 2150 60  0000 C CNN
	1    6750 2150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 53F2E04E
P 8650 2600
F 0 "#PWR02" H 8650 2600 30  0001 C CNN
F 1 "GND" H 8650 2530 30  0001 C CNN
F 2 "" H 8650 2600 60  0000 C CNN
F 3 "" H 8650 2600 60  0000 C CNN
	1    8650 2600
	1    0    0    -1  
$EndComp
Text Notes 6500 3250 2    60   ~ 0
Touchscreen ZIF Socket + Optional Header
Text Notes 8000 3250 2    60   ~ 0
LED Matrix Header
$Comp
L SD_CARD U5
U 1 1 53F2E748
P 14700 2000
F 0 "U5" H 14750 2700 60  0000 C CNN
F 1 "SD_CARD" H 14700 1300 60  0000 C CNN
F 2 "tapmatrix:114-00841-68" H 14700 2200 60  0001 C CNN
F 3 "" H 14700 2200 60  0000 C CNN
	1    14700 2000
	1    0    0    -1  
$EndComp
Text Notes 15100 3200 2    60   ~ 0
MicroSD Card Socket
NoConn ~ 14000 1500
$Comp
L GND #PWR03
U 1 1 53F3050A
P 15850 2600
F 0 "#PWR03" H 15850 2600 30  0001 C CNN
F 1 "GND" H 15850 2530 30  0001 C CNN
F 2 "" H 15850 2600 60  0000 C CNN
F 3 "" H 15850 2600 60  0000 C CNN
	1    15850 2600
	1    0    0    -1  
$EndComp
NoConn ~ 15400 2100
$Comp
L GND #PWR04
U 1 1 53F30748
P 13850 2600
F 0 "#PWR04" H 13850 2600 30  0001 C CNN
F 1 "GND" H 13850 2530 30  0001 C CNN
F 2 "" H 13850 2600 60  0000 C CNN
F 3 "" H 13850 2600 60  0000 C CNN
	1    13850 2600
	1    0    0    -1  
$EndComp
$Comp
L 74LS125 U3
U 1 1 53F3911B
P 10100 1650
F 0 "U3" H 10100 1750 50  0000 L BNN
F 1 "74LS125" H 10150 1500 40  0000 L TNN
F 2 "SMD_Packages:SOIC-14_W" H 10100 1650 60  0001 C CNN
F 3 "" H 10100 1650 60  0000 C CNN
	1    10100 1650
	1    0    0    -1  
$EndComp
$Comp
L 74LS125 U3
U 3 1 53F3915E
P 11900 1650
F 0 "U3" H 11900 1750 50  0000 L BNN
F 1 "74LS125" H 11950 1500 40  0000 L TNN
F 2 "SMD_Packages:SOIC-14_W" H 11900 1650 60  0001 C CNN
F 3 "" H 11900 1650 60  0000 C CNN
	3    11900 1650
	1    0    0    -1  
$EndComp
$Comp
L 74LS125 U3
U 2 1 53F391BB
P 10100 2600
F 0 "U3" H 10100 2700 50  0000 L BNN
F 1 "74LS125" H 10150 2450 40  0000 L TNN
F 2 "SMD_Packages:SOIC-14_W" H 10100 2600 60  0001 C CNN
F 3 "" H 10100 2600 60  0000 C CNN
	2    10100 2600
	1    0    0    -1  
$EndComp
$Comp
L 74LS125 U3
U 4 1 53F39266
P 11900 2600
F 0 "U3" H 11900 2700 50  0000 L BNN
F 1 "74LS125" H 11950 2450 40  0000 L TNN
F 2 "SMD_Packages:SOIC-14_W" H 11900 2600 60  0001 C CNN
F 3 "" H 11900 2600 60  0000 C CNN
	4    11900 2600
	1    0    0    -1  
$EndComp
NoConn ~ 11900 2900
NoConn ~ 11450 2600
NoConn ~ 12350 2600
$Comp
L +3.3V #PWR05
U 1 1 53F3A13D
P 9950 1300
F 0 "#PWR05" H 9950 1260 30  0001 C CNN
F 1 "+3.3V" H 9950 1410 30  0000 C CNN
F 2 "" H 9950 1300 60  0000 C CNN
F 3 "" H 9950 1300 60  0000 C CNN
	1    9950 1300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR06
U 1 1 53F3A188
P 9950 2300
F 0 "#PWR06" H 9950 2260 30  0001 C CNN
F 1 "+3.3V" H 9950 2410 30  0000 C CNN
F 2 "" H 9950 2300 60  0000 C CNN
F 3 "" H 9950 2300 60  0000 C CNN
	1    9950 2300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR07
U 1 1 53F3A19C
P 11750 1250
F 0 "#PWR07" H 11750 1210 30  0001 C CNN
F 1 "+3.3V" H 11750 1360 30  0000 C CNN
F 2 "" H 11750 1250 60  0000 C CNN
F 3 "" H 11750 1250 60  0000 C CNN
	1    11750 1250
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 53F3A864
P 11450 1200
F 0 "R5" V 11530 1200 40  0000 C CNN
F 1 "10K" V 11457 1201 40  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 11380 1200 30  0001 C CNN
F 3 "" H 11450 1200 30  0000 C CNN
	1    11450 1200
	1    0    0    -1  
$EndComp
$Comp
L 6_PIN_1 JP1
U 1 1 53F44018
P 2000 1300
F 0 "JP1" H 2100 1650 60  0000 C CNN
F 1 "6_PIN_1" H 2100 950 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06" H 2000 1300 60  0001 C CNN
F 3 "" H 2000 1300 60  0000 C CNN
	1    2000 1300
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR08
U 1 1 53F44209
P 1150 1100
F 0 "#PWR08" H 1150 1190 20  0001 C CNN
F 1 "+5V" H 1150 1190 30  0000 C CNN
F 2 "" H 1150 1100 60  0000 C CNN
F 3 "" H 1150 1100 60  0000 C CNN
	1    1150 1100
	1    0    0    -1  
$EndComp
NoConn ~ 1650 1550
$Comp
L GND #PWR09
U 1 1 53F442CA
P 1150 1550
F 0 "#PWR09" H 1150 1550 30  0001 C CNN
F 1 "GND" H 1150 1480 30  0001 C CNN
F 2 "" H 1150 1550 60  0000 C CNN
F 3 "" H 1150 1550 60  0000 C CNN
	1    1150 1550
	1    0    0    -1  
$EndComp
$Comp
L 6_PIN_2 JP2
U 1 1 53F443E9
P 2000 2450
F 0 "JP2" H 2100 2800 60  0000 C CNN
F 1 "6_PIN_2" H 2100 2100 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06" H 2000 2450 60  0001 C CNN
F 3 "" H 2000 2450 60  0000 C CNN
	1    2000 2450
	1    0    0    -1  
$EndComp
$Comp
L 8_PIN_1 JP3
U 1 1 53F44977
P 2900 1400
F 0 "JP3" H 3000 1850 60  0000 C CNN
F 1 "8_PIN_1" H 3000 950 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x08" H 2900 1500 60  0001 C CNN
F 3 "" H 2900 1500 60  0000 C CNN
	1    2900 1400
	1    0    0    -1  
$EndComp
$Comp
L 8_PIN_2 JP4
U 1 1 53F449E0
P 2900 2550
F 0 "JP4" H 3000 3000 60  0000 C CNN
F 1 "8_PIN_2" H 3000 2100 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x08" H 2900 2650 60  0001 C CNN
F 3 "" H 2900 2650 60  0000 C CNN
	1    2900 2550
	1    0    0    -1  
$EndComp
Text Label 3600 2300 0    60   ~ 0
MATRIX_DATA
Text Label 8350 2000 0    60   ~ 0
MATRIX_DATA
Text Label 3600 2200 0    60   ~ 0
MATRIX_WR
Text Label 8350 1850 0    60   ~ 0
MATRIX_WR
Text Label 8350 1550 0    60   ~ 0
MATRIX_CS
Text Label 3600 2800 0    60   ~ 0
MATRIX_CS
Text Label 3600 2900 0    60   ~ 0
MATRIX_CLK
Text Label 6400 1550 0    60   ~ 0
MATRIX_CLK
$Comp
L +5V #PWR010
U 1 1 53F45B08
P 6750 2450
F 0 "#PWR010" H 6750 2540 20  0001 C CNN
F 1 "+5V" H 6750 2540 30  0000 C CNN
F 2 "" H 6750 2450 60  0000 C CNN
F 3 "" H 6750 2450 60  0000 C CNN
	1    6750 2450
	1    0    0    -1  
$EndComp
Text Label 3600 1350 0    60   ~ 0
MISO
Text Label 10650 2600 0    60   ~ 0
DATA_IN
Text Label 9400 2600 0    60   ~ 0
MOSI
Text Label 3600 1450 0    60   ~ 0
MOSI
Text Label 1350 2200 0    60   ~ 0
XM
Text Label 1350 2300 0    60   ~ 0
YP
Text Label 1350 2400 0    60   ~ 0
XP
Text Label 1350 2500 0    60   ~ 0
YM
Text Label 4400 2400 0    60   ~ 0
XM
Text Label 4400 2500 0    60   ~ 0
YP
Text Label 4400 2600 0    60   ~ 0
XP
Text Label 4400 2700 0    60   ~ 0
YM
Text Label 15400 1900 0    60   ~ 0
MISO
Text Label 15400 1500 0    60   ~ 0
SCLK
Text Label 10650 1650 0    60   ~ 0
SCLK
Text Label 3600 1250 0    60   ~ 0
SCK
Text Label 9400 1650 0    60   ~ 0
SCK
Text Label 12400 1650 0    60   ~ 0
CS
Text Label 13650 1700 0    60   ~ 0
CS
Text Label 13650 1900 0    60   ~ 0
DATA_IN
Text Label 3550 1550 0    60   ~ 0
SD_CS
Text Label 11150 1650 0    60   ~ 0
SD_CS
$Comp
L +3.3V #PWR011
U 1 1 53F5A65D
P 13700 2100
F 0 "#PWR011" H 13700 2060 30  0001 C CNN
F 1 "+3.3V" H 13700 2210 30  0000 C CNN
F 2 "" H 13700 2100 60  0000 C CNN
F 3 "" H 13700 2100 60  0000 C CNN
	1    13700 2100
	1    0    0    -1  
$EndComp
Text Notes 10800 3250 0    60   ~ 0
Quad-Buffer
$Comp
L MCP4921-E/SN U2
U 1 1 53F6C019
P 9400 4650
F 0 "U2" H 9400 5000 40  0000 L BNN
F 1 "MCP4921-E/SN" H 9400 4950 40  0000 L BNN
F 2 "SMD_Packages:SOIC-8-N" H 9400 4650 30  0000 C CIN
F 3 "" H 9400 4650 60  0000 C CNN
	1    9400 4650
	1    0    0    -1  
$EndComp
Text Label 8400 4700 0    60   ~ 0
DAC_CS
Text Label 3600 2700 0    60   ~ 0
DAC_CS
Text Label 8400 4600 0    60   ~ 0
DAC_SCK
Text Label 3600 2600 0    60   ~ 0
DAC_SCK
Text Label 8400 4500 0    60   ~ 0
DAC_DI
Text Label 3600 2500 0    60   ~ 0
DAC_DI
Text Label 8400 4800 0    60   ~ 0
DAC_LAT
Text Label 3600 2400 0    60   ~ 0
DAC_LAT
$Comp
L GND #PWR012
U 1 1 53F6CA44
P 9300 5050
F 0 "#PWR012" H 9300 5050 30  0001 C CNN
F 1 "GND" H 9300 4980 30  0001 C CNN
F 2 "" H 9300 5050 60  0000 C CNN
F 3 "" H 9300 5050 60  0000 C CNN
	1    9300 5050
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 53F6CADB
P 9750 5250
F 0 "C7" H 9750 5350 40  0000 L CNN
F 1 "0.1uF" H 9756 5165 40  0000 L CNN
F 2 "Capacitors_SMD:c_0603" H 9788 5100 30  0001 C CNN
F 3 "" H 9750 5250 60  0000 C CNN
	1    9750 5250
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR013
U 1 1 53F6CD9C
P 9750 5450
F 0 "#PWR013" H 9750 5450 30  0001 C CNN
F 1 "GND" H 9750 5380 30  0001 C CNN
F 2 "" H 9750 5450 60  0000 C CNN
F 3 "" H 9750 5450 60  0000 C CNN
	1    9750 5450
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 53F6CDB0
P 10000 5050
F 0 "R3" V 10080 5050 40  0000 C CNN
F 1 "100K" V 10007 5051 40  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9930 5050 30  0001 C CNN
F 3 "" H 10000 5050 30  0000 C CNN
	1    10000 5050
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR014
U 1 1 53F6CE02
P 10250 5050
F 0 "#PWR014" H 10250 5140 20  0001 C CNN
F 1 "+5V" H 10250 5140 30  0000 C CNN
F 2 "" H 10250 5050 60  0000 C CNN
F 3 "" H 10250 5050 60  0000 C CNN
	1    10250 5050
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 53F6CE16
P 10350 4650
F 0 "R4" V 10430 4650 40  0000 C CNN
F 1 "1.5K" V 10357 4651 40  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 10280 4650 30  0001 C CNN
F 3 "" H 10350 4650 30  0000 C CNN
	1    10350 4650
	0    1    1    0   
$EndComp
$Comp
L C C8
U 1 1 53F6CF04
P 10600 5250
F 0 "C8" H 10600 5350 40  0000 L CNN
F 1 "0.01uF" H 10606 5165 40  0000 L CNN
F 2 "Capacitors_SMD:c_0603" H 10638 5100 30  0001 C CNN
F 3 "" H 10600 5250 60  0000 C CNN
	1    10600 5250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 53F6D14D
P 10600 5450
F 0 "#PWR015" H 10600 5450 30  0001 C CNN
F 1 "GND" H 10600 5380 30  0001 C CNN
F 2 "" H 10600 5450 60  0000 C CNN
F 3 "" H 10600 5450 60  0000 C CNN
	1    10600 5450
	1    0    0    -1  
$EndComp
$Comp
L TL072 U4
U 1 1 53F6DDA6
P 12600 5000
F 0 "U4" H 12550 5200 60  0000 L CNN
F 1 "TL072" H 12550 4750 60  0000 L CNN
F 2 "SMD_Packages:SOIC-14_N" H 12600 5000 60  0001 C CNN
F 3 "" H 12600 5000 60  0000 C CNN
	1    12600 5000
	1    0    0    -1  
$EndComp
$Comp
L TL072 U4
U 2 1 53F6DDFF
P 14100 5000
F 0 "U4" H 14050 5200 60  0000 L CNN
F 1 "TL072" H 14050 4750 60  0000 L CNN
F 2 "SMD_Packages:SOIC-14_N" H 14100 5000 60  0001 C CNN
F 3 "" H 14100 5000 60  0000 C CNN
	2    14100 5000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 53F6E130
P 14000 5550
F 0 "#PWR016" H 14000 5550 30  0001 C CNN
F 1 "GND" H 14000 5480 30  0001 C CNN
F 2 "" H 14000 5550 60  0000 C CNN
F 3 "" H 14000 5550 60  0000 C CNN
	1    14000 5550
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 53F6E1A7
P 15100 5000
F 0 "C9" H 15100 5100 40  0000 L CNN
F 1 "100uF/6V" H 15106 4915 40  0000 L CNN
F 2 "Capacitors_Tantalum_SMD:TantalC_SizeC_EIA-6032_Reflow" H 15138 4850 30  0001 C CNN
F 3 "" H 15100 5000 60  0000 C CNN
	1    15100 5000
	0    -1   -1   0   
$EndComp
$Comp
L CONN_2 JP6
U 1 1 53F6E48F
P 15550 5500
F 0 "JP6" V 15500 5500 40  0000 C CNN
F 1 "SPKR" V 15600 5500 40  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02" H 15550 5500 60  0001 C CNN
F 3 "" H 15550 5500 60  0000 C CNN
	1    15550 5500
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR017
U 1 1 53F6E684
P 15900 5750
F 0 "#PWR017" H 15900 5750 30  0001 C CNN
F 1 "GND" H 15900 5680 30  0001 C CNN
F 2 "" H 15900 5750 60  0000 C CNN
F 3 "" H 15900 5750 60  0000 C CNN
	1    15900 5750
	1    0    0    -1  
$EndComp
$Comp
L APE8865N-33-HF-3 U1
U 1 1 53F6EC9F
P 4200 4850
F 0 "U1" H 3900 5100 40  0000 C CNN
F 1 "MCP1700T-3302E/MB" H 4200 5050 40  0000 C CNN
F 2 "Housings_SOT-89:SOT89-3_Housing" H 4200 5200 35  0001 C CIN
F 3 "" H 4200 4850 60  0000 C CNN
	1    4200 4850
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR018
U 1 1 53F6EE99
P 3550 4800
F 0 "#PWR018" H 3550 4890 20  0001 C CNN
F 1 "+5V" H 3550 4890 30  0000 C CNN
F 2 "" H 3550 4800 60  0000 C CNN
F 3 "" H 3550 4800 60  0000 C CNN
	1    3550 4800
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 53F6EFF0
P 5400 5000
F 0 "C6" H 5400 5100 40  0000 L CNN
F 1 "100uF/6V" H 5406 4915 40  0000 L CNN
F 2 "Capacitors_Tantalum_SMD:TantalC_SizeC_EIA-6032_Reflow" H 5438 4850 30  0001 C CNN
F 3 "" H 5400 5000 60  0000 C CNN
	1    5400 5000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR019
U 1 1 53F6F180
P 4200 5400
F 0 "#PWR019" H 4200 5400 30  0001 C CNN
F 1 "GND" H 4200 5330 30  0001 C CNN
F 2 "" H 4200 5400 60  0000 C CNN
F 3 "" H 4200 5400 60  0000 C CNN
	1    4200 5400
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 53F6F1E5
P 4650 5000
F 0 "C5" H 4650 5100 40  0000 L CNN
F 1 "0.1uF" H 4656 4915 40  0000 L CNN
F 2 "Capacitors_SMD:c_0603" H 4688 4850 30  0001 C CNN
F 3 "" H 4650 5000 60  0000 C CNN
	1    4650 5000
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR020
U 1 1 53F6F2EF
P 5050 4800
F 0 "#PWR020" H 5050 4760 30  0001 C CNN
F 1 "+3.3V" H 5050 4910 30  0000 C CNN
F 2 "" H 5050 4800 60  0000 C CNN
F 3 "" H 5050 4800 60  0000 C CNN
	1    5050 4800
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 53F6F305
P 2500 5000
F 0 "C2" H 2500 5100 40  0000 L CNN
F 1 "0.1uF" H 2506 4915 40  0000 L CNN
F 2 "Capacitors_SMD:c_0603" H 2538 4850 30  0001 C CNN
F 3 "" H 2500 5000 60  0000 C CNN
	1    2500 5000
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 53F6F384
P 2800 5000
F 0 "C3" H 2800 5100 40  0000 L CNN
F 1 "0.1uF" H 2806 4915 40  0000 L CNN
F 2 "Capacitors_SMD:c_0603" H 2838 4850 30  0001 C CNN
F 3 "" H 2800 5000 60  0000 C CNN
	1    2800 5000
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 53F6F42A
P 3100 5000
F 0 "C4" H 3100 5100 40  0000 L CNN
F 1 "100uF/6V" H 3106 4915 40  0000 L CNN
F 2 "Capacitors_Tantalum_SMD:TantalC_SizeC_EIA-6032_Reflow" H 3138 4850 30  0001 C CNN
F 3 "" H 3100 5000 60  0000 C CNN
	1    3100 5000
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 53F6F4A8
P 2200 5000
F 0 "C1" H 2200 5100 40  0000 L CNN
F 1 "0.1uF" H 2206 4915 40  0000 L CNN
F 2 "Capacitors_SMD:c_0603" H 2238 4850 30  0001 C CNN
F 3 "" H 2200 5000 60  0000 C CNN
	1    2200 5000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR021
U 1 1 53F6F8C4
P 3100 5300
F 0 "#PWR021" H 3100 5300 30  0001 C CNN
F 1 "GND" H 3100 5230 30  0001 C CNN
F 2 "" H 3100 5300 60  0000 C CNN
F 3 "" H 3100 5300 60  0000 C CNN
	1    3100 5300
	1    0    0    -1  
$EndComp
NoConn ~ 1650 1150
$Comp
L GND #PWR022
U 1 1 53F7124A
P 2200 5300
F 0 "#PWR022" H 2200 5300 30  0001 C CNN
F 1 "GND" H 2200 5230 30  0001 C CNN
F 2 "" H 2200 5300 60  0000 C CNN
F 3 "" H 2200 5300 60  0000 C CNN
	1    2200 5300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR023
U 1 1 53F7125E
P 2500 5300
F 0 "#PWR023" H 2500 5300 30  0001 C CNN
F 1 "GND" H 2500 5230 30  0001 C CNN
F 2 "" H 2500 5300 60  0000 C CNN
F 3 "" H 2500 5300 60  0000 C CNN
	1    2500 5300
	1    0    0    -1  
$EndComp
Text Notes 3150 5800 0    60   ~ 0
3.3V POWER SUPLY & BYPASS
Text Notes 9250 5800 0    60   ~ 0
DAC
Text Notes 12900 5800 0    60   ~ 0
AMP
Text Notes 11050 5800 0    60   ~ 0
(VOLUME REMOVED)
$Comp
L DS1307 IC1
U 1 1 53F71D16
P 3650 7650
F 0 "IC1" H 3650 7450 60  0000 C CNN
F 1 "DS1307" H 3650 7900 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_N" H 3650 7650 60  0001 C CNN
F 3 "" H 3650 7650 60  0000 C CNN
	1    3650 7650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 2700 4650 2700
Wire Wire Line
	4400 2500 4650 2500
Wire Wire Line
	-766900 -630300 -767100 -630300
Wire Wire Line
	-767100 -630300 -767100 -630200
Wire Wire Line
	-766900 -630100 -766900 -629900
Connection ~ -766900 -630000
Wire Wire Line
	-765850 -630300 -766100 -630300
Wire Wire Line
	-765850 -630600 -766100 -630600
Wire Wire Line
	-766100 -630100 -766100 -629900
Connection ~ -766100 -630000
Wire Wire Line
	-765750 -629950 -765750 -630000
Wire Wire Line
	-765750 -630000 -766100 -630000
Wire Wire Line
	7000 2000 6750 2000
Wire Wire Line
	6750 2000 6750 2150
Wire Wire Line
	7000 2300 7000 2600
Connection ~ 7000 2450
Wire Wire Line
	8250 2300 8250 2600
Connection ~ 8250 2450
Wire Wire Line
	8250 2450 8650 2450
Wire Wire Line
	8650 2450 8650 2600
Wire Wire Line
	15400 1700 15850 1700
Wire Wire Line
	15850 1700 15850 2600
Wire Wire Line
	15400 2300 15850 2300
Connection ~ 15850 2300
Wire Wire Line
	15400 2500 15850 2500
Connection ~ 15850 2500
Wire Wire Line
	14000 2300 13850 2300
Wire Wire Line
	13850 2300 13850 2600
Wire Wire Line
	14000 2500 13850 2500
Connection ~ 13850 2500
Connection ~ 11450 1650
Wire Wire Line
	1650 1450 1150 1450
Wire Wire Line
	1150 1350 1150 1550
Wire Wire Line
	1650 1350 1150 1350
Connection ~ 1150 1450
Wire Wire Line
	1350 2300 1650 2300
Wire Wire Line
	1350 2500 1650 2500
Wire Wire Line
	3450 2300 3800 2300
Wire Wire Line
	8250 2000 8750 2000
Wire Wire Line
	3450 2200 3800 2200
Wire Wire Line
	8250 1850 8750 1850
Wire Wire Line
	8250 1550 8750 1550
Wire Wire Line
	3450 2800 3800 2800
Wire Wire Line
	3450 2900 3800 2900
Wire Wire Line
	7000 2450 6750 2450
Wire Wire Line
	3450 1350 3800 1350
Wire Wire Line
	10550 2600 10900 2600
Wire Wire Line
	9650 2600 9400 2600
Wire Wire Line
	3450 1450 3800 1450
Wire Wire Line
	1650 2200 1350 2200
Wire Wire Line
	1650 2400 1350 2400
Wire Wire Line
	4650 2400 4400 2400
Wire Wire Line
	4650 2600 4400 2600
Wire Wire Line
	14000 1900 13650 1900
Wire Wire Line
	15400 1900 15700 1900
Wire Wire Line
	15400 1500 15700 1500
Wire Wire Line
	10550 1650 10900 1650
Wire Wire Line
	3450 1250 3800 1250
Wire Wire Line
	9650 1650 9400 1650
Wire Wire Line
	12350 1650 12650 1650
Wire Wire Line
	14000 1700 13650 1700
Wire Wire Line
	3450 1550 3800 1550
Wire Wire Line
	11450 1650 11150 1650
Wire Wire Line
	13700 2100 14000 2100
Wire Wire Line
	8850 4700 8400 4700
Wire Wire Line
	3450 2700 3800 2700
Wire Wire Line
	8850 4600 8400 4600
Wire Wire Line
	3450 2600 3800 2600
Wire Wire Line
	8850 4500 8400 4500
Wire Wire Line
	3450 2500 3800 2500
Wire Wire Line
	8850 4800 8400 4800
Wire Wire Line
	3450 2400 3800 2400
Wire Wire Line
	9500 5050 9750 5050
Connection ~ 9750 5050
Wire Wire Line
	9950 4650 10100 4650
Wire Wire Line
	10600 4650 10600 5050
Wire Wire Line
	10600 4650 11100 4650
Wire Wire Line
	13600 5100 13600 5650
Wire Wire Line
	13600 5650 14600 5650
Wire Wire Line
	14600 5650 14600 5000
Wire Wire Line
	13100 5000 13100 5550
Wire Wire Line
	13100 5550 13600 5550
Connection ~ 13600 5550
Wire Wire Line
	12100 5100 12100 5500
Wire Wire Line
	12100 5500 13100 5500
Connection ~ 13100 5500
Wire Wire Line
	11100 4900 12100 4900
Connection ~ 12100 4900
Wire Wire Line
	14600 5000 14900 5000
Wire Wire Line
	15900 5000 15900 5400
Wire Wire Line
	15900 5600 15900 5750
Wire Wire Line
	12100 4900 12100 4500
Wire Wire Line
	12100 4500 13600 4500
Wire Wire Line
	13600 4500 13600 4900
Wire Wire Line
	15300 5000 15900 5000
Wire Wire Line
	3750 4800 3550 4800
Wire Wire Line
	4650 4800 5400 4800
Connection ~ 4650 4800
Connection ~ 5050 4800
Wire Wire Line
	2800 4800 3100 4800
Wire Wire Line
	2800 5200 3100 5200
Wire Wire Line
	3100 5200 3100 5300
Connection ~ 3100 5200
Wire Wire Line
	2500 5200 2500 5300
Wire Wire Line
	2200 5200 2200 5300
$Comp
L BATTERY BT1
U 1 1 53F7C57F
P 1650 7700
F 0 "BT1" H 1650 7900 50  0000 C CNN
F 1 "BATTERY" H 1650 7510 50  0000 C CNN
F 2 "tapmatrix:CR2032" H 1650 7700 60  0001 C CNN
F 3 "" H 1650 7700 60  0000 C CNN
	1    1650 7700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR024
U 1 1 53F7C63E
P 2450 8000
F 0 "#PWR024" H 2450 8000 30  0001 C CNN
F 1 "GND" H 2450 7930 30  0001 C CNN
F 2 "" H 2450 8000 60  0000 C CNN
F 3 "" H 2450 8000 60  0000 C CNN
	1    2450 8000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 7550 2900 7550
Wire Wire Line
	1350 7400 1350 7700
NoConn ~ 4450 7400
NoConn ~ 2900 7250
NoConn ~ 2900 7400
$Comp
L R R1
U 1 1 53F81CD9
P 4850 7150
F 0 "R1" V 4930 7150 40  0000 C CNN
F 1 "R" V 4857 7151 40  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4780 7150 30  0001 C CNN
F 3 "" H 4850 7150 30  0000 C CNN
	1    4850 7150
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 53F81E1C
P 5500 7150
F 0 "R2" V 5580 7150 40  0000 C CNN
F 1 "R" V 5507 7151 40  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5430 7150 30  0001 C CNN
F 3 "" H 5500 7150 30  0000 C CNN
	1    5500 7150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 6700 2450 7550
Wire Wire Line
	2450 6700 5500 6700
Wire Wire Line
	4850 6900 4850 6700
Connection ~ 4850 6700
Wire Wire Line
	4450 7250 4450 6700
Connection ~ 4450 6700
Wire Wire Line
	4850 7400 4850 7550
Wire Wire Line
	4450 7550 5150 7550
Connection ~ 4850 7550
Text Label 4950 7550 0    60   ~ 0
I2C_SCL
Wire Wire Line
	5500 6700 5500 6900
Wire Wire Line
	4450 7700 5850 7700
Wire Wire Line
	5500 7700 5500 7400
Connection ~ 5500 7700
Text Label 5600 7700 0    60   ~ 0
I2C_SDA
Text Label 1200 2700 0    60   ~ 0
I2C_SCL
Text Label 1200 2600 0    60   ~ 0
I2C_SDA
Text Notes 3350 8300 0    60   ~ 0
REAL TIME CLOCK
Connection ~ 3550 4800
Connection ~ 14600 5000
Connection ~ 7000 1550
Connection ~ 13100 5000
Wire Wire Line
	1350 7400 2450 7400
Connection ~ 2450 7400
Connection ~ 3450 2900
Connection ~ 3450 2800
Connection ~ 3450 2700
Connection ~ 3450 2600
Connection ~ 3450 2500
Connection ~ 3450 2400
Connection ~ 3450 2300
Connection ~ 3450 2200
Wire Wire Line
	6400 1550 7000 1550
NoConn ~ 1650 1050
$Comp
L +5V #PWR025
U 1 1 53F94A7D
P 2200 4550
F 0 "#PWR025" H 2200 4640 20  0001 C CNN
F 1 "+5V" H 2200 4640 30  0000 C CNN
F 2 "" H 2200 4550 60  0000 C CNN
F 3 "" H 2200 4550 60  0000 C CNN
	1    2200 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 4550 2200 4800
$Comp
L +5V #PWR026
U 1 1 53F94B8C
P 2500 4550
F 0 "#PWR026" H 2500 4640 20  0001 C CNN
F 1 "+5V" H 2500 4640 30  0000 C CNN
F 2 "" H 2500 4550 60  0000 C CNN
F 3 "" H 2500 4550 60  0000 C CNN
	1    2500 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 4550 2500 4800
$Comp
L +5V #PWR027
U 1 1 53F94C7A
P 3100 4550
F 0 "#PWR027" H 3100 4640 20  0001 C CNN
F 1 "+5V" H 3100 4640 30  0000 C CNN
F 2 "" H 3100 4550 60  0000 C CNN
F 3 "" H 3100 4550 60  0000 C CNN
	1    3100 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 4800 3100 4550
Connection ~ 3100 4800
NoConn ~ 3450 1650
NoConn ~ 3450 1750
$Comp
L +5V #PWR028
U 1 1 53F96D63
P 9300 4000
F 0 "#PWR028" H 9300 4090 20  0001 C CNN
F 1 "+5V" H 9300 4090 30  0000 C CNN
F 2 "" H 9300 4000 60  0000 C CNN
F 3 "" H 9300 4000 60  0000 C CNN
	1    9300 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 4000 9300 4250
Wire Wire Line
	1200 2600 1650 2600
Wire Wire Line
	1200 2700 1650 2700
NoConn ~ 3450 1050
$Comp
L GND #PWR029
U 1 1 53F9875B
P 4050 1250
F 0 "#PWR029" H 4050 1250 30  0001 C CNN
F 1 "GND" H 4050 1180 30  0001 C CNN
F 2 "" H 4050 1250 60  0000 C CNN
F 3 "" H 4050 1250 60  0000 C CNN
	1    4050 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 1150 4050 1150
Wire Wire Line
	4050 1150 4050 1250
Wire Wire Line
	9950 1300 9950 1500
$Comp
L GND #PWR030
U 1 1 53F9AF0C
P 10100 2100
F 0 "#PWR030" H 10100 2100 30  0001 C CNN
F 1 "GND" H 10100 2030 30  0001 C CNN
F 2 "" H 10100 2100 60  0000 C CNN
F 3 "" H 10100 2100 60  0000 C CNN
	1    10100 2100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR031
U 1 1 53F9B0AC
P 10100 3050
F 0 "#PWR031" H 10100 3050 30  0001 C CNN
F 1 "GND" H 10100 2980 30  0001 C CNN
F 2 "" H 10100 3050 60  0000 C CNN
F 3 "" H 10100 3050 60  0000 C CNN
	1    10100 3050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR032
U 1 1 53F9B165
P 11900 2100
F 0 "#PWR032" H 11900 2100 30  0001 C CNN
F 1 "GND" H 11900 2030 30  0001 C CNN
F 2 "" H 11900 2100 60  0000 C CNN
F 3 "" H 11900 2100 60  0000 C CNN
	1    11900 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	11900 1950 11900 2100
Wire Wire Line
	10100 1950 10100 2100
Wire Wire Line
	10100 2900 10100 3050
Wire Wire Line
	11450 1450 11450 1650
$Comp
L +5V #PWR033
U 1 1 53F9B69B
P 11450 800
F 0 "#PWR033" H 11450 890 20  0001 C CNN
F 1 "+5V" H 11450 890 30  0000 C CNN
F 2 "" H 11450 800 60  0000 C CNN
F 3 "" H 11450 800 60  0000 C CNN
	1    11450 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	11450 800  11450 950 
Connection ~ 9300 4250
Connection ~ 2500 4550
Wire Wire Line
	14000 5400 14000 5550
Wire Wire Line
	11750 1250 11750 1500
Wire Wire Line
	9950 2300 9950 2450
$Comp
L +3.3V #PWR034
U 1 1 53F9C9E7
P 11750 2250
F 0 "#PWR034" H 11750 2210 30  0001 C CNN
F 1 "+3.3V" H 11750 2360 30  0000 C CNN
F 2 "" H 11750 2250 60  0000 C CNN
F 3 "" H 11750 2250 60  0000 C CNN
	1    11750 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	11750 2250 11750 2450
$Comp
L GND #PWR035
U 1 1 53F9CBA6
P 11750 3050
F 0 "#PWR035" H 11750 3050 30  0001 C CNN
F 1 "GND" H 11750 2980 30  0001 C CNN
F 2 "" H 11750 3050 60  0000 C CNN
F 3 "" H 11750 3050 60  0000 C CNN
	1    11750 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 5300 5400 5200
Wire Wire Line
	4200 5300 5400 5300
Wire Wire Line
	4200 5150 4200 5400
Connection ~ 4200 5300
Wire Wire Line
	4650 5200 4650 5300
Connection ~ 4650 5300
Wire Wire Line
	1150 1100 1150 1250
Wire Wire Line
	1150 1250 1650 1250
$Comp
L +5V #PWR036
U 1 1 53F9E6C1
P 14000 4350
F 0 "#PWR036" H 14000 4440 20  0001 C CNN
F 1 "+5V" H 14000 4440 30  0000 C CNN
F 2 "" H 14000 4350 60  0000 C CNN
F 3 "" H 14000 4350 60  0000 C CNN
	1    14000 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	14000 4350 14000 4600
Wire Wire Line
	1950 7700 2450 7700
Wire Wire Line
	2450 7700 2450 8000
Wire Wire Line
	2900 7700 2900 7850
Wire Wire Line
	2900 7850 2450 7850
Connection ~ 2450 7850
Wire Wire Line
	11750 2750 11750 3050
Connection ~ 10600 4650
Wire Wire Line
	11100 4650 11100 4900
$Comp
L 4_PIN_1 JP5
U 1 1 5407DC2F
P 6250 2550
F 0 "JP5" H 6250 2850 60  0000 C CNN
F 1 "4_PIN_1" V 6350 2550 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04" H 6250 2550 60  0001 C CNN
F 3 "" H 6250 2550 60  0000 C CNN
	1    6250 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 2400 5500 2400
Wire Wire Line
	5800 2500 5500 2500
Wire Wire Line
	5800 2600 5500 2600
Wire Wire Line
	5800 2700 5500 2700
Text Label 5500 2400 0    60   ~ 0
XM
Text Label 5500 2500 0    60   ~ 0
YP
Text Label 5500 2600 0    60   ~ 0
XP
Text Label 5500 2700 0    60   ~ 0
YM
$EndSCHEMATC
