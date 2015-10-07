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
$Descr A3 16535 11693
encoding utf-8
Sheet 11 16
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 14000 4800 1600 900 
U 560A06D4
F0 "Wandboard FTDI Interface" 60
F1 "wb_ftdi_interface.sch" 60
F2 "WB_FTDI_RXD" O L 14000 5000 60 
F3 "WB_FTDI_TXD" I L 14000 4900 60 
F4 "WB_FTDI_CTS" O L 14000 5100 60 
F5 "WB_FTDI_RTS" I L 14000 5200 60 
F6 "WB_FTDI_RI" O L 14000 5600 60 
F7 "WB_FTDI_DSR" O L 14000 5400 60 
F8 "WB_FTDI_DTR" I L 14000 5500 60 
F9 "WB_FTDI_DCD" I L 14000 5300 60 
$EndSheet
$Comp
L AS0B821-S78B-7H CN?
U 1 1 560F080E
P 4300 5600
F 0 "CN?" H 4600 9950 60  0000 C CNN
F 1 "AS0B821-S78B-7H" H 4350 1250 60  0000 C CNN
F 2 "" H 4300 5850 60  0000 C CNN
F 3 "" H 4300 5850 60  0000 C CNN
	1    4300 5600
	1    0    0    -1  
$EndComp
$Comp
L AS0B821-S78B-7H CN?
U 2 1 560F088D
P 9150 5600
F 0 "CN?" H 9450 9950 60  0000 C CNN
F 1 "AS0B821-S78B-7H" H 9200 1250 60  0000 C CNN
F 2 "" H 9150 5850 60  0000 C CNN
F 3 "" H 9150 5850 60  0000 C CNN
	2    9150 5600
	1    0    0    -1  
$EndComp
$Sheet
S 14050 1300 1550 1000
U 5614B323
F0 "Wandboard USB OTG" 60
F1 "wnd_usb_otg.sch" 60
F2 "VBUS_FS" B L 14050 1450 60 
F3 "OTG_FS_DM" B L 14050 1550 60 
F4 "OTG_FS_DP" B L 14050 1650 60 
F5 "OTG_FS_ID" B L 14050 1750 60 
F6 "OTG_FS_OC" O L 14050 1850 60 
F7 "OTG_FS_PSO" I L 14050 1950 60 
$EndSheet
$Sheet
S 14050 2650 1550 1000
U 56152E00
F0 "Wandboard SD Card" 60
F1 "wnd_micro_sd.sch" 60
$EndSheet
$EndSCHEMATC
