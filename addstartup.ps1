# Create a Shortcut with Windows PowerShell
$TargetFile = Join-Path -Path (Get-Location) -ChildPath "ZuumScheduler.exe"
$ShortcutFile = "$env:APPDATA\Microsoft\Windows\Start Menu\Programs\Startup\ZuumScheduler.lnk"
$WScriptShell = New-Object -ComObject WScript.Shell
$Shortcut = $WScriptShell.CreateShortcut($ShortcutFile)
$Shortcut.TargetPath = $TargetFile
$Shortcut.Save()