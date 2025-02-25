# ******************************************************************************************
#   Filename    : Makefile
#
#   Author      : Chalandi Amine
#
#   Owner       : Chalandi Amine
#
#   Date        : 22.11.2022
#
#   Description : Build system
#
# ******************************************************************************************

############################################################################################
# Defines
############################################################################################
PRJ_NAME    = coprocessor_binary
OUTPUT_DIR  = $(CURDIR)/../Output
OBJ_DIR     = $(OUTPUT_DIR)/obj
LD_SCRIPT   = $(SRC_DIR)/coprocessor.ld
SRC_DIR     = $(CURDIR)/../Code
RTOS        = 
PYTHON      = python
ERR_MSG_FORMATER_SCRIPT = $(CURDIR)/../scripts/CompilerErrorFormater.py
BIN2ASM_SCRIPT = $(CURDIR)/../scripts/bin2asm.py

############################################################################################
# Toolchain
############################################################################################
TOOLCHAIN          = riscv32-unknown-elf

ARCH               = -march=rv32imc         \
                     -mabi=ilp32            \
                     -msmall-data-limit=0   \
                     -falign-functions=4    \
                     -fomit-frame-pointer


DEFS               = -DI_KNOW_WHAT_I_AM_DOING


AS      = $(TOOLCHAIN)-gcc
CC      = $(TOOLCHAIN)-gcc
CPP     = $(TOOLCHAIN)-g++
LD      = $(TOOLCHAIN)-gcc
OBJDUMP = $(TOOLCHAIN)-objdump
OBJCOPY = $(TOOLCHAIN)-objcopy
SIZE    = $(TOOLCHAIN)-size
READELF = $(TOOLCHAIN)-readelf

############################################################################################
# Optimization Compiler flags
############################################################################################

OPT_MODIFIED_O2 = -O2                               \
                  -fno-reorder-blocks-and-partition \
                  -fno-reorder-functions

NO_OPT = -O0

OPT = $(OPT_MODIFIED_O2)

############################################################################################
# GCC Compiler verbose flags
############################################################################################

VERBOSE_GCC = -frecord-gcc-switches -fverbose-asm

############################################################################################
# C Compiler flags
############################################################################################

COPS  = $(OPT)                                        \
        $(ARCH)                                       \
        $(DEFS)                                       \
        -ffreestanding                                \
        -MD                                           \
        -Wa,-adhln=$(OBJ_DIR)/$(basename $(@F)).lst   \
        -gdwarf-4 -ggdb                               \
        -Wconversion                                  \
        -Wsign-conversion                             \
        -Wunused-parameter                            \
        -Wuninitialized                               \
        -Wmissing-declarations                        \
        -Wshadow                                      \
        -Wunreachable-code                            \
        -Wmissing-include-dirs                        \
        -x c                                          \
        -std=c11                                      \
        -Wall                                         \
        -Wextra                                       \
        -fomit-frame-pointer                          \
        -gdwarf-2                                     \
        -fno-exceptions

############################################################################################
# C++ Compiler flags
############################################################################################

CPPOPS  = $(OPT)                                        \
          $(ARCH)                                       \
          $(DEFS)                                       \
          -ffreestanding                                \
          -Wa,-adhln=$(OBJ_DIR)/$(basename $(@F)).lst   \
          -gdwarf-4 -ggdb                               \
          -Wconversion                                  \
          -Wsign-conversion                             \
          -Wunused-parameter                            \
          -Wuninitialized                               \
          -Wmissing-declarations                        \
          -Wshadow                                      \
          -Wunreachable-code                            \
          -Wmissing-include-dirs                        \
          -Wall                                         \
          -Wextra                                       \
          -fomit-frame-pointer                          \
          -gdwarf-2                                     \
          -fno-exceptions                               \
          -x c++                                        \
          -fno-rtti                                     \
          -fno-use-cxa-atexit                           \
          -fno-nonansi-builtins                         \
          -fno-threadsafe-statics                       \
          -fno-enforce-eh-specs                         \
          -ftemplate-depth=128                          \
          -Wzero-as-null-pointer-constant

############################################################################################
# Assembler flags
############################################################################################
ifeq ($(AS), $(TOOLCHAIN)-as)
  ASOPS =  $(ARCH)       \
           -alh          \
           -g
else
  ASOPS = $(OPT)                                        \
          $(ARCH)                                       \
          $(DEFS)                                       \
          -MD                                           \
          -Wa,-adhln=$(OBJ_DIR)/$(basename $(@F)).lst   \
          -gdwarf-4 -ggdb                               \
          -Wconversion                                  \
          -Wsign-conversion                             \
          -Wunused-parameter                            \
          -Wuninitialized                               \
          -Wmissing-declarations                        \
          -Wshadow                                      \
          -Wunreachable-code                            \
          -Wmissing-include-dirs                        \
          -x assembler                                  \
          -std=c11                                      \
          -Wall                                         \
          -Wextra                                       \
          -fomit-frame-pointer                          \
          -gdwarf-2                                     \
          -fno-exceptions
endif

############################################################################################
# Linker flags
############################################################################################

ifeq ($(LD), $(TOOLCHAIN)-ld)
  LOPS = -nostartfiles                          \
         -nostdlib                              \
         $(ARCH)                                \
         $(DEFS)                                \
         -e _start                              \
         --print-memory-usage                   \
         --print-map                            \
         -dT $(LD_SCRIPT)                       \
         -Map=$(OUTPUT_DIR)/$(PRJ_NAME).map     \
         --no-warn-rwx-segments                 \
         -z,max-page-size=4096                  \
         --specs=nano.specs                     \
         --specs=nosys.specs
else
  LOPS = -nostartfiles                          \
         -nostdlib                              \
         -fno-lto                               \
         $(ARCH)                                \
         $(DEFS)                                \
         -e _start                              \
         -Wl,--print-memory-usage               \
         -Wl,--print-map                        \
         -Wl,-dT $(LD_SCRIPT)                   \
         -Wl,-Map=$(OUTPUT_DIR)/$(PRJ_NAME).map \
         -Wl,--no-warn-rwx-segments             \
         -Wl,-z,max-page-size=4096              \
         --specs=nano.specs                     \
         --specs=nosys.specs
endif

############################################################################################
# Source Files
############################################################################################

SRC_FILES := $(SRC_DIR)/Appli/main.c            \
             $(SRC_DIR)/Startup/Startup.c       \
             $(SRC_DIR)/Startup/boot.s          \
             $(SRC_DIR)/Std/StdLib.c   


############################################################################################
# Include Paths
############################################################################################
INC_FILES := $(SRC_DIR)                       \
             $(SRC_DIR)/Appli                 \
             $(SRC_DIR)/Mcal                  \
             $(SRC_DIR)/Startup               \
             $(SRC_DIR)/Std


############################################################################################
# RTOS Files
############################################################################################
ifeq ($(RTOS),osek)
 include $(SRC_DIR)/OSEK/Os.makefile
 SRC_FILES += $(SRC_DIR)/Appli/tasks.c
endif

############################################################################################
# Rules
############################################################################################

VPATH = $(subst \,/,$(sort $(dir $(SRC_FILES)) $(OBJ_DIR)))

FILES_O = $(addprefix $(OBJ_DIR)/, $(notdir $(addsuffix .o, $(basename $(SRC_FILES)))))

ifeq ($(MAKECMDGOALS), BUILD_STAGE_2)
-include $(subst .o,.d,$(FILES_O))
endif

REBUILD_STAGE_1 : CLEAN PRE_BUILD
REBUILD_STAGE_2 : LINK
REBUILD_STAGE_3 : GENERATE POST_BUILD

BUILD_STAGE_1   : PRE_BUILD
BUILD_STAGE_2   : LINK
BUILD_STAGE_3   : GENERATE POST_BUILD

############################################################################################
# Recipes
############################################################################################
.PHONY : LINK
LINK : $(OUTPUT_DIR)/$(PRJ_NAME).elf
	@-echo "" > /dev/null


.PHONY : PRE_BUILD
PRE_BUILD:
	@-echo +++ Building ESP32-S3 RISC-V coprocessor image
	@git log -n 1 --decorate-refs=refs/heads/ --pretty=format:"+++ Git branch: %D (%h)" 2>/dev/null || true
	@git log -n 1 --clear-decorations 2> /dev/null > /dev/null || true
	@echo +++ info: "$(shell $(CC) -v 2>&1 | tail -n 1)"
	@echo +++ info: "$(shell make -v 2>&1 | head -n 1)"
	@echo +++ info: "$(shell $(PYTHON) --version 2>&1 | head -n 1)"
	@$(if $(shell test -d $(OBJ_DIR) && echo yes),,mkdir -p $(subst \,/,$(OBJ_DIR)))

.PHONY : POST_BUILD
POST_BUILD:
	@-echo +++ End
	@-echo ""


.PHONY : CLEAN
CLEAN :
	@-rm -rf $(OUTPUT_DIR)/$(PRJ_NAME).bin     2>/dev/null || true
	@-rm -rf $(OUTPUT_DIR)/$(PRJ_NAME).dis     2>/dev/null || true
	@-rm -rf $(OUTPUT_DIR)/$(PRJ_NAME).elf     2>/dev/null || true
	@-rm -rf $(OUTPUT_DIR)/$(PRJ_NAME).hex     2>/dev/null || true
	@-rm -rf $(OUTPUT_DIR)/$(PRJ_NAME).map     2>/dev/null || true
	@-rm -rf $(OUTPUT_DIR)/$(PRJ_NAME).readelf 2>/dev/null || true
	@-rm -rf $(OUTPUT_DIR)/$(PRJ_NAME).sym     2>/dev/null || true
	@-rm -rf $(OBJ_DIR)                        2>/dev/null || true
	@-mkdir -p $(subst \,/,$(OUTPUT_DIR))

$(OBJ_DIR)/%.o : %.c
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@-$(CC) $(COPS) $(addprefix -I, $(INC_FILES)) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
	@-$(PYTHON) $(ERR_MSG_FORMATER_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR

ifeq ($(AS), $(TOOLCHAIN)-as)
$(OBJ_DIR)/%.o : %.s
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@$(AS) $(ASOPS) $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err >$(OBJ_DIR)/$(basename $(@F)).lst
	@-$(PYTHON) $(ERR_MSG_FORMATER_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR
else
$(OBJ_DIR)/%.o : %.s
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@-$(AS) $(ASOPS) $(addprefix -I, $(INC_FILES)) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
	@-$(PYTHON) $(ERR_MSG_FORMATER_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR
endif

$(OBJ_DIR)/%.o : %.cpp
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@$(CPP) $(CPPOPS) $(addprefix -I, $(INC_FILES)) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
	@-$(PYTHON) $(ERR_MSG_FORMATER_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR

$(OUTPUT_DIR)/$(PRJ_NAME).elf : $(FILES_O) $(LD_SCRIPT)
	@-echo +++ LINK: $(subst \,/,$@)
ifeq ($(FORMAT_LINKER_ERR), )
	@$(LD) $(LOPS) $(FILES_O) -o $(OUTPUT_DIR)/$(PRJ_NAME).elf
else
	@$(LD) $(LOPS) $(FILES_O) -o $(OUTPUT_DIR)/$(PRJ_NAME).elf 2> $(OBJ_DIR)/linker.err || true
	@-$(PYTHON) $(LINKER_ERR_MSG_FORMATER_SCRIPT) $(OBJ_DIR)/linker.err
endif

.PHONY : GENERATE
GENERATE:
	@$(if $(wildcard $(OUTPUT_DIR)/$(PRJ_NAME).elf), ,$(error Error: Link not succeeded !))
	@-echo +++ generate: $(OUTPUT_DIR)/$(PRJ_NAME).readelf
	@$(READELF) -WhS $(OUTPUT_DIR)/$(PRJ_NAME).elf > $(OUTPUT_DIR)/$(PRJ_NAME).readelf
	@-echo +++ generate: $(OUTPUT_DIR)/$(PRJ_NAME).sym
	@$(READELF) -Ws $(OUTPUT_DIR)/$(PRJ_NAME).elf > $(OUTPUT_DIR)/$(PRJ_NAME).sym
	@-echo +++ generate: $(OUTPUT_DIR)/$(PRJ_NAME).dis
	@$(OBJDUMP) -d --visualize-jumps --wide $(OUTPUT_DIR)/$(PRJ_NAME).elf > $(OUTPUT_DIR)/$(PRJ_NAME).dis
	@-echo +++ generate: $(OUTPUT_DIR)/$(PRJ_NAME).hex
	@$(OBJCOPY) $(OUTPUT_DIR)/$(PRJ_NAME).elf -O ihex $(OUTPUT_DIR)/$(PRJ_NAME).hex
	@-echo +++ generate: $(OUTPUT_DIR)/$(PRJ_NAME).bin
	@$(OBJCOPY) $(OUTPUT_DIR)/$(PRJ_NAME).elf -O binary $(OUTPUT_DIR)/$(PRJ_NAME).bin
	@-echo +++ generate: $(OUTPUT_DIR)/$(PRJ_NAME).s
	@$(PYTHON) $(BIN2ASM_SCRIPT) -i $(OUTPUT_DIR)/$(PRJ_NAME).bin -o $(OUTPUT_DIR)/$(PRJ_NAME).s -s ".coprocessor" -l 16 -g coprocessor_bin
