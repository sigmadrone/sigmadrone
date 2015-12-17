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
Sheet 13 26
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 2000 4400 0    60   BiDi ~ 0
USG_OTG_VBUS
Text HLabel 2000 4500 0    60   BiDi ~ 0
USB_OGT_DM
Text HLabel 2000 4600 0    60   BiDi ~ 0
USB_OTG_DP
Text HLabel 2000 4700 0    60   BiDi ~ 0
USB_OTG_ID
Text HLabel 2000 2600 0    60   Output ~ 0
USB_OTG_OC
Text HLabel 2000 2500 0    60   Input ~ 0
USB_OTG_PWR_EN
$Comp
L VBUS053CZ-HAF-G-08 U?
U 1 1 56358133
P 5300 3700
F 0 "U?" H 5450 4000 60  0000 C CNN
F 1 "VBUS053CZ-HAF-G-08" H 5300 3400 60  0000 C CNN
F 2 "" H 5250 4400 60  0001 C CNN
F 3 "" H 5250 4400 60  0000 C CNN
	1    5300 3700
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR?
U 1 1 5635813A
P 3600 1400
F 0 "#PWR?" H 3600 1250 50  0001 C CNN
F 1 "+3V3" H 3600 1540 50  0000 C CNN
F 2 "" H 3600 1400 60  0000 C CNN
F 3 "" H 3600 1400 60  0000 C CNN
	1    3600 1400
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR?
U 1 1 56358140
P 3950 1400
F 0 "#PWR?" H 3950 1250 50  0001 C CNN
F 1 "+3V3" H 3950 1540 50  0000 C CNN
F 2 "" H 3950 1400 60  0000 C CNN
F 3 "" H 3950 1400 60  0000 C CNN
	1    3950 1400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 56358146
P 7350 3300
F 0 "#PWR?" H 7350 3050 50  0001 C CNN
F 1 "GND" H 7350 3150 50  0000 C CNN
F 2 "" H 7350 3300 60  0000 C CNN
F 3 "" H 7350 3300 60  0000 C CNN
	1    7350 3300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5635814C
P 6100 2900
F 0 "#PWR?" H 6100 2650 50  0001 C CNN
F 1 "GND" H 6100 2750 50  0000 C CNN
F 2 "" H 6100 2900 60  0000 C CNN
F 3 "" H 6100 2900 60  0000 C CNN
	1    6100 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 56358152
P 6100 4150
F 0 "#PWR?" H 6100 3900 50  0001 C CNN
F 1 "GND" H 6100 4000 50  0000 C CNN
F 2 "" H 6100 4150 60  0000 C CNN
F 3 "" H 6100 4150 60  0000 C CNN
	1    6100 4150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 56358158
P 9300 5650
F 0 "#PWR?" H 9300 5400 50  0001 C CNN
F 1 "GND" H 9300 5500 50  0000 C CNN
F 2 "" H 9300 5650 60  0000 C CNN
F 3 "" H 9300 5650 60  0000 C CNN
	1    9300 5650
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 5635815E
P 4300 1400
F 0 "#PWR?" H 4300 1250 50  0001 C CNN
F 1 "+5V" H 4300 1540 50  0000 C CNN
F 2 "" H 4300 1400 60  0000 C CNN
F 3 "" H 4300 1400 60  0000 C CNN
	1    4300 1400
	1    0    0    -1  
$EndComp
$Comp
L TPS2061CDBVT U?
U 1 1 56358164
P 5300 2500
F 0 "U?" H 5550 2750 60  0000 C CNN
F 1 "TPS2061CDBVT" H 5300 2250 60  0000 C CNN
F 2 "" H 5300 2300 60  0001 C CNN
F 3 "" H 5300 2300 60  0000 C CNN
	1    5300 2500
	1    0    0    -1  
$EndComp
NoConn ~ 5800 3650
NoConn ~ 5800 3750
$Comp
L R_SMT R?
U 1 1 56358170
P 3600 1650
F 0 "R?" V 3670 1650 50  0000 C CNN
F 1 "10K/5%" V 3530 1650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3750 1650 30  0001 C CNN
F 3 "" V 3680 1650 30  0000 C CNN
F 4 "0603" V 3600 1650 20  0000 C CNN "SMT"
F 5 "9331700" V 3800 1650 30  0001 C CNN "Part"
F 6 "Farnell" V 3850 1650 30  0001 C CNN "Provider"
	1    3600 1650
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 5635817A
P 3950 1650
F 0 "R?" V 4020 1650 50  0000 C CNN
F 1 "10K/5%" V 3880 1650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4100 1650 30  0001 C CNN
F 3 "" V 4030 1650 30  0000 C CNN
F 4 "0603" V 3950 1650 20  0000 C CNN "SMT"
F 5 "9331700" V 4150 1650 30  0001 C CNN "Part"
F 6 "Farnell" V 4200 1650 30  0001 C CNN "Provider"
	1    3950 1650
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C?
U 1 1 56358184
P 7350 2650
F 0 "C?" H 7375 2750 50  0000 L CNN
F 1 "10uF/16V/X5R" V 7200 2300 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7380 2500 30  0001 L CNN
F 3 "" H 7375 2750 60  0000 L CNN
F 4 "0805" H 7350 2650 20  0000 C CNN "SMT"
F 5 "1762635" H 7380 2460 30  0001 L CNN "Part"
F 6 "Farnell" H 7380 2420 30  0001 L CNN "Provider"
	1    7350 2650
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C?
U 1 1 5635818E
P 7850 2650
F 0 "C?" H 7875 2750 50  0000 L CNN
F 1 "100nF/50V/X7R" V 7700 2250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7880 2500 30  0001 L CNN
F 3 "" H 7875 2750 60  0000 L CNN
F 4 "0603" H 7850 2650 20  0000 C CNN "SMT"
F 5 "1414028" H 7880 2460 30  0001 L CNN "Part"
F 6 "Farnell" H 7880 2420 30  0001 L CNN "Provider"
	1    7850 2650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 56358195
P 7850 3300
F 0 "#PWR?" H 7850 3050 50  0001 C CNN
F 1 "GND" H 7850 3150 50  0000 C CNN
F 2 "" H 7850 3300 60  0000 C CNN
F 3 "" H 7850 3300 60  0000 C CNN
	1    7850 3300
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 5635819E
P 3350 4500
F 0 "R?" V 3300 4700 50  0000 C CNN
F 1 "22R/5%" V 3300 4250 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3500 4500 30  0001 C CNN
F 3 "" V 3430 4500 30  0000 C CNN
F 4 "0603" V 3350 4500 20  0000 C CNN "SMT"
F 5 "9331891" V 3550 4500 30  0001 C CNN "Part"
F 6 "Farnell" V 3600 4500 30  0001 C CNN "Provider"
	1    3350 4500
	0    1    1    0   
$EndComp
$Comp
L R_SMT R?
U 1 1 563581A8
P 3350 4600
F 0 "R?" V 3300 4800 50  0000 C CNN
F 1 "22R/5%" V 3300 4350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3500 4600 30  0001 C CNN
F 3 "" V 3430 4600 30  0000 C CNN
F 4 "0603" V 3350 4600 20  0000 C CNN "SMT"
F 5 "9331891" V 3550 4600 30  0001 C CNN "Part"
F 6 "Farnell" V 3600 4600 30  0001 C CNN "Provider"
	1    3350 4600
	0    1    1    0   
$EndComp
$Comp
L R_SMT R?
U 1 1 563581B2
P 8300 2650
F 0 "R?" V 8370 2650 50  0000 C CNN
F 1 "1k2/5%" V 8230 2650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8450 2650 30  0001 C CNN
F 3 "" V 8380 2650 30  0000 C CNN
F 4 "0603" V 8300 2650 20  0000 C CNN "SMT"
F 5 "9331751" V 8500 2650 30  0001 C CNN "Part"
F 6 "Farnell" V 8550 2650 30  0001 C CNN "Provider"
	1    8300 2650
	-1   0    0    1   
$EndComp
$Comp
L LED_SMT DL?
U 1 1 563581BC
P 8300 3050
F 0 "DL?" H 8300 3150 50  0000 C CNN
F 1 "GREEN" H 8300 2950 50  0000 C CNN
F 2 "LEDs:LED-0603" H 8300 2900 30  0001 C CNN
F 3 "" H 8300 3050 60  0000 C CNN
F 4 "0603" H 8185 3030 30  0000 C CNN "SMT"
F 5 "2322070" H 8300 2850 30  0001 C CNN "Part"
F 6 "Farnell" H 8300 2800 30  0001 C CNN "Provider"
	1    8300 3050
	0    -1   -1   0   
$EndComp
$Comp
L FB_SMT FB?
U 1 1 563581C6
P 6550 2400
F 0 "FB?" H 6550 2500 50  0000 C CNN
F 1 "BEAD/0.045OHM/2A" H 6550 2350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" H 6550 2300 30  0001 C CNN
F 3 "" H 6600 2400 60  0000 C CNN
F 4 "0603" H 6550 2400 25  0000 C CNN "SMT"
F 5 "1515714" H 6550 2250 30  0001 C CNN "Part"
F 6 "Farnell" H 6550 2200 30  0001 C CNN "Provider"
	1    6550 2400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 563581CD
P 8300 3300
F 0 "#PWR?" H 8300 3050 50  0001 C CNN
F 1 "GND" H 8300 3150 50  0000 C CNN
F 2 "" H 8300 3300 60  0000 C CNN
F 3 "" H 8300 3300 60  0000 C CNN
	1    8300 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 4500 3200 4500
Wire Wire Line
	2000 4600 3200 4600
Wire Wire Line
	3500 4500 9450 4500
Wire Wire Line
	3500 4600 9450 4600
Wire Wire Line
	2000 4400 9450 4400
Wire Wire Line
	2000 4700 9450 4700
Wire Wire Line
	2000 2500 4800 2500
Wire Wire Line
	2000 2600 4800 2600
Wire Wire Line
	3600 1400 3600 1500
Wire Wire Line
	3950 1400 3950 1500
Wire Wire Line
	3600 1800 3600 2500
Connection ~ 3600 2500
Wire Wire Line
	3950 1800 3950 2600
Connection ~ 3950 2600
Wire Wire Line
	8300 2800 8300 2850
Wire Wire Line
	8300 3250 8300 3300
Wire Wire Line
	6750 2400 8750 2400
Wire Wire Line
	7350 3300 7350 2800
Wire Wire Line
	7350 2500 7350 2400
Connection ~ 7350 2400
Wire Wire Line
	5800 3850 6100 3850
Wire Wire Line
	6100 3850 6100 4150
Wire Wire Line
	4800 3850 4250 3850
Wire Wire Line
	4250 3850 4250 5150
Connection ~ 4250 4700
Wire Wire Line
	4800 3750 4150 3750
Wire Wire Line
	4150 3750 4150 4600
Connection ~ 4150 4600
Wire Wire Line
	4800 3650 4050 3650
Wire Wire Line
	4050 3650 4050 4500
Connection ~ 4050 4500
Wire Wire Line
	4300 1400 4300 2400
Wire Wire Line
	4300 2400 4800 2400
Wire Wire Line
	5800 2600 6100 2600
Wire Wire Line
	6100 2600 6100 2900
Wire Wire Line
	4800 3550 3950 3550
Wire Wire Line
	3950 3550 3950 4400
Connection ~ 3950 4400
Wire Wire Line
	7850 3300 7850 2800
Wire Wire Line
	7850 2500 7850 2400
Connection ~ 7850 2400
Wire Wire Line
	5800 2400 6350 2400
Wire Wire Line
	8300 2400 8300 2500
Wire Wire Line
	8750 2400 8750 4400
Connection ~ 8750 4400
Connection ~ 8300 2400
$Comp
L GND #PWR?
U 1 1 56358213
P 2250 5650
F 0 "#PWR?" H 2250 5400 50  0001 C CNN
F 1 "GND" H 2250 5500 50  0000 C CNN
F 2 "" H 2250 5650 60  0000 C CNN
F 3 "" H 2250 5650 60  0000 C CNN
	1    2250 5650
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C?
U 1 1 5635821C
P 2250 5300
F 0 "C?" H 2275 5400 50  0000 L CNN
F 1 "47pF/100V/C0G" V 2100 4950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2280 5150 30  0001 L CNN
F 3 "" H 2275 5400 60  0000 L CNN
F 4 "0603" H 2250 5300 20  0000 C CNN "SMT"
F 5 "1813433" H 2280 5110 30  0001 L CNN "Part"
F 6 "Farnell" H 2280 5070 30  0001 L CNN "Provider"
	1    2250 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 5650 2250 5450
$Comp
L GND #PWR?
U 1 1 56358224
P 2650 5650
F 0 "#PWR?" H 2650 5400 50  0001 C CNN
F 1 "GND" H 2650 5500 50  0000 C CNN
F 2 "" H 2650 5650 60  0000 C CNN
F 3 "" H 2650 5650 60  0000 C CNN
	1    2650 5650
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C?
U 1 1 5635822D
P 2650 5300
F 0 "C?" H 2675 5400 50  0000 L CNN
F 1 "47pF/100V/C0G" V 2500 4950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2680 5150 30  0001 L CNN
F 3 "" H 2675 5400 60  0000 L CNN
F 4 "0603" H 2650 5300 20  0000 C CNN "SMT"
F 5 "1813433" H 2680 5110 30  0001 L CNN "Part"
F 6 "Farnell" H 2680 5070 30  0001 L CNN "Provider"
	1    2650 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 5650 2650 5450
Wire Wire Line
	2250 5150 2250 4600
Connection ~ 2250 4600
Wire Wire Line
	2650 5150 2650 4500
Connection ~ 2650 4500
$Comp
L R_SMT R?
U 1 1 56359C77
P 4250 5300
F 0 "R?" V 4320 5300 50  0000 C CNN
F 1 "NA" V 4180 5300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4400 5300 30  0001 C CNN
F 3 "" V 4330 5300 30  0000 C CNN
F 4 "0603" V 4250 5300 20  0000 C CNN "SMT"
F 5 "1469739" V 4450 5300 30  0001 C CNN "Part"
F 6 "Farnell" V 4500 5300 30  0001 C CNN "Provider"
	1    4250 5300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5635A01C
P 4250 5650
F 0 "#PWR?" H 4250 5400 50  0001 C CNN
F 1 "GND" H 4250 5500 50  0000 C CNN
F 2 "" H 4250 5650 60  0000 C CNN
F 3 "" H 4250 5650 60  0000 C CNN
	1    4250 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 5450 4250 5650
$Comp
L 629_105_150_921 CN?
U 1 1 5635B973
P 9850 4900
F 0 "CN?" H 10000 5550 60  0000 C CNN
F 1 "629_105_150_921" V 10100 4750 60  0000 C CNN
F 2 "" H 11400 6200 60  0000 C CNN
F 3 "" H 11400 6200 60  0000 C CNN
	1    9850 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 4800 9300 4800
Wire Wire Line
	9300 4800 9300 5650
Wire Wire Line
	9450 4900 9300 4900
Connection ~ 9300 4900
Connection ~ 9300 5000
Connection ~ 9300 5100
Connection ~ 9300 5200
Connection ~ 9300 5300
Connection ~ 9300 5400
Wire Wire Line
	9450 5000 9300 5000
Wire Wire Line
	9450 5100 9300 5100
Wire Wire Line
	9450 5200 9300 5200
Wire Wire Line
	9450 5300 9300 5300
Wire Wire Line
	9450 5400 9300 5400
$EndSCHEMATC
