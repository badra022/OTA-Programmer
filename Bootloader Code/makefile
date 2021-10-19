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
OUTPUT_NAME = bootloader
# mcu processor
CPU = cortex-m3
# program source files (not including external libraries)
SRC = ./src
INCLUDES = ./includes
# path to used drivers (which is Not standard)
USED_PERIPH = ./libs
# where to look for external libraries (consisting of .c files and .h files)
# e.g. PERIPH = GPIO RCC ...
PERIPH = RCC GPIO SYSTICK DELAY NVIC USART WIFI FPEC
# output folder name
BIN_NAME = bin



EXT := $(foreach dir, $(PERIPH), $(USED_PERIPH)/$(dir))	$(INCLUDES)

# include path
INCLUDE := $(foreach dir, $(EXT), -I$(dir))

# c flags
CFLAGS  = -g -O2 -Wall $(INCLUDE)
CFLAGS += -T$(INCLUDES)/stm32_flash.ld
CFLAGS += -mlittle-endian -mthumb -mcpu=$(CPU) -mthumb-interwork -std=gnu11
CFLAGS += -DSTM32F10X_MD 
CFLAGS += -Wl,--gc-sections

# executables
OBJCOPY 	= C:\ArmGCC\bin/arm-none-eabi-objcopy.exe
CC			= C:\ArmGCC\bin/arm-none-eabi-gcc.exe
SIZE		= C:\ArmGCC\bin/arm-none-eabi-size

# generate list of objects
CFILES    = $(foreach dir, $(SRC), $(wildcard $(dir)/*.c))
SFILES 	 := $(foreach dir, $(SRC), $(wildcard $(dir)/*.s)) $(foreach dir, $(EXT), $(wildcard $(dir)/*.s))
EXTC     := $(foreach dir, $(EXT), $(wildcard $(dir)/*.c))
OBJ       = $(CFILES:.c=.o) $(EXTC:.c=.o) $(SFILES:.s=.o)

# user targets
# compile all files
all: clean build $(OUTPUT_NAME).hex
	@$(SIZE) $(BIN_NAME)/$(OUTPUT_NAME).elf

build:
	@ mkdir -p $(BIN_NAME)

# remove compiled files
clean:
	@ echo "cleaning binaries..."
	@ rm -rf $(BIN_NAME) $(OUTPUT_NAME).o
	@ $(foreach dir, $(EXT), rm -f $(wildcard $(dir)/*.o))

.c.o:
	@ echo "assembling and compiling..."
	@ $(CC) $(CFLAGS) -c $< -o $@

.s.o:
	@ echo "assembling and compiling startup file..."
	@ $(CC) $(CFLAGS) -c $< -o $@

# elf file
$(BIN_NAME)/$(OUTPUT_NAME).elf: $(OBJ)
	@ $(CC) $(CFLAGS) $^ -o $(BIN_NAME)/$(OUTPUT_NAME).elf 

# hex file
$(OUTPUT_NAME).hex: $(BIN_NAME)/$(OUTPUT_NAME).elf
	@ echo "linking..."
	@ $(OBJCOPY) -O ihex $(BIN_NAME)/$(OUTPUT_NAME).elf ./$(BIN_NAME)/$(OUTPUT_NAME).hex
	@ $(OBJCOPY) -O binary $(BIN_NAME)/$(OUTPUT_NAME).elf ./$(BIN_NAME)/$(OUTPUT_NAME).bin
