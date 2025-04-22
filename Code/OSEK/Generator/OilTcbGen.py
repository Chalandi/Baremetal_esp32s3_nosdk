
import datetime
import getpass
from tabulate import tabulate

def OsConfigTcbGeneration(args, OilOs,
                                OilTasks,
                                OilEvents,
                                OilResources,
                                OilSpinlocks,
                                OilAlarms,
                                OilInterrupts,
                                OilMbx,
                                OilGenTemplate):

    OsGenTcbSourceFilePath  = args.gen_folder + "/OsTcb.c"
    OsGenTcbHeaderFilePath  = args.gen_folder + "/OsTcb.h"
    OsGenPrioHeaderFilePath = args.gen_folder + "/OsGenPrioTypes.h"

    OsGenTcbSourceFile  = open(OsGenTcbSourceFilePath, 'w', encoding='utf-8')
    OsGenTcbHeaderFile  = open(OsGenTcbHeaderFilePath, 'w', encoding='utf-8')
    OsGenPrioHeaderFile = open(OsGenPrioHeaderFilePath, 'w', encoding='utf-8')

######################################################################################################################################################################################
# generate TCB C file
######################################################################################################################################################################################
    OsGenTcbSourceFile.write("\n// **********************************************************************************************************************\n")
    OsGenTcbSourceFile.write("//                                                  OSEK OS Configuration\n")
    OsGenTcbSourceFile.write("// **********************************************************************************************************************\n")
    OsGenTcbSourceFile.write("\n")
    OsGenTcbSourceFile.write("// **********************************************************************************************************************\n")
    OsGenTcbSourceFile.write("// Filename        : OsTcb.c\n")
    OsGenTcbSourceFile.write("//\n")
    user = getpass.getuser()
    OsGenTcbSourceFile.write(f"// Author          : {user}\n")
    OsGenTcbSourceFile.write("//\n")
    OsGenTcbSourceFile.write(f"// Owner           : {user}\n")
    OsGenTcbSourceFile.write("//\n")
    OsGenTcbSourceFile.write("// Generator       : OilGenTool\n")
    OsGenTcbSourceFile.write("//\n")
    OsGenTcbSourceFile.write(f"// Oil file        : {args.oil_file}\n")
    OsGenTcbSourceFile.write("//\n")
    date = datetime.datetime.now().strftime("%d.%m.%Y")
    time = datetime.datetime.now().strftime("%H:%M:%S")
    OsGenTcbSourceFile.write(f"// Generation Time : {date} {time}\n")
    OsGenTcbSourceFile.write("//\n")
    OsGenTcbSourceFile.write(f"// Description     : Auto-generated OS Configuration file\n")
    OsGenTcbSourceFile.write("//\n")
    OsGenTcbSourceFile.write("// **********************************************************************************************************************\n")
    OsGenTcbSourceFile.write("\n")
    OsGenTcbSourceFile.write("#include \"OsTcb.h\"\n")
    OsGenTcbSourceFile.write("\n")
    OsGenTcbSourceFile.write("//=============================================================================\n")
    OsGenTcbSourceFile.write("// OS Configuration Summary\n")
    OsGenTcbSourceFile.write("//=============================================================================\n")
    OsGenTcbSourceFile.write("/*\n")
    # Display OS info
    OsGenTcbSourceFile.write("  OS:\n  "+tabulate(OilOs.OsList, headers=["Name", "Properties"], tablefmt='fancy_grid').replace("\n", "\n" + "  ")+"\n")
    OsGenTcbSourceFile.write("\n  Tasks:\n  "+tabulate(OilTasks.OsTasksList, headers=["Task", "Type", "Schedule", "Priority", "Activation", "Autostart", "Stack size", "Wait event mask", "Pinned core"], tablefmt='fancy_grid').replace("\n", "\n" + "  ")+"\n")
    OsGenTcbSourceFile.write(f"  Total number of tasks: {OilTasks.OsTaskTotalNumber}"+"\n")
    OsGenTcbSourceFile.write("\n  Task's events: "+"\n")
    OsGenTcbSourceFile.write("  " + tabulate(OilTasks.OsTaskEventL, headers=["Task", "Events"], tablefmt='fancy_grid').replace("\n", "\n" + "  ")+"\n")
    OsGenTcbSourceFile.write("\n  Task's resources: "+"\n")
    OsGenTcbSourceFile.write("  " + tabulate(OilTasks.OsTaskResourceL, headers=["Task", "Resources"], tablefmt='fancy_grid').replace("\n", "\n" + "  ")+"\n")

    # Display event info
    OsGenTcbSourceFile.write("\n  Events:\n  "+tabulate(OilEvents.EventsList, headers=["EVENT", "MASK"], tablefmt='fancy_grid').replace("\n", "\n" + "  ")+"\n")
    OsGenTcbSourceFile.write(f"  Total number of events: {OilEvents.EventsTotalNumber}"+"\n")

    # Display resources info
    OsGenTcbSourceFile.write("\n  Resources:\n  "+tabulate(OilResources.ResourcesList, headers=["Resource", "Priority ceiling", "Property", "Mask", "Pinned core"], tablefmt='fancy_grid').replace("\n", "\n" + "  ")+"\n")
    OsGenTcbSourceFile.write(f"  Total number of resources: {OilResources.ResourcesTotalNumber}"+"\n")

    # Display priority scheme
    if ((args.gen_folder != None) or (args.process)):
        for core_idx in range(OilOs.OsCoresTotalNumber):
            OsGenTcbSourceFile.write(f"\n  Priority Scheme on core{core_idx}:\n  "+tabulate(OilResources.PrioritiesList_per_core[core_idx], headers=["Name", "Priority"], tablefmt='fancy_grid').replace("\n", "\n" + "  ")+"\n")
            OsGenTcbSourceFile.write(f"  Total priority levels on core{core_idx}: {len(OilResources.PrioritiesList_per_core[core_idx])}"+"\n")

    # Display alarms info
    OsGenTcbSourceFile.write("\n  Alarms:\n  "+tabulate(OilAlarms.AlarmsList, headers=["Name", "Alarmtime", "Cycletime", "Event", "Task", "Action", "Autostart", "Callback", "Pinned core"], tablefmt='fancy_grid').replace("\n", "\n" + "  ")+"\n")
    OsGenTcbSourceFile.write(f"  Total number of alarms: {OilAlarms.AlarmsTotalNumber}"+"\n")

    # Display interrupts info
    for core_idx in range(OilOs.OsCoresTotalNumber):
        OsGenTcbSourceFile.write(f"\n  Interrupts on core{core_idx}:\n  "+tabulate(OilInterrupts.InterruptsList_per_core[core_idx], headers=["Name", "Category", "Vector", "Prio", "Nesting", "Core"], tablefmt='fancy_grid').replace("\n", "\n" + "  ")+"\n")
        OsGenTcbSourceFile.write(f"  Total number of used interrupts: {len(OilInterrupts.InterruptsList_per_core[core_idx])}"+"\n")
        OsGenTcbSourceFile.write(f"  Total number of CPU interrupts : {OilOs.OsMaxVectorEntries}"+"\n")
    OsGenTcbSourceFile.write("*/\n\n")

####################################################################
# TASKS:
####################################################################

    for TaskIdx in range(OilTasks.OsTaskTotalNumber):
        task_name             = OilTasks.OsTasksList[TaskIdx][0]
        task_schedule         = "FULL_PREEMPT" if (OilTasks.OsTasksList[TaskIdx][2] == "FULL") else "NONE_PREEMPT"
        task_default_priority = OilTasks.OsTasksList[TaskIdx][3]
        task_activation       = OilTasks.OsTasksList[TaskIdx][4]
        task_autostart        = OilTasks.OsTasksList[TaskIdx][5]
        task_stacksize        = OilTasks.OsTasksList[TaskIdx][6]
        task_wait_event_mask  = OilTasks.OsTasksList[TaskIdx][7]
        task_type             = "EXTENDED" if (task_wait_event_mask != 0) else "BASIC"
        Task_core             = OilTasks.OsTasksList[TaskIdx][8]

        # get processed task prio
        for i in range(len(OilResources.PrioritiesList_per_core[int(Task_core)])):
            if(task_name == OilResources.PrioritiesList_per_core[int(Task_core)][i][0]):
                task_priority = OilResources.PrioritiesList_per_core[int(Task_core)][i][1]

        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"/* Task: {task_name} (core{Task_core}) */\n")
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(
            f"TASK({task_name});\n"
            f"CREATE_STACK({task_name}, {task_stacksize});\n"
            f"static OsTcbType OsTcb_{task_name} = {{\n\
                                         pTASK({task_name}), /* Function */\n\
                                         {task_priority}, /* FixedPrio */\n\
                                         {task_autostart}, /* Autostart */\n\
                                         pTSTACK({task_name}), /* pstack_top */\n\
                                         pBSTACK({task_name}), /* pstack_bot */\n\
                                         pTSTACK({task_name}), /* pCurrentStackPointer */\n\
                                         SUSPENDED, /* TaskStatus */\n\
                                         0, /* Prio */\n\
                                         0, /* CeilingPrio */\n\
                                         0, /* SetEvtMask */\n\
                                         {hex(int(task_wait_event_mask))}, /* WaitEvtMask */ \n\
                                         {task_activation}, /* MaxAllowedMultipleActivation */\n\
                                         0, /* MultipleActivation */\n\
                                         {task_type}, /* TaskType */\n\
                                         {task_schedule} /* TaskSchedType */\n\
                                       }};\n\n"
            )

    for CoreIdx in range(OilOs.OsCoresTotalNumber):
        number_of_task_per_core = len(OilTasks.OsTasksList_per_core[CoreIdx])
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"/* OsTasksConfig (core{CoreIdx}) */\n")
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"static const OsTcbType* OsTasksConfig_core{CoreIdx}[{number_of_task_per_core}] = {{\n")
        for TaskIdx in range(number_of_task_per_core):
            OsGenTcbSourceFile.write(f"    &OsTcb_{OilTasks.OsTasksList_per_core[CoreIdx][TaskIdx][0]},\n")
        OsGenTcbSourceFile.write("};\n\n")

    OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
    OsGenTcbSourceFile.write(f"/* osTaskCoreAsgnLookupTable */\n")
    OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
    OsGenTcbSourceFile.write(f"const osObjectCoreAsgn_t osTaskCoreAsgnLookupTable[{OilTasks.OsTaskTotalNumber}] = {{\n")
    for TaskIdx in range(OilTasks.OsTaskTotalNumber):
        task_name = OilTasks.OsTasksList[TaskIdx][0]
        Task_core = OilTasks.OsTasksList[TaskIdx][8]
        # search local index
        task_local_idx = 0
        for t in range(len(OilTasks.OsTasksList_per_core[int(Task_core)])):
            if(task_name == OilTasks.OsTasksList_per_core[int(Task_core)][t][0]):
                task_local_idx = t
        OsGenTcbSourceFile.write(f"    {{.local_id={task_local_idx}, .pinned_core={Task_core}}}, /* {task_name} */\n")
    OsGenTcbSourceFile.write("};\n\n")

####################################################################
# ALARMS:
####################################################################

    for AlarmIdx in range(OilAlarms.AlarmsTotalNumber):
        alarm_name      = OilAlarms.AlarmsList[AlarmIdx][0]
        alarm_alarmtime = OilAlarms.AlarmsList[AlarmIdx][1] if OilAlarms.AlarmsList[AlarmIdx][1] != "-" else "0"
        alarm_cycletime = OilAlarms.AlarmsList[AlarmIdx][2] if OilAlarms.AlarmsList[AlarmIdx][2] != "-" else "0"
        alarm_event     = OilAlarms.AlarmsList[AlarmIdx][3] if OilAlarms.AlarmsList[AlarmIdx][3] != "-" else "0"
        alarm_task      = OilAlarms.AlarmsList[AlarmIdx][4] if OilAlarms.AlarmsList[AlarmIdx][4] != "-" else "0"
        alarm_action    = OilAlarms.AlarmsList[AlarmIdx][5]
        alarm_autostart = "ALARM_NO_AUTOSTART" if OilAlarms.AlarmsList[AlarmIdx][6] == "FALSE" else "ALARM_AUTOSTART"
        alarm_callback  = f"pALARMCALLBACK({OilAlarms.AlarmsList[AlarmIdx][7]})" if OilAlarms.AlarmsList[AlarmIdx][7] != "-" else "0"
        alarm_core      = OilAlarms.AlarmsList[AlarmIdx][8]

        if alarm_action == "SETEVENT":
            alarm_action = "ALARM_SET_EVENT"
        elif alarm_action == "ACTIVATETASK":
            alarm_action = "ALARM_ACTIVE_TASK"
        else:
            alarm_action = "ALARM_CALLBACK"

        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"/* Alarm: {alarm_name} (core{alarm_core}) */\n")
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        if alarm_callback != "-" and alarm_callback != "0":
            OsGenTcbSourceFile.write(f"ALARMCALLBACK({OilAlarms.AlarmsList[AlarmIdx][7]}) __attribute__((weak));\n")

        OsGenTcbSourceFile.write(
            f"static OsAlarmConfigType OsAlarm_{alarm_name} = {{\n\
                                                     {alarm_alarmtime}, /* InitTicks */\n\
                                                     {alarm_cycletime}, /* InitCycles */\n\
                                                     {alarm_event}, /* Event */\n\
                                                     {alarm_task}, /* TaskId */\n\
                                                     {alarm_action}, /* Action */\n\
                                                     {alarm_autostart}, /* AutoStart */\n\
                                                     ALARM_FREE, /* Status */\n\
                                                     0, /* Alarmtype */\n\
                                                     0, /* AlarmCategory */\n\
                                                     0, /* AlarmCheckPoint */\n\
                                                     {alarm_callback} /* CallBackFunc */\n\
                                                   }};\n\n"
            )

    for CoreIdx in range(OilOs.OsCoresTotalNumber):
        number_of_alarm_per_core = len(OilAlarms.OsAlarmsList_per_core[CoreIdx])
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"/* OsAlarmsConfig (core{CoreIdx}) */\n")
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"static const OsAlarmConfigType* OsAlarmsConfig_core{CoreIdx}[{number_of_alarm_per_core}] = {{\n")
        for AlarmIdx in range(number_of_alarm_per_core):
            OsGenTcbSourceFile.write(f"    &OsAlarm_{OilAlarms.OsAlarmsList_per_core[CoreIdx][AlarmIdx][0]},\n")
        OsGenTcbSourceFile.write("};\n\n")

    OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
    OsGenTcbSourceFile.write(f"/* osAlarmCoreAsgnLookupTable */\n")
    OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
    OsGenTcbSourceFile.write(f"const osObjectCoreAsgn_t osAlarmCoreAsgnLookupTable[{OilAlarms.AlarmsTotalNumber}] = {{\n")
    for AlarmIdx in range(OilAlarms.AlarmsTotalNumber):
        alarm_name = OilAlarms.AlarmsList[AlarmIdx][0]
        alarm_core = OilAlarms.AlarmsList[AlarmIdx][8]
        # search local index
        alarm_local_idx = 0
        for alarm in range(len(OilAlarms.OsAlarmsList_per_core[int(alarm_core)])):
            if(alarm_name == OilAlarms.OsAlarmsList_per_core[int(alarm_core)][alarm][0]):
                alarm_local_idx = alarm
        OsGenTcbSourceFile.write(f"    {{.local_id={alarm_local_idx}, .pinned_core={alarm_core}}}, /* {alarm_name} */\n")
    OsGenTcbSourceFile.write("};\n\n")

####################################################################
# RESOURCES:
####################################################################
    for ResourceIdx in range(OilResources.ResourcesTotalNumber):
        resource_name     = OilResources.ResourcesList[ResourceIdx][0]
        resource_prio     = OilResources.ResourcesList[ResourceIdx][1]
        resource_property = OilResources.ResourcesList[ResourceIdx][2]
        resource_mask     = OilResources.ResourcesList[ResourceIdx][3]
        resource_core     = OilResources.ResourcesList[ResourceIdx][4] #if resource_name != "RES_SCHEDULER" else "--all_cores"

        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"/* Resource: {resource_name} (core{resource_core}) */\n")
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(
            f"static OsResourceConfigType OsResource_{resource_name} = {{\n\
                                                            {resource_prio}, /* ResCeilingPrio */\n\
                                                            0, /* Occupied */\n\
                                                            0, /* CurrentOccupiedTask */\n\
                                                            {resource_mask} /* AuthorizedTask */\n\
                                                          }};\n\n")

    for CoreIdx in range(OilOs.OsCoresTotalNumber):
        number_of_resource_per_core = len(OilResources.ResourcesList_per_core[CoreIdx])
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"/* OsResourceConfig (core{CoreIdx}) */\n")
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"static const OsResourceConfigType* OsResourcesConfig_core{CoreIdx}[{number_of_resource_per_core}] = {{\n")
        for ResIdx in range(number_of_resource_per_core):
            OsGenTcbSourceFile.write(f"    &OsResource_{OilResources.ResourcesList_per_core[CoreIdx][ResIdx][0]},\n")
        OsGenTcbSourceFile.write("};\n\n")

    # RES_SCHEDULE Mapping per core
    OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
    OsGenTcbSourceFile.write(f"/* OsResSchedulerMappingPerCore */\n")
    OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
    OsGenTcbSourceFile.write(f"static const OsResourceId_t osResSchedulerMappingPerCore[{OilOs.OsCoresTotalNumber}] __attribute__((used)) = {{\n")
    for CoreIdx in range(OilOs.OsCoresTotalNumber):
        OsGenTcbSourceFile.write(f"    RES_SCHEDULER_CORE{CoreIdx},\n")
    OsGenTcbSourceFile.write("};\n\n")

    OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
    OsGenTcbSourceFile.write(f"/* osResourceCoreAsgnLookupTable */\n")
    OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
    OsGenTcbSourceFile.write(f"const osObjectCoreAsgn_t osResourceCoreAsgnLookupTable[{OilResources.ResourcesTotalNumber}] = {{\n")
    for ResIdx in range(OilResources.ResourcesTotalNumber):
        resource_name = OilResources.ResourcesList[ResIdx][0]
        resource_core = OilResources.ResourcesList[ResIdx][4]
        # search local index
        alarm_local_idx = 0
        for resource in range(len(OilResources.ResourcesList_per_core[int(resource_core)])):
            if(resource_name == OilResources.ResourcesList_per_core[int(resource_core)][resource][0]):
                alarm_local_idx = resource
        OsGenTcbSourceFile.write(f"    {{.local_id={alarm_local_idx}, .pinned_core={resource_core}}}, /* {resource_name} */\n")
    OsGenTcbSourceFile.write("};\n\n")

####################################################################
# Spinlocks:
####################################################################
    for SpinlockIdx in range(OilSpinlocks.OsSpinlocksTotalNumber):

        spinlock_name = OilSpinlocks.OsSpinlocksList[SpinlockIdx][0]

        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"/* Spinlock: {spinlock_name} */\n")
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"static OsSpinlockConfigType OsSpinlock_{spinlock_name} = {{0}};\n\n")

    OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
    OsGenTcbSourceFile.write(f"/* OsSpinlockConfig */\n")
    OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
    OsGenTcbSourceFile.write(f"static const OsSpinlockConfigType* OsSpinlocksConfig[{OilSpinlocks.OsSpinlocksTotalNumber}] = {{\n")
    for SpinlockIdx in range(OilSpinlocks.OsSpinlocksTotalNumber):
            spinlock_name = OilSpinlocks.OsSpinlocksList[SpinlockIdx][0]
            OsGenTcbSourceFile.write(f"    &OsSpinlock_{spinlock_name},\n")
    OsGenTcbSourceFile.write("};\n\n")

####################################################################
# Interrupts:
####################################################################
    for CoreIdx in range(OilOs.OsCoresTotalNumber):
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"/* Interrupt LUT (Core{CoreIdx}) */\n")
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"const OsIntIsrLtType OsIsrLookupTable_core{CoreIdx}[{OilOs.OsMaxVectorEntries}] = {{\n")
        for vector in range(int(OilOs.OsMaxVectorEntries)):
            isVectorExist = False
            VectorDataIdx = 0
            for i in range(len(OilInterrupts.InterruptsList_per_core[CoreIdx])):
                if(vector == int(OilInterrupts.InterruptsList_per_core[CoreIdx][i][2])):
                    isVectorExist = True
                    VectorDataIdx = i
            if(isVectorExist):
                int_name   = OilInterrupts.InterruptsList_per_core[CoreIdx][VectorDataIdx][0]
                int_cat    = OilInterrupts.InterruptsList_per_core[CoreIdx][VectorDataIdx][1]
                int_vector = OilInterrupts.InterruptsList_per_core[CoreIdx][VectorDataIdx][2]
                int_prio   = OilInterrupts.InterruptsList_per_core[CoreIdx][VectorDataIdx][3]
                int_nest   = "NOT_NESTED" if OilInterrupts.InterruptsList_per_core[CoreIdx][VectorDataIdx][4] == "FALSE" else "NESTED"
                int_isr    = f"pISR({int_name})" if int_cat == "2" else f"(pIsrFunc)&{int_name}"
                OsGenTcbSourceFile.write(f"    {{{int_isr}, (uint8){int_prio}, (OsInterruptNestingType){int_nest}, {int_cat}}}, /* Interrupt vector {vector} */\n")
            else:
                OsGenTcbSourceFile.write(f"    {{pISR(Undefined), (uint8)0, (OsInterruptNestingType)NOT_NESTED, 2}},  /* Interrupt vector {vector} */\n")
        OsGenTcbSourceFile.write("};\n\n")
        # generate
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"/* OsInterruptsConfig_core{CoreIdx} */\n")
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n") 
        OsGenTcbSourceFile.write(f"OsInterruptConfigType OsInterruptsConfig_core{CoreIdx} = {{\n")
        OsGenTcbSourceFile.write(f"    {OilOs.OsMaxVectorEntries}, /* OsNbrOfInterrupts */\n")
        OsGenTcbSourceFile.write(f"    {hex(OilInterrupts.InterruptCat1LowestPrio[CoreIdx][0])}ul, /* OsIntCat1LowestPrioLevel */\n")
        OsGenTcbSourceFile.write(f"    0, /* OsInterruptNestingDepth */\n")
        OsGenTcbSourceFile.write(f"    0, /* OsInterruptSavedPrioLevel */\n")
        OsGenTcbSourceFile.write(f"    0, /* OsInterruptSavedGlobalMask */\n")
        OsGenTcbSourceFile.write(f"    0, /* OsInterruptSuspendOsNestingCall */\n")
        OsGenTcbSourceFile.write(f"    0, /* OsInterruptSuspendAllNestingCall */\n")
        OsGenTcbSourceFile.write(f"    {{0}}, /* OsIntNestSavedStackPointer[OS_INTERRUPT_NESTING_DEPTH_LEVEL] */\n")
        OsGenTcbSourceFile.write(f"    {{0}}, /* OsIntNestSavedPrioLevel[OS_INTERRUPT_NESTING_DEPTH_LEVEL] */\n")
        OsGenTcbSourceFile.write(f"    (OsIntIsrLtType*)&OsIsrLookupTable_core{CoreIdx}[0] /* OsIsrLookupTablePtr */\n")
        OsGenTcbSourceFile.write("};\n\n")

####################################################################
# local OCB per core:
####################################################################

    for CoreIdx in range(OilOs.OsCoresTotalNumber):
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"/* osLcCfg_core{CoreIdx} */\n")
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"static Ocb_t osLcCfg_core{CoreIdx} = {{\n")
        OsGenTcbSourceFile.write(f"    (OsTcbType**)&OsTasksConfig_core{CoreIdx}[0], /* pTcb */\n")
        OsGenTcbSourceFile.write(f"    (OsAlarmConfigType**)&OsAlarmsConfig_core{CoreIdx}[0], /* pAlarm */\n")
        OsGenTcbSourceFile.write(f"    (OsResourceConfigType**)&OsResourcesConfig_core{CoreIdx}[0], /* pRes */\n")
        OsGenTcbSourceFile.write(f"    (OsSpinlockConfigType**)&OsSpinlocksConfig[0], /* pSpinlock */\n")
        OsGenTcbSourceFile.write(f"    (OsInterruptConfigType*)&OsInterruptsConfig_core{CoreIdx}, /* pInt */\n")
        OsGenTcbSourceFile.write(f"    0, /* OsIsRunning */\n")
        OsGenTcbSourceFile.write(f"    0, /* OsHighPrioReadyTaskId */\n")
        OsGenTcbSourceFile.write(f"    OS_NUMBER_OF_TASKS_ON_CORE{CoreIdx}, /* OsCurrentTaskId */\n")
        OsGenTcbSourceFile.write(f"    0, /* OsSysTickCounter */\n")
        OsGenTcbSourceFile.write(f"    0, /* OsIntCallDispatcher */\n")
        OsGenTcbSourceFile.write(f"    0, /* OsLockDispatcher */\n")
        OsGenTcbSourceFile.write(f"    0, /* OsCat2InterruptLevel */\n")
        OsGenTcbSourceFile.write(f"    0, /* OsCurrentSystemStackPtr */\n")
        OsGenTcbSourceFile.write(f"    {{0}}, /* OsCrossCoreBuffer[10] */\n")
        OsGenTcbSourceFile.write(f"    0, /* OsCrossCoreBufferLock */\n")
        OsGenTcbSourceFile.write(f"    0, /* OsCrossCoreBufferSyncFlag */\n")
        OsGenTcbSourceFile.write(f"    {{0}}, /* OsHwSchedPrioReg */\n")
        OsGenTcbSourceFile.write(f"    0, /* OsSpinlockNestingDepth */\n")
        OsGenTcbSourceFile.write(f"    {len(OilTasks.OsTasksList_per_core[CoreIdx])}, /* OsNumberOfTasks */\n")
        OsGenTcbSourceFile.write(f"    {len(OilAlarms.OsAlarmsList_per_core[CoreIdx])}, /* OsNumberOfAlarms */\n")
        OsGenTcbSourceFile.write(f"    {len(OilResources.ResourcesList_per_core[CoreIdx])},  /* OsNumberOfResources */\n")

        if (OilOs.OsStartupHook == "TRUE") :
            OsGenTcbSourceFile.write(f"    osStartupHook_core{CoreIdx}, /* OsStartupHook */\n")
        else:
            OsGenTcbSourceFile.write(f"    0, /* OsStartupHook */\n")

        if (OilOs.OsPreTaskHook == "TRUE") :
            OsGenTcbSourceFile.write(f"    osPreTaskHook_core{CoreIdx}, /* OsPreTaskHook */\n")
        else:
            OsGenTcbSourceFile.write(f"    0, /* OsPreTaskHook */\n")

        if (OilOs.OsPostTaskHook == "TRUE") :
            OsGenTcbSourceFile.write(f"    osPostTaskHook_core{CoreIdx}, /* OsPostTaskHook */\n")
        else:
            OsGenTcbSourceFile.write(f"    0, /* OsPostTaskHook */\n")

        if (OilOs.OsShutdownHook == "TRUE") :
            OsGenTcbSourceFile.write(f"    osShutdownHook_core{CoreIdx}, /* OsShutdownHook */\n")
        else:
            OsGenTcbSourceFile.write(f"    0, /* OsShutdownHook */\n")

        if (OilOs.OsErrorHook == "TRUE") :
            OsGenTcbSourceFile.write(f"    osErrorHook_core{CoreIdx} /* OsErrorHook */\n")
        else:
            OsGenTcbSourceFile.write(f"    0 /* OsErrorHook */\n")

        OsGenTcbSourceFile.write("};\n\n")

####################################################################
# global OCB:
####################################################################
    OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
    OsGenTcbSourceFile.write(f"/* OCB Configuration */\n")
    OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
    OsGenTcbSourceFile.write(f"volatile Ocb_t* OCB_Cfg[{OilOs.OsCoresTotalNumber}] = {{\n")
    for CoreIdx in range(OilOs.OsCoresTotalNumber):
        OsGenTcbSourceFile.write(f"    (Ocb_t*)&osLcCfg_core{CoreIdx},\n")
    OsGenTcbSourceFile.write("};\n\n")

####################################################################
# logical to physical cores mapping:
####################################################################
    OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
    OsGenTcbSourceFile.write(f"/* logical to physical core id mapping*/\n")
    OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
    OsGenTcbSourceFile.write(f"const uint8 osLogicalToPhysicalCoreIdMapping[{OilOs.OsCoresTotalNumber}] = {{\n")
    for CoreIdx in range(OilOs.OsCoresTotalNumber):
        OsGenTcbSourceFile.write(f"    {OilOs.OsCoreList[CoreIdx][2]}, /* {OilOs.OsCoreList[CoreIdx][0]} : {OilOs.OsCoreList[CoreIdx][3]} - {OilOs.OsCoreList[CoreIdx][4]} */\n")
    OsGenTcbSourceFile.write("};\n\n")

####################################################################
# Mailboxes:
####################################################################
    if(OilMbx.OsMbxTotalNumber > 0):
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        OsGenTcbSourceFile.write(f"/* IPCs */\n")
        OsGenTcbSourceFile.write("/********************************************************************************************************************/\n")
        for MbxIdx in range(OilMbx.OsMbxTotalNumber):
            Mbx_name     = OilMbx.OsMbxList[MbxIdx][0]
            Mbx_task     = OilMbx.OsMbxList[MbxIdx][1]
            Mbx_resource = OilMbx.OsMbxList[MbxIdx][2]
            Mbx_event    = OilMbx.OsMbxList[MbxIdx][3]
            Mbx_mode     = OilMbx.OsMbxList[MbxIdx][4]
            Mbx_size     = OilMbx.OsMbxList[MbxIdx][5]
            OsGenTcbSourceFile.write(f"OS_IPC_DEF_MBX_QUEUE({Mbx_name}, {Mbx_task}, {Mbx_size}, {Mbx_resource}, {Mbx_event}, {Mbx_mode});\n")

    OsGenTcbSourceFile.write("\n")
    OsGenTcbSourceFile.close()

######################################################################################################################################################################################
# generate TCB header file
######################################################################################################################################################################################
    OsGenTcbHeaderFile.write("\n// **********************************************************************************************************************\n")
    OsGenTcbHeaderFile.write("//                                                  OSEK OS Configuration\n")
    OsGenTcbHeaderFile.write("// **********************************************************************************************************************\n")
    OsGenTcbHeaderFile.write("\n")
    OsGenTcbHeaderFile.write("// **********************************************************************************************************************\n")
    OsGenTcbHeaderFile.write("// Filename        : OsTcb.h\n")
    OsGenTcbHeaderFile.write("//\n")
    user = getpass.getuser()
    OsGenTcbHeaderFile.write(f"// Author          : {user}\n")
    OsGenTcbHeaderFile.write("//\n")
    OsGenTcbHeaderFile.write(f"// Owner           : {user}\n")
    OsGenTcbHeaderFile.write("//\n")
    OsGenTcbHeaderFile.write("// Generator       : OilGenTool\n")
    OsGenTcbHeaderFile.write("//\n")
    OsGenTcbHeaderFile.write(f"// Oil file        : {args.oil_file}\n")
    OsGenTcbHeaderFile.write("//\n")
    date = datetime.datetime.now().strftime("%d.%m.%Y")
    time = datetime.datetime.now().strftime("%H:%M:%S")
    OsGenTcbHeaderFile.write(f"// Generation Time : {date} {time}\n")
    OsGenTcbHeaderFile.write("//\n")
    OsGenTcbHeaderFile.write("// Description     : Auto-generated OS Configuration file\n")
    OsGenTcbHeaderFile.write("//\n")
    OsGenTcbHeaderFile.write("// **********************************************************************************************************************\n")
    OsGenTcbHeaderFile.write("\n")
    OsGenTcbHeaderFile.write("#ifndef __OS_TCB_H__\n")
    OsGenTcbHeaderFile.write("#define __OS_TCB_H__\n")
    OsGenTcbHeaderFile.write("\n")
    OsGenTcbHeaderFile.write("#include \"OsTypes.h\"\n")
    OsGenTcbHeaderFile.write("\n")

    # Task enum
    OsGenTcbHeaderFile.write("typedef enum {\n")
    for task_id in range(OilTasks.OsTaskTotalNumber):
        OsGenTcbHeaderFile.write(f"    {OilTasks.OsTasksList[task_id][0]},\n")
    OsGenTcbHeaderFile.write("    OS_INVALID_TASK,\n")
    OsGenTcbHeaderFile.write("    OS_NUMBER_OF_TASKS = OS_INVALID_TASK,\n")
    OsGenTcbHeaderFile.write("}OsTaskId_t;\n\n")

    for core_idx in range(OilOs.OsCoresTotalNumber):
        OsGenTcbHeaderFile.write(f"#define OS_NUMBER_OF_TASKS_ON_CORE{core_idx} {len(OilTasks.OsTasksList_per_core[core_idx])}\n")
    OsGenTcbHeaderFile.write("\n\n")

    # Alarm enum
    OsGenTcbHeaderFile.write("typedef enum {\n")
    for alarm_id in range(OilAlarms.AlarmsTotalNumber):
        OsGenTcbHeaderFile.write(f"    {OilAlarms.AlarmsList[alarm_id][0]},\n")
    OsGenTcbHeaderFile.write("    OS_INVALID_ALARM,\n")
    OsGenTcbHeaderFile.write("    OS_NUMBER_OF_ALARMS = OS_INVALID_ALARM\n")
    OsGenTcbHeaderFile.write("}OsAlarmId_t;\n\n")

    # Resource enum
    OsGenTcbHeaderFile.write("typedef enum {\n")
    for res_id in range(OilResources.ResourcesTotalNumber):
        OsGenTcbHeaderFile.write(f"    {OilResources.ResourcesList[res_id][0]},\n")
    OsGenTcbHeaderFile.write("    OS_INVALID_RESOURCE,\n")
    OsGenTcbHeaderFile.write("    OS_NUMBER_OF_RESOURCES = OS_INVALID_RESOURCE\n")
    OsGenTcbHeaderFile.write("}OsResourceId_t;\n\n")

    OsGenTcbHeaderFile.write("#define RES_SCHEDULER osResSchedulerMappingPerCore[osGetLogicalCoreId(osGetCoreId())]\n\n")

    # Spinlocks enum
    OsGenTcbHeaderFile.write("typedef enum {\n")
    for SpinlockIdx in range(OilSpinlocks.OsSpinlocksTotalNumber):
        spinlock_name = OilSpinlocks.OsSpinlocksList[SpinlockIdx][0]
        OsGenTcbHeaderFile.write(f"    {spinlock_name},\n")
    OsGenTcbHeaderFile.write("    OS_INVALID_SPINLOCK,\n")
    OsGenTcbHeaderFile.write("    OS_NUMBER_OF_SPINLOCKS = OS_INVALID_SPINLOCK\n")
    OsGenTcbHeaderFile.write("}OsSpinlockId_t;\n\n")

    # Task function prototypes
    OsGenTcbHeaderFile.write("/* task function prototype */\n")
    for task_id in range(OilTasks.OsTaskTotalNumber):
        OsGenTcbHeaderFile.write(f"TASK({OilTasks.OsTasksList[task_id][0]});\n")
    OsGenTcbHeaderFile.write("\n")

    # Mailboxes
    OsGenTcbHeaderFile.write("/* IPCs */\n")
    for MbxIdx in range(OilMbx.OsMbxTotalNumber):
        Mbx_name     = OilMbx.OsMbxList[MbxIdx][0]
        OsGenTcbHeaderFile.write(f"extern OsIpcMbxCfgType IpcMbx_{Mbx_name};\n")
        OsGenTcbHeaderFile.write(f"#define OS_IPC_{Mbx_name.upper()}  (OsIpcMbxCfgType* const)&IpcMbx_{Mbx_name}\n")
    OsGenTcbHeaderFile.write("\n")

    # Hooks prototypes
    OsGenTcbHeaderFile.write("/* hooks function */\n")
    if (OilOs.OsStartupHook == "TRUE") :
        OsGenTcbHeaderFile.write(f"#define OS_STARTUPHOOK\n")

    if (OilOs.OsPreTaskHook == "TRUE") :
        OsGenTcbHeaderFile.write(f"#define OS_PRETASKHOOK\n")

    if (OilOs.OsPostTaskHook == "TRUE") :
        OsGenTcbHeaderFile.write(f"#define OS_POSTTASKHOOK\n")

    if (OilOs.OsShutdownHook == "TRUE") :
        OsGenTcbHeaderFile.write(f"#define OS_SHUTDOWNHOOK\n")

    if (OilOs.OsErrorHook == "TRUE") :
        OsGenTcbHeaderFile.write(f"#define OS_ERRORHOOK\n")
    OsGenTcbHeaderFile.write("\n\n")

    for core_idx in range(OilOs.OsCoresTotalNumber):
        if (OilOs.OsStartupHook == "TRUE") :
            OsGenTcbHeaderFile.write(f"void osStartupHook_core{core_idx}(void);\n")

        if (OilOs.OsPreTaskHook == "TRUE") :
            OsGenTcbHeaderFile.write(f"void osPreTaskHook_core{core_idx}(void);\n")

        if (OilOs.OsPostTaskHook == "TRUE") :
            OsGenTcbHeaderFile.write(f"void osPostTaskHook_core{core_idx}(void);\n")

        if (OilOs.OsShutdownHook == "TRUE") :
            OsGenTcbHeaderFile.write(f"void osShutdownHook_core{core_idx}(OsStatusType error);\n")

        if (OilOs.OsErrorHook == "TRUE") :
            OsGenTcbHeaderFile.write(f"void osErrorHook_core{core_idx}(OsStatusType error);\n")
    OsGenTcbHeaderFile.write("\n\n")

    # events
    OsGenTcbHeaderFile.write("/* event mask definition */\n")
    for event_idx in range(OilEvents.EventsTotalNumber):
        event_name = OilEvents.EventsList[event_idx][0]
        event_mask = OilEvents.EventsList[event_idx][1]
        OsGenTcbHeaderFile.write(f"#define {event_name}    {event_mask}\n")
    OsGenTcbHeaderFile.write("\n")

    # Wait event mask for each task
    OsGenTcbHeaderFile.write("/* wait event mask for each task */\n")
    for task_id in range(OilTasks.OsTaskTotalNumber):
        task_name = OilTasks.OsTasksList[task_id][0]
        task_wait_event_mask  = OilTasks.OsTasksList[task_id][7]
        OsGenTcbHeaderFile.write(f"#define {task_name.upper()}_WAIT_EVENT_MASK    {hex(int(task_wait_event_mask))}\n");
    OsGenTcbHeaderFile.write("\n")

    OsGenTcbHeaderFile.write(f"#define OS_NUMBER_OF_CORES    {OilOs.OsCoresTotalNumber}UL\n\n")

    for core_idx in range(OilOs.OsCoresTotalNumber):
        OsGenTcbHeaderFile.write(f"extern const OsIntIsrLtType OsIsrLookupTable_core{core_idx}[{OilOs.OsMaxVectorEntries}];\n")

    OsGenTcbHeaderFile.write(f"extern const osObjectCoreAsgn_t osTaskCoreAsgnLookupTable[{OilTasks.OsTaskTotalNumber}];\n")
    OsGenTcbHeaderFile.write(f"extern const osObjectCoreAsgn_t osAlarmCoreAsgnLookupTable[{OilAlarms.AlarmsTotalNumber}];\n")
    OsGenTcbHeaderFile.write(f"extern const osObjectCoreAsgn_t osResourceCoreAsgnLookupTable[{OilResources.ResourcesTotalNumber}];\n")
    OsGenTcbHeaderFile.write(f"extern const uint8 osLogicalToPhysicalCoreIdMapping[{OilOs.OsCoresTotalNumber}];\n")
    OsGenTcbHeaderFile.write("\n")
    OsGenTcbHeaderFile.write(f"extern volatile Ocb_t* OCB_Cfg[{OilOs.OsCoresTotalNumber}];\n\n")
    OsGenTcbHeaderFile.write("/* ISR functions prototypes */\n")
    for vect in range(len(OilInterrupts.InterruptsList)):
        int_name = OilInterrupts.InterruptsList[vect][0]
        int_cat  = OilInterrupts.InterruptsList[vect][1]
        int_isr  = f"ISR({int_name});" if int_cat == "2" else f"void {int_name}(void);"
        OsGenTcbHeaderFile.write(f"{int_isr}\n")
    OsGenTcbHeaderFile.write("ISR(Undefined);\n\n")
    OsGenTcbHeaderFile.write("#endif /* __OS_TCB_H__ */\n")
    OsGenTcbHeaderFile.close()

######################################################################################################################################################################################
# generate Prio type header file
######################################################################################################################################################################################
    OsGenPrioHeaderFile.write("\n// **********************************************************************************************************************\n")
    OsGenPrioHeaderFile.write("//                                                  OSEK OS Configuration\n")
    OsGenPrioHeaderFile.write("// **********************************************************************************************************************\n")
    OsGenPrioHeaderFile.write("\n")
    OsGenPrioHeaderFile.write("// **********************************************************************************************************************\n")
    OsGenPrioHeaderFile.write("// Filename        : OsGenPrioTypes.h\n")
    OsGenPrioHeaderFile.write("//\n")
    user = getpass.getuser()
    OsGenPrioHeaderFile.write(f"// Author          : {user}\n")
    OsGenPrioHeaderFile.write("//\n")
    OsGenPrioHeaderFile.write(f"// Owner           : {user}\n")
    OsGenPrioHeaderFile.write("//\n")
    OsGenPrioHeaderFile.write("// Generator       : OilGenTool\n")
    OsGenPrioHeaderFile.write("//\n")
    OsGenPrioHeaderFile.write(f"// Oil file        : {args.oil_file}\n")
    OsGenPrioHeaderFile.write("//\n")
    date = datetime.datetime.now().strftime("%d.%m.%Y")
    time = datetime.datetime.now().strftime("%H:%M:%S")
    OsGenPrioHeaderFile.write(f"// Generation Time : {date} {time}\n")
    OsGenPrioHeaderFile.write("//\n")
    OsGenPrioHeaderFile.write("// Description     : Auto-generated OS Configuration file\n")
    OsGenPrioHeaderFile.write("//\n")
    OsGenPrioHeaderFile.write("// **********************************************************************************************************************\n")
    OsGenPrioHeaderFile.write("\n")
    OsGenPrioHeaderFile.write("#ifndef _OSPRIOTYPES_H_\n")
    OsGenPrioHeaderFile.write("#define _OSPRIOTYPES_H_\n")
    OsGenPrioHeaderFile.write("\n")
    OsGenPrioHeaderFile.write("#include \"stdint.h\"\n")
    OsGenPrioHeaderFile.write("\n")

    # check how many 4-byte is needed to store all prio
    high_prio = 0
    for core_idx in range(OilOs.OsCoresTotalNumber):
        if(high_prio < OilResources.PrioritiesList_per_core[core_idx][len(OilResources.PrioritiesList_per_core[core_idx])-1][1]):
            high_prio = OilResources.PrioritiesList_per_core[core_idx][len(OilResources.PrioritiesList_per_core[core_idx])-1][1]

    OsGenPrioHeaderFile.write("typedef struct\n")
    OsGenPrioHeaderFile.write("{\n")
    for prio_level in range((int(high_prio / 32)) + 1):
        OsGenPrioHeaderFile.write(f"  uint32_t OsGroupPrioLevel{prio_level}; /* PRIORITY {32*prio_level} --> {32*prio_level + 31} */\n")

    OsGenPrioHeaderFile.write("}OsSchedPrioType;\n")
    OsGenPrioHeaderFile.write("\n")
    OsGenPrioHeaderFile.write("#endif /* _OSPRIOTYPES_H_ */\n")
    OsGenPrioHeaderFile.close()
