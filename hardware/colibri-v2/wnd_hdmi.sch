EESchema Schematic File Version 2
LIBS:colibri-wnd-rescue
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
L TPD12S521DBTR U8
U 1 1 56184875
P 5750 3500
F 0 "U8" H 6450 4550 60  0000 C CNN
F 1 "TPD12S521DBTR" H 6250 2450 60  0000 C CNN
F 2 "" H 5750 3000 60  0001 C CNN
F 3 "" H 5750 3000 60  0000 C CNN
	1    5750 3500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0117
U 1 1 56186A61
P 9350 5550
F 0 "#PWR0117" H 9350 5300 50  0001 C CNN
F 1 "GND" H 9350 5400 50  0000 C CNN
F 2 "" H 9350 5550 60  0000 C CNN
F 3 "" H 9350 5550 60  0000 C CNN
	1    9350 5550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0118
U 1 1 56186DD2
P 7400 1300
F 0 "#PWR0118" H 7400 1050 50  0001 C CNN
F 1 "GND" H 7400 1150 50  0000 C CNN
F 2 "" H 7400 1300 60  0000 C CNN
F 3 "" H 7400 1300 60  0000 C CNN
	1    7400 1300
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR0119
U 1 1 5618793A
P 7000 5550
F 0 "#PWR0119" H 7000 5300 50  0001 C CNN
F 1 "GND" H 7000 5400 50  0000 C CNN
F 2 "" H 7000 5550 60  0000 C CNN
F 3 "" H 7000 5550 60  0000 C CNN
	1    7000 5550
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR0120
U 1 1 56187AA3
P 8250 1300
F 0 "#PWR0120" H 8250 1150 50  0001 C CNN
F 1 "+5V" H 8250 1440 50  0000 C CNN
F 2 "" H 8250 1300 60  0000 C CNN
F 3 "" H 8250 1300 60  0000 C CNN
	1    8250 1300
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR0121
U 1 1 56187ACA
P 8550 1300
F 0 "#PWR0121" H 8550 1150 50  0001 C CNN
F 1 "+5V" H 8550 1440 50  0000 C CNN
F 2 "" H 8550 1300 60  0000 C CNN
F 3 "" H 8550 1300 60  0000 C CNN
	1    8550 1300
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR0122
U 1 1 56187AE5
P 8850 1300
F 0 "#PWR0122" H 8850 1150 50  0001 C CNN
F 1 "+3V3" H 8850 1440 50  0000 C CNN
F 2 "" H 8850 1300 60  0000 C CNN
F 3 "" H 8850 1300 60  0000 C CNN
	1    8850 1300
	1    0    0    -1  
$EndComp
$Comp
L 685_119_136_923 CN4
U 1 1 56188483
P 10150 4000
F 0 "CN4" H 10450 5550 60  0000 C CNN
F 1 "685_119_136_923" H 10150 2650 60  0000 C CNN
F 2 "" H 10000 4200 60  0001 C CNN
F 3 "" H 10000 4200 60  0000 C CNN
	1    10150 4000
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR0123
U 1 1 561892D1
P 3950 1250
F 0 "#PWR0123" H 3950 1100 50  0001 C CNN
F 1 "+5V" H 3950 1390 50  0000 C CNN
F 2 "" H 3950 1250 60  0000 C CNN
F 3 "" H 3950 1250 60  0000 C CNN
	1    3950 1250
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR0124
U 1 1 56189387
P 3650 1250
F 0 "#PWR0124" H 3650 1100 50  0001 C CNN
F 1 "+3V3" H 3650 1390 50  0000 C CNN
F 2 "" H 3650 1250 60  0000 C CNN
F 3 "" H 3650 1250 60  0000 C CNN
	1    3650 1250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0125
U 1 1 5618958B
P 4550 1250
F 0 "#PWR0125" H 4550 1000 50  0001 C CNN
F 1 "GND" H 4550 1100 50  0000 C CNN
F 2 "" H 4550 1250 60  0000 C CNN
F 3 "" H 4550 1250 60  0000 C CNN
	1    4550 1250
	-1   0    0    1   
$EndComp
Text HLabel 1650 2900 0    60   Input ~ 0
HDMI_D2+
Text HLabel 1650 3100 0    60   Input ~ 0
HDMI_D2-
Text HLabel 1650 3200 0    60   Input ~ 0
HDMI_D1+
Text HLabel 1650 3400 0    60   Input ~ 0
HDMI_D1-
Text HLabel 1650 3500 0    60   Input ~ 0
HDMI_D0+
Text HLabel 1650 3700 0    60   Input ~ 0
HDMI_D0-
Text HLabel 1650 3800 0    60   Input ~ 0
HDMI_CLK+
Text HLabel 1650 4000 0    60   Input ~ 0
HDMI_CLK-
Text HLabel 1650 4100 0    60   Input ~ 0
HDMI_CEC_IN
Text HLabel 1650 4200 0    60   Input ~ 0
DDC_SCL(5V)
Text HLabel 1650 4300 0    60   BiDi ~ 0
DDC_SDA(5V)
Text HLabel 1650 4400 0    60   Input ~ 0
HDMI1_HPD
$Comp
L +5V #PWR0126
U 1 1 56189FA2
P 3350 1250
F 0 "#PWR0126" H 3350 1100 50  0001 C CNN
F 1 "+5V" H 3350 1390 50  0000 C CNN
F 2 "" H 3350 1250 60  0000 C CNN
F 3 "" H 3350 1250 60  0000 C CNN
	1    3350 1250
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR0127
U 1 1 5618A003
P 3050 1250
F 0 "#PWR0127" H 3050 1100 50  0001 C CNN
F 1 "+5V" H 3050 1390 50  0000 C CNN
F 2 "" H 3050 1250 60  0000 C CNN
F 3 "" H 3050 1250 60  0000 C CNN
	1    3050 1250
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR0128
U 1 1 5618A158
P 2750 1250
F 0 "#PWR0128" H 2750 1100 50  0001 C CNN
F 1 "+3V3" H 2750 1390 50  0000 C CNN
F 2 "" H 2750 1250 60  0000 C CNN
F 3 "" H 2750 1250 60  0000 C CNN
	1    2750 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 2900 9550 2900
Wire Wire Line
	6850 3000 9550 3000
Wire Wire Line
	6850 3100 9550 3100
Wire Wire Line
	6850 3200 9550 3200
Wire Wire Line
	6850 3300 9550 3300
Wire Wire Line
	6850 3400 9550 3400
Wire Wire Line
	6850 3500 9550 3500
Wire Wire Line
	6850 3600 9550 3600
Wire Wire Line
	6850 3700 9550 3700
Wire Wire Line
	6850 3800 9550 3800
Wire Wire Line
	6850 3900 9550 3900
Wire Wire Line
	6850 4000 9550 4000
Wire Wire Line
	6850 4100 9550 4100
Wire Wire Line
	6850 4200 9550 4200
Wire Wire Line
	6850 4300 9550 4300
Wire Wire Line
	6850 4400 9550 4400
Wire Wire Line
	9350 4600 9550 4600
Wire Wire Line
	9350 3000 9350 5550
Wire Wire Line
	9550 4900 9350 4900
Connection ~ 9350 4900
Connection ~ 9350 5000
Connection ~ 9350 5100
Connection ~ 9350 5200
Wire Wire Line
	9550 5000 9350 5000
Wire Wire Line
	9550 5100 9350 5100
Wire Wire Line
	9550 5200 9350 5200
Connection ~ 9350 3000
Connection ~ 9350 4600
Connection ~ 9350 3300
Connection ~ 9350 3600
Connection ~ 9350 3900
Wire Wire Line
	6850 2600 9550 2600
Wire Wire Line
	7400 2600 7400 2300
Wire Wire Line
	6850 2700 7000 2700
Wire Wire Line
	7400 1300 7400 2000
Connection ~ 7400 2600
Wire Wire Line
	7000 2700 7000 2300
Wire Wire Line
	7000 2000 7000 1850
Wire Wire Line
	7000 1850 7400 1850
Connection ~ 7400 1850
Wire Wire Line
	6850 2800 7000 2800
Wire Wire Line
	7000 2800 7000 5550
Connection ~ 7000 3000
Connection ~ 7000 3300
Connection ~ 7000 3600
Connection ~ 7000 3900
Wire Wire Line
	8550 2350 8550 4200
Connection ~ 8550 4200
Wire Wire Line
	8250 2050 8250 1300
Wire Wire Line
	8550 2050 8550 1300
Wire Wire Line
	8850 2050 8850 1300
Wire Wire Line
	3650 2000 3650 1250
Wire Wire Line
	3650 2300 3650 2700
Wire Wire Line
	3650 2700 4650 2700
Wire Wire Line
	3950 1250 3950 2600
Wire Wire Line
	3950 2600 4650 2600
Wire Wire Line
	4550 1250 4550 2000
Wire Wire Line
	4200 2000 4200 1850
Wire Wire Line
	4200 1850 4550 1850
Connection ~ 4550 1850
Wire Wire Line
	4200 2300 4200 2700
Connection ~ 4200 2700
Wire Wire Line
	4550 2300 4550 2600
Connection ~ 4550 2600
Wire Wire Line
	1650 2900 4650 2900
Wire Wire Line
	1650 3100 4650 3100
Wire Wire Line
	1650 3200 4650 3200
Wire Wire Line
	1650 3400 4650 3400
Wire Wire Line
	1650 3500 4650 3500
Wire Wire Line
	1650 3700 4650 3700
Wire Wire Line
	1650 3800 4650 3800
Wire Wire Line
	1650 4000 4650 4000
Wire Wire Line
	1650 4100 4650 4100
Wire Wire Line
	1650 4200 4650 4200
Wire Wire Line
	1650 4300 4650 4300
Wire Wire Line
	1650 4400 4650 4400
Wire Wire Line
	3350 2000 3350 1250
Wire Wire Line
	3050 2000 3050 1250
Wire Wire Line
	8250 2350 8250 4300
Connection ~ 8250 4300
Wire Wire Line
	8850 2350 8850 4100
Connection ~ 8850 4100
Wire Wire Line
	2750 2300 2750 4100
Connection ~ 2750 4100
Wire Wire Line
	3050 2300 3050 4200
Connection ~ 3050 4200
Wire Wire Line
	3350 2300 3350 4300
Connection ~ 3350 4300
$Comp
L GND #PWR0129
U 1 1 5618AE70
P 4500 5550
F 0 "#PWR0129" H 4500 5300 50  0001 C CNN
F 1 "GND" H 4500 5400 50  0000 C CNN
F 2 "" H 4500 5550 60  0000 C CNN
F 3 "" H 4500 5550 60  0000 C CNN
	1    4500 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 2800 4500 2800
Wire Wire Line
	4500 2800 4500 5550
Wire Wire Line
	4650 3000 4500 3000
Connection ~ 4500 3000
Wire Wire Line
	4650 3300 4500 3300
Connection ~ 4500 3300
Wire Wire Line
	4650 3600 4500 3600
Connection ~ 4500 3600
Wire Wire Line
	4650 3900 4500 3900
Connection ~ 4500 3900
$Comp
L D_SMT D11
U 1 1 5634CB2C
P 2750 1750
F 0 "D11" H 2750 1850 50  0000 C CNN
F 1 "BAS316" H 2750 1650 50  0000 C CNN
F 2 "Diodes_SMD:SOD-323" H 2750 1600 30  0001 C CNN
F 3 "" H 2750 1850 60  0000 C CNN
F 4 "SOD-323" H 2650 1780 30  0001 C CNN "SMT"
F 5 "8734275" H 2750 1550 30  0001 C CNN "Part"
F 6 "Farnell" H 2750 1500 30  0001 C CNN "Provider"
	1    2750 1750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2750 1250 2750 1600
Wire Wire Line
	2750 1900 2750 2000
$Comp
L C_SMT C41
U 1 1 5634DA0E
P 4550 2150
F 0 "C41" H 4575 2250 50  0000 L CNN
F 1 "100nF/50V/X7R" V 4400 1750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4580 2000 30  0001 L CNN
F 3 "" H 4575 2250 60  0000 L CNN
F 4 "0603" H 4550 2150 20  0000 C CNN "SMT"
F 5 "1414028" H 4580 1960 30  0001 L CNN "Part"
F 6 "Farnell" H 4580 1920 30  0001 L CNN "Provider"
	1    4550 2150
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C40
U 1 1 5634DAED
P 4200 2150
F 0 "C40" H 4225 2250 50  0000 L CNN
F 1 "100nF/50V/X7R" V 4050 1750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4230 2000 30  0001 L CNN
F 3 "" H 4225 2250 60  0000 L CNN
F 4 "0603" H 4200 2150 20  0000 C CNN "SMT"
F 5 "1414028" H 4230 1960 30  0001 L CNN "Part"
F 6 "Farnell" H 4230 1920 30  0001 L CNN "Provider"
	1    4200 2150
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C42
U 1 1 5634E308
P 7000 2150
F 0 "C42" H 7025 2250 50  0000 L CNN
F 1 "100nF/50V/X7R" V 6850 1750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7030 2000 30  0001 L CNN
F 3 "" H 7025 2250 60  0000 L CNN
F 4 "0603" H 7000 2150 20  0000 C CNN "SMT"
F 5 "1414028" H 7030 1960 30  0001 L CNN "Part"
F 6 "Farnell" H 7030 1920 30  0001 L CNN "Provider"
	1    7000 2150
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C43
U 1 1 5634E387
P 7400 2150
F 0 "C43" H 7425 2250 50  0000 L CNN
F 1 "100nF/50V/X7R" V 7250 1750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7430 2000 30  0001 L CNN
F 3 "" H 7425 2250 60  0000 L CNN
F 4 "0603" H 7400 2150 20  0000 C CNN "SMT"
F 5 "1414028" H 7430 1960 30  0001 L CNN "Part"
F 6 "Farnell" H 7430 1920 30  0001 L CNN "Provider"
	1    7400 2150
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R89
U 1 1 5634F18B
P 3650 2150
F 0 "R89" H 3700 2300 50  0000 C CNN
F 1 "10K/5%" V 3550 2150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3800 2150 30  0001 C CNN
F 3 "" V 3730 2150 30  0000 C CNN
F 4 "0603" V 3650 2150 20  0000 C CNN "SMT"
F 5 "9331700" V 3850 2150 30  0001 C CNN "Part"
F 6 "Farnell" V 3900 2150 30  0001 C CNN "Provider"
	1    3650 2150
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R88
U 1 1 5634F324
P 3350 2150
F 0 "R88" H 3400 2300 50  0000 C CNN
F 1 "2k2/5%" V 3250 2150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3500 2150 30  0001 C CNN
F 3 "" V 3430 2150 30  0000 C CNN
F 4 "0603" V 3350 2150 20  0000 C CNN "SMT"
F 5 "9331913" V 3550 2150 30  0001 C CNN "Part"
F 6 "Farnell" V 3600 2150 30  0001 C CNN "Provider"
	1    3350 2150
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R87
U 1 1 5634F3B9
P 3050 2150
F 0 "R87" H 3100 2300 50  0000 C CNN
F 1 "2k2/5%" V 2950 2150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3200 2150 30  0001 C CNN
F 3 "" V 3130 2150 30  0000 C CNN
F 4 "0603" V 3050 2150 20  0000 C CNN "SMT"
F 5 "9331913" V 3250 2150 30  0001 C CNN "Part"
F 6 "Farnell" V 3300 2150 30  0001 C CNN "Provider"
	1    3050 2150
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R86
U 1 1 5634F41A
P 2750 2150
F 0 "R86" H 2800 2300 50  0000 C CNN
F 1 "1K/5%" V 2650 2150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2900 2150 30  0001 C CNN
F 3 "" V 2830 2150 30  0000 C CNN
F 4 "0603" V 2750 2150 20  0000 C CNN "SMT"
F 5 "9331697" V 2950 2150 30  0001 C CNN "Part"
F 6 "Farnell" V 3000 2150 30  0001 C CNN "Provider"
	1    2750 2150
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R90
U 1 1 5634F4EF
P 8250 2200
F 0 "R90" H 8300 2350 50  0000 C CNN
F 1 "1k8/5%" V 8150 2200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8400 2200 30  0001 C CNN
F 3 "" V 8330 2200 30  0000 C CNN
F 4 "0603" V 8250 2200 20  0000 C CNN "SMT"
F 5 "9331867" V 8450 2200 30  0001 C CNN "Part"
F 6 "Farnell" V 8500 2200 30  0001 C CNN "Provider"
	1    8250 2200
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R91
U 1 1 5634F626
P 8550 2200
F 0 "R91" H 8600 2350 50  0000 C CNN
F 1 "1k8/5%" V 8450 2200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8700 2200 30  0001 C CNN
F 3 "" V 8630 2200 30  0000 C CNN
F 4 "0603" V 8550 2200 20  0000 C CNN "SMT"
F 5 "9331867" V 8750 2200 30  0001 C CNN "Part"
F 6 "Farnell" V 8800 2200 30  0001 C CNN "Provider"
	1    8550 2200
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R92
U 1 1 5634F849
P 8850 2200
F 0 "R92" H 8900 2350 50  0000 C CNN
F 1 "27K/5%" V 8750 2200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9000 2200 30  0001 C CNN
F 3 "" V 8930 2200 30  0000 C CNN
F 4 "0603" V 8850 2200 20  0000 C CNN "SMT"
F 5 "9331980" V 9050 2200 30  0001 C CNN "Part"
F 6 "Farnell" V 9100 2200 30  0001 C CNN "Provider"
	1    8850 2200
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG0130
U 1 1 56CB7D0C
P 3800 1000
F 0 "#FLG0130" H 3800 1095 50  0001 C CNN
F 1 "PWR_FLAG" H 3800 1224 50  0000 C CNN
F 2 "" H 3800 1000 50  0000 C CNN
F 3 "" H 3800 1000 50  0000 C CNN
	1    3800 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 1000 3800 2400
Wire Wire Line
	3800 2400 3650 2400
Connection ~ 3650 2400
$EndSCHEMATC
