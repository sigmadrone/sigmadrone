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
Sheet 23 27
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
L TPS54332DDA U?
U 1 1 56199896
P 5350 1600
F 0 "U?" H 5600 2050 60  0000 C CNN
F 1 "TPS54332DDA" H 5350 950 60  0000 C CNN
F 2 "" H 5350 1500 60  0001 C CNN
F 3 "" H 5350 1500 60  0000 C CNN
	1    5350 1600
	1    0    0    -1  
$EndComp
Text HLabel 1400 1300 0    60   Input ~ 0
Vsys
$Comp
L INDUCTOR L?
U 1 1 561998C4
P 7300 1300
F 0 "L?" V 7250 1300 50  0000 C CNN
F 1 "3u3/4A6" V 7400 1300 50  0000 C CNN
F 2 "" H 7300 1300 60  0001 C CNN
F 3 "" H 7300 1300 60  0000 C CNN
F 4 "NA" V 7300 1300 60  0001 C CNN "SMT"
F 5 "2082660" V 7300 1300 60  0001 C CNN "Part"
F 6 "Farnell" V 7300 1300 60  0001 C CNN "Provider"
	1    7300 1300
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR?
U 1 1 56199922
P 9650 1050
F 0 "#PWR?" H 9650 900 50  0001 C CNN
F 1 "+5V" H 9650 1190 50  0000 C CNN
F 2 "" H 9650 1050 60  0000 C CNN
F 3 "" H 9650 1050 60  0000 C CNN
	1    9650 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 1300 7900 1300
Wire Wire Line
	7900 1300 9100 1300
Wire Wire Line
	9100 1300 9650 1300
Wire Wire Line
	9650 1050 9650 1300
Wire Wire Line
	9650 1300 9650 1450
Wire Wire Line
	5900 1300 6400 1300
Wire Wire Line
	6400 1300 6750 1300
Wire Wire Line
	6750 1300 7000 1300
$Comp
L GND #PWR?
U 1 1 56199A56
P 9650 3200
F 0 "#PWR?" H 9650 2950 50  0001 C CNN
F 1 "GND" H 9650 3050 50  0000 C CNN
F 2 "" H 9650 3200 60  0000 C CNN
F 3 "" H 9650 3200 60  0000 C CNN
	1    9650 3200
	1    0    0    -1  
$EndComp
Connection ~ 9650 1300
$Comp
L GND #PWR?
U 1 1 56199B3D
P 9100 3200
F 0 "#PWR?" H 9100 2950 50  0001 C CNN
F 1 "GND" H 9100 3050 50  0000 C CNN
F 2 "" H 9100 3200 60  0000 C CNN
F 3 "" H 9100 3200 60  0000 C CNN
	1    9100 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 3200 9100 2500
Wire Wire Line
	9100 2200 9100 1300
Connection ~ 9100 1300
$Comp
L D_Schottky D?
U 1 1 56199BD6
P 6750 1600
F 0 "D?" H 6750 1700 50  0000 C CNN
F 1 "B560C-13-F" H 6750 1500 50  0000 C CNN
F 2 "" H 6750 1600 60  0001 C CNN
F 3 "" H 6750 1600 60  0000 C CNN
	1    6750 1600
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 56199C14
P 6750 3200
F 0 "#PWR?" H 6750 2950 50  0001 C CNN
F 1 "GND" H 6750 3050 50  0000 C CNN
F 2 "" H 6750 3200 60  0000 C CNN
F 3 "" H 6750 3200 60  0000 C CNN
	1    6750 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 3200 6750 1750
Wire Wire Line
	6750 1450 6750 1300
Connection ~ 6750 1300
$Comp
L GND #PWR?
U 1 1 5619AB65
P 7900 3200
F 0 "#PWR?" H 7900 2950 50  0001 C CNN
F 1 "GND" H 7900 3050 50  0000 C CNN
F 2 "" H 7900 3200 60  0000 C CNN
F 3 "" H 7900 3200 60  0000 C CNN
	1    7900 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 1750 7900 1900
Wire Wire Line
	7900 1900 7900 2200
Wire Wire Line
	7900 2500 7900 3200
Wire Wire Line
	9650 1750 9650 2150
Wire Wire Line
	9650 2550 9650 3200
Wire Wire Line
	7900 1450 7900 1300
Connection ~ 7900 1300
Wire Wire Line
	5900 1900 7900 1900
Connection ~ 7900 1900
$Comp
L GND #PWR?
U 1 1 5619AE2E
P 6100 3200
F 0 "#PWR?" H 6100 2950 50  0001 C CNN
F 1 "GND" H 6100 3050 50  0000 C CNN
F 2 "" H 6100 3200 60  0000 C CNN
F 3 "" H 6100 3200 60  0000 C CNN
	1    6100 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 2100 6100 2100
Wire Wire Line
	6100 2100 6100 2200
Wire Wire Line
	6100 2500 6100 2700
Wire Wire Line
	6100 3000 6100 3200
Wire Wire Line
	1400 1300 2200 1300
Wire Wire Line
	2200 1300 3500 1300
Wire Wire Line
	3500 1300 4800 1300
Wire Wire Line
	3500 1750 3500 1950
Wire Wire Line
	3500 1950 3500 2200
Wire Wire Line
	3500 1450 3500 1300
Connection ~ 3500 1300
$Comp
L GND #PWR?
U 1 1 5619B337
P 3500 3200
F 0 "#PWR?" H 3500 2950 50  0001 C CNN
F 1 "GND" H 3500 3050 50  0000 C CNN
F 2 "" H 3500 3200 60  0000 C CNN
F 3 "" H 3500 3200 60  0000 C CNN
	1    3500 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 2500 3500 3200
$Comp
L GND #PWR?
U 1 1 5619B4A1
P 4150 3200
F 0 "#PWR?" H 4150 2950 50  0001 C CNN
F 1 "GND" H 4150 3050 50  0000 C CNN
F 2 "" H 4150 3200 60  0000 C CNN
F 3 "" H 4150 3200 60  0000 C CNN
	1    4150 3200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5619B4D6
P 4600 3200
F 0 "#PWR?" H 4600 2950 50  0001 C CNN
F 1 "GND" H 4600 3050 50  0000 C CNN
F 2 "" H 4600 3200 60  0000 C CNN
F 3 "" H 4600 3200 60  0000 C CNN
	1    4600 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 1900 4600 1900
Wire Wire Line
	4600 1900 4600 2100
Wire Wire Line
	4600 2100 4600 3200
Wire Wire Line
	4800 2100 4600 2100
Connection ~ 4600 2100
Wire Wire Line
	4800 1700 4150 1700
Wire Wire Line
	4150 1700 4150 2200
Wire Wire Line
	4150 2500 4150 3200
Wire Wire Line
	6300 1700 6400 1700
Wire Wire Line
	6400 1700 6400 1300
Connection ~ 6400 1300
Wire Wire Line
	5900 1700 6000 1700
Wire Wire Line
	4800 1500 3800 1500
Wire Wire Line
	3800 1500 3800 1950
Wire Wire Line
	3800 1950 3500 1950
Connection ~ 3500 1950
Wire Wire Line
	2200 4250 2200 1300
Connection ~ 2200 1300
$Comp
L +5V #PWR?
U 1 1 562430FB
P 10450 1050
F 0 "#PWR?" H 10450 900 50  0001 C CNN
F 1 "+5V" H 10450 1190 50  0000 C CNN
F 2 "" H 10450 1050 60  0000 C CNN
F 3 "" H 10450 1050 60  0000 C CNN
	1    10450 1050
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG?
U 1 1 56243140
P 10450 1300
F 0 "#FLG?" H 10450 1395 50  0001 C CNN
F 1 "PWR_FLAG" H 10450 1480 50  0000 C CNN
F 2 "" H 10450 1300 60  0000 C CNN
F 3 "" H 10450 1300 60  0000 C CNN
	1    10450 1300
	-1   0    0    1   
$EndComp
Wire Wire Line
	10450 1050 10450 1300
$Comp
L PWR_FLAG #FLG?
U 1 1 56243249
P 10450 4250
F 0 "#FLG?" H 10450 4345 50  0001 C CNN
F 1 "PWR_FLAG" H 10450 4430 50  0000 C CNN
F 2 "" H 10450 4250 60  0000 C CNN
F 3 "" H 10450 4250 60  0000 C CNN
	1    10450 4250
	-1   0    0    1   
$EndComp
Wire Wire Line
	10450 4000 10450 4250
$Comp
L +3V3 #PWR?
U 1 1 56243288
P 9650 4000
F 0 "#PWR?" H 9650 3850 50  0001 C CNN
F 1 "+3V3" H 9650 4140 50  0000 C CNN
F 2 "" H 9650 4000 60  0000 C CNN
F 3 "" H 9650 4000 60  0000 C CNN
	1    9650 4000
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C?
U 1 1 5637101E
P 4150 2350
F 0 "C?" H 4175 2450 50  0000 L CNN
F 1 "100nF/50V/X7R" V 4000 1950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4180 2200 30  0001 L CNN
F 3 "" H 4175 2450 60  0000 L CNN
F 4 "0603" H 4150 2350 20  0000 C CNN "SMT"
F 5 "1414028" H 4180 2160 30  0001 L CNN "Part"
F 6 "Farnell" H 4180 2120 30  0001 L CNN "Provider"
	1    4150 2350
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C?
U 1 1 56371028
P 9100 2350
F 0 "C?" H 9125 2450 50  0000 L CNN
F 1 "10uF/16V/X5R" V 8950 2000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 9130 2200 30  0001 L CNN
F 3 "" H 9125 2450 60  0000 L CNN
F 4 "0805" H 9100 2350 20  0000 C CNN "SMT"
F 5 "1762635" H 9130 2160 30  0001 L CNN "Part"
F 6 "Farnell" H 9130 2120 30  0001 L CNN "Provider"
	1    9100 2350
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C?
U 1 1 56371612
P 6150 1700
F 0 "C?" V 6200 1550 50  0000 L CNN
F 1 "100nF/50V/X7R" V 6000 1300 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6180 1550 30  0001 L CNN
F 3 "" H 6175 1800 60  0000 L CNN
F 4 "0603" H 6150 1700 20  0000 C CNN "SMT"
F 5 "1414028" H 6180 1510 30  0001 L CNN "Part"
F 6 "Farnell" H 6180 1470 30  0001 L CNN "Provider"
	1    6150 1700
	0    -1   -1   0   
$EndComp
$Comp
L C_SMT C?
U 1 1 5637201C
P 6100 2850
F 0 "C?" H 6125 2950 50  0000 L CNN
F 1 "470pF/50V/X7R" V 5950 2450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6130 2700 30  0001 L CNN
F 3 "" H 6125 2950 60  0000 L CNN
F 4 "0603" H 6100 2850 20  0000 C CNN "SMT"
F 5 "498580" H 6130 2660 30  0001 L CNN "Part"
F 6 "Farnell" H 6130 2620 30  0001 L CNN "Provider"
	1    6100 2850
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 563724C8
P 3500 1600
F 0 "R?" V 3570 1600 50  0000 C CNN
F 1 "150K/5%" V 3430 1600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3650 1600 30  0001 C CNN
F 3 "" V 3580 1600 30  0000 C CNN
F 4 "0603" V 3500 1600 20  0000 C CNN "SMT"
F 5 "9331824" V 3700 1600 30  0001 C CNN "Part"
F 6 "Farnell" V 3750 1600 30  0001 C CNN "Provider"
	1    3500 1600
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 5637270C
P 3500 2350
F 0 "R?" V 3570 2350 50  0000 C CNN
F 1 "150K/5%" V 3430 2350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3650 2350 30  0001 C CNN
F 3 "" V 3580 2350 30  0000 C CNN
F 4 "0603" V 3500 2350 20  0000 C CNN "SMT"
F 5 "9331824" V 3700 2350 30  0001 C CNN "Part"
F 6 "Farnell" V 3750 2350 30  0001 C CNN "Provider"
	1    3500 2350
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56372873
P 6100 2350
F 0 "R?" V 6170 2350 50  0000 C CNN
F 1 "150K/5%" V 6030 2350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6250 2350 30  0001 C CNN
F 3 "" V 6180 2350 30  0000 C CNN
F 4 "0603" V 6100 2350 20  0000 C CNN "SMT"
F 5 "9331824" V 6300 2350 30  0001 C CNN "Part"
F 6 "Farnell" V 6350 2350 30  0001 C CNN "Provider"
	1    6100 2350
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 5637295D
P 7900 2350
F 0 "R?" V 7970 2350 50  0000 C CNN
F 1 "1K96/1%" V 7830 2350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8050 2350 30  0001 C CNN
F 3 "" V 7980 2350 30  0000 C CNN
F 4 "0603" V 7900 2350 20  0000 C CNN "SMT"
F 5 "2138374" V 8100 2350 30  0001 C CNN "Part"
F 6 "Farnell" V 8150 2350 30  0001 C CNN "Provider"
	1    7900 2350
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56372B25
P 7900 1600
F 0 "R?" V 7970 1600 50  0000 C CNN
F 1 "10K2/1%" V 7830 1600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8050 1600 30  0001 C CNN
F 3 "" V 7980 1600 30  0000 C CNN
F 4 "0603" V 7900 1600 20  0000 C CNN "SMT"
F 5 "1170889" V 8100 1600 30  0001 C CNN "Part"
F 6 "Farnell" V 8150 1600 30  0001 C CNN "Provider"
	1    7900 1600
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 5637326A
P 9650 1600
F 0 "R?" V 9720 1600 50  0000 C CNN
F 1 "750R/5%" V 9580 1600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9800 1600 30  0001 C CNN
F 3 "" V 9730 1600 30  0000 C CNN
F 4 "0603" V 9650 1600 20  0000 C CNN "SMT"
F 5 "9331506" V 9850 1600 30  0001 C CNN "Part"
F 6 "Farnell" V 9900 1600 30  0001 C CNN "Provider"
	1    9650 1600
	1    0    0    -1  
$EndComp
$Comp
L LED_SMT DL?
U 1 1 5637602B
P 9650 2350
F 0 "DL?" H 9650 2450 50  0000 C CNN
F 1 "RED" H 9650 2250 50  0000 C CNN
F 2 "LEDs:LED-0603" H 9650 2200 30  0001 C CNN
F 3 "" H 9650 2350 60  0000 C CNN
F 4 "0603" H 9535 2330 30  0000 C CNN "SMT"
F 5 "2322071" H 9650 2150 30  0001 C CNN "Part"
F 6 "Farnell" H 9650 2100 30  0001 C CNN "Provider"
	1    9650 2350
	0    -1   -1   0   
$EndComp
$Comp
L TPS54332DDA U?
U 1 1 56376DE3
P 5350 4550
F 0 "U?" H 5600 5000 60  0000 C CNN
F 1 "TPS54332DDA" H 5350 3900 60  0000 C CNN
F 2 "" H 5350 4450 60  0001 C CNN
F 3 "" H 5350 4450 60  0000 C CNN
	1    5350 4550
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR L?
U 1 1 56376DEC
P 7300 4250
F 0 "L?" V 7250 4250 50  0000 C CNN
F 1 "3u3/4A6" V 7400 4250 50  0000 C CNN
F 2 "" H 7300 4250 60  0001 C CNN
F 3 "" H 7300 4250 60  0000 C CNN
F 4 "NA" V 7300 4250 60  0001 C CNN "SMT"
F 5 "2082660" V 7300 4250 60  0001 C CNN "Part"
F 6 "Farnell" V 7300 4250 60  0001 C CNN "Provider"
	1    7300 4250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7600 4250 7900 4250
Wire Wire Line
	7900 4250 8550 4250
Wire Wire Line
	8550 4250 9100 4250
Wire Wire Line
	9100 4250 9650 4250
Wire Wire Line
	9650 4000 9650 4250
Wire Wire Line
	9650 4250 9650 4400
Wire Wire Line
	5900 4250 6400 4250
Wire Wire Line
	6400 4250 6750 4250
Wire Wire Line
	6750 4250 7000 4250
$Comp
L GND #PWR?
U 1 1 56376E00
P 9650 6150
F 0 "#PWR?" H 9650 5900 50  0001 C CNN
F 1 "GND" H 9650 6000 50  0000 C CNN
F 2 "" H 9650 6150 60  0000 C CNN
F 3 "" H 9650 6150 60  0000 C CNN
	1    9650 6150
	1    0    0    -1  
$EndComp
Connection ~ 9650 4250
$Comp
L GND #PWR?
U 1 1 56376E07
P 9100 6150
F 0 "#PWR?" H 9100 5900 50  0001 C CNN
F 1 "GND" H 9100 6000 50  0000 C CNN
F 2 "" H 9100 6150 60  0000 C CNN
F 3 "" H 9100 6150 60  0000 C CNN
	1    9100 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 6150 9100 5450
Wire Wire Line
	9100 5150 9100 4250
Connection ~ 9100 4250
$Comp
L D_Schottky D?
U 1 1 56376E10
P 6750 4550
F 0 "D?" H 6750 4650 50  0000 C CNN
F 1 "B560C-13-F" H 6750 4450 50  0000 C CNN
F 2 "" H 6750 4550 60  0001 C CNN
F 3 "" H 6750 4550 60  0000 C CNN
	1    6750 4550
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 56376E16
P 6750 6150
F 0 "#PWR?" H 6750 5900 50  0001 C CNN
F 1 "GND" H 6750 6000 50  0000 C CNN
F 2 "" H 6750 6150 60  0000 C CNN
F 3 "" H 6750 6150 60  0000 C CNN
	1    6750 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 6150 6750 4700
Wire Wire Line
	6750 4400 6750 4250
Connection ~ 6750 4250
$Comp
L GND #PWR?
U 1 1 56376E1F
P 7900 6150
F 0 "#PWR?" H 7900 5900 50  0001 C CNN
F 1 "GND" H 7900 6000 50  0000 C CNN
F 2 "" H 7900 6150 60  0000 C CNN
F 3 "" H 7900 6150 60  0000 C CNN
	1    7900 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 4700 7900 4850
Wire Wire Line
	7900 4850 7900 5150
Wire Wire Line
	7900 5450 7900 6150
Wire Wire Line
	9650 4700 9650 5100
Wire Wire Line
	9650 5500 9650 6150
Wire Wire Line
	7900 4400 7900 4250
Connection ~ 7900 4250
Wire Wire Line
	5900 4850 7900 4850
Connection ~ 7900 4850
$Comp
L GND #PWR?
U 1 1 56376E2E
P 6100 6150
F 0 "#PWR?" H 6100 5900 50  0001 C CNN
F 1 "GND" H 6100 6000 50  0000 C CNN
F 2 "" H 6100 6150 60  0000 C CNN
F 3 "" H 6100 6150 60  0000 C CNN
	1    6100 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 5050 6100 5050
Wire Wire Line
	6100 5050 6100 5150
Wire Wire Line
	6100 5450 6100 5650
Wire Wire Line
	6100 5950 6100 6150
Wire Wire Line
	2200 4250 3500 4250
Wire Wire Line
	3500 4250 4800 4250
Wire Wire Line
	3500 4700 3500 4900
Wire Wire Line
	3500 4900 3500 5150
Wire Wire Line
	3500 4400 3500 4250
Connection ~ 3500 4250
$Comp
L GND #PWR?
U 1 1 56376E3E
P 3500 6150
F 0 "#PWR?" H 3500 5900 50  0001 C CNN
F 1 "GND" H 3500 6000 50  0000 C CNN
F 2 "" H 3500 6150 60  0000 C CNN
F 3 "" H 3500 6150 60  0000 C CNN
	1    3500 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 5450 3500 6150
$Comp
L GND #PWR?
U 1 1 56376E45
P 4150 6150
F 0 "#PWR?" H 4150 5900 50  0001 C CNN
F 1 "GND" H 4150 6000 50  0000 C CNN
F 2 "" H 4150 6150 60  0000 C CNN
F 3 "" H 4150 6150 60  0000 C CNN
	1    4150 6150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 56376E4B
P 4600 6150
F 0 "#PWR?" H 4600 5900 50  0001 C CNN
F 1 "GND" H 4600 6000 50  0000 C CNN
F 2 "" H 4600 6150 60  0000 C CNN
F 3 "" H 4600 6150 60  0000 C CNN
	1    4600 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 4850 4600 4850
Wire Wire Line
	4600 4850 4600 5050
Wire Wire Line
	4600 5050 4600 6150
Wire Wire Line
	4800 5050 4600 5050
Connection ~ 4600 5050
Wire Wire Line
	4800 4650 4150 4650
Wire Wire Line
	4150 4650 4150 5150
Wire Wire Line
	4150 5450 4150 6150
Wire Wire Line
	6300 4650 6400 4650
Wire Wire Line
	6400 4650 6400 4250
Connection ~ 6400 4250
Wire Wire Line
	5900 4650 6000 4650
Wire Wire Line
	4800 4450 3800 4450
Wire Wire Line
	3800 4450 3800 4900
Wire Wire Line
	3800 4900 3500 4900
Connection ~ 3500 4900
$Comp
L C_SMT C?
U 1 1 56376E64
P 4150 5300
F 0 "C?" H 4175 5400 50  0000 L CNN
F 1 "100nF/50V/X7R" V 4000 4900 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4180 5150 30  0001 L CNN
F 3 "" H 4175 5400 60  0000 L CNN
F 4 "0603" H 4150 5300 20  0000 C CNN "SMT"
F 5 "1414028" H 4180 5110 30  0001 L CNN "Part"
F 6 "Farnell" H 4180 5070 30  0001 L CNN "Provider"
	1    4150 5300
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C?
U 1 1 56376E6D
P 9100 5300
F 0 "C?" H 9125 5400 50  0000 L CNN
F 1 "10uF/16V/X5R" V 8950 4950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 9130 5150 30  0001 L CNN
F 3 "" H 9125 5400 60  0000 L CNN
F 4 "0805" H 9100 5300 20  0000 C CNN "SMT"
F 5 "1762635" H 9130 5110 30  0001 L CNN "Part"
F 6 "Farnell" H 9130 5070 30  0001 L CNN "Provider"
	1    9100 5300
	1    0    0    -1  
$EndComp
$Comp
L C_SMT C?
U 1 1 56376E76
P 6150 4650
F 0 "C?" V 6200 4500 50  0000 L CNN
F 1 "100nF/50V/X7R" V 6000 4250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6180 4500 30  0001 L CNN
F 3 "" H 6175 4750 60  0000 L CNN
F 4 "0603" H 6150 4650 20  0000 C CNN "SMT"
F 5 "1414028" H 6180 4460 30  0001 L CNN "Part"
F 6 "Farnell" H 6180 4420 30  0001 L CNN "Provider"
	1    6150 4650
	0    -1   -1   0   
$EndComp
$Comp
L C_SMT C?
U 1 1 56376E7F
P 6100 5800
F 0 "C?" H 6125 5900 50  0000 L CNN
F 1 "470pF/50V/X7R" V 5950 5400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6130 5650 30  0001 L CNN
F 3 "" H 6125 5900 60  0000 L CNN
F 4 "0603" H 6100 5800 20  0000 C CNN "SMT"
F 5 "498580" H 6130 5610 30  0001 L CNN "Part"
F 6 "Farnell" H 6130 5570 30  0001 L CNN "Provider"
	1    6100 5800
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56376E88
P 3500 4550
F 0 "R?" V 3570 4550 50  0000 C CNN
F 1 "150K/5%" V 3430 4550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3650 4550 30  0001 C CNN
F 3 "" V 3580 4550 30  0000 C CNN
F 4 "0603" V 3500 4550 20  0000 C CNN "SMT"
F 5 "9331824" V 3700 4550 30  0001 C CNN "Part"
F 6 "Farnell" V 3750 4550 30  0001 C CNN "Provider"
	1    3500 4550
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56376E91
P 3500 5300
F 0 "R?" V 3570 5300 50  0000 C CNN
F 1 "150K/5%" V 3430 5300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3650 5300 30  0001 C CNN
F 3 "" V 3580 5300 30  0000 C CNN
F 4 "0603" V 3500 5300 20  0000 C CNN "SMT"
F 5 "9331824" V 3700 5300 30  0001 C CNN "Part"
F 6 "Farnell" V 3750 5300 30  0001 C CNN "Provider"
	1    3500 5300
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56376E9A
P 6100 5300
F 0 "R?" V 6170 5300 50  0000 C CNN
F 1 "150K/5%" V 6030 5300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6250 5300 30  0001 C CNN
F 3 "" V 6180 5300 30  0000 C CNN
F 4 "0603" V 6100 5300 20  0000 C CNN "SMT"
F 5 "9331824" V 6300 5300 30  0001 C CNN "Part"
F 6 "Farnell" V 6350 5300 30  0001 C CNN "Provider"
	1    6100 5300
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56376EA3
P 7900 5300
F 0 "R?" V 7970 5300 50  0000 C CNN
F 1 "3K24/1%" V 7830 5300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8050 5300 30  0001 C CNN
F 3 "" V 7980 5300 30  0000 C CNN
F 4 "0603" V 7900 5300 20  0000 C CNN "SMT"
F 5 "1170839" V 8100 5300 30  0001 C CNN "Part"
F 6 "Farnell" V 8150 5300 30  0001 C CNN "Provider"
	1    7900 5300
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56376EAC
P 7900 4550
F 0 "R?" V 7970 4550 50  0000 C CNN
F 1 "10K2/1%" V 7830 4550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8050 4550 30  0001 C CNN
F 3 "" V 7980 4550 30  0000 C CNN
F 4 "0603" V 7900 4550 20  0000 C CNN "SMT"
F 5 "1170889" V 8100 4550 30  0001 C CNN "Part"
F 6 "Farnell" V 8150 4550 30  0001 C CNN "Provider"
	1    7900 4550
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56376EB5
P 9650 4550
F 0 "R?" V 9720 4550 50  0000 C CNN
F 1 "750R/5%" V 9580 4550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9800 4550 30  0001 C CNN
F 3 "" V 9730 4550 30  0000 C CNN
F 4 "0603" V 9650 4550 20  0000 C CNN "SMT"
F 5 "9331506" V 9850 4550 30  0001 C CNN "Part"
F 6 "Farnell" V 9900 4550 30  0001 C CNN "Provider"
	1    9650 4550
	1    0    0    -1  
$EndComp
$Comp
L LED_SMT DL?
U 1 1 56376EBE
P 9650 5300
F 0 "DL?" H 9650 5400 50  0000 C CNN
F 1 "RED" H 9650 5200 50  0000 C CNN
F 2 "LEDs:LED-0603" H 9650 5150 30  0001 C CNN
F 3 "" H 9650 5300 60  0000 C CNN
F 4 "0603" H 9535 5280 30  0000 C CNN "SMT"
F 5 "2322071" H 9650 5100 30  0001 C CNN "Part"
F 6 "Farnell" H 9650 5050 30  0001 C CNN "Provider"
	1    9650 5300
	0    -1   -1   0   
$EndComp
$Comp
L +3V3 #PWR?
U 1 1 56377904
P 10450 4000
F 0 "#PWR?" H 10450 3850 50  0001 C CNN
F 1 "+3V3" H 10450 4140 50  0000 C CNN
F 2 "" H 10450 4000 60  0000 C CNN
F 3 "" H 10450 4000 60  0000 C CNN
	1    10450 4000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 56525E76
P 8550 6150
F 0 "#PWR?" H 8550 5900 50  0001 C CNN
F 1 "GND" H 8550 6000 50  0000 C CNN
F 2 "" H 8550 6150 60  0000 C CNN
F 3 "" H 8550 6150 60  0000 C CNN
	1    8550 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 6150 8550 5450
Wire Wire Line
	8550 5150 8550 4250
$Comp
L C_SMT C?
U 1 1 56525E81
P 8550 5300
F 0 "C?" H 8575 5400 50  0000 L CNN
F 1 "10uF/16V/X5R" V 8400 4950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8580 5150 30  0001 L CNN
F 3 "" H 8575 5400 60  0000 L CNN
F 4 "0805" H 8550 5300 20  0000 C CNN "SMT"
F 5 "1762635" H 8580 5110 30  0001 L CNN "Part"
F 6 "Farnell" H 8580 5070 30  0001 L CNN "Provider"
	1    8550 5300
	1    0    0    -1  
$EndComp
Connection ~ 8550 4250
$Comp
L GND #PWR?
U 1 1 56526013
P 8550 3200
F 0 "#PWR?" H 8550 2950 50  0001 C CNN
F 1 "GND" H 8550 3050 50  0000 C CNN
F 2 "" H 8550 3200 60  0000 C CNN
F 3 "" H 8550 3200 60  0000 C CNN
	1    8550 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 3200 8550 2500
Wire Wire Line
	8550 2200 8550 1300
$Comp
L C_SMT C?
U 1 1 5652601E
P 8550 2350
F 0 "C?" H 8575 2450 50  0000 L CNN
F 1 "10uF/16V/X5R" V 8400 2000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8580 2200 30  0001 L CNN
F 3 "" H 8575 2450 60  0000 L CNN
F 4 "0805" H 8550 2350 20  0000 C CNN "SMT"
F 5 "1762635" H 8580 2160 30  0001 L CNN "Part"
F 6 "Farnell" H 8580 2120 30  0001 L CNN "Provider"
	1    8550 2350
	1    0    0    -1  
$EndComp
Connection ~ 8550 1300
$EndSCHEMATC
