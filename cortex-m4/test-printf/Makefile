
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


CIMSIS_OBJ_C   := $(CIMSIS_SRC_C:.c=.o)
CORTEXM_OBJ_C  := $(CORTEXM_SRC_C:.c=.o)
DIAG_OBJ_C     := $(DIAG_SRC_C:.c=.o)
HAL_OBJ_C      := $(HAL_SRC_C:.c=.o)
APP_OBJ_C      := $(APP_SRC_C:.c=.o)
APP_OBJ_CPP    := $(APP_SRC_CPP:.cpp=.o)
NEWLIB_OBJ_C   := $(NEWLIB_SRC_C:.c=.o)
FREERTOS_OBJ_C := $(FREERTOS_SRC_C:.c=.o)
NEWLIB_OBJ_CXX := $(NEWLIB_SRC_CXX:.cxx=.o)


OBJECTS        :=   $(CIMSIS_OBJ_C) \
					$(CORTEXM_OBJ_C) \
					$(DIAG_OBJ_C) \
					$(HAL_OBJ_C) \
					$(NEWLIB_OBJ_C) \
					$(NEWLIB_OBJ_CXX) \
					$(APP_OBJ_C) \
					$(APP_OBJ_CPP) \
					$(FREERTOS_OBJ_C) \
					
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
CFLAGS += -DDEBUG -DTRACE 
CFLAGS += -DOS_USE_TRACE_ITM
#CFLAGS += -DOS_USE_TRACE_SEMIHOSTING_DEBUG
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

