@echo off
set ROOT=%~dp0..
set TOOLSDIR=%ROOT%\tools
set OUT=%ROOT%\out
set LOG=%OUT%\logs
if not exist "%LOG%" mkdir "%LOG%"
if not exist "%OUT%\disc" mkdir "%OUT%\disc"

set GCC_BIN=
for /r "%TOOLSDIR%\m68k-elf" %%f in (m68k-elf-gcc.exe) do if not defined GCC_BIN set GCC_BIN=%%~dpf
if not defined GCC_BIN (
  echo m68k-elf-gcc not found. Ensure bootstrap completed.
  exit /b 1
)
set PATH=%GCC_BIN%;%PATH%

set COMMON_SRC=%ROOT%\src_common\minlib.c

set CFLAGS=-std=c11 -mcpu=68000 -mshort -Os -ffreestanding -nostdlib -I%ROOT%\src_common -I%ROOT%\src_main

set CMD="%GCC_BIN%m68k-elf-gcc.exe" %CFLAGS% -T %ROOT%\build\main.ld %ROOT%\asm\main_vectors.s %COMMON_SRC% %ROOT%\src_main\*.c -Wl,-Map=%LOG%\main.map -o %OUT%\main.elf -lgcc

echo Building main CPU...
%CMD% >%LOG%\main_build.log 2>&1
if errorlevel 1 (
  type %LOG%\main_build.log
  echo Main CPU build failed.
  exit /b 1
)
"%GCC_BIN%m68k-elf-objcopy.exe" -O binary %OUT%\main.elf %OUT%\disc\MAIN.BIN
exit /b 0
