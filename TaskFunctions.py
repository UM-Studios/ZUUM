import subprocess
import datetime
import re
import os

zoompath = r"%APPDATA%\Zoom\bin\Zoom.exe"

def cmd_command(cmd):
    return subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT).communicate()

#setup templates
#userid = re.compile(r"S-1-5-21-(\d+-?)+").search(cmd_command(['whoami', '/user', '/fo', 'csv', '/nh'])[0].decode("utf-8")).group(0)
#template = '<?xml version="1.0" encoding="UTF-16"?>\n<Task version="1.4" xmlns="http://schemas.microsoft.com/windows/2004/02/mit/task">\n  <RegistrationInfo>\n    <Date>[NOW]</Date>\n    <Author>CREATEZOOMTASK</Author>\n    <URI>\ZoomJoin '+name+'</URI>\n  </RegistrationInfo>\n  <Triggers>\n    '+triggers+'  </Triggers>\n  <Principals>\n    <Principal id="Author">\n      <UserId>'+userid+'</UserId>\n      <LogonType>InteractiveToken</LogonType>\n      <RunLevel>LeastPrivilege</RunLevel>\n    </Principal>\n  </Principals>\n  <Settings>\n    <MultipleInstancesPolicy>StopExisting</MultipleInstancesPolicy>\n    <DisallowStartIfOnBatteries>false</DisallowStartIfOnBatteries>\n    <StopIfGoingOnBatteries>false</StopIfGoingOnBatteries>\n    <AllowHardTerminate>true</AllowHardTerminate>\n    <StartWhenAvailable>false</StartWhenAvailable>\n    <RunOnlyIfNetworkAvailable>false</RunOnlyIfNetworkAvailable>\n    <IdleSettings>\n      <StopOnIdleEnd>true</StopOnIdleEnd>\n      <RestartOnIdle>false</RestartOnIdle>\n    </IdleSettings>\n    <AllowStartOnDemand>true</AllowStartOnDemand>\n    <Enabled>true</Enabled>\n    <Hidden>false</Hidden>\n    <RunOnlyIfIdle>false</RunOnlyIfIdle>\n    <DisallowStartOnRemoteAppSession>false</DisallowStartOnRemoteAppSession>\n    <UseUnifiedSchedulingEngine>true</UseUnifiedSchedulingEngine>\n    <WakeToRun>false</WakeToRun>\n    <ExecutionTimeLimit>PT1H</ExecutionTimeLimit>\n    <Priority>7</Priority>\n  </Settings>\n  <Actions Context="Author">\n    <Exec>\n      <Command>' + zoompath + '/Command>\n      <Arguments>'+arg+'</Arguments>\n    </Exec>\n  </Actions>\n</Task>'
#triggertemplate = 


class Trigger:
    def __init__(self, day, time=datetime.datetime.now()):
        self.day = day
        self.time = time

def create_XML(name, link, *sch):
    userid = re.compile(r"S-1-5-21-(\d+-?)+").search(cmd_command(['whoami', '/user', '/fo', 'csv', '/nh'])[0].decode("utf-8")).group(0)
    linkre = re.compile(r"^(?:(?:https?:\/\/)?(?:us02web\.)?zoom\.us\/[jw]\/)(\d+)\??(tk=[a-zA-Z0-9_.-]+)?&?(pwd=[a-zA-Z0-9]+)?$")
    m = re.match(linkre, link)
    arg = linkre.sub(r'"--url=zoommtg://zoom.us/join?action=join&amp;confno='+m.group(1)+r'&amp;'+m.group(2)+r'&amp;'+m.group(3)+r'"',link)
    triggers = ''
    for i in sch:
        triggers += '<CalendarTrigger>\n    <StartBoundary>'+i.time.isoformat(timespec='seconds')+'</StartBoundary>\n    <Enabled>true</Enabled>\n    <ScheduleByWeek>\n        <DaysOfWeek>\n            <'+i.day+' />\n        </DaysOfWeek>\n        <WeeksInterval>1</WeeksInterval>\n    </ScheduleByWeek>\n</CalendarTrigger>\n'
    xml = '<?xml version="1.0" encoding="UTF-16"?>\n<Task version="1.4" xmlns="http://schemas.microsoft.com/windows/2004/02/mit/task">\n  <RegistrationInfo>\n    <Date>'+datetime.datetime.now().isoformat(timespec='seconds')+'</Date>\n    <Author>CREATEZOOMTASK</Author>\n    <URI>\ZoomJoin'+name+'</URI>\n  </RegistrationInfo>\n  <Triggers>\n    '+triggers+'  </Triggers>\n  <Principals>\n    <Principal id="Author">\n      <UserId>'+userid+'</UserId>\n      <LogonType>InteractiveToken</LogonType>\n      <RunLevel>LeastPrivilege</RunLevel>\n    </Principal>\n  </Principals>\n  <Settings>\n    <MultipleInstancesPolicy>StopExisting</MultipleInstancesPolicy>\n    <DisallowStartIfOnBatteries>false</DisallowStartIfOnBatteries>\n    <StopIfGoingOnBatteries>false</StopIfGoingOnBatteries>\n    <AllowHardTerminate>true</AllowHardTerminate>\n    <StartWhenAvailable>false</StartWhenAvailable>\n    <RunOnlyIfNetworkAvailable>false</RunOnlyIfNetworkAvailable>\n    <IdleSettings>\n      <StopOnIdleEnd>true</StopOnIdleEnd>\n      <RestartOnIdle>false</RestartOnIdle>\n    </IdleSettings>\n    <AllowStartOnDemand>true</AllowStartOnDemand>\n    <Enabled>true</Enabled>\n    <Hidden>false</Hidden>\n    <RunOnlyIfIdle>false</RunOnlyIfIdle>\n    <DisallowStartOnRemoteAppSession>false</DisallowStartOnRemoteAppSession>\n    <UseUnifiedSchedulingEngine>true</UseUnifiedSchedulingEngine>\n    <WakeToRun>false</WakeToRun>\n    <ExecutionTimeLimit>PT1H</ExecutionTimeLimit>\n    <Priority>7</Priority>\n  </Settings>\n  <Actions Context="Author">\n    <Exec>\n      <Command>' + zoompath + '</Command>\n      <Arguments>'+arg+'</Arguments>\n    </Exec>\n  </Actions>\n</Task>'
    return xml

def write_XML(xml, install=False, filename='task', taskname='Unnamed'):
    f = open(filename+".xml", "w")
    f.write(xml)
    f.close()
    if install:
        i = 0
        ext = ''
        while True:
            if cmd_command(['schtasks', '/create', '/XML', filename+'.xml', '/tn', 'ZoomJoin'+taskname+ext])[0].decode("utf-8").startswith('SUCCESS'):
                break
            i += 1
            ext = '(' + str(i) + ')'
        os.remove(filename+'.xml')

#______________testing______________
#print(template)
days = Trigger("Sunday", )
test_xml = create_XML('hellothere', 'https://us02web.zoom.us/w/88392313240?tk=E5YZhz_cGRVZvNoUcBRrrxatyH6E5xI66QVzcMRC7O4.DQIAAAAUlJepmBZ0RW9LMFlWVlNxU0tmYlRMOVRZV1BRAAAAAAAAAAAAAAAAAAAAAAAAAAAA&pwd=Z1R5cXQ4K0M2UHhFOEFrbm5xazBHUT09', days)
#print(test_xml)
write_XML(test_xml, True)
