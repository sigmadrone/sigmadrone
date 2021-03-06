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
Sheet 9 28
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
L +3V3 #PWR093
U 1 1 55FF262C
P 6500 3450
F 0 "#PWR093" H 6500 3300 50  0001 C CNN
F 1 "+3V3" H 6500 3590 50  0000 C CNN
F 2 "" H 6500 3450 60  0000 C CNN
F 3 "" H 6500 3450 60  0000 C CNN
	1    6500 3450
	1    0    0    -1  
$EndComp
$Comp
L Q_PMOS_GSD Q12
U 1 1 55FF2632
P 6400 3950
F 0 "Q12" H 6700 4000 50  0000 R CNN
F 1 "IRLML6402TRPBF" H 7250 3900 50  0000 R CNN
F 2 "Sigmadrone:SOT23_IRLML6402" H 6600 4050 29  0001 C CNN
F 3 "" H 6400 3950 60  0000 C CNN
	1    6400 3950
	1    0    0    1   
$EndComp
Wire Wire Line
	6500 3750 6500 3450
$Comp
L GND #PWR094
U 1 1 55FF2644
P 5600 4700
F 0 "#PWR094" H 5600 4450 50  0001 C CNN
F 1 "GND" H 5600 4550 50  0000 C CNN
F 2 "" H 5600 4700 60  0000 C CNN
F 3 "" H 5600 4700 60  0000 C CNN
	1    5600 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 4200 5600 3950
Connection ~ 5600 3950
Wire Wire Line
	5600 4500 5600 4700
Text HLabel 8150 4500 2    60   Output ~ 0
V_SENS1
Wire Wire Line
	6500 4150 6500 4500
Wire Wire Line
	6500 4500 8150 4500
Text HLabel 3250 3950 0    60   Input ~ 0
V_SENS_CTRL
$Comp
L R_SMT R56
U 1 1 562EF326
P 5600 4350
F 0 "R56" V 5670 4350 50  0000 C CNN
F 1 "100K/5%" V 5530 4350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5750 4350 30  0001 C CNN
F 3 "" V 5680 4350 30  0000 C CNN
F 4 "0603" V 5600 4350 20  0000 C CNN "SMT"
F 5 "9233628" V 5800 4350 30  0001 C CNN "Part"
F 6 "Farnell" V 5850 4350 30  0001 C CNN "Provider"
	1    5600 4350
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R55
U 1 1 5636BACE
P 5050 3950
F 0 "R55" V 5000 4150 50  0000 C CNN
F 1 "22R/5%" V 5000 3700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5200 3950 30  0001 C CNN
F 3 "" V 5130 3950 30  0000 C CNN
F 4 "0603" V 5050 3950 20  0000 C CNN "SMT"
F 5 "9331891" V 5250 3950 30  0001 C CNN "Part"
F 6 "Farnell" V 5300 3950 30  0001 C CNN "Provider"
	1    5050 3950
	0    1    1    0   
$EndComp
Wire Wire Line
	3250 3950 4900 3950
Wire Wire Line
	5200 3950 6200 3950
$EndSCHEMATC
