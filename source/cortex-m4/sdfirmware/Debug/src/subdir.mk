################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/attitudetracker.cpp \
../src/circbuffer.cpp \
../src/digitalin.cpp \
../src/digitalout.cpp \
../src/gpiopin.cpp \
../src/hwtimer.cpp \
../src/l3gd20.cpp \
../src/lsm303d.cpp \
../src/main.cpp \
../src/pwmdecoder.cpp \
../src/pwmencoder.cpp \
../src/rtoshooks.cpp \
../src/spimaster.cpp \
../src/spislave.cpp \
../src/timestamp.cpp \
../src/uart.cpp 

C_SRCS += \
../src/_initialize_hardware.c \
../src/_write.c 

OBJS += \
./src/_initialize_hardware.o \
./src/_write.o \
./src/attitudetracker.o \
./src/circbuffer.o \
./src/digitalin.o \
./src/digitalout.o \
./src/gpiopin.o \
./src/hwtimer.o \
./src/l3gd20.o \
./src/lsm303d.o \
./src/main.o \
./src/pwmdecoder.o \
./src/pwmencoder.o \
./src/rtoshooks.o \
./src/spimaster.o \
./src/spislave.o \
./src/timestamp.o \
./src/uart.o 

C_DEPS += \
./src/_initialize_hardware.d \
./src/_write.d 

CPP_DEPS += \
./src/attitudetracker.d \
./src/circbuffer.d \
./src/digitalin.d \
./src/digitalout.d \
./src/gpiopin.d \
./src/hwtimer.d \
./src/l3gd20.d \
./src/lsm303d.d \
./src/main.d \
./src/pwmdecoder.d \
./src/pwmencoder.d \
./src/rtoshooks.d \
./src/spimaster.d \
./src/spislave.d \
./src/timestamp.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DSTM32F429xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../rexjson" -I"../../../include" -I"../freertos/include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DSTM32F429xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../rexjson" -I"../bsp-discovery" -I"../../../include" -I"../freertos/include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu++11 -fabi-version=0 -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


