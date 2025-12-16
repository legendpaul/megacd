@echo off
set ROOT=%~dp0..
set TOOLSDIR=%ROOT%\tools
set OUT=%ROOT%\out
set LOG=%OUT%\logs
if not exist "%LOG%" mkdir "%LOG%"
set PATH=%TOOLSDIR%\m68k-elf\bin;%PATH%

echo Building sub CPU...
"%TOOLSDIR%\m68k-elf\bin\m68k-elf-gcc.exe" -std=c11 -mcpu=68000 -mshort -Os -ffreestanding -nostdlib -I%ROOT%\src_common -I%ROOT%\src_sub -T %ROOT%\build\sub.ld %ROOT%\asm\sub_vectors.s %ROOT%\src_sub\*.c -Wl,-Map=%LOG%\sub.map -o %OUT%\sub.elf >%LOG%\sub_build.log 2>&1
if errorlevel 1 (
  type %LOG%\sub_build.log
  echo Sub CPU build failed.
  exit /b 1
)
"%TOOLSDIR%\m68k-elf\bin\m68k-elf-objcopy.exe" -O binary %OUT%\sub.elf %OUT%\disc\SUB.BIN
exit /b 0
