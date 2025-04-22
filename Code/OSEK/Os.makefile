
############################################################################################
# Source Files
############################################################################################

SRC_FILES += $(SRC_DIR)/OSEK/OsAlarm.c
SRC_FILES += $(SRC_DIR)/OSEK/OsCat2Int.c
SRC_FILES += $(SRC_DIR)/OSEK/OsCore.c
SRC_FILES += $(SRC_DIR)/OSEK/OsCrossCore.c
SRC_FILES += $(SRC_DIR)/OSEK/OsEvt.c
SRC_FILES += $(SRC_DIR)/OSEK/OsFifo.c
SRC_FILES += $(SRC_DIR)/OSEK/OsIpc.c
SRC_FILES += $(SRC_DIR)/OSEK/OsResource.c
SRC_FILES += $(SRC_DIR)/OSEK/OsSched.c
SRC_FILES += $(SRC_DIR)/OSEK/OsSpinlock.c
SRC_FILES += $(SRC_DIR)/OSEK/OsTask.c
SRC_FILES += $(SRC_DIR)/OSEK/HwPlatform/$(CORE_FAMILY)/OsAsm.s
SRC_FILES += $(SRC_DIR)/OSEK/HwPlatform/$(CORE_FAMILY)/OsHwPltfm.c
SRC_FILES += $(SRC_DIR)/OSEK/HwPlatform/$(CORE_FAMILY)/OsHwSchedPrio.s

############################################################################################
# Include Files
############################################################################################
INC_FILES += $(SRC_DIR)/OSEK
INC_FILES += $(SRC_DIR)/OSEK/HwPlatform/$(CORE_FAMILY)

############################################################################################
# Tools
############################################################################################
OS_TCB_GEN_TOOL =$(SRC_DIR)/OSEK/Generator/OilConfigGenerator.py
OS_SIZE_SCRIPT=$(SRC_DIR)/OSEK/Script/OsSize.py

############################################################################################
# Recipes
############################################################################################

# custom target to be used in the main makefile
CUSTOM_PRE_BUILD_TARGET  = OS_TCB_GEN
CUSTOM_POST_BUILD_TARGET = OS_SIZE_INFO

.PHONY : OS_TCB_GEN
OS_TCB_GEN:
	@echo +++ Generate OS Configuration
	@$(if $(shell test -d $(OS_TCB_GEN_FOLDER) && echo yes),,mkdir -p $(subst \,/,$(OS_TCB_GEN_FOLDER)))
	@$(PYTHON) $(OS_TCB_GEN_TOOL) -i $(OS_OIL_FILE) -gen $(OS_TCB_GEN_FOLDER)



.PHONY : OS_SIZE_INFO
OS_SIZE_INFO:
	@-echo
	@$(PYTHON) $(OS_SIZE_SCRIPT) $(OUTPUT_DIR)/$(PRJ_NAME).map --sections .text .data .bss
	@-echo
