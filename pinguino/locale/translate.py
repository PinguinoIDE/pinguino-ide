#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""-------------------------------------------------------------------------
    Pinguino Translation Script

    author:			Alexis Sánchez
    contact:		aasanchez@gmail.com
    first release:	2012-05-23

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
-------------------------------------------------------------------------"""


import os
import glob
import shutil

print "\033[1;32mClean source folder:\033[0m"
files = glob.glob('sources/*')
for f in files:
    os.remove(f)
print "Done!!"
print "\033[1;32mCopy files to translate\033[0m"
src = os.getcwd()
shutil.copy2(os.path.join(src.rstrip('locale'), 'wxgui/pinguino.py'),
                                                'sources/pinguino.py')
shutil.copy2(os.path.join(src.rstrip('locale'), 'wxgui/editor/editeur.py'),
                                                'sources/editeur.py')
shutil.copy2(os.path.join(src.rstrip('locale'), 'wxgui/editor/general.py'),
                                                'sources/general.py')
shutil.copy2(os.path.join(src.rstrip('locale'), 'wxgui/editor/lateral_tool_area/dock_files.py'),
                                                'sources/dock_files.py')
shutil.copy2(os.path.join(src.rstrip('locale'), 'wxgui/editor/frames/framesX4.py'),
                                                'sources/framesX4.py')
print "Done!!"
print "\033[1;32mSelect the language to translate\033[0m"
print "1.- Deutsch"
print "2.- English"
print "3.- Spanish"
print "4.- Français"
print "5.- Italian"
print "6.- Portugues"
print "7.- Portuguese-Brazilian"

n = input('Input the number pls:')
if n == 1:
    os.popen("poedit de/LC_MESSAGES/pinguino.po")
elif n == 2:
    os.popen("poedit en/LC_MESSAGES/pinguino.po")
elif n == 3:
    os.popen("poedit es/LC_MESSAGES/pinguino.po")
elif n == 4:
    os.popen("poedit fr/LC_MESSAGES/pinguino.po")
elif n == 5:
    os.popen("poedit it/LC_MESSAGES/pinguino.po")
elif n == 6:
    os.popen("poedit pt/LC_MESSAGES/pinguino.po")
elif n == 7:
    os.popen("poedit pt_BR/LC_MESSAGES/pinguino.po")
