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
Sheet 18 23
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Entry Wire Line
	3300 800  3400 900 
Entry Wire Line
	3300 900  3400 1000
Entry Wire Line
	3300 1000 3400 1100
Entry Wire Line
	3300 1100 3400 1200
Entry Wire Line
	3300 1200 3400 1300
Entry Wire Line
	3300 1300 3400 1400
Entry Wire Line
	3300 1400 3400 1500
Entry Wire Line
	3300 1500 3400 1600
Entry Wire Line
	3300 1700 3400 1800
Entry Wire Line
	3300 1800 3400 1900
Entry Wire Line
	3300 1900 3400 2000
Entry Wire Line
	3300 2000 3400 2100
Entry Wire Line
	3300 2100 3400 2200
Entry Wire Line
	3300 2200 3400 2300
Entry Wire Line
	3300 2300 3400 2400
Entry Wire Line
	3300 2400 3400 2500
Entry Wire Line
	3300 2600 3400 2700
Entry Wire Line
	3300 2700 3400 2800
Entry Wire Line
	3300 2800 3400 2900
Entry Wire Line
	3300 2900 3400 3000
Entry Wire Line
	3300 3000 3400 3100
Entry Wire Line
	3300 3100 3400 3200
Entry Wire Line
	3300 3200 3400 3300
Entry Wire Line
	3300 3300 3400 3400
Text HLabel 1800 750  0    60   Input ~ 0
DISP0_DAT[0..23]
Text Label 3550 900  0    60   ~ 0
DISP0_DAT0
Text Label 3550 1000 0    60   ~ 0
DISP0_DAT1
Text Label 3550 1100 0    60   ~ 0
DISP0_DAT2
Text Label 3550 1200 0    60   ~ 0
DISP0_DAT3
Text Label 3550 1300 0    60   ~ 0
DISP0_DAT4
Text Label 3550 1400 0    60   ~ 0
DISP0_DAT5
Text Label 3550 1500 0    60   ~ 0
DISP0_DAT6
Text Label 3550 1600 0    60   ~ 0
DISP0_DAT7
Text Label 3550 1800 0    60   ~ 0
DISP0_DAT8
Text Label 3550 1900 0    60   ~ 0
DISP0_DAT9
Text Label 3550 2000 0    60   ~ 0
DISP0_DAT10
Text Label 3550 2100 0    60   ~ 0
DISP0_DAT11
Text Label 3550 2200 0    60   ~ 0
DISP0_DAT12
Text Label 3550 2300 0    60   ~ 0
DISP0_DAT13
Text Label 3550 2400 0    60   ~ 0
DISP0_DAT14
Text Label 3550 2500 0    60   ~ 0
DISP0_DAT15
Text Label 3550 2700 0    60   ~ 0
DISP0_DAT16
Text Label 3550 2800 0    60   ~ 0
DISP0_DAT17
Text Label 3550 2900 0    60   ~ 0
DISP0_DAT18
Text Label 3550 3000 0    60   ~ 0
DISP0_DAT19
Text Label 3550 3100 0    60   ~ 0
DISP0_DAT20
Text Label 3550 3200 0    60   ~ 0
DISP0_DAT21
Text Label 3550 3300 0    60   ~ 0
DISP0_DAT22
Text Label 3550 3400 0    60   ~ 0
DISP0_DAT23
$Comp
L Wurth-68715014522-LCD CN4
U 1 1 561CAF87
P 5100 3550
F 0 "CN4" H 5300 6350 60  0000 C CNN
F 1 "Wurth-68715014522-LCD" V 5500 1500 60  0000 C CNN
F 2 "" H 5100 3900 60  0000 C CNN
F 3 "" H 5100 3900 60  0000 C CNN
	1    5100 3550
	1    0    0    -1  
$EndComp
Text HLabel 1800 3600 0    60   Input ~ 0
DISP0_DRDY
Text HLabel 1800 3700 0    60   Input ~ 0
DISP0_HSYNC
Text HLabel 1800 3800 0    60   Input ~ 0
DISP0_VSYNC
Text HLabel 1800 4000 0    60   Input ~ 0
DISP0_CLK
$Comp
L R R92
U 1 1 561CC37E
P 2500 3600
F 0 "R92" V 2450 3750 50  0000 C CNN
F 1 "22R" V 2500 3600 50  0000 C CNN
F 2 "" V 2430 3600 30  0000 C CNN
F 3 "" H 2500 3600 30  0000 C CNN
	1    2500 3600
	0    1    1    0   
$EndComp
$Comp
L R R93
U 1 1 561CC421
P 2500 3700
F 0 "R93" V 2450 3850 50  0000 C CNN
F 1 "22R" V 2500 3700 50  0000 C CNN
F 2 "" V 2430 3700 30  0000 C CNN
F 3 "" H 2500 3700 30  0000 C CNN
	1    2500 3700
	0    1    1    0   
$EndComp
$Comp
L R R94
U 1 1 561CC43A
P 2500 3800
F 0 "R94" V 2450 3950 50  0000 C CNN
F 1 "22R" V 2500 3800 50  0000 C CNN
F 2 "" V 2430 3800 30  0000 C CNN
F 3 "" H 2500 3800 30  0000 C CNN
	1    2500 3800
	0    1    1    0   
$EndComp
$Comp
L R R95
U 1 1 561CC456
P 2500 4000
F 0 "R95" V 2450 4150 50  0000 C CNN
F 1 "22R" V 2500 4000 50  0000 C CNN
F 2 "" V 2430 4000 30  0000 C CNN
F 3 "" H 2500 4000 30  0000 C CNN
	1    2500 4000
	0    1    1    0   
$EndComp
$Comp
L TSC2046IPWR U15
U 1 1 561E5475
P 7850 5000
F 0 "U15" H 8100 5500 60  0000 C CNN
F 1 "TSC2046IPWR" H 7850 4500 60  0000 C CNN
F 2 "" H 7850 4350 60  0000 C CNN
F 3 "" H 7850 4350 60  0000 C CNN
	1    7850 5000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR138
U 1 1 561E55DA
P 7000 6100
F 0 "#PWR138" H 7000 5850 50  0001 C CNN
F 1 "GND" H 7000 5950 50  0000 C CNN
F 2 "" H 7000 6100 60  0000 C CNN
F 3 "" H 7000 6100 60  0000 C CNN
	1    7000 6100
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR137
U 1 1 561E5805
P 7000 4400
F 0 "#PWR137" H 7000 4250 50  0001 C CNN
F 1 "+3V3" H 7000 4540 50  0000 C CNN
F 2 "" H 7000 4400 60  0000 C CNN
F 3 "" H 7000 4400 60  0000 C CNN
	1    7000 4400
	1    0    0    -1  
$EndComp
Text Label 6400 4850 0    60   ~ 0
TOUCH_X1
Text Label 6400 4950 0    60   ~ 0
TOUCH_Y1
Text Label 6400 5050 0    60   ~ 0
TOUCH_X2
Text Label 6400 5150 0    60   ~ 0
TOUCH_Y2
$Comp
L C C80
U 1 1 561E5B52
P 9050 5850
F 0 "C80" H 9075 5950 50  0000 L CNN
F 1 "0.1u/10%/16V" V 8900 5600 50  0000 L CNN
F 2 "" H 9088 5700 30  0000 C CNN
F 3 "" H 9050 5850 60  0000 C CNN
	1    9050 5850
	1    0    0    -1  
$EndComp
$Comp
L C C82
U 1 1 561E5B9E
P 9550 5850
F 0 "C82" H 9575 5950 50  0000 L CNN
F 1 "0.1u/10%/16V" V 9400 5600 50  0000 L CNN
F 2 "" H 9588 5700 30  0000 C CNN
F 3 "" H 9550 5850 60  0000 C CNN
	1    9550 5850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR139
U 1 1 561E5D84
P 9050 6100
F 0 "#PWR139" H 9050 5850 50  0001 C CNN
F 1 "GND" H 9050 5950 50  0000 C CNN
F 2 "" H 9050 6100 60  0000 C CNN
F 3 "" H 9050 6100 60  0000 C CNN
	1    9050 6100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR143
U 1 1 561E5DA7
P 9550 6100
F 0 "#PWR143" H 9550 5850 50  0001 C CNN
F 1 "GND" H 9550 5950 50  0000 C CNN
F 2 "" H 9550 6100 60  0000 C CNN
F 3 "" H 9550 6100 60  0000 C CNN
	1    9550 6100
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR142
U 1 1 561E5EE9
P 9550 3850
F 0 "#PWR142" H 9550 3700 50  0001 C CNN
F 1 "+3V3" H 9550 3990 50  0000 C CNN
F 2 "" H 9550 3850 60  0000 C CNN
F 3 "" H 9550 3850 60  0000 C CNN
	1    9550 3850
	1    0    0    -1  
$EndComp
Text HLabel 10100 4650 2    60   Input ~ 0
TP_SPI_CLK
Text HLabel 10100 4750 2    60   Input ~ 0
TP_SPI_CS
Text HLabel 10100 4850 2    60   Input ~ 0
TP_SPI_MOSI
Text HLabel 10100 4950 2    60   Output ~ 0
TP_SPI_MISO
Text HLabel 10100 5150 2    60   Output ~ 0
TP_PENIRQ
$Comp
L R R106
U 1 1 561E7AC7
P 8700 4650
F 0 "R106" V 8650 4800 50  0000 C CNN
F 1 "22R" V 8700 4650 50  0000 C CNN
F 2 "" V 8630 4650 30  0000 C CNN
F 3 "" H 8700 4650 30  0000 C CNN
	1    8700 4650
	0    -1   1    0   
$EndComp
$Comp
L R R107
U 1 1 561E7B4E
P 8700 4750
F 0 "R107" V 8650 4900 50  0000 C CNN
F 1 "22R" V 8700 4750 50  0000 C CNN
F 2 "" V 8630 4750 30  0000 C CNN
F 3 "" H 8700 4750 30  0000 C CNN
	1    8700 4750
	0    -1   1    0   
$EndComp
$Comp
L R R108
U 1 1 561E7B7C
P 8700 4850
F 0 "R108" V 8650 5000 50  0000 C CNN
F 1 "22R" V 8700 4850 50  0000 C CNN
F 2 "" V 8630 4850 30  0000 C CNN
F 3 "" H 8700 4850 30  0000 C CNN
	1    8700 4850
	0    -1   1    0   
$EndComp
$Comp
L R R109
U 1 1 561E7BAD
P 8700 4950
F 0 "R109" V 8650 5100 50  0000 C CNN
F 1 "22R" V 8700 4950 50  0000 C CNN
F 2 "" V 8630 4950 30  0000 C CNN
F 3 "" H 8700 4950 30  0000 C CNN
	1    8700 4950
	0    -1   1    0   
$EndComp
$Comp
L R R113
U 1 1 561E7CD1
P 9250 4200
F 0 "R113" H 9200 4050 50  0000 C CNN
F 1 "10K" V 9250 4200 50  0000 C CNN
F 2 "" V 9180 4200 30  0000 C CNN
F 3 "" H 9250 4200 30  0000 C CNN
	1    9250 4200
	1    0    0    1   
$EndComp
$Comp
L R R112
U 1 1 561E7DA5
P 9150 4200
F 0 "R112" H 9100 4050 50  0000 C CNN
F 1 "10K" V 9150 4200 50  0000 C CNN
F 2 "" V 9080 4200 30  0000 C CNN
F 3 "" H 9150 4200 30  0000 C CNN
	1    9150 4200
	1    0    0    1   
$EndComp
$Comp
L R R111
U 1 1 561E7DE3
P 9050 4200
F 0 "R111" H 9000 4050 50  0000 C CNN
F 1 "10K" V 9050 4200 50  0000 C CNN
F 2 "" V 8980 4200 30  0000 C CNN
F 3 "" H 9050 4200 30  0000 C CNN
	1    9050 4200
	1    0    0    1   
$EndComp
$Comp
L R R110
U 1 1 561E7E20
P 8950 4200
F 0 "R110" H 8900 4050 50  0000 C CNN
F 1 "10K" V 8950 4200 50  0000 C CNN
F 2 "" V 8880 4200 30  0000 C CNN
F 3 "" H 8950 4200 30  0000 C CNN
	1    8950 4200
	1    0    0    1   
$EndComp
$Comp
L R R114
U 1 1 561E7E60
P 9350 4200
F 0 "R114" H 9300 4050 50  0000 C CNN
F 1 "10K" V 9350 4200 50  0000 C CNN
F 2 "" V 9280 4200 30  0000 C CNN
F 3 "" H 9350 4200 30  0000 C CNN
	1    9350 4200
	1    0    0    1   
$EndComp
$Comp
L R R115
U 1 1 561E7EB3
P 9450 4200
F 0 "R115" H 9400 4050 50  0000 C CNN
F 1 "10K" V 9450 4200 50  0000 C CNN
F 2 "" V 9380 4200 30  0000 C CNN
F 3 "" H 9450 4200 30  0000 C CNN
	1    9450 4200
	1    0    0    1   
$EndComp
NoConn ~ 7300 5250
Text Label 3550 4300 0    60   ~ 0
TOUCH_X1
Text Label 3550 4500 0    60   ~ 0
TOUCH_Y1
Text Label 3550 4400 0    60   ~ 0
TOUCH_X2
Text Label 3550 4600 0    60   ~ 0
TOUCH_Y2
$Comp
L R R96
U 1 1 561F2AD2
P 2500 4800
F 0 "R96" V 2450 4950 50  0000 C CNN
F 1 "22R" V 2500 4800 50  0000 C CNN
F 2 "" V 2430 4800 30  0000 C CNN
F 3 "" H 2500 4800 30  0000 C CNN
	1    2500 4800
	0    1    1    0   
$EndComp
$Comp
L R R97
U 1 1 561F2C2B
P 2500 4900
F 0 "R97" V 2450 5050 50  0000 C CNN
F 1 "22R" V 2500 4900 50  0000 C CNN
F 2 "" V 2430 4900 30  0000 C CNN
F 3 "" H 2500 4900 30  0000 C CNN
	1    2500 4900
	0    1    1    0   
$EndComp
$Comp
L R R98
U 1 1 561F2C77
P 2500 5000
F 0 "R98" V 2450 5150 50  0000 C CNN
F 1 "22R" V 2500 5000 50  0000 C CNN
F 2 "" V 2430 5000 30  0000 C CNN
F 3 "" H 2500 5000 30  0000 C CNN
	1    2500 5000
	0    1    1    0   
$EndComp
$Comp
L R R99
U 1 1 561F2CC6
P 2500 5100
F 0 "R99" V 2450 5250 50  0000 C CNN
F 1 "22R" V 2500 5100 50  0000 C CNN
F 2 "" V 2430 5100 30  0000 C CNN
F 3 "" H 2500 5100 30  0000 C CNN
	1    2500 5100
	0    1    1    0   
$EndComp
$Comp
L R R100
U 1 1 561F2D18
P 2500 5200
F 0 "R100" V 2450 5350 50  0000 C CNN
F 1 "22R" V 2500 5200 50  0000 C CNN
F 2 "" V 2430 5200 30  0000 C CNN
F 3 "" H 2500 5200 30  0000 C CNN
	1    2500 5200
	0    1    1    0   
$EndComp
$Comp
L R R101
U 1 1 561F2D6D
P 2500 5300
F 0 "R101" V 2450 5450 50  0000 C CNN
F 1 "22R" V 2500 5300 50  0000 C CNN
F 2 "" V 2430 5300 30  0000 C CNN
F 3 "" H 2500 5300 30  0000 C CNN
	1    2500 5300
	0    1    1    0   
$EndComp
$Comp
L R R103
U 1 1 561F2DC5
P 2500 6200
F 0 "R103" V 2450 6350 50  0000 C CNN
F 1 "22R" V 2500 6200 50  0000 C CNN
F 2 "" V 2430 6200 30  0000 C CNN
F 3 "" H 2500 6200 30  0000 C CNN
	1    2500 6200
	0    1    1    0   
$EndComp
$Comp
L +3V3 #PWR129
U 1 1 561F4483
P 3700 5600
F 0 "#PWR129" H 3700 5450 50  0001 C CNN
F 1 "+3V3" H 3700 5740 50  0000 C CNN
F 2 "" H 3700 5600 60  0000 C CNN
F 3 "" H 3700 5600 60  0000 C CNN
	1    3700 5600
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR130
U 1 1 561F451C
P 3700 5900
F 0 "#PWR130" H 3700 5750 50  0001 C CNN
F 1 "+5V" H 3700 6040 50  0000 C CNN
F 2 "" H 3700 5900 60  0000 C CNN
F 3 "" H 3700 5900 60  0000 C CNN
	1    3700 5900
	0    -1   -1   0   
$EndComp
$Comp
L +3V3 #PWR131
U 1 1 561F49E1
P 3750 6850
F 0 "#PWR131" H 3750 6700 50  0001 C CNN
F 1 "+3V3" H 3750 6990 50  0000 C CNN
F 2 "" H 3750 6850 60  0000 C CNN
F 3 "" H 3750 6850 60  0000 C CNN
	1    3750 6850
	-1   0    0    1   
$EndComp
$Comp
L R R105
U 1 1 561F4C11
P 4050 6550
F 0 "R105" H 4000 6400 50  0000 C CNN
F 1 "10K" V 4050 6550 50  0000 C CNN
F 2 "" V 3980 6550 30  0000 C CNN
F 3 "" H 4050 6550 30  0000 C CNN
	1    4050 6550
	1    0    0    1   
$EndComp
$Comp
L GND #PWR132
U 1 1 561F4D74
P 4050 6850
F 0 "#PWR132" H 4050 6600 50  0001 C CNN
F 1 "GND" H 4050 6700 50  0000 C CNN
F 2 "" H 4050 6850 60  0000 C CNN
F 3 "" H 4050 6850 60  0000 C CNN
	1    4050 6850
	1    0    0    -1  
$EndComp
Text HLabel 1800 6200 0    60   Input ~ 0
DISP0_PWREN
Text HLabel 1800 4800 0    60   Input ~ 0
DISP0_SPI_CLK
Text HLabel 1800 4900 0    60   Input ~ 0
DISP0_SPI_MOSI
Text HLabel 1800 5000 0    60   Output ~ 0
DISP0_SPI_MISO
Text HLabel 1800 5100 0    60   Input ~ 0
DISP0_SPI_CS
Text HLabel 1800 5200 0    60   Input ~ 0
DISP0_I2C_CLK
Text HLabel 1800 5300 0    60   BiDi ~ 0
DISP0_I2C_SDA
$Comp
L GND #PWR128
U 1 1 561F5E25
P 3700 5400
F 0 "#PWR128" H 3700 5150 50  0001 C CNN
F 1 "GND" H 3700 5250 50  0000 C CNN
F 2 "" H 3700 5400 60  0000 C CNN
F 3 "" H 3700 5400 60  0000 C CNN
	1    3700 5400
	0    1    1    0   
$EndComp
Text HLabel 1800 6100 0    60   Input ~ 0
DISP0_REST
$Comp
L R R104
U 1 1 561F6C18
P 3750 6550
F 0 "R104" H 3700 6400 50  0000 C CNN
F 1 "10K" V 3750 6550 50  0000 C CNN
F 2 "" V 3680 6550 30  0000 C CNN
F 3 "" H 3750 6550 30  0000 C CNN
	1    3750 6550
	1    0    0    1   
$EndComp
$Comp
L R R102
U 1 1 561F7023
P 2500 6100
F 0 "R102" V 2450 6250 50  0000 C CNN
F 1 "22R" V 2500 6100 50  0000 C CNN
F 2 "" V 2430 6100 30  0000 C CNN
F 3 "" H 2500 6100 30  0000 C CNN
	1    2500 6100
	0    1    1    0   
$EndComp
$Comp
L C C76
U 1 1 561F7598
P 7600 1350
F 0 "C76" H 7625 1450 50  0000 L CNN
F 1 "1u/10%/10V" V 7450 1100 50  0000 L CNN
F 2 "" H 7638 1200 30  0000 C CNN
F 3 "" H 7600 1350 60  0000 C CNN
	1    7600 1350
	1    0    0    -1  
$EndComp
$Comp
L C C72
U 1 1 561F76A1
P 6400 1350
F 0 "C72" H 6425 1450 50  0000 L CNN
F 1 "47u/20%/6.3V" V 6250 1100 50  0000 L CNN
F 2 "" H 6438 1200 30  0000 C CNN
F 3 "" H 6400 1350 60  0000 C CNN
	1    6400 1350
	1    0    0    -1  
$EndComp
$Comp
L C C74
U 1 1 561F7782
P 7000 1350
F 0 "C74" H 7025 1450 50  0000 L CNN
F 1 "0.1u/10%/16V" V 6850 1100 50  0000 L CNN
F 2 "" H 7038 1200 30  0000 C CNN
F 3 "" H 7000 1350 60  0000 C CNN
	1    7000 1350
	1    0    0    -1  
$EndComp
$Comp
L C C78
U 1 1 561F7CFE
P 8200 1350
F 0 "C78" H 8225 1450 50  0000 L CNN
F 1 "0.1u/10%/16V" V 8050 1100 50  0000 L CNN
F 2 "" H 8238 1200 30  0000 C CNN
F 3 "" H 8200 1350 60  0000 C CNN
	1    8200 1350
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR133
U 1 1 561F7FD1
P 6400 800
F 0 "#PWR133" H 6400 650 50  0001 C CNN
F 1 "+3V3" H 6400 940 50  0000 C CNN
F 2 "" H 6400 800 60  0000 C CNN
F 3 "" H 6400 800 60  0000 C CNN
	1    6400 800 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR134
U 1 1 561F807C
P 6400 1850
F 0 "#PWR134" H 6400 1600 50  0001 C CNN
F 1 "GND" H 6400 1700 50  0000 C CNN
F 2 "" H 6400 1850 60  0000 C CNN
F 3 "" H 6400 1850 60  0000 C CNN
	1    6400 1850
	1    0    0    -1  
$EndComp
$Comp
L C C77
U 1 1 561F88B1
P 7600 2900
F 0 "C77" H 7625 3000 50  0000 L CNN
F 1 "1u/10%/10V" V 7450 2650 50  0000 L CNN
F 2 "" H 7638 2750 30  0000 C CNN
F 3 "" H 7600 2900 60  0000 C CNN
	1    7600 2900
	1    0    0    -1  
$EndComp
$Comp
L C C73
U 1 1 561F88B7
P 6400 2900
F 0 "C73" H 6425 3000 50  0000 L CNN
F 1 "47u/20%/6.3V" V 6250 2650 50  0000 L CNN
F 2 "" H 6438 2750 30  0000 C CNN
F 3 "" H 6400 2900 60  0000 C CNN
	1    6400 2900
	1    0    0    -1  
$EndComp
$Comp
L C C75
U 1 1 561F88BD
P 7000 2900
F 0 "C75" H 7025 3000 50  0000 L CNN
F 1 "0.1u/10%/16V" V 6850 2650 50  0000 L CNN
F 2 "" H 7038 2750 30  0000 C CNN
F 3 "" H 7000 2900 60  0000 C CNN
	1    7000 2900
	1    0    0    -1  
$EndComp
$Comp
L C C79
U 1 1 561F88CD
P 8200 2900
F 0 "C79" H 8225 3000 50  0000 L CNN
F 1 "0.1u/10%/16V" V 8050 2650 50  0000 L CNN
F 2 "" H 8238 2750 30  0000 C CNN
F 3 "" H 8200 2900 60  0000 C CNN
	1    8200 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR136
U 1 1 561F88DD
P 6400 3400
F 0 "#PWR136" H 6400 3150 50  0001 C CNN
F 1 "GND" H 6400 3250 50  0000 C CNN
F 2 "" H 6400 3400 60  0000 C CNN
F 3 "" H 6400 3400 60  0000 C CNN
	1    6400 3400
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR135
U 1 1 561F8ABA
P 6400 2350
F 0 "#PWR135" H 6400 2200 50  0001 C CNN
F 1 "+5V" H 6400 2490 50  0000 C CNN
F 2 "" H 6400 2350 60  0000 C CNN
F 3 "" H 6400 2350 60  0000 C CNN
	1    6400 2350
	1    0    0    -1  
$EndComp
$Comp
L C C83
U 1 1 561F8ED0
P 9850 1350
F 0 "C83" H 9875 1450 50  0000 L CNN
F 1 "1u/10%/10V" V 9700 1100 50  0000 L CNN
F 2 "" H 9888 1200 30  0000 C CNN
F 3 "" H 9850 1350 60  0000 C CNN
	1    9850 1350
	1    0    0    -1  
$EndComp
$Comp
L C C81
U 1 1 561F8EDC
P 9250 1350
F 0 "C81" H 9275 1450 50  0000 L CNN
F 1 "0.1u/10%/16V" V 9100 1100 50  0000 L CNN
F 2 "" H 9288 1200 30  0000 C CNN
F 3 "" H 9250 1350 60  0000 C CNN
	1    9250 1350
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR140
U 1 1 561F8EEE
P 9250 800
F 0 "#PWR140" H 9250 650 50  0001 C CNN
F 1 "+3V3" H 9250 940 50  0000 C CNN
F 2 "" H 9250 800 60  0000 C CNN
F 3 "" H 9250 800 60  0000 C CNN
	1    9250 800 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR141
U 1 1 561F8EF4
P 9250 1850
F 0 "#PWR141" H 9250 1600 50  0001 C CNN
F 1 "GND" H 9250 1700 50  0000 C CNN
F 2 "" H 9250 1850 60  0000 C CNN
F 3 "" H 9250 1850 60  0000 C CNN
	1    9250 1850
	1    0    0    -1  
$EndComp
Text Notes 4500 6800 0    120  ~ 24
mars board_V1.0.pdf\npage 12 (LCD PORT)
$Comp
L R R91
U 1 1 561FD8C2
P 2500 3500
F 0 "R91" V 2450 3650 50  0000 C CNN
F 1 "22R" V 2500 3500 50  0000 C CNN
F 2 "" V 2430 3500 30  0000 C CNN
F 3 "" H 2500 3500 30  0000 C CNN
	1    2500 3500
	0    1    1    0   
$EndComp
Text HLabel 1800 3500 0    60   Input ~ 0
DISP0_CNTRST
Text Notes 6550 4100 0    120  ~ 24
Research for \ncapacitive controller???
$Comp
L R R90
U 1 1 5624940C
P 2500 3400
F 0 "R90" V 2450 3550 50  0000 C CNN
F 1 "NA" V 2500 3400 50  0000 C CNN
F 2 "" V 2430 3400 30  0000 C CNN
F 3 "" H 2500 3400 30  0000 C CNN
	1    2500 3400
	0    1    1    0   
$EndComp
$Comp
L GND #PWR127
U 1 1 5624CCFB
P 2200 3300
F 0 "#PWR127" H 2200 3050 50  0001 C CNN
F 1 "GND" H 2200 3150 50  0000 C CNN
F 2 "" H 2200 3300 60  0000 C CNN
F 3 "" H 2200 3300 60  0000 C CNN
	1    2200 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	4550 900  3400 900 
Wire Wire Line
	4550 1000 3400 1000
Wire Wire Line
	4550 1100 3400 1100
Wire Wire Line
	4550 1200 3400 1200
Wire Wire Line
	4550 1300 3400 1300
Wire Wire Line
	4550 1400 3400 1400
Wire Wire Line
	4550 1500 3400 1500
Wire Wire Line
	4550 1600 3400 1600
Wire Wire Line
	4550 1800 3400 1800
Wire Wire Line
	4550 1900 3400 1900
Wire Wire Line
	4550 2000 3400 2000
Wire Wire Line
	4550 2100 3400 2100
Wire Wire Line
	4550 2200 3400 2200
Wire Wire Line
	4550 2300 3400 2300
Wire Wire Line
	4550 2400 3400 2400
Wire Wire Line
	4550 2500 3400 2500
Wire Wire Line
	4550 2700 3400 2700
Wire Wire Line
	4550 2800 3400 2800
Wire Wire Line
	4550 2900 3400 2900
Wire Wire Line
	4550 3000 3400 3000
Wire Wire Line
	4550 3100 3400 3100
Wire Wire Line
	4550 3200 3400 3200
Wire Wire Line
	4550 3300 3400 3300
Wire Wire Line
	4550 3400 3400 3400
Wire Bus Line
	3300 750  3300 3300
Wire Bus Line
	3300 750  1800 750 
Wire Wire Line
	2650 3600 4550 3600
Wire Wire Line
	2650 3700 4550 3700
Wire Wire Line
	2650 3800 4550 3800
Wire Wire Line
	2650 4000 4550 4000
Wire Wire Line
	1800 3600 2350 3600
Wire Wire Line
	1800 3700 2350 3700
Wire Wire Line
	1800 3800 2350 3800
Wire Wire Line
	1800 4000 2350 4000
Wire Wire Line
	7000 4400 7000 4750
Wire Wire Line
	7000 4650 7300 4650
Wire Wire Line
	7300 5350 7000 5350
Wire Wire Line
	7000 5350 7000 6100
Wire Wire Line
	7000 4750 7300 4750
Connection ~ 7000 4650
Wire Wire Line
	6400 4850 7300 4850
Wire Wire Line
	6400 4950 7300 4950
Wire Wire Line
	6400 5050 7300 5050
Wire Wire Line
	6400 5150 7300 5150
Wire Wire Line
	9050 6100 9050 6000
Wire Wire Line
	9550 6100 9550 6000
Wire Wire Line
	8400 5350 9050 5350
Wire Wire Line
	9050 5350 9050 5700
Wire Wire Line
	9550 5250 8400 5250
Wire Wire Line
	9550 3850 9550 5700
Wire Wire Line
	8550 4650 8400 4650
Wire Wire Line
	8550 4750 8400 4750
Wire Wire Line
	8550 4850 8400 4850
Wire Wire Line
	8550 4950 8400 4950
Wire Wire Line
	8850 4650 10100 4650
Wire Wire Line
	8850 4750 10100 4750
Wire Wire Line
	8850 4850 10100 4850
Wire Wire Line
	8850 4950 10100 4950
Wire Wire Line
	8400 5150 10100 5150
Wire Wire Line
	8950 4350 8950 4650
Connection ~ 8950 4650
Wire Wire Line
	9050 4350 9050 4750
Connection ~ 9050 4750
Wire Wire Line
	9150 4350 9150 4850
Connection ~ 9150 4850
Wire Wire Line
	9250 4350 9250 4950
Connection ~ 9250 4950
Wire Wire Line
	9350 4350 9350 5050
Wire Wire Line
	9350 5050 8400 5050
Wire Wire Line
	9450 4350 9450 5150
Connection ~ 9450 5150
Connection ~ 9550 5250
Wire Wire Line
	9450 3950 9450 4050
Wire Wire Line
	8950 3950 9550 3950
Wire Wire Line
	8950 3950 8950 4050
Wire Wire Line
	9350 4050 9350 3950
Connection ~ 9350 3950
Wire Wire Line
	9250 4050 9250 3950
Connection ~ 9250 3950
Wire Wire Line
	9150 4050 9150 3950
Connection ~ 9150 3950
Wire Wire Line
	9050 4050 9050 3950
Connection ~ 9050 3950
Connection ~ 9550 3950
Connection ~ 9450 3950
Wire Wire Line
	3550 4300 4550 4300
Wire Wire Line
	3550 4400 4550 4400
Wire Wire Line
	3550 4500 4550 4500
Wire Wire Line
	3550 4600 4550 4600
Wire Wire Line
	2650 4800 4550 4800
Wire Wire Line
	2650 4900 4550 4900
Wire Wire Line
	2650 5000 4550 5000
Wire Wire Line
	2650 5100 4550 5100
Wire Wire Line
	2650 5200 4550 5200
Wire Wire Line
	2650 5300 4550 5300
Wire Wire Line
	3700 5600 4550 5600
Wire Wire Line
	4550 5700 4350 5700
Wire Wire Line
	4350 5700 4350 5600
Connection ~ 4350 5600
Wire Wire Line
	3700 5900 4550 5900
Wire Wire Line
	4550 5800 4350 5800
Wire Wire Line
	4350 5800 4350 5900
Connection ~ 4350 5900
Wire Wire Line
	4050 6700 4050 6850
Wire Wire Line
	1800 4800 2350 4800
Wire Wire Line
	1800 4900 2350 4900
Wire Wire Line
	1800 5000 2350 5000
Wire Wire Line
	1800 5100 2350 5100
Wire Wire Line
	1800 5200 2350 5200
Wire Wire Line
	1800 5300 2350 5300
Wire Wire Line
	3700 5400 4550 5400
Wire Wire Line
	4350 4100 4550 4100
Wire Wire Line
	4350 1700 4350 5400
Connection ~ 4350 5400
Connection ~ 4350 4100
Connection ~ 4350 3900
Wire Wire Line
	4550 2600 4350 2600
Wire Wire Line
	4550 1700 4350 1700
Connection ~ 4350 2600
Wire Wire Line
	3750 6700 3750 6850
Wire Wire Line
	2650 6100 4550 6100
Wire Wire Line
	2650 6200 4550 6200
Wire Wire Line
	1800 6100 2350 6100
Wire Wire Line
	1800 6200 2350 6200
Wire Wire Line
	4050 6400 4050 6200
Connection ~ 4050 6200
Wire Wire Line
	3750 6400 3750 6100
Connection ~ 3750 6100
Wire Wire Line
	7600 950  7600 1200
Wire Wire Line
	6400 950  8200 950 
Wire Wire Line
	6400 800  6400 1200
Wire Wire Line
	6400 1500 6400 1850
Wire Wire Line
	6400 1700 8200 1700
Wire Wire Line
	7600 1700 7600 1500
Wire Wire Line
	7000 1200 7000 950 
Connection ~ 7000 950 
Wire Wire Line
	7000 1500 7000 1700
Connection ~ 7000 1700
Wire Wire Line
	8200 950  8200 1200
Connection ~ 7600 950 
Wire Wire Line
	8200 1700 8200 1500
Connection ~ 7600 1700
Connection ~ 6400 950 
Connection ~ 6400 1700
Wire Wire Line
	7600 2500 7600 2750
Wire Wire Line
	6400 2500 8200 2500
Wire Wire Line
	6400 2350 6400 2750
Wire Wire Line
	6400 3050 6400 3400
Wire Wire Line
	6400 3250 8200 3250
Wire Wire Line
	7600 3250 7600 3050
Wire Wire Line
	7000 2750 7000 2500
Connection ~ 7000 2500
Wire Wire Line
	7000 3050 7000 3250
Connection ~ 7000 3250
Wire Wire Line
	8200 2500 8200 2750
Connection ~ 7600 2500
Wire Wire Line
	8200 3250 8200 3050
Connection ~ 7600 3250
Connection ~ 6400 2500
Connection ~ 6400 3250
Wire Wire Line
	9250 800  9250 1200
Wire Wire Line
	9250 1500 9250 1850
Wire Wire Line
	9850 1200 9850 950 
Wire Wire Line
	9850 1500 9850 1700
Connection ~ 9250 950 
Connection ~ 9250 1700
Wire Wire Line
	9850 950  9250 950 
Wire Wire Line
	9850 1700 9250 1700
Wire Wire Line
	1800 3500 2350 3500
Wire Wire Line
	2650 3500 2900 3500
Wire Wire Line
	2900 3500 2900 3600
Connection ~ 2900 3600
Wire Wire Line
	3100 3400 2650 3400
Wire Wire Line
	2350 3400 2100 3400
Wire Wire Line
	2100 3400 2100 3500
Connection ~ 2100 3500
Wire Wire Line
	4550 3900 4350 3900
Wire Wire Line
	3100 3500 4550 3500
Wire Wire Line
	3100 3300 3100 3500
Wire Wire Line
	2350 3300 2200 3300
$Comp
L R R89
U 1 1 5624D6FB
P 2500 3300
F 0 "R89" V 2450 3450 50  0000 C CNN
F 1 "0R" V 2500 3300 50  0000 C CNN
F 2 "" V 2430 3300 30  0000 C CNN
F 3 "" H 2500 3300 30  0000 C CNN
	1    2500 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	2650 3300 3100 3300
Connection ~ 3100 3400
$EndSCHEMATC
