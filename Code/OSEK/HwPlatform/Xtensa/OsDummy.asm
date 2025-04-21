
#define OS_ASM_SECTION(name)         .section name
#define OS_ASM_ALIGN(alignment)      .align alignment
#define OS_ASM_GLOBL(name)           .globl  name
#define OS_ASM_FUNC(name)            .type   name, @function
#define OS_ASM_OBJECT(name)          .type   name, @object
#define OS_ASM_LABEL(name)           name:
#define OS_ASM_SIZE(name)            .size name, .-#name


#define __OS_TYPES_H__
#include"OsTypes.h"

/*-------------------------------------------------------------------------------------------------------------------------*/
/* \brief  OsDummyAsmFunc                                                                                                  */
/*                                                                                                                         */
/* \descr  A dummy assembly function to demostrate the capability of building a file with mixed languages (C and assembly) */
/*                                                                                                                         */
/* \param  void                                                                                                            */
/*                                                                                                                         */
/* \return void                                                                                                            */
/*-------------------------------------------------------------------------------------------------------------------------*/
OS_ASM_SECTION(".text")
OS_ASM_ALIGN(4)
OS_ASM_GLOBL(OsDummyAsmFunc)
OS_ASM_FUNC(OsDummyAsmFunc)
OS_ASM_LABEL(OsDummyAsmFunc)
                              nop
                              ret
OS_ASM_SIZE(OsDummyAsmFunc)
