@echo off
cd /d "%~dp0"
IF [%1]==[] (echo missing version name
exit /b 1) else (set ver=%1)
if exist "Releases\ZUUM-%ver%-setup-win.exe" (echo version already exists
exit /b 1)
echo removing old directory
@RD /S /Q ZUUM.dist
echo compiling...
python -m nuitka --follow-imports --mingw64 --standalone --plugin-enable=tk-inter --windows-disable-console --windows-icon="InstallerFiles\zmlogo.ico" ZUUM.py
xcopy /q /y InstallerFiles\extra ZUUM.dist
echo deleting api-ms-win-s
del ZUUM.dist\api-ms-win-*
echo setting manifest
@"C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x64\mt.exe" -nologo -manifest "InstallerFiles\ZUUM.exe.manifest" -outputresource:"ZUUM.dist\ZUUM.exe";#1
echo creating SFX
@"%ProgramFiles%\WinRAR\WinRAR.exe" a -afrar -cfg- -ed -ep1 -iadm -k -m5 -r -tl -iicon"InstallerFiles\zmlogo.ico" -iimg"InstallerFiles\zmlogo.png" "-sfx%ProgramFiles%\WinRAR\Default.sfx" -z"InstallerFiles\InstallerComment.txt" "Releases\ZUUM-%ver%-setup-win.exe" "ZUUM.dist\*"