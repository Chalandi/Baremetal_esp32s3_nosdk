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
.section  .vector,"ax"
.global _vector_table
.type _vector_table, @function
.align 1024

_vector_table:

        .org _vector_table + 0x00
        WindowVectors:
                        j .

        .org _vector_table + 0x180
        Level2InterruptVector:
                        j .

        .org _vector_table + 0x1c0
        Level3InterruptVector:
                        j irq6_timer1

        .org _vector_table + 0x200
        Level4InterruptVector:
                        j .

        .org _vector_table + 0x240
        Level5InterruptVector:
                        j .

        .org _vector_table + 0x280
        DebugExceptionVector:
                        j .

        .org _vector_table + 0x2c0
        NMIExceptionVector:
                        j .

        .org _vector_table + 0x300
        Level1KernalInterruptVector:
                        j .

        .org _vector_table + 0x340
        Level1UserInterruptVector:
                        j .

        .org _vector_table + 0x3C0
        DoubleExceptionVector:
                        j .

        .org _vector_table + 0x400
        InvalidExceptionVector:
                        j .

            .extern blink_led_c0

irq6_timer1:
             SaveCpuContext
             call0 blink_led
             RestoreCpuContext
             rfi 3


.size _vector_table, .-_vector_table

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
a2 � a7    |  Function Arguments             |
a8 � a11   |  Temporary                      |  caller-saved
a12 � a15  |                                 |  callee-saved
a15        |  Stack-Frame Pointer (optional) |

*/