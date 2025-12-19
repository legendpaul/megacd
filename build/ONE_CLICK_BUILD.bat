@echo off
set ROOT=%~dp0..
set OUT=%ROOT%\out
set TOOLSDIR=%ROOT%\tools
set LOG=%OUT%\logs
if exist "%OUT%\disc" rmdir /S /Q "%OUT%\disc" >NUL 2>&1
if not exist "%LOG%" mkdir "%LOG%"
if not exist "%OUT%" mkdir "%OUT%"

echo Checking tools...
if not exist "%TOOLSDIR%\m68k-elf\_ok" (
  call "%ROOT%\build\bootstrap_tools.bat"
  if errorlevel 1 exit /b 1
)

if not exist "%OUT%\disc" mkdir "%OUT%\disc"

call "%ROOT%\build\build_ip.bat"
if errorlevel 1 goto :fail
call "%ROOT%\build\build_sub.bat"
if errorlevel 1 goto :fail
call "%ROOT%\build\build_main.bat"
if errorlevel 1 goto :fail
call "%ROOT%\build\make_disc.bat"
if errorlevel 1 goto :fail

echo SUCCESS! Outputs in %OUT%
exit /b 0
:fail
echo Build failed. Check logs under out\logs.
exit /b 1
