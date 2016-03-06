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
Sheet 22 28
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 2000 4350 0    60   BiDi ~ 0
OTG_FS_VBUS
Text HLabel 2000 4450 0    60   BiDi ~ 0
OTG_FS_DM
Text HLabel 2000 4550 0    60   BiDi ~ 0
OTG_FS_DP
Text HLabel 2000 4650 0    60   BiDi ~ 0
OTG_FS_ID
Text HLabel 2000 2550 0    60   Output ~ 0
OTG_FS_OC
Text HLabel 2000 2450 0    60   Input ~ 0
OTG_FS_PSO
$Comp
L VBUS053CZ-HAF-G-08 U20
U 1 1 5614ED91
P 5300 3650
F 0 "U20" H 5450 3950 60  0000 C CNN
F 1 "VBUS053CZ-HAF-G-08" H 5300 3350 60  0000 C CNN
F 2 "Sigmadrone:VBUS053CZ-HAF" H 5250 4350 60  0001 C CNN
F 3 "" H 5250 4350 60  0000 C CNN
	1    5300 3650
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR0223
U 1 1 5614EDA6
P 3600 1350
F 0 "#PWR0223" H 3600 1200 50  0001 C CNN
F 1 "+3V3" H 3600 1490 50  0000 C CNN
F 2 "" H 3600 1350 60  0000 C CNN
F 3 "" H 3600 1350 60  0000 C CNN
	1    3600 1350
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR0224
U 1 1 5614EDB3
P 3950 1350
F 0 "#PWR0224" H 3950 1200 50  0001 C CNN
F 1 "+3V3" H 3950 1490 50  0000 C CNN
F 2 "" H 3950 1350 60  0000 C CNN
F 3 "" H 3950 1350 60  0000 C CNN
	1    3950 1350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0225
U 1 1 5614EE0A
P 7350 3250
F 0 "#PWR0225" H 7350 3000 50  0001 C CNN
F 1 "GND" H 7350 3100 50  0000 C CNN
F 2 "" H 7350 3250 60  0000 C CNN
F 3 "" H 7350 3250 60  0000 C CNN
	1    7350 3250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0226
U 1 1 5614EE10
P 6100 2850
F 0 "#PWR0226" H 6100 2600 50  0001 C CNN
F 1 "GND" H 6100 2700 50  0000 C CNN
F 2 "" H 6100 2850 60  0000 C CNN
F 3 "" H 6100 2850 60  0000 C CNN
	1    6100 2850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0227
U 1 1 5614EE16
P 6100 4100
F 0 "#PWR0227" H 6100 3850 50  0001 C CNN
F 1 "GND" H 6100 3950 50  0000 C CNN
F 2 "" H 6100 4100 60  0000 C CNN
F 3 "" H 6100 4100 60  0000 C CNN
	1    6100 4100
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR0228
U 1 1 5614EE65
P 4300 1350
F 0 "#PWR0228" H 4300 1200 50  0001 C CNN
F 1 "+5V" H 4300 1490 50  0000 C CNN
F 2 "" H 4300 1350 60  0000 C CNN
F 3 "" H 4300 1350 60  0000 C CNN
	1    4300 1350
	1    0    0    -1  
$EndComp
$Comp
L TPS2061CDBVT U19
U 1 1 5614EE6B
P 5300 2450
F 0 "U19" H 5550 2700 60  0000 C CNN
F 1 "TPS2061CDBVT" H 5300 2200 60  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 5300 2250 60  0001 C CNN
F 3 "" H 5300 2250 60  0000 C CNN
	1    5300 2450
	1    0    0    -1  
$EndComp
NoConn ~ 5800 3600
NoConn ~ 5800 3700
$Comp
L R_SMT R167
U 1 1 562F624B
P 3600 1600
F 0 "R167" V 3670 1600 50  0000 C CNN
F 1 "10K/5%" V 3530 1600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3750 1600 30  0001 C CNN
F 3 "" V 3680 1600 30  0000 C CNN
F 4 "0603" V 3600 1600 20  0000 C CNN "SMT"
F 5 "9331700" V 3800 1600 30  0001 C CNN "Part"
F 6 "Farnell" V 3850 1600 30  0001 C CNN "Provider"
	1    3600 1600
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R168
U 1 1 562F6341
P 3950 1600
F 0 "R168" V 4020 1600 50  0000 C CNN
F 1 "10K/5%" V 3880 1600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4100 1600 30  0001 C CNN
F 3 "" V 4030 1600 30  0000 C CNN
F 4 "0603" V 3950 1600 20  0000 C CNN "SMT"
F 5 "9331700" V 4150 1600 30  0001 C CNN "Part"
F 6 "Farnell" V 4200 1600 30  0001 C CNN "Provider"
	1    3950 1600
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C72
U 1 1 562F7433
P 7350 2600
F 0 "C72" H 7375 2700 50  0000 L CNN
F 1 "10uF/16V/X5R" V 7200 2250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7380 2450 30  0001 L CNN
F 3 "" H 7375 2700 60  0000 L CNN
F 4 "0805" H 7350 2600 20  0000 C CNN "SMT"
F 5 "1762635" H 7380 2410 30  0001 L CNN "Part"
F 6 "Farnell" H 7380 2370 30  0001 L CNN "Provider"
	1    7350 2600
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C73
U 1 1 562F83CF
P 7850 2600
F 0 "C73" H 7875 2700 50  0000 L CNN
F 1 "100nF/50V/X7R" V 7700 2200 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7880 2450 30  0001 L CNN
F 3 "" H 7875 2700 60  0000 L CNN
F 4 "0603" H 7850 2600 20  0000 C CNN "SMT"
F 5 "1414028" H 7880 2410 30  0001 L CNN "Part"
F 6 "Farnell" H 7880 2370 30  0001 L CNN "Provider"
	1    7850 2600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0229
U 1 1 562F853E
P 7850 3250
F 0 "#PWR0229" H 7850 3000 50  0001 C CNN
F 1 "GND" H 7850 3100 50  0000 C CNN
F 2 "" H 7850 3250 60  0000 C CNN
F 3 "" H 7850 3250 60  0000 C CNN
	1    7850 3250
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R165
U 1 1 562F16AC
P 3250 4450
F 0 "R165" V 3200 4650 50  0000 C CNN
F 1 "22R/5%" V 3200 4200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3400 4450 30  0001 C CNN
F 3 "" V 3330 4450 30  0000 C CNN
F 4 "0603" V 3250 4450 20  0000 C CNN "SMT"
F 5 "9331891" V 3450 4450 30  0001 C CNN "Part"
F 6 "Farnell" V 3500 4450 30  0001 C CNN "Provider"
	1    3250 4450
	0    1    1    0   
$EndComp
$Comp
L R_SMT R166
U 1 1 562F179F
P 3250 4550
F 0 "R166" V 3200 4750 50  0000 C CNN
F 1 "22R/5%" V 3200 4300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3400 4550 30  0001 C CNN
F 3 "" V 3330 4550 30  0000 C CNN
F 4 "0603" V 3250 4550 20  0000 C CNN "SMT"
F 5 "9331891" V 3450 4550 30  0001 C CNN "Part"
F 6 "Farnell" V 3500 4550 30  0001 C CNN "Provider"
	1    3250 4550
	0    1    1    0   
$EndComp
$Comp
L R_SMT R169
U 1 1 562F238F
P 8300 2600
F 0 "R169" V 8370 2600 50  0000 C CNN
F 1 "1k2/5%" V 8230 2600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8450 2600 30  0001 C CNN
F 3 "" V 8380 2600 30  0000 C CNN
F 4 "0603" V 8300 2600 20  0000 C CNN "SMT"
F 5 "9331751" V 8500 2600 30  0001 C CNN "Part"
F 6 "Farnell" V 8550 2600 30  0001 C CNN "Provider"
	1    8300 2600
	-1   0    0    1   
$EndComp
$Comp
L LED_SMT DL13
U 1 1 563227CA
P 8300 3000
F 0 "DL13" H 8300 3100 50  0000 C CNN
F 1 "GREEN" H 8300 2900 50  0000 C CNN
F 2 "LEDs:LED_0603" H 8300 2850 30  0001 C CNN
F 3 "" H 8300 3000 60  0000 C CNN
F 4 "0603" H 8185 2980 30  0000 C CNN "SMT"
F 5 "2322070" H 8300 2800 30  0001 C CNN "Part"
F 6 "Farnell" H 8300 2750 30  0001 C CNN "Provider"
	1    8300 3000
	0    -1   -1   0   
$EndComp
$Comp
L FB_SMT FB5
U 1 1 56323B2F
P 6550 2350
F 0 "FB5" H 6550 2450 50  0000 C CNN
F 1 "BEAD/0.045OHM/2A" H 6550 2300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" H 6550 2250 30  0001 C CNN
F 3 "" H 6600 2350 60  0000 C CNN
F 4 "0603" H 6550 2350 25  0000 C CNN "SMT"
F 5 "1515714" H 6550 2200 30  0001 C CNN "Part"
F 6 "Farnell" H 6550 2150 30  0001 C CNN "Provider"
	1    6550 2350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0230
U 1 1 56354FBA
P 8300 3250
F 0 "#PWR0230" H 8300 3000 50  0001 C CNN
F 1 "GND" H 8300 3100 50  0000 C CNN
F 2 "" H 8300 3250 60  0000 C CNN
F 3 "" H 8300 3250 60  0000 C CNN
	1    8300 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 4450 3100 4450
Wire Wire Line
	2000 4550 3100 4550
Wire Wire Line
	3400 4450 9450 4450
Wire Wire Line
	3400 4550 9450 4550
Wire Wire Line
	2000 4350 9450 4350
Wire Wire Line
	2000 4650 9450 4650
Wire Wire Line
	2000 2450 4800 2450
Wire Wire Line
	2000 2550 4800 2550
Wire Wire Line
	3600 1350 3600 1450
Wire Wire Line
	3950 1350 3950 1450
Wire Wire Line
	3600 1750 3600 2450
Connection ~ 3600 2450
Wire Wire Line
	3950 1750 3950 2550
Connection ~ 3950 2550
Wire Wire Line
	8300 2750 8300 2800
Wire Wire Line
	8300 3200 8300 3250
Wire Wire Line
	6750 2350 8750 2350
Wire Wire Line
	7350 3250 7350 2750
Wire Wire Line
	7350 1950 7350 2450
Connection ~ 7350 2350
Wire Wire Line
	5800 3800 6100 3800
Wire Wire Line
	6100 3800 6100 4100
Wire Wire Line
	4800 3800 4250 3800
Wire Wire Line
	4250 3800 4250 4650
Connection ~ 4250 4650
Wire Wire Line
	4800 3700 4150 3700
Wire Wire Line
	4150 3700 4150 4550
Connection ~ 4150 4550
Wire Wire Line
	4800 3600 4050 3600
Wire Wire Line
	4050 3600 4050 4450
Connection ~ 4050 4450
Wire Wire Line
	4300 1350 4300 2350
Wire Wire Line
	4300 2350 4800 2350
Wire Wire Line
	5800 2550 6100 2550
Wire Wire Line
	6100 2550 6100 2850
Wire Wire Line
	4800 3500 3950 3500
Wire Wire Line
	3950 3500 3950 4350
Connection ~ 3950 4350
Wire Wire Line
	7850 3250 7850 2750
Wire Wire Line
	7850 2450 7850 2350
Connection ~ 7850 2350
Wire Wire Line
	5800 2350 6350 2350
Wire Wire Line
	8300 2350 8300 2450
Wire Wire Line
	8750 2350 8750 4350
Connection ~ 8750 4350
Connection ~ 8300 2350
$Comp
L GND #PWR0231
U 1 1 563559F0
P 2250 5600
F 0 "#PWR0231" H 2250 5350 50  0001 C CNN
F 1 "GND" H 2250 5450 50  0000 C CNN
F 2 "" H 2250 5600 60  0000 C CNN
F 3 "" H 2250 5600 60  0000 C CNN
	1    2250 5600
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C70
U 1 1 563559F9
P 2250 5150
F 0 "C70" H 2275 5250 50  0000 L CNN
F 1 "47pF/100V/C0G" V 2100 4800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2280 5000 30  0001 L CNN
F 3 "" H 2275 5250 60  0000 L CNN
F 4 "0603" H 2250 5150 20  0000 C CNN "SMT"
F 5 "1813433" H 2280 4960 30  0001 L CNN "Part"
F 6 "Farnell" H 2280 4920 30  0001 L CNN "Provider"
	1    2250 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 5600 2250 5300
$Comp
L GND #PWR0232
U 1 1 56355B50
P 2650 5600
F 0 "#PWR0232" H 2650 5350 50  0001 C CNN
F 1 "GND" H 2650 5450 50  0000 C CNN
F 2 "" H 2650 5600 60  0000 C CNN
F 3 "" H 2650 5600 60  0000 C CNN
	1    2650 5600
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C71
U 1 1 56355B59
P 2650 5150
F 0 "C71" H 2675 5250 50  0000 L CNN
F 1 "47pF/100V/C0G" V 2500 4800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2680 5000 30  0001 L CNN
F 3 "" H 2675 5250 60  0000 L CNN
F 4 "0603" H 2650 5150 20  0000 C CNN "SMT"
F 5 "1813433" H 2680 4960 30  0001 L CNN "Part"
F 6 "Farnell" H 2680 4920 30  0001 L CNN "Provider"
	1    2650 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 5600 2650 5300
Wire Wire Line
	2250 5000 2250 4550
Connection ~ 2250 4550
Wire Wire Line
	2650 5000 2650 4450
Connection ~ 2650 4450
$Comp
L GND #PWR0233
U 1 1 5635D0BD
P 9300 5600
F 0 "#PWR0233" H 9300 5350 50  0001 C CNN
F 1 "GND" H 9300 5450 50  0000 C CNN
F 2 "" H 9300 5600 60  0000 C CNN
F 3 "" H 9300 5600 60  0000 C CNN
	1    9300 5600
	1    0    0    -1  
$EndComp
$Comp
L 629_105_150_921 CN8
U 1 1 5635D0C3
P 9850 4850
F 0 "CN8" H 10000 5500 60  0000 C CNN
F 1 "629_105_150_921" V 10100 4700 60  0000 C CNN
F 2 "Sigmadrone:WURTH_629105150921" H 11400 6150 60  0001 C CNN
F 3 "" H 11400 6150 60  0000 C CNN
	1    9850 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 4750 9300 4750
Wire Wire Line
	9300 4750 9300 5600
Wire Wire Line
	9450 4850 9300 4850
Connection ~ 9300 4850
Connection ~ 9300 4950
Connection ~ 9300 5050
Connection ~ 9300 5150
Connection ~ 9300 5250
Connection ~ 9300 5350
Wire Wire Line
	9450 4950 9300 4950
Wire Wire Line
	9450 5050 9300 5050
Wire Wire Line
	9450 5150 9300 5150
Wire Wire Line
	9450 5250 9300 5250
Wire Wire Line
	9450 5350 9300 5350
$Comp
L PWR_FLAG #FLG0234
U 1 1 56CD0D04
P 7350 1950
F 0 "#FLG0234" H 7350 2045 50  0001 C CNN
F 1 "PWR_FLAG" H 7350 2174 50  0000 C CNN
F 2 "" H 7350 1950 50  0000 C CNN
F 3 "" H 7350 1950 50  0000 C CNN
	1    7350 1950
	1    0    0    -1  
$EndComp
$EndSCHEMATC
