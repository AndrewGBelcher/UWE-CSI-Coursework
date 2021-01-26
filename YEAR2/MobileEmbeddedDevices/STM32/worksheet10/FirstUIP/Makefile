# name of executable - change this to change the executables name

ELF=uIPdemo.elf

# object files
#$(UIP_OBJS)
OBJS=  $(STARTUP) $(RTOS_OBJS) $(SYS_OBJS) $(DHCP_OBJS)  com_port.o main.o 
DHCP_OBJS = timer.o  uip.o uip_arp.o psock.o uIP_Task.o emac.o hello_world.o dhcpc.o #  
SYS_OBJS= stm32f10x_rcc.o stm32f10x_gpio.o stm32f10x_tim.o  misc.o stm32f10x_usart.o  sys_calls.o core_cm3.o stm32_eth.o   
#UIP_OBJS= timer.o uip.o uip_arp.o psock.o uIP_Task.o hello_world.o emac.o 
# Tool path

TOOLROOT=/usr/bin

# Library path
WORKDIR=/home/seed/Downloads/med
LIBROOT=$(WORKDIR)/STM32F10x_StdPeriph_Lib_V3.5.0


#Uip and hello app
UIP_COMMON_DIR=../Common/ethernet/uIP/uip-1.0/uip
#HELLO_DEMO_DIR=hello
DHCP_DIR = dhcpc#dhcpc.c timer.c

# Tools

CC=$(TOOLROOT)/arm-none-eabi-gcc
LD=$(TOOLROOT)/arm-none-eabi-gcc
AR=$(TOOLROOT)/arm-none-eabi-ar
AS=$(TOOLROOT)/arm-none-eabi-as

# Code Paths

DEVICE=$(LIBROOT)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
CORE=$(LIBROOT)/Libraries/CMSIS/CM3/CoreSupport
PERIPH=$(LIBROOT)/Libraries/STM32F10x_StdPeriph_Driver
ETH=$(LIBROOT)/Libraries/STM32_ETH_Driver

# Search path for standard files

vpath %.c .

# Search path for perpheral library and FreeRTOS

vpath %.c $(CORE)
vpath %.c $(PERIPH)/src
vpath %.c $(DEVICE)
vpath %.c $(ETH)/src
vpath %.c $(UIP_COMMON_DIR)
#vpath %.c $(HELLO_DEMO_DIR)
vpath %.c $(DHCP_DIR)

#  Processor specific

PTYPE = STM32F10X_CL
LDSCRIPT = stm32f100.ld
STARTUP= startup_stm32f10x.o system_stm32f10x.o 

# compilation flags for gdb

CFLAGS  = -O0 -g

# Compilation Flags

FULLASSERT = -DUSE_FULL_ASSERT 

LDFLAGS+= -T$(LDSCRIPT) -mthumb -mcpu=cortex-m3 
CFLAGS+= -mcpu=cortex-m3 -mthumb -w
CFLAGS+= -I. -I$(DEVICE) -I$(ETH)/inc -I$(CORE) -I$(PERIPH)/inc -I$(DHCP_DIR) -I$(UIP_COMMON_DIR) -I. #-I$(HELLO_DEMO_DIR)
CFLAGS+= -D PACK_STRUCT_END=__attribute\(\(packed\)\) 
CFLAG+= -D ALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\)
CFLAGS+=  -D$(PTYPE)  -D GCC_ARMCM3 -DUSE_STDPERIPH_DRIVER $(FULLASSERT)

# Build executable 

$(ELF) : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

# compile and generate dependency info

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
	$(CC) -MM $(CFLAGS) $< > $*.d 

%.o: %.s
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) $(ELF) 

debug: $(ELF)
	arm-none-eabi-gdb $(ELF)


# pull in dependencies

-include $(OBJS:.o=.d)




