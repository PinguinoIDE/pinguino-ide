#!/usr/bin/env python
#  -*- coding: UTF-8 -*-

"""-------------------------------------------------------------------------
    pdl2wiki
    8 Mar 2013 - Régis Blanchot <rblanbchot@gmail.com>
    argument : name of the library (sd for sd.pdl)
    output : wiki format
    copy output at the end of whatever function description
    -------------------------------------------------------------------------"""

import os
import sys

if sys.argv[1] == "":
    for fichier in os.listdir(os.getcwd()):
        filename,extension=os.path.splitext(fichier)
        if extension == '.pdl':
            # check content of the PDL file
            libfile=open(fichier,'r')
            for line in libfile:
                if line!="\n":
                    print "[[Category:LanguageReference]] [[Category:Functions]] [[Category:", filename, "library]]"
                    print "== Name =="
                    print line[0:line.find(" ")]
                    print "== Description =="
                    print "== Syntax =="
                    print "== Example =="
                    print "== Known bugs =="
                    print "== See also =="
                    print "== Comments =="
            libfile.close()
else:
    filename = sys.argv[1]
    print filename
    fichier = filename + ".pdl"
    # check content of the PDL file
    libfile=open(fichier,'r')
    for line in libfile:
        if line!="\n":
            print "[[Category:LanguageReference]] [[Category:Functions]] [[Category:", filename, "library]]"
            print "== Name =="
            print line[0:line.find(" ")]
            print "== Description =="
            print "== Syntax =="
            print "== Example =="
            print "== Known bugs =="
            print "== See also =="
            print "== Comments =="
    libfile.close()
