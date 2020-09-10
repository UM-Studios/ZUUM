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
    return [task for task in get_task_list() if task.id==id][0]

@app.route("/")
def index():
    return render_template('MeetingManager.html')

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
        return redirect(url_for('trigger', id=id))
    return render_template('NewMeeting.html')

@app.route("/<string:id>/trigger", methods=["GET", "POST"])
def trigger(id):
    task = get_task_by_id(id)
    if request.method == "POST":
        day = request.form['trigger_day']
        time = request.form['time']
        hour, minute = time.split(":")
        trigger_time = datetime.now()
        trigger_time = trigger_time.replace(hour=int(hour), minute=int(minute))
        trigger = Trigger(day, trigger_time)
        task.add_trigger(trigger)
        task.commit_changes()
        return redirect(url_for('trigger', id=task.id))
    return render_template('AddTrigger.html', task=task)

@app.route("/<string:id>/edit_task", methods=["GET", "POST"])
def edit_task(id):
    task = get_task_by_id(id)
    if request.method == "POST":
        day = request.form['trigger_day']
        time = request.form['time']
        hour, minute = time.split(":")
        trigger_time = datetime.now()
        trigger_time = trigger_time.replace(hour=int(hour), minute=int(minute))
        trigger = Trigger(day, trigger_time)
        task.add_trigger(trigger)
        task.commit_changes()
        return redirect(url_for('trigger', id=task.id))
    return render_template('AddTrigger.html', task=task)

@app.route("/<string:id>/run_task", methods=["GET","POST"])
def run_task(id):
    task = get_task_by_id(id)
    task.run()
    flash("Running Task", "info")
    return redirect(url_for('meetings'))

@app.route("/<string:id>/delete_task", methods=["GET","POST"])
def delete_task(id):
    task = get_task_by_id(id)
    name = task.get_task_name()
    task.delete()
    flash(f"Deleted {name}", "info")
    return redirect(url_for('meetings'))

if __name__ == "__main__":
    app.run(debug=True)