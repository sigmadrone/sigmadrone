################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/stm32f429i_discovery.c \
../bsp/stm32f429i_discovery_eeprom.c \
../bsp/stm32f429i_discovery_io.c \
../bsp/stm32f429i_discovery_lcd.c \
../bsp/stm32f429i_discovery_sdram.c \
../bsp/stm32f429i_discovery_ts.c 

OBJS += \
./bsp/stm32f429i_discovery.o \
./bsp/stm32f429i_discovery_eeprom.o \
./bsp/stm32f429i_discovery_io.o \
./bsp/stm32f429i_discovery_lcd.o \
./bsp/stm32f429i_discovery_sdram.o \
./bsp/stm32f429i_discovery_ts.o 

C_DEPS += \
./bsp/stm32f429i_discovery.d \
./bsp/stm32f429i_discovery_eeprom.d \
./bsp/stm32f429i_discovery_io.d \
./bsp/stm32f429i_discovery_lcd.d \
./bsp/stm32f429i_discovery_sdram.d \
./bsp/stm32f429i_discovery_ts.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/%.o: ../bsp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DSTM32F429xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I/home/mstoilov/workspace-yocto/sigmadrone/source/cortex-m4/sdfirmware/include -I/home/mstoilov/workspace-yocto/sigmadrone/source/cortex-m4/sdfirmware/freertos/include -I/home/mstoilov/workspace-yocto/sigmadrone/source/cortex-m4/sdfirmware/system/include -I/home/mstoilov/workspace-yocto/sigmadrone/source/cortex-m4/sdfirmware/system/include/cmsis -I/home/mstoilov/workspace-yocto/sigmadrone/source/cortex-m4/sdfirmware/system/include/stm32f4-hal -I/home/mstoilov/workspace-yocto/sigmadrone/source/include -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


