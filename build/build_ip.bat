@echo off
setlocal enabledelayedexpansion
set ROOT=%~dp0..
set TOOLSDIR=%ROOT%\tools
set OUT=%ROOT%\out
set LOG=%OUT%\logs
if not exist "%LOG%" mkdir "%LOG%"
if not exist "%OUT%\disc" mkdir "%OUT%\disc"

set GCC_BIN=%TOOLSDIR%\m68k-elf\bin\
if not exist "%GCC_BIN%m68k-elf-as.exe" (
  echo m68k-elf-as not found - Ensure bootstrap completed
  exit /b 1
)
set PATH=%GCC_BIN%;%PATH%

echo Building IP.BIN boot sector..
"%GCC_BIN%m68k-elf-as.exe" "%ROOT%\src_ip\ip.s" -o "%OUT%\ip.o" >"%LOG%\ip_build.log" 2>&1
if errorlevel 1 (
  type "%LOG%\ip_build.log"
  echo IP build failed
  exit /b 1
)
"%GCC_BIN%m68k-elf-ld.exe" -T "%ROOT%\build\ip.ld" "%OUT%\ip.o" -o "%OUT%\ip.elf" >>"%LOG%\ip_build.log" 2>&1
if errorlevel 1 (
  type "%LOG%\ip_build.log"
  echo IP link failed
  exit /b 1
)
"%GCC_BIN%m68k-elf-objcopy.exe" -O binary "%OUT%\ip.elf" "%OUT%\disc\IP.BIN"
exit /b 0
