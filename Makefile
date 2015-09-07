#################################################
# MAKEFILE For STM32F7xxx Devices 				#
# (c) 20120630 Nemui Trinomius					#
# http://nemuisan.blog.bai.ne.jp				#
# Configured for STM32F7 by SpiralRay			#
#################################################

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

USE_EMWIN = 1


OUTDIR = ./Build
OBJS= $(SOURCES_C:%.c=$(OUTDIR)/%.o)  $(SOURCES_CPP:%.cpp=$(OUTDIR)/%.o) $(SOURCES_S:%.s=$(OUTDIR)/%.o)
DEPS= $(SOURCES_C:%.c=$(OUTDIR)/%.d) $(SOURCES_CPP:$(OUTDIR)/%.cpp=%.d)

################
# Sources

SOURCES_S = Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/gcc/startup_stm32f746xx.s

SOURCES_C = $(wildcard src/*.c)
SOURCES_C += sys/syscalls.c

#SOURCES_C += Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/system_stm32f7xx.c
SOURCES_C += $(wildcard Drivers/BSP/STM32746G-Discovery/*.c)
SOURCES_C += $(wildcard Drivers/STM32F7xx_HAL_Driver/Src/*.c)
SOURCES_C += $(wildcard Drivers/BSP/Components/ft5336/*.c)

SOURCES_CPP =

SOURCES = $(SOURCES_S) $(SOURCES_C) $(SOURCES_CPP)

# Set library PATH
LIBPATHS     = $(EMWIN_PATH)/Lib
LIBRARY_DIRS = $(addprefix -L,$(LIBPATHS))
STATIC_LIB = 

################
# Includes and Defines

INCLUDES += -I . -I src -I inc -I sys
INCLUDES += -I Drivers/CMSIS/Include
INCLUDES += -I Drivers/CMSIS/Device/ST/STM32F7xx/Include
INCLUDES += -I Drivers/STM32F7xx_HAL_Driver/Inc
INCLUDES += -I Drivers/BSP/STM32746G-Discovery

INCLUDES += $(addprefix -I ,$(INCPATHS))

DEFINES = -DSTM32 -DSTM32F7 -DSTM32F746xx -DSTM32F746NGHx -DSTM32F746G_DISCO -DUSE_STM32746G_DISCOVERY -DUSE_HAL_DRIVER

ifeq ($(OS_SUPPORT),USE_FREERTOS)
DEFINES += -D$(OS_SUPPORT)
endif

ifeq ($(LOG_SUPPORT),USE_LCD_LOG)
 INCPATHS  += Utilities/Log
 SOURCES_C += Utilities/Log/lcd_log.c
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

SOURCES_C += $(wildcard $(RTOS_PATH)/*.c)
 
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

SOURCES_C += $(wildcard $(LwIP_PATH)/src/api/*.c)
SOURCES_C += $(wildcard $(LwIP_PATH)/src/core/*.c)
SOURCES_C += $(wildcard $(LwIP_PATH)/src/core/ipv4/*.c)
SOURCES_C += $(wildcard $(LwIP_PATH)/src/core/snmp/*.c)
SOURCES_C += $(LwIP_PATH)/src/netif/etharp.c
SOURCES_C += $(LwIP_PATH)/src/netif/slipif.c
SOURCES_C += $(wildcard $(LwIP_PATH)/src/netif/ppp/*.c)

ifeq ($(OS_SUPPORT),USE_FREERTOS)
 INCPATHS  += $(LwIP_PATH)/system/OS
 SOURCES_C += $(LwIP_PATH)/system/OS/sys_arch.c
else
 INCPATHS  += $(LwIP_PATH)/system/noOS
endif
endif


EMWIN_PATH = ./Middlewares/ST/STemWin

ifeq ($(USE_EMWIN),1)

INCPATHS += \
	$(EMWIN_PATH)/inc

ifeq ($(OS_SUPPORT),USE_FREERTOS)
 SOURCES_C += $(EMWIN_PATH)/OS/GUI_X_OS.c
 STATIC_LIB += -l_STemWin528_CM7_OS_GCC
 DEFINES += -DOS_SUPPORT
else
 SOURCES_C += $(EMWIN_PATH)/OS/GUI_X.c
 STATIC_LIB += -l_STemWin528_CM7_GCC
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
REMOVE		= $(REMOVAL) -f
REMOVEDIR 	= $(REMOVAL) -rf

################
# Compiler options

MCUFLAGS = -mcpu=cortex-m7 -mlittle-endian
MCUFLAGS += -mfloat-abi=hard -mfpu=fpv5-sp-d16
MCUFLAGS += -mthumb

#DEBUGFLAGS = -O0 -g -gdwarf-2
DEBUGFLAGS = -O2

CFLAGS = -c -MMD -MP -std=c11
CFLAGS += -Wall -Wextra --pedantic -Wno-unused-parameter

CFLAGS_EXTRA = -nostartfiles -fdata-sections -ffunction-sections
CFLAGS_EXTRA += -Wl,--gc-sections -Wl,-Map=$(PROJECT).map

CFLAGS += $(DEFINES) $(MCUFLAGS) $(DEBUG_FLAGS) $(CFLAGS_EXTRA) $(INCLUDES)

LDFLAGS = -static $(MCUFLAGS)
LDFLAGS += -Wl,--start-group -lgcc -lm -lc -lg -lstdc++ -lsupc++ -Wl,--end-group
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -T STM32F746NGH6_AXIM.ld -L. -Lldscripts
LDFLAGS += -Xlinker -Map -Xlinker $(PROJECT).map
LDFLAGS += $(LIBRARY_DIRS)

# Object Copy and dfu generation FLAGS
OBJCPFLAGS = -O
OBJDUMPFLAGS = -h -S
DFU	  = hex2dfu
DFLAGS = -w

################
# Build rules

all: $(PROJECT).hex $(PROJECT).bin

$(PROJECT).hex: $(PROJECT).elf
	$(OBJCOPY) -O ihex $(PROJECT).elf $(PROJECT).hex

$(PROJECT).elf: $(OBJS)
	@$(MSGECHO)
	@$(MSGECHO) $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(PROJECT).elf $(STATIC_LIB)
	$(SIZE) -A $(PROJECT).elf

$(PROJECT).bin: $(PROJECT).elf
	@$(MSGECHO)
	@$(MSGECHO) Objcopy: $@
	$(OBJCOPY) $(OBJCPFLAGS) binary $< $@ 

$(OUTDIR)/%.o:%.c
ifneq (,$(filter $(shell uname),Darwin Linux))
	@if [ ! -e `/usr/bin/dirname $@` ]; then /bin/mkdir -p `/usr/bin/dirname $@`; fi
else
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
endif
	@$(MSGECHO) Compile: $<
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
	@$(MSGECHO)
$(OUTDIR)/%.o:%.cpp
ifneq (,$(filter $(shell uname),Darwin Linux))
	@if [ ! -e `/usr/bin/dirname $@` ]; then /bin/mkdir -p `/usr/bin/dirname $@`; fi
else
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
endif
	@$(MSGECHO) Compile: $<
	$(CPP) $(CXXFLAGS) $(INCLUDES) $< -o $@
	@$(MSGECHO)
$(OUTDIR)/%.o:%.s
ifneq (,$(filter $(shell uname),Darwin Linux))
	@if [ ! -e `/usr/bin/dirname $@` ]; then /bin/mkdir -p `/usr/bin/dirname $@`; fi
else
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
endif
	@$(MSGECHO) Assemble: $<
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
	@$(MSGECHO)

flash :
ifneq (,$(filter $(shell uname),Darwin Linux))
	@echo "Writing $(TARGET).bin"
	@st-flash --reset write $(TARGET).bin 0x08000000
else
	@"$(STLINK_PATH)/ST-LINK_CLI.exe" -c SWD UR -P $(PROJECT).hex 0x08000000
	@"$(STLINK_PATH)/ST-LINK_CLI.exe" -c SWD UR -Rst
endif

.PHONY clean:
	$(REMOVE) $(OBJS)
	$(REMOVE) $(DEPS)
	$(REMOVE) $(PROJECT).elf $(PROJECT).hex $(PROJECT).map $(PROJECT).bin
	$(REMOVEDIR) $(OUTDIR)/*

-include $(DEPS)

# EOF