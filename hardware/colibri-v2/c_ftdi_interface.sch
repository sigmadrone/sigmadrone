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
LIBS:sigmadrone
LIBS:colibri-wnd-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 28
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L FT232RL U4
U 1 1 558F3BD7
P 7250 3800
F 0 "U4" H 7600 4850 60  0000 C CNN
F 1 "FT232RL" H 7000 2750 60  0000 C CNN
F 2 "" H 7250 3800 60  0001 C CNN
F 3 "" H 7250 3800 60  0000 C CNN
	1    7250 3800
	1    0    0    -1  
$EndComp
$Comp
L USB_B P4
U 1 1 558F3BFF
P 2000 3200
F 0 "P4" H 2200 3000 50  0000 C CNN
F 1 "USB_B" H 1950 3400 50  0000 C CNN
F 2 "" V 1950 3100 60  0001 C CNN
F 3 "" V 1950 3100 60  0000 C CNN
	1    2000 3200
	0    -1   1    0   
$EndComp
Wire Wire Line
	2300 3000 3550 3000
Wire Wire Line
	3950 3000 6550 3000
Wire Wire Line
	2300 3100 6550 3100
Wire Wire Line
	2300 3200 6550 3200
$Comp
L GND #PWR033
U 1 1 558F7353
P 6350 5000
F 0 "#PWR033" H 6350 4750 50  0001 C CNN
F 1 "GND" H 6350 4850 50  0000 C CNN
F 2 "" H 6350 5000 60  0000 C CNN
F 3 "" H 6350 5000 60  0000 C CNN
	1    6350 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 4300 6350 4300
Wire Wire Line
	6350 4300 6350 5000
Wire Wire Line
	6550 4400 6350 4400
Connection ~ 6350 4400
Wire Wire Line
	6350 4500 6550 4500
Connection ~ 6350 4500
Wire Wire Line
	6550 4600 6350 4600
Connection ~ 6350 4600
Wire Wire Line
	6350 4700 6550 4700
Connection ~ 6350 4700
Wire Wire Line
	6550 4200 5950 4200
Wire Wire Line
	5950 4200 5950 4450
$Comp
L GND #PWR034
U 1 1 558F73F8
P 5950 5000
F 0 "#PWR034" H 5950 4750 50  0001 C CNN
F 1 "GND" H 5950 4850 50  0000 C CNN
F 2 "" H 5950 5000 60  0000 C CNN
F 3 "" H 5950 5000 60  0000 C CNN
	1    5950 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 4750 5950 5000
$Comp
L +3V3 #PWR035
U 1 1 558F7465
P 6350 2000
F 0 "#PWR035" H 6350 1850 50  0001 C CNN
F 1 "+3V3" H 6350 2140 50  0000 C CNN
F 2 "" H 6350 2000 60  0000 C CNN
F 3 "" H 6350 2000 60  0000 C CNN
	1    6350 2000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR036
U 1 1 558F759A
P 5950 2000
F 0 "#PWR036" H 5950 1750 50  0001 C CNN
F 1 "GND" H 5950 1850 50  0000 C CNN
F 2 "" H 5950 2000 60  0000 C CNN
F 3 "" H 5950 2000 60  0000 C CNN
	1    5950 2000
	-1   0    0    1   
$EndComp
Wire Wire Line
	5950 2000 5950 2250
Wire Wire Line
	5950 2550 5950 2900
$Comp
L GND #PWR037
U 1 1 558F7782
P 5550 2000
F 0 "#PWR037" H 5550 1750 50  0001 C CNN
F 1 "GND" H 5550 1850 50  0000 C CNN
F 2 "" H 5550 2000 60  0000 C CNN
F 3 "" H 5550 2000 60  0000 C CNN
	1    5550 2000
	-1   0    0    1   
$EndComp
Wire Wire Line
	5550 2000 5550 2250
Wire Wire Line
	5550 2550 5550 2900
Connection ~ 5950 2900
$Comp
L GND #PWR038
U 1 1 558F79DD
P 5150 2000
F 0 "#PWR038" H 5150 1750 50  0001 C CNN
F 1 "GND" H 5150 1850 50  0000 C CNN
F 2 "" H 5150 2000 60  0000 C CNN
F 3 "" H 5150 2000 60  0000 C CNN
	1    5150 2000
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR039
U 1 1 558F79F8
P 2650 2000
F 0 "#PWR039" H 2650 1750 50  0001 C CNN
F 1 "GND" H 2650 1850 50  0000 C CNN
F 2 "" H 2650 2000 60  0000 C CNN
F 3 "" H 2650 2000 60  0000 C CNN
	1    2650 2000
	-1   0    0    1   
$EndComp
Wire Wire Line
	5150 2000 5150 2250
Wire Wire Line
	5150 2550 5150 3000
Connection ~ 5150 3000
Wire Wire Line
	2650 2000 2650 2250
Wire Wire Line
	2650 2550 2650 3000
Connection ~ 2650 3000
$Comp
L GND #PWR040
U 1 1 558F7AED
P 2650 5000
F 0 "#PWR040" H 2650 4750 50  0001 C CNN
F 1 "GND" H 2650 4850 50  0000 C CNN
F 2 "" H 2650 5000 60  0000 C CNN
F 3 "" H 2650 5000 60  0000 C CNN
	1    2650 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 3300 2650 3300
Wire Wire Line
	2650 3300 2650 5000
$Comp
L GND #PWR041
U 1 1 558F7BAB
P 1900 5000
F 0 "#PWR041" H 1900 4750 50  0001 C CNN
F 1 "GND" H 1900 4850 50  0000 C CNN
F 2 "" H 1900 5000 60  0000 C CNN
F 3 "" H 1900 5000 60  0000 C CNN
	1    1900 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 3500 1900 5000
NoConn ~ 6550 3400
NoConn ~ 6550 4000
NoConn ~ 6550 4100
NoConn ~ 7950 4100
NoConn ~ 7950 4000
NoConn ~ 7950 3900
Wire Wire Line
	6350 2900 6350 2000
Wire Wire Line
	5550 2900 6550 2900
Connection ~ 6350 2900
Text HLabel 9500 2900 2    60   Output ~ 0
C_FTDI_TXD
Text HLabel 9500 3000 2    60   Input ~ 0
C_FTDI_RXD
Text HLabel 9500 3100 2    60   Input ~ 0
C_FTDI_RTS
Text HLabel 9500 3200 2    60   Output ~ 0
C_FTDI_CTS
Text HLabel 9500 3600 2    60   Output ~ 0
C_FTDI_RI
NoConn ~ 7950 3500
Wire Wire Line
	8650 3300 8950 3300
Wire Wire Line
	8950 3300 8950 3400
Wire Wire Line
	8950 3400 7950 3400
Wire Wire Line
	7950 3300 8350 3300
Wire Wire Line
	7950 2900 9500 2900
Wire Wire Line
	7950 3000 9500 3000
Wire Wire Line
	7950 3100 9500 3100
Wire Wire Line
	7950 3200 9500 3200
$Comp
L +3V3 #PWR042
U 1 1 558F916C
P 9650 3900
F 0 "#PWR042" H 9650 3750 50  0001 C CNN
F 1 "+3V3" H 9650 4040 50  0000 C CNN
F 2 "" H 9650 3900 60  0000 C CNN
F 3 "" H 9650 3900 60  0000 C CNN
	1    9650 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	8850 3900 9050 3900
Wire Wire Line
	9450 3900 9650 3900
$Comp
L +3V3 #PWR043
U 1 1 558F926F
P 9650 4250
F 0 "#PWR043" H 9650 4100 50  0001 C CNN
F 1 "+3V3" H 9650 4390 50  0000 C CNN
F 2 "" H 9650 4250 60  0000 C CNN
F 3 "" H 9650 4250 60  0000 C CNN
	1    9650 4250
	0    1    1    0   
$EndComp
Wire Wire Line
	8850 4250 9050 4250
Wire Wire Line
	9450 4250 9650 4250
Wire Wire Line
	7950 3700 8300 3700
Wire Wire Line
	8300 3700 8300 3900
Wire Wire Line
	8300 3900 8550 3900
Wire Wire Line
	7950 3800 8150 3800
Wire Wire Line
	8150 3800 8150 4250
Wire Wire Line
	8150 4250 8550 4250
$Comp
L FB_SMT FB1
U 1 1 56324688
P 3750 3000
F 0 "FB1" H 3750 3100 50  0000 C CNN
F 1 "BEAD/0.6OHM/0.2A" H 3750 2950 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" H 3750 2900 30  0001 C CNN
F 3 "" H 3800 3000 60  0000 C CNN
F 4 "0805" H 3750 3000 25  0000 C CNN "SMT"
F 5 "1515714" H 3750 2850 30  0001 C CNN "Part"
F 6 "Farnell" H 3750 2800 30  0001 C CNN "Provider"
	1    3750 3000
	1    0    0    -1  
$EndComp
$Comp
L LED_SMT DL5
U 1 1 563259B8
P 9250 3900
F 0 "DL5" H 9250 4000 50  0000 C CNN
F 1 "GREEN" H 9250 3800 50  0000 C CNN
F 2 "LEDs:LED-0603" H 9250 3750 30  0001 C CNN
F 3 "" H 9250 3900 60  0000 C CNN
F 4 "0603" H 9135 3880 30  0000 C CNN "SMT"
F 5 "2322070" H 9250 3700 30  0001 C CNN "Part"
F 6 "Farnell" H 9250 3650 30  0001 C CNN "Provider"
	1    9250 3900
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C20
U 1 1 56326228
P 2650 2400
F 0 "C20" H 2675 2500 50  0000 L CNN
F 1 "10uF/16V/X5R" V 2500 2050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2680 2250 30  0001 L CNN
F 3 "" H 2675 2500 60  0000 L CNN
F 4 "0805" H 2650 2400 20  0000 C CNN "SMT"
F 5 "1762635" H 2680 2210 30  0001 L CNN "Part"
F 6 "Farnell" H 2680 2170 30  0001 L CNN "Provider"
	1    2650 2400
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C21
U 1 1 563262E1
P 5150 2400
F 0 "C21" H 5175 2500 50  0000 L CNN
F 1 "100nF/50V/X7R" V 5000 2000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5180 2250 30  0001 L CNN
F 3 "" H 5175 2500 60  0000 L CNN
F 4 "0603" H 5150 2400 20  0000 C CNN "SMT"
F 5 "1414028" H 5180 2210 30  0001 L CNN "Part"
F 6 "Farnell" H 5180 2170 30  0001 L CNN "Provider"
	1    5150 2400
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C23
U 1 1 56326366
P 5950 2400
F 0 "C23" H 5975 2500 50  0000 L CNN
F 1 "100nF/50V/X7R" V 5800 2000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5980 2250 30  0001 L CNN
F 3 "" H 5975 2500 60  0000 L CNN
F 4 "0603" H 5950 2400 20  0000 C CNN "SMT"
F 5 "1414028" H 5980 2210 30  0001 L CNN "Part"
F 6 "Farnell" H 5980 2170 30  0001 L CNN "Provider"
	1    5950 2400
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C22
U 1 1 5632641B
P 5550 2400
F 0 "C22" H 5575 2500 50  0000 L CNN
F 1 "10uF/16V/X5R" V 5400 2050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5580 2250 30  0001 L CNN
F 3 "" H 5575 2500 60  0000 L CNN
F 4 "0805" H 5550 2400 20  0000 C CNN "SMT"
F 5 "1762635" H 5580 2210 30  0001 L CNN "Part"
F 6 "Farnell" H 5580 2170 30  0001 L CNN "Provider"
	1    5550 2400
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R33
U 1 1 56326E04
P 8700 4250
F 0 "R33" V 8770 4250 50  0000 C CNN
F 1 "270R/5%" V 8630 4250 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8850 4250 30  0001 C CNN
F 3 "" V 8780 4250 30  0000 C CNN
F 4 "0603" V 8700 4250 20  0000 C CNN "SMT"
F 5 "9330917" V 8900 4250 30  0001 C CNN "Part"
F 6 "Farnell" V 8950 4250 30  0001 C CNN "Provider"
	1    8700 4250
	0    1    1    0   
$EndComp
$Comp
L R_SMT R32
U 1 1 56327083
P 8700 3900
F 0 "R32" V 8770 3900 50  0000 C CNN
F 1 "270R/5%" V 8630 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8850 3900 30  0001 C CNN
F 3 "" V 8780 3900 30  0000 C CNN
F 4 "0603" V 8700 3900 20  0000 C CNN "SMT"
F 5 "9330917" V 8900 3900 30  0001 C CNN "Part"
F 6 "Farnell" V 8950 3900 30  0001 C CNN "Provider"
	1    8700 3900
	0    1    1    0   
$EndComp
$Comp
L R_SMT R31
U 1 1 56327129
P 8500 3300
F 0 "R31" V 8570 3300 50  0000 C CNN
F 1 "N/A" V 8430 3300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8650 3300 30  0001 C CNN
F 3 "" V 8580 3300 30  0000 C CNN
F 4 "0603" V 8500 3300 20  0000 C CNN "SMT"
F 5 "9330917" V 8700 3300 30  0001 C CNN "Part"
F 6 "Farnell" V 8750 3300 30  0001 C CNN "Provider"
	1    8500 3300
	0    1    1    0   
$EndComp
$Comp
L C_SMT C24
U 1 1 563272A4
P 5950 4600
F 0 "C24" H 5975 4700 50  0000 L CNN
F 1 "100nF/50V/X7R" V 5800 4200 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5980 4450 30  0001 L CNN
F 3 "" H 5975 4700 60  0000 L CNN
F 4 "0603" H 5950 4600 20  0000 C CNN "SMT"
F 5 "1414028" H 5980 4410 30  0001 L CNN "Part"
F 6 "Farnell" H 5980 4370 30  0001 L CNN "Provider"
	1    5950 4600
	1    0    0    -1  
$EndComp
NoConn ~ 7950 3600
Wire Wire Line
	7950 3600 9500 3600
NoConn ~ 7950 3100
NoConn ~ 7950 3200
$Comp
L PWR_FLAG #FLG044
U 1 1 56CC9B04
P 4350 2000
F 0 "#FLG044" H 4350 2095 50  0001 C CNN
F 1 "PWR_FLAG" H 4350 2224 50  0000 C CNN
F 2 "" H 4350 2000 50  0000 C CNN
F 3 "" H 4350 2000 50  0000 C CNN
	1    4350 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 2000 4350 3000
Connection ~ 4350 3000
$Comp
L LED_SMT DL17
U 1 1 56CFEF69
P 9250 4250
F 0 "DL17" H 9250 4350 50  0000 C CNN
F 1 "RED" H 9250 4150 50  0000 C CNN
F 2 "LEDs:LED-0603" H 9250 4100 30  0001 C CNN
F 3 "" H 9250 4250 60  0000 C CNN
F 4 "0603" H 9135 4230 30  0000 C CNN "SMT"
F 5 "2322071" H 9250 4050 30  0001 C CNN "Part"
F 6 "Farnell" H 9250 4000 30  0001 C CNN "Provider"
	1    9250 4250
	1    0    0    -1  
$EndComp
$EndSCHEMATC
