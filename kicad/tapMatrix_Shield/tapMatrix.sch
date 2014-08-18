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
P 4500 1350
F 0 "P?" V 4450 1350 50  0000 C CNN
F 1 "TS_ZIF" V 4550 1350 50  0000 C CNN
F 2 "" H 4500 1350 60  0000 C CNN
F 3 "" H 4500 1350 60  0000 C CNN
	1    4500 1350
	1    0    0    -1  
$EndComp
Text GLabel 4150 1200 0    60   Input ~ 0
A0
Text GLabel 3900 1300 0    60   Input ~ 0
A1
Text GLabel 4150 1400 0    60   Input ~ 0
A2
Text GLabel 3900 1500 0    60   Input ~ 0
A3
Wire Wire Line
	3900 1500 4150 1500
Wire Wire Line
	3900 1300 4150 1300
$EndSCHEMATC
