
PROJECT = test-printf

all : $(PROJECT)

$(PROJECT) : $(OBJECTS)


CIMSIS_SRC_C = $(wildcard system/cmsis/*.c)
CORTEXM_SRC_C = $(wildcard system/cortexm/*.c)
DIAG_SRC_C = $(wildcard system/diag/*.c)
HAL_SRC_C = $(wildcard system/stm32f4-hal/*.c)
NEWLIB_SRC_C = $(wildcard system/newlib/*.c)
NEWLIB_SRC_CXX = $(wildcard system/newlib/*.cxx)
FREERTOS_SRC_C = $(wildcard freertos/*.c)
APP_SRC_C = $(wildcard application/*.c)
APP_SRC_CPP = $(wildcard application/*.cpp)
BSP_SRC_C = $(wildcard BSP/STM32F429I-Discovery/*.c)
FONTS_SRC_C = $(wildcard BSP/Fonts/*.c)
L3GD20_SRC_C = $(wildcard BSP/Components/l3gd20/*.c)
L3GD20DRV_SRC_C = $(wildcard BSP/Components/l3gd20_driver/*.c)
STMPE811_SRC_C = $(wildcard BSP/Components/stmpe811/*.c)
ILI9341_SRC_C = $(wildcard BSP/Components/ili9341/*.c)


CIMSIS_OBJ_C   := $(CIMSIS_SRC_C:.c=.o)
CORTEXM_OBJ_C  := $(CORTEXM_SRC_C:.c=.o)
DIAG_OBJ_C     := $(DIAG_SRC_C:.c=.o)
HAL_OBJ_C      := $(HAL_SRC_C:.c=.o)
APP_OBJ_C      := $(APP_SRC_C:.c=.o)
APP_OBJ_CPP    := $(APP_SRC_CPP:.cpp=.o)
NEWLIB_OBJ_C   := $(NEWLIB_SRC_C:.c=.o)
FREERTOS_OBJ_C := $(FREERTOS_SRC_C:.c=.o)
NEWLIB_OBJ_CXX := $(NEWLIB_SRC_CXX:.cxx=.o)
BSP_OBJ_C      := $(BSP_SRC_C:.c=.o)
L3GD20_OBJ_C   := $(L3GD20_SRC_C:.c=.o)
L3GD20DRV_OBJ_C:= $(L3GD20DRV_SRC_C:.c=.o)
STMPE811_OBJ_C := $(STMPE811_SRC_C:.c=.o)
ILI9341_OBJ_C  := $(ILI9341_SRC_C:.c=.o)
FONTS_OBJ_C    := $(FONTS_SRC_C:.c=.o)


OBJECTS        :=   $(CIMSIS_OBJ_C) \
					$(CORTEXM_OBJ_C) \
					$(DIAG_OBJ_C) \
					$(HAL_OBJ_C) \
					$(NEWLIB_OBJ_C) \
					$(NEWLIB_OBJ_CXX) \
					$(APP_OBJ_C) \
					$(APP_OBJ_CPP) \
					$(FREERTOS_OBJ_C) \
					$(BSP_OBJ_C) \
					$(L3GD20_OBJ_C) \
					$(L3GD20DRV_OBJ_C) \
					$(STMPE811_OBJ_C) \
					$(ILI9341_OBJ_C)
					

					
DEPS           := $(OBJECTS:.o=.d)

INCLUDES = \
	-I . \
	-I include \
	-I include/arm \
	-I include/cmsis \
	-I include/diag \
	-I include/stm32f4-hal \
	-I freertos \
	-I freertos/include \
	-I application \
	-I BSP/STM32F429I-Discovery \
	-I BSP/Components \
	-I BSP/Components/Common \
	-I BSP/Components/l3gd20 \
	-I BSP/Components/l3gd20_driver \


############################################################################### 
PREFIX = $(GCC_BIN)arm-none-eabi-
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
CPP = $(PREFIX)g++
AR = $(PREFIX)ar
AS = $(PREFIX)as
LD = $(PREFIX)gcc
SIZE = $(PREFIX)size
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump

CPU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=$(FLOAT_ABI)
CFLAGS = $(CPU) -c -g -O0 -Wall
CFLAGS += -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections
CFLAGS += -D__FPU_PRESENT -D__FPU_USED
CFLAGS += -DDEBUG -DTRACE 
# CFLAGS += -DOS_USE_TRACE_ITM
CFLAGS += -DOS_USE_TRACE_SEMIHOSTING_DEBUG
CFLAGS += -DSTM32F429xx -DHSE_VALUE=8000000

CC_FLAGS = $(CFLAGS)
CC_FLAGS += -std=gnu11 -MMD -MP

CXX_FLAGS = $(CFLAGS)
CXX_FLAGS += -std=gnu++11 -MMD -MP -fabi-version=0

LD_FLAGS = $(CPU) -Wl,--gc-sections #--specs=nano.specs -u _printf_float -u _scanf_float
LD_FLAGS += -Wl,-Map=$(PROJECT).map,--cref -Wl,--defsym=__real_main=main
LIBRARIES = -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys

ifeq ($(HARDFP),1)
	FLOAT_ABI = hard
else
	FLOAT_ABI = softfp
endif

ifeq ($(DEBUG), 1)
  CC_FLAGS += -DDEBUG -O0
else
  CC_FLAGS += -DNDEBUG -Os
endif

all: $(PROJECT).bin $(PROJECT).hex size

clean:
	rm -f $(PROJECT).bin $(PROJECT).elf $(PROJECT).hex $(PROJECT).map $(PROJECT).lst $(OBJECTS) $(DEPS)

.s.o:
	$(AS) $(CPU) -o $@ $<

.c.o:
	$(CC)  $(CC_FLAGS) $(INCLUDES) -o $@ $<

.cpp.o:
	$(CPP) $(CXX_FLAGS) $(INCLUDES) -o $@ $<


$(PROJECT).elf: $(OBJECTS)
	$(LD) $(LD_FLAGS) -T ldscripts/mem.ld -T ldscripts/sections.ld  -nostartfiles -Wl,--gc-sections -Wl,-Map=$(PROJECT).map $(LIBRARY_PATHS) -o $@ $^ $(LIBRARIES)

$(PROJECT).bin: $(PROJECT).elf
	@$(OBJCOPY) -O binary $< $@

$(PROJECT).hex: $(PROJECT).elf
	@$(OBJCOPY) -O ihex $< $@

$(PROJECT).lst: $(PROJECT).elf
	@$(OBJDUMP) -Sdh $< > $@

lst: $(PROJECT).lst

size:
	$(SIZE) $(PROJECT).elf

-include $(DEPS)

