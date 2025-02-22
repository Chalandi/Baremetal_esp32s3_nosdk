@echo off

echo Checking for processes using port 3333...

REM Find the process ID (PID) using port 3333
for /f "tokens=5" %%a in ('netstat -ano ^| findstr :3333') do (
    set PID=%%a
)

REM Check if PID is set
if defined PID (
    echo Killing process with PID %PID%...
    taskkill /PID %PID% /F
    echo Process with PID %PID% terminated.
) else (
    echo No process found using port 3333.
)

echo Done.
pause
