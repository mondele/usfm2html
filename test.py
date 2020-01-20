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

from usfmModule import usfmConverter

convert_tit  = u""+' Convert Files '
prefs        = u""+' Preferences '
term_message = u""+"I think I'll just run in the terminal"
fileBrowse   = u""+"Select File…"
destbrowse   = u""+"Select Target Directory…"
actionName   = u""+"Convert the File(s)"
useFile      = u""+"Convert a File"
useFolder    = u""+"Convert a Folder"
dirBrowse    = u""+"Select Source Directory…"

pickFile    = True

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
        self.tabControl = ttk.Notebook(self.win)

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
        
        class getDirectoryButton(ttk.Button):
            def __init__(self, parent):
                self.initDirectory = path.dirname(__file__)
                self.dName = filedialog.askdirectory(parent=parent.win, initialdir=self.initDirectory)
                parent.config(state='enabled')
                parent.delete(0, tk.END)
                parent.insert(0, self.dName)

        def getDirectoryName(field):
            fDir = path.dirname(__file__)
            dName = filedialog.askdirectory(parent=self.win, initialdir=fDir)
            field.config(state='enabled')
            field.delete(0, tk.END)
            field.insert(0, dName)

        def convertFile():
            fileNames = self.fileEntry.get()
            directoryName = self.outDir.get()
            newConversion = usfmConverter(fileNames, directoryName)
            newConversion.usfm2html()
        
        def radCall():
            radSel=radVar.get()
            if   radSel == 0:
                self.cb.configure(text=dirBrowse, command=getDirectoryName)
            elif radSel == 1:
                self.cb.configure(text=fileBrowse, command=getFileName)


        tab1 = ttk.Frame(self.tabControl)
        self.tabControl.add(tab1, text=convert_tit)  # convert_tit text set from localization
        tab2 = ttk.Frame(self.tabControl)
        self.tabControl.add(tab2, text = prefs)      # prefs text set from localization

        self.tabControl.pack(expand=1, fill="both")

        conversion = ttk.LabelFrame(tab1, text = convert_tit)
        conversion.grid(column=0, row=0, padx=8, pady=4)

        radVar = tk.IntVar()          # radio button
        radVar.set(0)
        rad1 = tk.Radiobutton(conversion, text=useFile, variable=radVar, value=0, command=radCall)              # 9
        rad1.grid(column=0, row=0, sticky=tk.W)  # 10
        rad2 = tk.Radiobutton(conversion, text=useFolder, variable=radVar, value=1, command=radCall)                             # 11
        rad2.grid(column=1, row=0, sticky=tk.W)  # 12


        fileLoc = tk.StringVar()
        if pickFile:
            cbCommand   = getFileName
            cbText      = fileBrowse
        else:
            cbCommand   = getDirectoryName
            cbText      = dirBrowse

        self.cb = ttk.Button(conversion, text=cbText, command=cbCommand)   # filebrowse text set from localization
        self.cb.grid(column=0, row=1, sticky=tk.E)
        
        self.fileEntry = ttk.Entry(conversion, textvariable=fileLoc)
        self.fileEntry.entryLen = 20
        self.fileEntry.grid(column=1, row=1, sticky=tk.W)

        targetDir = tk.StringVar()
        self.outDir = ttk.Entry(conversion, width=self.fileEntry.entryLen, textvariable=targetDir)
        self.outDir.grid(column=1, row=2, sticky=tk.W)

        self.tb = ttk.Button(conversion, text=destbrowse)
        self.tb.configure(command=getDirectoryName(self.outDir))   # filebrowse text set from localization
        self.tb.grid(column=0, row=2, sticky=tk.W)

        ab = ttk.Button(conversion, text=actionName, command=convertFile)
        ab.grid(column=0, row=3, sticky=tk.W)


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