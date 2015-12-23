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
Sheet 14 27
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
L uSDCARD-H1.85 MSD?
U 1 1 56153956
P 6350 4000
F 0 "MSD?" H 6600 3300 60  0000 C CNN
F 1 "uSDCARD-H1.85" H 6450 4700 60  0000 C CNN
F 2 "" H 6350 4000 60  0001 C CNN
F 3 "" H 6350 4000 60  0000 C CNN
	1    6350 4000
	1    0    0    -1  
$EndComp
Text HLabel 3450 3450 0    60   BiDi ~ 0
SD1_DATA2
Text HLabel 3450 3550 0    60   BiDi ~ 0
SD1_DATA3
Text HLabel 3450 3850 0    60   BiDi ~ 0
SD1_CLK
Text HLabel 3450 4050 0    60   BiDi ~ 0
SD1_DATA0
Text HLabel 3450 4150 0    60   BiDi ~ 0
SD1_DATA1
Text HLabel 3450 4250 0    60   BiDi ~ 0
SD1_CMD
Text HLabel 3450 4350 0    60   Output ~ 0
SD1_CD
Text HLabel 3450 4450 0    60   Output ~ 0
SD1_WP
$Comp
L GND #PWR?
U 1 1 5617363A
P 5350 5200
F 0 "#PWR?" H 5350 4950 50  0001 C CNN
F 1 "GND" H 5350 5050 50  0000 C CNN
F 2 "" H 5350 5200 60  0000 C CNN
F 3 "" H 5350 5200 60  0000 C CNN
	1    5350 5200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5617364F
P 6100 5200
F 0 "#PWR?" H 6100 4950 50  0001 C CNN
F 1 "GND" H 6100 5050 50  0000 C CNN
F 2 "" H 6100 5200 60  0000 C CNN
F 3 "" H 6100 5200 60  0000 C CNN
	1    6100 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 4550 5350 4550
Wire Wire Line
	5350 3950 5350 4550
Wire Wire Line
	5350 4550 5350 5200
Wire Wire Line
	6100 4850 6100 4950
Wire Wire Line
	6100 4950 6100 5200
Wire Wire Line
	6200 4850 6200 4950
Wire Wire Line
	6200 4950 6100 4950
Connection ~ 6100 4950
Wire Wire Line
	3450 3450 4400 3450
Wire Wire Line
	4400 3450 5500 3450
Wire Wire Line
	3450 3550 4500 3550
Wire Wire Line
	4500 3550 5500 3550
Wire Wire Line
	3450 4050 4700 4050
Wire Wire Line
	4700 4050 5500 4050
Wire Wire Line
	3450 4150 4800 4150
Wire Wire Line
	4800 4150 5500 4150
Wire Wire Line
	3450 4250 4900 4250
Wire Wire Line
	4900 4250 5500 4250
Wire Wire Line
	3450 4350 4300 4350
Wire Wire Line
	4300 4350 5500 4350
Wire Wire Line
	5500 3950 5350 3950
Connection ~ 5350 4550
Wire Wire Line
	4100 3850 4600 3850
Wire Wire Line
	4600 3850 5500 3850
Wire Wire Line
	3800 3850 3450 3850
$Comp
L +3V3 #PWR?
U 1 1 561737FF
P 5350 1550
F 0 "#PWR?" H 5350 1400 50  0001 C CNN
F 1 "+3V3" H 5350 1690 50  0000 C CNN
F 2 "" H 5350 1550 60  0000 C CNN
F 3 "" H 5350 1550 60  0000 C CNN
	1    5350 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 1550 5350 1850
Wire Wire Line
	5350 1850 5350 3750
Wire Wire Line
	5350 3750 5500 3750
Wire Wire Line
	3450 4450 4200 4450
Wire Wire Line
	4200 4450 4200 4600
$Comp
L GND #PWR?
U 1 1 56173E31
P 4200 5200
F 0 "#PWR?" H 4200 4950 50  0001 C CNN
F 1 "GND" H 4200 5050 50  0000 C CNN
F 2 "" H 4200 5200 60  0000 C CNN
F 3 "" H 4200 5200 60  0000 C CNN
	1    4200 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 4900 4200 5200
Wire Wire Line
	4900 2350 4900 4250
Connection ~ 4900 4250
Wire Wire Line
	4800 2350 4800 4150
Connection ~ 4800 4150
Wire Wire Line
	4700 2350 4700 4050
Connection ~ 4700 4050
Wire Wire Line
	4600 2350 4600 3850
Connection ~ 4600 3850
Wire Wire Line
	4500 2350 4500 3550
Connection ~ 4500 3550
Wire Wire Line
	4400 2350 4400 3450
Connection ~ 4400 3450
Wire Wire Line
	4300 2350 4300 4350
Connection ~ 4300 4350
Wire Wire Line
	4300 2050 4300 1850
Wire Wire Line
	4300 1850 4400 1850
Wire Wire Line
	4400 1850 4500 1850
Wire Wire Line
	4500 1850 4600 1850
Wire Wire Line
	4600 1850 4700 1850
Wire Wire Line
	4700 1850 4800 1850
Wire Wire Line
	4800 1850 4900 1850
Wire Wire Line
	4900 1850 5350 1850
Wire Wire Line
	5350 1850 5900 1850
Wire Wire Line
	5900 1850 6500 1850
Connection ~ 5350 1850
Wire Wire Line
	4400 2050 4400 1850
Connection ~ 4400 1850
Wire Wire Line
	4500 2050 4500 1850
Connection ~ 4500 1850
Wire Wire Line
	4600 2050 4600 1850
Connection ~ 4600 1850
Wire Wire Line
	4700 2050 4700 1850
Connection ~ 4700 1850
Wire Wire Line
	4800 2050 4800 1850
Connection ~ 4800 1850
Wire Wire Line
	4900 2050 4900 1850
Connection ~ 4900 1850
$Comp
L GND #PWR?
U 1 1 5617463C
P 6500 2850
F 0 "#PWR?" H 6500 2600 50  0001 C CNN
F 1 "GND" H 6500 2700 50  0000 C CNN
F 2 "" H 6500 2850 60  0000 C CNN
F 3 "" H 6500 2850 60  0000 C CNN
	1    6500 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 1850 6500 2050
Wire Wire Line
	5900 2050 5900 1850
Connection ~ 5900 1850
Wire Wire Line
	6500 2350 6500 2700
Wire Wire Line
	6500 2700 6500 2850
Wire Wire Line
	5900 2350 5900 2700
Wire Wire Line
	5900 2700 6500 2700
Connection ~ 6500 2700
Text Notes 7400 4100 0    120  ~ 24
Do we need this?
$Comp
L C_SMT C?
U 1 1 563467DB
P 5900 2200
F 0 "C?" H 5925 2300 50  0000 L CNN
F 1 "100nF/50V/X7R" V 5750 1800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5930 2050 30  0001 L CNN
F 3 "" H 5925 2300 60  0000 L CNN
F 4 "0603" H 5900 2200 20  0000 C CNN "SMT"
F 5 "1414028" H 5930 2010 30  0001 L CNN "Part"
F 6 "Farnell" H 5930 1970 30  0001 L CNN "Provider"
	1    5900 2200
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C?
U 1 1 563467E5
P 6500 2200
F 0 "C?" H 6525 2300 50  0000 L CNN
F 1 "10uF/16V/X5R" V 6350 1850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6530 2050 30  0001 L CNN
F 3 "" H 6525 2300 60  0000 L CNN
F 4 "0805" H 6500 2200 20  0000 C CNN "SMT"
F 5 "1762635" H 6530 2010 30  0001 L CNN "Part"
F 6 "Farnell" H 6530 1970 30  0001 L CNN "Provider"
	1    6500 2200
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56347EB1
P 4200 4750
F 0 "R?" V 4250 4900 50  0000 C CNN
F 1 "10K/5%" V 4150 4500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4350 4750 30  0001 C CNN
F 3 "" V 4280 4750 30  0000 C CNN
F 4 "0603" V 4200 4750 20  0000 C CNN "SMT"
F 5 "9331700" V 4400 4750 30  0001 C CNN "Part"
F 6 "Farnell" V 4450 4750 30  0001 C CNN "Provider"
	1    4200 4750
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56347F11
P 4300 2200
F 0 "R?" V 4350 2350 50  0000 C CNN
F 1 "10K/5%" V 4250 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4450 2200 30  0001 C CNN
F 3 "" V 4380 2200 30  0000 C CNN
F 4 "0603" V 4300 2200 20  0000 C CNN "SMT"
F 5 "9331700" V 4500 2200 30  0001 C CNN "Part"
F 6 "Farnell" V 4550 2200 30  0001 C CNN "Provider"
	1    4300 2200
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56347F54
P 4400 2200
F 0 "R?" V 4450 2350 50  0000 C CNN
F 1 "10K/5%" V 4350 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4550 2200 30  0001 C CNN
F 3 "" V 4480 2200 30  0000 C CNN
F 4 "0603" V 4400 2200 20  0000 C CNN "SMT"
F 5 "9331700" V 4600 2200 30  0001 C CNN "Part"
F 6 "Farnell" V 4650 2200 30  0001 C CNN "Provider"
	1    4400 2200
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56347FA0
P 4500 2200
F 0 "R?" V 4550 2350 50  0000 C CNN
F 1 "10K/5%" V 4450 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4650 2200 30  0001 C CNN
F 3 "" V 4580 2200 30  0000 C CNN
F 4 "0603" V 4500 2200 20  0000 C CNN "SMT"
F 5 "9331700" V 4700 2200 30  0001 C CNN "Part"
F 6 "Farnell" V 4750 2200 30  0001 C CNN "Provider"
	1    4500 2200
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56347FE9
P 4600 2200
F 0 "R?" V 4650 2350 50  0000 C CNN
F 1 "10K/5%" V 4550 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4750 2200 30  0001 C CNN
F 3 "" V 4680 2200 30  0000 C CNN
F 4 "0603" V 4600 2200 20  0000 C CNN "SMT"
F 5 "9331700" V 4800 2200 30  0001 C CNN "Part"
F 6 "Farnell" V 4850 2200 30  0001 C CNN "Provider"
	1    4600 2200
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 5634803D
P 4700 2200
F 0 "R?" V 4750 2350 50  0000 C CNN
F 1 "10K/5%" V 4650 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4850 2200 30  0001 C CNN
F 3 "" V 4780 2200 30  0000 C CNN
F 4 "0603" V 4700 2200 20  0000 C CNN "SMT"
F 5 "9331700" V 4900 2200 30  0001 C CNN "Part"
F 6 "Farnell" V 4950 2200 30  0001 C CNN "Provider"
	1    4700 2200
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56348098
P 4800 2200
F 0 "R?" V 4850 2350 50  0000 C CNN
F 1 "10K/5%" V 4750 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4950 2200 30  0001 C CNN
F 3 "" V 4880 2200 30  0000 C CNN
F 4 "0603" V 4800 2200 20  0000 C CNN "SMT"
F 5 "9331700" V 5000 2200 30  0001 C CNN "Part"
F 6 "Farnell" V 5050 2200 30  0001 C CNN "Provider"
	1    4800 2200
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 563480FA
P 4900 2200
F 0 "R?" V 4950 2350 50  0000 C CNN
F 1 "10K/5%" V 4850 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5050 2200 30  0001 C CNN
F 3 "" V 4980 2200 30  0000 C CNN
F 4 "0603" V 4900 2200 20  0000 C CNN "SMT"
F 5 "9331700" V 5100 2200 30  0001 C CNN "Part"
F 6 "Farnell" V 5150 2200 30  0001 C CNN "Provider"
	1    4900 2200
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56348ED3
P 3950 3850
F 0 "R?" V 3900 4050 50  0000 C CNN
F 1 "22R/5%" V 3900 3600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4100 3850 30  0001 C CNN
F 3 "" V 4030 3850 30  0000 C CNN
F 4 "0603" V 3950 3850 20  0000 C CNN "SMT"
F 5 "9331891" V 4150 3850 30  0001 C CNN "Part"
F 6 "Farnell" V 4200 3850 30  0001 C CNN "Provider"
	1    3950 3850
	0    1    1    0   
$EndComp
$EndSCHEMATC
