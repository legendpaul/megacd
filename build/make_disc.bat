@echo off
set ROOT=%~dp0..
set OUT=%ROOT%\out
set DISC=%OUT%\disc
set LOG=%OUT%\logs
set TOOLSDIR=%ROOT%\tools
if not exist "%LOG%" mkdir "%LOG%"

if exist "%DISC%" rmdir /S /Q "%DISC%"
mkdir "%DISC%"
mkdir "%DISC%\DATA"

echo Copying disc template...
xcopy /E /I /Y "%ROOT%\disc_template" "%DISC%" >NUL

if not exist "%DISC%\MAIN.BIN" (
  echo MAIN.BIN missing. Did main build run?
  exit /b 1
)
if not exist "%DISC%\SUB.BIN" (
  echo SUB.BIN missing. Did sub build run?
  exit /b 1
)

REM Generate IP.BIN
if exist "%TOOLSDIR%\ipbin\ipbin.exe" (
  "%TOOLSDIR%\ipbin\ipbin.exe" -o "%DISC%\IP.BIN" -p "MEGACD" -d "MINNKA STARTER" -v "1.00" >"%LOG%\ipbin.log" 2>&1
) else (
  powershell -NoProfile -ExecutionPolicy Bypass -Command "$bytes = New-Object byte[] 2048; $bytes[0]=0x41; $bytes[1]=0x42; [IO.File]::WriteAllBytes('%DISC%\\IP.BIN',$bytes)" >>"%LOG%\ipbin.log" 2>&1
)

set MKISO=%TOOLSDIR%\mkisofs\cdrtools-3.02a09-win32-bin\mkisofs.exe
if not exist "%MKISO%" set MKISO=%TOOLSDIR%\mkisofs\mkisofs.exe

if not exist "%MKISO%" (
  echo mkisofs not found.
  exit /b 1
)

"%MKISO%" -quiet -iso-level 1 -o "%OUT%\game.bin" -G "%DISC%\IP.BIN" -pad "%DISC%" >"%LOG%\mkisofs.log" 2>&1
if errorlevel 1 (
  type "%LOG%\mkisofs.log"
  echo mkisofs failed.
  exit /b 1
)

echo FILE "game.bin" BINARY>"%OUT%\game.cue"
echo   TRACK 01 MODE1/2048>>"%OUT%\game.cue"
echo     INDEX 01 00:00:00>>"%OUT%\game.cue"

echo Disc image ready at %OUT%\game.bin
exit /b 0
