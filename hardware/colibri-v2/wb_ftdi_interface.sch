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
Sheet 12 28
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
L FT232RL U?
U 1 1 560A13A8
P 7000 4150
F 0 "U?" H 7350 5200 60  0000 C CNN
F 1 "FT232RL" H 6750 3100 60  0000 C CNN
F 2 "" H 7000 4150 60  0001 C CNN
F 3 "" H 7000 4150 60  0000 C CNN
	1    7000 4150
	1    0    0    -1  
$EndComp
$Comp
L USB_B P?
U 1 1 560A13AF
P 1750 3550
F 0 "P?" H 1950 3350 50  0000 C CNN
F 1 "USB_B" H 1700 3750 50  0000 C CNN
F 2 "" V 1700 3450 60  0001 C CNN
F 3 "" V 1700 3450 60  0000 C CNN
	1    1750 3550
	0    -1   1    0   
$EndComp
Wire Wire Line
	2050 3350 3300 3350
Wire Wire Line
	3700 3350 6300 3350
Wire Wire Line
	2050 3450 6300 3450
Wire Wire Line
	2050 3550 6300 3550
$Comp
L GND #PWR?
U 1 1 560A13C1
P 6100 5350
F 0 "#PWR?" H 6100 5100 50  0001 C CNN
F 1 "GND" H 6100 5200 50  0000 C CNN
F 2 "" H 6100 5350 60  0000 C CNN
F 3 "" H 6100 5350 60  0000 C CNN
	1    6100 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 4650 6100 4650
Wire Wire Line
	6100 4650 6100 5350
Wire Wire Line
	6300 4750 6100 4750
Connection ~ 6100 4750
Wire Wire Line
	6100 4850 6300 4850
Connection ~ 6100 4850
Wire Wire Line
	6300 4950 6100 4950
Connection ~ 6100 4950
Wire Wire Line
	6100 5050 6300 5050
Connection ~ 6100 5050
Wire Wire Line
	6300 4550 5700 4550
Wire Wire Line
	5700 4550 5700 4800
$Comp
L GND #PWR?
U 1 1 560A13DA
P 5700 5350
F 0 "#PWR?" H 5700 5100 50  0001 C CNN
F 1 "GND" H 5700 5200 50  0000 C CNN
F 2 "" H 5700 5350 60  0000 C CNN
F 3 "" H 5700 5350 60  0000 C CNN
	1    5700 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 5100 5700 5350
$Comp
L +3V3 #PWR?
U 1 1 560A13E1
P 6100 2350
F 0 "#PWR?" H 6100 2200 50  0001 C CNN
F 1 "+3V3" H 6100 2490 50  0000 C CNN
F 2 "" H 6100 2350 60  0000 C CNN
F 3 "" H 6100 2350 60  0000 C CNN
	1    6100 2350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 560A13EE
P 5700 2350
F 0 "#PWR?" H 5700 2100 50  0001 C CNN
F 1 "GND" H 5700 2200 50  0000 C CNN
F 2 "" H 5700 2350 60  0000 C CNN
F 3 "" H 5700 2350 60  0000 C CNN
	1    5700 2350
	-1   0    0    1   
$EndComp
Wire Wire Line
	5700 2350 5700 2600
Wire Wire Line
	5700 2900 5700 3250
$Comp
L GND #PWR?
U 1 1 560A13FD
P 5300 2350
F 0 "#PWR?" H 5300 2100 50  0001 C CNN
F 1 "GND" H 5300 2200 50  0000 C CNN
F 2 "" H 5300 2350 60  0000 C CNN
F 3 "" H 5300 2350 60  0000 C CNN
	1    5300 2350
	-1   0    0    1   
$EndComp
Wire Wire Line
	5300 2350 5300 2600
Wire Wire Line
	5300 2900 5300 3250
Connection ~ 5700 3250
$Comp
L GND #PWR?
U 1 1 560A1414
P 4900 2350
F 0 "#PWR?" H 4900 2100 50  0001 C CNN
F 1 "GND" H 4900 2200 50  0000 C CNN
F 2 "" H 4900 2350 60  0000 C CNN
F 3 "" H 4900 2350 60  0000 C CNN
	1    4900 2350
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR?
U 1 1 560A141A
P 2400 2350
F 0 "#PWR?" H 2400 2100 50  0001 C CNN
F 1 "GND" H 2400 2200 50  0000 C CNN
F 2 "" H 2400 2350 60  0000 C CNN
F 3 "" H 2400 2350 60  0000 C CNN
	1    2400 2350
	-1   0    0    1   
$EndComp
Wire Wire Line
	4900 2350 4900 2600
Wire Wire Line
	4900 2900 4900 3350
Connection ~ 4900 3350
Wire Wire Line
	2400 2350 2400 2600
Wire Wire Line
	2400 2900 2400 3350
Connection ~ 2400 3350
$Comp
L GND #PWR?
U 1 1 560A1426
P 2400 5350
F 0 "#PWR?" H 2400 5100 50  0001 C CNN
F 1 "GND" H 2400 5200 50  0000 C CNN
F 2 "" H 2400 5350 60  0000 C CNN
F 3 "" H 2400 5350 60  0000 C CNN
	1    2400 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 3650 2400 3650
Wire Wire Line
	2400 3650 2400 5350
$Comp
L GND #PWR?
U 1 1 560A142E
P 1650 5350
F 0 "#PWR?" H 1650 5100 50  0001 C CNN
F 1 "GND" H 1650 5200 50  0000 C CNN
F 2 "" H 1650 5350 60  0000 C CNN
F 3 "" H 1650 5350 60  0000 C CNN
	1    1650 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 3850 1650 5350
NoConn ~ 6300 3750
NoConn ~ 6300 4350
NoConn ~ 6300 4450
NoConn ~ 7700 4450
NoConn ~ 7700 4350
NoConn ~ 7700 4250
Wire Wire Line
	6100 3250 6100 2350
Wire Wire Line
	5300 3250 6300 3250
Connection ~ 6100 3250
Text HLabel 9250 3250 2    60   Output ~ 0
WB_FTDI_RXD
Text HLabel 9250 3350 2    60   Input ~ 0
WB_FTDI_TXD
Text HLabel 9250 3450 2    60   Output ~ 0
WB_FTDI_CTS
Text HLabel 9250 3550 2    60   Input ~ 0
WB_FTDI_RTS
Text HLabel 9250 3950 2    60   Output ~ 0
WB_FTDI_RI
Wire Wire Line
	7700 3250 9250 3250
Wire Wire Line
	7700 3350 9250 3350
Wire Wire Line
	7700 3450 9250 3450
Wire Wire Line
	7700 3550 9250 3550
Wire Wire Line
	7700 3950 9250 3950
$Comp
L +3V3 #PWR?
U 1 1 560A1456
P 9400 4250
F 0 "#PWR?" H 9400 4100 50  0001 C CNN
F 1 "+3V3" H 9400 4390 50  0000 C CNN
F 2 "" H 9400 4250 60  0000 C CNN
F 3 "" H 9400 4250 60  0000 C CNN
	1    9400 4250
	0    1    1    0   
$EndComp
Wire Wire Line
	8600 4250 8800 4250
Wire Wire Line
	9200 4250 9400 4250
$Comp
L +3V3 #PWR?
U 1 1 560A146C
P 9400 4600
F 0 "#PWR?" H 9400 4450 50  0001 C CNN
F 1 "+3V3" H 9400 4740 50  0000 C CNN
F 2 "" H 9400 4600 60  0000 C CNN
F 3 "" H 9400 4600 60  0000 C CNN
	1    9400 4600
	0    1    1    0   
$EndComp
Wire Wire Line
	8600 4600 8800 4600
Wire Wire Line
	9200 4600 9400 4600
Wire Wire Line
	7700 4050 8050 4050
Wire Wire Line
	8050 4050 8050 4250
Wire Wire Line
	8050 4250 8300 4250
Wire Wire Line
	7700 4150 7900 4150
Wire Wire Line
	7900 4150 7900 4600
Wire Wire Line
	7900 4600 8300 4600
Text HLabel 9250 3650 2    60   Output ~ 0
WB_FTDI_DSR
Wire Wire Line
	7700 3650 9250 3650
Text HLabel 9250 3750 2    60   Input ~ 0
WB_FTDI_DTR
Wire Wire Line
	7700 3750 9250 3750
Text HLabel 9250 3850 2    60   Input ~ 0
WB_FTDI_DCD
Wire Wire Line
	7700 3850 9250 3850
$Comp
L R_SMT R?
U 1 1 56335349
P 8450 4250
F 0 "R?" V 8520 4250 50  0000 C CNN
F 1 "270R/5%" V 8380 4250 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8600 4250 30  0001 C CNN
F 3 "" V 8530 4250 30  0000 C CNN
F 4 "0603" V 8450 4250 20  0000 C CNN "SMT"
F 5 "9330917" V 8650 4250 30  0001 C CNN "Part"
F 6 "Farnell" V 8700 4250 30  0001 C CNN "Provider"
	1    8450 4250
	0    1    1    0   
$EndComp
$Comp
L R_SMT R?
U 1 1 5633542F
P 8450 4600
F 0 "R?" V 8520 4600 50  0000 C CNN
F 1 "270R/5%" V 8380 4600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8600 4600 30  0001 C CNN
F 3 "" V 8530 4600 30  0000 C CNN
F 4 "0603" V 8450 4600 20  0000 C CNN "SMT"
F 5 "9330917" V 8650 4600 30  0001 C CNN "Part"
F 6 "Farnell" V 8700 4600 30  0001 C CNN "Provider"
	1    8450 4600
	0    1    1    0   
$EndComp
$Comp
L LED_SMT DL?
U 1 1 5633554F
P 9000 4250
F 0 "DL?" H 9000 4350 50  0000 C CNN
F 1 "GREEN" H 9000 4150 50  0000 C CNN
F 2 "LEDs:LED-0603" H 9000 4100 30  0001 C CNN
F 3 "" H 9000 4250 60  0000 C CNN
F 4 "0603" H 8885 4230 30  0000 C CNN "SMT"
F 5 "2322070" H 9000 4050 30  0001 C CNN "Part"
F 6 "Farnell" H 9000 4000 30  0001 C CNN "Provider"
	1    9000 4250
	1    0    0    -1  
$EndComp
$Comp
L LED D?
U 1 1 56335832
P 9000 4600
F 0 "D?" H 9000 4700 50  0000 C CNN
F 1 "RED" H 9000 4500 50  0000 C CNN
F 2 "" H 9000 4600 60  0001 C CNN
F 3 "" H 9000 4600 60  0000 C CNN
	1    9000 4600
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C?
U 1 1 56335D21
P 5700 4950
F 0 "C?" H 5725 5050 50  0000 L CNN
F 1 "100nF/50V/X7R" V 5550 4550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5730 4800 30  0001 L CNN
F 3 "" H 5725 5050 60  0000 L CNN
F 4 "0603" H 5700 4950 20  0000 C CNN "SMT"
F 5 "1414028" H 5730 4760 30  0001 L CNN "Part"
F 6 "Farnell" H 5730 4720 30  0001 L CNN "Provider"
	1    5700 4950
	1    0    0    -1  
$EndComp
$Comp
L FB_SMT FB?
U 1 1 56336479
P 3500 3350
F 0 "FB?" H 3500 3450 50  0000 C CNN
F 1 "BEAD/0.6OHM/0.2A" H 3500 3300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" H 3500 3250 30  0001 C CNN
F 3 "" H 3550 3350 60  0000 C CNN
F 4 "0805" H 3500 3350 25  0000 C CNN "SMT"
F 5 "1515714" H 3500 3200 30  0001 C CNN "Part"
F 6 "Farnell" H 3500 3150 30  0001 C CNN "Provider"
	1    3500 3350
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C?
U 1 1 5633664F
P 2400 2750
F 0 "C?" H 2425 2850 50  0000 L CNN
F 1 "10uF/16V/X5R" V 2250 2400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2430 2600 30  0001 L CNN
F 3 "" H 2425 2850 60  0000 L CNN
F 4 "0805" H 2400 2750 20  0000 C CNN "SMT"
F 5 "1762635" H 2430 2560 30  0001 L CNN "Part"
F 6 "Farnell" H 2430 2520 30  0001 L CNN "Provider"
	1    2400 2750
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C?
U 1 1 5633687F
P 5300 2750
F 0 "C?" H 5325 2850 50  0000 L CNN
F 1 "10uF/16V/X5R" V 5150 2400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5330 2600 30  0001 L CNN
F 3 "" H 5325 2850 60  0000 L CNN
F 4 "0805" H 5300 2750 20  0000 C CNN "SMT"
F 5 "1762635" H 5330 2560 30  0001 L CNN "Part"
F 6 "Farnell" H 5330 2520 30  0001 L CNN "Provider"
	1    5300 2750
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C?
U 1 1 563369C6
P 5700 2750
F 0 "C?" H 5725 2850 50  0000 L CNN
F 1 "100nF/50V/X7R" V 5550 2350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5730 2600 30  0001 L CNN
F 3 "" H 5725 2850 60  0000 L CNN
F 4 "0603" H 5700 2750 20  0000 C CNN "SMT"
F 5 "1414028" H 5730 2560 30  0001 L CNN "Part"
F 6 "Farnell" H 5730 2520 30  0001 L CNN "Provider"
	1    5700 2750
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C?
U 1 1 56336BEE
P 4900 2750
F 0 "C?" H 4925 2850 50  0000 L CNN
F 1 "100nF/50V/X7R" V 4750 2350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4930 2600 30  0001 L CNN
F 3 "" H 4925 2850 60  0000 L CNN
F 4 "0603" H 4900 2750 20  0000 C CNN "SMT"
F 5 "1414028" H 4930 2560 30  0001 L CNN "Part"
F 6 "Farnell" H 4930 2520 30  0001 L CNN "Provider"
	1    4900 2750
	1    0    0    -1  
$EndComp
$EndSCHEMATC
