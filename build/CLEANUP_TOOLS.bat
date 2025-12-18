@echo off
echo Cleaning up corrupted tool downloads...

if exist "C:\svn\git\megacd\tools\gcc-m68k.zip" (
    echo Deleting corrupted gcc-m68k.zip...
    del /F /Q "C:\svn\git\megacd\tools\gcc-m68k.zip"
)

if exist "C:\svn\git\megacd\tools\m68k-elf" (
    echo Removing m68k-elf directory...
    rmdir /S /Q "C:\svn\git\megacd\tools\m68k-elf"
)

if exist "C:\svn\git\megacd\tools\mkisofs.zip" (
    echo Checking mkisofs.zip
    for %%A in ("C:\svn\git\megacd\tools\mkisofs.zip") do if %%~zA LSS 1000 (
        echo Deleting corrupted mkisofs.zip
        del /F /Q "C:\svn\git\megacd\tools\mkisofs.zip"
    )
)

if exist "C:\svn\git\megacd\tools\ipgen.zip" (
    echo Checking ipgen.zip
    for %%A in ("C:\svn\git\megacd\tools\ipgen.zip") do if %%~zA LSS 1000 (
        echo Deleting corrupted ipgen.zip
        del /F /Q "C:\svn\git\megacd\tools\ipgen.zip"
    )
)

echo.
echo Cleanup complete! Now run ONE_CLICK_BUILD.bat again.
pause
