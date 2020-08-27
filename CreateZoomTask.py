from tkinter import *

window = Tk()
window.title("Task Information") 

#gui elements
nameText = Label(window, text="Meeting Name:")
nameIn = Entry(window, width=20)
linkText = Label(window, text="Paste Zoom Link:")
linkIn = Entry(window, width="60")

#place elements
nameText.grid(column=0, row=0)
nameIn.grid(column=1, row=0, sticky="W")
linkText.grid(column=0, row=1)
linkIn.grid(column=1, row=1)

window.mainloop()