from tkinter import *
from tkinter import ttk
from tkinter import font
from tkinter import messagebox
import re
from TaskFunctions import *
import datetime
import calendar

zoompath = r"%APPDATA%\Zoom\bin\Zoom.exe"

window = Tk()
window.title("Zoom Task Manager")
window.geometry("780x400")
window.resizable(False, True)

titlefont = font.Font(family='Arial', size=10, underline=1)

class MainWindow:
    def __init__(self, master):
        self.master = master
        self.scrollregion = Frame(self.master)
        self.scrollregion.pack(fill=BOTH, expand=1)
        self.canvas = Canvas(self.scrollregion, width=400, height=400, highlightthickness=0)
        self.canvas.pack(side=LEFT, fill=BOTH, expand=1)
        self.scrollbar = Scrollbar(self.scrollregion, command=self.canvas.yview)
        self.scrollbar.pack(side=RIGHT, fill='y')
        self.canvas.configure(yscrollcommand = self.scrollbar.set)
        self.canvas.bind('<Configure>', lambda e: self.canvas.configure(scrollregion=self.canvas.bbox('all')))
        self.tasklist = Frame(self.canvas, width=780)
        self.canvas.create_window((0,0), window=self.tasklist, anchor='nw')
        self.refresh_list()
    def show(self):
        self.taskNameLabel.pack(side=LEFT)
        self.triggersLabel.pack(side=LEFT)
        self.nextRunLabel.pack(side=LEFT)
        self.columnLabels.pack(fill=X)
        for task in self.taskframes:
            task.show()
        self.createButton.pack(side=LEFT)
    def edit(self, task):
        self.popupWindow = Toplevel(self.master)
        self.popupWindow.transient(self.master)
        self.popupWindow.geometry("%dx%d+%d+%d" % (430,250,self.master.winfo_x() + 50, self.master.winfo_y() + 130))
        self.popupWindow.resizable(False, True)
        self.popup = PopupMenu(self.popupWindow, self, type='edit', task=task)
        self.popupWindow.grab_set()
        self.master.wait_window(self.popupWindow)
    def create(self):
        self.popupWindow = Toplevel(self.master)
        self.popupWindow.transient(self.master)
        self.popupWindow.geometry("%dx%d+%d+%d" % (430,250,self.master.winfo_x() + 50, self.master.winfo_y() + 130))
        self.popupWindow.resizable(False, True)
        self.popup = PopupMenu(self.popupWindow, self)
        self.popupWindow.grab_set()
        self.master.wait_window(self.popupWindow)
    def get_list(self):
        self.list = get_task_list()
        self.taskframes = []
        for i in range(len(self.list)):
            self.taskframes.append(TaskFrame(self.list[i], Frame(self.tasklist), self))
    def refresh_list(self):
        self.tasklist.destroy()
        self.tasklist = Frame(self.canvas, width=780)
        self.canvas.create_window((0,0), window=self.tasklist, anchor='nw')
        self.get_list()
        self.columnLabels = Frame(self.tasklist)
        self.taskNameLabel = Sized_Label(self.columnLabels, width=150, height=20, text="Tasks", font=titlefont, anchor='w')
        self.triggersLabel = Sized_Label(self.columnLabels, width=375, height=20, text="Triggers", font=titlefont, anchor='w')
        self.nextRunLabel = Sized_Label(self.columnLabels, width=100, height=20, text="Next Run Time", font=titlefont, anchor='w')
        self.createButton = Button(self.tasklist, text="Create new task", command=self.create, anchor='w')
        #self.tasklist.pack(fill='y', side=LEFT)#grid(column=0, row=0)
        self.show()

class TaskFrame:
    def __init__(self, task, parent, masterclass):
        self.masterclass = masterclass
        self.parent = parent
        self.task = task
        self.triggertext = ''
        for i in range(len(task.triggers)):
            self.triggertext += f"{task.triggers[i].time.time().strftime('%I:%M:%S %p').lstrip('0')} {task.triggers[i].day}"
            if i < len(task.triggers)-1:
                self.triggertext += ", "
        self.triggertext += 'No triggers' if self.triggertext == '' else ''
        self.nexttext = f"{task.get_next_trigger().time.time().strftime('%I:%M:%S %p').lstrip('0')} {task.get_next_trigger().day}" if task.triggers else 'No Triggers'
        self.namebutton = Sized_Label(parent, width=150, height=20, text=task.get_task_name(), anchor='w')
        self.triggers = Sized_Label(parent, width=375, height=20, text=self.triggertext, anchor='w')
        self.nextTrigger = Sized_Label(parent, width=155, height=20, text=self.nexttext, anchor='w')
        self.runbutton = Button(parent, text="Run", command=self.run)
        self.editbutton = Button(parent, text="Edit", command=self.edit)
        self.deletebutton = Button(parent, text="-", bg='red3', fg='white', command=self.delete)
    def show(self):
        self.parent.pack(fill=X)
        self.namebutton.pack(side=LEFT)
        self.triggers.pack(side=LEFT)
        self.nextTrigger.pack(side=LEFT)
        self.runbutton.pack(side=LEFT)
        self.editbutton.pack(side=LEFT)
        self.deletebutton.pack(side=LEFT)
    def run(self):
        run_task(self.task)
    def edit(self):
        self.masterclass.edit(self.task)
    def delete(self):
        self.confirm = messagebox.askquestion('Delete Task',f"Delete {self.task.get_task_name()}?")
        if self.confirm == 'yes':
            delete_task(self.task)
            self.masterclass.refresh_list()

class TriggerFrame:
    def __init__(self, trigger, parent, parentclass):
        self.parent = parent
        self.parentclass = parentclass
        self.trigger = trigger
        self.day = StringVar()
        self.time = StringVar()
        self.daytext = Label(self.parent, textvariable=self.day, width=20, anchor='w')
        self.timetext = Label(self.parent, textvariable=self.time, width=30, anchor='w')
        self.refresh()
        self.editbutton = Button(parent, text="Edit", command=self.edit)
        self.deletebutton = Button(parent, text="-", bg='red3', fg='white', command=self.delete)
    def show(self):
        self.parent.pack(fill=X)
        self.daytext.pack(side=LEFT)
        self.timetext.pack(side=LEFT)
        self.editbutton.pack(side=LEFT)
        self.deletebutton.pack(side=LEFT)
    def edit(self):
        self.popupWindow = Toplevel(self.parent.master)
        self.popupWindow.transient(self.parent.master)
        self.popupWindow.geometry("+%d+%d" % (self.parentclass.master.winfo_x() + 50, self.parentclass.master.winfo_y() + 50))
        self.popup = TriggerPopup(self.popupWindow, self.parentclass, type='edit', triggerframe=self)
        self.popupWindow.grab_set()
        self.parent.master.wait_window(self.popupWindow)
    def refresh(self):
        self.day.set(self.trigger.day)
        self.time.set(self.trigger.time.time().strftime("%I:%M:%S %p").lstrip("0").replace(" 0", " "))
    def delete(self):
        self.parent.destroy()
        self.parentclass.triggerlist.remove(self)

class PopupMenu:
    def __init__(self, master, masterclass, type='create', task=''):
        self.master = master
        self.masterclass = masterclass
        
        self.scrollregion = Frame(self.master)
        self.scrollregion.pack(fill=BOTH, expand=1)
        self.canvas = Canvas(self.scrollregion, width=400, height=400, highlightthickness=0)
        self.canvas.pack(side=LEFT, fill=BOTH, expand=1)
        self.scrollbar = Scrollbar(self.scrollregion, command=self.canvas.yview)
        self.scrollbar.pack(side=RIGHT, fill='y')
        self.canvas.configure(yscrollcommand = self.scrollbar.set)
        self.canvas.bind('<Configure>', lambda e: self.canvas.configure(scrollregion=self.canvas.bbox('all')))
        self.mainframe = Frame(self.canvas)
        self.canvas.create_window((0,0), window=self.mainframe, anchor='nw')
        
        self.entryFrame = Frame(self.mainframe)
        self.triggerFrame = Frame(self.mainframe)
        self.confirmFrame = Frame(self.mainframe)
        self.name = StringVar()
        self.link = StringVar()
        self.error = StringVar()
        self.task = task
        self.type=type

        self.get_list()

        if type == 'create':
            self.master.title('Create New Task')
        elif type == 'edit' and self.task != '':
            self.master.title('Edit Task')
            self.name.set(task.get_task_name())
            self.link.set(task.get_link())
        else:
            raise ValueError("Invalid type or can't edit nonexistant task")
        
        #variables
        self.prevValidName = self.name.get()
        self.prevName = self.name.get()
        self.prevValidLink = self.link.get()
        self.prevLink = self.link.get()

        #trace
        self.name.trace('w', lambda n, i, m: self.edit_check('name', self.name.get(), self.link.get()))
        self.link.trace('w', lambda n, i, m: self.edit_check('link', self.name.get(), self.link.get()))

        #elements
        self.errorText = Label(self.mainframe, textvariable=self.error)
        self.nameText = Label(self.entryFrame, text='Meeting Name:')
        self.nameIn = Entry(self.entryFrame, width=20, textvariable=self.name)
        self.nameIn.bind("<FocusOut>", lambda e: self.edit_check('name', self.name.get(), self.link.get(), focusout=True))
        self.linkText = Label(self.entryFrame, text="Paste Zoom Link:")
        self.linkIn = Entry(self.entryFrame, width=51, textvariable=self.link)
        self.linkIn.bind("<FocusOut>", lambda e: self.edit_check('link', self.name.get(), self.link.get(), focusout=True))
        self.triggerText = Label(self.triggerFrame, font=titlefont, text='Triggers')
        self.addTriggerButton = Button(self.mainframe, text='Add Trigger', command=self.createTrigger)
        self.okButton = Button(self.confirmFrame, text=('Apply' if type == 'edit' else 'Create'), command=self.install_task)
        self.cancelButton = Button(self.confirmFrame, text='Cancel', command=lambda:self.master.destroy())
        #show
        self.entryFrame.grid(column=0, row=0, sticky="W")
        self.triggerFrame.grid(column=0, row=1, sticky="W")
        self.addTriggerButton.grid(column=0, row=2, sticky='W')
        self.errorText.grid(column=0, row=3, sticky="W")
        self.confirmFrame.grid(column=0, row=4, sticky="W")
        self.nameText.grid(column=0, row=1, sticky='W')
        self.nameIn.grid(column=1, row=1, sticky="W")
        self.linkText.grid(column=0, row=2)
        self.linkIn.grid(column=1, row=2)
        
        
        self.okButton.grid(column=0, row=0, sticky="W")
        self.cancelButton.grid(column=1, row=0, sticky="W")

        self.triggerText.pack(side=TOP, anchor='w')
        self.show()

        self.edit_check('name', self.name.get(), self.link.get())

        self.master.lift()
    def edit_check(self, field, name, link, focusout=False):
        if name_validate(self.name.get()) and link_validate(self.link.get()):
            self.okButton.configure(state=NORMAL)
            self.error.set("You should be good to go!")
            return
        self.okButton.configure(state=DISABLED)
        if field == 'name':
            if name_validate(self.prevName) or self.prevName == '':
                    self.prevValidName = self.prevName
            if not name_validate(name):
                self.error.set("Invalid name")
                if focusout:
                    self.name.set(self.prevValidName)
                    if self.prevName != '':
                        self.error.set("You should be good to go! (Invalid name replaced with most recent valid name)")
                    elif name != '':
                        self.error.set("The name you entered was invalid")
            else:
                self.error.set("You should be good to go!")
            #print(self.prevValidName, self.prevName, name)
            self.prevName = name
        elif field == 'link':
            if link_validate(self.prevLink) or self.prevLink == '':
                    self.prevValidLink = self.prevLink
            if not link_validate(link):
                self.error.set("Invalid link")
                if focusout:
                    self.link.set(self.prevValidLink)
                    if self.prevLink != '':
                        self.error.set(f"You should be good to go! (invalid link was replaced with most recent valid link)")
                    elif link != '':
                        self.error.set("The link you entered was invalid")
        if name == '':
            self.error.set("Missing Name")
        elif link == '':
            self.error.set("Missing Link")
    def createTrigger(self):
        self.popupWindow = Toplevel(self.master)
        self.popupWindow.transient(self.master)
        self.popupWindow.geometry("+%d+%d" % (self.master.winfo_x() + 50, self.master.winfo_y() + 50))
        self.popup = TriggerPopup(self.popupWindow, self)
        self.popupWindow.grab_set()
        self.master.wait_window(self.popupWindow)
    def newtrigger(self, day, time):
        trigger = Trigger(day, time)
        self.triggerlist.append(TriggerFrame(trigger, Frame(self.triggerFrame), self))
    def get_list(self):
        self.triggerlist = []
        self.list = self.task.triggers if self.task != '' else []
        for trigger in self.list:
            self.triggerlist.append(TriggerFrame(trigger, Frame(self.triggerFrame), self))
    def show(self):
        for trigger in self.triggerlist:
            trigger.show()
    def install_task(self):
        newtask = Task(self.name.get(), self.link.get(), [triggerframe.trigger for triggerframe in self.triggerlist])
        if self.type == 'edit':
            delete_task(self.task)
        write_XML(build_XML(create_XML_tree('task'), newtask, zoompath), install=True, filename='uitest', taskname=newtask.get_task_name())
        self.masterclass.refresh_list()
        self.master.destroy()

class TriggerPopup:
    def __init__(self, master, parentclass, type='create', triggerframe=''):
        self.master = master
        self.parentclass = parentclass
        self.dayFrame = Frame(self.master)
        self.triggerframe = triggerframe
        self.type = type
        if self.type == 'create':
            self.master.title('Create New Trigger')
            self.dentry = DateEntry(self.dayFrame, font=('Arial', 15, NORMAL), border=0)
            self.okButton = Button(self.master, text='Create', command=self.create)
        elif self.type == 'edit' and self.triggerframe.trigger != '':
            self.master.title('Edit Trigger')
            self.dentry = DateEntry(self.dayFrame, day=self.triggerframe.trigger.day, time=self.triggerframe.trigger.time, font=('Arial', 13, NORMAL), border=0)
            self.okButton = Button(self.master, text='Apply', command=self.edit)
        else:
            raise ValueError("Invalid type or can't edit nonexistant trigger")
        #self.okButton = Button(self.master, text=('Apply' if self.type == 'edit' else 'Create'), command=self.create)
        self.dayFrame.pack()
        self.dentry.pack()
        self.okButton.pack()
    def create(self):
        self.dentry.fix('ampm')
        self.dentry.fix('hour')
        self.dentry.fix('minute')
        self.dentry.fix('second')
        self.parentclass.newtrigger(self.dentry.day.get(), datetime.datetime.strptime(f"{datetime.datetime.now().year:04}-{datetime.datetime.now().month:02}-{datetime.datetime.now().day:02} {self.dentry.hour.get().zfill(2)}:{self.dentry.minute.get().zfill(2)}:{self.dentry.second.get().zfill(2)} {self.dentry.ampm.get()}", "%Y-%m-%d %I:%M:%S %p"))
        self.parentclass.show()
        self.master.destroy()
    def edit(self):
        self.dentry.fix('ampm')
        self.dentry.fix('hour')
        self.dentry.fix('minute')
        self.dentry.fix('second')
        self.triggerframe.trigger = Trigger(self.dentry.day.get(), datetime.datetime.strptime(f"{datetime.datetime.now().year:04}-{datetime.datetime.now().month:02}-{datetime.datetime.now().day:02} {self.dentry.hour.get().zfill(2)}:{self.dentry.minute.get().zfill(2)}:{self.dentry.second.get().zfill(2)} {self.dentry.ampm.get()}", "%Y-%m-%d %I:%M:%S %p"))
        self.triggerframe.refresh()
        self.master.destroy()

class DateEntry(Frame):
    def __init__(self, master, day='', time='', frame_look={}, **look):
        args = dict(relief=SUNKEN, border=1)
        args.update(frame_look)
        Frame.__init__(self, master, **args)

        args = {'relief': FLAT}
        args.update(look)
        self.day = StringVar()
        self.hour = StringVar()
        self.minute = StringVar()
        self.second = StringVar()
        self.ampm = StringVar()
        
        if day != '':
            self.day.set(day)
        else:
            self.day.set(datetime.datetime.now().strftime('%A'))
        if time != '':
            self.hour.set(time.strftime('%I'))
            self.minute.set(time.minute)
            self.second.set(time.second)
            self.ampm.set(time.strftime('%p'))
        self.fix('ampm')
        self.fix('hour')
        self.fix('minute')
        self.fix('second')
        incrmt = self.master.register(self.increment)
        check = self.master.register(self.actual_check)

        self.day_entry = ttk.Combobox(self, width=20, state="readonly", textvariable = self.day)
        self.day_entry['values'] = ('Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday')

        self.entry_1 = Entry(self, width=2, textvariable=self.hour, name='hour', validate = 'key', validatecommand=(check, '%W', '%P'), **args)
        self.label_1 = Label(self, text=':', **args)
        self.entry_2 = Entry(self, width=2, textvariable=self.minute, name='minute', validate = 'key', validatecommand=(check, '%W', '%P'), **args)
        self.label_2 = Label(self, text=':', **args)
        self.entry_3 = Entry(self, width=2, textvariable=self.second, name='second', validate = 'key', validatecommand=(check, '%W', '%P'), **args)
        self.label_3 = Label(self, text=' ', **args)
        self.entry_4 = Spinbox(self, width=3, textvariable=self.ampm, command=(incrmt, '%d'), name='ampm', validate = 'key', validatecommand=(check, '%W', '%P'), **args)

        self.entry_1.bind('<FocusIn>', lambda e: self.select(self.entry_1))
        self.entry_1.bind('<FocusOut>', lambda e: self.fix('hour'))
        self.entry_2.bind('<FocusIn>', lambda e: self.select(self.entry_2))
        self.entry_2.bind('<FocusOut>', lambda e: self.fix('minute'))
        self.entry_3.bind('<FocusIn>', lambda e: self.select(self.entry_3))
        self.entry_3.bind('<FocusOut>', lambda e: self.fix('second'))
        self.entry_4.bind('<FocusIn>', lambda e: self.select(self.entry_4))
        self.entry_4.bind('<FocusOut>', lambda e: self.fix('ampm'))


        self.day_entry.pack(side=LEFT)
        self.entry_1.pack(side=LEFT)
        self.label_1.pack(side=LEFT)
        self.entry_2.pack(side=LEFT)
        self.label_2.pack(side=LEFT)
        self.entry_3.pack(side=LEFT)
        self.label_3.pack(side=LEFT)
        self.entry_4.pack(side=LEFT)
        
        #print(f"{datetime.datetime.now().year:04}-{datetime.datetime.now().month:02}-{datetime.datetime.now().day:02} {self.hour.get().zfill(2)}:{self.minute.get().zfill(2)}:{self.second.get().zfill(2)} {self.ampm.get()}")

        #self.entry_1.bind('<KeyRelease>', lambda e: self._check(0, 2))
        #self.entry_2.bind('<KeyRelease>', lambda e: self._check(1, 2))
        #self.entry_3.bind('<KeyRelease>', lambda e: self._check(2, 2))
        #self.entry_4.bind('<KeyRelease>', lambda e: self._check(3, 2))
    
    def actual_check(self, name, after):
        focus = name.split('.')[-1]
        #print(name, focus)
        if focus == 'hour':
            return (after.isdigit() and int(after) >= 1 and int(after) <= 23) or after == ''
        elif focus == 'minute':
            return (after.isdigit() and int(after) >= 0 and int(after) <= 59) or after == ''
        elif focus == 'second':
            return (after.isdigit() and int(after) >= 0 and int(after) <= 59) or after == ''
        elif focus == 'ampm':
            return bool(re.compile(r"^([aA]|[pP])[mM]?$").match(after)) or after == ''
        else:
            return False

    def fix(self, focus):
        if focus == 'hour':
            if self.hour.get() == '':
                self.hour.set(datetime.datetime.now().hour)
            if int(self.hour.get()) > 12:
                self.ampm.set('PM')
            self.hour.set(f'{(((int(self.hour.get()) - 1) % 12) + 1):02}')
        if focus == 'minute':
            if self.minute.get() == '':
                self.minute.set('0')
            self.minute.set(f'{int(self.minute.get()):02}')
        if focus == 'second':
            if self.second.get() == '':
                self.second.set('0')
            self.second.set(f'{int(self.second.get()):02}')
        elif focus == 'ampm':
            self.ampm.set('AM' if bool(re.compile(r"^[aA][mM]?$").match(self.ampm.get())) else 'PM')

    def increment(self, direction, block=1):
        focus = str(self.master.focus_get()).split('.')[-1]
        #print(direction, focus)
        add = 1 if direction == 'up' else -1
        if focus == 'hour':
            self.hour.set(f"{(((int(self.hour.get()) + add - 1) % 12) + 1):02}")
        elif focus == 'minute':
            self.minute.set(f"{(block * round((int(self.minute.get()) + add * block) / block) % 60):02}")
        elif focus == 'second':
            self.second.set(f"{((int(self.second.get()) + add) % 60):02}")
        elif focus == 'ampm':
            self.ampm.set('PM' if self.ampm.get() == 'AM' else 'AM')
    def select(self, widget):
        #print(widget)
        widget.selection_range(0, END)

class Sized_Label(Frame):
    def __init__(self, master, width=0, height=0, **kwargs):
        self.width = width
        self.height = height

        Frame.__init__(self, master, width=self.width, height=self.height)
        self.Label_widget = Label(self, **kwargs)
        self.Label_widget.pack(expand=YES, fill=BOTH)

    def pack(self, *args, **kwargs):
        Frame.pack(self, *args, **kwargs)
        self.pack_propagate(False)

    def grid(self, *args, **kwargs):
        Frame.grid(self, *args, **kwargs)
        self.grid_propagate(False)

#gui elements
#createTab = ttk.Frame(create)

#nameText = Label(createTab, text="Meeting Name:")
#nameIn = Entry(createTab, width=20)
#linkText = Label(createTab, text="Paste Zoom Link:")
#linkIn = Entry(createTab, width=40)



#place elements
#nameText.grid(column=0, row=1)
#nameIn.grid(column=1, row=1, sticky="W")
#linkText.grid(column=0, row=2)
#linkIn.grid(column=1, row=2)

#createTab.grid(column=0, row=0)



#show_task_frames(taskframes)

#popup = PopupMenu(window, 'edit', taskframes[0].task)
#print(popup.name.get())
#popup.name.trace("w", callback)




mainwindow = MainWindow(window)
mainwindow.show()
#mainwindow.edit(mainwindow.taskframes[1].task)
window.mainloop()