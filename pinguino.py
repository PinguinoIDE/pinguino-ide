#! /usr/bin/python
#-*- coding: utf-8 -*-

import sys
import os
sys.path.append(os.path.join("qtgui", "resources"))
sys.path.append(os.path.join("python_modules"))

from qtgui.ide import PinguinoIDE
from PySide.QtGui import QApplication

def main(args):
    app=QApplication(args)
    frame=PinguinoIDE()
    frame.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    if len(sys.argv) == 1:
        main(sys.argv)
    
    else:  #command line
        print("\nError:\n\tFor command line use ./pinguino/pinguino.py instead of pinguino.py\n")

    