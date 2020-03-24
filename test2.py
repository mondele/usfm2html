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
        self.action.configure(text=' Hello ') # Action is an object? instantiated from ttk.Button
    
    def createWidgets(self):
        
        def radioCall(): # Function for switching between File and Directory modes
            radioSel = radioVar.get()
            print(radioSel)
        
        self.tabControl = ttk.Notebook(self.win)    # Use tkinter.Notebook class for window filling
                                                    # Our filler is called tabControl
        tab1 = ttk.Frame(self.tabControl)
        self.tabControl.add(tab1, text=" Convert Files ")  # Create a tab in the Notebook called tab1. It will have the title "Convert Files"
        
        tab2 = ttk.Frame(self.tabControl)
        self.tabControl.add(tab2, text = " Preferences ")      # prefs text set from localization

        # tab1 is henceforth known as "conversion"
        conversion = ttk.LabelFrame(tab1, text="Convert Files")
        conversion.grid(column=0, row=0, padx=9, pady=4)
        
      ######## Create radio buttons to switch between File and Directory mode
      
        radioVar = tk.IntVar()                       # Create radio button named radioVar
        radioVar.set(0)                               # Default value 0

        setFile = tk.Radiobutton(conversion, text="Convert a File", variable= radioVar, value=0, command=radioCall)
        setFile.grid(column=0, row=0, sticky=tk.W)

        setDir = tk.Radiobutton(conversion, text="Convert a Directory", variable=radioVar, value=1, command=radioCall)
        setDir.grid(column=1, row=0, sticky=tk.W)

      ######## Buttons created

        # now we set tab2 to be called "configuration"
        configuration = ttk.LabelFrame(tab2, text="Preferences")
        configuration.grid(column=0, row=0, padx=9, pady=4)

        self.action = ttk.Button(win, text="Convert", command=self.clickMe)

        self.tabControl.pack(expand=1, fill="both")         # This adds the tabs visibly to the Notebook
        
#==============
# Start GUI
#==============
mainWindow = main()
mainWindow.win.mainloop()