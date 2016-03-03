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
Sheet 13 28
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 3150 2850 0    60   BiDi ~ 0
TRD0_P
Text HLabel 3150 2950 0    60   BiDi ~ 0
TRD0_N
Text HLabel 3150 3150 0    60   BiDi ~ 0
TRD1_P
Text HLabel 3150 3250 0    60   BiDi ~ 0
TRD1_N
Text HLabel 3150 3450 0    60   BiDi ~ 0
TRD2_P
Text HLabel 3150 3550 0    60   BiDi ~ 0
TRD2_N
Text HLabel 3150 3750 0    60   BiDi ~ 0
TRD3_P
Text HLabel 3150 3850 0    60   BiDi ~ 0
TRD3_N
Wire Wire Line
	3150 2850 5650 2850
Wire Wire Line
	3150 2950 5650 2950
Wire Wire Line
	3150 3150 5650 3150
Wire Wire Line
	3150 3250 5650 3250
Wire Wire Line
	3150 3450 5650 3450
Wire Wire Line
	3150 3550 5650 3550
Wire Wire Line
	3150 3750 5650 3750
Wire Wire Line
	3150 3850 5650 3850
Wire Wire Line
	5650 4050 4400 4050
Wire Wire Line
	4100 4050 3400 4050
Wire Wire Line
	3400 4050 3400 4350
Wire Wire Line
	3100 4150 5650 4150
$Comp
L GND #PWR0119
U 1 1 561772BC
P 3100 4150
F 0 "#PWR0119" H 3100 3900 50  0001 C CNN
F 1 "GND" H 3100 4000 50  0000 C CNN
F 2 "" H 3100 4150 60  0000 C CNN
F 3 "" H 3100 4150 60  0000 C CNN
	1    3100 4150
	0    1    1    0   
$EndComp
Text HLabel 3150 4450 0    60   Input ~ 0
LED1_ACT
Text HLabel 3150 4650 0    60   Input ~ 0
LED1_nLINK100
Text HLabel 3150 4750 0    60   Input ~ 0
LED1_nLINK1000
Wire Wire Line
	3150 4650 4100 4650
Wire Wire Line
	3150 4750 4100 4750
$Comp
L C_SMT C39
U 1 1 56349CAD
P 4250 4050
F 0 "C39" V 4200 4150 50  0000 L CNN
F 1 "100nF/50V/X7R" V 4200 3350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4280 3900 30  0001 L CNN
F 3 "" H 4275 4150 60  0000 L CNN
F 4 "0603" H 4250 4050 20  0000 C CNN "SMT"
F 5 "1414028" H 4280 3860 30  0001 L CNN "Part"
F 6 "Farnell" H 4280 3820 30  0001 L CNN "Provider"
	1    4250 4050
	0    1    1    0   
$EndComp
$Comp
L R_SMT R84
U 1 1 5634AECD
P 4250 4650
F 0 "R84" V 4300 4800 50  0000 C CNN
F 1 "330R/5%" V 4200 4350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4400 4650 30  0001 C CNN
F 3 "" V 4330 4650 30  0000 C CNN
F 4 "0603" V 4250 4650 20  0000 C CNN "SMT"
F 5 "9332014" V 4450 4650 30  0001 C CNN "Part"
F 6 "Farnell" V 4500 4650 30  0001 C CNN "Provider"
	1    4250 4650
	0    1    1    0   
$EndComp
$Comp
L GND #PWR0120
U 1 1 56CB1E1A
P 6350 5350
F 0 "#PWR0120" H 6350 5100 50  0001 C CNN
F 1 "GND" H 6350 5200 50  0000 C CNN
F 2 "" H 6350 5350 60  0000 C CNN
F 3 "" H 6350 5350 60  0000 C CNN
	1    6350 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 5050 6350 5350
Connection ~ 6350 5250
$Comp
L 7499111446 LAN1
U 1 1 56CB322E
P 6350 3800
AR Path="/56CB322E" Ref="LAN1"  Part="1" 
AR Path="/5609F831/561764C8/56CB322E" Ref="LAN1"  Part="1" 
F 0 "LAN1" H 6877 3747 60  0000 L CNN
F 1 "7499111446" H 6877 3641 60  0000 L CNN
F 2 "Sigmadrone:WURTH_7499111446" H 6350 4250 60  0001 C CNN
F 3 "" H 6350 4250 60  0000 C CNN
	1    6350 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 4650 5650 4650
Wire Wire Line
	4400 4750 5650 4750
$Comp
L R_SMT R251
U 1 1 56CB708A
P 4250 4750
F 0 "R251" V 4300 4900 50  0000 C CNN
F 1 "0R/5%" V 4200 4500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4400 4750 30  0001 C CNN
F 3 "" V 4330 4750 30  0000 C CNN
F 4 "0603" V 4250 4750 20  0000 C CNN "SMT"
F 5 "1469739" V 4450 4750 30  0001 C CNN "Part"
F 6 "Farnell" V 4500 4750 30  0001 C CNN "Provider"
	1    4250 4750
	0    1    1    0   
$EndComp
$Comp
L R_SMT R250
U 1 1 56CB719B
P 4250 4450
F 0 "R250" V 4300 4600 50  0000 C CNN
F 1 "330R/5%" V 4200 4150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4400 4450 30  0001 C CNN
F 3 "" V 4330 4450 30  0000 C CNN
F 4 "0603" V 4250 4450 20  0000 C CNN "SMT"
F 5 "9332014" V 4450 4450 30  0001 C CNN "Part"
F 6 "Farnell" V 4500 4450 30  0001 C CNN "Provider"
	1    4250 4450
	0    1    1    0   
$EndComp
$Comp
L R_SMT R85
U 1 1 56CB71C8
P 4250 4350
F 0 "R85" V 4300 4500 50  0000 C CNN
F 1 "0R/5%" V 4200 4100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4400 4350 30  0001 C CNN
F 3 "" V 4330 4350 30  0000 C CNN
F 4 "0603" V 4250 4350 20  0000 C CNN "SMT"
F 5 "1469739" V 4450 4350 30  0001 C CNN "Part"
F 6 "Farnell" V 4500 4350 30  0001 C CNN "Provider"
	1    4250 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	4400 4450 5650 4450
Wire Wire Line
	4400 4350 5650 4350
Wire Wire Line
	3150 4450 4100 4450
Wire Wire Line
	3400 4350 4100 4350
Connection ~ 3400 4150
Wire Wire Line
	6450 5250 6450 5050
$Comp
L PWR_FLAG #FLG0121
U 1 1 56CDF282
P 5300 2550
F 0 "#FLG0121" H 5300 2645 50  0001 C CNN
F 1 "PWR_FLAG" H 5300 2774 50  0000 C CNN
F 2 "" H 5300 2550 50  0000 C CNN
F 3 "" H 5300 2550 50  0000 C CNN
	1    5300 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 2550 5300 4050
Connection ~ 5300 4050
Wire Wire Line
	6450 5250 6350 5250
$EndSCHEMATC
