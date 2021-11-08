#!/usr/bin/env python3
'''
USFM to HTML is a script designed to make it easier to print Scripture from USFM files.
The suite of tags supported is currently limited because the tool is developed by Wycliffe
Associates, and the number of tags our tools use is limited.

Developed by John Wood and released for general use.
'''
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

import numpy as np
import pandas as pd

import os	# file system commands
import os.path # commands to handle paths
import re	# regular expressions
import sys	# command line arguments
import shutil	# high level file operations
from MainGUI import *

def convert_to_html(convert_file):

    """
    Convert USFM text (read from a file) to HTML. Use styles so that we can later
    touch this up in LibreOffice without too much trouble.

    We include a link to a default stylesheet: bible_styles.css, so that we can
    easily override the internal styles by dropping a .css file in to the same
    directory as our html.

    Parameters
    ----------
    This should be fed one USFM file at a time.

    This function writes to a file.
    """

    # Variable initialization
    localized.book_name="Unknown" # Script was failing with 'book_name unknown' errors. This initializes the variable before it can be needed.
    localized.chapter = "Chapter"
    footnoteNum = 1

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
        '       <link href="bible_styles.css" rel="stylesheet">',
        '    </head>',
        '    <body>']))

    with open(convert_file) as input_file: #open the usfm file and scan it
        # The tags we can expect in a USFM file are:
        #   \id  : Book ID code
        #   \ide : Character encoding specification. If this is not UTF-8, we're probably in trouble.
        #   \h   : Running header text. *Deprecated* use of numbered variable syntax.
        #            Formerly \h# text was used for different lines of the running header
        #   \toc1: Long Table of Contents text. At this point we don't use a different name in the ToC
        #   \toc2: Similar. Seldom any different than \toc1, although it could be shorter (if,
        #            for example, \toc1 used "The Book of ..." in its title)
        #   \toc3: Very unlikely for us to need this, as it's a very short form of the book name.
        #   \mt  : Major title. This is probably what we'll use for the book title, unless we use \h
        #            This can be combined with numbers to make a grand, multi-line title
        #   \s5  : Section heading #5. Actually, this is used for chunk division. As such, in
        #            English it can make a decent paragraph break. Unfortunately, in other languages,
        #            this just goes where it was in the source. The other \s# tags are used for section
        #            headings.
        #   \c   : Followed by a digit, this marks a chapter.
        #   \cl  : Followed by a word, becomes the word for "chapter". Should be used *after* the
        #            chapter number (\c #) unless it changes the whole book (i.e. \cl Psalm) in which
        #            case it should go before the first chapter mark (\c 1).
        #   \v   : Verse marker. There is another verse tag, \vp ... \vp* that indicates a non-Western
        #            number for the verse number.
        #   \p   : Paragraph marker. Like <p> in HTML
        #   \f   : Beginning of a footnote. This is more complicated than the tags we've seen so far.
        #            Footnotes are supposed to be bracketed by /f and /f*. However, there is also supposed
        #            to be a + indicating that "the caller should be generated automatically by the editor
        #            or publishing tools". The alternative is a -, which "indicates that no caller should
        #            be generated". So, I think that means "/f -" indicates a footnote for the translator/
        #            editor's use, only, and not for others to view. \fq is used to mark a quotation in the
        #            footnote. In general, this is so the quotation can be italicized, or emphasized in some
        #            way. An additional mark, \fqa, can be used for marking an alternate text, which is
        #            generally displayed in the same form as a quotation, but some software might want to know
        #            that it's a different sort of thing. We do sometimes use \ft (usually as \f + \ft) to show the
        #            beginning of the actual note text.
        #
        #            We don't use the \fr (footnote reference), \fk (footnote refers to this key word), \fl (footnote label
        #            text), \fw (footnote witness list), \fp (paragraph break in the footnote), \fv ... \fv* (mark for a verse
        #            number in the footnote (so that it can be superscripted))
        #   \q   : Quotation. \q1, \q2, \q3, etc., indicate increasing indentation.
        #   \m   : Continuation (margin) paragraph. This is where a paragraph continues after a quote, and shouldn't be indented.

        for line in input_file:
            line=line.rstrip()
            # strip out tags we don't want/need
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
                line=None
            #strip out tags we don't want/need
            #elif re.match(r'^\\',line):
            else:
                book=re.match(r'\\h (.+)',line) # we use the \h tag for the book name
                if book:
                    book_name=book.group(1)

                #if '\\' not in line:
                #    line=re.sub(r'(.+)','<h3 class="chapitre">\\1</h3>',line) # print lines that have no tags as just the content of the line
                line=re.sub(r'\\v (\d+?) ','<span class="verse_number"><sup>\\1 </sup></span>',line)                            # change verse marking
                line=re.sub(r'\\toc1 (.+)','<h2 class="fancy_name">\\1</h2>',line)                                              # set fancy name of book
                line=re.sub(r'\\h (.+)','<h1 class="book_name">\\1</h1>',line)                                                  # set the book name
                line=re.sub(r'\\c (\d+)','<pagebreak/><?page-break><h3 class="chapter">'+book_name+' Chapter \\1</h3>',line)    # change chapter marking
                line=re.sub(r'\\s (.+)','<h4 class="section">\\1</h4>',line)                                                    # section headings from .sfm files
                line=re.sub(r'\\q1','<p class="quote1">\\1</p>',line)                                                           # quote level 1
                line=re.sub(r'\\q2','<p class="quote2">\\1</p>',line)                                                           # quote level 2
                line=re.sub(r'\\q','<p class="quote1">\\1</p>',line)                                                            # quote without # is Quote 1
                line=re.sub(r'\\p|\\m','</p><p>',line)                                                                          # close paragraph marks
                # line=re.sub(r'\\s5','<hr>',line)  # We had a request to print 'by chunk'. This line can be uncommented for that, or we can implement a pref

                line=re.sub(r'\\f\*','</span>',line)                                                                      #close footnotes
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
