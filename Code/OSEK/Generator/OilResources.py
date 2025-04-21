import sys
import re
import os
from jinja2 import Template
from tabulate import tabulate

# Globals
ResourcesTotalNumber = 0
ResourcesList = []
ResourcesList_per_core = [[], []]
PrioritiesList_per_core = [[], []]

def OilResourcesParser(args, Oilfile):
    
    global ResourcesTotalNumber, ResourcesList

    # Search for resource names and properties
    resource_pattern      = re.compile(r"RESOURCE\s+(\w+)\s*\{([\s\S]*?)\};", re.MULTILINE)
    resource_prop_pattern = re.compile(r"RESOURCEPROPERTY\s*=\s*(\w+);")
    resource_pinned_core_pattern = re.compile(r"CORE\s*=\s*(\w+);")
    resources             = resource_pattern.findall(Oilfile)

    # Parse the resource list
    for resource in resources:
        resource_name, resource_properties = resource
        resource_prop_match = resource_prop_pattern.search(resource_properties)
        resource_prop       = resource_prop_match.group(1) if resource_prop_match else None

        resource_pinned_core_match = resource_pinned_core_pattern.search(resource_properties)
        resource_pinned_core       = resource_pinned_core_match.group(1) if resource_pinned_core_match else "0"

        if resource_prop != "STANDARD" :
            print(f"The resource {resource_name} must have \"STANDARD\" as property !")
            exit()
        
        ResourcesTotalNumber += 1
        # Format: [Name, prio, property, mask, core]
        ResourcesList.append([resource_name, 0, resource_prop, 0, resource_pinned_core])
        ResourcesList_per_core[int(resource_pinned_core)].append([resource_name, 0, resource_prop, 0])

    #print(f"ResourcesList = {ResourcesList}")
    #print(f"ResourcesList_per_core[0] = {ResourcesList_per_core[0]}")
    #print(f"ResourcesList_per_core[1] = {ResourcesList_per_core[1]}")