/******************************************************************************************
  Filename    : OsHwSchedPrio.s

  Core        : ARM Cortex®-M

  Author      : Chalandi Amine

  Owner       : Chalandi Amine

  Date        : 26.10.2020

  Description : This function is searching for the highest ready task using the HW capability

******************************************************************************************/

.file "OsHwSchedPrio.s"
.syntax unified

.thumb_func
.section ".text"
.align 4
.globl  osHwSearchForHighPrio
.type osHwSearchForHighPrio, %function
.extern OsSchedPrioTypeSize
.extern osGetOsHwSchedPrioRegAddress

.equ osNoReadyTaskWasFound, 0xA0A0F5F5UL

osHwSearchForHighPrio: 
                       push {r4-r7,lr}
                       ldr r0,=OsSchedPrioTypeSize
                       ldr r7, [r0]
                       add r7,r7, #-4
                       bl osGetOsHwSchedPrioRegAddress
                       mov r4, r0
                       mov r0, #31
                       add r4, r4, r7
__ContinueSearching:   
                       ldr r5, [r4 , #0]
                       clz r6, r5
                       cmp r6, #32
                       bne __ReadyTaskFound
                       add r4,r4, #-4
                       add r7,r7, #-4
                       cmp r7, #0
                       bpl __ContinueSearching
                       ldr r0, =osNoReadyTaskWasFound
                       pop {r4-r7,pc}
__ReadyTaskFound:       
                       lsr r7, r7, #2
                       lsl r7, r7, #5
                       sub r0, r0, r6
                       add r0, r0, r7
                       pop {r4-r7,pc}


.thumb_func
.section ".text"
.align 4
.globl  __memclr__8words_align
__memclr__8words_align:
                       push {r5-r12,lr}
                       mov r5,  #0
                       mov r6,  #0
                       mov r7,  #0
                       mov r8,  #0
                       mov r9,  #0
                       mov r10, #0
                       mov r11, #0
                       mov r12, #0
_ContinueToClear:      stm r0!, {r5-r12}
                       subs r1, r1, #32
                       bne _ContinueToClear
                       pop {r5-r12,pc}


.thumb_func
.section ".text"
.align 4
.globl  __memcpy__8words_align
__memcpy__8words_align:
                       push {r5-r12,lr}
_ContinueToCopy:       ldm r1!, {r5-r12}
                       stm r0!, {r5-r12}
                       subs r2, r2, #32
                       bne _ContinueToCopy
                       pop {r5-r12,pc}

