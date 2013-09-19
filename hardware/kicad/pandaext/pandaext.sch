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
Sheet 1 8
Title "Pandaboard extension"
Date "18 jun 2013"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 2950 900  0    118  ~ 24
Push On/Off Control
$Sheet
S 1100 1150 1300 1000
U 514CC7D4
F0 "Battery Charger" 50
F1 "batterycharger.sch" 50
F2 "Vout" O R 2400 1300 60 
$EndSheet
$Sheet
S 2850 1150 1200 1000
U 514D55A0
F0 "On/Off Control" 50
F1 "onoffcontrol.sch" 50
F2 "Vout" I L 2850 1300 60 
F3 "ON_Vout" O R 4050 1300 60 
$EndSheet
$Sheet
S 4900 1150 1200 1000
U 514E8F29
F0 "Voltage Regulators" 50
F1 "voltageregulators.sch" 50
F2 "ON_Vout" I L 4900 1300 60 
$EndSheet
Wire Wire Line
	2400 1300 2850 1300
Wire Wire Line
	4050 1300 4900 1300
$Sheet
S 2900 2900 1300 1000
U 5157500B
F0 "I2C Sensors" 50
F1 "i2c-sensors.sch" 50
$EndSheet
$Sheet
S 1100 2900 1300 1050
U 5157503F
F0 "I2C Bus" 50
F1 "i2c-bus.sch" 50
$EndSheet
$Sheet
S 1100 4300 1300 900 
U 5158E1D5
F0 "External Header - GPIO Translation" 50
F1 "header-gpio.sch" 50
$EndSheet
$Sheet
S 4900 2900 1300 1000
U 519039D0
F0 "Servo Controller" 50
F1 "servo-control.sch" 50
$EndSheet
$EndSCHEMATC
