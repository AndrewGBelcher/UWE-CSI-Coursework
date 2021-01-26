

# Tool path

TOOLROOT=/usr/bin

WORKDIR=/home/seed/Downloads/med
LIBROOT=$(WORKDIR)/STM32F10x_StdPeriph_Lib_V3.5.0
NEWLIB=$(WORKDIR)/newlib/newlib-arm-none-eabi/newlib
NEWLIBINCLUDE=$(WORKDIR)/newlib/src/newlib/libc/include/


# Tools

CC=$(TOOLROOT)/arm-none-eabi-gcc
LD=$(TOOLROOT)/arm-none-eabi-gcc
AR=$(TOOLROOT)/arm-none-eabi-ar
AS=$(TOOLROOT)/arm-none-eabi-as

# Code Paths

DEVICE=$(LIBROOT)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
CORE=$(LIBROOT)/Libraries/CMSIS/CM3/CoreSupport
PERIPH=$(LIBROOT)/Libraries/STM32F10x_StdPeriph_Driver

# Tools

CC=$(TOOLROOT)/arm-none-eabi-gcc
LD=$(TOOLROOT)/arm-none-eabi-gcc
AR=$(TOOLROOT)/arm-none-eabi-ar
AS=$(TOOLROOT)/arm-none-eabi-as


# Search path for standard files

vpath %.c .



# compilation flags for gdb



# Compilation Flags




CFLAGS+= -mcpu=cortex-m3 -mthumb 



# compile and generate dependency info

asm: main.c
	$(CC) -c $(CFLAGS) $< -o $@


%.o: main.s
	$(CC) -c $(CFLAGS) $< -o $@








