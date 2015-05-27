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
LIBS:special
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
LIBS:BQ24070
LIBS:LTC2954-2
LIBS:S-8241
LIBS:SC189
LIBS:TPS61032
LIBS:L3GD20
LIBS:LSM303DLHC
LIBS:PCA9306
LIBS:PCA9685
LIBS:NCP551SN18T1G
LIBS:74AVC4T774
LIBS:header
LIBS:pandaext-cache
EELAYER 24 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 8
Title ""
Date "18 jun 2013"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L 74AVC4T774 U?
U 1 1 5158E463
P 3750 2400
F 0 "U?" H 3750 2300 50  0000 C CNN
F 1 "74AVC4T774" H 3550 1450 50  0000 C CNN
F 2 "MODULE" H 4050 3350 50  0001 C CNN
F 3 "DOCUMENTATION" H 3750 2400 50  0001 C CNN
	1    3750 2400
	1    0    0    -1  
$EndComp
$Comp
L 1V8 #PWR?
U 1 1 5158E484
P 2700 800
F 0 "#PWR?" H 2700 940 20  0001 C CNN
F 1 "1V8" H 2700 910 30  0000 C CNN
F 2 "" H 2700 800 60  0000 C CNN
F 3 "" H 2700 800 60  0000 C CNN
	1    2700 800 
	1    0    0    -1  
$EndComp
$Comp
L 3V3 #PWR?
U 1 1 5158E493
P 4750 800
F 0 "#PWR?" H 4750 900 40  0001 C CNN
F 1 "3V3" H 4750 925 40  0000 C CNN
F 2 "" H 4750 800 60  0000 C CNN
F 3 "" H 4750 800 60  0000 C CNN
	1    4750 800 
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5158E4B0
P 5200 1200
F 0 "C?" H 5200 1300 40  0000 L CNN
F 1 "0.1uF" H 5206 1115 40  0000 L CNN
F 2 "~" H 5238 1050 30  0000 C CNN
F 3 "~" H 5200 1200 60  0000 C CNN
	1    5200 1200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5158E4C4
P 5200 1550
F 0 "#PWR?" H 5200 1550 30  0001 C CNN
F 1 "GND" H 5200 1480 30  0001 C CNN
F 2 "" H 5200 1550 60  0000 C CNN
F 3 "" H 5200 1550 60  0000 C CNN
	1    5200 1550
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5158E4E8
P 2250 1200
F 0 "C?" H 2250 1300 40  0000 L CNN
F 1 "0.1uF" H 2256 1115 40  0000 L CNN
F 2 "~" H 2288 1050 30  0000 C CNN
F 3 "~" H 2250 1200 60  0000 C CNN
	1    2250 1200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5158E4EE
P 2250 1550
F 0 "#PWR?" H 2250 1550 30  0001 C CNN
F 1 "GND" H 2250 1480 30  0001 C CNN
F 2 "" H 2250 1550 60  0000 C CNN
F 3 "" H 2250 1550 60  0000 C CNN
	1    2250 1550
	1    0    0    -1  
$EndComp
Text Notes 3350 3750 0    60   ~ 0
DIR   OPERATION\n\nL      B --> A\nH      A --> B
$Comp
L GND #PWR?
U 1 1 5158E537
P 4800 3550
F 0 "#PWR?" H 4800 3550 30  0001 C CNN
F 1 "GND" H 4800 3480 30  0001 C CNN
F 2 "" H 4800 3550 60  0000 C CNN
F 3 "" H 4800 3550 60  0000 C CNN
	1    4800 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 1600 4750 1600
Wire Wire Line
	4750 1600 4750 800 
Wire Wire Line
	3000 1600 2700 1600
Wire Wire Line
	2700 1600 2700 800 
Wire Wire Line
	5200 1000 4750 1000
Connection ~ 4750 1000
Wire Wire Line
	5200 1400 5200 1550
Wire Wire Line
	2250 1400 2250 1550
Wire Wire Line
	2250 1000 2700 1000
Connection ~ 2700 1000
Wire Notes Line
	3300 3600 4200 3600
Wire Notes Line
	4200 3600 4200 4100
Wire Notes Line
	4200 4100 3300 4100
Wire Notes Line
	3300 4100 3300 3600
Wire Wire Line
	4800 3000 4800 3550
Wire Wire Line
	4500 3000 4800 3000
Wire Wire Line
	4500 3200 4800 3200
Connection ~ 4800 3200
$Comp
L CONN_14X2 P?
U 1 1 5158ED69
P 3700 6000
F 0 "P?" H 3700 6750 60  0000 C CNN
F 1 "CONN_14X2" V 3700 6050 50  0000 C CNN
F 2 "~" H 3700 6050 60  0000 C CNN
F 3 "~" H 3700 6050 60  0000 C CNN
	1    3700 6000
	1    0    0    -1  
$EndComp
Text GLabel 4800 4700 2    60   Output ~ 0
VIO_1V8
Text GLabel 2600 4700 0    60   Output ~ 0
DCIN_JACK
Text GLabel 4800 4900 2    60   BiDi ~ 0
GPMC_AD7/SDMMC2_DAT7
Text GLabel 2600 4900 0    60   BiDi ~ 0
MCSPI1_CS3/GPIO_140
Text GLabel 4800 5100 2    60   BiDi ~ 0
GPMC_AD6/SDMMC2_DAT6
Text GLabel 2600 5100 0    60   BiDi ~ 0
UART4_TX/GPIO_156
Text GLabel 4800 5300 2    60   BiDi ~ 0
GPMC_AD5/SDMMC2_DAT5
Text GLabel 2600 5300 0    60   BiDi ~ 0
UART4_RX/GPIO_155
Text GLabel 4800 5500 2    60   BiDi ~ 0
GPMC_AD4/SDMMC2_DAT4
Text GLabel 2600 5500 0    60   BiDi ~ 0
MCSPI1_CS1/GPIO_138
Text GLabel 4800 5700 2    60   BiDi ~ 0
GPMC_AD3/SDMMC2_DAT3
Text GLabel 2600 5700 0    60   BiDi ~ 0
MCSPI1_SIMO/GPIO_136
Text GLabel 4800 5900 2    60   BiDi ~ 0
GPMC_AD2/SDMMC2_DAT2
Text GLabel 2600 5900 0    60   BiDi ~ 0
MCSPI1_CS2/GPIO_139
Text GLabel 4800 6100 2    60   BiDi ~ 0
GPMC_AD1/SDMMC2_DAT1
Text GLabel 2600 6100 0    60   BiDi ~ 0
MCSPI1_CS0/GPIO_137
Text GLabel 4800 6300 2    60   BiDi ~ 0
GPMC_AD0/SDMMC2_DAT0
Text GLabel 2600 6300 0    60   BiDi ~ 0
MCSPI1_SOMI/GPIO_135
Text GLabel 4800 6500 2    60   BiDi ~ 0
GPMC_NWE/SDMMC2_CMD
Text GLabel 2600 6500 0    60   BiDi ~ 0
MCSPI1_CLK/GPIO_134
Text GLabel 4800 6700 2    60   BiDi ~ 0
GPMC_NOE/SDMMC2_CLK
Text GLabel 2600 6700 0    60   BiDi ~ 0
GPMC_AD15/GPIO_39
Text GLabel 4800 6900 2    60   BiDi ~ 0
I2C4_SDA/GPIO_133
Text GLabel 2600 6900 0    60   BiDi ~ 0
I2C4_SCL/GPIO_132
Text GLabel 4800 7100 2    60   BiDi ~ 0
REGEN1
Text GLabel 2600 7100 0    60   BiDi ~ 0
SYS_NRESPWRON
Text GLabel 4800 7300 2    60   BiDi ~ 0
DGND
Text GLabel 2600 7300 0    60   BiDi ~ 0
DGND
Text GLabel 5350 2400 2    60   Input ~ 0
ACCEL/INT1
Wire Wire Line
	4500 2400 5350 2400
Text GLabel 5350 2200 2    60   Input ~ 0
GYRO/DRDY
Wire Wire Line
	4500 2200 5350 2200
Wire Wire Line
	2600 4700 3300 4700
Wire Wire Line
	2600 4900 3300 4900
Wire Wire Line
	2600 5100 3300 5100
Wire Wire Line
	2600 5300 3300 5300
Wire Wire Line
	2600 5500 3300 5500
Wire Wire Line
	2600 5700 3300 5700
Wire Wire Line
	2600 5900 3300 5900
Wire Wire Line
	2600 6100 3300 6100
Wire Wire Line
	2600 6300 3300 6300
Wire Wire Line
	2600 6500 3300 6500
Wire Wire Line
	2600 6700 3300 6700
Wire Wire Line
	2600 6900 3300 6900
Wire Wire Line
	2600 7100 3300 7100
Wire Wire Line
	2600 7300 3300 7300
Wire Wire Line
	4100 7300 4800 7300
Wire Wire Line
	4100 7100 4800 7100
Wire Wire Line
	4100 6900 4800 6900
Wire Wire Line
	4100 6700 4800 6700
Wire Wire Line
	4100 6500 4800 6500
Wire Wire Line
	4100 6300 4800 6300
Wire Wire Line
	4100 6100 4800 6100
Wire Wire Line
	4100 5900 4800 5900
Wire Wire Line
	4100 5700 4800 5700
Wire Wire Line
	4100 5500 4800 5500
Wire Wire Line
	4100 5300 4800 5300
Wire Wire Line
	4100 5100 4800 5100
Wire Wire Line
	4100 4900 4800 4900
Wire Wire Line
	4100 4700 4800 4700
Text GLabel 2600 2400 0    60   BiDi ~ 0
GPMC_AD15/GPIO_39
Text GLabel 2600 2200 0    60   BiDi ~ 0
MCSPI1_SOMI/GPIO_135
Wire Wire Line
	3000 2200 2600 2200
Wire Wire Line
	3000 2400 2600 2400
$Comp
L GND #PWR?
U 1 1 5190348C
P 2650 3550
F 0 "#PWR?" H 2650 3550 30  0001 C CNN
F 1 "GND" H 2650 3480 30  0001 C CNN
F 2 "" H 2650 3550 60  0000 C CNN
F 3 "" H 2650 3550 60  0000 C CNN
	1    2650 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 3550 2650 3000
Wire Wire Line
	2650 3000 3000 3000
Wire Wire Line
	3000 3200 2650 3200
Connection ~ 2650 3200
$EndSCHEMATC