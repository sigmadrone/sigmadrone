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
Sheet 13 23
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
L USB_OTG P8
U 1 1 5614C935
P 9650 4750
F 0 "P8" H 9975 4625 50  0000 C CNN
F 1 "USB_OTG" H 9650 4950 50  0000 C CNN
F 2 "" V 9600 4650 60  0001 C CNN
F 3 "" V 9600 4650 60  0000 C CNN
	1    9650 4750
	0    1    1    0   
$EndComp
Text HLabel 1600 4550 0    60   BiDi ~ 0
VBUS_FS
Text HLabel 1600 4650 0    60   BiDi ~ 0
OTG_FS_DM
Text HLabel 1600 4750 0    60   BiDi ~ 0
OTG_FS_DP
Text HLabel 1600 4850 0    60   BiDi ~ 0
OTG_FS_ID
Text HLabel 1600 2600 0    60   Output ~ 0
OTG_FS_OC
Text HLabel 1600 2500 0    60   Input ~ 0
OTG_FS_PSO
$Comp
L VBUS053CZ-HAF-G-08 U10
U 1 1 5614C949
P 7500 3750
F 0 "U10" H 7650 4050 60  0000 C CNN
F 1 "VBUS053CZ-HAF-G-08" H 7500 3450 60  0000 C CNN
F 2 "" H 7450 4450 60  0001 C CNN
F 3 "" H 7450 4450 60  0000 C CNN
	1    7500 3750
	1    0    0    -1  
$EndComp
$Comp
L R R61
U 1 1 5614C950
P 4650 4650
F 0 "R61" V 4600 4800 50  0000 C CNN
F 1 "22" V 4650 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4580 4650 30  0001 C CNN
F 3 "" H 4650 4650 30  0000 C CNN
	1    4650 4650
	0    1    1    0   
$EndComp
$Comp
L R R62
U 1 1 5614C957
P 4650 4750
F 0 "R62" V 4600 4900 50  0000 C CNN
F 1 "22" V 4650 4750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4580 4750 30  0001 C CNN
F 3 "" H 4650 4750 30  0000 C CNN
	1    4650 4750
	0    1    1    0   
$EndComp
$Comp
L +3V3 #PWR087
U 1 1 5614C95E
P 2100 1400
F 0 "#PWR087" H 2100 1250 50  0001 C CNN
F 1 "+3V3" H 2100 1540 50  0000 C CNN
F 2 "" H 2100 1400 60  0000 C CNN
F 3 "" H 2100 1400 60  0000 C CNN
	1    2100 1400
	1    0    0    -1  
$EndComp
$Comp
L R R60
U 1 1 5614C964
P 2100 1950
F 0 "R60" V 2180 1950 50  0000 C CNN
F 1 "10K" V 2100 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2030 1950 30  0001 C CNN
F 3 "" H 2100 1950 30  0000 C CNN
	1    2100 1950
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR088
U 1 1 5614C96B
P 2450 1400
F 0 "#PWR088" H 2450 1250 50  0001 C CNN
F 1 "+3V3" H 2450 1540 50  0000 C CNN
F 2 "" H 2450 1400 60  0000 C CNN
F 3 "" H 2450 1400 60  0000 C CNN
	1    2450 1400
	1    0    0    -1  
$EndComp
$Comp
L R R63
U 1 1 5614C971
P 2450 1950
F 0 "R63" V 2530 1950 50  0000 C CNN
F 1 "47K" V 2450 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2380 1950 30  0001 C CNN
F 3 "" H 2450 1950 30  0000 C CNN
	1    2450 1950
	1    0    0    -1  
$EndComp
$Comp
L LED D13
U 1 1 5614C978
P 2800 2100
F 0 "D13" H 2800 2200 50  0000 C CNN
F 1 "RED" H 2800 2000 50  0000 C CNN
F 2 "" H 2800 2100 60  0001 C CNN
F 3 "" H 2800 2100 60  0000 C CNN
	1    2800 2100
	0    -1   -1   0   
$EndComp
$Comp
L R R64
U 1 1 5614C97F
P 2800 1650
F 0 "R64" V 2880 1650 50  0000 C CNN
F 1 "620" V 2800 1650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2730 1650 30  0001 C CNN
F 3 "" H 2800 1650 30  0000 C CNN
	1    2800 1650
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR089
U 1 1 5614C986
P 2800 1400
F 0 "#PWR089" H 2800 1250 50  0001 C CNN
F 1 "+3V3" H 2800 1540 50  0000 C CNN
F 2 "" H 2800 1400 60  0000 C CNN
F 3 "" H 2800 1400 60  0000 C CNN
	1    2800 1400
	1    0    0    -1  
$EndComp
$Comp
L LED D14
U 1 1 5614C98C
P 7550 1850
F 0 "D14" H 7550 1950 50  0000 C CNN
F 1 "GREEN" H 7550 1750 50  0000 C CNN
F 2 "" H 7550 1850 60  0001 C CNN
F 3 "" H 7550 1850 60  0000 C CNN
	1    7550 1850
	0    -1   -1   0   
$EndComp
$Comp
L R R67
U 1 1 5614C993
P 7550 1450
F 0 "R67" V 7630 1450 50  0000 C CNN
F 1 "620" V 7550 1450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 7480 1450 30  0001 C CNN
F 3 "" H 7550 1450 30  0000 C CNN
	1    7550 1450
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR090
U 1 1 5614C99A
P 7550 1200
F 0 "#PWR090" H 7550 1050 50  0001 C CNN
F 1 "+3V3" H 7550 1340 50  0000 C CNN
F 2 "" H 7550 1200 60  0000 C CNN
F 3 "" H 7550 1200 60  0000 C CNN
	1    7550 1200
	1    0    0    -1  
$EndComp
$Comp
L Q_NMOS_GSD Q7
U 1 1 5614C9A0
P 7450 2400
F 0 "Q7" H 7750 2450 50  0000 R CNN
F 1 "BSS138PW" H 8100 2350 50  0000 R CNN
F 2 "" H 7650 2500 29  0001 C CNN
F 3 "" H 7450 2400 60  0000 C CNN
	1    7450 2400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR091
U 1 1 5614C9A7
P 7550 2850
F 0 "#PWR091" H 7550 2600 50  0001 C CNN
F 1 "GND" H 7550 2700 50  0000 C CNN
F 2 "" H 7550 2850 60  0000 C CNN
F 3 "" H 7550 2850 60  0000 C CNN
	1    7550 2850
	1    0    0    -1  
$EndComp
$Comp
L R R65
U 1 1 5614C9AD
P 6750 2400
F 0 "R65" V 6830 2400 50  0000 C CNN
F 1 "10K" V 6750 2400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6680 2400 30  0001 C CNN
F 3 "" H 6750 2400 30  0000 C CNN
	1    6750 2400
	0    1    1    0   
$EndComp
$Comp
L R R66
U 1 1 5614C9B4
P 7300 2700
F 0 "R66" V 7380 2700 50  0000 C CNN
F 1 "47K" V 7300 2700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 7230 2700 30  0001 C CNN
F 3 "" H 7300 2700 30  0000 C CNN
	1    7300 2700
	0    1    1    0   
$EndComp
$Comp
L C C62
U 1 1 5614C9BB
P 7350 5350
F 0 "C62" H 7375 5450 50  0000 L CNN
F 1 "220u/6V3_T" H 6850 5250 50  0000 L CNN
F 2 "" H 7388 5200 30  0001 C CNN
F 3 "" H 7350 5350 60  0000 C CNN
	1    7350 5350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR092
U 1 1 5614C9C2
P 7350 5550
F 0 "#PWR092" H 7350 5300 50  0001 C CNN
F 1 "GND" H 7350 5400 50  0000 C CNN
F 2 "" H 7350 5550 60  0000 C CNN
F 3 "" H 7350 5550 60  0000 C CNN
	1    7350 5550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR093
U 1 1 5614C9C8
P 4500 2850
F 0 "#PWR093" H 4500 2600 50  0001 C CNN
F 1 "GND" H 4500 2700 50  0000 C CNN
F 2 "" H 4500 2850 60  0000 C CNN
F 3 "" H 4500 2850 60  0000 C CNN
	1    4500 2850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR094
U 1 1 5614C9CE
P 8300 4100
F 0 "#PWR094" H 8300 3850 50  0001 C CNN
F 1 "GND" H 8300 3950 50  0000 C CNN
F 2 "" H 8300 4100 60  0000 C CNN
F 3 "" H 8300 4100 60  0000 C CNN
	1    8300 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 4650 4500 4650
Wire Wire Line
	1600 4750 4500 4750
Wire Wire Line
	4800 4650 6250 4650
Wire Wire Line
	6250 4650 9350 4650
Wire Wire Line
	4800 4750 6350 4750
Wire Wire Line
	6350 4750 9350 4750
Wire Wire Line
	1600 4850 6450 4850
Wire Wire Line
	6450 4850 9350 4850
Wire Wire Line
	1600 2500 2100 2500
Wire Wire Line
	2100 2500 3350 2500
Wire Wire Line
	1600 2600 2450 2600
Wire Wire Line
	2450 2600 2800 2600
Wire Wire Line
	2800 2600 3350 2600
Wire Wire Line
	2100 1400 2100 1800
Wire Wire Line
	2450 1400 2450 1800
Wire Wire Line
	2100 2100 2100 2500
Connection ~ 2100 2500
Wire Wire Line
	2450 2100 2450 2600
Connection ~ 2450 2600
Wire Wire Line
	2800 1400 2800 1500
Wire Wire Line
	2800 1800 2800 1900
Wire Wire Line
	2800 2300 2800 2600
Connection ~ 2800 2600
Wire Wire Line
	7550 1200 7550 1300
Wire Wire Line
	7550 1600 7550 1650
Wire Wire Line
	7550 2050 7550 2200
Wire Wire Line
	7550 2600 7550 2700
Wire Wire Line
	7550 2700 7550 2850
Wire Wire Line
	6900 2400 7000 2400
Wire Wire Line
	7000 2400 7250 2400
Wire Wire Line
	4350 2400 5000 2400
Wire Wire Line
	5000 2400 6600 2400
Wire Wire Line
	7350 5550 7350 5500
Wire Wire Line
	7150 2700 7000 2700
Wire Wire Line
	7000 2700 7000 2400
Connection ~ 7000 2400
Wire Wire Line
	7450 2700 7550 2700
Connection ~ 7550 2700
Wire Wire Line
	8000 3900 8300 3900
Wire Wire Line
	8300 3900 8300 4100
Wire Wire Line
	7000 3900 6450 3900
Wire Wire Line
	6450 3900 6450 4850
Connection ~ 6450 4850
Wire Wire Line
	7000 3800 6350 3800
Wire Wire Line
	6350 3800 6350 4750
Connection ~ 6350 4750
Wire Wire Line
	7000 3700 6250 3700
Wire Wire Line
	6250 3700 6250 4650
Connection ~ 6250 4650
$Comp
L GND #PWR095
U 1 1 5614CA07
P 9250 5400
F 0 "#PWR095" H 9250 5150 50  0001 C CNN
F 1 "GND" H 9250 5250 50  0000 C CNN
F 2 "" H 9250 5400 60  0000 C CNN
F 3 "" H 9250 5400 60  0000 C CNN
	1    9250 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 4950 9250 4950
Wire Wire Line
	9250 4950 9250 5300
Wire Wire Line
	9250 5300 9250 5400
Wire Wire Line
	9750 5150 9750 5300
Wire Wire Line
	9750 5300 9250 5300
Connection ~ 9250 5300
$Comp
L +5V #PWR096
U 1 1 5614CA12
P 3150 1400
F 0 "#PWR096" H 3150 1250 50  0001 C CNN
F 1 "+5V" H 3150 1540 50  0000 C CNN
F 2 "" H 3150 1400 60  0000 C CNN
F 3 "" H 3150 1400 60  0000 C CNN
	1    3150 1400
	1    0    0    -1  
$EndComp
$Comp
L TPS2061CDBVT U9
U 1 1 5614D158
P 3850 2500
F 0 "U9" H 4100 2750 60  0000 C CNN
F 1 "TPS2061CDBVT" H 3850 2250 60  0000 C CNN
F 2 "" H 3850 2300 60  0001 C CNN
F 3 "" H 3850 2300 60  0000 C CNN
	1    3850 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 1400 3150 2400
Wire Wire Line
	3150 2400 3350 2400
Wire Wire Line
	4350 2600 4500 2600
Wire Wire Line
	4500 2600 4500 2850
Wire Wire Line
	7000 3600 6150 3600
Wire Wire Line
	6150 3600 6150 4550
Connection ~ 6150 4550
$Comp
L C C?
U 1 1 5624E1F7
P 7750 5350
F 0 "C?" H 7775 5450 50  0000 L CNN
F 1 "100n/50V" H 7775 5250 50  0000 L CNN
F 2 "" H 7788 5200 30  0001 C CNN
F 3 "" H 7750 5350 60  0000 C CNN
	1    7750 5350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5624E1FD
P 7750 5550
F 0 "#PWR?" H 7750 5300 50  0001 C CNN
F 1 "GND" H 7750 5400 50  0000 C CNN
F 2 "" H 7750 5550 60  0000 C CNN
F 3 "" H 7750 5550 60  0000 C CNN
	1    7750 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 5550 7750 5500
$Comp
L INDUCTOR L?
U 1 1 5624E30E
P 5550 4550
F 0 "L?" V 5500 4550 50  0000 C CNN
F 1 "BLM18EG101TN1D" V 5650 4550 50  0000 C CNN
F 2 "" H 5550 4550 60  0000 C CNN
F 3 "" H 5550 4550 60  0000 C CNN
	1    5550 4550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7350 4550 7350 5200
Wire Wire Line
	7750 4550 7750 5200
Wire Wire Line
	1600 4550 5000 4550
Wire Wire Line
	5000 4550 5250 4550
Wire Wire Line
	5850 4550 6150 4550
Wire Wire Line
	6150 4550 7350 4550
Wire Wire Line
	7350 4550 7750 4550
Wire Wire Line
	7750 4550 9350 4550
Connection ~ 7350 4550
Connection ~ 7750 4550
Wire Wire Line
	5000 2400 5000 4550
Connection ~ 5000 4550
Connection ~ 5000 2400
$EndSCHEMATC
