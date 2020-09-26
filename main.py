from TaskFunctions import *
from flask import Flask, render_template, request, redirect, url_for, flash
#from flaskwebgui import FlaskUI
from datetime import datetime
import hashlib

zoompath = r"%APPDATA%\Zoom\bin\Zoom.exe"

app = Flask(__name__)

app.secret_key = '8af32bf6ff121fecbce4cbb67f5cb43b'

#ui = FlaskUI(app)

current_triggers = []

def get_task_by_id(id):
    try:
        return [task for task in get_task_list() if task.id==id][0]
    except:
        return False

@app.route("/")
def calendar():
    days = {"Sunday":[],"Monday":[],"Tuesday":[],"Wednesday":[],"Thursday":[],"Friday":[],"Saturday":[]}
    for task in get_task_list():
        for trigger in task.triggers:
            days[trigger.day].append([trigger.formatted_time(), task, trigger.time.hour*60+trigger.time.minute+65])
    data = [["Sun", days["Sunday"]],
            ["Mon", days["Monday"]],
            ["Tue", days["Tuesday"]],
            ["Wed", days["Wednesday"]],
            ["Thu", days["Thursday"]],
            ["Fri", days["Friday"]],
            ["Sat", days["Saturday"]]]
    return render_template('Calendar.html', data=data)

@app.route("/meetings")
def meetings():
    return render_template('Meetings.html', meetings = get_task_list())

@app.route("/new_meeting", methods=["GET", "POST"])
def new_meeting():
    if request.method == "POST":
        name = request.form['meeting_name']
        link = request.form['meeting_link']
        newtask = Task(name, link)
        id = hashlib.sha224(newtask.commit_changes().encode('utf-8')).hexdigest()
        return redirect(url_for('edit_task', id=id))
    return render_template('NewMeeting.html')

@app.route("/<string:id>/edit_task", methods=["GET", "POST"])
def edit_task(id):
    task = get_task_by_id(id)
    if task == False:
        flash("That task doesn't exist", "info")
    elif request.method == "POST":
        if request.form['action'] == "Add Run Time":
            day = request.form['trigger_day']
            time = request.form['time']
            if (len(time)==5):
                hour, minute = time.split(":")
                trigger_time = datetime.now()
                trigger_time = trigger_time.replace(hour=int(hour), minute=int(minute))
                trigger = Trigger(day, trigger_time)
                task.add_trigger(trigger)
                task.commit_changes()
            flash("Run Time Added", "success")
        elif request.form['action'] == "Save Changes":
            name = f"ZoomJoin\\{request.form['meeting_name']}"
            task.delete()
            link = request.form['meeting_link']
            numTriggers = len(task.triggers)
            task.rename(name)
            task.id = hashlib.sha224(task.get_task_name().encode('utf-8')).hexdigest()
            task.set_link(link)
            task.triggers = []
            day = request.form['trigger_day']
            time = request.form['time']
            if (len(time)==5):
                hour, minute = time.split(":")
                trigger_time = datetime.now()
                trigger_time = trigger_time.replace(hour=int(hour), minute=int(minute))
                trigger = Trigger(day, trigger_time)
                task.add_trigger(trigger)
            for i in range(0,numTriggers):
                day = request.form[f'trigger_day{i+1}']
                time = request.form[f'time{i+1}']
                if (len(time)==5):
                    hour, minute = time.split(":")
                    trigger_time = datetime.now()
                    trigger_time = trigger_time.replace(hour=int(hour), minute=int(minute))
                    trigger = Trigger(day, trigger_time)
                    task.add_trigger(trigger)
            task.commit_changes()
            flash("Meeting Updated", "success")
        return redirect(url_for('edit_task', id=task.id))
    return render_template('EditTask.html', task=task)

@app.route("/<string:id>/run_task", methods=["GET","POST"])
def run_task(id):
    task = get_task_by_id(id)
    task.run()
    flash("Running Task", "info")
    return redirect(url_for('meetings'))

@app.route("/<string:id>/delete_task", methods=["GET","POST"])
def delete_task(id):
    task = get_task_by_id(id)
    task.delete()
    flash(f"Deleted {task.get_task_name()}", "success")
    return redirect(url_for('meetings'))

@app.route("/<string:id>/enable_task", methods=["GET","POST"])
def enable_task(id):
    task = get_task_by_id(id)
    task.enabled = True
    task.commit_changes()
    flash(f"Enabled {task.get_task_name()}", "success")
    return redirect(url_for('meetings'))

@app.route("/<string:id>/disable_task", methods=["GET","POST"])
def disable_task(id):
    task = get_task_by_id(id)
    task.enabled = False
    task.commit_changes()
    flash(f"Disabled {task.get_task_name()}", "success")
    return redirect(url_for('meetings'))

if __name__ == "__main__":
    app.run(debug=True)