"""
This is an example RPC client that connects to the RPyC based scheduler service.
It first connects to the RPyC server on localhost:12345.
Then it schedules a job to run on 2 second intervals and sleeps for 10 seconds.
After that, it unschedules the job and exits.
"""

from APSched import Task, CTrigger, DTrigger, TaskList, weeknums, tree_print, NeverTrigger
import sys

import rpyc

import os
from apscheduler.schedulers.background import BackgroundScheduler
from appdata import appdata
from apscheduler.triggers.combining import OrTrigger
from apscheduler.triggers.base import BaseTrigger
from datetime import datetime, timedelta

class NoneTrigger(BaseTrigger):
    def get_next_fire_time(self, previous_fire_time, now):
        return None

conn = rpyc.connect('localhost', 12345, config={"allow_all_attrs": True, "allow_pickle": True})
scheduler = conn.root
mylink = 'https://zoom.us/j/123456789&zc=0&browser=chrome&uname=Betty&stype=100&uid=lBGPGzGdT8-2Yf3kjDY5gg&sid=lBGPGzGdT8-2Yf3kjDY5gg&tk=xxxxxx'
mytask = Task.task_from_browser('bdss', True, mylink, [NeverTrigger()])
# tree_print(mytask.__dict__)

# mytask.configure(scheduler)

for task in Task.get_task_list(scheduler).items():
    task[1].delete(scheduler)


Task.get_task_list(scheduler).print_tasks()