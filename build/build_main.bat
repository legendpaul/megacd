@echo off
set ROOT=%~dp0..
set TOOLSDIR=%ROOT%\tools
set OUT=%ROOT%\out
set LOG=%OUT%\logs
if not exist "%LOG%" mkdir "%LOG%"
set PATH=%TOOLSDIR%\m68k-elf\bin;%PATH%

echo Building main CPU...
"%TOOLSDIR%\m68k-elf\bin\m68k-elf-gcc.exe" -std=c11 -mcpu=68000 -mshort -Os -ffreestanding -nostdlib -I%ROOT%\src_common -I%ROOT%\src_main -T %ROOT%\build\main.ld %ROOT%\asm\main_vectors.s %ROOT%\src_main\*.c -Wl,-Map=%LOG%\main.map -o %OUT%\main.elf >%LOG%\main_build.log 2>&1
if errorlevel 1 (
  type %LOG%\main_build.log
  echo Main CPU build failed.
  exit /b 1
)
"%TOOLSDIR%\m68k-elf\bin\m68k-elf-objcopy.exe" -O binary %OUT%\main.elf %OUT%\disc\MAIN.BIN
exit /b 0
