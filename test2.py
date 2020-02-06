#!/Library/Frameworks/Python.framework/Versions/3.7/bin/python3
# coding=<UTF-8>
import tkinter as tk
from tkinter import ttk
from tkinter import Menu
from tkinter import messagebox
from tkinter import filedialog

class main(object):
    def __init__(self):
        # Create instance
        self.win = tk.Tk()  # instance is called 'win'

        # Add title to window
        self.win.title("Python GUI - USFM 2 HTML")
        self.createWidgets()    # In typical OOP fashion, we're calling a method that is defined below.
    
    # Callback functions
    def clickMe(self):
        self.action.configure(text=' Hello ')
    
    def createWidgets(self):
        self.tabControl = ttk.Notebook(self.win)    # Use tkinter.Notebook class for window filling
                                                    # Our filler is called tabControl
        tab1 = ttk.Frame(self.tabControl)
        self.tabControl.add(tab1, text=" Convert Files ")  # convert_tit text set from localization
        
        tab2 = ttk.Frame(self.tabControl)
        self.tabControl.add(tab2, text = " Preferences ")      # prefs text set from localization

#==============
# Start GUI
#==============
mainWindow = main()
mainWindow.win.mainloop()