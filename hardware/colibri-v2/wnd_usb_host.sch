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
Sheet 15 28
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
L VBUS053CZ-HAF-G-08 U9
U 1 1 5619F269
P 3850 5150
F 0 "U9" H 4000 5450 60  0000 C CNN
F 1 "VBUS053CZ-HAF-G-08" H 3850 4850 60  0000 C CNN
F 2 "Sigmadrone:VBUS053CZ-HAF" H 3800 5850 60  0001 C CNN
F 3 "" H 3800 5850 60  0000 C CNN
	1    3850 5150
	1    0    0    -1  
$EndComp
$Comp
L 692_121_030_100 CN5
U 1 1 561A064E
P 9850 3950
F 0 "CN5" H 10100 4700 60  0000 C CNN
F 1 "692_121_030_100" H 9850 3200 60  0000 C CNN
F 2 "Sigmadrone:692121030100" H 9850 3950 60  0001 C CNN
F 3 "" H 9850 3950 60  0000 C CNN
	1    9850 3950
	1    0    0    -1  
$EndComp
Text HLabel 1900 2000 0    60   Output ~ 0
USB_HOST_OC
$Comp
L +3V3 #PWR0136
U 1 1 561A1B20
P 3150 1000
F 0 "#PWR0136" H 3150 850 50  0001 C CNN
F 1 "+3V3" H 3150 1140 50  0000 C CNN
F 2 "" H 3150 1000 60  0000 C CNN
F 3 "" H 3150 1000 60  0000 C CNN
	1    3150 1000
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR0137
U 1 1 561A1B3B
P 2750 1000
F 0 "#PWR0137" H 2750 850 50  0001 C CNN
F 1 "+3V3" H 2750 1140 50  0000 C CNN
F 2 "" H 2750 1000 60  0000 C CNN
F 3 "" H 2750 1000 60  0000 C CNN
	1    2750 1000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0138
U 1 1 561A1B48
P 7500 2700
F 0 "#PWR0138" H 7500 2450 50  0001 C CNN
F 1 "GND" H 7500 2550 50  0000 C CNN
F 2 "" H 7500 2700 60  0000 C CNN
F 3 "" H 7500 2700 60  0000 C CNN
	1    7500 2700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0139
U 1 1 561A1B4E
P 5300 2700
F 0 "#PWR0139" H 5300 2450 50  0001 C CNN
F 1 "GND" H 5300 2550 50  0000 C CNN
F 2 "" H 5300 2700 60  0000 C CNN
F 3 "" H 5300 2700 60  0000 C CNN
	1    5300 2700
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR0140
U 1 1 561A1B64
P 3900 1000
F 0 "#PWR0140" H 3900 850 50  0001 C CNN
F 1 "+5V" H 3900 1140 50  0000 C CNN
F 2 "" H 3900 1000 60  0000 C CNN
F 3 "" H 3900 1000 60  0000 C CNN
	1    3900 1000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0141
U 1 1 561A2244
P 7900 2700
F 0 "#PWR0141" H 7900 2450 50  0001 C CNN
F 1 "GND" H 7900 2550 50  0000 C CNN
F 2 "" H 7900 2700 60  0000 C CNN
F 3 "" H 7900 2700 60  0000 C CNN
	1    7900 2700
	1    0    0    -1  
$EndComp
Text HLabel 1900 3550 0    60   BiDi ~ 0
USB_HOST_DM
Text HLabel 1900 3650 0    60   BiDi ~ 0
USB_HOST_DP
Text HLabel 1900 3750 0    60   BiDi ~ 0
StdA_SSRX-
Text HLabel 1900 3850 0    60   BiDi ~ 0
StdA_SSRX+
Text HLabel 1900 3950 0    60   BiDi ~ 0
GND_DRAIN
Text HLabel 1900 4050 0    60   BiDi ~ 0
StdA_SSTX-
Text HLabel 1900 4150 0    60   BiDi ~ 0
StdA_SSTX+
$Comp
L GND #PWR0142
U 1 1 561A3388
P 8300 6100
F 0 "#PWR0142" H 8300 5850 50  0001 C CNN
F 1 "GND" H 8300 5950 50  0000 C CNN
F 2 "" H 8300 6100 60  0000 C CNN
F 3 "" H 8300 6100 60  0000 C CNN
	1    8300 6100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0143
U 1 1 561A33B7
P 7500 6100
F 0 "#PWR0143" H 7500 5850 50  0001 C CNN
F 1 "GND" H 7500 5950 50  0000 C CNN
F 2 "" H 7500 6100 60  0000 C CNN
F 3 "" H 7500 6100 60  0000 C CNN
	1    7500 6100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0144
U 1 1 561A3438
P 5300 6100
F 0 "#PWR0144" H 5300 5850 50  0001 C CNN
F 1 "GND" H 5300 5950 50  0000 C CNN
F 2 "" H 5300 6100 60  0000 C CNN
F 3 "" H 5300 6100 60  0000 C CNN
	1    5300 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 2000 4100 2000
Wire Wire Line
	3150 1000 3150 1050
Wire Wire Line
	3150 1350 3150 2000
Connection ~ 3150 2000
Wire Wire Line
	2750 1000 2750 1050
Wire Wire Line
	2750 1350 2750 1400
Wire Wire Line
	2750 1800 2750 2000
Connection ~ 2750 2000
Wire Wire Line
	7500 2700 7500 2200
Wire Wire Line
	7500 1900 7500 1800
Connection ~ 7500 1800
Wire Wire Line
	3900 1000 3900 1800
Wire Wire Line
	3900 1800 4100 1800
Wire Wire Line
	5100 2000 5300 2000
Wire Wire Line
	5300 2000 5300 2700
Wire Wire Line
	7900 2700 7900 2200
Wire Wire Line
	7900 1800 7900 1900
Connection ~ 7900 1800
Wire Wire Line
	5100 1800 6100 1800
Wire Wire Line
	6500 1800 9100 1800
Wire Wire Line
	9100 1800 9100 3350
Wire Wire Line
	3200 3350 9300 3350
Wire Wire Line
	1900 3550 9300 3550
Wire Wire Line
	1900 3650 9300 3650
Wire Wire Line
	1900 3750 9300 3750
Wire Wire Line
	1900 3850 9300 3850
Wire Wire Line
	1900 3950 9300 3950
Wire Wire Line
	1900 4050 9300 4050
Wire Wire Line
	1900 4150 9300 4150
Wire Wire Line
	6900 5500 7500 5500
Wire Wire Line
	7500 5500 7500 6100
Wire Wire Line
	5900 5500 5300 5500
Wire Wire Line
	5300 5500 5300 6100
Wire Wire Line
	8300 4950 8300 6100
Wire Wire Line
	5900 5100 5600 5100
Wire Wire Line
	5600 5100 5600 4150
Connection ~ 5600 4150
Wire Wire Line
	5900 5200 5500 5200
Wire Wire Line
	5500 5200 5500 4050
Connection ~ 5500 4050
Wire Wire Line
	5900 5300 5400 5300
Wire Wire Line
	5400 5300 5400 3850
Connection ~ 5400 3850
Wire Wire Line
	5900 5400 5300 5400
Wire Wire Line
	5300 5400 5300 3750
Connection ~ 5300 3750
Wire Wire Line
	6900 5100 7200 5100
Wire Wire Line
	7200 5100 7200 4150
Connection ~ 7200 4150
Wire Wire Line
	6900 5200 7300 5200
Wire Wire Line
	7300 5200 7300 4050
Connection ~ 7300 4050
Wire Wire Line
	6900 5300 7400 5300
Wire Wire Line
	7400 5300 7400 3850
Connection ~ 7400 3850
Wire Wire Line
	6900 5400 7500 5400
Wire Wire Line
	7500 5400 7500 3750
Connection ~ 7500 3750
Wire Wire Line
	3350 5100 3100 5100
Wire Wire Line
	3100 5100 3100 3550
Connection ~ 3100 3550
Wire Wire Line
	3350 5200 3000 5200
Wire Wire Line
	3000 5200 3000 3650
Connection ~ 3000 3650
Connection ~ 9100 3350
Wire Wire Line
	3350 5000 3200 5000
Wire Wire Line
	3200 5000 3200 3350
$Comp
L GND #PWR0145
U 1 1 561A6CE3
P 4500 6100
F 0 "#PWR0145" H 4500 5850 50  0001 C CNN
F 1 "GND" H 4500 5950 50  0000 C CNN
F 2 "" H 4500 6100 60  0000 C CNN
F 3 "" H 4500 6100 60  0000 C CNN
	1    4500 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 5300 4500 5300
Wire Wire Line
	4500 5300 4500 6100
NoConn ~ 4350 5200
NoConn ~ 4350 5100
NoConn ~ 3350 5300
$Comp
L GND #PWR0146
U 1 1 561A74D9
P 9100 6100
F 0 "#PWR0146" H 9100 5850 50  0001 C CNN
F 1 "GND" H 9100 5950 50  0000 C CNN
F 2 "" H 9100 6100 60  0000 C CNN
F 3 "" H 9100 6100 60  0000 C CNN
	1    9100 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 4550 8300 3950
Connection ~ 8300 3950
Wire Wire Line
	9300 4250 9100 4250
Wire Wire Line
	9100 4250 9100 6100
Wire Wire Line
	9300 4450 9100 4450
Connection ~ 9100 4450
Wire Wire Line
	9300 4550 9100 4550
Connection ~ 9100 4550
$Comp
L R_SMT R93
U 1 1 563513C2
P 2750 1200
F 0 "R93" H 2800 1350 50  0000 C CNN
F 1 "620R/5%" V 2680 1200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2900 1200 30  0001 C CNN
F 3 "" V 2830 1200 30  0000 C CNN
F 4 "0603" V 2750 1200 20  0000 C CNN "SMT"
F 5 "2141352" V 2950 1200 30  0001 C CNN "Part"
F 6 "Farnell" V 3000 1200 30  0001 C CNN "Provider"
	1    2750 1200
	1    0    0    -1  
$EndComp
$Comp
L LED_SMT DL8
U 1 1 563518B5
P 2750 1600
F 0 "DL8" V 2850 1500 50  0000 C CNN
F 1 "RED" H 2700 1700 50  0000 C CNN
F 2 "LEDs:LED_0603" H 2750 1450 30  0001 C CNN
F 3 "" H 2750 1600 60  0000 C CNN
F 4 "0603" H 2635 1580 30  0000 C CNN "SMT"
F 5 "2322071" H 2750 1400 30  0001 C CNN "Part"
F 6 "Farnell" H 2750 1350 30  0001 C CNN "Provider"
	1    2750 1600
	0    -1   -1   0   
$EndComp
$Comp
L R_SMT R95
U 1 1 563526BF
P 3550 1200
F 0 "R95" H 3600 1350 50  0000 C CNN
F 1 "10K/5%" V 3450 1200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3700 1200 30  0001 C CNN
F 3 "" V 3630 1200 30  0000 C CNN
F 4 "0603" V 3550 1200 20  0000 C CNN "SMT"
F 5 "9331700" V 3750 1200 30  0001 C CNN "Part"
F 6 "Farnell" V 3800 1200 30  0001 C CNN "Provider"
	1    3550 1200
	1    0    0    -1  
$EndComp
$Comp
L CP_SMT C44
U 1 1 5635343E
P 7500 2050
F 0 "C44" H 7525 2150 50  0000 L CNN
F 1 "220uF/10%/6V3" V 7350 1650 50  0000 L CNN
F 2 "Capacitors_Tantalum_SMD:TantalC_SizeC_EIA-6032_Reflow" H 7538 1900 30  0001 C CNN
F 3 "" H 7500 2050 60  0000 C CNN
F 4 "2312" H 7500 2050 25  0000 C CNN "SMT"
F 5 "1462420" H 7500 1850 30  0001 C CNN "Part"
F 6 "Farnell" H 7500 1800 30  0001 C CNN "Provider"
	1    7500 2050
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C45
U 1 1 563526B5
P 7900 2050
F 0 "C45" H 7925 2150 50  0000 L CNN
F 1 "100nF/50V/X7R" V 7750 1650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7930 1900 30  0001 L CNN
F 3 "" H 7925 2150 60  0000 L CNN
F 4 "0603" H 7900 2050 20  0000 C CNN "SMT"
F 5 "1414028" H 7930 1860 30  0001 L CNN "Part"
F 6 "Farnell" H 7930 1820 30  0001 L CNN "Provider"
	1    7900 2050
	1    0    0    -1  
$EndComp
$Comp
L FB_SMT FB3
U 1 1 56353A77
P 6300 1800
F 0 "FB3" H 6300 1900 50  0000 C CNN
F 1 "BEAD/0.045OHM/2A" H 6300 1750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" H 6300 1700 30  0001 C CNN
F 3 "" H 6350 1800 60  0000 C CNN
F 4 "0603" H 6300 1800 25  0000 C CNN "SMT"
F 5 "1515714" H 6300 1650 30  0001 C CNN "Part"
F 6 "Farnell" H 6300 1600 30  0001 C CNN "Provider"
	1    6300 1800
	1    0    0    -1  
$EndComp
$Comp
L FB_SMT FB4
U 1 1 56354381
P 8300 4750
F 0 "FB4" H 8300 4850 50  0000 C CNN
F 1 "BEAD/0.045OHM/2A" H 8300 4700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" H 8300 4650 30  0001 C CNN
F 3 "" H 8350 4750 60  0000 C CNN
F 4 "0603" H 8300 4750 25  0000 C CNN "SMT"
F 5 "1515714" H 8300 4600 30  0001 C CNN "Part"
F 6 "Farnell" H 8300 4550 30  0001 C CNN "Provider"
	1    8300 4750
	0    1    1    0   
$EndComp
$Comp
L TPS2065CDBVT U10
U 1 1 563650DC
P 4600 1900
F 0 "U10" H 4850 2150 60  0000 C CNN
F 1 "TPS2065CDBVT" H 4600 1650 60  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 4600 1700 60  0001 C CNN
F 3 "" H 4600 1700 60  0000 C CNN
	1    4600 1900
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR0147
U 1 1 56365222
P 3550 1000
F 0 "#PWR0147" H 3550 850 50  0001 C CNN
F 1 "+5V" H 3550 1140 50  0000 C CNN
F 2 "" H 3550 1000 60  0000 C CNN
F 3 "" H 3550 1000 60  0000 C CNN
	1    3550 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 1000 3550 1050
Wire Wire Line
	3550 1350 3550 1900
Wire Wire Line
	3550 1900 4100 1900
$Comp
L R_SMT R94
U 1 1 563654E4
P 3150 1200
F 0 "R94" H 3200 1350 50  0000 C CNN
F 1 "10K/5%" V 3050 1200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3300 1200 30  0001 C CNN
F 3 "" V 3230 1200 30  0000 C CNN
F 4 "0603" V 3150 1200 20  0000 C CNN "SMT"
F 5 "9331700" V 3350 1200 30  0001 C CNN "Part"
F 6 "Farnell" V 3400 1200 30  0001 C CNN "Provider"
	1    3150 1200
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R96
U 1 1 56366BD8
P 8300 2050
F 0 "R96" V 8370 2050 50  0000 C CNN
F 1 "1k2/5%" V 8230 2050 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8450 2050 30  0001 C CNN
F 3 "" V 8380 2050 30  0000 C CNN
F 4 "0603" V 8300 2050 20  0000 C CNN "SMT"
F 5 "9331751" V 8500 2050 30  0001 C CNN "Part"
F 6 "Farnell" V 8550 2050 30  0001 C CNN "Provider"
	1    8300 2050
	-1   0    0    1   
$EndComp
$Comp
L LED_SMT DL9
U 1 1 56366BE2
P 8300 2450
F 0 "DL9" H 8300 2550 50  0000 C CNN
F 1 "GREEN" H 8300 2350 50  0000 C CNN
F 2 "LEDs:LED_0603" H 8300 2300 30  0001 C CNN
F 3 "" H 8300 2450 60  0000 C CNN
F 4 "0603" H 8185 2430 30  0000 C CNN "SMT"
F 5 "2322070" H 8300 2250 30  0001 C CNN "Part"
F 6 "Farnell" H 8300 2200 30  0001 C CNN "Provider"
	1    8300 2450
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR0148
U 1 1 56366BE9
P 8300 2700
F 0 "#PWR0148" H 8300 2450 50  0001 C CNN
F 1 "GND" H 8300 2550 50  0000 C CNN
F 2 "" H 8300 2700 60  0000 C CNN
F 3 "" H 8300 2700 60  0000 C CNN
	1    8300 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 2200 8300 2250
Wire Wire Line
	8300 2650 8300 2700
Wire Wire Line
	8300 1800 8300 1900
Connection ~ 8300 1800
$Comp
L PWR_FLAG #FLG0149
U 1 1 56CB7F4B
P 6950 1550
F 0 "#FLG0149" H 6950 1645 50  0001 C CNN
F 1 "PWR_FLAG" H 6950 1774 50  0000 C CNN
F 2 "" H 6950 1550 50  0000 C CNN
F 3 "" H 6950 1550 50  0000 C CNN
	1    6950 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 1550 6950 1800
Connection ~ 6950 1800
$Comp
L VBUS054CD-FHI-GS08 U11
U 1 1 56D3FA39
P 6400 5300
F 0 "U11" H 6600 5650 60  0000 C CNN
F 1 "VBUS054CD-FHI-GS08" H 6400 4850 60  0000 C CNN
F 2 "Sigmadrone:VBUS054CD-FHI" H 6350 6000 60  0001 C CNN
F 3 "" H 6350 6000 60  0000 C CNN
	1    6400 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 5600 7500 5600
Connection ~ 7500 5600
$EndSCHEMATC
