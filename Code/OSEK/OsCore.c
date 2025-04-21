// *****************************************************************************************************************
// Filename    : OsCore.c
// 
// OS          : OSEK 2.2.3  
// 
// CC          : ECC1/BCC1
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : Operating system services implementation
// 
// *****************************************************************************************************************

//------------------------------------------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------------------------------------------
#include"OsAPIs.h"
#include"OsInternal.h"
#include"OsTcb.h"
#include"OsAsm.h"
#include"OsHwPltfm.h"

//------------------------------------------------------------------------------------------------------------------
// Static function prototypes
//------------------------------------------------------------------------------------------------------------------
static void osInitTimer(void);
static void osStartTimer(void);
static void osIdleLoop(void);

//=============================================================================
// Globals
//=============================================================================
#if (OS_NUMBER_OF_CORES > 1)
  static uint32 osMulticoreLock  = 0;
  static volatile uint32 osMulticoreSyncVal = 0;

  #define OS_MULTICORE_SYNC_MASK   (uint32)((1ul << OS_NUMBER_OF_CORES) - 1)
#endif

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_StartOS
///
/// \descr  The user can call this system service to start the operating system in a specific mode
///
/// \param  OsAppModeType Mode
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_StartOS(OsAppModeType Mode)
{
  (void) Mode;

  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  if(TRUE == osIsInterruptContext())
  {
    for(;;);
  }
  
  if(OCB_Cfg[osActiveCore]->OsNumberOfTasks > 0)
  {
    /* INIT TCBs */
    for(uint32 tcbIdx = 0; tcbIdx < OCB_Cfg[osActiveCore]->OsNumberOfTasks; tcbIdx++)
    {
      /* Init all stacks with the magic marker */
      const uint32 stack_size = (OCB_Cfg[osActiveCore]->pTcb[tcbIdx]->pstack_top - OCB_Cfg[osActiveCore]->pTcb[tcbIdx]->pstack_bot + sizeof(uint32)) / sizeof(uint32);

      for(uint32 offset = 0; offset < stack_size; offset++)
      {
        ((volatile uint32*)OCB_Cfg[osActiveCore]->pTcb[tcbIdx]->pstack_bot)[offset] = (uint32)OS_STACK_MAGIC_MARKER;
      }
      
      /* Set default tasks priorities */
      OCB_Cfg[osActiveCore]->pTcb[tcbIdx]->Prio = OCB_Cfg[osActiveCore]->pTcb[tcbIdx]->FixedPrio;
    }
    
    /* Start all autostart task */
    for(uint32 tcbIdx = 0; tcbIdx < OCB_Cfg[osActiveCore]->OsNumberOfTasks; tcbIdx++)
    {
      if(OCB_Cfg[osActiveCore]->pTcb[tcbIdx]->Autostart == OS_AUTOSTART && OCB_Cfg[osActiveCore]->pTcb[tcbIdx]->TaskStatus == SUSPENDED)
      {
        /* Switch to PRE_READY state */
        OCB_Cfg[osActiveCore]->pTcb[tcbIdx]->TaskStatus = PRE_READY;

        /* set the current task's ready bit */
        osSetTaskPrioReady(OCB_Cfg[osActiveCore]->pTcb[tcbIdx]->Prio);
        
        /* Update the number of multiple activation */
        if(OCB_Cfg[osActiveCore]->pTcb[tcbIdx]->TaskType == BASIC)
        {
          OCB_Cfg[osActiveCore]->pTcb[tcbIdx]->MultipleActivation++;
        }
      }
    }

    /* Start all relative autostart alarms */
    for(uint32 alarmIdx = 0; alarmIdx < OCB_Cfg[osActiveCore]->OsNumberOfAlarms; alarmIdx++)
    {
      if(OCB_Cfg[osActiveCore]->pAlarm[alarmIdx]->AutoStart == ALARM_AUTOSTART)
      {
        OS_SetRelAlarm((OsAlarmType)alarmIdx,OCB_Cfg[osActiveCore]->pAlarm[alarmIdx]->InitTicks,OCB_Cfg[osActiveCore]->pAlarm[alarmIdx]->InitCycles);
      }
    }

    /* Call Scheduler */
    (void)osSchedule();

    /* Init Interrupts */
    osInitInterrupts();

    /* Synchronize all cores */
    osMulticoreSync();

    /* Call startup hook function */
    #ifdef OS_STARTUPHOOK
    (OCB_Cfg[osActiveCore]->OsStartupHook)();
    #endif

   /* Init system tick timer */
    osInitTimer();

    /* Start system tick timer */
    osStartTimer();

    /* Enable the interrupts */
    ENABLE_INTERRUPTS();
    
    /* Set the OS running flag */
    OCB_Cfg[osActiveCore]->OsIsRunning = OS_TRUE;

    /* Call the dispatcher */
    osDispatch();
  }
  
  /* Infinite loop */
  osIdleLoop();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osInitTimer
///
/// \descr  Init the HW timer used as system tick timer
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
static void osInitTimer(void)
{
  osHwTimerInit();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osStartTimer
///
/// \descr  Start the HW timer used as system tick timer
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
static void osStartTimer(void)
{
  osHwTimerStart();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osReloadTimer
///
/// \descr  Reload the HW timer used as system tick timer
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
static void osReloadTimer(void)
{
  osHwTimerReload();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osDispatcher
///
/// \descr  Context switch engine
///
/// \param  uint32 StackPtr: Current stack pointer of the active task
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
uint32 osDispatcher(uint32 StackPtr)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  /* Save the current stack pointer of the running task before switching the context */
  if(OCB_Cfg[osActiveCore]->OsCurrentTaskId < OCB_Cfg[osActiveCore]->OsNumberOfTasks)
  {
    OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->pCurrentStackPointer = StackPtr;

    /* Check the stack pointer against stack overflow */
    if( (!(StackPtr <(uint32)(OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->pstack_top) &&
           StackPtr >= (uint32)(OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->pstack_bot)))  ||
       (*(uint32*)(OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->pstack_bot) != OS_STACK_MAGIC_MARKER)
      )
    {
      /* Stack overflow */
      osInternalError(E_OS_STACKFAULT);
    }
  }
  else
  {
    /* save the system stack */
    OCB_Cfg[osActiveCore]->OsCurrentSystemStackPtr = StackPtr;
  }

  /* Set the new current task */
  OCB_Cfg[osActiveCore]->OsCurrentTaskId = OCB_Cfg[osActiveCore]->OsHighPrioReadyTaskId;
  
  if(OCB_Cfg[osActiveCore]->OsCurrentTaskId < OCB_Cfg[osActiveCore]->OsNumberOfTasks)
  {  
    /* check if we need to create a new stack frame for the new task */
    if(OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->TaskStatus == PRE_READY)
    {

      /* Update the current task state */
      OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->TaskStatus = RUNNING;

      /* Clear the current task's ready bit */
      osClearTaskPrioReady(OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->Prio);
      
      /* Call osPreTaskHook */
      #ifdef OS_PRETASKHOOK
      (OCB_Cfg[osActiveCore]->OsPreTaskHook)();
      #endif
      
      /* Create Stack Frame for the 1st execution */
      const uint32 NewStackFramePtr = OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->pstack_top;
      const pFunc  NewTaskPtr       = OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->Function;

      /* Save the new stack ptr */
      OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->pCurrentStackPointer = NewStackFramePtr;

      /* Start the new task */
      osStartNewTask(NewStackFramePtr, NewTaskPtr);

    }
    else if(OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->TaskStatus == READY)
    {
      OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->TaskStatus = RUNNING;

      /* Clear the current task's ready bit */
      osClearTaskPrioReady(OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->Prio);
      
      /* Call osPreTaskHook */
      #ifdef OS_PRETASKHOOK
      (OCB_Cfg[osActiveCore]->OsPreTaskHook)();
      #endif
    }
  }
  else
  {
    /* there is no ready task in the system 
       The OS will stay in endless loop until
       an event will be occurred */
    return(OCB_Cfg[osActiveCore]->OsCurrentSystemStackPtr);
  }
  return(OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->pCurrentStackPointer);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  Interrupt service routine
///
/// \descr  ISR of system tick interrupt
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
ISR(SysTickTimer)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  OCB_Cfg[osActiveCore]->OsSysTickCounter++;
  osAlarmsManagement();
  osReloadTimer();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osStoreStackPointer
///
/// \descr  Store the current stack pointer in case of category 2 interrupt
///
/// \param  uint32 StackPtrValue: Current stack pointer
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void osStoreStackPointer(uint32 StackPtrValue)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  /* get the interrupt nesting level */
  const uint32 OsInterruptNestingDepth = osGetIntNestingLevel();

  /* save the interrupt nesting level stack pointer */
  OCB_Cfg[osActiveCore]->pInt->OsIntNestSavedStackPointer[OsInterruptNestingDepth - 1u] = StackPtrValue;

  /* save the interrupt nesting level priority */
  OCB_Cfg[osActiveCore]->pInt->OsIntNestSavedPrioLevel[OsInterruptNestingDepth - 1u] = osGetInterruptPriorityMask();

  /* store the preempted task context only in nested level 1,
     the other nesting interrupts will use the current stack */
  if(OsInterruptNestingDepth == 1u)
  {
    /* set the Cat2 interrupt flag */
    OCB_Cfg[osActiveCore]->OsCat2InterruptLevel = 1;

    if(OCB_Cfg[osActiveCore]->OsCurrentTaskId < OCB_Cfg[osActiveCore]->OsNumberOfTasks)
    {
      /* preempted from task level */
      OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->pCurrentStackPointer = StackPtrValue; /* current task stack will be used */
    }
    else
    {
      /* preempted from OS level */
      OCB_Cfg[osActiveCore]->OsCurrentSystemStackPtr = StackPtrValue; /* system stack will be used */
    }
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osGetSavedStackPointer
///
/// \descr  return the saved stack pointer in case of category 2 interrupt
///
/// \param  void
///
/// \return uint32 : saved stack pointer
//------------------------------------------------------------------------------------------------------------------
uint32 osGetSavedStackPointer(void)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  /* get the interrupt nesting level */
  const uint32 OsInterruptNestingDepth = osGetIntNestingLevel();

  if(OsInterruptNestingDepth == 1u)
  {
    if(OCB_Cfg[osActiveCore]->OsCurrentTaskId < OCB_Cfg[osActiveCore]->OsNumberOfTasks)
    {
      return(OCB_Cfg[osActiveCore]->pTcb[OCB_Cfg[osActiveCore]->OsCurrentTaskId]->pCurrentStackPointer);
    }
    else
    {
      return(OCB_Cfg[osActiveCore]->OsCurrentSystemStackPtr);
    }
  }
  else
  {
    return(OCB_Cfg[osActiveCore]->pInt->OsIntNestSavedStackPointer[OsInterruptNestingDepth - 1u]);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osIntCallDispatch
///
/// \descr  Call the dispatcher to switch the context if needed after an category 2 interrupt 
///
/// \param  uint32 StackPtr: Current stack pointer
///
/// \return uint32 : The new stack pointer after switching the context otherwise the last saved stack pointer
//------------------------------------------------------------------------------------------------------------------
uint32 osIntCallDispatch(uint32 StackPtr)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  /* get the interrupt nesting level */
  const uint32 OsInterruptNestingDepth = osGetIntNestingLevel();

  /* restore the interrupt nesting priority level */
  osSetInterruptPriorityMask(OCB_Cfg[osActiveCore]->pInt->OsIntNestSavedPrioLevel[OsInterruptNestingDepth - 1u]);

  if(OsInterruptNestingDepth == 1u)
  {
    /* Disable the Hw nesting */
    osDisableIntNesting();

    /* Reset the flag */
    OCB_Cfg[osActiveCore]->OsCat2InterruptLevel = 0;
    
    if(OCB_Cfg[osActiveCore]->OsIntCallDispatcher == 1)
    {
      /* The internal system state is changed by the ISR cat2 (an Event is triggered)
         at this point the OS will switch to the new context (target task context) */

      /* Reset the flag */
      OCB_Cfg[osActiveCore]->OsIntCallDispatcher = 0;

      /* Call the Dispatcher to execute the context switch process */
      return(osDispatcher(StackPtr));
    }
    else
    {
      /* The system behavior is not changed by the ISR cat2
         The cpu will restore the context saved by the interrupt */
      return(StackPtr);
    }
  }
  else
  {
    /* in nesting interrupt behavior
       restore the stack pointer from the saved context */
    return(StackPtr);
  }
}  

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_GetActiveApplicationMode
///
/// \descr  This service returns the current application mode.
///
/// \param  void
///
/// \return OsAppModeType : The application mode
//------------------------------------------------------------------------------------------------------------------
OsAppModeType OS_GetActiveApplicationMode(void)
{
  return(APP_MODE);
}  

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_ShutdownOS
///
/// \descr  The user can call this system service to abort the overall system (e.g. emergency off). 
///         The operating system also calls this function internally, 
///         if it has reached an undefined internal state and is no longer ready to run.
///
/// \param  OsStatusType Error: Error causing the shutdown of the OS
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_ShutdownOS(OsStatusType Error)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

#ifdef OS_SHUTDOWNHOOK
  (OCB_Cfg[osActiveCore]->OsShutdownHook)(Error);
#else
  (void)Error;
#endif
  
  DISABLE_INTERRUPTS();
  
  /* Kill all tasks */
  for(uint32 tcbIdx = 0; tcbIdx < OCB_Cfg[osActiveCore]->OsNumberOfTasks; tcbIdx++)
  {
    OCB_Cfg[osActiveCore]->pTcb[tcbIdx]->TaskStatus = SUSPENDED;
  }
  for(;;);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  osIdleLoop
///
/// \descr  This function is called by the dispatcher when no ready task is found.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
static void osIdleLoop(void)
{
  /* When no ready task is found the CPU will be waiting here for any event */
  for(;;);
}

//-----------------------------------------------------------------------------
/// \brief  OsGetSystemTicks
///
/// \descr  Get the system tick counter
///
/// \param  void
///
/// \return uint64 
//-----------------------------------------------------------------------------
uint64 osGetSystemTicksCounter(void)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  return(OCB_Cfg[osActiveCore]->OsSysTickCounter);
}


//-----------------------------------------------------------------------------
/// \brief  osGetSystemTicksElapsedTime
///
/// \descr  Get the system elapsed time relative to the prvTicks
///
/// \param  prvTicks : perivous system tick
///
/// \return uint64 
//-----------------------------------------------------------------------------
uint64 osGetSystemTicksElapsedTime(uint64 prvTicks)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  return((uint64)(OCB_Cfg[osActiveCore]->OsSysTickCounter - prvTicks));
}

//-----------------------------------------------------------------------------
/// \brief  osErrTaskExitWithoutTerminate
///
/// \descr  Catch abnormal exit from task function
///
/// \param  void 
///
/// \return void 
//-----------------------------------------------------------------------------
void osErrTaskExitWithoutTerminate(void)
{
  DISABLE_INTERRUPTS();
  for(;;);
}

//-----------------------------------------------------------------------------
/// \brief  osKernelError
///
/// \descr  Catch all internal OS errors
///
/// \param  err 
///
/// \return void 
//-----------------------------------------------------------------------------
void osKernelError(OsStatusType err)
{
  DISABLE_INTERRUPTS();
  #ifdef OS_ERRORHOOK
    const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());
    (OCB_Cfg[osActiveCore]->OsErrorHook)(err);
  #else
    (void)err;
  #endif
    for(;;);
}

//-----------------------------------------------------------------------------
/// \brief  osGetMaximumStackUsage
///
/// \descr  Calculate the maximum stack usage during runtime
///
/// \param  LocalTaskId 
///
/// \return uint32 
//-----------------------------------------------------------------------------
uint32 osGetMaximumStackUsage(uint32 LocalTaskId)
{
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());
  uint32* pStack = (uint32*)NULL;
  uint32 Bottom = OCB_Cfg[osActiveCore]->pTcb[LocalTaskId]->pstack_bot;
  uint32 Top    = OCB_Cfg[osActiveCore]->pTcb[LocalTaskId]->pstack_top;

  #define ALIGN4_ADDRESS(x) ((x) & (0xFFFFFFFCUL))

  while(sizeof(uint32) != (Top - Bottom))
  {
    pStack = (uint32*)(ALIGN4_ADDRESS(Bottom + ((Top - Bottom + sizeof(uint32)) / 2)));

    if(*pStack != OS_STACK_MAGIC_MARKER)
    {
       Top = (uint32)pStack;
    }
    else
    {
      Bottom = (uint32)pStack;
    }
  }
  return((uint32)(OCB_Cfg[osActiveCore]->pTcb[LocalTaskId]->pstack_top) - Top + sizeof(uint32));
}

//-----------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  
///
/// \return 
//-----------------------------------------------------------------------------
uint8_t osGetLogicalCoreId(uint8_t PhysicalCoreId)
{
  const uint8_t size = sizeof(osLogicalToPhysicalCoreIdMapping)/sizeof(uint8_t);

  for(uint8_t core_id = 0; core_id < size; core_id++)
  {
    if(PhysicalCoreId == osLogicalToPhysicalCoreIdMapping[core_id])
    {
        return(core_id);
    }
  }

  /* System Error : the physical core is not assigned to any logical core */
  osKernelError(E_OS_MULTICORE_NOT_ASSIGNED);
  return((uint8_t)-1);
}

//-----------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  
///
/// \return 
//-----------------------------------------------------------------------------
osObjectCoreAsgn_t osGetLocalAlarmAssignment(uint32_t SystemAlarmId)
{
  if(SystemAlarmId <= OS_NUMBER_OF_ALARMS)
  {
    return(osAlarmCoreAsgnLookupTable[SystemAlarmId]);
  }
  else
  {
    osKernelError(E_OS_ID);
    return((osObjectCoreAsgn_t){0});
  }
}

//-----------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  
///
/// \return 
//-----------------------------------------------------------------------------
osObjectCoreAsgn_t osGetLocalTaskAssignment(uint32_t SystemTaskId)
{
  if(SystemTaskId <= OS_NUMBER_OF_TASKS)
  {
    return(osTaskCoreAsgnLookupTable[SystemTaskId]);
  }
  else
  {
    osKernelError(E_OS_ID);
    return((osObjectCoreAsgn_t){0});
  }
}

//-----------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  
///
/// \return 
//-----------------------------------------------------------------------------
osObjectCoreAsgn_t osGetLocalResourceAssignment(uint32_t SystemResourceId)
{
  if(SystemResourceId <= OS_NUMBER_OF_RESOURCES)
  {
    return(osResourceCoreAsgnLookupTable[SystemResourceId]);
  }
  else
  {
    osKernelError(E_OS_ID);
    return((osObjectCoreAsgn_t){0});
  }
}
//-----------------------------------------------------------------------------
/// \brief  
///
/// \descr  
///
/// \param  
///
/// \return 
//-----------------------------------------------------------------------------
void osMulticoreSync(void)
{
#if (OS_NUMBER_OF_CORES > 1)
  const uint32 osActiveCore = osGetLogicalCoreId(osGetCoreId());

  /* aquire the multicore lock */
  osHwAcquireSpinLock(&osMulticoreLock);

  osMulticoreSyncVal |= (1UL << osActiveCore);

  /* release the multicore lock */
  osHwReleaseSpinLock(&osMulticoreLock);

  while(osMulticoreSyncVal != OS_MULTICORE_SYNC_MASK);
#endif
}
