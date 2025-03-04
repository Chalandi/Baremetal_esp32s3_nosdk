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
PRJ_NAME    = baremetal_esp32s3_nosdk
OUTPUT_DIR  = $(CURDIR)/../Output
OBJ_DIR     = $(OUTPUT_DIR)/obj
LD_SCRIPT   = $(SRC_DIR)/Memory_Map.ld
SRC_DIR     = $(CURDIR)/../Code
COPROCESSOR = #RISC-V
RTOS        = 
PYTHON      = python
ESPTOOL     = esptool
ERR_MSG_FORMATER_SCRIPT = $(CURDIR)/../Tools/scripts/CompilerErrorFormater.py
LINKER_ERR_MSG_FORMATER_SCRIPT = $(CURDIR)/../Tools/scripts/LinkerErrorFormater.py
FORMAT_LINKER_ERR =
REFORMAT_BIN      = $(CURDIR)/../Tools/scripts/ConcatenateDataBlocks.py

############################################################################################
# Toolchain
############################################################################################
TOOLCHAIN          = xtensa-esp32s3-elf

ARCH               =  -mabi=call0                   \
                      -mno-text-section-literals    \
                      -mstrict-align                \
                      -mlongcalls                   \
                      -fomit-frame-pointer          \
                      -fstrict-volatile-bitfields   \
                      -fno-jump-tables              \
                      -fno-tree-switch-conversion   \
                      -fno-stack-protector

DEFS_IEEE754_SF = -DL_divsf3

DEFS               = -DI_KNOW_WHAT_I_AM_DOING \
                     $(DEFS_IEEE754_SF)


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
         -e Startup_Init                        \
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
             $(SRC_DIR)/Startup/IntHandler.c    \
             $(SRC_DIR)/Startup/boot.s          \
             $(SRC_DIR)/Std/ieee754-sf.S        \
             $(SRC_DIR)/Startup/IntVectTable.s  \
             $(SRC_DIR)/Mcal/Mcu.c              \
             $(SRC_DIR)/Std/StdLib.c



############################################################################################
# Include Paths
############################################################################################
INC_FILES := $(SRC_DIR)                             \
             $(SRC_DIR)/Appli                       \
             $(SRC_DIR)/Mcal                        \
             $(SRC_DIR)/Startup                     \
             $(SRC_DIR)/Std

############################################################################################
# External Libaries for CALL0 ABI
############################################################################################
EXTERNAL_LIB := -L$(SRC_DIR)/Std/lib_call0_abi  \
                -l:libgcc_call0_abi.a           \
                -l:libc_call0_abi.a             \
                -l:libg_call0_abi.a             \
                -l:libm_call0_abi.a

############################################################################################
# Coprocessor build
############################################################################################
ifeq ($(COPROCESSOR), RISC-V)
  DEFS += -DCOPROCESSOR_ENABLED
  SRC_FILES += $(SRC_DIR)/Coprocessor/Output/coprocessor_binary.s
  COPROCESSOR_MAKEFILE = coprocessor.mk
  COPROCESSOR_MAKEFILE_DIR =../Code/Coprocessor/Build
endif

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

VPATH := $(subst \,/,$(sort $(dir $(SRC_FILES)) $(OBJ_DIR)))
FILES_O := $(addprefix $(OBJ_DIR)/, $(notdir $(addsuffix .o, $(basename $(SRC_FILES)))))

ifeq ($(MAKECMDGOALS), BUILD_STAGE_2)
-include $(subst .o,.d,$(FILES_O))
endif

REBUILD_STAGE_1 : CLEAN PRE_BUILD
REBUILD_STAGE_2 : link
REBUILD_STAGE_3 : GENERATE POST_BUILD

BUILD_STAGE_1   : PRE_BUILD
BUILD_STAGE_2   : link
BUILD_STAGE_3   : GENERATE POST_BUILD

############################################################################################
# Recipes
############################################################################################
.PHONY : link
link : $(OUTPUT_DIR)/$(PRJ_NAME).elf
	@-echo "" > /dev/null

.PHONY : PRE_BUILD
PRE_BUILD:
ifeq ($(COPROCESSOR), RISC-V)
	@make REBUILD_STAGE_1 -C $(COPROCESSOR_MAKEFILE_DIR) -f $(COPROCESSOR_MAKEFILE) --no-print-directory
	@make REBUILD_STAGE_2 -k -j $(getconf _NPROCESSORS_ONLN) -C $(COPROCESSOR_MAKEFILE_DIR) -f $(COPROCESSOR_MAKEFILE) --no-print-directory
	@make REBUILD_STAGE_3 -C $(COPROCESSOR_MAKEFILE_DIR) -f $(COPROCESSOR_MAKEFILE) --no-print-directory
#	@false
endif
	@-echo +++ Building ESP32-S3 baremetal image
	@command -v $(ESPTOOL) >/dev/null 2>&1 || pip install $(ESPTOOL)
	@git log -n 1 --decorate-refs=refs/heads/ --pretty=format:"+++ Git branch: %D (%h)" 2>/dev/null || true
	@git log -n 1 --clear-decorations 2> /dev/null > /dev/null || true
	@echo +++ info: "$(shell $(CC) -v 2>&1 | tail -n 1)"
	@echo +++ info: "$(shell make -v 2>&1 | head -n 1)"
	@echo +++ info: "$(shell $(PYTHON) --version 2>&1 | head -n 1)"
	@echo +++ info: "$(shell $(ESPTOOL) version 2>&1 | head -n 1)"
	@$(if $(shell test -d $(OBJ_DIR) && echo yes),,mkdir -p $(subst \,/,$(OBJ_DIR)))

.PHONY : POST_BUILD
POST_BUILD:
	@-echo +++ End


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

$(OBJ_DIR)/%.o : %.S
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@-$(CC) $(OPT) $(ARCH) $(DEFS) $(addprefix -I, $(INC_FILES)) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
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
	@-echo +++ link: $(subst \,/,$@)
ifeq ($(FORMAT_LINKER_ERR), )
	@$(LD) $(LOPS) $(FILES_O) $(EXTERNAL_LIB) -o $(OUTPUT_DIR)/$(PRJ_NAME).elf
else
	@$(LD) $(LOPS) $(FILES_O) $(EXTERNAL_LIB) -o $(OUTPUT_DIR)/$(PRJ_NAME).elf 2> $(OBJ_DIR)/linker.err || true
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
	@$(ESPTOOL) --chip esp32s3 elf2image --flash_mode dio --flash_freq 80m --flash_size 2MB --min-rev-full 0 --max-rev-full 99 -o $(OUTPUT_DIR)/$(PRJ_NAME).bin $(OUTPUT_DIR)/$(PRJ_NAME).elf 2>&1 >/dev/null
#	@$(ESPTOOL) image_info --version 2 $(OUTPUT_DIR)/$(PRJ_NAME).bin
#	@$(ESPTOOL) --chip esp32s3 erase_flash 
	@-echo +++ flash: Flashing the binary to the QSPI Flash Memory ...
	@$(ESPTOOL) --chip esp32s3 write_flash --flash_mode dio --flash_freq 80m --flash_size 2MB  0 $(OUTPUT_DIR)/$(PRJ_NAME).bin  >/dev/null

ifneq ($(OS_SIZE_SCRIPT), )
	@-echo  
	@$(PYTHON) $(OS_SIZE_SCRIPT) $(OUTPUT_DIR)/$(PRJ_NAME).map --sections .text .data .bss
	@-echo  
endif
