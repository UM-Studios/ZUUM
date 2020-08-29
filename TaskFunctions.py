import subprocess
import datetime
import re
import os
import xml.etree.ElementTree as ET

zoompath = r"%APPDATA%\Zoom\bin\Zoom.exe"

def cmd_command(cmd):
    return subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT).communicate()

class Trigger:
    def __init__(self, day, time=datetime.datetime.now()):
        self.day = day
        self.time = time

class Task:
    
    def __init__(self, name, link, triggers = []):
        self.linkre = re.compile(r"^(?:(?:https?:\/\/)?(?:us02web\.)?zoom\.us\/[jw]\/)(\d+)\??(tk=[a-zA-Z0-9_.-]+)?&?(pwd=[a-zA-Z0-9]+)?$")
        self.argsre = re.compile(r'"--url=zoommtg:\/\/zoom.us\/join\?action=join(?:&confno=)?(\d+)&?(tk=[a-zA-Z0-9_.-]+)?&?(pwd=[a-zA-Z0-9]+)?"')
        self.name = name
        if re.match(self.linkre, link):
            m = re.match(self.linkre, link)
        elif re.match(self.argsre, link):
            m = re.match(self.argsre, link)
        self.args = [str(i or '') for i in m.groups()]
        self.triggers = triggers
        self.userid = re.compile(r"S-1-5-21-(\d+-?)+").search(cmd_command("whoami /user /fo csv /nh")[0].decode("utf-8")).group(0)
    def add_trigger(self, trigger):
        self.triggers.append(trigger)
    def get_args(self):
        return r'"--url=zoommtg://zoom.us/join?action=join&confno='+self.args[0]+r'&'+self.args[1]+r'&'+self.args[2]+r'"'
    def get_link(self):
        return 'https://zoom.us/w/'+str(self.args[0])+'?'+str(self.args[1])+'&'+str(self.args[2])

def build_XML(tree, task, path, author="CREATEZOOMTASK",):
    root = tree.getroot()[0]
    root[0].text = datetime.datetime.now().isoformat(timespec='seconds')
    root[1].text = author
    root[2].text = task.name

    root = tree.getroot()[1]
    for trigger in task.triggers or []:
        subroot = create_XML_tree('trigger').getroot()
        subroot[0].text = trigger.time.isoformat(timespec='seconds')
        ET.SubElement(subroot[2][0], '{http://schemas.microsoft.com/windows/2004/02/mit/task}'+trigger.day)
        root.append(subroot)

    tree.getroot()[2][0][0].text = task.userid

    root = tree.getroot()[4][0]
    root[0].text = path
    root[1].text = task.get_args()
    return tree

def create_XML_tree(type):
    if type == 'task':
        return ET.ElementTree(ET.fromstring('<?xml version="1.0" encoding="UTF-16"?>\n<Task version="1.4" xmlns="http://schemas.microsoft.com/windows/2004/02/mit/task">\n  <RegistrationInfo>\n    <Date></Date>\n    <Author></Author>\n    <URI></URI>\n  </RegistrationInfo>\n  <Triggers>\n      </Triggers>\n  <Principals>\n    <Principal id="Author">\n      <UserId></UserId>\n      <LogonType>InteractiveToken</LogonType>\n      <RunLevel>LeastPrivilege</RunLevel>\n    </Principal>\n  </Principals>\n  <Settings>\n    <MultipleInstancesPolicy>StopExisting</MultipleInstancesPolicy>\n    <DisallowStartIfOnBatteries>false</DisallowStartIfOnBatteries>\n    <StopIfGoingOnBatteries>false</StopIfGoingOnBatteries>\n    <AllowHardTerminate>true</AllowHardTerminate>\n    <StartWhenAvailable>false</StartWhenAvailable>\n    <RunOnlyIfNetworkAvailable>false</RunOnlyIfNetworkAvailable>\n    <IdleSettings>\n      <StopOnIdleEnd>true</StopOnIdleEnd>\n      <RestartOnIdle>false</RestartOnIdle>\n    </IdleSettings>\n    <AllowStartOnDemand>true</AllowStartOnDemand>\n    <Enabled>true</Enabled>\n    <Hidden>false</Hidden>\n    <RunOnlyIfIdle>false</RunOnlyIfIdle>\n    <DisallowStartOnRemoteAppSession>false</DisallowStartOnRemoteAppSession>\n    <UseUnifiedSchedulingEngine>true</UseUnifiedSchedulingEngine>\n    <WakeToRun>false</WakeToRun>\n    <ExecutionTimeLimit>PT1H</ExecutionTimeLimit>\n    <Priority>7</Priority>\n  </Settings>\n  <Actions Context="Author">\n    <Exec>\n      <Command></Command>\n      <Arguments></Arguments>\n    </Exec>\n  </Actions>\n</Task>'.encode('utf-16-be')))
    if type == 'trigger':
        return ET.ElementTree(ET.fromstring('<?xml version="1.0" encoding="UTF-16"?>\n<CalendarTrigger xmlns="http://schemas.microsoft.com/windows/2004/02/mit/task">\n    <StartBoundary></StartBoundary>\n    <Enabled>true</Enabled>\n    <ScheduleByWeek>\n        <DaysOfWeek />\n        <WeeksInterval>1</WeeksInterval>\n    </ScheduleByWeek>\n</CalendarTrigger>\n'.encode('utf-16-be')))

def write_XML(tree, install=False, filename='task', taskname='Unnamed'):
    tree.write(filename+".xml", encoding='UTF-16')
    if install:
        i = 0
        ext = ''
        while True:    
            if cmd_command(f"schtasks /create /XML \"{filename}.xml\" /tn \"ZoomJoin\\{taskname}{ext}\"")[0].decode("utf-8").startswith('SUCCESS'):
                break
            i += 1
            ext = ' (' + str(i) + ')'
        os.remove(filename+'.xml')
        return taskname+ext

def get_task_list():
    #tree = ET.parse('test.xml')
    root = ET.fromstring(('<?xml version="1.0" encoding="UTF-16"?>'+cmd_command(f"schtasks /query /tn \\ZoomJoin\\ /xml")[0].decode("utf-8").replace('<?xml version="1.0" encoding="UTF-16"?>', '')).encode('utf-16-be'))
    tlist = []
    for i in range(len(root)):
        triggers = []
        for j in range(len(root[i][3])):
            triggers.append(Trigger(root[i][3][j][1][1][0].tag.split('}')[1], root[i][3][j][0].text))
        tlist.append(Task(root[i][0][2].text, root[i][4][0][1].text, triggers))
    return tlist

def get_task_XML():
    root = ET.fromstring(('<?xml version="1.0" encoding="UTF-16"?>'+cmd_command(f"schtasks /query /tn \\ZoomJoin\\ /xml")[0].decode("utf-8").replace('<?xml version="1.0" encoding="UTF-16"?>', '')).encode('utf-16-be'))
    return ET.ElementTree(root)

#______________testing______________
if __name__ == "__main__":
    days = [Trigger("Sunday"), Trigger("Monday")]
    #hello = Task('hello where', 'https://us02web.zoom.us/w/88392313240?tk=E5YZhz_cGRVZvNoUcBRrrxatyH6E5xI66QVzcMRC7O4.DQIAAAAUlJepmBZ0RW9LMFlWVlNxU0tmYlRMOVRZV1BRAAAAAAAAAAAAAAAAAAAAAAAAAAAA&pwd=Z1R5cXQ4K0M2UHhFOEFrbm5xazBHUT09', days)
    #print(write_XML(test_xml, taskname='hello there'))

    #print(get_task_list()[2].triggers[0].day)
    argsre = re.compile(r'"--url=zoommtg:\/\/zoom.us\/join\?action=join(?:&confno=)?(\d+)&?(tk=[a-zA-Z0-9_.-]+)?&?(pwd=[a-zA-Z0-9]+)?"')
    linkre = re.compile(r"^(?:(?:https?:\/\/)?(?:us02web\.)?zoom\.us\/[jw]\/)(\d+)\??(tk=[a-zA-Z0-9_.-]+)?&?(pwd=[a-zA-Z0-9]+)?$")

    #print(get_task_list().getroot()[0][4][0][1].text)
    #print(get_task_list().getroot()[0][4][0][1].text)
    print(get_task_list()[3])

    viewer = get_task_XML().getroot()[4][4][0][1].text
    #print(viewer)
    #print(re.match(argsre, viewer).group(1))
    #print(re.match(argsre, '"--url=zoommtg://zoom.us/join?action=join&amp;confno=88392313240&amp;tk=E5YZhz_cGRVZvNoUcBRrrxatyH6E5xI66QVzcMRC7O4.DQIAAAAUlJepmBZ0RW9LMFlWVlNxU0tmYlRMOVRZV1BRAAAAAAAAAAAAAAAAAAAAAAAAAAAA&amp;pwd=Z1R5cXQ4K0M2UHhFOEFrbm5xazBHUT09"').group(1))
    #print(re.match(linkre, 'https://us02web.zoom.us/w/88392313240?tk=E5YZhz_cGRVZvNoUcBRrrxatyH6E5xI66QVzcMRC7O4.DQIAAAAUlJepmBZ0RW9LMFlWVlNxU0tmYlRMOVRZV1BRAAAAAAAAAAAAAAAAAAAAAAAAAAAA&pwd=Z1R5cXQ4K0M2UHhFOEFrbm5xazBHUT09').group(1))
    #print(create_XML_tree()[0][0].tag)
    #print(create_XML_tree()[0].text)
    #print(build_XML(create_XML_tree('task'), 'hello there', days, 'userid', 'path', 'args'))
    #tree = build_XML(create_XML_tree('task'), hello, zoompath)
    #write_XML(tree, True)
    #print(cmd_command("whoami /user /fo csv /nh"))
    
    #print(test_xml)
    
    #xlm = cmd_command(f"schtasks /query /tn \\ZoomJoin\\ /xml")[0].decode("utf-8")
    #xlm = ('<?xml version="1.0" encoding="UTF-16"?>'+cmd_command(f"schtasks /query /tn \\ZoomJoin\\ /xml")[0].decode("utf-8").replace('<?xml version="1.0" encoding="UTF-16"?>', ''))
    
    #fi = open("test2.xml", "w")
    #fi.write(xlm)
    #fi.close()