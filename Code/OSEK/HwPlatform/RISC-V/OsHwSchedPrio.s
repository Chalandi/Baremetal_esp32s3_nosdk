/******************************************************************************************
  Filename    : OsHwSchedPrio.s

  Core        : RISC-V RV32IMAC

  Author      : Chalandi Amine

  Owner       : Chalandi Amine

  Date        : 13.01.2023

  Description : This function is searching for the highest ready task

******************************************************************************************/

.file "OsHwSchedPrio.s"

/*******************************************************************************************
  \brief  
  
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

.equ osNoReadyTaskWasFound, 0xA0A0F5F5UL

osHwSearchForHighPrio:
                       addi sp, sp, -4
                       sw ra, 0(sp)
                       la a0, OsSchedPrioTypeSize
                       lw a3, 0(a0)
                       addi a3, a3, -4
                       jal osGetOsHwSchedPrioRegAddress
                       add a1, a0, a3
.L_ContinueSearching:
                       lw a0, 0(a1)
/*
#ifdef(OS_USE_SW_CLZ)
                       addi sp, sp, -4*4
                       sw a1, 0(sp)
                       sw a2, 4(sp)
                       sw a3, 8(sp)
                       sw a4, 12(sp)
                       jal __os_sw_clz
                       lw a1, 0(sp)
                       lw a2, 4(sp)
                       lw a3, 8(sp)
                       lw a4, 12(sp)
                       addi sp, sp, 4*4
                       mv a2, a0
#else
*/
                       clz a2,a0
/*#endif*/
                       li a4, 32
                       bne a2, a4, .L_ReadyTaskFound
                       addi a1, a1, -4
                       addi a3, a3, -4
                       bgeu a3, zero, .L_ContinueSearching
                       la a0, osNoReadyTaskWasFound
                       lw ra, 0(sp)
                       addi sp, sp, 4
                       ret
.L_ReadyTaskFound:
                       li a0, 31
                       srli a3, a3, 2
                       slli a3, a3, 5
                       sub a0, a0, a2
                       add a0, a0, a3
                       lw ra, 0(sp)
                       addi sp, sp, 4
                       ret

.size osHwSearchForHighPrio, .-osHwSearchForHighPrio

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/

.section ".text", "ax"
.globl __os_sw_clz
.type __os_sw_clz, @function
.align 4

__os_sw_clz:
             li a3, 32
             beq a0, zero, .L_end
             li a1, 32
             li a3, 0
             mv a2, a3
             not a2,a2
.L_continue:
             srli a1, a1, 1
             beq a1, zero, .L_end
             sll a2, a2, a1
             and a4, a0, a2
             bne a4, zero, .L_continue
             sll a0, a0, a1
             add a3, a3, a1
             j .L_continue
.L_end:
             mv a0, a3
             ret

.size __os_sw_clz, .-__os_sw_clz

