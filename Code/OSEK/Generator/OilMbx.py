import sys
import re
import os
from jinja2 import Template
from tabulate import tabulate

# Globals
OsMbxTotalNumber = 0
OsMbxList = []


def OilMbxParser(args, Oilfile):
    
    global OsMbxTotalNumber, OsMbxList

    # Search for IPC names and properties
    Mbx_pattern          = re.compile(r"IPC\s+(\w+)\s*\{([\s\S]*?)\};", re.MULTILINE)
    Mbx_task_pattern     = re.compile(r"TASK\s*=\s*(\w+);")
    Mbx_resource_pattern = re.compile(r"RESOURCE\s*=\s*(\w+);")
    Mbx_event_pattern    = re.compile(r"EVENT\s*=\s*(\w+);")
    Mbx_mode_pattern     = re.compile(r"MODE\s*=\s*(\w+);")
    Mbx_size_pattern     = re.compile(r"SIZE\s*=\s*(\w+);")

    Mbxs = Mbx_pattern.findall(Oilfile)

    # Parse the Mailbox list
    for Mbx in Mbxs:
        Mbx_name, Mbx_properties = Mbx
        Mbx_task_match     = Mbx_task_pattern.search(Mbx_properties)
        Mbx_resource_match = Mbx_resource_pattern.search(Mbx_properties)
        Mbx_event_match    = Mbx_event_pattern.search(Mbx_properties)
        Mbx_mode_match     = Mbx_mode_pattern.search(Mbx_properties)
        Mbx_size_match     = Mbx_size_pattern.search(Mbx_properties)

        Mbx_task       = Mbx_task_match.group(1) if Mbx_task_match else None
        Mbx_resource   = Mbx_resource_match.group(1) if Mbx_resource_match else None
        Mbx_event      = Mbx_event_match.group(1) if Mbx_event_match else None
        Mbx_mode       = Mbx_mode_match.group(1) if Mbx_mode_match else None
        Mbx_size       = Mbx_size_match.group(1) if Mbx_size_match else None

        if(Mbx_mode != "LOCAL") and (Mbx_mode != "BROADCAST"):
            print("[ERROR] IPC mode must be \"LOCAL\" or \"BROADCAST\"")
            exit(1)

        if(Mbx_mode == "LOCAL"):
            Mbx_mode = "IPC_MBX_MODE_LOCAL"
        else:
            Mbx_mode = "IPC_MBX_MODE_BROADCAST"

        if(Mbx_size != None):
                try:
                    int(Mbx_size, 0)
                except ValueError:
                    print("[ERROR] IPC size must be a decimal or a Hex (0x..) number!")
                    exit(1)

        OsMbxTotalNumber += 1
        # Format: [Name, Task, Resource, Event, Mode, Size]
        OsMbxList.append([Mbx_name, Mbx_task, Mbx_resource, Mbx_event, Mbx_mode, Mbx_size])

    #print(f"OsMbxTotalNumber = {OsMbxTotalNumber}")
    #print(f"OsMbxList = {OsMbxList}")


