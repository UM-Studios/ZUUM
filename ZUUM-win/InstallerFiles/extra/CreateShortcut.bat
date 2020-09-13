:: This script was originally created by Matthew Wai at TenForums.com/members/matthew-wai.html
:: http://www.tenforums.com/tutorials/57690-create-elevated-shortcut-without-uac-prompt-windows-10-a.html       
:: ************************************************************************************/
@echo off & Title Create an elevated shortcut without a UAC prompt
(Net session >nul 2>&1)||(PowerShell start """%~0""" -verb RunAs & Exit /B)
:: ************************************************************************************/
cd /d "%~dp0"
@ECHO OFF & setlocal
:: echo. & echo.
SET 【Name】=ZUUM
:: echo. & echo.
SET 【Path】=%~dp0ZUUM.exe

:: echo.
:: echo       The script is creating an elevated task with highest privileges.
:: echo       Please wait for a while.
:: echo.
:: ************************************************************************************/
For /f "tokens=*" %%I in ('WhoAmI /user') Do (for %%A in (%%~I) Do (set "【SID】=%%A"))

Set "Path=""%【Path】%"""
Set "(Path)=%Path: ""=""%"
Set "【Task_name】=%【Name】%"	
IF EXIST "%temp%\%【Task_name】%.xml" (DEL "%temp%\%【Task_name】%.xml")	
IF EXIST "%temp%\Task.vbs" (DEL "%temp%\Task.vbs")	

::echo %(Path)%
::pause

echo Set X=CreateObject("Scripting.FileSystemObject") >> "%temp%\Task.vbs"
echo Set Z=X.CreateTextFile("%temp%\%【Task_name】%.xml",True,True)>> "%temp%\Task.vbs"
Set "W=echo Z.writeline "	
(	
%W%"<?xml version=""1.0"" encoding=""UTF-16""?>"
%W%"<Task version=""1.4"" xmlns=""http://schemas.microsoft.com/windows/2004/02/mit/task"">"
%W%"<RegistrationInfo>"
%W%"<Author>%username%</Author>"
%W%"<Description>To run the application/CMD script as an administrator with no UAC prompt.</Description>"
%W%"</RegistrationInfo>"
%W%"<Triggers />"
%W%"<Principals>"
%W%"<Principal id=""Author"">"
%W%"<UserId>%【SID】%</UserId>"
%W%"<LogonType>InteractiveToken</LogonType>"
%W%"<RunLevel>HighestAvailable</RunLevel>"
%W%"</Principal>"
%W%"</Principals>"
%W%"<Settings>"
%W%"<MultipleInstancesPolicy>IgnoreNew</MultipleInstancesPolicy>"
%W%"<DisallowStartIfOnBatteries>false</DisallowStartIfOnBatteries>"
%W%"<StopIfGoingOnBatteries>true</StopIfGoingOnBatteries>"
%W%"<AllowHardTerminate>true</AllowHardTerminate>"
%W%"<StartWhenAvailable>false</StartWhenAvailable>"
%W%"<RunOnlyIfNetworkAvailable>false</RunOnlyIfNetworkAvailable>"
%W%"<IdleSettings>"
%W%"<StopOnIdleEnd>true</StopOnIdleEnd>"
%W%"<RestartOnIdle>false</RestartOnIdle>"
%W%"</IdleSettings>"
%W%"<AllowStartOnDemand>true</AllowStartOnDemand>"
%W%"<Enabled>true</Enabled>"
%W%"<Hidden>false</Hidden>"
%W%"<RunOnlyIfIdle>false</RunOnlyIfIdle>"
%W%"<DisallowStartOnRemoteAppSession>false</DisallowStartOnRemoteAppSession>"
%W%"<UseUnifiedSchedulingEngine>true</UseUnifiedSchedulingEngine>"
%W%"<WakeToRun>false</WakeToRun>"
%W%"<ExecutionTimeLimit>PT72H</ExecutionTimeLimit>"
%W%"<Priority>7</Priority>"
%W%"</Settings>"
%W%"<Actions Context=""Author"">"
%W%"<Exec>"
%W%"<Command>%(Path)%</Command>"
%W%"</Exec>"
%W%"</Actions>"
%W%"</Task>"
)>> "%temp%\Task.vbs"	
echo Z.Close >> "%temp%\Task.vbs"	
"%temp%\Task.vbs"	
Del "%temp%\Task.vbs"	
"%WinDir%\System32\schtasks.exe" /delete /tn "Apps\%【Task_name】%" /F
"%WinDir%\System32\schtasks.exe" /create /xml "%temp%\%【Task_name】%.xml" /tn "Apps\%【Task_name】%"
DEL "%temp%\%【Task_name】%.xml"

IF EXIST "%temp%\Shortcut.vbs" (DEL "%temp%\Shortcut.vbs")
(echo Set A = CreateObject^("WScript.Shell"^) & echo Desktop = A.SpecialFolders^("Desktop"^) & echo Set B = A.CreateShortcut^(Desktop ^& "\%【Name】%.lnk"^) & echo B.IconLocation = "%【Path】%" & echo B.TargetPath = "%windir%\System32\schtasks.exe" & echo B.Arguments = "/run /tn ""Apps\%【Task_name】%""" & echo B.Save & echo WScript.Quit)> "%temp%\Shortcut.vbs"
"%temp%\Shortcut.vbs"	

Del "%temp%\Shortcut.vbs"

Exit

:: If errorlevel 1 (Del "%temp%\Shortcut.vbs" & echo.
:: echo  ====================================================================================
:: echo      The script has failed to complete the operation.
:: echo      Please press any key to close this message.
:: echo  ====================================================================================
:: pause > nul & Exit) else (Del "%temp%\Shortcut.vbs" & echo.
:: echo  ====================================================================================
:: echo      The shortcut "%【Name】%" has been created on the desktop.
:: echo      You may double-click on it to run the application.
:: echo      Please press any key to close this message.
:: echo  ====================================================================================
:: pause > nul & Exit)
