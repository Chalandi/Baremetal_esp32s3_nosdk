#####################################################################################
# 
# Filename    : OilGen.py
# 
# Author      : Chalandi Amine
#
# Owner       : Chalandi Amine
# 
# Date        : 29.01.2023
# 
# Description : OSEK OIL code generator tool
# 
#####################################################################################

import sys
import re
import os
import subprocess

# Dependencies:
try:
    from jinja2 import Template
except ImportError:
    subprocess.check_call([sys.executable, "-m", "pip", "install", "jinja2"])

try:
    from tabulate import tabulate
except ImportError:
    subprocess.check_call([sys.executable, "-m", "pip", "install", "tabulate"])

try:
    import argparse
except ImportError:
    subprocess.check_call([sys.executable, "-m", "pip", "install", "argparse"])


import OilOs
import OilTasks
import OilEvents
import OilResources
import OilSpinlocks
import OilAlarms
import OilInterrupts
import OilGenTemplate
import OilTcbGen
import OilMbx

# ANSI color code definition
class Color:
    BLACK     = '\033[30m'
    RED       = '\033[31m'
    GREEN     = '\033[32m'
    YELLOW    = '\033[33m'
    BLUE      = '\033[34m'
    MAGENTA   = '\033[35m'
    CYAN      = '\033[36m'
    WHITE     = '\033[37m'
    UNDERLINE = '\033[4m'
    RESET     = '\033[0m'

# Command-line syntax :  py  <ThisPythonScriptName>  -h

# Get command-line arguments
parser = argparse.ArgumentParser()
parser.add_argument("-i", dest="oil_file", help="Input OSEK OIL file")
parser.add_argument("-gen", dest="gen_folder", help="Output folder to generate OSEK TCB data")
parser.add_argument("-template", dest="template_oil_file", help="Create an OSEK OIL template file")
args = parser.parse_args()

# Check args
if (args.oil_file == None) and (args.template_oil_file == None):
    parser.print_usage()
    quit()

# Check if the input file exists
if (args.oil_file!= None) and (os.path.exists(args.oil_file) == False):
    print(f"The input file \"{args.oil_file}\" does not exist !")
    sys.exit(0)

# Check if the output folder
gen_folder = os.path.abspath(args.gen_folder)
if (args.gen_folder!= None) and (os.path.exists(gen_folder) == False):
    try:
        os.makedirs(gen_folder, exist_ok=True)
    except Exception as e:
        print(f"Failed to create folder \"{gen_folder}\": {e}")
        sys.exit(1)

# Get the OIL file content
if (args.oil_file != None):
    OilFileDescriptor = open(args.oil_file, 'r')
    OilFileContent    = OilFileDescriptor.read()

    # Remove all comments
    OilFileContent = re.sub(r'//.*', '', OilFileContent)
    OilFileContent = re.sub(r'/\*.*?\*/', '', OilFileContent, flags=re.DOTALL)

    # Parse OS
    OilOs.OilOsParser(args, OilFileContent)

    # Parse tasks
    OilTasks.OilTaskParser(args, OilFileContent)

    # Parse events
    OilEvents.OilEventsParser(args, OilFileContent)

    # Parse resources
    OilResources.OilResourcesParser(args, OilFileContent)

    # Parse spinlocks
    OilSpinlocks.OilSpinlocksParser(args, OilFileContent)

    # Parse alarms
    OilAlarms.OilAlarmsParser(args, OilFileContent)

    # Parse interrupts
    OilInterrupts.OilInterruptsParser(args, OilFileContent)

    # Parse mailboxes
    OilMbx.OilMbxParser(args, OilFileContent)

# Generate TCB:
if (args.gen_folder != None):
    ##################################################################################################
    # Process the event's mask
    ##################################################################################################
    # steps:
    # 1- Create the initial task's event mask from the hard coded masks in the OIL file.
    # 2- For each task's event mask defined with AUTO in OIL file we assign to it a free mask.
    # 3- Set the new event mask into OilEvents.OilEventsList
    ##################################################################################################
    for taskIdx in range(OilTasks.OsTaskTotalNumber):
        # Create the task event mask from the hard coded event's mask in OIL file
        TaskEventMask    = 0
        CurrentEventMask = 0

        for event in range(len(OilTasks.osTaskEvents[taskIdx])):
            TaskEventMask |= OilEvents.OilEventsGetMask(OilTasks.osTaskEvents[taskIdx][event])

        # For each event from the task's events
        for taskEventList in OilTasks.osTaskEvents:
           if len(taskEventList) > 32:
               print(f"{OilTasks.OsTasksList[taskIdx][0]} has more than 32 events !")
               exit()

           # For each event assigned to the current task check for the OIL defined mask if exist 
           # otherwise pick one from the local generated list
           for taskEventIdx in range(len(taskEventList)):

               if taskEventList[taskEventIdx] in OilTasks.osTaskEvents[taskIdx]:
                   # Update the task event mask
                   TaskEventMask  = TaskEventMask | CurrentEventMask
                   # Get current event's MASK from events list
                   #print(f"taskEventList[taskEventIdx] = {taskEventList[taskEventIdx]}")
                   CurrentEventMask = OilEvents.OilEventsGetMask(taskEventList[taskEventIdx])
                   if(CurrentEventMask == 0):
                      # Assign a new event's mask
                      CurrentEventMask =  OilEvents.OilEventsGenMask(TaskEventMask)
                      OilEvents.OilEventsSetMask(taskEventList[taskEventIdx], CurrentEventMask)

        # check if there is a duplicated event mask
        #print(f"OilTasks.osTaskEvents[{taskIdx}] = {OilTasks.osTaskEvents[taskIdx]}")
        events_mask = 0
        for event in OilTasks.osTaskEvents[taskIdx]:
            event_mask = OilEvents.OilEventsGetMask(event)
            if (events_mask & event_mask == 0):
                events_mask = events_mask | event_mask
            else:
                print(f"error: Duplicated event Mask \"{event} = {hex(event_mask)}\" for task id {taskIdx} please fix the hard coded event in the OIL file !!!!")
                exit()

    ##################################################################################################
    # Calculate Task's wait event mask
    ##################################################################################################
    for taskIdx in range(OilTasks.OsTaskTotalNumber):
        TaskWaitEventMask = 0
        for EventMask in range(len(OilTasks.osTaskEvents[taskIdx])):
            TaskWaitEventMask |= OilEvents.OilEventsGetMask(OilTasks.osTaskEvents[taskIdx][EventMask])
        OilTasks.OsTasksList[taskIdx][7] = TaskWaitEventMask

    ##################################################################################################
    # Check if resources are used cross cores (which is not allowed)
    ##################################################################################################
    detected_cross_core_resources = 0
    for ResourceIdx in range(OilResources.ResourcesTotalNumber):
        resource_name = OilResources.ResourcesList[ResourceIdx][0]
        resource_pinned_core = OilResources.ResourcesList[ResourceIdx][4]

        # search for the resource in the assigned task
        for TaskIdx in range(OilTasks.OsTaskTotalNumber):
            for TaskResource in range(len(OilTasks.osTaskResources[TaskIdx])):
                Task_resource_name = OilTasks.osTaskResources[TaskIdx][TaskResource]
                Task_pinned_core = OilTasks.OsTasksList[TaskIdx][8]

                if(resource_name == Task_resource_name) and (resource_pinned_core != Task_pinned_core):
                    print(f"[ERROR]: The resource \"{resource_name}\" pinned to logical core \"{resource_pinned_core}\" is assigned to the task \"{OilTasks.OsTasksList[TaskIdx][0]}\" pinned to core \"{Task_pinned_core}\", cross-core resources are not allowed in OSEK !")
                    detected_cross_core_resources += 1

    if(detected_cross_core_resources > 0):
        print("please fix your OIL file and try again!")
        exit()

    ##################################################################################################
    # Process the priorities (Priority Ceiling Protocol) per core
    ##################################################################################################
    # steps for each core:
    # 1- Create list of all tasks and resources with their default priorities
    # 2- Create a room between all tasks priority (offset = Task_index * 100)
    # 3- Calculate the priority ceiling of each resource
    # 4- Sort and update all tasks and resources priorities (preparing for using CPU's CLZ instruction)
    # 5- Add "RES_SCHEDULER" on top
    ##################################################################################################
    for CoreIdx in range(OilOs.OsCoresTotalNumber):
        # Create a prio list of all tasks belong to current core id + enough space to introduce the resources
        for TaskIdx in range(len(OilTasks.OsTasksList_per_core[CoreIdx])):
            TaskName = OilTasks.OsTasksList_per_core[CoreIdx][TaskIdx][0]
            TaskPrio = int(OilTasks.OsTasksList_per_core[CoreIdx][TaskIdx][3], base=0) + TaskIdx * 100
            OilResources.PrioritiesList_per_core[CoreIdx].append([TaskName, TaskPrio])

        # Calculate the priority ceiling of each resource belong to current core id
        for ResourceIdx in range(len(OilResources.ResourcesList_per_core[CoreIdx])):
            ResourceName = OilResources.ResourcesList_per_core[CoreIdx][ResourceIdx][0]
            ResourcePrio = OilResources.ResourcesList_per_core[CoreIdx][ResourceIdx][1]

            for TaskIdx in range(len(OilTasks.OsTasksList_per_core[CoreIdx])):
                for i in range(len(OilTasks.osTaskResources_per_core[CoreIdx][TaskIdx])):
                    if (ResourceName == OilTasks.osTaskResources_per_core[CoreIdx][TaskIdx][i]):
                        # Create the resource mask (set the bit of the task id that using this resource)
                        OilResources.ResourcesList_per_core[CoreIdx][ResourceIdx][3] |= (1 << TaskIdx)

                        # Calculate the prio ceiling
                        if (ResourcePrio < int(OilTasks.OsTasksList_per_core[CoreIdx][TaskIdx][3], base=0) + TaskIdx * 100):
                            OilResources.ResourcesList_per_core[CoreIdx][ResourceIdx][1] = int(OilTasks.OsTasksList_per_core[CoreIdx][TaskIdx][3],
                                                                             base=0) + TaskIdx * 100 + 1

        # Update the priorities list by adding the resources prio ceiling calculated previously
        for ResourceIdx in range(len(OilResources.ResourcesList_per_core[CoreIdx])):
            ResourceName = OilResources.ResourcesList_per_core[CoreIdx][ResourceIdx][0]
            ResourcePrio = OilResources.ResourcesList_per_core[CoreIdx][ResourceIdx][1]
            OilResources.PrioritiesList_per_core[CoreIdx].append([ResourceName, ResourcePrio])

        # Sort the priorities and update the tasks and resources priorities
        OilResources.PrioritiesList_per_core[CoreIdx].sort(key=lambda x: x[1])
        for idx in range(len(OilResources.PrioritiesList_per_core[CoreIdx])):
            OilResources.PrioritiesList_per_core[CoreIdx][idx][1] = idx

            for ResourceIdx in range(len(OilResources.ResourcesList_per_core[CoreIdx])):
                if (OilResources.PrioritiesList_per_core[CoreIdx][idx][0] == OilResources.ResourcesList_per_core[CoreIdx][ResourceIdx][0]):
                    OilResources.ResourcesList_per_core[CoreIdx][ResourceIdx][1] = str(OilResources.PrioritiesList_per_core[CoreIdx][idx][1])

            for TaskIdx in range(len(OilResources.ResourcesList_per_core[CoreIdx])):
                if (OilResources.PrioritiesList_per_core[CoreIdx][idx][0] == OilTasks.OsTasksList_per_core[CoreIdx][TaskIdx][0]):
                    OilTasks.OsTasksList_per_core[CoreIdx][TaskIdx][3] = str(OilResources.PrioritiesList_per_core[CoreIdx][idx][1])

        # Add "RES_SCHEDULER" on top of ResourcesList_per_core[CoreIdx]
        OilResources.ResourcesList_per_core[CoreIdx].append([f"RES_SCHEDULER_CORE{CoreIdx}", OilResources.PrioritiesList_per_core[CoreIdx][len(OilResources.PrioritiesList_per_core[CoreIdx]) - 1][1] + 1, "STANDARD",
                                           (1 << len(OilTasks.OsTasksList_per_core[CoreIdx])) - 1])

        # Add "RES_SCHEDULER" on top of OilResources.PrioritiesList_per_core[CoreIdx]
        OilResources.PrioritiesList_per_core[CoreIdx].append([OilResources.ResourcesList_per_core[CoreIdx][len(OilResources.ResourcesList_per_core[CoreIdx]) - 1][0],
                               OilResources.ResourcesList_per_core[CoreIdx][len(OilResources.ResourcesList_per_core[CoreIdx]) - 1][1]])

    # update the global resource list of all cores
    res_scheduler_prio = 0
    res_scheduler_mask = 0

    for CoreIdx in range(OilOs.OsCoresTotalNumber):
        for resId in range(len(OilResources.ResourcesList_per_core[CoreIdx])):
            res_list_idx = -1
            # search for the core specific resource in the global list
            for ResourceIdx in range(OilResources.ResourcesTotalNumber):
                 resource_name = OilResources.ResourcesList[ResourceIdx][0]
                 if(resource_name == OilResources.ResourcesList_per_core[CoreIdx][resId][0]):
                     res_list_idx = ResourceIdx
            if(res_list_idx != -1):
                # Update the resource list 
                OilResources.ResourcesList[res_list_idx][1] = OilResources.ResourcesList_per_core[CoreIdx][resId][1]
                OilResources.ResourcesList[res_list_idx][3] = OilResources.ResourcesList_per_core[CoreIdx][resId][3]
            elif(OilResources.ResourcesList_per_core[CoreIdx][resId][0] == f"RES_SCHEDULER_CORE{CoreIdx}"):
                # add the core RES_SCHEDULER
                res_scheduler_prio = OilResources.ResourcesList_per_core[CoreIdx][resId][1]
                res_scheduler_mask = OilResources.ResourcesList_per_core[CoreIdx][resId][3]
                OilResources.ResourcesList.append([f"RES_SCHEDULER_CORE{CoreIdx}", res_scheduler_prio, "STANDARD", res_scheduler_mask, CoreIdx])
                OilResources.ResourcesTotalNumber += 1
            else:
                print("Core specific resource was not found in the global resource list")
                exit(0)

    ##################################################################################################
    # Calculate interrupt cat1 prio mask per core
    ##################################################################################################
    for CoreIdx in range(OilOs.OsCoresTotalNumber):
        IntCat1LowPrio  = 0xffffffff
        IntCat2HighPrio = 0
        for IntIdx in range(len(OilInterrupts.InterruptsList_per_core[CoreIdx])):
            IntCatType = OilInterrupts.InterruptsList_per_core[CoreIdx][IntIdx][1]
            Intprio    = OilInterrupts.InterruptsList_per_core[CoreIdx][IntIdx][3]
            if(IntCatType == '1' and int(Intprio) < IntCat1LowPrio):
                IntCat1LowPrio = int(Intprio)
            elif(IntCatType == '2' and int(Intprio) > IntCat2HighPrio):
                IntCat2HighPrio = int(Intprio)
        if(IntCat1LowPrio < IntCat2HighPrio):
            print("Error: The lowest cat1 prio is lower than highest cat2 prio !!!")
            print(f" IntCat1LowPrio = {IntCat1LowPrio}, IntCat2HighPrio = {IntCat2HighPrio}")
            quit()
        else:
            if(IntCat1LowPrio == 0xffffffff):
                IntCat1LowPrio = IntCat2HighPrio + 1
            OilInterrupts.InterruptCat1LowestPrio[CoreIdx].append(IntCat1LowPrio)

    ##################################################################################################
    # Code Generation
    ##################################################################################################
    OilTcbGen.OsConfigTcbGeneration(args,
                                      OilOs,
                                      OilTasks,
                                      OilEvents,
                                      OilResources,
                                      OilSpinlocks,
                                      OilAlarms,
                                      OilInterrupts,
                                      OilMbx,
                                      OilGenTemplate)

# Generate OIL template
if args.template_oil_file:
    OilGenTemplate.OilGenTemplate(args)


# Close the oil file
if (args.oil_file!= None):
    OilFileDescriptor.close()
