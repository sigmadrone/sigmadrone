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
Sheet 15 23
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
L 7499111121A LAN1
U 1 1 56176AEE
P 6350 3800
F 0 "LAN1" H 6750 4900 60  0000 C CNN
F 1 "7499111121A" H 6150 4900 60  0000 C CNN
F 2 "" H 6350 4250 60  0000 C CNN
F 3 "" H 6350 4250 60  0000 C CNN
	1    6350 3800
	1    0    0    -1  
$EndComp
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
$Comp
L C C67
U 1 1 5617727F
P 4250 4050
F 0 "C67" V 4200 4100 50  0000 L CNN
F 1 "100n/50V" V 4200 3600 50  0000 L CNN
F 2 "" H 4288 3900 30  0000 C CNN
F 3 "" H 4250 4050 60  0000 C CNN
	1    4250 4050
	0    1    1    0   
$EndComp
Wire Wire Line
	5650 4050 4400 4050
Wire Wire Line
	4100 4050 3800 4050
Wire Wire Line
	3800 4050 3800 4150
Wire Wire Line
	3800 4150 5650 4150
$Comp
L GND #PWR0105
U 1 1 561772BC
P 5000 5150
F 0 "#PWR0105" H 5000 4900 50  0001 C CNN
F 1 "GND" H 5000 5000 50  0000 C CNN
F 2 "" H 5000 5150 60  0000 C CNN
F 3 "" H 5000 5150 60  0000 C CNN
	1    5000 5150
	1    0    0    -1  
$EndComp
Text HLabel 3150 4450 0    60   Input ~ 0
LED1_ACT
Text HLabel 3150 4750 0    60   Input ~ 0
LED1_nLINK100
Text HLabel 3150 4900 0    60   Input ~ 0
LED1_nLINK1000
Wire Wire Line
	3150 4450 5650 4450
Wire Wire Line
	5650 4350 5250 4350
Wire Wire Line
	5250 2300 5250 4650
Wire Wire Line
	5250 4650 5650 4650
Wire Wire Line
	5000 5150 5000 4150
Connection ~ 5000 4150
$Comp
L +3V3 #PWR0106
U 1 1 561773DA
P 5250 2300
F 0 "#PWR0106" H 5250 2150 50  0001 C CNN
F 1 "+3V3" H 5250 2440 50  0000 C CNN
F 2 "" H 5250 2300 60  0000 C CNN
F 3 "" H 5250 2300 60  0000 C CNN
	1    5250 2300
	1    0    0    -1  
$EndComp
Connection ~ 5250 4350
$Comp
L R R76
U 1 1 56177409
P 4250 4750
F 0 "R76" V 4330 4750 50  0000 C CNN
F 1 "100" V 4250 4750 50  0000 C CNN
F 2 "" V 4180 4750 30  0000 C CNN
F 3 "" H 4250 4750 30  0000 C CNN
	1    4250 4750
	0    1    1    0   
$EndComp
$Comp
L R R77
U 1 1 56177461
P 4250 4900
F 0 "R77" V 4330 4900 50  0000 C CNN
F 1 "100" V 4250 4900 50  0000 C CNN
F 2 "" V 4180 4900 30  0000 C CNN
F 3 "" H 4250 4900 30  0000 C CNN
	1    4250 4900
	0    1    1    0   
$EndComp
Wire Wire Line
	3150 4750 4100 4750
Wire Wire Line
	4400 4750 5650 4750
Wire Wire Line
	3150 4900 4100 4900
Wire Wire Line
	4400 4900 4700 4900
Wire Wire Line
	4700 4900 4700 4750
Connection ~ 4700 4750
Text Notes 7000 3850 0    60   ~ 12
To optimize the size!!!\n(Zhoro will research)
$EndSCHEMATC
