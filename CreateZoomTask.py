from tkinter import *
import re

window = Tk()
window.title("Task Information")
window.geometry("400x400")

n = 0

def show():
    global n
    n += 1
    Label(window, text=nameIn.get()).grid(column=0, row=n+3)
    Label(window, text=linkIn.get()).grid(column=1, row=n+3)

#gui elements
nameText = Label(window, text="Meeting Name:")
nameIn = Entry(window, width=20)
linkText = Label(window, text="Paste Zoom Link:")
linkIn = Entry(window, width="60")
tableName = Label(window, text="Name")
tableLink = Label(window, text="Link")
display = Button(window, text="Display", command=show)

#place elements
nameText.grid(column=0, row=0)
nameIn.grid(column=1, row=0, sticky="W")
linkText.grid(column=0, row=1)
linkIn.grid(column=1, row=1)
tableName.grid(column=0, row=3)
tableLink.grid(column=1, row=3)
display.grid(column=0, row=2)

window.mainloop()