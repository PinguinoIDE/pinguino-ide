#! /usr/bin/python
#-*- coding: utf-8 -*-

import sys
import os

sys.path.append(os.path.join("qtgui", "resources"))

python_path_modules = os.path.join("python_modules")
if os.path.isdir(python_path_modules): sys.path.append(python_path_modules)

import gitdb

from qtgui.ide import PinguinoIDE
from PySide.QtGui import QApplication

def main(args):
    app=QApplication(args)
    frame=PinguinoIDE()
    frame.show()
    app.exec_()

if __name__ == "__main__":
    if len(sys.argv) == 1:
        main(sys.argv)
    
    else:  #command line
        print("\nError:\n\tFor command line use ./pinguino/pinguino.py instead of pinguino.py\n")

    