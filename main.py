from APSched import Task, Trigger, TaskList, weeknums, tree_print
from flask import Flask, render_template, request, redirect, url_for, flash
from flaskwebgui import FlaskUI
from datetime import datetime
import hashlib
import json

import rpyc

import os
import sys
from apscheduler.schedulers.background import BackgroundScheduler
from appdata import appdata

from apscheduler.triggers.combining import OrTrigger

from subprocess import Popen

#zoompath = r"%APPDATA%\Zoom\bin\Zoom.exe"

app = Flask(__name__)

app.secret_key = '8af32bf6ff121fecbce4cbb67f5cb43b'

#ui = FlaskUI(app)

try:
    conn = rpyc.connect('localhost', 12345, config={"allow_all_attrs": True, "allow_pickle": True})
except ConnectionRefusedError:
    print('error')
    devnull = open(os.devnull, 'wb') # Use this in Python < 3.3
    # Python >= 3.3 has subprocess.DEVNULL
    #Popen([sys.executable, 'server.py'], stdout=devnull, stderr=devnull, shell=True)
    try:
        conn = rpyc.connect('localhost', 12345, config={"allow_all_attrs": True, "allow_pickle": True})
    except ConnectionRefusedError:
        exit('could not connect to server')

scheduler = conn.root

current_triggers = []


@app.route("/")
@app.route("/calendar")
def calendar():
    #days = {"Sunday":[],"Monday":[],"Tuesday":[],"Wednesday":[],"Thursday":[],"Friday":[],"Saturday":[]}
    days = {i:[] for i in range(7)}
    for task in Task.get_task_list(scheduler).values():
        for trigger in task.triggers:
            days[trigger.day].append([trigger.formatted_time(), task, trigger.time.hour*60+trigger.time.minute+65])
    data = [["Mon", days[0]],
            ["Tue", days[1]],
            ["Wed", days[2]],
            ["Thu", days[3]],
            ["Fri", days[4]],
            ["Sat", days[5]],
            ["Sun", days[6]]]
    return render_template('Calendar.html', data=data)

@app.route("/meetings")
def meetings():
    return render_template('Meetings.html', meetings = Task.get_task_list(scheduler), linkre = Task.browser_re)

@app.route("/new_meeting", methods=["GET", "POST"])
def new_meeting():
    if request.method == "POST":
        name = request.form['meeting_name']
        link = request.form['meeting_link']
        newtask = Task.task_from_browser(name, False, link)
        newtask.configure(scheduler)
        id = newtask.id
        return redirect(url_for('edit_task', id=id))
    return render_template('NewMeeting.html')

@app.route("/<string:id>/edit_task", methods=["GET", "POST"])
def edit_task(id):
    print('hello')
    task = Task.get_task_list(scheduler)[id]
    changes = dict.fromkeys(('name', 'link', 'triggers', 'enabled'))
    if not task:
        flash("That task doesn't exist", "info")
    elif request.method == "POST":
        data = json.loads(request.form['data'])
        #print(data)
        changes['name'] = request.form['meeting_name']
        changes['link'] = request.form['meeting_link']
        for day in data['triggers']:
            for trigger in day[2]:
                hour, minute = trigger[2].split(":")
                try:
                    changes['triggers'].append(Trigger(day_of_week=weeknums[day[0]], hour=int(hour), minute=int(minute)))
                except AttributeError:
                    changes['triggers'] = [Trigger(day_of_week=weeknums[day[0]], hour=int(hour), minute=int(minute))]
        task.configure(scheduler, **changes)
        flash("Meeting Updated", "success")
        # tree_print(data, 0)
    return redirect(url_for('meetings'))

@app.route("/<string:id>/run_task", methods=["GET","POST"])
def run_task(id):
    task = Task.get_task_list(scheduler)[id]
    #scheduler.run_job(task.id, 'default')
    #print(task.get_task_list(scheduler).print_tasks())
    print(url_for('edit_task', id=task.id))
    flash("Running Task", "info")
    return redirect(url_for('meetings'))

@app.route("/<string:id>/delete_task", methods=["GET","POST"])
def delete_task(id):
    task = Task.get_task_list(scheduler)[id]
    task.delete(scheduler)
    flash(f"Deleted {task.name}", "success")
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
    print(task.id)
    if task.enabled:
        task.disable(scheduler)
    else:
        task.enable(scheduler)
    return redirect(url_for('meetings'))

@app.route("/<string:id>/shift_task/<string:direction>", methods=["GET","POST"])
def shift_task(id, direction):
    print(id, direction)
    tasklist = Task.get_task_list(scheduler)
    tasklist.shift_task(id, int(direction))
    return redirect(url_for('meetings'))
"""
@app.route("/<string:id>/shift_down", methods=["GET","POST"])
def shift_down(id):
    tasklist = Task.get_task_list(scheduler)
    tasklist.shift_task(id, 1)
    return redirect(url_for('meetings'))
"""
if __name__ == "__main__":
    app.run(debug=True)