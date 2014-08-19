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
Date "17 Aug 2014"
Rev "0.1"
Comp "Responsive Devices, Inc."
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CONN_6 P?
U 1 1 53F232C2
P 1850 1450
F 0 "P?" V 1800 1450 60  0000 C CNN
F 1 "90120-0766" V 1900 1450 60  0000 C CNN
F 2 "" H 1850 1450 60  0000 C CNN
F 3 "" H 1850 1450 60  0000 C CNN
	1    1850 1450
	1    0    0    -1  
$EndComp
Text GLabel 1500 1200 0    60   Input ~ 0
RESET
Text GLabel 1100 1300 0    60   Input ~ 0
3.3V
Text GLabel 1500 1400 0    60   Input ~ 0
5V
Text GLabel 1500 1700 0    60   Input ~ 0
V_IN
Wire Wire Line
	1100 1300 1500 1300
$Comp
L GND #PWR?
U 1 1 53F233FB
P 900 1700
F 0 "#PWR?" H 900 1700 30  0001 C CNN
F 1 "GND" H 900 1630 30  0001 C CNN
F 2 "" H 900 1700 60  0000 C CNN
F 3 "" H 900 1700 60  0000 C CNN
	1    900  1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 1500 900  1500
Wire Wire Line
	900  1500 900  1700
Wire Wire Line
	1500 1600 900  1600
Connection ~ 900  1600
$Comp
L CONN_6 P?
U 1 1 53F2345E
P 1850 2450
F 0 "P?" V 1800 2450 60  0000 C CNN
F 1 "90120-0766" V 1900 2450 60  0000 C CNN
F 2 "" H 1850 2450 60  0000 C CNN
F 3 "" H 1850 2450 60  0000 C CNN
	1    1850 2450
	1    0    0    -1  
$EndComp
Text GLabel 1500 2200 0    60   Input ~ 0
A0
Text GLabel 1250 2300 0    60   Input ~ 0
A1
Text GLabel 1500 2400 0    60   Input ~ 0
A2
Text GLabel 1250 2500 0    60   Input ~ 0
A3
Text GLabel 1500 2600 0    60   Input ~ 0
A4
Text GLabel 1250 2700 0    60   Input ~ 0
A5
Wire Wire Line
	1500 2300 1250 2300
Wire Wire Line
	1500 2500 1250 2500
Wire Wire Line
	1500 2700 1250 2700
$Comp
L CONN_8 P?
U 1 1 53F2358E
P 2400 1550
F 0 "P?" V 2350 1550 60  0000 C CNN
F 1 "90120-0768" V 2450 1550 60  0000 C CNN
F 2 "" H 2400 1550 60  0000 C CNN
F 3 "" H 2400 1550 60  0000 C CNN
	1    2400 1550
	-1   0    0    -1  
$EndComp
Text GLabel 2750 1200 2    60   Input ~ 0
AREF
Text GLabel 2750 1400 2    60   Input ~ 0
D13
Text GLabel 3050 1500 2    60   Input ~ 0
D12
Text GLabel 2750 1600 2    60   Input ~ 0
D11
Text GLabel 3050 1700 2    60   Input ~ 0
D10
Text GLabel 2750 1800 2    60   Input ~ 0
D9
Text GLabel 3050 1900 2    60   Input ~ 0
D8
Wire Wire Line
	3050 1500 2750 1500
Wire Wire Line
	3050 1700 2750 1700
Wire Wire Line
	3050 1900 2750 1900
$Comp
L GND #PWR?
U 1 1 53F2376F
P 3200 1350
F 0 "#PWR?" H 3200 1350 30  0001 C CNN
F 1 "GND" H 3200 1280 30  0001 C CNN
F 2 "" H 3200 1350 60  0000 C CNN
F 3 "" H 3200 1350 60  0000 C CNN
	1    3200 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 1300 3200 1300
Wire Wire Line
	3200 1300 3200 1350
$Comp
L CONN_8 P?
U 1 1 53F2380A
P 2400 2550
F 0 "P?" V 2350 2550 60  0000 C CNN
F 1 "90120-0768" V 2450 2550 60  0000 C CNN
F 2 "" H 2400 2550 60  0000 C CNN
F 3 "" H 2400 2550 60  0000 C CNN
	1    2400 2550
	-1   0    0    -1  
$EndComp
Text GLabel 2750 2200 2    60   Input ~ 0
D7
Text GLabel 3050 2300 2    60   Input ~ 0
D6
Text GLabel 2750 2400 2    60   Input ~ 0
D5
Text GLabel 3050 2500 2    60   Input ~ 0
D4
Text GLabel 2750 2600 2    60   Input ~ 0
D3
Text GLabel 3050 2700 2    60   Input ~ 0
D2
Text GLabel 2750 2800 2    60   Input ~ 0
D1
Text GLabel 3050 2900 2    60   Input ~ 0
D0
Wire Wire Line
	3050 2300 2750 2300
Wire Wire Line
	3050 2500 2750 2500
Wire Wire Line
	3050 2700 2750 2700
Wire Wire Line
	3050 2900 2750 2900
Text Notes 2550 3250 2    60   ~ 0
Arduino UNO Headers
$Comp
L TS_ZIF P?
U 1 1 53F23B9F
P 5300 2550
F 0 "P?" H 5350 2850 50  0000 C CNN
F 1 "TS_ZIF" V 5500 2550 50  0000 C CNN
F 2 "" H 5300 2550 60  0000 C CNN
F 3 "" H 5300 2550 60  0000 C CNN
	1    5300 2550
	1    0    0    -1  
$EndComp
Text GLabel 4950 2400 0    60   Input ~ 0
A0
Text GLabel 4700 2500 0    60   Input ~ 0
A1
Text GLabel 4950 2600 0    60   Input ~ 0
A2
Text GLabel 4700 2700 0    60   Input ~ 0
A3
Wire Wire Line
	4700 2700 4950 2700
Wire Wire Line
	4700 2500 4950 2500
$Comp
L LED_MATRIX_HDR P?
U 1 1 53F2DDF1
P 7650 2100
F 0 "P?" H 7650 2850 60  0000 C CNN
F 1 "LED_MATRIX_HDR" H 7650 1400 50  0000 C CNN
F 2 "" H 7650 2100 60  0000 C CNN
F 3 "" H 7650 2100 60  0000 C CNN
	1    7650 2100
	1    0    0    -1  
$EndComp
Text GLabel 7000 1550 0    60   Input ~ 0
D0
NoConn ~ 7000 1700
NoConn ~ 7000 1850
NoConn ~ 7000 2150
NoConn ~ 8250 2150
NoConn ~ 8250 1700
$Comp
L GND #PWR?
U 1 1 53F2DE59
P 6750 2150
F 0 "#PWR?" H 6750 2150 30  0001 C CNN
F 1 "GND" H 6750 2080 30  0001 C CNN
F 2 "" H 6750 2150 60  0000 C CNN
F 3 "" H 6750 2150 60  0000 C CNN
	1    6750 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	-69150 -60200 -69350 -60200
Wire Wire Line
	-69350 -60200 -69350 -60100
Wire Wire Line
	-69150 -60000 -69150 -59800
Connection ~ -69150 -59900
Text GLabel 7000 2450 0    60   Input ~ 0
5V
Text GLabel 8550 1550 2    60   Input ~ 0
D1
Text GLabel 8250 2000 2    60   Input ~ 0
D6
Text GLabel 8550 1850 2    60   Input ~ 0
D7
Wire Wire Line
	-68100 -60200 -68350 -60200
Wire Wire Line
	-68100 -60500 -68350 -60500
Wire Wire Line
	-68350 -60000 -68350 -59800
Connection ~ -68350 -59900
$Comp
L GND #PWR?
U 1 1 53F2E04E
P 8650 2600
F 0 "#PWR?" H 8650 2600 30  0001 C CNN
F 1 "GND" H 8650 2530 30  0001 C CNN
F 2 "" H 8650 2600 60  0000 C CNN
F 3 "" H 8650 2600 60  0000 C CNN
	1    8650 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	-68000 -59850 -68000 -59900
Wire Wire Line
	-68000 -59900 -68350 -59900
Text Notes 5500 3250 2    60   ~ 0
Touchscreen ZIF Socket
Text Notes 8000 3250 2    60   ~ 0
LED Matrix Header
$Comp
L SD_CARD U?
U 1 1 53F2E748
P 10650 2050
F 0 "U?" H 10700 2750 60  0000 C CNN
F 1 "SD_CARD" H 10650 1350 60  0000 C CNN
F 2 "" H 10650 2250 60  0000 C CNN
F 3 "" H 10650 2250 60  0000 C CNN
	1    10650 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 2000 6750 2000
Wire Wire Line
	6750 2000 6750 2150
Wire Wire Line
	7000 2300 7000 2600
Connection ~ 7000 2450
Wire Wire Line
	8550 1850 8250 1850
Wire Wire Line
	8550 1550 8250 1550
Wire Wire Line
	8250 2300 8250 2600
Connection ~ 8250 2450
Wire Wire Line
	8250 2450 8650 2450
Wire Wire Line
	8650 2450 8650 2600
Text Notes 11050 3250 2    60   ~ 0
MicroSD Card Socket
NoConn ~ 9950 1550
Text GLabel 9950 1750 0    60   Input ~ 0
CS
Text GLabel 9950 1950 0    60   Input ~ 0
MOSI
Text GLabel 9950 2150 0    60   Input ~ 0
3.3V
Text GLabel 11350 1550 2    60   Input ~ 0
SCLK
$Comp
L GND #PWR?
U 1 1 53F3050A
P 11800 2650
F 0 "#PWR?" H 11800 2650 30  0001 C CNN
F 1 "GND" H 11800 2580 30  0001 C CNN
F 2 "" H 11800 2650 60  0000 C CNN
F 3 "" H 11800 2650 60  0000 C CNN
	1    11800 2650
	1    0    0    -1  
$EndComp
Text GLabel 11350 1950 2    60   Input ~ 0
D12
NoConn ~ 11350 2150
Wire Wire Line
	11350 1750 11800 1750
Wire Wire Line
	11800 1750 11800 2650
Wire Wire Line
	11350 2350 11800 2350
Connection ~ 11800 2350
Wire Wire Line
	11350 2550 11800 2550
Connection ~ 11800 2550
$Comp
L GND #PWR?
U 1 1 53F30748
P 9800 2650
F 0 "#PWR?" H 9800 2650 30  0001 C CNN
F 1 "GND" H 9800 2580 30  0001 C CNN
F 2 "" H 9800 2650 60  0000 C CNN
F 3 "" H 9800 2650 60  0000 C CNN
	1    9800 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9950 2350 9800 2350
Wire Wire Line
	9800 2350 9800 2650
Wire Wire Line
	9950 2550 9800 2550
Connection ~ 9800 2550
$EndSCHEMATC
