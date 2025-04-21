import sys
import re
import os
from jinja2 import Template
from tabulate import tabulate

# Globals
OsName             = ''
OsMulticore        = ''
OsStartupHook      = ''
OsErrorHook        = ''
OsShutdownHook     = ''
OsPreTaskHook      = ''
OsPostTaskHook     = ''
OsStackCheck       = ''
OsCpuFrequency     = ''
OsMaxVectorEntries = ''
OsTickTime         = ''
OsList             = []
OsCoreList         = []
OsPropertiesList   = []
OsCoresTotalNumber = 0

def OilOsParser(args, Oilfile):
    
    global OsStartupHook
    global OsErrorHook
    global OsShutdownHook
    global OsPreTaskHook
    global OsPostTaskHook
    global OsStackCheck
    global OsCpuFrequency
    global OsMaxVectorEntries
    global OsTickTime
    global OsCoresTotalNumber
    global OsCoreList

    # Search for OS properties
    os_pattern = re.compile(r"OS\s+(\w+)\s*\{([\s\S]*?)\};", re.MULTILINE)
    os = os_pattern.findall(Oilfile)

    # Display OS information
    for os_instance in os:
        os_name, os_properties = os_instance

    # Display OS information
    multicore_pattern          = re.compile(r"MULTICORE\s*=\s*(\w+);")
    startup_hook_pattern       = re.compile(r"STARTUPHOOK\s*=\s*(\w+);")
    error_hook_pattern         = re.compile(r"ERRORHOOK\s*=\s*(\w+);")
    shutdown_hook_pattern      = re.compile(r"SHUTDOWNHOOK\s*=\s*(\w+);")
    pre_task_hook_pattern      = re.compile(r"PRETASKHOOK\s*=\s*(\w+);")
    post_task_hook_pattern     = re.compile(r"POSTTASKHOOK\s*=\s*(\w+);")
    stack_check_pattern        = re.compile(r"WithStackCheck\s*=\s*(\w+);")
    cpu_frequency_pattern      = re.compile(r"CpuFrequency\s*=\s*(\d+\.\d+);")
    max_vector_entries_pattern = re.compile(r"MaximumVectorEntries\s*=\s*(\w+);")
    tick_time_pattern          = re.compile(r"TickTime\s*=\s*(\d+);")

    multicore_match          = multicore_pattern.search(os_properties)
    startup_hook_match       = startup_hook_pattern.search(os_properties)
    error_hook_match         = error_hook_pattern.search(os_properties)
    shutdown_hook_match      = shutdown_hook_pattern.search(os_properties)
    pre_task_hook_match      = pre_task_hook_pattern.search(os_properties)
    post_task_hook_match     = post_task_hook_pattern.search(os_properties)
    stack_check_match        = stack_check_pattern.search(os_properties)
    cpu_frequency_match      = cpu_frequency_pattern.search(os_properties)
    max_vector_entries_match = max_vector_entries_pattern.search(os_properties)
    tick_time_match          = tick_time_pattern.search(os_properties)

    OsName             = os_name
    OsMulticore        = multicore_match.group(1) if multicore_match else None
    OsStartupHook      = startup_hook_match.group(1) if startup_hook_match else None
    OsErrorHook        = error_hook_match.group(1) if error_hook_match else None
    OsShutdownHook     = shutdown_hook_match.group(1) if shutdown_hook_match else None
    OsPreTaskHook      = pre_task_hook_match.group(1) if pre_task_hook_match else None
    OsPostTaskHook     = post_task_hook_match.group(1) if post_task_hook_match else None
    OsStackCheck       = stack_check_match.group(1) if stack_check_match else None
    OsCpuFrequency     = cpu_frequency_match.group(1) if cpu_frequency_match else None
    OsMaxVectorEntries = max_vector_entries_match.group(1) if max_vector_entries_match else None
    OsTickTime         = tick_time_match.group(1) if tick_time_match else None

    OsList.append([OsName, f"OsMulticore        =  {OsMulticore       }\n\
OsStartupHook      =  {OsStartupHook     }\n\
OsErrorHook        =  {OsErrorHook       }\n\
OsShutdownHook     =  {OsShutdownHook    }\n\
OsPreTaskHook      =  {OsPreTaskHook     }\n\
OsPostTaskHook     =  {OsPostTaskHook    }\n\
OsStackCheck       =  {OsStackCheck      }\n\
OsCpuFrequency     =  {OsCpuFrequency    }\n\
OsMaxVectorEntries =  {OsMaxVectorEntries}\n\
OsTickTime         =  {OsTickTime        }" ])

    OsPropertiesList.append([OsName,
                             OsMulticore,
                             OsStartupHook,
                             OsErrorHook,
                             OsShutdownHook,
                             OsPreTaskHook,
                             OsPostTaskHook,
                             OsStackCheck,
                             OsCpuFrequency,
                             OsMaxVectorEntries,
                             OsTickTime])

    # Search for CPU_CORE names and properties
    cpu_core_pattern             = re.compile(r"CPU_CORE\s+(\w+)\s*\{([\s\S]*?)\};", re.MULTILINE)
    cpu_core_logical_id_pattern  = re.compile(r"CORE_LOGICAL_ID\s*=\s*(\w+);")
    cpu_core_physical_id_pattern = re.compile(r"CORE_PHYSICAL_ID\s*=\s*(\w+);")
    cpu_core_vendor_pattern      = re.compile(r"VENDOR\s*=\s*\"(\w+)\";")
    cpu_core_type_pattern        = re.compile(r"CORE_TYPE\s*=\s*\"(\w+)\";")


    cores = cpu_core_pattern.findall(Oilfile)

    for core in cores:
        # Get the core name and the core properties
        core_name, core_properties = core

        # Count the number of cores
        OsCoresTotalNumber += 1

        # Search for core properties value
        cpu_core_logical_id_match   = cpu_core_logical_id_pattern.search(core_properties)
        cpu_core_physical_id_match  = cpu_core_physical_id_pattern.search(core_properties)
        cpu_core_vendor_match       = cpu_core_vendor_pattern.search(core_properties)
        cpu_core_type_match         = cpu_core_type_pattern.search(core_properties)

        cpu_core_logical_id       = cpu_core_logical_id_match.group(1) if cpu_core_logical_id_match else str(OsCoresTotalNumber - 1)
        cpu_core_physical_id      = cpu_core_physical_id_match.group(1) if cpu_core_physical_id_match else None
        cpu_core_vendor           = cpu_core_vendor_match.group(1) if cpu_core_vendor_match else None
        cpu_core_type             = cpu_core_type_match.group(1) if cpu_core_type_match else None

        OsCoreList.append([core_name, cpu_core_logical_id, cpu_core_physical_id, cpu_core_vendor, cpu_core_type])
