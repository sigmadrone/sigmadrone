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
Sheet 20 26
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 7100 3650 2    60   BiDi ~ 0
EEPROM_SDA
Text HLabel 7100 3550 2    60   Input ~ 0
EEPROM_SCL
$Comp
L +3V3 #PWR?
U 1 1 5669460C
P 4850 2700
F 0 "#PWR?" H 4850 2550 50  0001 C CNN
F 1 "+3V3" H 4850 2840 50  0000 C CNN
F 2 "" H 4850 2700 50  0000 C CNN
F 3 "" H 4850 2700 50  0000 C CNN
	1    4850 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 2700 4850 3550
Wire Wire Line
	4400 3350 5100 3350
Wire Wire Line
	4850 3450 5100 3450
Connection ~ 4850 3350
Wire Wire Line
	4850 3550 5100 3550
Connection ~ 4850 3450
$Comp
L GND #PWR?
U 1 1 56694707
P 4850 4050
F 0 "#PWR?" H 4850 3800 50  0001 C CNN
F 1 "GND" H 4850 3900 50  0000 C CNN
F 2 "" H 4850 4050 50  0000 C CNN
F 3 "" H 4850 4050 50  0000 C CNN
	1    4850 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 3750 4850 3750
Wire Wire Line
	4850 3750 4850 4050
$Comp
L GND #PWR?
U 1 1 566951C8
P 6300 4050
F 0 "#PWR?" H 6300 3800 50  0001 C CNN
F 1 "GND" H 6300 3900 50  0000 C CNN
F 2 "" H 6300 4050 50  0000 C CNN
F 3 "" H 6300 4050 50  0000 C CNN
	1    6300 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 3750 6300 3750
Wire Wire Line
	6300 3750 6300 4050
Wire Wire Line
	6000 3550 7100 3550
Wire Wire Line
	6000 3650 7100 3650
$Comp
L C_SMT C?
U 1 1 56695669
P 4400 3600
F 0 "C?" H 4425 3700 50  0000 L CNN
F 1 "100nF/50V/X7R" V 4250 3450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4430 3450 30  0001 L CNN
F 3 "" H 4425 3700 60  0000 L CNN
F 4 "0603" H 4400 3600 20  0000 C CNN "SMT"
F 5 "1414028" H 4430 3410 30  0001 L CNN "Part"
F 6 "Farnell" H 4430 3370 30  0001 L CNN "Provider"
	1    4400 3600
	-1   0    0    1   
$EndComp
$Comp
L BR24Lxxx-W U?
U 1 1 566962EE
P 5550 3550
F 0 "U?" H 5650 3900 60  0000 C CNN
F 1 "BR24LFVJ-W" H 5550 3200 60  0000 C CNN
F 2 "Housings_SSOP:TSSOP-8_3x3mm_Pitch0.65mm" H 5550 3150 60  0001 C CNN
F 3 "" H 5550 3450 60  0000 C CNN
	1    5550 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 3350 4400 3450
$Comp
L GND #PWR?
U 1 1 5669653A
P 4400 4050
F 0 "#PWR?" H 4400 3800 50  0001 C CNN
F 1 "GND" H 4400 3900 50  0000 C CNN
F 2 "" H 4400 4050 50  0000 C CNN
F 3 "" H 4400 4050 50  0000 C CNN
	1    4400 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 3750 4400 4050
$EndSCHEMATC
