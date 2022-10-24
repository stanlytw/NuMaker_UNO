
#BSP PATH
# Output directories
#ARM_GCC_TOOLCHAIN=C:\cygwin\home\Arduino-1.5.2-nuvoton\build\windows\work\hardware\tools\g++_arm_none_eabi\bin
#PROJECT_BASE_PATH=C:\Users\SChung\Desktop\system
#CORE=cortex-m0
#CHIP=M051Series
#OUTPUT_BIN=.\

CHIP=M032BT
ARM_GCC_TOOLCHAIN=C:/Users/hphuang/AppData/Local/Arduino15/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin
PROJECT_BASE_PATH=C:/Users/hphuang/AppData/Local/Arduino15/packages/nuvoton/hardware/nuvoton/1.6.10/system/$(CHIP)

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
BSP_BASE=$(mkfile_dir)

CORE=cortex-m0

OUTPUT_BIN=./
# ---------------------------------------------------------------------------------------

# Tool suffix when cross-compiling
CROSS_COMPILE = $(ARM_GCC_TOOLCHAIN)/arm-none-eabi-


# Compilation tools
AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
NM = $(CROSS_COMPILE)nm
ifeq ($(OS),Windows_NT)
RM=cs-rm -Rf
#RM=del /s /f
else
RM=rm -Rf
endif

SEP=/

# ---------------------------------------------------------------------------------------
# C Flags

CFLAGS += -Wall -Wchar-subscripts -Wcomment -Wformat=2 -Wimplicit-int
CFLAGS += -Werror-implicit-function-declaration -Wmain -Wparentheses
CFLAGS += -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wunused
CFLAGS += -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef
CFLAGS += -Wshadow -Wpointer-arith -Wbad-function-cast -Wwrite-strings
CFLAGS += -Wsign-compare -Waggregate-return -Wstrict-prototypes
# CFLAGS += -Wmissing-prototypes -Wmissing-declarations
CFLAGS += -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations
CFLAGS += -Wpacked -Wredundant-decls -Wnested-externs -Winline -Wlong-long
CFLAGS += -Wunreachable-code
CFLAGS += -Wcast-align
#CFLAGS += -Wmissing-noreturn
#CFLAGS += -Wconversion

# To reduce application size use only integer printf function.
CFLAGS += -Dprintf=iprintf

CFLAGS += --param max-inline-insns-single=500 -mcpu=$(CORE) -mthumb -mlong-calls -ffunction-sections -fdata-sections -std=c99
CFLAGS += $(OPTIMIZATION) $(INCLUDES) -D$(CHIP)


# ---------------------------------------------------------------------------------------
# ASM Flags

ASFLAGS = -mcpu=$(CORE) -mthumb -Wall -a -g $(INCLUDES)


# ---------------------------------------------------------------------------------------
#BSP compiler 
CMSIS_ARM_PATH=$(PROJECT_BASE_PATH)/CMSIS/Include
CMSIS_DEVICE_PATH=$(PROJECT_BASE_PATH)/Device
CMSIS_STDDRIVER_PATH=$(PROJECT_BASE_PATH)/StdDriver

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

vpath %.h $(BSP_BASE)Device/Nuvoton/$(CHIP)/Include $(BSP_BASE)CMSIS/Include $(BSP_BASE)StdDriver/inc 
vpath %.c $(BSP_BASE)Device/Nuvoton/$(CHIP)/Source $(BSP_BASE)Device/Nuvoton/$(CHIP)/Source/GCC $(BSP_BASE)CMSIS/Include $(BSP_BASE)StdDriver/src

VPATH=$(BSP_BASE)CMSIS/Device/Nuvoton/$(CHIP)/Source/GCC
VPATH+=$(BSP_BASE)CMSIS/Device/Nuvoton/$(CHIP)/Source
# VPATH+=$(CMSIS_ARM_PATH)
VPATH+=$(BSP_BASE)StdDriver/src


INCLUDES = -I$(BSP_BASE)Device/Nuvoton/$(CHIP)/Include
INCLUDES += -I$(BSP_BASE)CMSIS/Include
INCLUDES += -I$(BSP_BASE)StdDriver/inc
#INCLUDES += -I$(BSP_BASE)../../cores/nuvoton

#-------------------------------------------------------------------------------
ifdef DEBUG
OPTIMIZATION = -g -O0 -DDEBUG
else
OPTIMIZATION = -Os
endif

#-------------------------------------------------------------------------------
ifdef DEBUG
OUTPUT_OBJ=debug
OUTPUT_LIB=libchip_$(CHIP)_gcc_dbg.a
else
OUTPUT_OBJ=release
OUTPUT_LIB=libchip_$(CHIP)_gcc_rel.a
endif

OUTPUT_PATH=$(OUTPUT_BIN)/$(OUTPUT_OBJ)_$(CHIP)

#-------------------------------------------------------------------------------
# C source files and objects
#-------------------------------------------------------------------------------
C_SRC=$(wildcard $(BSP_BASE)CMSIS/Include/*.c)
C_SRC+=$(wildcard $(BSP_BASE)Device/Nuvoton/$(CHIP)/Source/*.c)
C_SRC+=$(wildcard $(BSP_BASE)Device/Nuvoton/$(CHIP)/Source/GCC/*.c)
C_SRC+=$(wildcard $(BSP_BASE)StdDriver/src/*.c)


C_OBJ_TEMP=$(patsubst %.c, %.o, $(notdir $(C_SRC)))

# during development, remove some files
C_OBJ_FILTER=_syscalls.o 
# remove the _syscall.o to avoid the wrong _write/_read function linked issue occurs in Arduino IDE for static library linking.

C_OBJ=$(filter-out $(C_OBJ_FILTER), $(C_OBJ_TEMP))

#-------------------------------------------------------------------------------
# Assembler source files and objects
#-------------------------------------------------------------------------------
A_SRC=$(wildcard $(BSP_BASE)Device/Nuvoton/$(CHIP)/Source/GCC/*.s)

A_OBJ_TEMP=$(patsubst %.s, %.o, $(notdir $(A_SRC)))

# during development, remove some files
A_OBJ_FILTER=

A_OBJ=$(filter-out $(A_OBJ_FILTER), $(A_OBJ_TEMP))

#-------------------------------------------------------------------------------
# Rules
#-------------------------------------------------------------------------------
all: $(CHIP)

$(CHIP): create_output $(OUTPUT_LIB)

.PHONY: create_output
create_output:
	@echo ------------------------------------------------------------
	@echo ARM_GCC_TOOLCHAIN=$(ARM_GCC_TOOLCHAIN)
	@echo PROJECT_BASE_PATH=$(PROJECT_BASE_PATH)
	@echo CMSIS_ARM_PATH=$(CMSIS_ARM_PATH)
	@echo AR=$(AR)
	@echo CC=$(CC)
	@echo C_SRC=$(C_SRC)
	@echo mkfile_dir=$(mkfile_dir)
	@echo BSP_BASE=$(BSP_BASE)
	@echo CMSIS_STDDRIVER_PATH=$(CMSIS_STDDRIVER_PATH)      
	@echo VPATH=$(VPATH)
	@echo CORE =$(CORE)
	@echo CHIP=$(CHIP)
	@echo OUTPUT_BIN=$(OUTPUT_BIN)
	@echo ------------------------------------------------------------
	@echo Compiler Information :
$(addprefix $(OUTPUT_PATH)/,$(C_OBJ)): $(OUTPUT_PATH)/%.o: %.c
	@echo before compile $(CC)
	@"$(CC)" -fms-extensions -c $(CFLAGS) $< -o $@ 

$(addprefix $(OUTPUT_PATH)/,$(A_OBJ)): $(OUTPUT_PATH)/%.o: %.s
	@"$(AS)" $(ASFLAGS) $< -o $@
	
$(OUTPUT_LIB): $(addprefix $(OUTPUT_PATH)/, $(C_OBJ)) $(addprefix $(OUTPUT_PATH)/, $(A_OBJ))
	@"$(AR)" -r "$(OUTPUT_BIN)/$@" $^
	@"$(NM)" "$(OUTPUT_BIN)/$@" > "$(OUTPUT_BIN)/$@.txt"

.PHONY: clean
clean:
	@echo ------------------------------------------------------------------------------------
	@echo --- Cleaning $(CHIP) files $(OUTPUT_PATH) $(subst /,$(SEP),$(OUTPUT_BIN)/$(OUTPUT_LIB))
	@$(RM) $(OUTPUT_PATH) 1>$(DEV_NUL) 2>&1
	@$(RM) $(subst /,$(SEP),$(OUTPUT_BIN)/$(OUTPUT_LIB)) 1>$(DEV_NUL) 2>&1
	@$(RM) $(subst /,$(SEP),$(OUTPUT_BIN)/$(OUTPUT_LIB)).txt 1>$(DEV_NUL) 2>&1
	@echo ------------------------------------------------------------------------------------

# dependencies
$(addprefix $(OUTPUT_PATH)/,$(C_OBJ)): $(OUTPUT_PATH)/%.o: $(wildcard $(CMSIS_DEVICE_PATH)/Nuvoton/$(CHIP)/Include/*.h) $(wildcard $(CMSIS_BASE_PATH)/*.h)
