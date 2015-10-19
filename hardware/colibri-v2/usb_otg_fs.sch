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
Sheet 20 23
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
L USB_OTG P9
U 1 1 5614ED84
P 9300 5150
F 0 "P9" H 9625 5025 50  0000 C CNN
F 1 "USB_OTG" H 9300 5350 50  0000 C CNN
F 2 "" V 9250 5050 60  0000 C CNN
F 3 "" V 9250 5050 60  0000 C CNN
	1    9300 5150
	0    1    1    0   
$EndComp
Text HLabel 2600 4950 0    60   BiDi ~ 0
VBUS_FS
Text HLabel 2600 5050 0    60   BiDi ~ 0
OTG_FS_DM
Text HLabel 2600 5150 0    60   BiDi ~ 0
OTG_FS_DP
Text HLabel 2600 5250 0    60   BiDi ~ 0
OTG_FS_ID
Text HLabel 2600 3000 0    60   Output ~ 0
OTG_FS_OC
Text HLabel 2600 2900 0    60   Input ~ 0
OTG_FS_PSO
$Comp
L VBUS053CZ-HAF-G-08 U18
U 1 1 5614ED91
P 5300 4150
F 0 "U18" H 5450 4450 60  0000 C CNN
F 1 "VBUS053CZ-HAF-G-08" H 5300 3850 60  0000 C CNN
F 2 "" H 5250 4850 60  0000 C CNN
F 3 "" H 5250 4850 60  0000 C CNN
	1    5300 4150
	1    0    0    -1  
$EndComp
$Comp
L R R121
U 1 1 5614ED98
P 3400 5050
F 0 "R121" V 3350 5200 50  0000 C CNN
F 1 "22" V 3400 5050 50  0000 C CNN
F 2 "" V 3330 5050 30  0000 C CNN
F 3 "" H 3400 5050 30  0000 C CNN
	1    3400 5050
	0    1    1    0   
$EndComp
$Comp
L R R122
U 1 1 5614ED9F
P 3400 5150
F 0 "R122" V 3350 5300 50  0000 C CNN
F 1 "22" V 3400 5150 50  0000 C CNN
F 2 "" V 3330 5150 30  0000 C CNN
F 3 "" H 3400 5150 30  0000 C CNN
	1    3400 5150
	0    1    1    0   
$EndComp
$Comp
L +3V3 #PWR149
U 1 1 5614EDA6
P 3100 1800
F 0 "#PWR149" H 3100 1650 50  0001 C CNN
F 1 "+3V3" H 3100 1940 50  0000 C CNN
F 2 "" H 3100 1800 60  0000 C CNN
F 3 "" H 3100 1800 60  0000 C CNN
	1    3100 1800
	1    0    0    -1  
$EndComp
$Comp
L R R120
U 1 1 5614EDAC
P 3100 2350
F 0 "R120" V 3180 2350 50  0000 C CNN
F 1 "10K" V 3100 2350 50  0000 C CNN
F 2 "" V 3030 2350 30  0000 C CNN
F 3 "" H 3100 2350 30  0000 C CNN
	1    3100 2350
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR150
U 1 1 5614EDB3
P 3450 1800
F 0 "#PWR150" H 3450 1650 50  0001 C CNN
F 1 "+3V3" H 3450 1940 50  0000 C CNN
F 2 "" H 3450 1800 60  0000 C CNN
F 3 "" H 3450 1800 60  0000 C CNN
	1    3450 1800
	1    0    0    -1  
$EndComp
$Comp
L R R123
U 1 1 5614EDB9
P 3450 2350
F 0 "R123" V 3530 2350 50  0000 C CNN
F 1 "47K" V 3450 2350 50  0000 C CNN
F 2 "" V 3380 2350 30  0000 C CNN
F 3 "" H 3450 2350 30  0000 C CNN
	1    3450 2350
	1    0    0    -1  
$EndComp
$Comp
L LED D16
U 1 1 5614EDC0
P 3800 2500
F 0 "D16" H 3800 2600 50  0000 C CNN
F 1 "RED" H 3800 2400 50  0000 C CNN
F 2 "" H 3800 2500 60  0000 C CNN
F 3 "" H 3800 2500 60  0000 C CNN
	1    3800 2500
	0    -1   -1   0   
$EndComp
$Comp
L R R124
U 1 1 5614EDC7
P 3800 2050
F 0 "R124" V 3880 2050 50  0000 C CNN
F 1 "620" V 3800 2050 50  0000 C CNN
F 2 "" V 3730 2050 30  0000 C CNN
F 3 "" H 3800 2050 30  0000 C CNN
	1    3800 2050
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR151
U 1 1 5614EDCE
P 3800 1800
F 0 "#PWR151" H 3800 1650 50  0001 C CNN
F 1 "+3V3" H 3800 1940 50  0000 C CNN
F 2 "" H 3800 1800 60  0000 C CNN
F 3 "" H 3800 1800 60  0000 C CNN
	1    3800 1800
	1    0    0    -1  
$EndComp
$Comp
L LED D17
U 1 1 5614EDD4
P 9100 2250
F 0 "D17" H 9100 2350 50  0000 C CNN
F 1 "GREEN" H 9100 2150 50  0000 C CNN
F 2 "" H 9100 2250 60  0000 C CNN
F 3 "" H 9100 2250 60  0000 C CNN
	1    9100 2250
	0    -1   -1   0   
$EndComp
$Comp
L R R127
U 1 1 5614EDDB
P 9100 1850
F 0 "R127" V 9180 1850 50  0000 C CNN
F 1 "620" V 9100 1850 50  0000 C CNN
F 2 "" V 9030 1850 30  0000 C CNN
F 3 "" H 9100 1850 30  0000 C CNN
	1    9100 1850
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR157
U 1 1 5614EDE2
P 9100 1600
F 0 "#PWR157" H 9100 1450 50  0001 C CNN
F 1 "+3V3" H 9100 1740 50  0000 C CNN
F 2 "" H 9100 1600 60  0000 C CNN
F 3 "" H 9100 1600 60  0000 C CNN
	1    9100 1600
	1    0    0    -1  
$EndComp
$Comp
L Q_NMOS_GSD Q9
U 1 1 5614EDE8
P 9000 2800
F 0 "Q9" H 9300 2850 50  0000 R CNN
F 1 "BSS138PW" H 9650 2750 50  0000 R CNN
F 2 "" H 9200 2900 29  0000 C CNN
F 3 "" H 9000 2800 60  0000 C CNN
	1    9000 2800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR158
U 1 1 5614EDEF
P 9100 3250
F 0 "#PWR158" H 9100 3000 50  0001 C CNN
F 1 "GND" H 9100 3100 50  0000 C CNN
F 2 "" H 9100 3250 60  0000 C CNN
F 3 "" H 9100 3250 60  0000 C CNN
	1    9100 3250
	1    0    0    -1  
$EndComp
$Comp
L R R125
U 1 1 5614EDF5
P 8300 2800
F 0 "R125" V 8380 2800 50  0000 C CNN
F 1 "10K" V 8300 2800 50  0000 C CNN
F 2 "" V 8230 2800 30  0000 C CNN
F 3 "" H 8300 2800 30  0000 C CNN
	1    8300 2800
	0    1    1    0   
$EndComp
$Comp
L R R126
U 1 1 5614EDFC
P 8850 3100
F 0 "R126" V 8930 3100 50  0000 C CNN
F 1 "47K" V 8850 3100 50  0000 C CNN
F 2 "" V 8780 3100 30  0000 C CNN
F 3 "" H 8850 3100 30  0000 C CNN
	1    8850 3100
	0    1    1    0   
$EndComp
$Comp
L C C87
U 1 1 5614EE03
P 6500 3050
F 0 "C87" H 6525 3150 50  0000 L CNN
F 1 "C" H 6525 2950 50  0000 L CNN
F 2 "" H 6538 2900 30  0000 C CNN
F 3 "" H 6500 3050 60  0000 C CNN
	1    6500 3050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR155
U 1 1 5614EE0A
P 6500 3250
F 0 "#PWR155" H 6500 3000 50  0001 C CNN
F 1 "GND" H 6500 3100 50  0000 C CNN
F 2 "" H 6500 3250 60  0000 C CNN
F 3 "" H 6500 3250 60  0000 C CNN
	1    6500 3250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR153
U 1 1 5614EE10
P 6100 3250
F 0 "#PWR153" H 6100 3000 50  0001 C CNN
F 1 "GND" H 6100 3100 50  0000 C CNN
F 2 "" H 6100 3250 60  0000 C CNN
F 3 "" H 6100 3250 60  0000 C CNN
	1    6100 3250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR154
U 1 1 5614EE16
P 6100 4500
F 0 "#PWR154" H 6100 4250 50  0001 C CNN
F 1 "GND" H 6100 4350 50  0000 C CNN
F 2 "" H 6100 4500 60  0000 C CNN
F 3 "" H 6100 4500 60  0000 C CNN
	1    6100 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 5050 3250 5050
Wire Wire Line
	2600 5150 3250 5150
Wire Wire Line
	3550 5050 9000 5050
Wire Wire Line
	3550 5150 9000 5150
Wire Wire Line
	2600 4950 9000 4950
Wire Wire Line
	2600 5250 9000 5250
Wire Wire Line
	2600 2900 4800 2900
Wire Wire Line
	2600 3000 4800 3000
Wire Wire Line
	3100 1800 3100 2200
Wire Wire Line
	3450 1800 3450 2200
Wire Wire Line
	3100 2500 3100 2900
Connection ~ 3100 2900
Wire Wire Line
	3450 2500 3450 3000
Connection ~ 3450 3000
Wire Wire Line
	3800 1800 3800 1900
Wire Wire Line
	3800 2200 3800 2300
Wire Wire Line
	3800 2700 3800 3000
Connection ~ 3800 3000
Wire Wire Line
	9100 1600 9100 1700
Wire Wire Line
	9100 2000 9100 2050
Wire Wire Line
	9100 2450 9100 2600
Wire Wire Line
	9100 3000 9100 3250
Wire Wire Line
	8450 2800 8800 2800
Wire Wire Line
	5800 2800 8150 2800
Wire Wire Line
	6950 2800 6950 4950
Connection ~ 6950 4950
Connection ~ 6950 2800
Wire Wire Line
	6500 3250 6500 3200
Wire Wire Line
	6500 2900 6500 2800
Connection ~ 6500 2800
Wire Wire Line
	8700 3100 8550 3100
Wire Wire Line
	8550 3100 8550 2800
Connection ~ 8550 2800
Wire Wire Line
	9000 3100 9100 3100
Connection ~ 9100 3100
Wire Wire Line
	5800 4300 6100 4300
Wire Wire Line
	6100 4300 6100 4500
Wire Wire Line
	4800 4300 4250 4300
Wire Wire Line
	4250 4300 4250 5250
Connection ~ 4250 5250
Wire Wire Line
	4800 4200 4150 4200
Wire Wire Line
	4150 4200 4150 5150
Connection ~ 4150 5150
Wire Wire Line
	4800 4100 4050 4100
Wire Wire Line
	4050 4100 4050 5050
Connection ~ 4050 5050
$Comp
L GND #PWR156
U 1 1 5614EE59
P 8900 5800
F 0 "#PWR156" H 8900 5550 50  0001 C CNN
F 1 "GND" H 8900 5650 50  0000 C CNN
F 2 "" H 8900 5800 60  0000 C CNN
F 3 "" H 8900 5800 60  0000 C CNN
	1    8900 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 5350 8900 5350
Wire Wire Line
	8900 5350 8900 5800
Wire Wire Line
	9400 5550 9400 5700
Wire Wire Line
	9400 5700 8900 5700
Connection ~ 8900 5700
$Comp
L +5V #PWR152
U 1 1 5614EE65
P 4150 1800
F 0 "#PWR152" H 4150 1650 50  0001 C CNN
F 1 "+5V" H 4150 1940 50  0000 C CNN
F 2 "" H 4150 1800 60  0000 C CNN
F 3 "" H 4150 1800 60  0000 C CNN
	1    4150 1800
	1    0    0    -1  
$EndComp
$Comp
L TPS2061CDBVT U17
U 1 1 5614EE6B
P 5300 2900
F 0 "U17" H 5550 3150 60  0000 C CNN
F 1 "TPS2061CDBVT" H 5300 2650 60  0000 C CNN
F 2 "" H 5300 2700 60  0000 C CNN
F 3 "" H 5300 2700 60  0000 C CNN
	1    5300 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 1800 4150 2800
Wire Wire Line
	4150 2800 4800 2800
Wire Wire Line
	5800 3000 6100 3000
Wire Wire Line
	6100 3000 6100 3250
NoConn ~ 5800 4100
NoConn ~ 5800 4200
Wire Wire Line
	4800 4000 3950 4000
Wire Wire Line
	3950 4000 3950 4950
Connection ~ 3950 4950
Text Notes 4950 2500 0    60   ~ 0
or use TPS2041CDBVT ???
$EndSCHEMATC
