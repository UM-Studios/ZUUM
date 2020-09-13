from createTask import Task
import datetime
import re

'''
1. big task class that contains name, link, triggers
2. task name valid
3. get all tasks that have been created
4. run task given name
5. delete task given name
6. create task given a Task
7. get next task
8. edit task
'''

day_to_num = {
  'Monday': 1,
  'Tuesday': 2,
  'Wednesday': 3,
  'Thursday': 4,
  'Friday': 5,
  'Saturday': 6,
  'Sunday': 7
}

num_to_day = {
  1: 'Monday',
  2: 'Tuesday',
  3: 'Wednesday',
  4: 'Thursday',
  5: 'Friday',
  6: 'Saturday',
  7: 'Sunday'
}

class Trigger:
  def __init__(self, day, time=datetime.datetime.now()):
    self.day = day
    self.time = time

class FullTask():

  def __init__(self, name, link, triggers):
    self.name = name 
    self.link = link
    self.triggers = triggers

  def get_next_trigger(self):
    ahead = []
    min = 0
    for i in range(len(self.triggers)):
      nextday = datetime.datetime.now() + datetime.timedelta((day_to_num[self.triggers[i].day] - 1 - datetime.datetime.now().weekday())%7)
      nextdatetime = datetime.datetime.combine(nextday, self.triggers[i].time.time())
      ahead.append((nextdatetime - datetime.datetime.now()) % datetime.timedelta(days=7))
      if ahead[i] < ahead[min]:
        min = i
    return self.triggers[min] if self.triggers else ''

  #creates tasks
  def create_tasks(self):
    Task(self.name, 1, 1, 1, self.link).create_file()
    for trigger in self.triggers:
      t = Task(self.name, trigger.time.minute, trigger.time.hour, day_to_num[trigger.day], self.link)
      t.schedule_cron()
      t.add_task()
    if len(self.triggers) == 0:
      t = Task(self.name, None, None, None, self.link)
      t.add_task()

  #deletes task and all its triggers
  @classmethod
  def delete_full_task(cls, name):
    all_tasks = Task.get_all_tasks()
    for task in all_tasks:
      if task['name'] == name:
        Task(task['name'], task['minute'], task['hour'], task['day_of_week'], task['link']).delete_task()

  #runs task with name
  @classmethod
  def run_task(cls, name):
    Task.run_task(name)

  #returns list of full tasks
  @classmethod
  def get_all_full_tasks(cls):
    all_tasks = Task.get_all_tasks()
    full_tasks_raw = {}

    for task in all_tasks:
      tup = (task['name'], task['link'])
      if tup not in full_tasks_raw:
        full_tasks_raw[tup] = []
      if task['hour'] != None:
        full_tasks_raw[tup].append((datetime.datetime(2020, 1, 1, hour = int(task['hour']), minute = int(task['minute'])), num_to_day[int(task['day_of_week'])]))
        
    full_tasks = []
    for full_task in full_tasks_raw:
      cur_task = FullTask(full_task[0], full_task[1], [])
      for trigger in full_tasks_raw[full_task]:
        cur_task.triggers.append(Trigger(trigger[1], trigger[0]))
      full_tasks.append(cur_task)
    
    return full_tasks

  #if returns true then its valid
  @classmethod
  def validate_filename(cls, filename):
    return re.compile(r'^\..*|.*:.*').search(filename) == None and not len(filename) == 0 and not len(filename) > 255

  @classmethod
  def link_validate(cls, link):
    return bool(re.compile(r"^(?:(?:https?:\/\/)?(?:us02web\.)?zoom\.us\/[jw]\/)(\d+)\??(tk=[a-zA-Z0-9_.-]+)?&?(pwd=[a-zA-Z0-9]+)?(?:#.*)?$").match(link))