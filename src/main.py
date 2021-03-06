from APSched import Task, CTrigger, TaskList, weeknums, tree_print
from flask import Flask, render_template, request, redirect, url_for, flash
from flaskwebgui import FlaskUI
from datetime import datetime
from time import sleep
import hashlib
import json

import rpyc

import os
import sys
from apscheduler.schedulers.background import BackgroundScheduler
from appdata import appdata

from apscheduler.triggers.combining import OrTrigger

from subprocess import Popen, run, call
# import logging

# logging.basicConfig(level=logging.DEBUG)

#zoompath = r"%APPDATA%\Zoom\bin\Zoom.exe"

# prevent it from breaking when running with no console
try:
    sys.stdout.write("\n")
    sys.stdout.flush()
except (IOError, AttributeError):
    class dummyStream:
        ''' dummyStream behaves like a stream but does nothing. '''
        def __init__(self): pass
        def write(self,data): pass
        def read(self,data): pass
        def flush(self): pass
        def close(self): pass
    # and now redirect all default streams to this dummyStream:
    sys.stdout = dummyStream()
    sys.stderr = dummyStream()
    sys.stdin = dummyStream()
    sys.__stdout__ = dummyStream()
    sys.__stderr__ = dummyStream()
    sys.__stdin__ = dummyStream()

app = Flask(__name__)

app.secret_key = '8af32bf6ff121fecbce4cbb67f5cb43b'
if getattr(sys, "frozen", False):
    # The application is frozen
    app.root_path = os.path.dirname(sys.executable)
    # datadir = os.path.dirname(sys.executable)
else:
    # The application is not frozen
    # Change this bit to match where you store your data files:
    app.root_path = os.path.dirname(__file__)

ui = FlaskUI(app=app)

try:
    conn = rpyc.connect('localhost', 12345, config={"allow_all_attrs": True, "allow_pickle": True})
    scheduler = conn.root
except ConnectionRefusedError:
    scheduler = None
    # print('error')
    # devnull = open(os.devnull, 'wb') # Use this in Python < 3.3
    # # Python >= 3.3 has subprocess.DEVNULL
    # #Popen([sys.executable, 'server.py'], stdout=devnull, stderr=devnull, shell=True)
    # try:
    #     conn = rpyc.connect('localhost', 12345, config={"allow_all_attrs": True, "allow_pickle": True})
    # except ConnectionRefusedError:
    #     exit('could not connect to server')

@app.route("/")
def main():
    if scheduler:
        return redirect(url_for('meetings'))
    else:
        return redirect(url_for('connection_error'))

@app.route("/connection_error")
def connection_error():
    return render_template('ConnectionError.html')

@app.route("/start_scheduler", methods=["GET", "POST"])
def start_scheduler():
    if 'start' in request.form:
        global scheduler
        if sys.platform == "win32":
            try:
                call(["powershell.exe", "./addstartup.ps1"])
                Popen(["ZuumScheduler.exe"])
            except FileNotFoundError:
                pass
        else:
            # hi colin start zuum scheduler
            call(['sh', '-c', 'pw="$(osascript -e \'Tell application "System Events" to display dialog "Password:" default answer "" with hidden answer\' -e \'text returned of result\' 2>/dev/null)" && echo "$pw" | sudo -S cp com.ZUUM.scheduler.plist ~/Library/LaunchAgents'])
            # call(['launchctl', 'start', 'com.ZUUM.scheduler'])
            # call(['osascript', '-e', 'tell application (path to frontmost application as text) to display dialog "'+os.getcwd()+'" buttons {"OK"} with icon stop'])
            Popen(["/applications/zuum.app/contents/macos/zuumscheduler"])
            # Popen(["ZuumScheduler"])
            pass
        for i in range(3):
            try:
                conn = rpyc.connect('localhost', 12345, config={"allow_all_attrs": True, "allow_pickle": True})
                scheduler = conn.root
                break
            except ConnectionRefusedError:
                scheduler = None
                sleep(3)
    # elif 'add' in request.form:
    #     if sys.platform == "win32":
    #         call(["powershell.exe", "./addstartup.ps1"])
    #     else:
    #         # hi colin add zuum scheduler to startup
    #         call(['sh', 'addstartup.sh'])
    #         pass
    #     flash('Added to startup', 'info')
    return redirect(url_for('main'))
# @app.route("/calendar")
# def calendar():
#     #days = {"Sunday":[],"Monday":[],"Tuesday":[],"Wednesday":[],"Thursday":[],"Friday":[],"Saturday":[]}
#     days = {i:[] for i in range(7)}
#     for task in Task.get_task_list(scheduler).values():
#         for trigger in task.triggers:
#             days[trigger.day].append([trigger.formatted_time(), task, trigger.time.hour*60+trigger.time.minute+65])
#     data = [["Mon", days[0]],
#             ["Tue", days[1]],
#             ["Wed", days[2]],
#             ["Thu", days[3]],
#             ["Fri", days[4]],
#             ["Sat", days[5]],
#             ["Sun", days[6]]]
#     return render_template('Calendar.html', data=data)

@app.route("/meetings")
def meetings():
    return render_template('Meetings.html', meetings = Task.get_task_list(scheduler), linkre = Task.browser_re)

@app.route("/new_meeting", methods=["GET", "POST"])
def new_meeting():
    if request.method == "POST":
        data = json.loads(request.form['data'])
        name = request.form['meeting_name']
        link = request.form['meeting_link']
        triggers = []
        for day in data['triggers']:
            for trigger in day[2]:
                hour, minute = trigger[2].split(":")
                triggers.append(CTrigger(day_of_week=weeknums[day[0]], hour=int(hour), minute=int(minute)))
        newtask = Task.task_from_browser(name, True, link, triggers)
        newtask.configure(scheduler)
    return redirect(url_for('meetings'))

@app.route("/<string:id>/edit_task", methods=["GET", "POST"])
def edit_task(id):
    task = Task.get_task_list(scheduler)[id]
    changes = dict.fromkeys(('name', 'link', 'triggers', 'enabled'))
    if not task:
        flash("That task doesn't exist", "info")
    elif request.method == "POST":
        #tree_print(request.form['data'], 0)
        data = json.loads(request.form['data'])
        #print(data)
        changes['name'] = request.form['meeting_name']
        changes['link'] = request.form['meeting_link']
        changes['triggers'] = []
        for day in data['triggers']:
            for trigger in day[2]:
                hour, minute = trigger[2].split(":")
                changes['triggers'].append(CTrigger(day_of_week=weeknums[day[0]], hour=int(hour), minute=int(minute)))
        task.configure(scheduler, **changes)
        #flash("Meeting Updated", "success")
        #tree_print(changes, 0)
    return redirect(url_for('meetings'))

@app.route("/<string:id>/run_task", methods=["GET","POST"])
def run_task(id):
    task = Task.get_task_list(scheduler)[id]
    scheduler.run_job(task.id, 'default')
    #task.get_task_list(scheduler).print_tasks()
    flash("Running Task", "info")
    return redirect(url_for('meetings'))

@app.route("/<string:id>/delete_task", methods=["GET","POST"])
def delete_task(id):
    task = Task.get_task_list(scheduler)[id]
    task.delete(scheduler)
    # flash(f"Deleted {task.name}", "success")
    return redirect(url_for('meetings'))

@app.route("/<string:id>/enable_task", methods=["GET","POST"])
def enable_task(id):
    task = Task.get_task_list(scheduler)[id]
    task.enable(scheduler)
    flash(f"Enabled {task.name}", "success")
    return redirect(url_for('meetings'))

@app.route("/<string:id>/disable_task", methods=["GET","POST"])
def disable_task(id):
    task = Task.get_task_list(scheduler)[id]
    task.disable(scheduler)
    flash(f"Disabled {task.name}", "success")
    return redirect(url_for('meetings'))

@app.route("/<string:id>/change_state", methods=["GET","POST"])
def change_state(id):
    task = Task.get_task_list(scheduler)[id]
    # print(task.id)
    if task.enabled:
        task.disable(scheduler)
    else:
        task.enable(scheduler)
    return redirect(url_for('meetings'))

@app.route("/<string:id>/shift_task/<string:direction>", methods=["GET","POST"])
def shift_task(id, direction):
    # print(id, direction)
    tasklist = Task.get_task_list(scheduler)
    tasklist.shift_task(id, int(direction))
    return redirect(url_for('meetings'))

if __name__ == "__main__":
    ui.run()
    #app.run(debug=True)
