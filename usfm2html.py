#!/usr/bin/env python3
#usfm2html.py
#version 0.6

# by John Wood -- for Tech Advance
# This script reads a USFM 2 file and outputs a "pretty"
# HTML version of the file.
# It is intended that the HTML will then be further tweaked
# in LibreOffice or something similar, but of course you're
# free to do as you please.
#
# It will not check for a HTML file before writing to it.
# I will decide later if that is a bug or a feature.
#
# At this point it will only work on a file. Later it may work
# on a folder/directory
# It also does not work with wildcards for multiple files (i.e. *.usfm)

# Change in v. 0.7
#   Added a simple graphical user interface that allows the user to pick a USFM file to convert.

# Change in v. 0.6
#   Changed /s5 to print a horizontal line. Also changed paragraph start/stop because of this.

# Change in v. 0.5:
#   Added code to ignore the Strong's numbers, etc., in USFM 3 files

# Change in v. 0.4:
#   Removed <!--NewPage--> comment because it caused problems in LibreOffice, and didn't seem to work as intended.

# Changes in v. 0.3:
#   Correctly deals with UTF-8 files that otherwise may not display correctly.

# Changes in v. 0.2:
#   Adding/changing code to deal with Bahasa USFM files

# Usage: python3 usfm2html.py <path to USFM file>
# Usage with GUI: python3 usfm2html.py

# The output of this command can be run through html2ps and then ps2pdf in order to produce pdf output.
# Please note that any errors in the USFM will likely produce formatting errors in the HTML, and thus in the PDF.

#Import necessary python components

import os	# file system commands
import os.path # commands to handle paths
import re	# regular expressions
import sys	# command line arguments
import shutil	# high level file operations
from MainGUI import *


def convert_to_html(convert_file):
    book_name="Unknown" # Script was failing with 'book_name unknown' errors. This initializes the variable before it can be needed.

    if not re.search(r'.u?sfm',convert_file, flags=re.IGNORECASE):
        print("Not a USFM file as far as I can tell.")
        sys.exit(1)

    # Extract the file name from the path
    file_name = os.path.basename(convert_file) 

    # Generate taket_file name
    target_file=re.sub(r'.u?sfm','.html', file_name, flags=re.IGNORECASE)

    # Add target_file to the same path as convert_file 
    target_file = os.path.dirname(convert_file) + os.sep + target_file

    print("Converting "+convert_file+" to "+target_file+"\n")

    with open(target_file, "w+") as newfile:
        newfile.write('\n'.join([
        '<!DOCTYPE html>',
        '<html >',
        '    <head>',
        '        <meta charset="UTF-8">',
        '        <title></title>',
        '       <style type="text/css">',
        '           .verse_number {color:darkgrey}',
        '           h1, h2, h3, .section {text-align: center}',
        '           .footnote {color:grey;}',
        '           .chapter {page-break-before: always}',
        '           .chapitre {font-style: italic}',
        '           .book_title {text-decoration:underline}',
        '       </style>',
        '    </head>',
        '<body>']))

    with open(convert_file) as input_file: #open the usfm file and scan it
        for line in input_file:
            line=line.rstrip()
            if re.match(r'\\toc2',line):
                line=None
            elif re.match(r'\\toc3',line):
                line=None
            elif re.match(r'\\id(e?)',line):
                line=None
            elif re.match(r'\\rem',line):
                line=None
            elif re.match(r'\\b',line):
                line=None
            elif re.match(r'\\mt',line):
                line=None #strip out tags we don't want/need
            #elif re.match(r'^\\',line):
            else:
                book=re.match(r'\\h (.+)',line)
                if book:
                    book_name=book.group(1)
                if '\\' not in line:
                    line=re.sub(r'(.+)','<h3 class="chapitre">\\1</h3>',line) # print lines that have no tags as though they are chapters
                line=re.sub(r'\\v (\d+?) ','<span class="verse_number"><sup>\\1 </sup></span>',line) # change verse marking
                line=re.sub(r'\\toc1 (.+)','<h2 class="fancy_name">\\1</h2>',line) #set fancy name of book
                line=re.sub(r'\\h (.+)','<h1 class="book_name">\\1</h1>',line) #set the book name
                line=re.sub(r'\\c (\d+)','<pagebreak/><?page-break><h3 class="chapter">'+book_name+' Chapter \\1</h3>',line) # change chapter marking
                line=re.sub(r'\\s (.+)','<h4 class="section">\\1</h4>',line) # section headings from .sfm files
                line=re.sub(r'\\q1','<br />&nbsp;',line) #quote level 1
                line=re.sub(r'\\q2','<br />&nbsp;&nbsp;&nbsp;',line) #quote level 2
                line=re.sub(r'\\q','<br />',line) #change quote tags to line breaks
                line=re.sub(r'\\p|\\m','</p><p>',line) #close paragraph marks
                line=re.sub(r'\\s5','<hr>',line) #chunk marks paragraphs
                line=re.sub(r'\\f\*','</sup></span>',line) #close footnotes
                line=re.sub(r'\\f \+ \\ft','<span class="footnote"><sup>',line) #start footnotes
                line=re.sub(r'\\fqa\*','</em>',line) #close quote in footnote
                line=re.sub(r'\\fqa','<em>',line) #open quote in footnote
                line=re.sub(r'\\f \+ \\fr','<span class="footnote"><sup><strong>',line) #open reference in footnote
                line=re.sub(r'\\fk','</strong><em>',line) #close reference in footnote
                line=re.sub(r'\\ft','</strong></em>',line) # yeah, something to do with footnotes
                line=re.sub(r'\\li','<li>',line) #list items
                line=re.sub(r'\\bk\*','</span>',line) #inline book titles close
                line=re.sub(r'\\bk','<span class="book_title">',line) #inline book titles open
                line=re.sub(r'\\w(.*?)(\|.+?\\w\*)','\\1',line) # remove Strongs numbers, etc.
            if line!=None:
                with open(target_file, "a+") as newfile:
                        newfile.write(line+"\n")

    with open(target_file, "a+") as newfile:
        newfile.write("</body></html>")
    newfile.close()

    return target_file


arguments=sys.argv[1:]
count_args=len(arguments)
print("usfm2html: Making your output prettier since 2018\n")
if count_args!=1: #If there is not exactly one argument, fail with a usage remark.
    if count_args == 0:
        print ("usfm2html.py script to convert USFM 2 scripture to pretty HTML")
        print("Usage: python3 usfm2html.py <path to USFM file>")

        gui = MainGUI(convert_to_html)
        gui.start()

    elif count_args > 1:
        print ("usfm2html.py currently only handles one file at a time")
        print ("If you are using a bash shell, try")
        print ("    for n in $(ls *.usfm); do usfm2html $n; done")
    sys.exit(1)
else:
    convert_to_html(sys.argv[1])