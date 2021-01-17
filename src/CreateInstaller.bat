@echo off
cd /d "%~dp0"
IF [%1]==[] (echo missing version name
exit /b 1) else (set ver=%1)
if exist "..\Releases\ZUUM-%ver%-setup-win.exe" (echo version already exists
exit /b 1)
echo removing old directory
@RD /S /Q "build\win\Zuum"
echo compiling...
python setup.py build
echo creating SFX
@"%ProgramFiles%\WinRAR\WinRAR.exe" a -afrar -cfg- -ed -ep1 -iadm -k -m5 -r -tl -iicon"InstallerFiles\zmlogo.ico" -iimg"InstallerFiles\zmlogo.png" "-sfx%ProgramFiles%\WinRAR\Default.sfx" -z"InstallerFiles\InstallerComment.txt" "..\Releases\ZUUM-%ver%-setup-win.exe" "build\win\Zuum\*"