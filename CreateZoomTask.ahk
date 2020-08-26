#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

;Zoom.exe path:
zoompath := "%APPDATA%\Zoom\bin\Zoom.exe"

clipsave := ClipboardALL
RunWait, cmd.exe /c whoami /user /fo csv /nh | clip,,hide
userid := RegExReplace(clipboard, """" . ".+" . """" . "," . """" . "(S-[-\d]+)" . """", "$1")
Clipboard := clipsave

template := "<?xml version=" . """" . "1.0" . """" . " encoding=" . """" . "UTF-16" . """" . "?>`n<Task version=" . """" . "1.4" . """" . " xmlns=" . """" . "http://schemas.microsoft.com/windows/2004/02/mit/task" . """" . ">`n  <RegistrationInfo>`n    <Date>[NOW]</Date>`n    <Author>CREATEZOOMTASK</Author>`n    <URI>\[MEETINGNAME]</URI>`n  </RegistrationInfo>`n  <Triggers>    [TRIGGERS]`n  </Triggers>`n  <Principals>`n    <Principal id=" . """" . "Author" . """" . ">`n      <UserId>" . userid . "</UserId>`n      <LogonType>InteractiveToken</LogonType>`n      <RunLevel>LeastPrivilege</RunLevel>`n    </Principal>`n  </Principals>`n  <Settings>`n    <MultipleInstancesPolicy>StopExisting</MultipleInstancesPolicy>`n    <DisallowStartIfOnBatteries>false</DisallowStartIfOnBatteries>`n    <StopIfGoingOnBatteries>false</StopIfGoingOnBatteries>`n    <AllowHardTerminate>true</AllowHardTerminate>`n    <StartWhenAvailable>false</StartWhenAvailable>`n    <RunOnlyIfNetworkAvailable>false</RunOnlyIfNetworkAvailable>`n    <IdleSettings>`n      <StopOnIdleEnd>true</StopOnIdleEnd>`n      <RestartOnIdle>false</RestartOnIdle>`n    </IdleSettings>`n    <AllowStartOnDemand>true</AllowStartOnDemand>`n    <Enabled>true</Enabled>`n    <Hidden>false</Hidden>`n    <RunOnlyIfIdle>false</RunOnlyIfIdle>`n    <DisallowStartOnRemoteAppSession>false</DisallowStartOnRemoteAppSession>`n    <UseUnifiedSchedulingEngine>true</UseUnifiedSchedulingEngine>`n    <WakeToRun>false</WakeToRun>`n    <ExecutionTimeLimit>PT1H</ExecutionTimeLimit>`n    <Priority>7</Priority>`n  </Settings>`n  <Actions Context=" . """" . "Author" . """" . ">`n    <Exec>`n      <Command>" . zoompath . "</Command>`n      <Arguments>[ARGS]</Arguments>`n    </Exec>`n  </Actions>`n</Task>"

TriggerTemplate := "`n<CalendarTrigger>`n    <StartBoundary>[TIME]</StartBoundary>`n    <Enabled>true</Enabled>`n    <ScheduleByWeek>`n        <DaysOfWeek>`n            <[DAY] />`n        </DaysOfWeek>`n        <WeeksInterval>1</WeeksInterval>`n    </ScheduleByWeek>`n</CalendarTrigger>"

Loop, 7 {
	%A_Index%E :=
	%A_Index%T :=
}

Gui,add,Text,x2 y0 w120 h13 vNameText, Meeting Name:
Gui,add,Edit,x2 y15 h21 vMeetingName gUpdateGui ,
Gui,Add,Text,x2 y50 w120 h13 vPasteText,Paste Zoom link:
Gui,Add,Edit,x2 y65 w400 h21 vLink gUpdateGui,
Gui,Add,Text,x2 y100 w100 h13 vSelectDay,Select Day:
Gui,Add,ListBox,x2 y115 w120 h100 vDay gUpdateGui, Sunday||Monday|Tuesday|Wednesday|Thursday|Friday|
addChecks()
addTimes()
Gui,Add,Button, x1 y220 w80 gSubmit vGo, OK
Gui,Add,Text,x85 y223 w500 vErrorText,
GoSub UpdateGui
Gui,Show,,Task Information
return

UpdateGui:
	Gui, submit, NoHide
	hideall()
	sum := 0
	Loop, 7 {
		sum += %A_Index%E
	}
	if (MeetingName = "") {
		GuiControl, Text, ErrorText, Missing meeting name
		GuiControl, Disable, Go
	} else if (!RegExMatch(MeetingName, "^[\S]+$")) {
		GuiControl, Text, ErrorText, No spaces allowed in meeting name
		GuiControl, Disable, Go
	} else if (Link = "") {
		GuiControl, Text, ErrorText, Missing Zoom link
		GuiControl, Disable, Go
	} else if (!RegExMatch(Link, "^(?:(?:https?:\/\/)?(?:us02web\.)?zoom\.us\/j\/)(\d+)\??(pwd=[a-zA-Z0-9]+)?$")) {
		GuiControl, Text, ErrorText, Invalid Zoom link
		GuiControl, Disable, Go
	} else if (sum = 0) {
		GuiControl, Text, ErrorText, No time scheduled
		GuiControl, Disable, Go
	} else {
		GuiControl, Text, ErrorText, You should be good to go!
		GuiControl, Enable, Go
	}
	GuiControl, Show, ErrorText
	show(dayNumber(Day))
	selected := dayNumber(Day)
	choice := "|"
	Loop, 7 {
		selTime := 
		dayNumber := (mod(A_Index+5, 7)+1)
		choice .= dayName(daynumber)
		if (%dayNumber%E)
			selTime := RegExReplace(%dayNumber%T, "(\d{4})(\d{2})(\d{2})(\d{2})(\d{2})(\d{2})", " $4:$5:$6")
			choice .= selTime
		choice .= "|"
		if (selected = mod(A_Index+5, 7)+1)
			choice .= "|"
	}
	;msgbox % choice
	GuiControl,,Day,%choice%
	;GuiControl, Focus, NameText
return

TUpdateGui:
	GoSub UpdateGui
	focus := dayNumber(Day) . "E"
	GuiControl, Focus, %focus%
return

Submit:
	Gui, submit, nohide
	argument := RegExReplace(Link, "(?:(?:https?:\/\/)?(?:us02web\.)?zoom\.us\/j\/)(\d+)\??(pwd=[a-zA-Z0-9]+)?", """" . "--url=zoommtg://zoom.us/join?action=join&amp;confno=$1&amp;$2" . """")
	MeetingName := StrReplace(MeetingName, " ")
	FormatTime, now, A_Now, yyyy-MM-dd'T'HH:mm:ss
	withNow := StrReplace(template, "[NOW]", now)
	withName := StrReplace(withNow, "[MEETINGNAME]", MeetingName)
	loop, 7 {
		if (%A_Index%E) {
			;seltime := %A_Index%T
			;FormatTime, selectedtime, seltime, yyyy-MM-dd'T'HH:mm:ss
			selectedtime := RegExReplace(%A_Index%T, "(\d{4})(\d{2})(\d{2})(\d{2})(\d{2})(\d{2})", "$1-$2-$3T$4:$5:$6")
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

dayNumber(name) {
	name := RegExReplace(name, "([A-Z][a-zA-Z]+day)( (\d{2}:?){3})?", "$1")
	switch name {
		case "Sunday":
		return 7
		case "Monday":
		return 1
		case "Tuesday":
		return 2
		case "Wednesday":
		return 3
		case "Thursday":
		return 4
		case "Friday":
		return 5
		case "Saturday":
		return 6
	}
}

addChecks() {
	loop, 7 {
		height := 106+12.9*(mod(A_Index, 7)+1)
		Gui,Add,Checkbox,x130 y%height% w70 h13 v%A_Index%E gUpdateGui,Enabled
	}
}
addTimes() {
	loop, 7 {
		height := 102+12.9*(mod(A_Index, 7)+1)
		Gui,Add,DateTime,x200 y%height% w100 h21 v%A_Index%T gTUpdateGui,Time
	}
}

show(day) {
	GuiControl, Show, %day%E
	if (%day%E)
		GuiControl, Show, %day%T
}

hideall() {
	GuiControl, Hide, ErrorText
	loop, 7 {
		GuiControl, Hide, %A_Index%E
		GuiControl, Hide, %A_Index%T
	}
}

GuiClose:
ExitApp