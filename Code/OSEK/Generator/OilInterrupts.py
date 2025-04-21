import sys
import re
import os
from jinja2 import Template
from tabulate import tabulate

# Globals
InterruptsTotalNumber   = 0
InterruptCat1LowestPrio = [[], []]
InterruptsList          = []
InterruptsList_per_core = [[], []]

def OilInterruptsParser(args, Oilfile):
    
    global InterruptsTotalNumber

    # Search for ISR names and properties
    isr_pattern = re.compile(r"ISR\s+(\w+)\s*\{([\s\S]*?)\};", re.MULTILINE)
    isrs = isr_pattern.findall(Oilfile)
    
    # Display ISR information
    for isr in isrs:
        isr_name, isr_properties = isr

        # Search for category
        isr_cat_pattern = re.compile(r"CATEGORY\s*=\s*(\d+);")
        isr_cat_match = isr_cat_pattern.search(isr_properties)
        isr_cat = isr_cat_match.group(1) if isr_cat_match else None

        # Search for vector address
        isr_vector_pattern = re.compile(r"VectorAddress\s*=\s*(\w+);")
        isr_vector_match = isr_vector_pattern.search(isr_properties)
        isr_vector = isr_vector_match.group(1) if isr_vector_match else None

        # Search for nesting level activation
        isr_nest_pattern = re.compile(r"Nesting\s*=\s*(\w+);")
        isr_nest_match = isr_nest_pattern.search(isr_properties)
        isr_nest = isr_nest_match.group(1) if isr_nest_match else None
        isr_nest = "FALSE" if isr_nest == None else isr_nest

        # Search for priority number
        isr_prio_pattern = re.compile(r"Priority\s*=\s*(\w+);")
        isr_prio_match = isr_prio_pattern.search(isr_properties)
        isr_prio = isr_prio_match.group(1) if isr_prio_match else None
        isr_prio = 0 if isr_prio == None else isr_prio

        # Core
        isr_core_pattern = re.compile(r"CORE\s*=\s*(\w+);")
        isr_core_match = isr_core_pattern.search(isr_properties)
        isr_core = isr_core_match.group(1) if isr_core_match else "0"

        # Update list infomation
        InterruptsTotalNumber += 1
        InterruptsList.append([isr_name, isr_cat, isr_vector, isr_prio, isr_nest, isr_core])
        InterruptsList_per_core[int(isr_core)].append([isr_name, isr_cat, isr_vector, isr_prio, isr_nest])
