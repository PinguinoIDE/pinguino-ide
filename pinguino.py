#! /usr/bin/python
#-*- coding: utf-8 -*-

import sys
import os
sys.path.append("python_modules")
sys.path.append(os.path.join("qtgui", "resources"))

from qtgui.ide import PinguinoIDE
from PySide.QtGui import QApplication

def main(args):
    app=QApplication(args)
    frame=PinguinoIDE()
    frame.show()
    sys.exit(app.exec_())

if __name__=='__main__':
    main(sys.argv)