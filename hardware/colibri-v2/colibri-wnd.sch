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
Sheet 1 14
Title "Colibri WND"
Date ""
Rev "A1"
Comp "sygmadrone.org"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 7000 1100 4250 8400
U 557677C0
F0 "cortex" 60
F1 "cortex.sch" 60
$EndSheet
$Sheet
S 3900 1150 1850 800 
U 5576830B
F0 "JTAG/SWD" 60
F1 "swd_jtag.sch" 60
F2 "TRST" O R 5750 1250 60 
F3 "TDO/SWO" I R 5750 1350 60 
F4 "TCK/SWCLK" O R 5750 1450 60 
F5 "TMS/SWDIO" B R 5750 1550 60 
F6 "TDI" B R 5750 1650 60 
F7 "RESET#" O R 5750 1750 60 
F8 "JTAG_POWER_ON" O R 5750 1850 60 
$EndSheet
$Sheet
S 3900 2250 1850 1200
U 5586441E
F0 "Leds/Buttons" 60
F1 "leds_buttons.sch" 60
F2 "BUTTON_0" O R 5750 2350 60 
F3 "BUTTON_1" O R 5750 2450 60 
F4 "BUTTON_2" O R 5750 2550 60 
F5 "BUTTON_3" O R 5750 2650 60 
F6 "BUTTON_4" O R 5750 2750 60 
F7 "RESET#" O R 5750 2850 60 
F8 "LED1" I R 5750 3050 60 
F9 "LED3" I R 5750 3250 60 
F10 "LED2" I R 5750 3150 60 
F11 "LED4" I R 5750 3350 60 
F12 "BOOT0" O R 5750 2950 60 
$EndSheet
$Sheet
S 3900 4550 1850 1100
U 55865745
F0 "MEMS" 60
F1 "mems.sch" 60
F2 "SPI5_SCK" I R 5750 4850 60 
F3 "SPI5_MOSI" I R 5750 4950 60 
F4 "SPI5_MISO" O R 5750 5050 60 
F5 "MEMS_GYR_INT2" O R 5750 5150 60 
F6 "MEMS_GYR_INT1" O R 5750 5250 60 
F7 "MEMS_GYR_NCS" I R 5750 4750 60 
F8 "MEMS_ACC_INT2" O R 5750 5350 60 
F9 "MEMS_ACC_INT1" O R 5750 5450 60 
F10 "MEMS_ACC_NCS" I R 5750 4650 60 
F11 "V_SENS1" I R 5750 5550 60 
$EndSheet
$Sheet
S 3900 6500 1850 950 
U 55895050
F0 "STM32F4 SDRAM" 60
F1 "stm32f4_sdram.sch" 60
F2 "FMC_B[0..1]" B R 5750 6600 60 
F3 "FMC_SDNE1" I R 5750 6850 60 
F4 "FMC_SDNWE" I R 5750 7250 60 
F5 "FMC_SDNRAS" I R 5750 7050 60 
F6 "FMC_SDNCAS" I R 5750 7150 60 
F7 "FMC_SDCLK" I R 5750 7350 60 
F8 "FMC_SDCKE1" I R 5750 6950 60 
F9 "FMC_NBL[0..1]" B R 5750 6700 60 
$EndSheet
$Sheet
S 12850 2050 1850 800 
U 558B2A96
F0 "USB_OTG_FS" 60
F1 "usb_otg_fs.sch" 60
F2 "VBUS_FS" B R 14700 2400 60 
F3 "OTG_FS_DM" B R 14700 2500 60 
F4 "OTG_FS_DP" B R 14700 2600 60 
F5 "OTG_FS_ID" B R 14700 2700 60 
F6 "OTG_FS_OC" O R 14700 2300 60 
F7 "OTG_FS_PSO" I R 14700 2200 60 
$EndSheet
$Sheet
S 12850 3200 1850 600 
U 558F3436
F0 "Cortex FTDI Interface" 60
F1 "c_ftdi_interface.sch" 60
F2 "C_FTDI_RTS" I R 14700 3500 60 
F3 "C_FTDI_CTS" O R 14700 3600 60 
F4 "C_FTDI_RI" O R 14700 3700 60 
F5 "C_FTDI_TXD" O R 14700 3300 60 
F6 "C_FTDI_RXD" I R 14700 3400 60 
$EndSheet
$Sheet
S 3900 5900 1850 400 
U 558FDA75
F0 "Pressure Sensor" 60
F1 "pressure_sensor.sch" 60
F2 "PS_I2C_SDA" B R 5750 6000 60 
F3 "PS_I2C_SCL" I R 5750 6100 60 
F4 "V_SENS1" I R 5750 6200 60 
$EndSheet
$Sheet
S 12850 1150 2100 600 
U 5596276C
F0 "Power Control" 60
F1 "powerctrl.sch" 60
$EndSheet
$Sheet
S 3900 3700 1850 550 
U 55FF210B
F0 "Sensors Power Control" 60
F1 "sensors_pwr_ctrl.sch" 60
F2 "V_SENS_CTRL" I R 5750 3900 60 
F3 "V_SENS1" O R 5750 4000 60 
$EndSheet
$Sheet
S 3900 7800 1850 1700
U 5609F831
F0 "Wandboard Connector" 60
F1 "wb_connector.sch" 60
$EndSheet
Wire Wire Line
	5750 4000 6150 4000
Wire Wire Line
	6150 4000 6150 6200
Wire Wire Line
	6150 6200 5750 6200
Wire Wire Line
	5750 5550 6150 5550
Connection ~ 6150 5550
Text Notes 1200 950  0    60   ~ 0
PROBA
$Sheet
S 12850 4150 2050 1000
U 56138561
F0 "Test Sheet" 60
F1 "test.sch" 60
$EndSheet
$EndSCHEMATC
