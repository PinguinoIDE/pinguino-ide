#! /usr/bin/python2
#-*- coding: utf-8 -*-

import os
import codecs
from ..methods.decorators import Decorator

########################################################################
class Stdout(object):
    """"""

    #----------------------------------------------------------------------
    @Decorator.alert_tab("tab_stdout")
    def update_stdout(self):
        """"""
        file_ = os.path.join(os.getenv("PINGUINO_USER_PATH"), "source", "stdout")

        if os.path.exists(file_):
            stdout = codecs.open(file_, "r", "utf-8")
            content = stdout.readlines()
            stdout.close()
            self.main.plainTextEdit_stdout.insertPlainText("".join(content))

