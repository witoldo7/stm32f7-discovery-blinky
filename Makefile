#################################################
# MAKEFILE For STM32F4xxx Devices 				#
# (c) 20120630 Nemui Trinomius					#
# http://nemuisan.blog.bai.ne.jp				#
#################################################

# Environment Dependent!!! This Environment assure under WINDOWS !!
# Throw path into YOUR environments for each Operation Systems.

ifneq (,$(filter $(shell uname),Darwin Linux))

else
export PATH = %SYSTEMROOT%;$(TOOLDIR)/bin;$(OCDIR);$(DFUDIR);$(MAKEDIR)
endif

# Toolchain prefix (i.e arm-none-eabi -> arm-none-eabi-gcc.exe)
ifneq (,$(filter $(shell uname),Darwin Linux))
TCHAIN  = /usr/local/arm-cs-tools/bin/arm-none-eabi
else
TCHAIN  = "$(GNU_ARM_PATH)/bin/arm-none-eabi"
endif

# Development Tools based on GNU Compiler Collection
DEVTOOL = LAUNCHPAD
#DEVTOOL = BLEEDING_EDGE
#DEVTOOL = YAGARTO
#DEVTOOL = DEVKITARM
#DEVTOOL = SOURCERY

# Check BuildTools
ifeq ($(DEVTOOL),LAUNCHPAD)
 TOOLDIR = C:/Devz/ARM/Launchpad
 NANOLIB = --specs=nano.specs
 NANOLIB += -u _printf_float
 NANOLIB += -u _scanf_float
 REMOVAL = rm
else ifeq ($(DEVTOOL),BLEEDING_EDGE)
 TOOLDIR = C:/Devz/ARM/Bleeding-edge
 REMOVAL = rm
else ifeq ($(DEVTOOL),DEVKITARM)
 TOOLDIR = C:/Devz/ARM/devkitARM
 REMOVAL = rm
else ifeq ($(DEVTOOL),YAGARTO)
 TOOLDIR = C:/Devz/ARM/Yagarto
 REMOVAL = rm
else ifeq ($(DEVTOOL),SOURCERY)
 TOOLDIR = C:/Devz/ARM/Sourcery
 REMOVAL = rm
else
 $(error SET BUILD-TOOLS AT FIRST!!)
endif

#Set DFUse Directory
DFUDIR	= C:/Devz/ARM/ST/DFUse/BIN
#Set Shell Definitions
WSHELL  = cmd
MSGECHO = echo
#Set GDB/Insight Directory
GDBDIR  = C:/Devz/ARM/insight/bin
INSIGHT = $(GDBDIR)/arm-none-eabi-insight
# Environment Dependent!!!

PROJECT = blinky

# Use FreeRTOS?
#OS_SUPPORT		= BARE_METAL
OS_SUPPORT		= USE_FREERTOS

# Use LwIP?
#LWIP_SUPPORT	= USE_LWIP

#LOG_SUPPORT = USE_LCD_LOG

################
# Sources

SOURCES_S = Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/gcc/startup_stm32f746xx.s

SOURCES_C = $(wildcard src/*.c)
SOURCES_C += sys/_sbrk.c
SOURCES_C += sys/stubs.c
#SOURCES_C += Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/system_stm32f7xx.c
SOURCES_C += $(wildcard Drivers/BSP/STM32746G-Discovery/*.c)
SOURCES_C += $(wildcard Drivers/STM32F7xx_HAL_Driver/Src/*.c)
SOURCES_C += $(wildcard Drivers/BSP/Components/ft5336/*.c)

SOURCES_CPP =

SOURCES = $(SOURCES_S) $(SOURCES_C) $(SOURCES_CPP)
OBJS = $(SOURCES_S:.s=.o) $(SOURCES_C:.c=.o) $(SOURCES_CPP:.cpp=.o)

################
# Includes and Defines

INCLUDES += -I . -I src -I inc -I sys
INCLUDES += -I Drivers/CMSIS/Include
INCLUDES += -I Drivers/CMSIS/Device/ST/STM32F7xx/Include
INCLUDES += -I Drivers/STM32F7xx_HAL_Driver/Inc
INCLUDES += -I Drivers/BSP/STM32746G-Discovery

INCLUDES += $(addprefix -I ,$(INCPATHS))

DEFINES = -DSTM32 -DSTM32F7 -DSTM32F746xx -DSTM32F746NGHx -DSTM32F746G_DISCO -DUSE_STM32746G_DISCOVERY

ifeq ($(LOG_SUPPORT),USE_LCD_LOG)
 INCPATHS  += Utilities/Log
 SOURCES_C += Utilities/Log/lcd_log.c
else
 SOURCES_C += sys/_io.c
endif

FATFS = ./Middlewares/Third_Party/FatFs/src

INCLUDES += -I $(FATFS) -I $(FATFS)/drivers

SOURCES_C += \
 $(wildcard $(FATFS)/*.c)				\
 $(FATFS)/drivers/sd_diskio.c			\
 $(FATFS)/option/unicode.c				\
 $(FATFS)/option/syscall.c

ifeq ($(OS_SUPPORT),USE_FREERTOS)

RTOS_PATH = ./Middlewares/Third_Party/FreeRTOS/Source

SOURCES_C += $(sort \
 $(patsubst %.c,%.o,$(wildcard $(RTOS_PATH)/*.c)))
 
SOURCES_C += \
	$(RTOS_PATH)/portable/GCC/ARM_CM7/r0p1/port.c	\
	$(RTOS_PATH)/portable/MemMang/heap_2.c			\
	$(RTOS_PATH)/CMSIS_RTOS/cmsis_os.c
 
INCPATHS	 += 						\
 $(RTOS_PATH)/include					\
 $(RTOS_PATH)/portable/GCC/ARM_CM7/r0p1	\
 $(RTOS_PATH)/CMSIS_RTOS

endif

ifeq ($(LWIP_SUPPORT),USE_LWIP)
LwIP_PATH = ./Middlewares/Third_Party/LwIP

INCPATHS	 += 						\
 $(LwIP_PATH)/system					\
 $(LwIP_PATH)/src/include				\
 $(LwIP_PATH)/src/include/ipv4			\
 $(LwIP_PATH)/src/include				\
 $(LwIP_PATH)/src/include/netif			\
 $(LwIP_PATH)/src/include/posix			\
 $(LwIP_PATH)/src/include/posix/sys

SOURCES_C += $(sort $(patsubst %.c,%.o,$(wildcard $(LwIP_PATH)/src/api/*.c)))
SOURCES_C += $(sort $(patsubst %.c,%.o,$(wildcard $(LwIP_PATH)/src/core/*.c)))
SOURCES_C += $(sort $(patsubst %.c,%.o,$(wildcard $(LwIP_PATH)/src/core/ipv4/*.c)))
SOURCES_C += $(sort $(patsubst %.c,%.o,$(wildcard $(LwIP_PATH)/src/core/snmp/*.c)))
SOURCES_C += $(LwIP_PATH)/src/netif/etharp.c
SOURCES_C += $(LwIP_PATH)/src/netif/slipif.c
SOURCES_C += $(sort $(patsubst %.c,%.o,$(wildcard $(LwIP_PATH)/src/netif/ppp/*.c)))

ifeq ($(OS_SUPPORT),USE_FREERTOS)
 INCPATHS  += $(LwIP_PATH)/system/OS
 SOURCES_C += $(LwIP_PATH)/system/OS/sys_arch.c
else
 INCPATHS  += $(LwIP_PATH)/system/noOS
endif
endif

################
# Compiler/Assembler/Linker/etc

PREFIX = arm-none-eabi

# TOOLCHAIN SETTING
CC 			= $(TCHAIN)-gcc
CPP 		= $(TCHAIN)-g++
OBJCOPY 	= $(TCHAIN)-objcopy
OBJDUMP 	= $(TCHAIN)-objdump
SIZE 		= $(TCHAIN)-size
AS 			= $(TCHAIN)-as
AR 			= $(TCHAIN)-ar
LD 			= $(TCHAIN)-g++
NM 			= $(TCHAIN)-nm
READELF 	= $(TCHAIN)-readelf
SIZE 		= $(TCHAIN)-size
GDB 		= $(TCHAIN)-gdb
RM = rm -f

################
# Compiler options

MCUFLAGS = -mcpu=cortex-m7 -mlittle-endian
MCUFLAGS += -mfloat-abi=hard -mfpu=fpv5-sp-d16
MCUFLAGS += -mthumb

DEBUGFLAGS = -O0 -g -gdwarf-2
#DEBUGFLAGS = -O2

CFLAGS = -std=c11
CFLAGS += -Wall -Wextra --pedantic

CFLAGS_EXTRA = -nostartfiles -fdata-sections -ffunction-sections
CFLAGS_EXTRA += -Wl,--gc-sections -Wl,-Map=$(PROJECT).map

CFLAGS += $(DEFINES) $(MCUFLAGS) $(DEBUG_FLAGS) $(CFLAGS_EXTRA) $(INCLUDES)

LDFLAGS = -static $(MCUFLAGS)
LDFLAGS += -Wl,--start-group -lgcc -lm -lc -lg -lstdc++ -lsupc++ -Wl,--end-group
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -T stm32f7-discovery.ld -L. -Lldscripts
LDFLAGS += -Xlinker -Map -Xlinker $(PROJECT).map

# Object Copy and dfu generation FLAGS
OBJCPFLAGS = -O
OBJDUMPFLAGS = -h -S -C
DFU	  = hex2dfu
DFLAGS = -w

################
# Build rules

all: $(PROJECT).hex $(PROJECT).bin

$(PROJECT).hex: $(PROJECT).elf
	$(OBJCOPY) -O ihex $(PROJECT).elf $(PROJECT).hex

$(PROJECT).elf: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(PROJECT).elf
	$(SIZE) -A $(PROJECT).elf

$(PROJECT).bin: $(PROJECT).elf
	@$(MSGECHO)
	@$(MSGECHO) Objcopy: $@
	$(OBJCOPY) $(OBJCPFLAGS) binary $< $@ 

clean:
	$(RM) $(OBJS) $(PROJECT).elf $(PROJECT).hex $(PROJECT).map $(PROJECT).bin
	
flash :
ifneq (,$(filter $(shell uname),Darwin Linux))
	@echo "Writing $(TARGET).bin"
	@st-flash --reset write $(TARGET).bin 0x08000000
else
	@"$(STLINK_PATH)/ST-LINK_CLI.exe" -c SWD UR -P $(PROJECT).hex 0x08000000
	@"$(STLINK_PATH)/ST-LINK_CLI.exe" -c SWD UR -Rst
endif

# EOF
