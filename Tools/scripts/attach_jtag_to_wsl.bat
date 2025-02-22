@echo off

usbipd bind --hardware-id 303a:1001 --force
usbipd.exe attach -w --hardware-id 303a:1001

pause

