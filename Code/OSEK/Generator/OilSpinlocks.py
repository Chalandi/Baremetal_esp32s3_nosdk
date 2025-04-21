import sys
import re
import os
from jinja2 import Template
from tabulate import tabulate

# Globals
OsSpinlocksTotalNumber = 0
OsSpinlocksList = []


def OilSpinlocksParser(args, Oilfile):
    
    global OsSpinlocksTotalNumber, OsSpinlocksList

    # Search for Spinlock names and properties
    Spinlock_pattern      = re.compile(r"SPINLOCK\s+(\w+)\s*\{([\s\S]*?)\};", re.MULTILINE)
    Spinlock_prop_pattern = re.compile(r"SPINLOCKPROPERTY\s*=\s*(\w+);")
    Spinlocks             = Spinlock_pattern.findall(Oilfile)

    # Parse the Spinlock list
    for Spinlock in Spinlocks:
        Spinlock_name, Spinlock_properties = Spinlock
        Spinlock_prop_match = Spinlock_prop_pattern.search(Spinlock_properties)
        Spinlock_prop       = Spinlock_prop_match.group(1) if Spinlock_prop_match else None

        if Spinlock_prop != "STANDARD" :
            print(f"The Spinlock {Spinlock_name} must have \"STANDARD\" as property !")
            exit()

        OsSpinlocksTotalNumber += 1
        # Format: [Name]
        OsSpinlocksList.append([Spinlock_name])

    #print(f"OsSpinlocksTotalNumber = {OsSpinlocksTotalNumber}")
    #print(f"OsSpinlocksList = {OsSpinlocksList}")


