# Create Zoom Task

This script creates a GUI for creating task scheduler tasks to run zoom meetings. You can schedule to automatically join certain meetings at certain times of certain days.

You only need the .exe file unless you want to customize the script. Simply run it and follow the instructions on screen.

## Instructions

You can download the .exe file by clicking on it then clicking download. Simply double click the .exe file to run the script. 

## Specifications

Meeting Name will identify the task. The task name will be ZoomJoin\[MeetingName\].

Hashed password is optional in the pasted Zoom link, but if it is not included, Zoom will ask for the meeting password every time the task is run.

You will have to open task scheduler to modify, remove, or manually run tasks. This functionality will come in a future update.

## Customization

If you didn't install Zoom in the default location, you can specify it near the top of the ahk script. 

## OS Compatability

At the moment, this script will only work on Windows, as other OSs don't support AutoHotKey. If you make this script work on any other OS, please submit a pull request so we can add it to this repo.
