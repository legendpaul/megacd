@echo off
set ROOT=%~dp0..
set OUT=%ROOT%\out
set DISC=%OUT%\disc
set LOG=%OUT%\logs
set TOOLSDIR=%ROOT%\tools
if not exist "%LOG%" mkdir "%LOG%"

if not exist "%DISC%" mkdir "%DISC%"
if not exist "%DISC%\DATA" mkdir "%DISC%\DATA"

echo Copying disc template..
xcopy /E /I /Y "%ROOT%\disc_template" "%DISC%" >NUL

if not exist "%DISC%\MAIN.BIN" (
  echo MAIN.BIN missing - Did main build run?
  exit /b 1
)
if not exist "%DISC%\SUB.BIN" (
  echo SUB.BIN missing - Did sub build run?
  exit /b 1
)
if not exist "%DISC%\IP.BIN" (
  echo IP.BIN missing - Did IP build run?
  exit /b 1
)

REM Use the REAL mkisofs.exe
set MKISO=%TOOLSDIR%\mkisofs\mkisofs.exe

if not exist "%MKISO%" (
  echo mkisofs.exe not found
  exit /b 1
)

REM Create ISO - Sega CD needs the IP.BIN prepended via -G flag
echo Creating bootable Sega CD ISO...
"%MKISO%" -o "%OUT%\temp.iso" -V "SEGACD" -sysid "SEGA" -G "%DISC%\IP.BIN" -iso-level 1 -pad "%DISC%" >"%LOG%\mkisofs.log" 2>&1

if errorlevel 1 (
  type "%LOG%\mkisofs.log"
  echo mkisofs failed
  exit /b 1
)

REM Rename temp.iso to game.bin
if exist "%OUT%\game.bin" del "%OUT%\game.bin"
move "%OUT%\temp.iso" "%OUT%\game.bin" >NUL

REM Create CUE file
(
  echo FILE "game.bin" BINARY
  echo   TRACK 01 MODE1/2048
  echo     INDEX 01 00:00:00
) > "%OUT%\game.cue"

echo Disc image ready at %OUT%\game.bin
exit /b 0
