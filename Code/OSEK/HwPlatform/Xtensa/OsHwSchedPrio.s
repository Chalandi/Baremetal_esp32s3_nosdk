/******************************************************************************************
  Filename    : OsHwSchedPrio.s

  Core        : Xtensa Lx7

  Author      : Chalandi Amine

  Owner       : Chalandi Amine

  Date        : 09.03.2025

  Description : This function is searching for the highest ready priority

******************************************************************************************/

.file "OsHwSchedPrio.s"

/*******************************************************************************************
  \brief  uint32_t osHwSearchForHighPrio(void)
  
  \param  
  
  \return 
********************************************************************************************/

.section ".text", "ax"
.align 4
.globl  osHwSearchForHighPrio
.type osHwSearchForHighPrio, %function
.extern OsSchedPrioTypeSize
.extern osGetOsHwSchedPrioRegAddress
.extern __os_sw_clz

.equ osXtensaStackAlignmentBoundaries, 16
.equ osNoReadyTaskWasFound, -1

osHwSearchForHighPrio:
                       addi sp, sp, -osXtensaStackAlignmentBoundaries
                       s32i.n a0, sp, 0
                       movi a2, OsSchedPrioTypeSize
                       l32i a5, a2, 0
                       addi a5, a5, -4
                       call0 osGetOsHwSchedPrioRegAddress
                       mov.n a3, a2
                       movi.n a2, 31
                       add.n a3, a3, a5
.L_ContinueSearching:
                       l32i a6, a3, 0
                       s32i.n a2, sp, 4
                       mov.n a2, a6
                       call0 __os_sw_clz
                       mov.n a4, a2
                       l32i.n a2, sp, 4
                       bnei a4, 32, .L_ReadyTaskFound
                       addi a3, a3, -4
                       addi a5, a5, -4
                       bgez a5, .L_ContinueSearching
                       movi a2, osNoReadyTaskWasFound
                       l32i.n a0, sp, 0
                       addi sp, sp, osXtensaStackAlignmentBoundaries
                       ret
.L_ReadyTaskFound:
                       srli a5, a5, 2
                       slli a5, a5, 5
                       sub a2, a2, a4
                       add a2, a2, a5
                       l32i.n a0, sp, 0
                       addi sp, sp, osXtensaStackAlignmentBoundaries
                       ret

.size osHwSearchForHighPrio, .-osHwSearchForHighPrio

/*******************************************************************************************
  \brief  SW implementation of counting leading zeros instruction (clz)
  
  \decl   uint32_t __os_sw_clz(uint32_t n)
  
  \param  a 32-bit number

  \return number of leading zeros (counting from left to right)
********************************************************************************************/

.section ".text", "ax"
.global __os_sw_clz
.type __os_sw_clz, @function
.align 4

__os_sw_clz:
    movi.n a10, 32
    beqz a2, .L_end
    movi.n a8, 32
    movi.n a10, 0
    movi.n a9, -1

.L_continue:
    srli a8, a8, 1
    beqz a8, .L_end
    ssl  a8
    sll  a9, a9
    and  a11, a2, a9
    bnez a11, .L_continue
    ssl  a8
    sll  a2, a2
    add a10, a10, a8
    j    .L_continue

.L_end:
    mov.n a2, a10
    ret

.size __os_sw_clz, . - __os_sw_clz


/*  CALL0 ABI:
--------------------------------------------------------------------
|  Register   |  Usage                            |  Preserver     |
--------------------------------------------------------------------
|  a0         |  Return Address                   |  caller-saved  |
|  a1 (sp)    |  Stack Pointer                    | *callee-saved  |
|  a2 – a7    |  Function Arguments (6 registers) |  caller-saved  |
|  a8 – a11   |  Temporary (4 registers)          |  caller-saved  |
|  a12 – a15  |  Temporary (4 registers)          | *callee-saved  |
|  a15        |  Stack-Frame Pointer (optional)   | *callee-saved  |
--------------------------------------------------------------------
*/
