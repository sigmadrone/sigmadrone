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
Sheet 4 28
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 1350 3400 0    60   Output ~ 0
BUTTON_0
Text HLabel 1350 5150 0    60   Output ~ 0
BUTTON_1
Text HLabel 1350 6850 0    60   Output ~ 0
BUTTON_2
Text HLabel 4300 3400 0    60   Output ~ 0
BUTTON_3
Text HLabel 4300 5150 0    60   Output ~ 0
BUTTON_4
Text HLabel 1350 1700 0    60   Output ~ 0
RESET#
Text HLabel 8200 1200 0    60   Input ~ 0
LED1
Text HLabel 8200 1800 0    60   Input ~ 0
LED3
Text HLabel 8200 1500 0    60   Input ~ 0
LED2
Text HLabel 8200 2100 0    60   Input ~ 0
LED4
$Comp
L GND #PWR?
U 1 1 56074E25
P 3350 2250
F 0 "#PWR?" H 3350 2000 50  0001 C CNN
F 1 "GND" H 3350 2100 50  0000 C CNN
F 2 "" H 3350 2250 60  0000 C CNN
F 3 "" H 3350 2250 60  0000 C CNN
	1    3350 2250
	1    0    0    -1  
$EndComp
Text HLabel 4350 1700 0    60   Output ~ 0
BOOT0
$Comp
L GND #PWR?
U 1 1 5623679B
P 10100 2400
F 0 "#PWR?" H 10100 2150 50  0001 C CNN
F 1 "GND" H 10100 2250 50  0000 C CNN
F 2 "" H 10100 2400 60  0000 C CNN
F 3 "" H 10100 2400 60  0000 C CNN
	1    10100 2400
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR?
U 1 1 5623E09E
P 6300 1050
F 0 "#PWR?" H 6300 900 50  0001 C CNN
F 1 "+3V3" H 6300 1190 50  0000 C CNN
F 2 "" H 6300 1050 60  0000 C CNN
F 3 "" H 6300 1050 60  0000 C CNN
	1    6300 1050
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56287BD9
P 2100 1700
F 0 "R?" V 2170 1700 50  0000 C CNN
F 1 "100R/5%" V 2030 1700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2250 1700 30  0001 C CNN
F 3 "" V 2180 1700 30  0000 C CNN
F 4 "0603" V 2100 1700 20  0000 C CNN "SMT"
F 5 "9331689" V 2300 1700 30  0001 C CNN "Part"
F 6 "Farnell" V 2350 1700 30  0001 C CNN "Provider"
	1    2100 1700
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 56289795
P 2150 5700
F 0 "#PWR?" H 2150 5450 50  0001 C CNN
F 1 "GND" H 2150 5550 50  0000 C CNN
F 2 "" H 2150 5700 60  0000 C CNN
F 3 "" H 2150 5700 60  0000 C CNN
	1    2150 5700
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR?
U 1 1 562897A6
P 3350 4500
F 0 "#PWR?" H 3350 4350 50  0001 C CNN
F 1 "+3V3" H 3350 4640 50  0000 C CNN
F 2 "" H 3350 4500 60  0000 C CNN
F 3 "" H 3350 4500 60  0000 C CNN
	1    3350 4500
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 562897B5
P 2900 4650
F 0 "R?" V 2970 4650 50  0000 C CNN
F 1 "100R/5%" V 2830 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3050 4650 30  0001 C CNN
F 3 "" V 2980 4650 30  0000 C CNN
F 4 "0603" V 2900 4650 20  0000 C CNN "SMT"
F 5 "9331689" V 3100 4650 30  0001 C CNN "Part"
F 6 "Farnell" V 3150 4650 30  0001 C CNN "Provider"
	1    2900 4650
	0    1    1    0   
$EndComp
$Comp
L C_SMT C?
U 1 1 562897BE
P 2150 4900
F 0 "C?" H 2175 5000 50  0000 L CNN
F 1 "100nF/50V" H 2175 4800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2180 4750 30  0001 L CNN
F 3 "" H 2175 5000 60  0000 L CNN
F 4 "0603" H 2150 4900 20  0000 C CNN "SMT"
F 5 "1414028" H 2180 4710 30  0001 L CNN "Part"
F 6 "Farnell" H 2180 4670 30  0001 L CNN "Provider"
	1    2150 4900
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 5628AE41
P 8750 1200
F 0 "R?" V 8820 1200 50  0000 C CNN
F 1 "1K/5%" V 8680 1200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8900 1200 30  0001 C CNN
F 3 "" V 8830 1200 30  0000 C CNN
F 4 "0603" V 8750 1200 20  0000 C CNN "SMT"
F 5 "9331697" V 8950 1200 30  0001 C CNN "Part"
F 6 "Farnell" V 9000 1200 30  0001 C CNN "Provider"
	1    8750 1200
	0    1    1    0   
$EndComp
$Comp
L R_SMT R?
U 1 1 5628B183
P 8750 1500
F 0 "R?" V 8820 1500 50  0000 C CNN
F 1 "1K/5%" V 8680 1500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8900 1500 30  0001 C CNN
F 3 "" V 8830 1500 30  0000 C CNN
F 4 "0603" V 8750 1500 20  0000 C CNN "SMT"
F 5 "9331697" V 8950 1500 30  0001 C CNN "Part"
F 6 "Farnell" V 9000 1500 30  0001 C CNN "Provider"
	1    8750 1500
	0    1    1    0   
$EndComp
$Comp
L R_SMT R?
U 1 1 5628B214
P 8750 1800
F 0 "R?" V 8820 1800 50  0000 C CNN
F 1 "1K/5%" V 8680 1800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8900 1800 30  0001 C CNN
F 3 "" V 8830 1800 30  0000 C CNN
F 4 "0603" V 8750 1800 20  0000 C CNN "SMT"
F 5 "9331697" V 8950 1800 30  0001 C CNN "Part"
F 6 "Farnell" V 9000 1800 30  0001 C CNN "Provider"
	1    8750 1800
	0    1    1    0   
$EndComp
$Comp
L R_SMT R?
U 1 1 5628B2F4
P 8750 2100
F 0 "R?" V 8820 2100 50  0000 C CNN
F 1 "1K/5%" V 8680 2100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8900 2100 30  0001 C CNN
F 3 "" V 8830 2100 30  0000 C CNN
F 4 "0603" V 8750 2100 20  0000 C CNN "SMT"
F 5 "9331697" V 8950 2100 30  0001 C CNN "Part"
F 6 "Farnell" V 9000 2100 30  0001 C CNN "Provider"
	1    8750 2100
	0    1    1    0   
$EndComp
$Comp
L SW_PUSH_TACKTILE SW?
U 1 1 5628D63B
P 2900 5150
F 0 "SW?" H 3050 5260 50  0000 C CNN
F 1 "WURTH 436353045816" H 2900 4950 50  0000 C CNN
F 2 "" H 2900 5150 60  0000 C CNN
F 3 "" H 2900 5150 60  0000 C CNN
	1    2900 5150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5628DEF7
P 2150 3950
F 0 "#PWR?" H 2150 3700 50  0001 C CNN
F 1 "GND" H 2150 3800 50  0000 C CNN
F 2 "" H 2150 3950 60  0000 C CNN
F 3 "" H 2150 3950 60  0000 C CNN
	1    2150 3950
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR?
U 1 1 5628DF08
P 3350 2750
F 0 "#PWR?" H 3350 2600 50  0001 C CNN
F 1 "+3V3" H 3350 2890 50  0000 C CNN
F 2 "" H 3350 2750 60  0000 C CNN
F 3 "" H 3350 2750 60  0000 C CNN
	1    3350 2750
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 5628DF17
P 2900 2900
F 0 "R?" V 2970 2900 50  0000 C CNN
F 1 "100R/5%" V 2830 2900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3050 2900 30  0001 C CNN
F 3 "" V 2980 2900 30  0000 C CNN
F 4 "0603" V 2900 2900 20  0000 C CNN "SMT"
F 5 "9331689" V 3100 2900 30  0001 C CNN "Part"
F 6 "Farnell" V 3150 2900 30  0001 C CNN "Provider"
	1    2900 2900
	0    1    1    0   
$EndComp
$Comp
L C_SMT C?
U 1 1 5628DF20
P 2150 3150
F 0 "C?" H 2175 3250 50  0000 L CNN
F 1 "100nF/50V" H 2175 3050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2180 3000 30  0001 L CNN
F 3 "" H 2175 3250 60  0000 L CNN
F 4 "0603" H 2150 3150 20  0000 C CNN "SMT"
F 5 "1414028" H 2180 2960 30  0001 L CNN "Part"
F 6 "Farnell" H 2180 2920 30  0001 L CNN "Provider"
	1    2150 3150
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH_TACKTILE SW?
U 1 1 5628DF2F
P 2900 3400
F 0 "SW?" H 3050 3510 50  0000 C CNN
F 1 "WURTH 436353045816" H 2900 3200 50  0000 C CNN
F 2 "" H 2900 3400 60  0000 C CNN
F 3 "" H 2900 3400 60  0000 C CNN
	1    2900 3400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5628E10E
P 2150 7400
F 0 "#PWR?" H 2150 7150 50  0001 C CNN
F 1 "GND" H 2150 7250 50  0000 C CNN
F 2 "" H 2150 7400 60  0000 C CNN
F 3 "" H 2150 7400 60  0000 C CNN
	1    2150 7400
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR?
U 1 1 5628E11F
P 3350 6200
F 0 "#PWR?" H 3350 6050 50  0001 C CNN
F 1 "+3V3" H 3350 6340 50  0000 C CNN
F 2 "" H 3350 6200 60  0000 C CNN
F 3 "" H 3350 6200 60  0000 C CNN
	1    3350 6200
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 5628E12E
P 2900 6350
F 0 "R?" V 2970 6350 50  0000 C CNN
F 1 "100R/5%" V 2830 6350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3050 6350 30  0001 C CNN
F 3 "" V 2980 6350 30  0000 C CNN
F 4 "0603" V 2900 6350 20  0000 C CNN "SMT"
F 5 "9331689" V 3100 6350 30  0001 C CNN "Part"
F 6 "Farnell" V 3150 6350 30  0001 C CNN "Provider"
	1    2900 6350
	0    1    1    0   
$EndComp
$Comp
L C_SMT C?
U 1 1 5628E137
P 2150 6600
F 0 "C?" H 2175 6700 50  0000 L CNN
F 1 "100nF/50V" H 2175 6500 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2180 6450 30  0001 L CNN
F 3 "" H 2175 6700 60  0000 L CNN
F 4 "0603" H 2150 6600 20  0000 C CNN "SMT"
F 5 "1414028" H 2180 6410 30  0001 L CNN "Part"
F 6 "Farnell" H 2180 6370 30  0001 L CNN "Provider"
	1    2150 6600
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH_TACKTILE SW?
U 1 1 5628E146
P 2900 6850
F 0 "SW?" H 3050 6960 50  0000 C CNN
F 1 "WURTH 436353045816" H 2900 6650 50  0000 C CNN
F 2 "" H 2900 6850 60  0000 C CNN
F 3 "" H 2900 6850 60  0000 C CNN
	1    2900 6850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5628E31B
P 5100 3950
F 0 "#PWR?" H 5100 3700 50  0001 C CNN
F 1 "GND" H 5100 3800 50  0000 C CNN
F 2 "" H 5100 3950 60  0000 C CNN
F 3 "" H 5100 3950 60  0000 C CNN
	1    5100 3950
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR?
U 1 1 5628E32C
P 6300 2750
F 0 "#PWR?" H 6300 2600 50  0001 C CNN
F 1 "+3V3" H 6300 2890 50  0000 C CNN
F 2 "" H 6300 2750 60  0000 C CNN
F 3 "" H 6300 2750 60  0000 C CNN
	1    6300 2750
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 5628E33B
P 5850 2900
F 0 "R?" V 5920 2900 50  0000 C CNN
F 1 "100R/5%" V 5780 2900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6000 2900 30  0001 C CNN
F 3 "" V 5930 2900 30  0000 C CNN
F 4 "0603" V 5850 2900 20  0000 C CNN "SMT"
F 5 "9331689" V 6050 2900 30  0001 C CNN "Part"
F 6 "Farnell" V 6100 2900 30  0001 C CNN "Provider"
	1    5850 2900
	0    1    1    0   
$EndComp
$Comp
L C_SMT C?
U 1 1 5628E344
P 5100 3150
F 0 "C?" H 5125 3250 50  0000 L CNN
F 1 "100nF/50V" H 5125 3050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5130 3000 30  0001 L CNN
F 3 "" H 5125 3250 60  0000 L CNN
F 4 "0603" H 5100 3150 20  0000 C CNN "SMT"
F 5 "1414028" H 5130 2960 30  0001 L CNN "Part"
F 6 "Farnell" H 5130 2920 30  0001 L CNN "Provider"
	1    5100 3150
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH_TACKTILE SW?
U 1 1 5628E353
P 5850 3400
F 0 "SW?" H 6000 3510 50  0000 C CNN
F 1 "WURTH 436353045816" H 5850 3200 50  0000 C CNN
F 2 "" H 5850 3400 60  0000 C CNN
F 3 "" H 5850 3400 60  0000 C CNN
	1    5850 3400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5628E528
P 5100 5700
F 0 "#PWR?" H 5100 5450 50  0001 C CNN
F 1 "GND" H 5100 5550 50  0000 C CNN
F 2 "" H 5100 5700 60  0000 C CNN
F 3 "" H 5100 5700 60  0000 C CNN
	1    5100 5700
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR?
U 1 1 5628E539
P 6300 4500
F 0 "#PWR?" H 6300 4350 50  0001 C CNN
F 1 "+3V3" H 6300 4640 50  0000 C CNN
F 2 "" H 6300 4500 60  0000 C CNN
F 3 "" H 6300 4500 60  0000 C CNN
	1    6300 4500
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 5628E548
P 5850 4650
F 0 "R?" V 5920 4650 50  0000 C CNN
F 1 "100R/5%" V 5780 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6000 4650 30  0001 C CNN
F 3 "" V 5930 4650 30  0000 C CNN
F 4 "0603" V 5850 4650 20  0000 C CNN "SMT"
F 5 "9331689" V 6050 4650 30  0001 C CNN "Part"
F 6 "Farnell" V 6100 4650 30  0001 C CNN "Provider"
	1    5850 4650
	0    1    1    0   
$EndComp
$Comp
L C_SMT C?
U 1 1 5628E551
P 5100 4900
F 0 "C?" H 5125 5000 50  0000 L CNN
F 1 "100nF/50V" H 5125 4800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5130 4750 30  0001 L CNN
F 3 "" H 5125 5000 60  0000 L CNN
F 4 "0603" H 5100 4900 20  0000 C CNN "SMT"
F 5 "1414028" H 5130 4710 30  0001 L CNN "Part"
F 6 "Farnell" H 5130 4670 30  0001 L CNN "Provider"
	1    5100 4900
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH_TACKTILE SW?
U 1 1 5628E560
P 5850 5150
F 0 "SW?" H 6000 5260 50  0000 C CNN
F 1 "WURTH 436353045816" H 5850 4950 50  0000 C CNN
F 2 "" H 5850 5150 60  0000 C CNN
F 3 "" H 5850 5150 60  0000 C CNN
	1    5850 5150
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH_TACKTILE SW?
U 1 1 5628E629
P 5850 1700
F 0 "SW?" H 6000 1810 50  0000 C CNN
F 1 "WURTH 436353045816" H 5850 1500 50  0000 C CNN
F 2 "" H 5850 1700 60  0000 C CNN
F 3 "" H 5850 1700 60  0000 C CNN
	1    5850 1700
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH_TACKTILE SW?
U 1 1 5628E6E9
P 2900 1700
F 0 "SW?" H 3050 1810 50  0000 C CNN
F 1 "WURTH 436353045816" H 2900 1500 50  0000 C CNN
F 2 "" H 2900 1700 60  0000 C CNN
F 3 "" H 2900 1700 60  0000 C CNN
	1    2900 1700
	1    0    0    -1  
$EndComp
$Comp
L LED_SMT DL?
U 1 1 5631FE41
P 9450 1200
F 0 "DL?" H 9450 1300 50  0000 C CNN
F 1 "GREEN" H 9450 1100 50  0000 C CNN
F 2 "LEDs:LED-0603" H 9450 1050 30  0001 C CNN
F 3 "" H 9450 1200 60  0000 C CNN
F 4 "0603" H 9335 1180 30  0000 C CNN "SMT"
F 5 "2322070" H 9450 1000 30  0001 C CNN "Part"
F 6 "Farnell" H 9450 950 30  0001 C CNN "Provider"
	1    9450 1200
	-1   0    0    -1  
$EndComp
$Comp
L LED_SMT DL?
U 1 1 563203FE
P 9450 1500
F 0 "DL?" H 9450 1600 50  0000 C CNN
F 1 "GREEN" H 9450 1400 50  0000 C CNN
F 2 "LEDs:LED-0603" H 9450 1350 30  0001 C CNN
F 3 "" H 9450 1500 60  0000 C CNN
F 4 "0603" H 9335 1480 30  0000 C CNN "SMT"
F 5 "2322070" H 9450 1300 30  0001 C CNN "Part"
F 6 "Farnell" H 9450 1250 30  0001 C CNN "Provider"
	1    9450 1500
	-1   0    0    -1  
$EndComp
$Comp
L LED_SMT DL?
U 1 1 5632048F
P 9450 1800
F 0 "DL?" H 9450 1900 50  0000 C CNN
F 1 "RED" H 9450 1700 50  0000 C CNN
F 2 "LEDs:LED-0603" H 9450 1650 30  0001 C CNN
F 3 "" H 9450 1800 60  0000 C CNN
F 4 "0603" H 9335 1780 30  0000 C CNN "SMT"
F 5 "2322071" H 9450 1600 30  0001 C CNN "Part"
F 6 "Farnell" H 9450 1550 30  0001 C CNN "Provider"
	1    9450 1800
	-1   0    0    -1  
$EndComp
$Comp
L LED_SMT DL?
U 1 1 563205A7
P 9450 2100
F 0 "DL?" H 9450 2200 50  0000 C CNN
F 1 "RED" H 9450 2000 50  0000 C CNN
F 2 "LEDs:LED-0603" H 9450 1950 30  0001 C CNN
F 3 "" H 9450 2100 60  0000 C CNN
F 4 "0603" H 9335 2080 30  0000 C CNN "SMT"
F 5 "2322071" H 9450 1900 30  0001 C CNN "Part"
F 6 "Farnell" H 9450 1850 30  0001 C CNN "Provider"
	1    9450 2100
	-1   0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 5652D1AB
P 5850 1200
F 0 "R?" V 5920 1200 50  0000 C CNN
F 1 "100R/5%" V 5780 1200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6000 1200 30  0001 C CNN
F 3 "" V 5930 1200 30  0000 C CNN
F 4 "0603" V 5850 1200 20  0000 C CNN "SMT"
F 5 "9331689" V 6050 1200 30  0001 C CNN "Part"
F 6 "Farnell" V 6100 1200 30  0001 C CNN "Provider"
	1    5850 1200
	0    1    1    0   
$EndComp
$Comp
L C_SMT C?
U 1 1 5652D1B4
P 5100 1450
F 0 "C?" H 5125 1550 50  0000 L CNN
F 1 "100nF/50V" H 5125 1350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5130 1300 30  0001 L CNN
F 3 "" H 5125 1550 60  0000 L CNN
F 4 "0603" H 5100 1450 20  0000 C CNN "SMT"
F 5 "1414028" H 5130 1260 30  0001 L CNN "Part"
F 6 "Farnell" H 5130 1220 30  0001 L CNN "Provider"
	1    5100 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 1700 3350 2250
Wire Wire Line
	3350 1700 3200 1700
Wire Wire Line
	9650 1500 10100 1500
Connection ~ 10100 1500
Wire Wire Line
	10100 1800 9650 1800
Connection ~ 10100 1800
Wire Wire Line
	10100 2100 9650 2100
Wire Wire Line
	8900 1200 9250 1200
Wire Wire Line
	8900 1500 9250 1500
Wire Wire Line
	8900 1800 9250 1800
Wire Wire Line
	8900 2100 9250 2100
Wire Wire Line
	8200 1200 8600 1200
Wire Wire Line
	8200 1500 8600 1500
Wire Wire Line
	8200 1800 8600 1800
Wire Wire Line
	8200 2100 8600 2100
Wire Wire Line
	1350 1700 1950 1700
Wire Wire Line
	6300 1700 6150 1700
Wire Wire Line
	6300 1050 6300 1800
Wire Wire Line
	10100 1200 10100 2400
Connection ~ 10100 2100
Wire Wire Line
	2150 5700 2150 5600
Wire Wire Line
	1850 5150 2600 5150
Wire Wire Line
	2150 5050 2150 5300
Connection ~ 2150 5150
Wire Wire Line
	1550 5150 1350 5150
Wire Wire Line
	2150 4750 2150 4650
Wire Wire Line
	2150 4650 2750 4650
Wire Wire Line
	3050 4650 3350 4650
Wire Wire Line
	3350 4500 3350 5250
Wire Wire Line
	3350 5150 3200 5150
Connection ~ 3350 4650
Wire Wire Line
	3350 5250 3200 5250
Connection ~ 3350 5150
Wire Wire Line
	2600 5250 2450 5250
Wire Wire Line
	2450 5250 2450 5150
Connection ~ 2450 5150
Wire Wire Line
	2150 3950 2150 3850
Wire Wire Line
	1850 3400 2600 3400
Wire Wire Line
	2150 3300 2150 3550
Connection ~ 2150 3400
Wire Wire Line
	1550 3400 1350 3400
Wire Wire Line
	2150 3000 2150 2900
Wire Wire Line
	2150 2900 2750 2900
Wire Wire Line
	3050 2900 3350 2900
Wire Wire Line
	3350 2750 3350 3500
Wire Wire Line
	3350 3400 3200 3400
Connection ~ 3350 2900
Wire Wire Line
	3350 3500 3200 3500
Connection ~ 3350 3400
Wire Wire Line
	2600 3500 2450 3500
Wire Wire Line
	2450 3500 2450 3400
Connection ~ 2450 3400
Wire Wire Line
	2150 7400 2150 7300
Wire Wire Line
	1850 6850 2600 6850
Wire Wire Line
	2150 6750 2150 7000
Connection ~ 2150 6850
Wire Wire Line
	1550 6850 1350 6850
Wire Wire Line
	2150 6450 2150 6350
Wire Wire Line
	2150 6350 2750 6350
Wire Wire Line
	3050 6350 3350 6350
Wire Wire Line
	3350 6200 3350 6950
Wire Wire Line
	3350 6850 3200 6850
Connection ~ 3350 6350
Wire Wire Line
	3350 6950 3200 6950
Connection ~ 3350 6850
Wire Wire Line
	2600 6950 2450 6950
Wire Wire Line
	2450 6950 2450 6850
Connection ~ 2450 6850
Wire Wire Line
	5100 3950 5100 3850
Wire Wire Line
	4800 3400 5550 3400
Wire Wire Line
	5100 3300 5100 3550
Connection ~ 5100 3400
Wire Wire Line
	4500 3400 4300 3400
Wire Wire Line
	5100 3000 5100 2900
Wire Wire Line
	5100 2900 5700 2900
Wire Wire Line
	6000 2900 6300 2900
Wire Wire Line
	6300 2750 6300 3500
Wire Wire Line
	6300 3400 6150 3400
Connection ~ 6300 2900
Wire Wire Line
	6300 3500 6150 3500
Connection ~ 6300 3400
Wire Wire Line
	5550 3500 5400 3500
Wire Wire Line
	5400 3500 5400 3400
Connection ~ 5400 3400
Wire Wire Line
	5100 5700 5100 5600
Wire Wire Line
	4800 5150 5550 5150
Wire Wire Line
	5100 5050 5100 5300
Connection ~ 5100 5150
Wire Wire Line
	4500 5150 4300 5150
Wire Wire Line
	5100 4750 5100 4650
Wire Wire Line
	5100 4650 5700 4650
Wire Wire Line
	6000 4650 6300 4650
Wire Wire Line
	6300 4500 6300 5250
Wire Wire Line
	6300 5150 6150 5150
Connection ~ 6300 4650
Wire Wire Line
	6300 5250 6150 5250
Connection ~ 6300 5150
Wire Wire Line
	5550 5250 5400 5250
Wire Wire Line
	5400 5250 5400 5150
Connection ~ 5400 5150
Wire Wire Line
	6300 1800 6150 1800
Connection ~ 6300 1700
Wire Wire Line
	5550 1800 5400 1800
Wire Wire Line
	5400 1800 5400 1700
Connection ~ 5400 1700
Wire Wire Line
	3200 1800 3350 1800
Connection ~ 3350 1800
Wire Wire Line
	2600 1800 2450 1800
Wire Wire Line
	2450 1800 2450 1700
Connection ~ 2450 1700
Wire Wire Line
	9650 1200 10100 1200
Wire Wire Line
	5100 1300 5100 1200
Wire Wire Line
	5100 1200 5700 1200
Wire Wire Line
	6000 1200 6300 1200
Connection ~ 5100 1700
Connection ~ 6300 1200
$Comp
L R_SMT R?
U 1 1 5631DE54
P 5100 2000
F 0 "R?" V 5170 2000 50  0000 C CNN
F 1 "10K/5%" V 5030 2000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5250 2000 30  0001 C CNN
F 3 "" V 5180 2000 30  0000 C CNN
F 4 "0603" V 5100 2000 20  0000 C CNN "SMT"
F 5 "9331700" V 5300 2000 30  0001 C CNN "Part"
F 6 "Farnell" V 5350 2000 30  0001 C CNN "Provider"
	1    5100 2000
	-1   0    0    1   
$EndComp
Wire Wire Line
	5100 2250 5100 2150
Wire Wire Line
	5100 1600 5100 1850
$Comp
L GND #PWR?
U 1 1 5622D3C9
P 5100 2250
F 0 "#PWR?" H 5100 2000 50  0001 C CNN
F 1 "GND" H 5100 2100 50  0000 C CNN
F 2 "" H 5100 2250 60  0000 C CNN
F 3 "" H 5100 2250 60  0000 C CNN
	1    5100 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 1700 4500 1700
Wire Wire Line
	4800 1700 5550 1700
$Comp
L R_SMT R?
U 1 1 56537F92
P 1700 3400
F 0 "R?" V 1770 3400 50  0000 C CNN
F 1 "100R/5%" V 1630 3400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1850 3400 30  0001 C CNN
F 3 "" V 1780 3400 30  0000 C CNN
F 4 "0603" V 1700 3400 20  0000 C CNN "SMT"
F 5 "9331689" V 1900 3400 30  0001 C CNN "Part"
F 6 "Farnell" V 1950 3400 30  0001 C CNN "Provider"
	1    1700 3400
	0    1    1    0   
$EndComp
$Comp
L R_SMT R?
U 1 1 56538116
P 1700 5150
F 0 "R?" V 1770 5150 50  0000 C CNN
F 1 "100R/5%" V 1630 5150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1850 5150 30  0001 C CNN
F 3 "" V 1780 5150 30  0000 C CNN
F 4 "0603" V 1700 5150 20  0000 C CNN "SMT"
F 5 "9331689" V 1900 5150 30  0001 C CNN "Part"
F 6 "Farnell" V 1950 5150 30  0001 C CNN "Provider"
	1    1700 5150
	0    1    1    0   
$EndComp
$Comp
L R_SMT R?
U 1 1 565382A2
P 1700 6850
F 0 "R?" V 1770 6850 50  0000 C CNN
F 1 "100R/5%" V 1630 6850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1850 6850 30  0001 C CNN
F 3 "" V 1780 6850 30  0000 C CNN
F 4 "0603" V 1700 6850 20  0000 C CNN "SMT"
F 5 "9331689" V 1900 6850 30  0001 C CNN "Part"
F 6 "Farnell" V 1950 6850 30  0001 C CNN "Provider"
	1    1700 6850
	0    1    1    0   
$EndComp
$Comp
L R_SMT R?
U 1 1 5653837D
P 4650 5150
F 0 "R?" V 4720 5150 50  0000 C CNN
F 1 "100R/5%" V 4580 5150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4800 5150 30  0001 C CNN
F 3 "" V 4730 5150 30  0000 C CNN
F 4 "0603" V 4650 5150 20  0000 C CNN "SMT"
F 5 "9331689" V 4850 5150 30  0001 C CNN "Part"
F 6 "Farnell" V 4900 5150 30  0001 C CNN "Provider"
	1    4650 5150
	0    1    1    0   
$EndComp
$Comp
L R_SMT R?
U 1 1 56538499
P 4650 3400
F 0 "R?" V 4720 3400 50  0000 C CNN
F 1 "100R/5%" V 4580 3400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4800 3400 30  0001 C CNN
F 3 "" V 4730 3400 30  0000 C CNN
F 4 "0603" V 4650 3400 20  0000 C CNN "SMT"
F 5 "9331689" V 4850 3400 30  0001 C CNN "Part"
F 6 "Farnell" V 4900 3400 30  0001 C CNN "Provider"
	1    4650 3400
	0    1    1    0   
$EndComp
$Comp
L R_SMT R?
U 1 1 5653857A
P 4650 1700
F 0 "R?" V 4720 1700 50  0000 C CNN
F 1 "100R/5%" V 4580 1700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4800 1700 30  0001 C CNN
F 3 "" V 4730 1700 30  0000 C CNN
F 4 "0603" V 4650 1700 20  0000 C CNN "SMT"
F 5 "9331689" V 4850 1700 30  0001 C CNN "Part"
F 6 "Farnell" V 4900 1700 30  0001 C CNN "Provider"
	1    4650 1700
	0    1    1    0   
$EndComp
$Comp
L R_SMT R?
U 1 1 56538917
P 2150 3700
F 0 "R?" V 2220 3700 50  0000 C CNN
F 1 "100K/5%" V 2080 3700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2300 3700 30  0001 C CNN
F 3 "" V 2230 3700 30  0000 C CNN
F 4 "0603" V 2150 3700 20  0000 C CNN "SMT"
F 5 "9233628" V 2350 3700 30  0001 C CNN "Part"
F 6 "Farnell" V 2400 3700 30  0001 C CNN "Provider"
	1    2150 3700
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 565389F7
P 2150 5450
F 0 "R?" V 2220 5450 50  0000 C CNN
F 1 "100K/5%" V 2080 5450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2300 5450 30  0001 C CNN
F 3 "" V 2230 5450 30  0000 C CNN
F 4 "0603" V 2150 5450 20  0000 C CNN "SMT"
F 5 "9233628" V 2350 5450 30  0001 C CNN "Part"
F 6 "Farnell" V 2400 5450 30  0001 C CNN "Provider"
	1    2150 5450
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56538B8F
P 2150 7150
F 0 "R?" V 2220 7150 50  0000 C CNN
F 1 "100K/5%" V 2080 7150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2300 7150 30  0001 C CNN
F 3 "" V 2230 7150 30  0000 C CNN
F 4 "0603" V 2150 7150 20  0000 C CNN "SMT"
F 5 "9233628" V 2350 7150 30  0001 C CNN "Part"
F 6 "Farnell" V 2400 7150 30  0001 C CNN "Provider"
	1    2150 7150
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56538C61
P 5100 5450
F 0 "R?" V 5170 5450 50  0000 C CNN
F 1 "100K/5%" V 5030 5450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5250 5450 30  0001 C CNN
F 3 "" V 5180 5450 30  0000 C CNN
F 4 "0603" V 5100 5450 20  0000 C CNN "SMT"
F 5 "9233628" V 5300 5450 30  0001 C CNN "Part"
F 6 "Farnell" V 5350 5450 30  0001 C CNN "Provider"
	1    5100 5450
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56538D76
P 5100 3700
F 0 "R?" V 5170 3700 50  0000 C CNN
F 1 "100K/5%" V 5030 3700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5250 3700 30  0001 C CNN
F 3 "" V 5180 3700 30  0000 C CNN
F 4 "0603" V 5100 3700 20  0000 C CNN "SMT"
F 5 "9233628" V 5300 3700 30  0001 C CNN "Part"
F 6 "Farnell" V 5350 3700 30  0001 C CNN "Provider"
	1    5100 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 1700 2600 1700
$EndSCHEMATC
