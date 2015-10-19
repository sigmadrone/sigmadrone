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
Sheet 9 23
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
L CONN_01X02 P5
U 1 1 55962775
P 1300 1250
F 0 "P5" H 1300 1400 50  0000 C CNN
F 1 "CONN_01X02" V 1400 1250 50  0000 C CNN
F 2 "" H 1300 1250 60  0000 C CNN
F 3 "" H 1300 1250 60  0000 C CNN
	1    1300 1250
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X02 P6
U 1 1 559627DB
P 7700 1250
F 0 "P6" H 7700 1400 50  0000 C CNN
F 1 "CONN_01X02" V 7800 1250 50  0000 C CNN
F 2 "" H 7700 1250 60  0000 C CNN
F 3 "" H 7700 1250 60  0000 C CNN
	1    7700 1250
	1    0    0    1   
$EndComp
$Comp
L GND #PWR62
U 1 1 5596291C
P 4200 800
F 0 "#PWR62" H 4200 550 50  0001 C CNN
F 1 "GND" H 4200 650 50  0000 C CNN
F 2 "" H 4200 800 60  0000 C CNN
F 3 "" H 4200 800 60  0000 C CNN
	1    4200 800 
	-1   0    0    1   
$EndComp
$Comp
L 726386-2 T2
U 1 1 55962944
P 2800 800
F 0 "T2" H 2600 900 60  0000 C CNN
F 1 "BATCHARGE-" H 2700 650 60  0000 C CNN
F 2 "" H 2750 1290 60  0000 C CNN
F 3 "" H 2750 1290 60  0000 C CNN
	1    2800 800 
	1    0    0    -1  
$EndComp
$Comp
L AP6681GMT-HF-3TR Q4
U 1 1 559629F6
P 5850 2000
F 0 "Q4" H 5710 2110 50  0000 R CNN
F 1 "AP6681GMT-HF-3TR" V 6390 2330 50  0000 R CNN
F 2 "" H 6050 2100 29  0000 C CNN
F 3 "" H 5850 2000 60  0000 C CNN
	1    5850 2000
	0    1    -1   0   
$EndComp
$Comp
L 726386-2 T1
U 1 1 55962AA2
P 2750 1500
F 0 "T1" H 2550 1600 60  0000 C CNN
F 1 "BATCHARGE+" H 2650 1350 60  0000 C CNN
F 2 "" H 2700 1990 60  0000 C CNN
F 3 "" H 2700 1990 60  0000 C CNN
	1    2750 1500
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR60
U 1 1 55962B26
P 3800 800
F 0 "#PWR60" H 3800 650 50  0001 C CNN
F 1 "+BATT" H 3800 940 50  0000 C CNN
F 2 "" H 3800 800 60  0000 C CNN
F 3 "" H 3800 800 60  0000 C CNN
	1    3800 800 
	1    0    0    -1  
$EndComp
$Comp
L ZENER D9
U 1 1 55962BCA
P 5300 2150
F 0 "D9" H 5300 2250 50  0000 C CNN
F 1 "BZV55-C18" H 5300 2050 50  0000 C CNN
F 2 "" H 5300 2150 60  0000 C CNN
F 3 "" H 5300 2150 60  0000 C CNN
	1    5300 2150
	0    1    1    0   
$EndComp
$Comp
L R R54
U 1 1 55962C09
P 4850 2200
F 0 "R54" V 4930 2200 50  0000 C CNN
F 1 "10K" V 4850 2200 50  0000 C CNN
F 2 "" V 4780 2200 30  0000 C CNN
F 3 "" H 4850 2200 30  0000 C CNN
	1    4850 2200
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH SW9
U 1 1 559A1437
P 3300 10600
F 0 "SW9" H 3450 10710 50  0000 C CNN
F 1 "SW_PUSH" H 3300 10520 50  0000 C CNN
F 2 "" H 3300 10600 60  0000 C CNN
F 3 "" H 3300 10600 60  0000 C CNN
	1    3300 10600
	0    -1   -1   0   
$EndComp
$Comp
L R R51
U 1 1 559A12FA
P 3300 9600
F 0 "R51" V 3380 9600 50  0000 C CNN
F 1 "10K" V 3300 9600 50  0000 C CNN
F 2 "" V 3230 9600 30  0000 C CNN
F 3 "" H 3300 9600 30  0000 C CNN
	1    3300 9600
	1    0    0    -1  
$EndComp
$Comp
L R R52
U 1 1 559A11AB
P 3950 10700
F 0 "R52" V 4030 10700 50  0000 C CNN
F 1 "Load" V 3950 10700 50  0000 C CNN
F 2 "" V 3880 10700 30  0000 C CNN
F 3 "" H 3950 10700 30  0000 C CNN
	1    3950 10700
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH SW8
U 1 1 559A10E6
P 1750 9600
F 0 "SW8" H 1900 9710 50  0000 C CNN
F 1 "SW_PUSH" H 1750 9520 50  0000 C CNN
F 2 "" H 1750 9600 60  0000 C CNN
F 3 "" H 1750 9600 60  0000 C CNN
	1    1750 9600
	0    -1   -1   0   
$EndComp
$Comp
L R R48
U 1 1 559A0FE8
P 1750 10600
F 0 "R48" V 1830 10600 50  0000 C CNN
F 1 "10K" V 1750 10600 50  0000 C CNN
F 2 "" V 1680 10600 30  0000 C CNN
F 3 "" H 1750 10600 30  0000 C CNN
	1    1750 10600
	1    0    0    -1  
$EndComp
Text Notes 3600 10000 0    60   ~ 0
G
Text Notes 4150 10400 0    60   ~ 0
D
Text Notes 4150 9850 0    60   ~ 0
S
Text Notes 2600 9850 0    60   ~ 0
D
Text Notes 2600 10400 0    60   ~ 0
S
Text Notes 2150 10000 0    60   ~ 0
G
$Comp
L R R50
U 1 1 5599E41B
P 2450 9550
F 0 "R50" V 2530 9550 50  0000 C CNN
F 1 "Load" V 2450 9550 50  0000 C CNN
F 2 "" V 2380 9550 30  0000 C CNN
F 3 "" H 2450 9550 30  0000 C CNN
	1    2450 9550
	1    0    0    -1  
$EndComp
$Comp
L Battery BT1
U 1 1 5599E3CA
P 650 10100
F 0 "BT1" H 750 10150 50  0000 L CNN
F 1 "Battery" H 750 10050 50  0000 L CNN
F 2 "" V 650 10140 60  0000 C CNN
F 3 "" V 650 10140 60  0000 C CNN
	1    650  10100
	1    0    0    -1  
$EndComp
Text Notes 3650 9000 0    60   ~ 12
P-Channel
Text Notes 2100 9000 0    60   ~ 12
N-Channel
$Comp
L Q_PMOS_GSD Q3
U 1 1 5599E1BF
P 3850 10100
F 0 "Q3" H 4150 10150 50  0000 R CNN
F 1 "Q_PMOS_GSD" H 4500 10050 50  0000 R CNN
F 2 "" H 4050 10200 29  0000 C CNN
F 3 "" H 3850 10100 60  0000 C CNN
	1    3850 10100
	1    0    0    1   
$EndComp
$Comp
L Q_NMOS_GSD Q1
U 1 1 5599E166
P 2350 10100
F 0 "Q1" H 2650 10150 50  0000 R CNN
F 1 "Q_NMOS_GSD" H 3000 10050 50  0000 R CNN
F 2 "" H 2550 10200 29  0000 C CNN
F 3 "" H 2350 10100 60  0000 C CNN
	1    2350 10100
	1    0    0    -1  
$EndComp
$Comp
L R R53
U 1 1 55A48D8A
P 4400 2550
F 0 "R53" V 4480 2550 50  0000 C CNN
F 1 "470" V 4400 2550 50  0000 C CNN
F 2 "" V 4330 2550 30  0000 C CNN
F 3 "" H 4400 2550 30  0000 C CNN
	1    4400 2550
	0    1    1    0   
$EndComp
$Comp
L Q_NMOS_GSD Q2
U 1 1 55A48E6A
P 2400 2950
F 0 "Q2" H 2700 3000 50  0000 R CNN
F 1 "BSS806N/PMV56XN/PMV30UN2R" H 2400 3150 50  0000 R CNN
F 2 "" H 2600 3050 29  0000 C CNN
F 3 "" H 2400 2950 60  0000 C CNN
	1    2400 2950
	1    0    0    -1  
$EndComp
Text HLabel 1250 2950 0    60   Input ~ 0
VH_CTRL
$Comp
L R R49
U 1 1 55A49063
P 1900 2950
F 0 "R49" V 1980 2950 50  0000 C CNN
F 1 "1K" V 1900 2950 50  0000 C CNN
F 2 "" V 1830 2950 30  0000 C CNN
F 3 "" H 1900 2950 30  0000 C CNN
	1    1900 2950
	0    1    1    0   
$EndComp
$Comp
L R R47
U 1 1 55A49122
P 1550 3250
F 0 "R47" V 1630 3250 50  0000 C CNN
F 1 "10K" V 1550 3250 50  0000 C CNN
F 2 "" V 1480 3250 30  0000 C CNN
F 3 "" H 1550 3250 30  0000 C CNN
	1    1550 3250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR58
U 1 1 55A491DA
P 1550 3500
F 0 "#PWR58" H 1550 3250 50  0001 C CNN
F 1 "GND" H 1550 3350 50  0000 C CNN
F 2 "" H 1550 3500 60  0000 C CNN
F 3 "" H 1550 3500 60  0000 C CNN
	1    1550 3500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR59
U 1 1 55ADC831
P 2500 3450
F 0 "#PWR59" H 2500 3200 50  0001 C CNN
F 1 "GND" H 2500 3300 50  0000 C CNN
F 2 "" H 2500 3450 60  0000 C CNN
F 3 "" H 2500 3450 60  0000 C CNN
	1    2500 3450
	1    0    0    -1  
$EndComp
$Comp
L D_Schottky D7
U 1 1 55ADD5AF
P 4600 3500
F 0 "D7" H 4600 3600 50  0000 C CNN
F 1 "D_Schottky" H 4600 3400 50  0000 C CNN
F 2 "" H 4600 3500 60  0000 C CNN
F 3 "" H 4600 3500 60  0000 C CNN
	1    4600 3500
	-1   0    0    1   
$EndComp
$Comp
L +BATT #PWR61
U 1 1 55ADD6AC
P 3800 2950
F 0 "#PWR61" H 3800 2800 50  0001 C CNN
F 1 "+BATT" H 3800 3090 50  0000 C CNN
F 2 "" H 3800 2950 60  0000 C CNN
F 3 "" H 3800 2950 60  0000 C CNN
	1    3800 2950
	1    0    0    -1  
$EndComp
$Comp
L D_Schottky D8
U 1 1 55ADD839
P 4600 4050
F 0 "D8" H 4600 4150 50  0000 C CNN
F 1 "D_Schottky" H 4600 3950 50  0000 C CNN
F 2 "" H 4600 4050 60  0000 C CNN
F 3 "" H 4600 4050 60  0000 C CNN
	1    4600 4050
	-1   0    0    1   
$EndComp
$Comp
L BARREL_JACK CON1
U 1 1 55ADD8EB
P 1200 4150
F 0 "CON1" H 1200 4400 60  0000 C CNN
F 1 "BARREL_JACK" H 1200 3950 60  0000 C CNN
F 2 "" H 1200 4150 60  0000 C CNN
F 3 "" H 1200 4150 60  0000 C CNN
	1    1200 4150
	1    0    0    -1  
$EndComp
$Comp
L FDMA530PZ Q5
U 1 1 55ADE24A
P 9900 3700
F 0 "Q5" H 9760 3810 50  0000 R CNN
F 1 "FDMA530PZ" H 10500 3700 50  0000 R CNN
F 2 "" H 10100 3800 29  0000 C CNN
F 3 "" H 9900 3700 60  0000 C CNN
	1    9900 3700
	0    1    -1   0   
$EndComp
Text HLabel 10600 3500 2    60   Output ~ 0
Vsys
$Comp
L C C55
U 1 1 55ADF004
P 7800 5750
F 0 "C55" H 7825 5850 50  0000 L CNN
F 1 "33nF" H 7825 5650 50  0000 L CNN
F 2 "" H 7838 5600 30  0000 C CNN
F 3 "" H 7800 5750 60  0000 C CNN
	1    7800 5750
	1    0    0    -1  
$EndComp
$Comp
L C C56
U 1 1 55ADF053
P 8050 5750
F 0 "C56" H 8075 5850 50  0000 L CNN
F 1 "1uF" H 8075 5650 50  0000 L CNN
F 2 "" H 8088 5600 30  0000 C CNN
F 3 "" H 8050 5750 60  0000 C CNN
	1    8050 5750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR70
U 1 1 55ADF0A7
P 7800 6100
F 0 "#PWR70" H 7800 5850 50  0001 C CNN
F 1 "GND" H 7800 5950 50  0000 C CNN
F 2 "" H 7800 6100 60  0000 C CNN
F 3 "" H 7800 6100 60  0000 C CNN
	1    7800 6100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR71
U 1 1 55ADF0EA
P 8050 6100
F 0 "#PWR71" H 8050 5850 50  0001 C CNN
F 1 "GND" H 8050 5950 50  0000 C CNN
F 2 "" H 8050 6100 60  0000 C CNN
F 3 "" H 8050 6100 60  0000 C CNN
	1    8050 6100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR69
U 1 1 55ADF4DA
P 7550 6100
F 0 "#PWR69" H 7550 5850 50  0001 C CNN
F 1 "GND" H 7550 5950 50  0000 C CNN
F 2 "" H 7550 6100 60  0000 C CNN
F 3 "" H 7550 6100 60  0000 C CNN
	1    7550 6100
	1    0    0    -1  
$EndComp
$Comp
L C C53
U 1 1 55ADF7A7
P 6050 5750
F 0 "C53" H 6075 5850 50  0000 L CNN
F 1 "100nF" H 6075 5650 50  0000 L CNN
F 2 "" H 6088 5600 30  0000 C CNN
F 3 "" H 6050 5750 60  0000 C CNN
	1    6050 5750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR63
U 1 1 55ADF8F7
P 6050 6100
F 0 "#PWR63" H 6050 5850 50  0001 C CNN
F 1 "GND" H 6050 5950 50  0000 C CNN
F 2 "" H 6050 6100 60  0000 C CNN
F 3 "" H 6050 6100 60  0000 C CNN
	1    6050 6100
	1    0    0    -1  
$EndComp
$Comp
L C C54
U 1 1 55ADFAD5
P 7000 5750
F 0 "C54" H 7025 5850 50  0000 L CNN
F 1 "100nF" H 7025 5650 50  0000 L CNN
F 2 "" H 7038 5600 30  0000 C CNN
F 3 "" H 7000 5750 60  0000 C CNN
	1    7000 5750
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR67
U 1 1 55ADFBC3
P 7000 6100
F 0 "#PWR67" H 7000 5850 50  0001 C CNN
F 1 "GND" H 7000 5950 50  0000 C CNN
F 2 "" H 7000 6100 60  0000 C CNN
F 3 "" H 7000 6100 60  0000 C CNN
	1    7000 6100
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH SW10
U 1 1 55ADFD50
P 6550 5650
F 0 "SW10" H 6700 5760 50  0000 C CNN
F 1 "SW_PUSH" H 6550 5570 50  0000 C CNN
F 2 "" H 6550 5650 60  0000 C CNN
F 3 "" H 6550 5650 60  0000 C CNN
	1    6550 5650
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR64
U 1 1 55ADFE71
P 6550 6100
F 0 "#PWR64" H 6550 5850 50  0001 C CNN
F 1 "GND" H 6550 5950 50  0000 C CNN
F 2 "" H 6550 6100 60  0000 C CNN
F 3 "" H 6550 6100 60  0000 C CNN
	1    6550 6100
	1    0    0    -1  
$EndComp
$Comp
L R R56
U 1 1 55AE03CF
P 9100 4150
F 0 "R56" V 9180 4150 50  0000 C CNN
F 1 "10K" V 9100 4150 50  0000 C CNN
F 2 "" V 9030 4150 30  0000 C CNN
F 3 "" H 9100 4150 30  0000 C CNN
	1    9100 4150
	0    1    1    0   
$EndComp
$Comp
L R R55
U 1 1 55AE05D7
P 8500 4150
F 0 "R55" V 8580 4150 50  0000 C CNN
F 1 "100K" V 8500 4150 50  0000 C CNN
F 2 "" V 8430 4150 30  0000 C CNN
F 3 "" H 8500 4150 30  0000 C CNN
	1    8500 4150
	0    1    1    0   
$EndComp
$Comp
L ZENER D10
U 1 1 55AE0AEB
P 9400 3800
F 0 "D10" H 9400 3900 50  0000 C CNN
F 1 "BZX384-B16" H 9400 3700 50  0000 C CNN
F 2 "" H 9400 3800 60  0000 C CNN
F 3 "" H 9400 3800 60  0000 C CNN
	1    9400 3800
	0    1    1    0   
$EndComp
$Comp
L LTC2954CTS8-2 U7
U 1 1 55AE13DC
P 7750 4850
F 0 "U7" H 8050 5250 60  0000 C CNN
F 1 "LTC2954CTS8-2" H 7550 5250 60  0000 C CNN
F 2 "" H 7750 5480 60  0000 C CNN
F 3 "" H 7750 5480 60  0000 C CNN
	1    7750 4850
	1    0    0    -1  
$EndComp
$Comp
L OS102011MA1QN1 SW11
U 1 1 55AE2015
P 6850 2850
F 0 "SW11" V 6850 3350 60  0000 C CNN
F 1 "OS102011MA1QN1" V 6700 2500 60  0000 C CNN
F 2 "" H 6850 3000 60  0000 C CNN
F 3 "" H 6850 3000 60  0000 C CNN
	1    6850 2850
	0    1    1    0   
$EndComp
Wire Wire Line
	1500 1200 7500 1200
Wire Wire Line
	4200 800  4200 1200
Connection ~ 4200 1200
Wire Wire Line
	3000 750  3200 750 
Wire Wire Line
	3200 750  3200 1200
Connection ~ 3200 1200
Wire Wire Line
	3000 850  3200 850 
Connection ~ 3200 850 
Wire Wire Line
	1500 1300 2000 1300
Wire Wire Line
	6900 1300 7500 1300
Wire Wire Line
	6050 1800 6900 1800
Wire Wire Line
	6900 1800 6900 1300
Wire Wire Line
	2000 1800 5650 1800
Wire Wire Line
	2000 1300 2000 1800
Wire Wire Line
	2950 1450 3200 1450
Wire Wire Line
	3200 1450 3200 1800
Connection ~ 3200 1800
Wire Wire Line
	2950 1550 3200 1550
Connection ~ 3200 1550
Wire Wire Line
	3800 800  3800 1800
Connection ~ 3800 1800
Wire Wire Line
	4850 2050 4850 1800
Connection ~ 4850 1800
Wire Wire Line
	5300 1950 5300 1800
Connection ~ 5300 1800
Wire Wire Line
	5550 1900 5650 1900
Wire Wire Line
	5550 1700 5550 1900
Connection ~ 5550 1800
Wire Wire Line
	5550 1700 5650 1700
Wire Wire Line
	6050 1700 6150 1700
Wire Wire Line
	6150 1600 6150 1900
Connection ~ 6150 1800
Wire Wire Line
	6050 1600 6150 1600
Connection ~ 6150 1700
Wire Wire Line
	6150 1900 6050 1900
Wire Wire Line
	4850 2350 4850 2550
Wire Wire Line
	4550 2550 5850 2550
Wire Wire Line
	5300 2550 5300 2350
Wire Wire Line
	5850 2550 5850 2200
Connection ~ 5300 2550
Connection ~ 3300 11100
Wire Wire Line
	3300 11100 3300 10900
Connection ~ 3300 10100
Wire Wire Line
	3300 10100 3650 10100
Wire Wire Line
	3300 9750 3300 10300
Connection ~ 3300 9150
Wire Wire Line
	3300 9150 3300 9450
Connection ~ 2450 9150
Wire Wire Line
	3950 9150 3950 9900
Wire Wire Line
	3950 10550 3950 10300
Connection ~ 2450 11100
Wire Wire Line
	3950 11100 3950 10850
Connection ~ 1750 10100
Connection ~ 1750 9150
Wire Wire Line
	1750 9300 1750 9150
Connection ~ 1750 11100
Wire Wire Line
	1750 10750 1750 11100
Wire Wire Line
	1750 9900 1750 10450
Wire Wire Line
	2150 10100 1750 10100
Wire Wire Line
	650  11100 650  10250
Wire Wire Line
	650  11100 3950 11100
Wire Wire Line
	2450 10300 2450 11100
Wire Wire Line
	2450 9700 2450 9900
Wire Wire Line
	2450 9150 2450 9400
Wire Wire Line
	650  9150 3950 9150
Wire Wire Line
	650  9950 650  9150
Connection ~ 4850 2550
Wire Wire Line
	4250 2550 2500 2550
Wire Wire Line
	2500 2550 2500 2750
Wire Wire Line
	2200 2950 2050 2950
Wire Wire Line
	1250 2950 1750 2950
Wire Wire Line
	1550 2950 1550 3100
Wire Wire Line
	1550 3400 1550 3500
Connection ~ 1550 2950
Wire Wire Line
	2500 3450 2500 3150
Wire Wire Line
	3800 2950 3800 3500
Wire Wire Line
	3800 3500 4450 3500
Wire Wire Line
	1500 4050 4450 4050
Wire Wire Line
	4750 3500 6900 3500
Wire Wire Line
	7000 3500 9700 3500
Wire Wire Line
	9700 3600 9600 3600
Wire Wire Line
	9600 3600 9600 3500
Connection ~ 9600 3500
Wire Wire Line
	10100 3500 10600 3500
Wire Wire Line
	10200 3400 10100 3400
Wire Wire Line
	10200 3200 10200 3600
Connection ~ 10200 3500
Wire Wire Line
	10200 3600 10100 3600
Wire Wire Line
	10100 3300 10200 3300
Connection ~ 10200 3400
Wire Wire Line
	10100 3200 10200 3200
Connection ~ 10200 3300
Wire Wire Line
	4750 4050 5100 4050
Wire Wire Line
	5100 4050 5100 3500
Connection ~ 5100 3500
Wire Wire Line
	7800 5450 7800 5600
Wire Wire Line
	7800 5900 7800 6100
Wire Wire Line
	8050 5900 8050 6100
Wire Wire Line
	8050 5450 8050 5600
Wire Wire Line
	7000 3150 7000 4600
Wire Wire Line
	7000 4600 7150 4600
Connection ~ 7000 3500
Wire Wire Line
	6050 5900 6050 6100
Wire Wire Line
	6050 5000 7150 5000
Wire Wire Line
	7000 5000 7000 5600
Wire Wire Line
	7000 5900 7000 6100
Wire Wire Line
	6550 6100 6550 5950
Wire Wire Line
	6550 5350 6550 5000
Connection ~ 7000 5000
Wire Wire Line
	8350 4150 8150 4150
Wire Wire Line
	8150 4150 8150 3500
Connection ~ 8150 3500
Wire Wire Line
	9250 4150 9900 4150
Wire Wire Line
	9900 4150 9900 3900
Wire Wire Line
	8650 4150 8950 4150
Wire Wire Line
	8350 4600 8800 4600
Wire Wire Line
	8800 4600 8800 4150
Connection ~ 8800 4150
Wire Wire Line
	9400 3600 9400 3500
Connection ~ 9400 3500
Wire Wire Line
	9400 4000 9400 4150
Connection ~ 9400 4150
Wire Wire Line
	7550 5450 7550 6100
Wire Wire Line
	6900 3500 6900 3150
$Comp
L GND #PWR68
U 1 1 55AE28FA
P 7100 3250
F 0 "#PWR68" H 7100 3000 50  0001 C CNN
F 1 "GND" H 7100 3100 50  0000 C CNN
F 2 "" H 7100 3250 60  0000 C CNN
F 3 "" H 7100 3250 60  0000 C CNN
	1    7100 3250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR66
U 1 1 55AE294B
P 6700 3250
F 0 "#PWR66" H 6700 3000 50  0001 C CNN
F 1 "GND" H 6700 3100 50  0000 C CNN
F 2 "" H 6700 3250 60  0000 C CNN
F 3 "" H 6700 3250 60  0000 C CNN
	1    6700 3250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR65
U 1 1 55AE299C
P 6600 3250
F 0 "#PWR65" H 6600 3000 50  0001 C CNN
F 1 "GND" H 6600 3100 50  0000 C CNN
F 2 "" H 6600 3250 60  0000 C CNN
F 3 "" H 6600 3250 60  0000 C CNN
	1    6600 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 3150 6600 3250
Wire Wire Line
	6700 3150 6700 3250
Wire Wire Line
	7100 3150 7100 3250
Wire Wire Line
	6050 5600 6050 5000
Connection ~ 6550 5000
Wire Wire Line
	1500 4150 2400 4150
$EndSCHEMATC
