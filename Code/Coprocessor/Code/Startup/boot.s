
/*******************************************************************************************
  \brief  
  
  \param  
  
  \return 
********************************************************************************************/
.section .boot
.type _start, @function
.align 4
.extern __STACK_TOP
.extern Startup_Init
.globl _start

_start:
        /* setup the stack pointer */
        la sp, __STACK_TOP

        /* setup C/C++ runtime environment */
        j  Startup_Init

.size _start, .-_start
