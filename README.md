# usfm2html
Creates "pretty" html/css output from USFM files

# Changes in v. 0.3:
   Correctly deals with UTF-8 files that otherwise may not display correctly.

# Changes in v. 0.2:
   Adding/changing code to deal with Bahasa USFM files

  Usage python3 usfm2html.py <path to USFM file>

 The output of this command can be run through html2ps and then ps2pdf in order to produce pdf output.
 Please note that any errors in the USFM will likely produce formatting errors in the HTML, and thus in the PDF.
