import subprocess
import datetime
import re
import os
import xml.etree.ElementTree as ET

zoompath = r"%APPDATA%\Zoom\bin\Zoom.exe"

def cmd_command(cmd):
    return subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,  close_fds=True).communicate()

class Trigger:
    def __init__(self, day, time=datetime.datetime.now()):
        self.day = day
        self.time = time

class Task:
    def __init__(self, name, link, triggers = []):
        self.linkre = re.compile(r"^(?:(?:https?:\/\/)?(?:us02web\.)?zoom\.us\/[jw]\/)(\d+)\??(tk=[a-zA-Z0-9_.-]+)?&?(pwd=[a-zA-Z0-9]+)?$")
        self.argsre = re.compile(r'"(?:--url=)?zoommtg:\/\/zoom.us\/join\?action=join(?:&confno=)?(\d+)&?(tk=[a-zA-Z0-9_.-]+)?&?(pwd=[a-zA-Z0-9]+)?"')
        self.weekdays = {"Monday": 0, "Tuesday": 1, "Wednesday": 2, "Thursday": 3, "Friday": 4, "Saturday": 5, "Sunday": 6, }
        self.name = name
        if re.match(self.linkre, link):
            m = re.match(self.linkre, link)
        elif re.match(self.argsre, link):
            m = re.match(self.argsre, link)
        else:
            m = re.compile(r"()()()").match('')
        self.args = [str(i or '') for i in m.groups()]
        self.triggers = triggers
        self.userid = re.compile(r"S-1-5-21-(\d+-?)+").search(cmd_command("whoami /user /fo csv /nh")[0].decode("utf-8")).group(0)
    def add_trigger(self, trigger):
        self.triggers.append(trigger)
    def get_args(self):
        return r'"--url=zoommtg://zoom.us/join?action=join&confno='+self.args[0]+r'&'+self.args[1]+r'&'+self.args[2]+r'"'
    def get_link(self):
        return 'https://zoom.us/w/'+str(self.args[0])+'?'+str(self.args[1])+'&'+str(self.args[2])
    def set_link(self, link):
        if re.match(self.linkre, link):
            m = re.match(self.linkre, link)
        else: 
            raise ValueError("Invalid Link")
        self.args = [str(i or '') for i in m.groups()]
    def set_args(self, args):
        if re.match(self.argsre, args):
            m = re.match(self.argsre, args)
        else: 
            raise ValueError("Invalid Argument")
        self.args = [str(i or '') for i in m.groups()]
    def replace_trigger(self, index, new):
        self.triggers[index] = new
    def rename(self, name):
        self.name = name
    def get_task_name(self):
        return self.name.split('\\')[-1]
    def get_next_trigger(self):
        ahead = []
        for i in range(len(self.triggers)):
            min = 0
            nextday = datetime.datetime.now() + datetime.timedelta((self.weekdays[self.triggers[i].day] - datetime.datetime.now().weekday())%7)
            nextdatetime = datetime.datetime.combine(nextday, self.triggers[i].time.time())
            ahead.append((nextdatetime - datetime.datetime.now()) % datetime.timedelta(days=7))
            if ahead[i] < ahead[min]:
                min = i
        return self.triggers[min] if self.triggers else ''

def name_validate(name):
    return bool(re.compile(r'^(?!(?:COM[0-9]|CON|LPT[0-9]|NUL|PRN|AUX|com[0-9]|con|lpt[0-9]|nul|prn|aux)|\s|[\.]{2,})[^\\\/:*"?<>|]{1,254}(?<![\s\.])$').match(name))

def link_validate(link):
    return bool(re.compile(r"^(?:(?:https?:\/\/)?(?:us02web\.)?zoom\.us\/[jw]\/)(\d+)\??(tk=[a-zA-Z0-9_.-]+)?&?(pwd=[a-zA-Z0-9]+)?$").match(link))


def build_XML(tree, task, path, author="CREATEZOOMTASK",):
    ns = {'ns0': 'http://schemas.microsoft.com/windows/2004/02/mit/task'}
    root = tree.getroot().find("./ns0:RegistrationInfo", ns)
    root.find("./ns0:Date", ns).text = datetime.datetime.now().isoformat(timespec='seconds')
    root.find("./ns0:Author", ns).text = author
    root.find("./ns0:URI", ns).text = task.name

    root = tree.getroot().find("./ns0:Triggers", ns)
    for trigger in task.triggers or []:
        subroot = create_XML_tree('trigger').getroot()
        subroot.find("./ns0:StartBoundary", ns).text = trigger.time.isoformat(timespec='seconds')
        ET.SubElement(subroot.find("./ns0:ScheduleByWeek/ns0:DaysOfWeek", ns), '{http://schemas.microsoft.com/windows/2004/02/mit/task}'+trigger.day)
        root.append(subroot)

    tree.getroot().find("./ns0:Principals/ns0:Principal/ns0:UserId", ns).text = task.userid

    root = tree.getroot().find("./ns0:Actions/ns0:Exec", ns)
    root.find("./ns0:Command", ns).text = path
    root.find("./ns0:Arguments", ns).text = task.get_args()
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
    ns = {'ns0': 'http://schemas.microsoft.com/windows/2004/02/mit/task'}
    tlist = []
    query = cmd_command(f"schtasks /query /tn \\ZoomJoin\\ /xml")[0]
    #print(query)
    if not query.decode("utf-8").startswith('ERROR'):
        root = ET.fromstring(('<?xml version="1.0" encoding="UTF-16"?>'+query.decode("utf-8").replace('<?xml version="1.0" encoding="UTF-16"?>', '')).encode('utf-16-be'))
        for i in range(len(root)):
            triggers = []
            for j in range(len(root[i][3])):
                triggers.append(Trigger(root[i].find("./ns0:Triggers", ns)[j].find("./ns0:ScheduleByWeek/ns0:DaysOfWeek", ns)[0].tag.split('}')[1], datetime.datetime.fromisoformat(root[i].find("./ns0:Triggers", ns)[j].find("./ns0:StartBoundary", ns).text)))
            tlist.append(Task(root[i].find("./ns0:RegistrationInfo/ns0:URI", ns).text, root[i].find("./ns0:Actions/ns0:Exec/ns0:Arguments", ns).text, triggers))
    return tlist

#def get_task_XML():
#    root = ET.fromstring(('<?xml version="1.0" encoding="UTF-16"?>'+cmd_command(f"schtasks /query /tn \\ZoomJoin\\ /xml")[0].decode("utf-8").replace('<?xml version="1.0" encoding="UTF-16"?>', '')).encode('utf-16-be'))
#    return ET.ElementTree(root)

def run_task(task):
    cmd_command(f'schtasks /run /tn "{task.name}"')

def delete_task(task):
    cmd_command(f'schtasks /delete /tn "{task.name}" /F')

#______________testing______________
if __name__ == "__main__":
    ns = {'ns0': 'http://schemas.microsoft.com/windows/2004/02/mit/task'}
    days = [Trigger("Sunday"), Trigger("Wednesday", datetime.datetime(year=2020, month=9, day=2, hour=21))]
    hello = Task('hello where', 'https://us02web.zoom.us/w/88392313240?tk=E5YZhz_cGRVZvNoUcBRrrxatyH6E5xI66QVzcMRC7O4.DQIAAAAUlJepmBZ0RW9LMFlWVlNxU0tmYlRMOVRZV1BRAAAAAAAAAAAAAAAAAAAAAAAAAAAA&pwd=Z1R5cXQ4K0M2UHhFOEFrbm5xazBHUT09', days)

    #print(hello.get_next_trigger().day)
    print(get_task_list())

    #write_XML(build_XML(create_XML_tree('task'), hello, zoompath), filename='testy')

    #print(get_task_list()[2].triggers[0].day)
    #argsre = re.compile(r'"--url=zoommtg:\/\/zoom.us\/join\?action=join(?:&confno=)?(\d+)&?(tk=[a-zA-Z0-9_.-]+)?&?(pwd=[a-zA-Z0-9]+)?"')
    #linkre = re.compile(r"^(?:(?:https?:\/\/)?(?:us02web\.)?zoom\.us\/[jw]\/)(\d+)\??(tk=[a-zA-Z0-9_.-]+)?&?(pwd=[a-zA-Z0-9]+)?$")

    #print(get_task_list().getroot()[0][4][0][1].text)
    #print(get_task_list().getroot()[0][4][0][1].text)
    list = get_task_list()
    
    #run_task(list[0])

    #print(list[0].get_task_name())
    #get_task_XML().write("test.xml", encoding='UTF-16')
    #viewer = get_task_XML().getroot()[1].find("./ns0:Triggers", ns)[0].find("./ns0:ScheduleByWeek/ns0:DaysOfWeek", ns)[0].tag.split('}')[1]
    #print(viewer)