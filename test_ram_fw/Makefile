TARGET = main

# Default target chip.
MCU ?= STM32H7B0VB

# Define target chip information.
ifeq ($(MCU), STM32H7B0VB)
	MCU_FILES = STM32H7B0VB
	ST_MCU_DEF = STM32H7B0xx
	MCU_CLASS = H7
endif

#
# Generic STM32 makefile:
#
ifeq ($(MCU_CLASS), F0)
	MCU_SPEC = cortex-m0
else ifeq ($(MCU_CLASS), $(filter $(MCU_CLASS), L0 G0))
	MCU_SPEC = cortex-m0plus
else ifeq ($(MCU_CLASS), $(filter $(MCU_CLASS), F1 L1))
	MCU_SPEC = cortex-m3
else ifeq ($(MCU_CLASS), $(filter $(MCU_CLASS), L4 G4 WB))
	MCU_SPEC = cortex-m4
else ifeq ($(MCU_CLASS), $(filter $(MCU_CLASS), F7 H7))
	MCU_SPEC = cortex-m7
endif

# Toolchain definitions (ARM bare metal defaults)
TOOLCHAIN = /usr
CC = $(TOOLCHAIN)/bin/arm-none-eabi-gcc
AS = $(TOOLCHAIN)/bin/arm-none-eabi-as
LD = $(TOOLCHAIN)/bin/arm-none-eabi-ld
OC = $(TOOLCHAIN)/bin/arm-none-eabi-objcopy
OD = $(TOOLCHAIN)/bin/arm-none-eabi-objdump
OS = $(TOOLCHAIN)/bin/arm-none-eabi-size

# Assembly directives.
ASFLAGS += -c
ASFLAGS += -O0
ASFLAGS += -mcpu=$(MCU_SPEC)
ASFLAGS += -mthumb
ASFLAGS += -Wall
# (Set error messages to appear on a single line.)
ASFLAGS += -fmessage-length=0
ASFLAGS += -DVVC_$(MCU_CLASS)

# C compilation directives
CFLAGS += -mcpu=$(MCU_SPEC)
CFLAGS += -mthumb
ifeq ($(MCU_CLASS), $(filter $(MCU_CLASS), F0 F1 L0 L1 G0))
	CFLAGS += -msoft-float
	CFLAGS += -mfloat-abi=soft
else ifeq ($(MCU_CLASS), F7)
	CFLAGS += -mhard-float
	CFLAGS += -mfloat-abi=hard
	CFLAGS += -mfpu=fpv5-sp-d16
else ifeq ($(MCU_CLASS), H7)
	CFLAGS += -mhard-float
	CFLAGS += -mfloat-abi=hard
	CFLAGS += -mfpu=fpv5-d16
else
	CFLAGS += -mhard-float
	CFLAGS += -mfloat-abi=hard
	CFLAGS += -mfpu=fpv4-sp-d16
endif
CFLAGS += -Wall
CFLAGS += -g
CFLAGS += -fmessage-length=0
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += --specs=nosys.specs
CFLAGS += -D$(ST_MCU_DEF)
CFLAGS += -D$(MCU_FILES)
CFLAGS += -DVVC_$(MCU_CLASS)

# Linker directives.
LSCRIPT = ./../common/ld/$(MCU_FILES).ld
LFLAGS += -mcpu=$(MCU_SPEC)
LFLAGS += -mthumb
ifeq ($(MCU_CLASS), $(filter $(MCU_CLASS), F0 F1 L0 L1 G0))
	LFLAGS += -msoft-float
	LFLAGS += -mfloat-abi=soft
else ifeq ($(MCU_CLASS), F7)
	LFLAGS += -mhard-float
	LFLAGS += -mfloat-abi=hard
	LFLAGS += -mfpu=fpv5-sp-d16
else ifeq ($(MCU_CLASS), H7)
	LFLAGS += -mhard-float
	LFLAGS += -mfloat-abi=hard
	LFLAGS += -mfpu=fpv5-d16
else
	LFLAGS += -mhard-float
	LFLAGS += -mfloat-abi=hard
	LFLAGS += -mfpu=fpv4-sp-d16
endif
LFLAGS += -Wall
LFLAGS += --specs=nosys.specs
LFLAGS += -lgcc
LFLAGS += -Wl,--gc-sections
LFLAGS += -Wl,-L./../common/ld
LFLAGS += -T$(LSCRIPT)

C_SRC    =  ./src/main.c

INCLUDE  =  -I./
INCLUDE  += -I./../common/device_headers

OBJS  = $(AS_SRC:.S=.o)
OBJS += $(C_SRC:.c=.o)

.PHONY: all
all: $(TARGET).bin

%.o: %.S
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f ./../common/$(ST_MCU_DEF)_vt.S
	rm -f $(TARGET).elf
	rm -f $(TARGET).bin
