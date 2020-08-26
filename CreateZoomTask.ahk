#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

1E := 
2E :=
3E :=
4E :=
5E :=
6E :=
7E :=
1T :=
2T :=
3T :=
4T :=
5T :=
6T :=
7T :=

template := "<?xml version=" . """" . "1.0" . """" . " encoding=" . """" . "UTF-16" . """" . "?>`n<Task version=" . """" . "1.4" . """" . " xmlns=" . """" . "http://schemas.microsoft.com/windows/2004/02/mit/task" . """" . ">`n  <RegistrationInfo>`n    <Date>[NOW]</Date>`n    <Author>CREATEZOOMTASK</Author>`n    <URI>\[MEETINGNAME]</URI>`n  </RegistrationInfo>`n  <Triggers>    [TRIGGERS]`n  </Triggers>`n  <Principals>`n    <Principal id=" . """" . "Author" . """" . ">`n      <UserId>S-1-5-21-391003967-3623282886-662930924-1001</UserId>`n      <LogonType>InteractiveToken</LogonType>`n      <RunLevel>LeastPrivilege</RunLevel>`n    </Principal>`n  </Principals>`n  <Settings>`n    <MultipleInstancesPolicy>StopExisting</MultipleInstancesPolicy>`n    <DisallowStartIfOnBatteries>false</DisallowStartIfOnBatteries>`n    <StopIfGoingOnBatteries>false</StopIfGoingOnBatteries>`n    <AllowHardTerminate>true</AllowHardTerminate>`n    <StartWhenAvailable>false</StartWhenAvailable>`n    <RunOnlyIfNetworkAvailable>false</RunOnlyIfNetworkAvailable>`n    <IdleSettings>`n      <StopOnIdleEnd>true</StopOnIdleEnd>`n      <RestartOnIdle>false</RestartOnIdle>`n    </IdleSettings>`n    <AllowStartOnDemand>true</AllowStartOnDemand>`n    <Enabled>true</Enabled>`n    <Hidden>false</Hidden>`n    <RunOnlyIfIdle>false</RunOnlyIfIdle>`n    <DisallowStartOnRemoteAppSession>false</DisallowStartOnRemoteAppSession>`n    <UseUnifiedSchedulingEngine>true</UseUnifiedSchedulingEngine>`n    <WakeToRun>false</WakeToRun>`n    <ExecutionTimeLimit>PT1H</ExecutionTimeLimit>`n    <Priority>7</Priority>`n  </Settings>`n  <Actions Context=" . """" . "Author" . """" . ">`n    <Exec>`n      <Command>%APPDATA%\Zoom\bin\Zoom.exe</Command>`n      <Arguments>[ARGS]</Arguments>`n    </Exec>`n  </Actions>`n</Task>"

TriggerTemplate := "`n<CalendarTrigger>`n    <StartBoundary>[TIME]</StartBoundary>`n    <Enabled>true</Enabled>`n    <ScheduleByWeek>`n        <DaysOfWeek>`n            <[DAY] />`n        </DaysOfWeek>`n        <WeeksInterval>1</WeeksInterval>`n    </ScheduleByWeek>`n</CalendarTrigger>"

Gui,add,Text,x2 y0 w120 h13 vNameText, Meeting Name:
Gui,add,Edit,x2 y15 h21 vMeetingName,
Gui,Add,Text,x2 y50 w120 h13 vPasteText,Paste Zoom link:
Gui,Add,Edit,x2 y65 w400 h21 vLink,
Gui,Add,Text,x2 y100 w100 h13 vSelectDay,Select Day:
Gui,Add,DropDownList,x2 y120 w120 vDay gUpdateGui, Sunday||Monday|Tuesday|Wednesday|Thursday|Friday
addChecks()
addTimes()
Gui,Add,Button, x1 y150 w80 gSubmit, OK
gosub UpdateGui
Gui,Show,,Task Information
return

Submit:
	Gui, submit, nohide
	argument := RegExReplace(Link, "(?:(?:https?:\/\/)?zoom\.us\/j\/)(\d+)\??(pwd=[a-zA-Z0-9]+)?", """" . "--url=zoommtg://zoom.us/join?action=join&amp;confno=$1&amp;$2" . """")
	MeetingName := StrReplace(MeetingName, " ")
	FormatTime, now, A_Now, yyyy-MM-dd'T'HH:mm:ss
	withNow := StrReplace(template, "[NOW]", now)
	withName := StrReplace(withNow, "[MEETINGNAME]", MeetingName)
	loop, 7 {
		if (%A_Index%E) {
			;seltime := %A_Index%T
			;FormatTime, selectedtime, seltime, yyyy-MM-dd'T'HH:mm:ss
			selectedtime := RegExReplace(%A_Index%T, "(\d{4})(\d{2})(\d{2})(\d{2})(\d{2})(\d{2})", "$1-$2-$3T$4:$5:$6" )
			append := StrReplace(TriggerTemplate, "[TIME]", selectedtime)
			append := StrReplace(append, "[DAY]", dayName(A_Index))
			triggers .= append
		}
	}
	withTriggers := StrReplace(withName, "[TRIGGERS]", triggers)
	withArgs := StrReplace(withTriggers, "[ARGS]", argument)
	;msgbox % 1T
	filename := "ZoomJoin" . MeetingName
	FileAppend, %withArgs%, task.xml
	RunWait, cmd.exe /c schtasks.exe /create /XML task.xml /tn %filename%,,hide
	FileDelete, task.xml
	exitapp
return

UpdateGui:
	Gui, submit, NoHide
	hideall()
	switch Day {
		case "Sunday":
			show(7)
		return
		case "Monday":
			show(1)
		return
		case "Tuesday":
			show(2)
		return
		case "Wednesday":
			show(3)
		return
		case "Thursday":
			show(4)
		return
		case "Friday":
			show(5)
		return
		case "Saturday":
			show(6)
		return
	}
return

dayName(number) {
	switch number {
		case 1:
		return "Monday"
		case 2:
		return "Tuesday"
		case 3:
		return "Wednesday"
		case 4:
		return "Thursday"
		case 5:
		return "Friday"
		case 6:
		return "Saturday"
		case 7:
		return "Sunday"
	}
}

addChecks() {
	loop, 7 {
		Gui,Add,Checkbox,x130 y124 w70 h13 v%A_Index%E gUpdateGui,Enabled
	}
}
addTimes() {
	loop, 7 {
		Gui,Add,DateTime,x200 y120 w100 h21 v%A_Index%T,Time
	}
}

show(day) {
	GuiControl, Show, %day%E
	if (%day%E)
		GuiControl, Show, %day%T
}

hideall() {
	loop, 7 {
		GuiControl, Hide, %A_Index%E
		GuiControl, Hide, %A_Index%T
	}
}

GuiClose:
ExitApp