/******************************************************************************************
  Filename    : IntVectTable.s
  
  Core        : Xtensa LX7
  
  MCU         : ESP32-S3
    
  Author      : Chalandi Amine
 
  Owner       : Chalandi Amine
  
  Date        : 22.02.2025
  
  Description : Interrupt vector tables for ESP32-S3
  
******************************************************************************************/

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.macro SaveCpuContext
    addi sp, sp, -4*15
    s32i.n a0,  sp, 0*4
    s32i.n a2,  sp, 1*4
    s32i.n a3,  sp, 2*4
    s32i.n a4,  sp, 3*4
    s32i.n a5,  sp, 4*4
    s32i.n a6,  sp, 5*4
    s32i.n a7,  sp, 6*4
    s32i.n a8,  sp, 7*4
    s32i.n a9,  sp, 8*4
    s32i.n a10,  sp, 9*4
    s32i.n a11,  sp, 10*4
    s32i.n a12,  sp, 11*4
    s32i.n a13,  sp, 12*4
    s32i.n a14,  sp, 13*4
    s32i.n a15,  sp, 14*4
.endm

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.macro RestoreCpuContext
    l32i.n a0,  sp, 0*4
    l32i.n a2,  sp, 1*4
    l32i.n a3,  sp, 2*4
    l32i.n a4,  sp, 3*4
    l32i.n a5,  sp, 4*4
    l32i.n a6,  sp, 5*4
    l32i.n a7,  sp, 6*4
    l32i.n a8,  sp, 7*4
    l32i.n a9,  sp, 8*4
    l32i.n a10,  sp, 9*4
    l32i.n a11,  sp, 10*4
    l32i.n a12,  sp, 11*4
    l32i.n a13,  sp, 12*4
    l32i.n a14,  sp, 13*4
    l32i.n a15,  sp, 14*4
    addi sp, sp, 4*15
.endm

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.macro call_isr isr_name
    SaveCpuContext
    rsr a2, interrupt
    call0 \isr_name
    RestoreCpuContext
.endm

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section  .vector,"ax"
.global _vector_table
.type _vector_table, @function
.align 1024
.extern Level1KernalInterruptVectorHandler
.extern Level1UserInterruptVectorHandler
.extern Level2InterruptVectorHandler
.extern Level3InterruptVectorHandler
.extern Level4InterruptVectorHandler
.extern Level5InterruptVectorHandler

_vector_table:

        .org _vector_table + 0x00
        WindowVectors:
                        j .

        .org _vector_table + 0x180
        Level2InterruptVector:
                        j Level2InterruptVectorHandler

        .org _vector_table + 0x1c0
        Level3InterruptVector:
                        j Level3InterruptVectorHandler

        .org _vector_table + 0x200
        Level4InterruptVector:
                        j Level4InterruptVectorHandler

        .org _vector_table + 0x240
        Level5InterruptVector:
                        j Level5InterruptVectorHandler

        .org _vector_table + 0x280
        DebugExceptionVector:
                        j .

        .org _vector_table + 0x2c0
        NMIExceptionVector:
                        j .

        .org _vector_table + 0x300
        Level1KernalInterruptVector:
                        j Level1KernalInterruptVectorHandler

        .org _vector_table + 0x340
        Level1UserInterruptVector:
                        j Level1UserInterruptVectorHandler

        .org _vector_table + 0x3C0
        DoubleExceptionVector:
                        j .

        .org _vector_table + 0x400
        InvalidExceptionVector:
                        j .


.size _vector_table, .-_vector_table

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section  .text,"ax"
.global _vector_handlers
.type _vector_handlers, @function
.align 4
.extern Isr_Level1KernelInterrupt
.extern Isr_Level1UserInterrupt
.extern Isr_Level2Interrupt
.extern Isr_Level3Interrupt
.extern Isr_Level4Interrupt
.extern Isr_Level5Interrupt

_vector_handlers:

        Level2InterruptVectorHandler:
                        call_isr Isr_Level2Interrupt
                        rfi 2

        Level3InterruptVectorHandler:
                        call_isr Isr_Level3Interrupt
                        rfi 3

        Level4InterruptVectorHandler:
                        call_isr Isr_Level4Interrupt
                        rfi 4

        Level5InterruptVectorHandler:
                        call_isr Isr_Level5Interrupt
                        rfi 5

        Level1KernalInterruptVectorHandler:
                        call_isr Isr_Level1KernelInterrupt
                        rfe

        Level1UserInterruptVectorHandler:
                        call_isr Isr_Level1UserInterrupt
                        rfe

.size _vector_handlers, .-_vector_handlers

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section  .text,"ax"
.type enable_irq, @function
.align 4
.global enable_irq

enable_irq:
           wsr a2, INTENABLE
           ret

.size enable_irq, .-enable_irq

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section  .text,"ax"
.type disable_irq, @function
.align 4
.global disable_irq

disable_irq:
           wsr a0, intclear
           ret

.size disable_irq, .-disable_irq

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section  .text,"ax"
.type set_cpu_private_timer, @function
.align 4
.global set_cpu_private_timer

set_cpu_private_timer:
                       beqi a2, 0, .L_timer0
                       beqi a2, 1, .L_timer1
                       beqi a2, 2, .L_timer2
                       ret
.L_timer0:
                       rsr  a11, ccount
                       esync
                       add  a11, a11, a3
                       wsr  a11, ccompare0
                       esync
                       ret
.L_timer1:
                       rsr  a11, ccount
                       esync
                       add  a11, a11, a3
                       wsr  a11, ccompare1
                       esync
                       ret
.L_timer2:
                       rsr  a11, ccount
                       esync
                       add  a11, a11, a3
                       wsr  a11, ccompare2
                       esync
                       ret

.size set_cpu_private_timer, .-set_cpu_private_timer

/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section  .vector,"ax"
.global _dummy_vector_table
.type _dummy_vector_table, @function
.align 1024

_dummy_vector_table:

    .rept 20   // this loop create the full interrupt vector table (20 vectors of 0x40 each)
      .rept 12 // this loop create 0x40 bytes (5x12 + 4 = size of one interrupt vector)
        j .    // 3-byte op
        nop    // 2-byte op
      .endr
      nop      // 2-byte op
      nop      // 2-byte op
    .endr
.size _dummy_vector_table, .-_dummy_vector_table



  // software interrupt
  //movi a10, 0x80;
  //wsr.intset a10
  
  //RSIL --> read and set prio level



/**  CALL0 ABI:

Register   |  Use                            | Preserver
--------------------------------------------------------------
a0         |  Return Address                 |  caller-saved
a1 (sp)    |  Stack Pointer                  |  callee-saved
a2 – a7    |  Function Arguments             |  caller-saved
a8 – a11   |  Temporary                      |  caller-saved
a12 – a14  |                                 |  callee-saved
a15        |  Stack-Frame Pointer (optional) |  callee-saved

*/