from tkinter import *
import tkinter as tk
from tkinter import ttk
from tkinter.filedialog import askopenfilename
from tkinter.filedialog import askdirectory
from functools import partial
from tkinter import messagebox
import os.path 
import webbrowser

class MainGUI:

    def __init__(self, convert_to_html):
        # Save a reference to the convertion function
        self.convert_to_html = convert_to_html

        self.root = Tk()
        self.root.title("USFM 2 HTML")
        self.config_widgets()

    # Display the GUI 
    def start(self):
        self.root.mainloop()

    def config_widgets(self):
        pass
        # Configure the main frame that contains everything in the window
        main_frame = ttk.Frame(self.root, padding="12 12 12 12")
        main_frame.grid(column=0, row=0, sticky=(N, W, E, S))
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)

        # Configure hint with instruction message
        hint_lbl = ttk.Label(main_frame, text="Select a file you would like to convert")
        hint_lbl.grid(column=1, row=1, pady=(0, 10), columnspan=2, sticky=(N, W, E, S))

        # Configure file pick button
        pick_file_btn = ttk.Button(main_frame, text="Choose File", command=self.pick_file)
        pick_file_btn.grid(column=1, row=2, padx=(0, 10), sticky=W)
        pick_file_btn.focus()

        # Configure convert button
        convert_btn = ttk.Button(main_frame, text="Convert", command=self.convert)
        convert_btn.grid(column=2, row=3, pady=(10, 0), sticky=E)

        # Configure the readonly entry field that displays chosen file/path
        self.__file_path = StringVar()
        file_path_entry = ttk.Entry(main_frame, width=70, textvariable=self.__file_path)
        file_path_entry.grid(column=2, row=2, sticky=(N, W, E, S))
        file_path_entry.config(state='readonly')

    # Display a file picker window and populate the input field with
    # the user's choice
    def pick_file(self):
        path = os.path.normpath(askopenfilename(filetypes = (("USFM","*.usfm"),("all files","*.*"))))
        self.__file_path.set(path)
    
    # Uses convert_to_html function to convert the file chosen, 
    # then resets the GUI. If no file has been chosen, do nothing.
    def convert(self):
        path = self.__file_path.get()

        target_file = ""
        if(path): #If a file has been chosen, then attempt convertion
            target_file = self.convert_to_html(path)
        
        if(target_file): # If the HTML is generated successfuly
            messagebox.showinfo("Success", "Your HTML has been generated at: \n" + target_file)
            self.reset()
        else:
            messagebox.showerror("Error", "Something went wrong and the file could not be generated")

    # Resets the GUI to its original state.
    def reset(self):
        self.__file_path.set("")