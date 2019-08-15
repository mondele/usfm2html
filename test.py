#!/Library/Frameworks/Python.framework/Versions/3.7/bin/python3
# coding=<UTF-8>
import tkinter as tk
from tkinter import ttk
from tkinter import Menu
from tkinter import messagebox
from tkinter import filedialog

from os import path, makedirs

import sys

import json

from usfm2html import usfmConverter

convert_tit  = u""+' Convert Files '
prefs        = u""+' Preferences '
term_message = u""+"I think I'll just run in the terminal"
filebrowse   = u""+"Select File…"
destbrowse   = u""+"Select Target Directory"
actionName   = u""+"Convert the File(s)"

class main(object):
    def __init__(self):
        self.win = tk.Tk()

        self.win.title("USFM 2 HTML")
        self.create_widgets()
        #self.load_prefs()
    
    def _quit(self):
        self.win.quit()
        self.win.destroy()
        exit()
    
    def load_prefs(self):
        self.prefs = json.load("localization.json")
    
    def create_widgets(self):
        tabControl = ttk.Notebook(self.win)

        # Button Callback
        def getFileName():
            options = {}
            options['filetypes'] = [
                ('USFM files',('*.usfm','*.sfm')),
                ('text files',('*.txt,*.usfm,*.sfm')),
                ('all files', '*')
                ]
            fDir  = path.dirname(__file__)
            fName = list(filedialog.askopenfilenames(parent=self.win, initialdir=fDir,**options))
            self.fileEntry.config(state='enabled')
            self.fileEntry.delete(0, tk.END)
            self.fileEntry.insert(0, fName)
        
        def getDirectoryName():
            fDir = path.dirname(__file__)
            dName = filedialog.askdirectory(parent=self.win, initialdir=fDir)
            self.outDir.config(state='enabled')
            self.outDir.delete(0, tk.END)
            self.outDir.insert(0, dName)
            

        def convertFile():
            fileNames = self.fileEntry.get()
            directoryName = self.outDir.get()
            newConversion = usfmConverter(fileNames, directoryName)
            newConversion.usfm2html()

        tab1 = ttk.Frame(tabControl)
        tabControl.add(tab1, text=convert_tit)  # convert_tit text set from localization
        tab2 = ttk.Frame(tabControl)
        tabControl.add(tab2, text = prefs)      # prefs text set from localization

        tabControl.pack(expand=1, fill="both")

        conversion = ttk.LabelFrame(tab1, text = convert_tit)
        conversion.grid(column=0, row=0, padx=8, pady=4)

        cb = ttk.Button(conversion, text=filebrowse, command=getFileName)   # filebrowse text set from localization
        cb.grid(column=0, row=0, sticky=tk.W)
        
        fileLoc = tk.StringVar()
        self.entryLen = 20
        self.fileEntry = ttk.Entry(conversion, width=self.entryLen, textvariable=fileLoc)
        self.fileEntry.grid(column=1, row=0, sticky=tk.W)

        targetDir = tk.StringVar()
        self.outDir = ttk.Entry(conversion, width=self.entryLen, textvariable=targetDir)
        self.outDir.grid(column=1, row=1, sticky=tk.W)

        tb = ttk.Button(conversion, text=destbrowse, command=getDirectoryName)   # filebrowse text set from localization
        tb.grid(column=0, row=1, sticky=tk.W)

        ab = ttk.Button(conversion, text=actionName, command=convertFile)
        ab.grid(column=0, row=2, sticky=tk.W)


#mode = 1
#try:
#    if sys.stdin.isatty():
#        mode=0
#except AttributeError:  # stdin is NoneType if not in terminal mode
#    pass

#if mode == 0:
#    print(term_message) # term_message text set from localization
#else:
loop = main()
loop.win.mainloop()