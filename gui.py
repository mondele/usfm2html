#!/usr/bin/evn python3
import tkinter as tk
from tkinter import ttk
from tkinter import scrolledtext
from tkinter import Menu
from tkinter import messagebox
from tkinter import Spinbox

win = tk.Tk()
win.title("USFM 2 HTML")

class ToolTip(object):
    def __init__(self, widget):
        self.widget = widget
        self.tip_window = None

    def show_tip(self, tip_text):
        "Display text in a tooltip window"
        if self.tip_window or not tip_text:
            return
        x, y, _cx, cy = self.widget.bbox("insert")  # Get size of widget
        x = x + self.widget.winfo_rootx() + 25      # calculate to display tooltip
        y = y + cy + self.widget.winfo_rooty() +25  # below and to the right
        self.tip_window = tw = tk.Toplevel(self.widget) # create the new tooltip window
        tw.wm_overrideredirect(True)                # remove all Window Manager (wm) decorations
        tw.wm_geometry("+%d+%d" % (x, y))           #create window size

        label = tk.Label(tw, text=tip_text, justify=tk.LEFT,
         background="#ffffe0")
        label.pack(ipadx=1)
    
    def hide_tip(self):
        tw = self.tip_window
        self.tip_window = None
        if tw:
            tw.destroy()

def create_Tooltip(widget, text):
    ToolTip = ToolTip(widget)

    def enter(event):
        toolTip.show_tip(text)
    def leave(event):
        ToolTip.hide_tip()
    widget.bind('<Enter>', enter)
    widget.bind('<Leave>', leave)

class OOP():
    def __init__(self):
        self.win = tk.Tk()

        create_Tooltip(self.win, 'Hello GUI')

        self.win.title(" USFM 2 HTML ")
        self.create_widgets()

    def click_me(self):
        self.action.configure(text='Hello \n')
    
    def checkCallback(self, *ignored_args):
        if self.chVarUn.get():  self.check3.configure(state='disabled')
        else:                   self.check3.configure(state='normal')
        if self.chVarEn.get():  self.check2.configure(state='disabled')
        else:                   self.check2.configure(state='normal')
    
    # Update progressbar in callback loop
    def run_progressbar(self):
        self.progress_bar["maximum"] = 100
        for i in range(101):
            sleep(0.05)
            self.progress_bar["value"] = i  # increment progressbar
            self.progress_bar.update()      # have to call update() in loop
            self.progress_bar["value"] = 0  # reset/clear progressbar
    
    def start_progressbar(self):
        self.progress_bar.start()
    
    def stop_progressbar(self):
        self.progress_bar.stop()
    
    def progressbar_stop_after(self, wait_ms=1000):
        self.win.after(wait_ms, self.progress_bar.stop)
    
    # Exit GUI cleanly
    def _quit(self):
        self.win.quit()
        self.win.destroy()
        exit()
    
    def create_widgets(self):
        tabControl = ttk.Notebook(self.win)

        tab1 = ttk.Frame(tabControl)
        tabControl.add(tab1, text=" Converter ")

        tab2 = ttk.Frame(tabControl)
        tabControl.add(tab2, text=" Preferences ")

        tabControl.pack(expand=1, fill='both')

    # LabelFrame using tab1 as the parent
    converter = ttk.LabelFrame(tab1, text=' Files to Convert ')
    converter.grid(column=0, row=0, padx=8, pady=4)

    a_label = ttk.Label(converter, text="File(s) to convert:")
    a_label.grid(column=0, row=0, sticky='W')

    self.action = ttk.Button(converter, text="Select File", command=self.click_me)
    self.action.grid(column=2, row=1)

    # Tab Control 2
    self.prefs = ttk.LabelFrame(tab2, text=' Preferences ')
    self.prefs.grid(column=0, row=0, padx=8, pady=4)

    # Creating three checkboxes
    chVarDis = tk.IntVar()
    check1 = tk.Checkbutton(self.prefs, text="Disabled", variable=chVarDis, state='disabled')
    check1.select()
    check1.grid(column=0, row=0, sticky=tk.W)

    chVarUn = tk.IntVar()
    check2 = tk.Checkbutton(self.prefs, text="Unchecked", variable=chVarUn)
    check2.deselect()
    check2.grid(column=1, row=0, sticky=tk.W)

    chVarEn = tk.IntVar()
    check3 = tk.Checkbutton(self.prefs, text="Enabled", variable=chVarEn)
    check3 = deselect()
    check3.grid(column=1, row=0, sticky=tk.W)

    chVarUn.trace('w', lambda unused0, unused1, unused2 : self.checkCallbac())
    chVarEn.trace('w', lambda unused0, unused1, unused2 : self.checkCallbac())

    # Progressbar is for tab 1 (unlike in recipe)
    self.progress_bar = ttk.Progressbar(tab1, orient='horizontal', length=286, mode='determinate')
    self.progress_bar.grid(column=0, row=3, pady=2)