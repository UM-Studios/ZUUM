"""
This example demonstrates the use of the SQLAlchemy job store.
On each run, it adds a new alarm that fires after ten seconds.
You can exit the program, restart it and observe that any previous alarms that have not fired yet
are still active. You can also give it the database URL as an argument.
See the SQLAlchemy documentation on how to construct those.
"""
from appdata import appdata

import sys    
from datetime import datetime, time, MINYEAR
import os
import warnings
import re
from collections import OrderedDict
from pytz import UTC

from apscheduler.schedulers.background import BackgroundScheduler
from apscheduler.triggers.cron import CronTrigger
from apscheduler.triggers.combining import OrTrigger
from apscheduler.util import undefined

import rpyc
from rpyc.utils.server import ThreadedServer

if not os.path.exists(appdata):
    os.makedirs(appdata)

argsTemplate = dict.fromkeys(('confno', 'pwd', 'zc', 'browser', 'uname', 'stype', 'uid', 'sid', 'tk'), '')
weekdays = {0: "Monday", 1: "Tuesday", 2: "Wednesday", 3: "Thursday", 4: "Friday", 5: "Saturday", 6: "Sunday"}
weeknums = {"Monday": 0, "Tuesday": 1, "Wednesday": 2, "Thursday": 3, "Friday": 4, "Saturday": 5, "Sunday": 6}

def joinMeeting(args):
    os.startfile(f'zoommtg://zoom.us/join?{"&".join([arg+"="+args[arg] for arg in args if args[arg]])}')
    #os.startfile(Task.get_protocol_link(args))

class Trigger(CronTrigger):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.day = int(str(self.fields[4]))
        self.time = time(**{self.fields[i].name: int(str(self.fields[i])) for i in range(5,8)})
    @classmethod
    def from_cron_trigger(cls, crontrigger):
        return cls(**{field.name: (None if str(field) == '*' else str(field)) for field in crontrigger.fields})
    def formatted_day_time(self):
        return f'{weekdays[self.day]} at {self.time.strftime("%I:%M %p").lstrip("0")}'
    def formatted_time(self):
        return self.time.strftime('%H:%M')
    def formatted_day(self):
        return weekdays[self.day]

class Task:
    def __init__(self, name, enabled, args = argsTemplate, triggers = [], id = None, priority = 0):
        self.name = name
        self.args = args
        self.triggers = triggers
        self.trigger = OrTrigger(self.triggers)
        self.enabled = enabled if self.triggers else False
        self.id = id
        self.priority = priority
    @classmethod
    def task_from_job(cls, job):
        #return cls(job.args[0].name, job.args[0].enabled, id = job.id, args = job.args[0].args, triggers = job.args[0].triggers)#[Trigger.from_cron_trigger(ct) for ct in job.args[0].trigger.triggers])
        return cls(job.name, bool(job.next_run_time), id = job.id, args = job.args[0].args, priority = job.args[0].priority, triggers = [Trigger.from_cron_trigger(ct) for ct in job.trigger.triggers])

    @classmethod
    def task_from_browser(cls, name, enabled, link, triggers = []):
        return cls(name, enabled, Task.args_from_browser(link), triggers)
    @classmethod
    def task_from_protocol(cls, name, enabled, link, triggers = []):
        return cls(name, enabled, Task.args_from_protocol(link), triggers)
    @staticmethod
    def args_from_browser(link):
        if Task.browser_validate(link):
            none = re.compile(r'(^(?![\s\S]))').match('')
            args = argsTemplate
            args["confno"] = str((re.compile(r'zoom\.us\/[jw]\/(\d+)').search(link) or none).groups()[0] or '')
            args["pwd"] = str((re.compile(r'pwd=([a-zA-Z0-9]+)').search(link) or none).groups()[0] or '')
            args["zc"] = str((re.compile(r'zc=([01])').search(link) or none).groups()[0] or '')
            args["browser"] = str((re.compile(r'browser=(chrome|firefox|msie|safari)').search(link) or none).groups()[0] or '')
            args["uname"] = str((re.compile(r'uname=([a-zA-Z0-9]+)').search(link) or none).groups()[0] or '')
            args["stype"] = str((re.compile(r'stype=(100|0|1|101|99)').search(link) or none).groups()[0] or '')
            args["uid"] = str((re.compile(r'uid=([a-zA-Z0-9_.-]+)').search(link) or none).groups()[0] or '')
            args["sid"] = str((re.compile(r'sid=([a-zA-Z0-9_.-]+)').search(link) or none).groups()[0] or '')
            args["tk"] = str((re.compile(r'(?:tk|token)=([a-zA-Z0-9_.-]+)').search(link) or none).groups()[0] or '')
        else:
            raise AttributeError
        return args
    @staticmethod
    def args_from_protocol(link):
        if Task.protocol_validate(link):
            none = re.compile(r'(^(?![\s\S]))').match('')
            args = argsTemplate
            args["confno"] = str((re.compile(r'confno=(\d+)').search(link) or none).groups()[0] or '')
            args["pwd"] = str((re.compile(r'pwd=([a-zA-Z0-9]+)').search(link) or none).groups()[0] or '')
            args["zc"] = str((re.compile(r'zc=([01])').search(link) or none).groups()[0] or '')
            args["browser"] = str((re.compile(r'browser=(chrome|firefox|msie|safari)').search(link) or none).groups()[0] or '')
            args["uname"] = str((re.compile(r'uname=([a-zA-Z0-9]+)').search(link) or none).groups()[0] or '')
            args["stype"] = str((re.compile(r'stype=(100|0|1|101|99)').search(link) or none).groups()[0] or '')
            args["uid"] = str((re.compile(r'uid=([a-zA-Z0-9_.-]+)').search(link) or none).groups()[0] or '')
            args["sid"] = str((re.compile(r'sid=([a-zA-Z0-9_.-]+)').search(link) or none).groups()[0] or '')
            args["tk"] = str((re.compile(r'(?:tk|token)=([a-zA-Z0-9_.-]+)').search(link) or none).groups()[0] or '')
        else:
            raise AttributeError
        return args
    @staticmethod
    def browser_validate(link):
        return bool(re.compile(r'^((https?:\/\/)?(us02web\.)?zoom\.us\/[jw]\/)(\d+)\??((&?(pwd=[a-zA-Z0-9]+))?|(&?((tk|token)=[a-zA-Z0-9_.-]+))?|(&?(browser=(chrome|firefox|msie|safari)))?|(&?(zc=[01]))?|(&?(uname=[a-zA-Z0-9]+))?|(&?(stype=(100|0|1|101|99)))?|(&?(uid=[a-zA-Z0-9_.-]+))?|(&?(sid=[a-zA-Z0-9_.-]+))?){0,8}(?:#.*)?$').match(link))
    @staticmethod
    def protocol_validate(link):
        return bool(re.compile(r'^zoommtg:\/\/zoom\.us\/(start|join)\??((&?action=[a-zA-Z]+)?|(&?confno=\d+)?|(&?(pwd=[a-zA-Z0-9]+))?|(&?((tk|token)=[a-zA-Z0-9_.-]+))?|(&?(browser=(chrome|firefox|msie|safari)))?|(&?(zc=[01]))?|(&?(uname=[a-zA-Z0-9]+))?|(&?(stype=(100|0|1|101|99)))?|(&?(uid=[a-zA-Z0-9_.-]+))?|(&?(sid=[a-zA-Z0-9_.-]+))?){0,10}$').match(link))
    @staticmethod
    def name_validate(name):
        return True
    def browser_link(self):
        return Task.get_browser_link(self.args)
        #return f'https://zoom.us/j/{self.args["confno"]}&{"&".join([arg+"="+self.args[arg] for arg in self.args if self.args[arg] and arg != "confno"])}'
    @staticmethod
    def get_protocol_link(args):
        return f'zoommtg://zoom.us/join?{"&".join([arg+"="+args[arg] for arg in args if args[arg]])}'
    @staticmethod
    def get_browser_link(args):
        return f'https://zoom.us/j/{args["confno"]}&{"&".join([arg+"="+args[arg] for arg in args if args[arg] and arg != "confno"])}'
    def configure(self, scheduler, func=joinMeeting, jobstore = 'default', **changes):
        if self.id:
            changes = {change: changes[change] for change in changes if changes[change] is not None}
            if 'triggers' in changes:
                changes['trigger'] = OrTrigger(changes['triggers'])
                self.triggers = changes['triggers']
                self.trigger = changes['trigger']
                changes.pop('triggers')
            if 'enabled' in changes:
                self.enable(scheduler) if changes['enabled'] else self.disable(scheduler)
                changes.pop('enabled')
            if 'link' in changes:
                self.args = Task.args_from_browser(changes['link'])
                changes['args'] = [self]
                changes.pop('link')
            job = scheduler.modify_job(self.id, jobstore, **changes)
            self.__dict__.update(Task.task_from_job(job).__dict__)
        else:
            job = scheduler.add_job(func, trigger=self.trigger, args=[self], next_run_time = None, name=self.name, jobstore='default', executor='default')
            if self.enabled:
                scheduler.resume_job(job.id, jobstore)
            self.__dict__.update(Task.task_from_job(job).__dict__)
    def delete(self, scheduler, jobstore = 'default'):
        if self.id:
            scheduler.remove_job(self.id, jobstore)
        else:
            warnings.warn("Can't delete non-created task")
    def enable(self, scheduler, jobstore = 'default'):
        if self.triggers:
            self.__dict__.update(Task.task_from_job(scheduler.resume_job(self.id, jobstore)).__dict__)
        else:
            warnings.warn("Can't enable task with no triggers")
    def disable(self, scheduler, jobstore = 'default'):
        self.__dict__.update(Task.task_from_job(scheduler.pause_job(self.id, jobstore)).__dict__)
    def next_fire(self):
        return self.trigger.get_next_fire_time(None, datetime.now()) or None
    def formatted_next_run(self):
        next_fire = self.next_fire()
        return next_fire.strftime('%A at %I:%M %p').replace(" 0", " ") if next_fire else "None"
    @staticmethod
    def get_task_list(scheduler, jobstore = 'default'):
        list = {job.id: Task.task_from_job(job) for job in scheduler.get_jobs(jobstore = jobstore)}
        s = OrderedDict(sorted(list.items(), key=lambda t: (t[1].priority, t[1].next_fire() or datetime.max.replace(tzinfo=UTC), t[1].name)))
        return s

if __name__ == '__main__':
    if (len(sys.argv)==5):
        print('creating task')
        link = 'zoommtg://zoom.us/start?browser=chrome&confno=123456789&zc=0&stype=100&sid=lBGPGzGdT8-2Yf3kjDY5gg&uid=lBGPGzGdT8-2Yf3kjDY5gg&token=xxxxxx&uname=Betty'
        days = [Trigger(day_of_week=sys.argv[1], hour=sys.argv[2], minute=sys.argv[3])]
        task = Task.task_from_protocol(sys.argv[4], True, link, days)
    #for arg in task.args:
        #print(f'{arg}: {task.args[arg]}, {bool(task.args[arg])}')
    #print([f'zoommtg://zoom.us/join?{"&".join([arg+"="+task.args[arg] for arg in task.args if task.args[arg]])}'])
    #none = re.compile(r'(^(?![\s\S]))').match('')
    #print(str((re.compile(r'zoom\.us\/[jw]\/(\d+)').search(link) or none).groups()[0] or ''))
    scheduler = BackgroundScheduler()
    url = f'sqlite:///{os.path.join(appdata, "meeting_data.db")}'
    scheduler.add_jobstore('sqlalchemy', url=url)
    scheduler.start()
    if (len(sys.argv)==5):
        task.configure(scheduler)
    
    #scheduler.pause_job(job_id='705d441c3b35490290eb85f72a74dff4', jobstore='default')
    #scheduler.print_jobs()
    for task in Task.get_task_list(scheduler).values():
        print(task.triggers[0].formatted_next_run())
        #print(task.browser_link())
    #for job in jobs:
        #print(bool(job.next_run_time))
        #print(Task.task_from_job(job).name, Task.task_from_job(job).enabled)
    #print(fields)
    scheduler.shutdown()
    