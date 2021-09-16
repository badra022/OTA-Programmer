# simple ARM Makefile
#
# written by Ahmed Badra (http://github.com/badra022)
# released to the public domain

# Makefile
#
# targets:
#   all:    compiles the source code
#   clean:  removes all .hex, .elf, and .o files in the source code and library directories

# parameters (change this stuff accordingly)
# project name
PRJ = main
# avr mcu
CPU = cortex-m3
# program source files (not including external libraries)
SRC = $(PRJ).c
# where to look for external libraries (consisting of .c files and .h files)
# e.g. PERIPH = GPIO RCC ...
PERIPH = LIB RCC GPIO SYSTICK DELAY LEDS BUTTONS TFT IR SPI NVIC EXTI AFIO
# output folder name
BIN = bin
# object files folder name
OBJ_FOLDER = obj
# path to used drivers (which is Not standard)
USED_PERIPH = ../STM32F10x_imtPeriph

EXT := $(foreach dir, $(PERIPH), $(USED_PERIPH)/$(dir))
# # we will add also the path to linker file .ld and startup assembly code
# EXT += ../STM32F10x_myPeriph/

# add path to standard peripherals lib
EXT += .././STM32F10x_StdPeriph_Lib_V3.5.0
EXT += .././STM32F10x_StdPeriph_Lib_V3.5.0/CMSIS/Device/ST/STM32F10x/Include
EXT += .././STM32F10x_StdPeriph_Lib_V3.5.0/CMSIS/Include
EXT += .././STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/inc
EXT += .././STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/src

# include path
INCLUDE := $(foreach dir, $(EXT), -I$(dir))
# c flags
CFLAGS  = -g -O2 -Wall $(INCLUDE)
CFLAGS += -T../STM32F10x_StdPeriph_Lib_V3.5.0/stm32_flash.ld
CFLAGS += -mlittle-endian -mthumb -mcpu=$(CPU) -mthumb-interwork -std=gnu11
CFLAGS += -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER
CFLAGS += -Wl,--gc-sections

# executables
OBJCOPY 	= .././ArmGCC/bin/arm-none-eabi-objcopy.exe
CC			= .././ArmGCC/bin/arm-none-eabi-gcc.exe
SIZE		=.././ArmGCC/bin/arm-none-eabi-size

# generate list of objects
CFILES    = $(filter %.c, $(SRC))
EXTC     := $(foreach dir, $(EXT), $(wildcard $(dir)/*.c))
EXTS 	 := $(foreach dir, $(EXT), $(wildcard $(dir)/*.s))
OBJ       = $(CFILES:.c=.o) $(EXTC:.c=.o) $(EXTS:.s=.o)

# user targets
# compile all files
all: clean build $(PRJ).hex
	@$(SIZE) $(BIN)/$(PRJ).elf

build:
	@ mkdir -p $(BIN) $(OBJ_FOLDER)

# remove compiled files
clean:
	@ echo "cleaning binaries..."
	@ rm -rf $(BIN) $(OBJ_FOLDER) $(PRJ).o
	@ $(foreach dir, $(EXT), rm -f $(wildcard $(dir)/*.o))

.c.o:
	@ echo "assembling and compiling..."
	@ $(CC) $(CFLAGS) -c $< -o $@

.s.o:
	@ echo "assembling and compiling startup file..."
	@ $(CC) $(CFLAGS) -c $< -o $@

# elf file
$(BIN)/$(PRJ).elf: $(OBJ)
	@ $(CC) $(CFLAGS) $^ -o $(BIN)/$(PRJ).elf 

# hex file
$(PRJ).hex: $(BIN)/$(PRJ).elf
	@ echo "linking..."
	@ $(OBJCOPY) -O ihex $(BIN)/$(PRJ).elf ./$(BIN)/$(PRJ).hex
	@ $(OBJCOPY) -O binary $(BIN)/$(PRJ).elf ./$(BIN)/$(PRJ).bin
