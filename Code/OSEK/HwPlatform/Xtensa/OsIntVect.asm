
#define OS_ASM_SECTION(name)         .section name
#define OS_ASM_ALIGN(alignment)      .align alignment
#define OS_ASM_GLOBL(name)           .globl  name
#define OS_ASM_FUNC(name)            .type   name, @function
#define OS_ASM_OBJECT(name)            .type   name, @object
#define OS_ASM_LABEL(name)           name:
#define OS_ASM_SIZE(name)            .size name, .-#name


#define __OS_TYPES_H__
#include"OsTypes.h"

/*-------------------------------------------------------------------------------------------------------------------------*/
/* \brief  OS Interrupt Vector Table                                                                                       */
/*                                                                                                                         */
/* \descr  Lookup interrupt vector table type                                                                              */
/*                                                                                                                         */
/* \param  void                                                                                                            */
/*                                                                                                                         */
/* \return void                                                                                                            */
/*-------------------------------------------------------------------------------------------------------------------------*/
OS_ASM_SECTION(".rodata")
OS_ASM_ALIGN(4)
OS_ASM_GLOBL(osIntVectTable)
OS_ASM_OBJECT(osIntVectTable)
OS_ASM_LABEL(osIntVectTable)

#define OS_GEN_LOOKUP_IVT
#include "OsGenCfg.h"


OS_ASM_SIZE(osIntVectTable)
