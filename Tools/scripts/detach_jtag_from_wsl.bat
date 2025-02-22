@echo off

usbipd.exe detach --hardware-id 303a:1001
usbipd.exe unbind --hardware-id 303a:1001


pause