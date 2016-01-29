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
$Comp
L CONN_02X10 P?
U 1 1 55768A13
P 3750 2350
F 0 "P?" H 3750 2900 50  0000 C CNN
F 1 "CONN_02X10" V 3750 2350 50  0000 C CNN
F 2 "" H 3750 1150 60  0001 C CNN
F 3 "" H 3750 1150 60  0000 C CNN
	1    3750 2350
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 55768A1A
P 3300 3350
F 0 "#PWR?" H 3300 3100 60  0001 C CNN
F 1 "GND" H 3300 3200 60  0000 C CNN
F 2 "" H 3300 3350 60  0000 C CNN
F 3 "" H 3300 3350 60  0000 C CNN
	1    3300 3350
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X07 P?
U 1 1 55768A50
P 3800 4800
F 0 "P?" H 3800 5200 50  0000 C CNN
F 1 "CONN_01X07" V 3900 4800 50  0000 C CNN
F 2 "" H 3800 4800 60  0001 C CNN
F 3 "" H 3800 4800 60  0000 C CNN
	1    3800 4800
	-1   0    0    -1  
$EndComp
Text Label 4950 4600 2    60   ~ 0
TMS/SWDTA
Text Label 4950 4700 2    60   ~ 0
TCK/SWCLK
Text Label 4950 4900 2    60   ~ 0
RESET#
$Comp
L GND #PWR?
U 1 1 55768A5A
P 4150 5300
F 0 "#PWR?" H 4150 5050 60  0001 C CNN
F 1 "GND" H 4150 5150 60  0000 C CNN
F 2 "" H 4150 5300 60  0000 C CNN
F 3 "" H 4150 5300 60  0000 C CNN
	1    4150 5300
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X01 P?
U 1 1 55768A67
P 7900 2800
F 0 "P?" H 7900 2900 50  0000 C CNN
F 1 "CONN_02X01" H 7900 2700 50  0000 C CNN
F 2 "" H 7900 1600 60  0001 C CNN
F 3 "" H 7900 1600 60  0000 C CNN
	1    7900 2800
	-1   0    0    1   
$EndComp
$Comp
L +3V3 #PWR?
U 1 1 55768A6E
P 4150 3900
F 0 "#PWR?" H 4150 3750 60  0001 C CNN
F 1 "+3V3" H 4150 4040 60  0000 C CNN
F 2 "" H 4150 3900 60  0000 C CNN
F 3 "" H 4150 3900 60  0000 C CNN
	1    4150 3900
	1    0    0    -1  
$EndComp
Text HLabel 8500 2000 2    60   Output ~ 0
TRST
Text HLabel 8500 2500 2    60   Input ~ 0
TDO/SWO
Text HLabel 8500 2300 2    60   Output ~ 0
TCK/SWCLK
Text HLabel 8500 2200 2    60   BiDi ~ 0
TMS/SWDIO
Text HLabel 8500 2100 2    60   BiDi ~ 0
TDI
Text HLabel 8500 2600 2    60   Output ~ 0
RESET#
Text Label 4650 2600 2    60   ~ 0
RESET#
Text Label 4650 2500 2    60   ~ 0
TDO/SWO
Text Label 4650 2300 2    60   ~ 0
TCK/SWCLK
Text Label 4650 2200 2    60   ~ 0
TMS/SWDTA
Text Label 4650 2100 2    60   ~ 0
TDI
Text Label 4650 2000 2    60   ~ 0
TRST
$Comp
L GND #PWR?
U 1 1 5576A90F
P 5900 2400
F 0 "#PWR?" H 5900 2150 60  0001 C CNN
F 1 "GND" H 5900 2250 60  0000 C CNN
F 2 "" H 5900 2400 60  0000 C CNN
F 3 "" H 5900 2400 60  0000 C CNN
	1    5900 2400
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5576A932
P 5900 2700
F 0 "#PWR?" H 5900 2450 60  0001 C CNN
F 1 "GND" H 5900 2550 60  0000 C CNN
F 2 "" H 5900 2700 60  0000 C CNN
F 3 "" H 5900 2700 60  0000 C CNN
	1    5900 2700
	0    -1   -1   0   
$EndComp
Text HLabel 8500 2800 2    60   Output ~ 0
JTAG_POWER_ON
$Comp
L +3V3 #PWR?
U 1 1 5576ACC9
P 5400 1000
F 0 "#PWR?" H 5400 850 60  0001 C CNN
F 1 "+3V3" H 5400 1140 60  0000 C CNN
F 2 "" H 5400 1000 60  0000 C CNN
F 3 "" H 5400 1000 60  0000 C CNN
	1    5400 1000
	1    0    0    -1  
$EndComp
Text Label 4950 5100 2    60   ~ 0
JTAG_POWER
Text Label 4650 2800 2    60   ~ 0
JTAG_POWER
Wire Wire Line
	3500 2000 3300 2000
Wire Wire Line
	3300 2000 3300 3350
Wire Wire Line
	3500 2100 3300 2100
Connection ~ 3300 2100
Wire Wire Line
	3500 2200 3300 2200
Connection ~ 3300 2200
Wire Wire Line
	3500 2300 3300 2300
Connection ~ 3300 2300
Wire Wire Line
	3500 2400 3300 2400
Connection ~ 3300 2400
Wire Wire Line
	3500 2500 3300 2500
Connection ~ 3300 2500
Wire Wire Line
	3500 2600 3300 2600
Connection ~ 3300 2600
Wire Wire Line
	3500 2700 3300 2700
Connection ~ 3300 2700
Wire Wire Line
	3500 2800 3300 2800
Connection ~ 3300 2800
Wire Wire Line
	4000 2000 8500 2000
Wire Wire Line
	4000 2100 8500 2100
Wire Wire Line
	4000 2200 8500 2200
Wire Wire Line
	4000 2300 8500 2300
Wire Wire Line
	4000 2500 8500 2500
Wire Wire Line
	4000 2600 8500 2600
Wire Wire Line
	4000 2800 7650 2800
Wire Wire Line
	4000 1900 4050 1900
Wire Wire Line
	4050 1900 4050 1200
Wire Wire Line
	7050 1200 7050 1400
Connection ~ 6750 1200
Wire Wire Line
	3500 1900 3300 1900
Connection ~ 4050 1200
Wire Wire Line
	4000 4600 4950 4600
Wire Wire Line
	4000 4700 4950 4700
Wire Wire Line
	4000 4900 4950 4900
Wire Wire Line
	4150 4500 4000 4500
Wire Wire Line
	4150 3900 4150 4500
Wire Wire Line
	4000 2400 5300 2400
Wire Wire Line
	5600 2400 5900 2400
Wire Wire Line
	4000 2700 5300 2700
Wire Wire Line
	5600 2700 5900 2700
Wire Wire Line
	8150 2800 8500 2800
Wire Wire Line
	3300 1200 7950 1200
Wire Wire Line
	3300 1900 3300 1200
Wire Wire Line
	5400 1000 5400 1200
Connection ~ 5400 1200
Wire Wire Line
	6750 1400 6750 1200
Wire Wire Line
	4000 5100 4950 5100
Wire Wire Line
	4150 5300 4150 4800
Wire Wire Line
	4150 4800 4000 4800
Wire Wire Line
	7650 1200 7650 1400
Connection ~ 7050 1200
Wire Wire Line
	7350 1400 7350 1200
Connection ~ 7350 1200
Wire Wire Line
	7950 1700 7950 2500
Connection ~ 7950 2500
Wire Wire Line
	7950 1200 7950 1400
Connection ~ 7650 1200
Text Label 4950 5000 2    60   ~ 0
TDO/SWO
Wire Wire Line
	4000 5000 4950 5000
Text Notes 9250 2650 0    60   ~ 12
RESET# is pullud \nup on Buttons sheet
Wire Wire Line
	7650 1700 7650 2300
Connection ~ 7650 2300
Wire Wire Line
	7350 1700 7350 2200
Connection ~ 7350 2200
Wire Wire Line
	7050 1700 7050 2100
Connection ~ 7050 2100
Wire Wire Line
	6750 1700 6750 2000
Connection ~ 6750 2000
$Comp
L R_SMT R?
U 1 1 56289A38
P 5450 2400
F 0 "R?" V 5520 2400 50  0000 C CNN
F 1 "10K/5%" V 5380 2400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5600 2400 30  0001 C CNN
F 3 "" V 5530 2400 30  0000 C CNN
F 4 "0603" V 5450 2400 20  0000 C CNN "SMT"
F 5 "9331700" V 5650 2400 30  0001 C CNN "Part"
F 6 "Farnell" V 5700 2400 30  0001 C CNN "Provider"
	1    5450 2400
	0    1    1    0   
$EndComp
$Comp
L R_SMT R?
U 1 1 56289B1A
P 7950 1550
F 0 "R?" V 8020 1550 50  0000 C CNN
F 1 "10K/5%" V 7880 1550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8100 1550 30  0001 C CNN
F 3 "" V 8030 1550 30  0000 C CNN
F 4 "0603" V 7950 1550 20  0000 C CNN "SMT"
F 5 "9331700" V 8150 1550 30  0001 C CNN "Part"
F 6 "Farnell" V 8200 1550 30  0001 C CNN "Provider"
	1    7950 1550
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56289B65
P 7650 1550
F 0 "R?" V 7720 1550 50  0000 C CNN
F 1 "10K/5%" V 7580 1550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 7800 1550 30  0001 C CNN
F 3 "" V 7730 1550 30  0000 C CNN
F 4 "0603" V 7650 1550 20  0000 C CNN "SMT"
F 5 "9331700" V 7850 1550 30  0001 C CNN "Part"
F 6 "Farnell" V 7900 1550 30  0001 C CNN "Provider"
	1    7650 1550
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56289B9F
P 7350 1550
F 0 "R?" V 7420 1550 50  0000 C CNN
F 1 "10K/5%" V 7280 1550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 7500 1550 30  0001 C CNN
F 3 "" V 7430 1550 30  0000 C CNN
F 4 "0603" V 7350 1550 20  0000 C CNN "SMT"
F 5 "9331700" V 7550 1550 30  0001 C CNN "Part"
F 6 "Farnell" V 7600 1550 30  0001 C CNN "Provider"
	1    7350 1550
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56289BDC
P 7050 1550
F 0 "R?" V 7120 1550 50  0000 C CNN
F 1 "10K/5%" V 6980 1550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 7200 1550 30  0001 C CNN
F 3 "" V 7130 1550 30  0000 C CNN
F 4 "0603" V 7050 1550 20  0000 C CNN "SMT"
F 5 "9331700" V 7250 1550 30  0001 C CNN "Part"
F 6 "Farnell" V 7300 1550 30  0001 C CNN "Provider"
	1    7050 1550
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56289C1C
P 6750 1550
F 0 "R?" V 6820 1550 50  0000 C CNN
F 1 "10K/5%" V 6680 1550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6900 1550 30  0001 C CNN
F 3 "" V 6830 1550 30  0000 C CNN
F 4 "0603" V 6750 1550 20  0000 C CNN "SMT"
F 5 "9331700" V 6950 1550 30  0001 C CNN "Part"
F 6 "Farnell" V 7000 1550 30  0001 C CNN "Provider"
	1    6750 1550
	1    0    0    -1  
$EndComp
$Comp
L R_SMT R?
U 1 1 56289D4E
P 5450 2700
F 0 "R?" V 5520 2700 50  0000 C CNN
F 1 "10K/5%" V 5380 2700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5600 2700 30  0001 C CNN
F 3 "" V 5530 2700 30  0000 C CNN
F 4 "0603" V 5450 2700 20  0000 C CNN "SMT"
F 5 "9331700" V 5650 2700 30  0001 C CNN "Part"
F 6 "Farnell" V 5700 2700 30  0001 C CNN "Provider"
	1    5450 2700
	0    1    1    0   
$EndComp
$EndSCHEMATC
