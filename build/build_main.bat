@echo off
setlocal enabledelayedexpansion
set ROOT=%~dp0..
set TOOLSDIR=%ROOT%\tools
set OUT=%ROOT%\out
set LOG=%OUT%\logs
if not exist "%LOG%" mkdir "%LOG%"
if not exist "%OUT%\disc" mkdir "%OUT%\disc"

set GCC_BIN=%TOOLSDIR%\m68k-elf\bin\
if not exist "%GCC_BIN%m68k-elf-gcc.exe" (
  echo m68k-elf-gcc not found - Ensure bootstrap completed
  exit /b 1
)
set PATH=%GCC_BIN%;%PATH%

set COMMON_SRC=%ROOT%\src_common\minlib.c

set CFLAGS=-std=c11 -mcpu=68000 -mshort -Os -ffreestanding -nostdlib -I%ROOT%\src_common -I%ROOT%\src_main

set SRC_FILES=
for %%f in (%ROOT%\src_main\*.c) do set SRC_FILES=!SRC_FILES! "%%f"

echo Building main CPU..
"%GCC_BIN%m68k-elf-gcc.exe" %CFLAGS% -T "%ROOT%\build\main.ld" "%ROOT%\asm\main_vectors.s" "%COMMON_SRC%" %SRC_FILES% -Wl,-Map="%LOG%\main.map" -o "%OUT%\main.elf" -lgcc >"%LOG%\main_build.log" 2>&1
if errorlevel 1 (
  type "%LOG%\main_build.log"
  echo Main CPU build failed
  exit /b 1
)
"%GCC_BIN%m68k-elf-objcopy.exe" -O binary "%OUT%\main.elf" "%OUT%\disc\MAIN.BIN"
exit /b 0