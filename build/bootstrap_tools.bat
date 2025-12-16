@echo off
setlocal ENABLEDELAYEDEXPANSION

REM Bootstrap downloads for Mega-CD starter
set ROOT=%~dp0..
set TOOLSDIR=%ROOT%\tools
set LOGDIR=%ROOT%\out\logs
if not exist "%LOGDIR%" mkdir "%LOGDIR%"
set BOOTLOG=%LOGDIR%\bootstrap.log

call :ensure_dir "%TOOLSDIR%"

set DOWNLOAD_PS=powershell -NoProfile -ExecutionPolicy Bypass -Command

REM Tool definitions
set GCC_URL=https://github.com/Stephane-D/sgdk/releases/download/v1.80/gcc-13.2.0-m68k-elf-win32.zip
set GCC_SHA=9da661b63e2ef3c8821a2c776c3a12a7c3560ad956c8c1b85cfb010adeacd24a
set GCC_ARCHIVE=%TOOLSDIR%\gcc-m68k.zip
set GCC_DIR=%TOOLSDIR%\m68k-elf

set MKISO_URL=https://github.com/johnlane/cdrtools-latest/releases/download/3.02a09/cdrtools-3.02a09-win32-bin.zip
set MKISO_SHA=7bc0417e9e3b7c33ac11472edbba7d6851124c596cb8f17968e2db279b1091d3
set MKISO_ARCHIVE=%TOOLSDIR%\mkisofs.zip
set MKISO_DIR=%TOOLSDIR%\mkisofs

set IPGEN_URL=https://github.com/sega-dreamcast/ipbin4j/releases/download/v0.1.0/ipbin4j-win32.zip
set IPGEN_SHA=4a7af9a5dd86e41bc4f58ac09bffcea2c5831e3d8fc6b50da2ac1a5e0bf80f7c
set IPGEN_ARCHIVE=%TOOLSDIR%\ipgen.zip
set IPGEN_DIR=%TOOLSDIR%\ipbin

call :download_and_unpack "m68k-elf GCC" %GCC_URL% %GCC_ARCHIVE% %GCC_SHA% %GCC_DIR%
call :download_and_unpack "mkisofs" %MKISO_URL% %MKISO_ARCHIVE% %MKISO_SHA% %MKISO_DIR%
call :download_and_unpack "IP.BIN generator" %IPGEN_URL% %IPGEN_ARCHIVE% %IPGEN_SHA% %IPGEN_DIR%

echo Bootstrap complete.
exit /b 0

:download_and_unpack
set NAME=%~1
set URL=%~2
set ARCHIVE=%~3
set SHA_EXPECT=%~4
set DEST=%~5

if exist "%DEST%\_ok" (
  echo %NAME% already present.>>"%BOOTLOG%"
  goto :eof
)

echo Downloading %NAME% from %URL%>>"%BOOTLOG%"
if not exist "%ARCHIVE%" (
  %DOWNLOAD_PS% "Invoke-WebRequest -Uri '%URL%' -OutFile '%ARCHIVE%'" >>"%BOOTLOG%" 2>&1
  if errorlevel 1 (
    echo Failed to download %NAME%.
    exit /b 1
  )
)

%DOWNLOAD_PS% "$hash = (Get-FileHash '%ARCHIVE%' -Algorithm SHA256).Hash; if ($hash.ToLower() -ne '%SHA_EXPECT%') { Write-Error 'SHA mismatch expected %SHA_EXPECT% got ' + $hash }" >>"%BOOTLOG%" 2>&1
if errorlevel 1 (
  echo SHA256 mismatch for %NAME%.
  del "%ARCHIVE%" >NUL 2>&1
  exit /b 1
)

call :ensure_dir "%DEST%"
powershell -NoProfile -ExecutionPolicy Bypass -Command "Add-Type -AssemblyName System.IO.Compression.FileSystem; [System.IO.Compression.ZipFile]::ExtractToDirectory('%ARCHIVE%', '%DEST%');" >>"%BOOTLOG%" 2>&1
if errorlevel 1 (
  echo Failed to extract %NAME%.
  exit /b 1
)

echo ok>%DEST%\_ok

echo %NAME% ready at %DEST%>>"%BOOTLOG%"
goto :eof

:ensure_dir
if not exist %1 mkdir %1
exit /b 0
