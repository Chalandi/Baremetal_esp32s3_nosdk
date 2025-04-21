import sys
import re
import os
from jinja2 import Template
from tabulate import tabulate

# Globals
EventsTotalNumber = 0
EventsList = []

def OilEventsParser(args, Oilfile):

    # Globals
    global EventsTotalNumber

    # Search for event names and properties
    event_pattern = re.compile(r"EVENT\s+(\w+)\s*\{([\s\S]*?)\};", re.MULTILINE)
    event_mask_pattern  = re.compile(r"MASK\s*=\s*(\w+);")
    events = event_pattern.findall(Oilfile)
    
    
    for event in events:
        # Get the event name and the event properties
        event_name, event_properties = event
        event_mask_match = event_mask_pattern.search(event_properties)
        event_mask       = event_mask_match.group(1) if event_mask_match else None
        EventsTotalNumber += 1
        if event_mask == "AUTO":
            event_mask = 0
        elif event_mask.startswith('0x'):
            event_mask = int(event_mask[2:], 16)
        else:
            event_mask = int(event_mask, 10)
        # Check if the event_mask has only one bit set
        if (int(event_mask) & (int(event_mask) - 1)) != 0 :
            print(event_name+" mask value (" + str(hex(event_mask)) + ") is incorrect, more than one bit is set !")
            exit()
        # check if the event_mask is higher than 32-bit
        if (int(event_mask) > 0xffffffff):
            print(event_name+" mask value (" + str(hex(event_mask)) + ") does not fit in 32-bit !")
            exit()

        # Fill the event list
        EventsList.append([event_name, hex(event_mask)])
    #print(f"EventsList = {EventsList}")

def OilEventsGetMask(event):
    for idx in range(EventsTotalNumber):
        if event == EventsList[idx][0]:
            return (int(EventsList[idx][1], base=0))

    print("OilEventsGetMask failed !")
    exit()


def OilEventsSetMask(event, mask):
    global EventsTotalNumber
    for idx in range(EventsTotalNumber):
        if event == EventsList[idx][0]:
            EventsList[idx][1] = str(hex(mask))


def OilEventsGenMask(GlobalMaskEvent):
    for i in range(31):
        EventGenMask = 1 << i
        if(EventGenMask & GlobalMaskEvent) == 0:
            return(EventGenMask)
    print("OilEventsGenMask failed !")
    exit()
