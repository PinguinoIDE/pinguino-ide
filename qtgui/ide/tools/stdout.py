#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import codecs
from datetime import datetime

from PySide import QtGui

from ..methods.decorators import Decorator

########################################################################
class Stdout(object):
    """"""

    #----------------------------------------------------------------------
    @Decorator.alert_tab("Stdout")
    def update_stdout(self):
        """"""
        file_ = os.path.join(os.getenv("PINGUINO_USER_PATH"), "source", "stdout")

        self.main.plainTextEdit_stdout.moveCursor(QtGui.QTextCursor.End)

        intro = "#" * 70 + "\n" + str(datetime.now()) + "\n" + "#" * 70
        self.main.plainTextEdit_stdout.insertPlainText(intro)

        if os.path.exists(file_):
            stdout = codecs.open(file_, "r", "utf-8")
            content = stdout.readlines()
            stdout.close()
            self.main.plainTextEdit_stdout.insertPlainText("".join(content))
            self.main.plainTextEdit_stdout.moveCursor(QtGui.QTextCursor.EndOfLine)

        self.main.plainTextEdit_stdout.moveCursor(QtGui.QTextCursor.End)
        self.main.plainTextEdit_stdout.insertPlainText("\n"*4)




