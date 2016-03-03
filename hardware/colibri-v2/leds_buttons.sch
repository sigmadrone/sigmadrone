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
Sheet 3 28
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 1350 3150 0    60   Output ~ 0
BUTTON_0
Text HLabel 1350 4900 0    60   Output ~ 0
BUTTON_1
Text HLabel 1350 6600 0    60   Output ~ 0
BUTTON_2
Text HLabel 4450 4900 0    60   Output ~ 0
BUTTON_3
Text HLabel 4450 6600 0    60   Output ~ 0
BUTTON_4
Text HLabel 1350 1300 0    60   Output ~ 0
RESET#
Text HLabel 8650 1300 0    60   Input ~ 0
LED1
Text HLabel 8650 1900 0    60   Input ~ 0
LED3
Text HLabel 8650 1600 0    60   Input ~ 0
LED2
Text HLabel 8650 2200 0    60   Input ~ 0
LED4
$Comp
L GND #PWR10
U 1 1 56074E25
P 3350 1850
F 0 "#PWR10" H 3350 1600 50  0001 C CNN
F 1 "GND" H 3350 1700 50  0000 C CNN
F 2 "" H 3350 1850 60  0000 C CNN
F 3 "" H 3350 1850 60  0000 C CNN
	1    3350 1850
	1    0    0    -1  
$EndComp
Text HLabel 4500 3150 0    60   Output ~ 0
BOOT0
$Comp
L GND #PWR21
U 1 1 5623679B
P 10550 2500
F 0 "#PWR21" H 10550 2250 50  0001 C CNN
F 1 "GND" H 10550 2350 50  0000 C CNN
F 2 "" H 10550 2500 60  0000 C CNN
F 3 "" H 10550 2500 60  0000 C CNN
	1    10550 2500
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR18
U 1 1 5623E09E
P 6450 2500
F 0 "#PWR18" H 6450 2350 50  0001 C CNN
F 1 "+3V3" H 6450 2640 50  0000 C CNN
F 2 "" H 6450 2500 60  0000 C CNN
F 3 "" H 6450 2500 60  0000 C CNN
	1    6450 2500
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R12
U 1 1 56287BD9
P 2100 1300
F 0 "R12" V 2170 1300 50  0000 C CNN
F 1 "100R/5%" V 2030 1300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2250 1300 30  0001 C CNN
F 3 "" V 2180 1300 30  0000 C CNN
F 4 "0603" V 2100 1300 20  0000 C CNN "SMT"
F 5 "9331689" V 2300 1300 30  0001 C CNN "Part"
F 6 "Farnell" V 2350 1300 30  0001 C CNN "Provider"
	1    2100 1300
	0    1    1    0   
$EndComp
$Comp
L GND #PWR8
U 1 1 56289795
P 2150 5450
F 0 "#PWR8" H 2150 5200 50  0001 C CNN
F 1 "GND" H 2150 5300 50  0000 C CNN
F 2 "" H 2150 5450 60  0000 C CNN
F 3 "" H 2150 5450 60  0000 C CNN
	1    2150 5450
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR12
U 1 1 562897A6
P 3350 4250
F 0 "#PWR12" H 3350 4100 50  0001 C CNN
F 1 "+3V3" H 3350 4390 50  0000 C CNN
F 2 "" H 3350 4250 60  0000 C CNN
F 3 "" H 3350 4250 60  0000 C CNN
	1    3350 4250
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R17
U 1 1 562897B5
P 2900 4400
F 0 "R17" V 2970 4400 50  0000 C CNN
F 1 "100R/5%" V 2830 4400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3050 4400 30  0001 C CNN
F 3 "" V 2980 4400 30  0000 C CNN
F 4 "0603" V 2900 4400 20  0000 C CNN "SMT"
F 5 "9331689" V 3100 4400 30  0001 C CNN "Part"
F 6 "Farnell" V 3150 4400 30  0001 C CNN "Provider"
	1    2900 4400
	0    1    1    0   
$EndComp
$Comp
L C_SMT C2
U 1 1 562897BE
P 2150 4650
F 0 "C2" H 2175 4750 50  0000 L CNN
F 1 "100nF/50V" H 2175 4550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2180 4500 30  0001 L CNN
F 3 "" H 2175 4750 60  0000 L CNN
F 4 "0603" H 2150 4650 20  0000 C CNN "SMT"
F 5 "1414028" H 2180 4460 30  0001 L CNN "Part"
F 6 "Farnell" H 2180 4420 30  0001 L CNN "Provider"
	1    2150 4650
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R27
U 1 1 5628AE41
P 9200 1300
F 0 "R27" V 9270 1300 50  0000 C CNN
F 1 "1K/5%" V 9130 1300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9350 1300 30  0001 C CNN
F 3 "" V 9280 1300 30  0000 C CNN
F 4 "0603" V 9200 1300 20  0000 C CNN "SMT"
F 5 "9331697" V 9400 1300 30  0001 C CNN "Part"
F 6 "Farnell" V 9450 1300 30  0001 C CNN "Provider"
	1    9200 1300
	0    1    1    0   
$EndComp
$Comp
L R_SMT R28
U 1 1 5628B183
P 9200 1600
F 0 "R28" V 9270 1600 50  0000 C CNN
F 1 "1K/5%" V 9130 1600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9350 1600 30  0001 C CNN
F 3 "" V 9280 1600 30  0000 C CNN
F 4 "0603" V 9200 1600 20  0000 C CNN "SMT"
F 5 "9331697" V 9400 1600 30  0001 C CNN "Part"
F 6 "Farnell" V 9450 1600 30  0001 C CNN "Provider"
	1    9200 1600
	0    1    1    0   
$EndComp
$Comp
L R_SMT R29
U 1 1 5628B214
P 9200 1900
F 0 "R29" V 9270 1900 50  0000 C CNN
F 1 "1K/5%" V 9130 1900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9350 1900 30  0001 C CNN
F 3 "" V 9280 1900 30  0000 C CNN
F 4 "0603" V 9200 1900 20  0000 C CNN "SMT"
F 5 "9331697" V 9400 1900 30  0001 C CNN "Part"
F 6 "Farnell" V 9450 1900 30  0001 C CNN "Provider"
	1    9200 1900
	0    1    1    0   
$EndComp
$Comp
L R_SMT R30
U 1 1 5628B2F4
P 9200 2200
F 0 "R30" V 9270 2200 50  0000 C CNN
F 1 "1K/5%" V 9130 2200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9350 2200 30  0001 C CNN
F 3 "" V 9280 2200 30  0000 C CNN
F 4 "0603" V 9200 2200 20  0000 C CNN "SMT"
F 5 "9331697" V 9400 2200 30  0001 C CNN "Part"
F 6 "Farnell" V 9450 2200 30  0001 C CNN "Provider"
	1    9200 2200
	0    1    1    0   
$EndComp
$Comp
L GND #PWR7
U 1 1 5628DEF7
P 2150 3700
F 0 "#PWR7" H 2150 3450 50  0001 C CNN
F 1 "GND" H 2150 3550 50  0000 C CNN
F 2 "" H 2150 3700 60  0000 C CNN
F 3 "" H 2150 3700 60  0000 C CNN
	1    2150 3700
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR11
U 1 1 5628DF08
P 3350 2500
F 0 "#PWR11" H 3350 2350 50  0001 C CNN
F 1 "+3V3" H 3350 2640 50  0000 C CNN
F 2 "" H 3350 2500 60  0000 C CNN
F 3 "" H 3350 2500 60  0000 C CNN
	1    3350 2500
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R16
U 1 1 5628DF17
P 2900 2650
F 0 "R16" V 2970 2650 50  0000 C CNN
F 1 "100R/5%" V 2830 2650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3050 2650 30  0001 C CNN
F 3 "" V 2980 2650 30  0000 C CNN
F 4 "0603" V 2900 2650 20  0000 C CNN "SMT"
F 5 "9331689" V 3100 2650 30  0001 C CNN "Part"
F 6 "Farnell" V 3150 2650 30  0001 C CNN "Provider"
	1    2900 2650
	0    1    1    0   
$EndComp
$Comp
L C_SMT C1
U 1 1 5628DF20
P 2150 2900
F 0 "C1" H 2175 3000 50  0000 L CNN
F 1 "100nF/50V" H 2175 2800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2180 2750 30  0001 L CNN
F 3 "" H 2175 3000 60  0000 L CNN
F 4 "0603" H 2150 2900 20  0000 C CNN "SMT"
F 5 "1414028" H 2180 2710 30  0001 L CNN "Part"
F 6 "Farnell" H 2180 2670 30  0001 L CNN "Provider"
	1    2150 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR9
U 1 1 5628E10E
P 2150 7150
F 0 "#PWR9" H 2150 6900 50  0001 C CNN
F 1 "GND" H 2150 7000 50  0000 C CNN
F 2 "" H 2150 7150 60  0000 C CNN
F 3 "" H 2150 7150 60  0000 C CNN
	1    2150 7150
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR13
U 1 1 5628E11F
P 3350 5950
F 0 "#PWR13" H 3350 5800 50  0001 C CNN
F 1 "+3V3" H 3350 6090 50  0000 C CNN
F 2 "" H 3350 5950 60  0000 C CNN
F 3 "" H 3350 5950 60  0000 C CNN
	1    3350 5950
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R18
U 1 1 5628E12E
P 2900 6100
F 0 "R18" V 2970 6100 50  0000 C CNN
F 1 "100R/5%" V 2830 6100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3050 6100 30  0001 C CNN
F 3 "" V 2980 6100 30  0000 C CNN
F 4 "0603" V 2900 6100 20  0000 C CNN "SMT"
F 5 "9331689" V 3100 6100 30  0001 C CNN "Part"
F 6 "Farnell" V 3150 6100 30  0001 C CNN "Provider"
	1    2900 6100
	0    1    1    0   
$EndComp
$Comp
L C_SMT C3
U 1 1 5628E137
P 2150 6350
F 0 "C3" H 2175 6450 50  0000 L CNN
F 1 "100nF/50V" H 2175 6250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2180 6200 30  0001 L CNN
F 3 "" H 2175 6450 60  0000 L CNN
F 4 "0603" H 2150 6350 20  0000 C CNN "SMT"
F 5 "1414028" H 2180 6160 30  0001 L CNN "Part"
F 6 "Farnell" H 2180 6120 30  0001 L CNN "Provider"
	1    2150 6350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR15
U 1 1 5628E31B
P 5250 5450
F 0 "#PWR15" H 5250 5200 50  0001 C CNN
F 1 "GND" H 5250 5300 50  0000 C CNN
F 2 "" H 5250 5450 60  0000 C CNN
F 3 "" H 5250 5450 60  0000 C CNN
	1    5250 5450
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR19
U 1 1 5628E32C
P 6450 4250
F 0 "#PWR19" H 6450 4100 50  0001 C CNN
F 1 "+3V3" H 6450 4390 50  0000 C CNN
F 2 "" H 6450 4250 60  0000 C CNN
F 3 "" H 6450 4250 60  0000 C CNN
	1    6450 4250
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R25
U 1 1 5628E33B
P 6000 4400
F 0 "R25" V 6070 4400 50  0000 C CNN
F 1 "100R/5%" V 5930 4400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6150 4400 30  0001 C CNN
F 3 "" V 6080 4400 30  0000 C CNN
F 4 "0603" V 6000 4400 20  0000 C CNN "SMT"
F 5 "9331689" V 6200 4400 30  0001 C CNN "Part"
F 6 "Farnell" V 6250 4400 30  0001 C CNN "Provider"
	1    6000 4400
	0    1    1    0   
$EndComp
$Comp
L C_SMT C4
U 1 1 5628E344
P 5250 4650
F 0 "C4" H 5275 4750 50  0000 L CNN
F 1 "100nF/50V" H 5275 4550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5280 4500 30  0001 L CNN
F 3 "" H 5275 4750 60  0000 L CNN
F 4 "0603" H 5250 4650 20  0000 C CNN "SMT"
F 5 "1414028" H 5280 4460 30  0001 L CNN "Part"
F 6 "Farnell" H 5280 4420 30  0001 L CNN "Provider"
	1    5250 4650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR16
U 1 1 5628E528
P 5250 7150
F 0 "#PWR16" H 5250 6900 50  0001 C CNN
F 1 "GND" H 5250 7000 50  0000 C CNN
F 2 "" H 5250 7150 60  0000 C CNN
F 3 "" H 5250 7150 60  0000 C CNN
	1    5250 7150
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR20
U 1 1 5628E539
P 6450 5950
F 0 "#PWR20" H 6450 5800 50  0001 C CNN
F 1 "+3V3" H 6450 6090 50  0000 C CNN
F 2 "" H 6450 5950 60  0000 C CNN
F 3 "" H 6450 5950 60  0000 C CNN
	1    6450 5950
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R26
U 1 1 5628E548
P 6000 6100
F 0 "R26" V 6070 6100 50  0000 C CNN
F 1 "100R/5%" V 5930 6100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6150 6100 30  0001 C CNN
F 3 "" V 6080 6100 30  0000 C CNN
F 4 "0603" V 6000 6100 20  0000 C CNN "SMT"
F 5 "9331689" V 6200 6100 30  0001 C CNN "Part"
F 6 "Farnell" V 6250 6100 30  0001 C CNN "Provider"
	1    6000 6100
	0    1    1    0   
$EndComp
$Comp
L C_SMT C5
U 1 1 5628E551
P 5250 6350
F 0 "C5" H 5275 6450 50  0000 L CNN
F 1 "100nF/50V" H 5275 6250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5280 6200 30  0001 L CNN
F 3 "" H 5275 6450 60  0000 L CNN
F 4 "0603" H 5250 6350 20  0000 C CNN "SMT"
F 5 "1414028" H 5280 6160 30  0001 L CNN "Part"
F 6 "Farnell" H 5280 6120 30  0001 L CNN "Provider"
	1    5250 6350
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH_TACKTILE SW1
U 1 1 5628E6E9
P 2900 1300
F 0 "SW1" H 3050 1410 50  0000 C CNN
F 1 "OMRON B3F-1070" H 2900 1100 50  0000 C CNN
F 2 "Sigmadrone:SW_TH_Tactile_Omron_B3F-10xx_6x6" H 2900 1300 60  0001 C CNN
F 3 "" H 2900 1300 60  0000 C CNN
	1    2900 1300
	1    0    0    -1  
$EndComp
$Comp
L LED_SMT DL1
U 1 1 5631FE41
P 9900 1300
F 0 "DL1" H 9900 1400 50  0000 C CNN
F 1 "GREEN" H 9900 1200 50  0000 C CNN
F 2 "LEDs:LED_0603" H 9900 1150 30  0001 C CNN
F 3 "" H 9900 1300 60  0000 C CNN
F 4 "0603" H 9785 1280 30  0000 C CNN "SMT"
F 5 "2322070" H 9900 1100 30  0001 C CNN "Part"
F 6 "Farnell" H 9900 1050 30  0001 C CNN "Provider"
	1    9900 1300
	-1   0    0    -1  
$EndComp
$Comp
L LED_SMT DL2
U 1 1 563203FE
P 9900 1600
F 0 "DL2" H 9900 1700 50  0000 C CNN
F 1 "GREEN" H 9900 1500 50  0000 C CNN
F 2 "LEDs:LED_0603" H 9900 1450 30  0001 C CNN
F 3 "" H 9900 1600 60  0000 C CNN
F 4 "0603" H 9785 1580 30  0000 C CNN "SMT"
F 5 "2322070" H 9900 1400 30  0001 C CNN "Part"
F 6 "Farnell" H 9900 1350 30  0001 C CNN "Provider"
	1    9900 1600
	-1   0    0    -1  
$EndComp
$Comp
L LED_SMT DL3
U 1 1 5632048F
P 9900 1900
F 0 "DL3" H 9900 2000 50  0000 C CNN
F 1 "RED" H 9900 1800 50  0000 C CNN
F 2 "LEDs:LED_0603" H 9900 1750 30  0001 C CNN
F 3 "" H 9900 1900 60  0000 C CNN
F 4 "0603" H 9785 1880 30  0000 C CNN "SMT"
F 5 "2322071" H 9900 1700 30  0001 C CNN "Part"
F 6 "Farnell" H 9900 1650 30  0001 C CNN "Provider"
	1    9900 1900
	-1   0    0    -1  
$EndComp
$Comp
L LED_SMT DL4
U 1 1 563205A7
P 9900 2200
F 0 "DL4" H 9900 2300 50  0000 C CNN
F 1 "RED" H 9900 2100 50  0000 C CNN
F 2 "LEDs:LED_0603" H 9900 2050 30  0001 C CNN
F 3 "" H 9900 2200 60  0000 C CNN
F 4 "0603" H 9785 2180 30  0000 C CNN "SMT"
F 5 "2322071" H 9900 2000 30  0001 C CNN "Part"
F 6 "Farnell" H 9900 1950 30  0001 C CNN "Provider"
	1    9900 2200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3350 1300 3350 1850
Wire Wire Line
	3350 1300 3200 1300
Wire Wire Line
	10100 1600 10550 1600
Connection ~ 10550 1600
Wire Wire Line
	10550 1900 10100 1900
Connection ~ 10550 1900
Wire Wire Line
	10550 2200 10100 2200
Wire Wire Line
	9350 1300 9700 1300
Wire Wire Line
	9350 1600 9700 1600
Wire Wire Line
	9350 1900 9700 1900
Wire Wire Line
	9350 2200 9700 2200
Wire Wire Line
	8650 1300 9050 1300
Wire Wire Line
	8650 1600 9050 1600
Wire Wire Line
	8650 1900 9050 1900
Wire Wire Line
	8650 2200 9050 2200
Wire Wire Line
	1350 1300 1950 1300
Wire Wire Line
	6450 3150 6300 3150
Wire Wire Line
	6450 2500 6450 3250
Wire Wire Line
	10550 950  10550 2500
Connection ~ 10550 2200
Wire Wire Line
	2150 5450 2150 5350
Wire Wire Line
	1850 4900 2600 4900
Wire Wire Line
	2150 4800 2150 5050
Connection ~ 2150 4900
Wire Wire Line
	1550 4900 1350 4900
Wire Wire Line
	2150 4500 2150 4400
Wire Wire Line
	2150 4400 2750 4400
Wire Wire Line
	3050 4400 3350 4400
Wire Wire Line
	3350 4250 3350 5000
Wire Wire Line
	3350 4900 3200 4900
Connection ~ 3350 4400
Wire Wire Line
	3350 5000 3200 5000
Connection ~ 3350 4900
Wire Wire Line
	2600 5000 2450 5000
Wire Wire Line
	2450 5000 2450 4900
Connection ~ 2450 4900
Wire Wire Line
	2150 3700 2150 3600
Wire Wire Line
	1850 3150 2600 3150
Wire Wire Line
	2150 3050 2150 3300
Connection ~ 2150 3150
Wire Wire Line
	1550 3150 1350 3150
Wire Wire Line
	2150 2750 2150 2650
Wire Wire Line
	2150 2650 2750 2650
Wire Wire Line
	3050 2650 3350 2650
Wire Wire Line
	3350 2500 3350 3250
Wire Wire Line
	3350 3150 3200 3150
Connection ~ 3350 2650
Wire Wire Line
	3350 3250 3200 3250
Connection ~ 3350 3150
Wire Wire Line
	2600 3250 2450 3250
Wire Wire Line
	2450 3250 2450 3150
Connection ~ 2450 3150
Wire Wire Line
	2150 7150 2150 7050
Wire Wire Line
	1850 6600 2600 6600
Wire Wire Line
	2150 6500 2150 6750
Connection ~ 2150 6600
Wire Wire Line
	1550 6600 1350 6600
Wire Wire Line
	2150 6200 2150 6100
Wire Wire Line
	2150 6100 2750 6100
Wire Wire Line
	3050 6100 3350 6100
Wire Wire Line
	3350 5950 3350 6700
Wire Wire Line
	3350 6600 3200 6600
Connection ~ 3350 6100
Wire Wire Line
	3350 6700 3200 6700
Connection ~ 3350 6600
Wire Wire Line
	2600 6700 2450 6700
Wire Wire Line
	2450 6700 2450 6600
Connection ~ 2450 6600
Wire Wire Line
	5250 5450 5250 5350
Wire Wire Line
	4950 4900 5700 4900
Wire Wire Line
	5250 4800 5250 5050
Connection ~ 5250 4900
Wire Wire Line
	4650 4900 4450 4900
Wire Wire Line
	5250 4500 5250 4400
Wire Wire Line
	5250 4400 5850 4400
Wire Wire Line
	6150 4400 6450 4400
Wire Wire Line
	6450 4250 6450 5000
Wire Wire Line
	6450 4900 6300 4900
Connection ~ 6450 4400
Wire Wire Line
	6450 5000 6300 5000
Connection ~ 6450 4900
Wire Wire Line
	5700 5000 5550 5000
Wire Wire Line
	5550 5000 5550 4900
Connection ~ 5550 4900
Wire Wire Line
	5250 7150 5250 7050
Wire Wire Line
	4950 6600 5700 6600
Wire Wire Line
	5250 6500 5250 6750
Connection ~ 5250 6600
Wire Wire Line
	4650 6600 4450 6600
Wire Wire Line
	5250 6200 5250 6100
Wire Wire Line
	5250 6100 5850 6100
Wire Wire Line
	6150 6100 6450 6100
Wire Wire Line
	6450 5950 6450 6700
Wire Wire Line
	6450 6600 6300 6600
Connection ~ 6450 6100
Wire Wire Line
	6450 6700 6300 6700
Connection ~ 6450 6600
Wire Wire Line
	5700 6700 5550 6700
Wire Wire Line
	5550 6700 5550 6600
Connection ~ 5550 6600
Wire Wire Line
	6450 3250 6300 3250
Connection ~ 6450 3150
Wire Wire Line
	5700 3250 5550 3250
Wire Wire Line
	5550 3250 5550 3150
Connection ~ 5550 3150
Wire Wire Line
	3200 1400 3350 1400
Connection ~ 3350 1400
Wire Wire Line
	2600 1400 2450 1400
Wire Wire Line
	2450 1400 2450 1300
Connection ~ 2450 1300
Wire Wire Line
	10100 1300 10550 1300
Connection ~ 5250 3150
$Comp
L R_SMT R22
U 1 1 5631DE54
P 5250 3450
F 0 "R22" V 5320 3450 50  0000 C CNN
F 1 "10K/5%" V 5180 3450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5400 3450 30  0001 C CNN
F 3 "" V 5330 3450 30  0000 C CNN
F 4 "0603" V 5250 3450 20  0000 C CNN "SMT"
F 5 "9331700" V 5450 3450 30  0001 C CNN "Part"
F 6 "Farnell" V 5500 3450 30  0001 C CNN "Provider"
	1    5250 3450
	-1   0    0    1   
$EndComp
Wire Wire Line
	5250 3700 5250 3600
Wire Wire Line
	5250 3150 5250 3300
$Comp
L GND #PWR14
U 1 1 5622D3C9
P 5250 3700
F 0 "#PWR14" H 5250 3450 50  0001 C CNN
F 1 "GND" H 5250 3550 50  0000 C CNN
F 2 "" H 5250 3700 60  0000 C CNN
F 3 "" H 5250 3700 60  0000 C CNN
	1    5250 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 3150 5700 3150
$Comp
L R_SMT R9
U 1 1 56537F92
P 1700 3150
F 0 "R9" V 1770 3150 50  0000 C CNN
F 1 "100R/5%" V 1630 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1850 3150 30  0001 C CNN
F 3 "" V 1780 3150 30  0000 C CNN
F 4 "0603" V 1700 3150 20  0000 C CNN "SMT"
F 5 "9331689" V 1900 3150 30  0001 C CNN "Part"
F 6 "Farnell" V 1950 3150 30  0001 C CNN "Provider"
	1    1700 3150
	0    1    1    0   
$EndComp
$Comp
L R_SMT R10
U 1 1 56538116
P 1700 4900
F 0 "R10" V 1770 4900 50  0000 C CNN
F 1 "100R/5%" V 1630 4900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1850 4900 30  0001 C CNN
F 3 "" V 1780 4900 30  0000 C CNN
F 4 "0603" V 1700 4900 20  0000 C CNN "SMT"
F 5 "9331689" V 1900 4900 30  0001 C CNN "Part"
F 6 "Farnell" V 1950 4900 30  0001 C CNN "Provider"
	1    1700 4900
	0    1    1    0   
$EndComp
$Comp
L R_SMT R11
U 1 1 565382A2
P 1700 6600
F 0 "R11" V 1770 6600 50  0000 C CNN
F 1 "100R/5%" V 1630 6600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1850 6600 30  0001 C CNN
F 3 "" V 1780 6600 30  0000 C CNN
F 4 "0603" V 1700 6600 20  0000 C CNN "SMT"
F 5 "9331689" V 1900 6600 30  0001 C CNN "Part"
F 6 "Farnell" V 1950 6600 30  0001 C CNN "Provider"
	1    1700 6600
	0    1    1    0   
$EndComp
$Comp
L R_SMT R20
U 1 1 5653837D
P 4800 6600
F 0 "R20" V 4870 6600 50  0000 C CNN
F 1 "100R/5%" V 4730 6600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4950 6600 30  0001 C CNN
F 3 "" V 4880 6600 30  0000 C CNN
F 4 "0603" V 4800 6600 20  0000 C CNN "SMT"
F 5 "9331689" V 5000 6600 30  0001 C CNN "Part"
F 6 "Farnell" V 5050 6600 30  0001 C CNN "Provider"
	1    4800 6600
	0    1    1    0   
$EndComp
$Comp
L R_SMT R19
U 1 1 56538499
P 4800 4900
F 0 "R19" V 4870 4900 50  0000 C CNN
F 1 "100R/5%" V 4730 4900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4950 4900 30  0001 C CNN
F 3 "" V 4880 4900 30  0000 C CNN
F 4 "0603" V 4800 4900 20  0000 C CNN "SMT"
F 5 "9331689" V 5000 4900 30  0001 C CNN "Part"
F 6 "Farnell" V 5050 4900 30  0001 C CNN "Provider"
	1    4800 4900
	0    1    1    0   
$EndComp
$Comp
L R_SMT R13
U 1 1 56538917
P 2150 3450
F 0 "R13" V 2220 3450 50  0000 C CNN
F 1 "100K/5%" V 2080 3450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2300 3450 30  0001 C CNN
F 3 "" V 2230 3450 30  0000 C CNN
F 4 "0603" V 2150 3450 20  0000 C CNN "SMT"
F 5 "9233628" V 2350 3450 30  0001 C CNN "Part"
F 6 "Farnell" V 2400 3450 30  0001 C CNN "Provider"
	1    2150 3450
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R14
U 1 1 565389F7
P 2150 5200
F 0 "R14" V 2220 5200 50  0000 C CNN
F 1 "100K/5%" V 2080 5200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2300 5200 30  0001 C CNN
F 3 "" V 2230 5200 30  0000 C CNN
F 4 "0603" V 2150 5200 20  0000 C CNN "SMT"
F 5 "9233628" V 2350 5200 30  0001 C CNN "Part"
F 6 "Farnell" V 2400 5200 30  0001 C CNN "Provider"
	1    2150 5200
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R15
U 1 1 56538B8F
P 2150 6900
F 0 "R15" V 2220 6900 50  0000 C CNN
F 1 "100K/5%" V 2080 6900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2300 6900 30  0001 C CNN
F 3 "" V 2230 6900 30  0000 C CNN
F 4 "0603" V 2150 6900 20  0000 C CNN "SMT"
F 5 "9233628" V 2350 6900 30  0001 C CNN "Part"
F 6 "Farnell" V 2400 6900 30  0001 C CNN "Provider"
	1    2150 6900
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R24
U 1 1 56538C61
P 5250 6900
F 0 "R24" V 5320 6900 50  0000 C CNN
F 1 "100K/5%" V 5180 6900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5400 6900 30  0001 C CNN
F 3 "" V 5330 6900 30  0000 C CNN
F 4 "0603" V 5250 6900 20  0000 C CNN "SMT"
F 5 "9233628" V 5450 6900 30  0001 C CNN "Part"
F 6 "Farnell" V 5500 6900 30  0001 C CNN "Provider"
	1    5250 6900
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R23
U 1 1 56538D76
P 5250 5200
F 0 "R23" V 5320 5200 50  0000 C CNN
F 1 "100K/5%" V 5180 5200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5400 5200 30  0001 C CNN
F 3 "" V 5330 5200 30  0000 C CNN
F 4 "0603" V 5250 5200 20  0000 C CNN "SMT"
F 5 "9233628" V 5450 5200 30  0001 C CNN "Part"
F 6 "Farnell" V 5500 5200 30  0001 C CNN "Provider"
	1    5250 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 1300 2600 1300
Text HLabel 4450 1300 0    60   Output ~ 0
MODULE_RESET#
$Comp
L GND #PWR17
U 1 1 56C6D2EA
P 6450 1850
F 0 "#PWR17" H 6450 1600 50  0001 C CNN
F 1 "GND" H 6450 1700 50  0000 C CNN
F 2 "" H 6450 1850 60  0000 C CNN
F 3 "" H 6450 1850 60  0000 C CNN
	1    6450 1850
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R21
U 1 1 56C6D2F3
P 5200 1300
F 0 "R21" V 5270 1300 50  0000 C CNN
F 1 "100R/5%" V 5130 1300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5350 1300 30  0001 C CNN
F 3 "" V 5280 1300 30  0000 C CNN
F 4 "0603" V 5200 1300 20  0000 C CNN "SMT"
F 5 "9331689" V 5400 1300 30  0001 C CNN "Part"
F 6 "Farnell" V 5450 1300 30  0001 C CNN "Provider"
	1    5200 1300
	0    1    1    0   
$EndComp
Wire Wire Line
	6450 1300 6450 1850
Wire Wire Line
	6450 1300 6300 1300
Wire Wire Line
	4450 1300 5050 1300
Wire Wire Line
	6300 1400 6450 1400
Connection ~ 6450 1400
Wire Wire Line
	5700 1400 5550 1400
Wire Wire Line
	5550 1400 5550 1300
Connection ~ 5550 1300
Wire Wire Line
	5350 1300 5700 1300
$Comp
L SW_PUSH_TACKTILE SW2
U 1 1 56CBF3DE
P 2900 3150
F 0 "SW2" H 3050 3260 50  0000 C CNN
F 1 "OMRON B3F-1070" H 2900 2950 50  0000 C CNN
F 2 "Sigmadrone:SW_TH_Tactile_Omron_B3F-10xx_6x6" H 2900 3150 60  0001 C CNN
F 3 "" H 2900 3150 60  0000 C CNN
	1    2900 3150
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH_TACKTILE SW3
U 1 1 56CBF50A
P 2900 4900
F 0 "SW3" H 3050 5010 50  0000 C CNN
F 1 "OMRON B3F-1070" H 2900 4700 50  0000 C CNN
F 2 "Sigmadrone:SW_TH_Tactile_Omron_B3F-10xx_6x6" H 2900 4900 60  0001 C CNN
F 3 "" H 2900 4900 60  0000 C CNN
	1    2900 4900
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH_TACKTILE SW4
U 1 1 56CBF5DA
P 2900 6600
F 0 "SW4" H 3050 6710 50  0000 C CNN
F 1 "OMRON B3F-1070" H 2900 6400 50  0000 C CNN
F 2 "Sigmadrone:SW_TH_Tactile_Omron_B3F-10xx_6x6" H 2900 6600 60  0001 C CNN
F 3 "" H 2900 6600 60  0000 C CNN
	1    2900 6600
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH_TACKTILE SW6
U 1 1 56CBF7A1
P 6000 3150
F 0 "SW6" H 6150 3260 50  0000 C CNN
F 1 "OMRON B3F-1070" H 6000 2950 50  0000 C CNN
F 2 "Sigmadrone:SW_TH_Tactile_Omron_B3F-10xx_6x6" H 6000 3150 60  0001 C CNN
F 3 "" H 6000 3150 60  0000 C CNN
	1    6000 3150
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH_TACKTILE SW7
U 1 1 56CBF85F
P 6000 4900
F 0 "SW7" H 6150 5010 50  0000 C CNN
F 1 "OMRON B3F-1070" H 6000 4700 50  0000 C CNN
F 2 "Sigmadrone:SW_TH_Tactile_Omron_B3F-10xx_6x6" H 6000 4900 60  0001 C CNN
F 3 "" H 6000 4900 60  0000 C CNN
	1    6000 4900
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH_TACKTILE SW8
U 1 1 56CBF91A
P 6000 6600
F 0 "SW8" H 6150 6710 50  0000 C CNN
F 1 "OMRON B3F-1070" H 6000 6400 50  0000 C CNN
F 2 "Sigmadrone:SW_TH_Tactile_Omron_B3F-10xx_6x6" H 6000 6600 60  0001 C CNN
F 3 "" H 6000 6600 60  0000 C CNN
	1    6000 6600
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH_TACKTILE SW5
U 1 1 56CBFB52
P 6000 1300
F 0 "SW5" H 6150 1410 50  0000 C CNN
F 1 "OMRON B3F-1070" H 6000 1100 50  0000 C CNN
F 2 "Sigmadrone:SW_TH_Tactile_Omron_B3F-10xx_6x6" H 6000 1300 60  0001 C CNN
F 3 "" H 6000 1300 60  0000 C CNN
	1    6000 1300
	1    0    0    -1  
$EndComp
Text HLabel 8650 950  0    60   Input ~ 0
LED0
$Comp
L R_SMT R41
U 1 1 56CD11BD
P 9200 950
F 0 "R41" V 9270 950 50  0000 C CNN
F 1 "1K/5%" V 9130 950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9350 950 30  0001 C CNN
F 3 "" V 9280 950 30  0000 C CNN
F 4 "0603" V 9200 950 20  0000 C CNN "SMT"
F 5 "9331697" V 9400 950 30  0001 C CNN "Part"
F 6 "Farnell" V 9450 950 30  0001 C CNN "Provider"
	1    9200 950 
	0    1    1    0   
$EndComp
$Comp
L LED_SMT DL14
U 1 1 56CD11C6
P 9900 950
F 0 "DL14" H 9900 1050 50  0000 C CNN
F 1 "GREEN" H 9900 850 50  0000 C CNN
F 2 "LEDs:LED_0603" H 9900 800 30  0001 C CNN
F 3 "" H 9900 950 60  0000 C CNN
F 4 "0603" H 9785 930 30  0000 C CNN "SMT"
F 5 "2322070" H 9900 750 30  0001 C CNN "Part"
F 6 "Farnell" H 9900 700 30  0001 C CNN "Provider"
	1    9900 950 
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9350 950  9700 950 
Wire Wire Line
	8650 950  9050 950 
Wire Wire Line
	10100 950  10550 950 
Connection ~ 10550 1300
$EndSCHEMATC
