import sys
import re
import os
from jinja2 import Template
from tabulate import tabulate

# Globals
alarm_alarmtime   = "-"
alarm_cycletime   = "-"
alarm_event       = "-"
alarm_task        = "-"
alarm_action      = "-"
alarm_autostart   = "-"
alarm_callback    = "-"
AlarmsTotalNumber = 0
AlarmsList        = []
OsAlarmsList_per_core = [[], []]

def OilAlarmsParser(args, Oilfile):
     
    global AlarmsTotalNumber
    global alarm_alarmtime
    global alarm_cycletime
    global alarm_event    
    global alarm_task     
    global alarm_action   
    global alarm_autostart
    global alarm_callback 

    alarm_pattern = re.compile(r"ALARM\s+(\w+)\s+{((?:[^{}]|{(?:[^{}]|{.*?})*})*)};", re.DOTALL | re.MULTILINE)
    alarms = alarm_pattern.findall(Oilfile, re.DOTALL)
    for alarm in alarms:
        alarm_name, alarm_body = alarm
        #print(alarm_body)
        # search for alarm action
        alarm_action_pattern = re.compile(r"ACTION\s*=\s*(\w+)", re.MULTILINE)
        alarm_action_match   = alarm_action_pattern.search(alarm_body)
        alarm_action         = alarm_action_match.group(1) if alarm_action_match else None

        alarm_pinned_core_pattern = re.compile(r"CORE\s*=\s*(\w+);")
        alarm_pinned_core_match   = alarm_pinned_core_pattern.search(alarm_body)
        alarm_pinned_core         = alarm_pinned_core_match.group(1) if alarm_pinned_core_match else "0"

        # Check for syntax error
        if alarm_action == None              \
           or (alarm_action !="SETEVENT"     \
           and alarm_action !="ACTIVATETASK" \
           and alarm_action !="ALARMCALLBACK") :
           print(f"The alarm {alarm_name} has wrong action !")
           exit()

        # Search for Event, Task and Callback names
        alarm_event_pattern    = re.compile(r"EVENT\s*=\s*(\w+);", re.MULTILINE)
        alarm_task_pattern     = re.compile(r"TASK\s*=\s*(\w+);", re.MULTILINE)
        alarm_callback_pattern = re.compile(r"ALARMCALLBACKNAME\s*=\s*\"(\w+)\";", re.MULTILINE)
        
        alarm_event_match    = alarm_event_pattern.search(alarm_body)
        alarm_event          = alarm_event_match.group(1) if alarm_event_match else "-"
        alarm_task_match     = alarm_task_pattern.search(alarm_body)
        alarm_task           = alarm_task_match.group(1) if alarm_task_match else "-"
        alarm_callback_match = alarm_callback_pattern.search(alarm_body)
        alarm_callback       = alarm_callback_match.group(1) if alarm_callback_match else "-"
        
        # Check for syntax error
        if alarm_action =="SETEVENT" and (alarm_event == "-" or alarm_task == "-" or alarm_callback != "-") :
            print(f"The timer {alarm_name} has a wrong configuration !")
            exit()

        if alarm_action =="ACTIVATETASK" and (alarm_event != "-" or alarm_task == "-" or alarm_callback != "-") :
            print(f"The timer {alarm_name} has a wrong configuration !")
            exit()

        if alarm_action =="ALARMCALLBACK" and (alarm_event != "-" or alarm_task != "-" or alarm_callback == "-") :
            print(f"The timer {alarm_name} has a wrong configuration !")
            exit()

        # Search for timer autostart attribute
        alarm_autostart_pattern = re.compile(r"AUTOSTART\s*=\s*(\w+)", re.MULTILINE)
        alarm_autostart_match   = alarm_autostart_pattern.search(alarm_body)
        alarm_autostart         = alarm_autostart_match.group(1) if alarm_autostart_match else None

        if alarm_autostart == "TRUE" :
            # Search for autostart ALARMTIME and CYCLETIME configuration
            alarm_alarmtime_pattern = re.compile(r"ALARMTIME\s*=\s*(\w+);", re.MULTILINE)
            alarm_cycletime_pattern = re.compile(r"CYCLETIME\s*=\s*(\w+);", re.MULTILINE)
            alarm_alarmtime_match = alarm_alarmtime_pattern.search(alarm_body)
            alarm_cycletime_match = alarm_cycletime_pattern.search(alarm_body)
            alarm_alarmtime       = alarm_alarmtime_match.group(1) if alarm_alarmtime_match else "-"
            alarm_cycletime       = alarm_cycletime_match.group(1) if alarm_cycletime_match else "-"

            if alarm_alarmtime.startswith('0x'):
                alarm_alarmtime = int(alarm_alarmtime[2:], 16)
            else:
                alarm_alarmtime = int(alarm_alarmtime, 10)

            if alarm_cycletime.startswith('0x'):
                alarm_cycletime = int(alarm_cycletime[2:], 16)
            else:
                alarm_cycletime = int(alarm_cycletime, 10)

        AlarmsTotalNumber += 1
        AlarmsList.append([alarm_name, alarm_alarmtime, alarm_cycletime, alarm_event, alarm_task, alarm_action, alarm_autostart, alarm_callback, alarm_pinned_core])
        OsAlarmsList_per_core[int(alarm_pinned_core)].append([alarm_name, alarm_alarmtime, alarm_cycletime, alarm_event, alarm_task, alarm_action, alarm_autostart, alarm_callback])
    #print(f"OsAlarmsList_per_core[0] = {OsAlarmsList_per_core[0]}")
    #print(f"OsAlarmsList_per_core[1] = {OsAlarmsList_per_core[1]}")