from APSched import Task, Trigger, weeknums
from flask import Flask, render_template, request, redirect, url_for, flash
from flaskwebgui import FlaskUI
from datetime import datetime
import hashlib

import rpyc

import os
from apscheduler.schedulers.background import BackgroundScheduler
from appdata import appdata

from apscheduler.triggers.combining import OrTrigger

#zoompath = r"%APPDATA%\Zoom\bin\Zoom.exe"

app = Flask(__name__)

app.secret_key = '8af32bf6ff121fecbce4cbb67f5cb43b'

#ui = FlaskUI(app)

try:
    conn = rpyc.connect('localhost', 12345, config={"allow_all_attrs": True, "allow_pickle": True})
except ConnectionRefusedError:
    os.system("python server.py")
    try:
        conn = rpyc.connect('localhost', 12345, config={"allow_all_attrs": True, "allow_pickle": True})
    except ConnectionRefusedError:
        exit

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
    return render_template('Meetings.html', meetings = Task.get_task_list(scheduler))

@app.route("/new_meeting", methods=["GET", "POST"])
def new_meeting():
    if request.method == "POST":
        name = request.form['meeting_name']
        link = request.form['meeting_link']
        newtask = Task.task_from_browser(name, True, link)
        newtask.configure(scheduler, func='server:serv_joinMeeting')
        id = newtask.id
        return redirect(url_for('edit_task', id=id))
    return render_template('NewMeeting.html')

@app.route("/<string:id>/edit_task", methods=["GET", "POST"])
def edit_task(id):
    task = Task.get_task_list(scheduler)[id]
    changes = dict.fromkeys(('name', 'args', 'triggers', 'enabled'))
    if task == False:
        flash("That task doesn't exist", "info")
    elif request.method == "POST":
        if request.form['action'] == "+":
            changes['triggers'] = task.triggers
            day = request.form['trigger_day']
            time = request.form['time']
            if (len(time)==5):
                hour, minute = time.split(":")
                try:
                    changes['triggers'].append(Trigger(day_of_week=weeknums[day], hour=int(hour), minute=int(minute)))
                except AttributeError:
                    changes['triggers'] = [Trigger(day_of_week=weeknums[day], hour=int(hour), minute=int(minute))]
                task.configure(scheduler, **changes)
                flash("Run Time Added", "success")
            else:
                flash("Couldn't Add Time", "danger")
        elif request.form['action'] == "Save Changes":
            changes['name'] = request.form['meeting_name']
            link = request.form['meeting_link']
            changes['args'] = [Task.args_from_browser(link)]
            #numTriggers = len(task.triggers)
            #task.rename(name)
            #task.id = hashlib.sha224(task.get_task_name().encode('utf-8')).hexdigest()
            #task.set_link(link)
            
            day = request.form['trigger_day']
            time = request.form['time']
            if (len(time)==5):
                hour, minute = time.split(":")
                try:
                    changes['triggers'].append(Trigger(day_of_week=weeknums[day], hour=int(hour), minute=int(minute)))
                except AttributeError:
                    changes['triggers'] = [Trigger(day_of_week=weeknums[day], hour=int(hour), minute=int(minute))]
            enabled = request.form.get('enabled')
            changes['enabled'] = True if enabled == None else False
            for i in range(0,len(task.triggers)):
                day = request.form[f'trigger_day{i+1}']
                time = request.form[f'time{i+1}']
                if (len(time)==5):
                    hour, minute = time.split(":")
                    try:
                        changes['triggers'].append(Trigger(day_of_week=weeknums[day], hour=int(hour), minute=int(minute)))
                    except AttributeError:
                        changes['triggers'] = [Trigger(day_of_week=weeknums[day], hour=int(hour), minute=int(minute))]
            task.configure(scheduler, **changes)
            flash("Meeting Updated", "success")
        return redirect(url_for('edit_task', id=task.id))
    return render_template('EditTask.html', task=task)

@app.route("/<string:id>/run_task", methods=["GET","POST"])
def run_task(id):
    task = Task.get_task_list(scheduler)[id]
    scheduler.run_job(task.id, 'default')
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

if __name__ == "__main__":
    app.run(debug=True)