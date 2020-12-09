@echo off & Title Add Windows Defender Exception

echo ZUUM uses administrator priviledges to access Task Scheduler, meaning it will likely get caught by antivirus software.

choice /n /m "Add an exception to Windows Defender? (y/n)"
if %errorlevel% == 1 (
    powershell -command "add-mppreference -exclusionprocess 'ZuumScheduler.exe'"
    echo. & echo Exclusion added. To undo this, check here: https://support.microsoft.com/en-us/help/4028485/windows-10-add-an-exclusion-to-windows-security
    echo. & echo If you have other antivirus software, you'll have to add an exception for C:\Program Files^(x86^)\ZUUM\ZuumScheduler.exe manually.
) else echo. & echo No exclusion added. If ZUUM gets disabled by antivirus software, you'll have to add an exception for C:\Program Files(x86)\ZUUM\ZuumScheduler.exe manually.
pause