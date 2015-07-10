#! /usr/bin/python2
#-*- coding: utf-8 -*-

import os
import logging

from PySide import QtGui

from ..custom_widgets import PinguinoTextEdit


########################################################################
class PinguinoLog(object):

    def __init__(self):

        self.main.plainTextEdit_log = PinguinoTextEdit(shell=False)
        self.main.plainTextEdit_log.setReadOnly(True)
        self.main.gridLayout_log.addWidget(self.main.plainTextEdit_log, 0, 0, 1, 1)


    #----------------------------------------------------------------------
    def enable_debugger(self):

        log = logging.getLogger()
        log.setLevel(logging.DEBUG)
        ch = logging.StreamHandler(self.main.plainTextEdit_log)
        formatter = logging.Formatter("[%(levelname)s] %(message)s")
        # formatter = logging.Formatter("[{levelname}] {message}")
        ch.setFormatter(formatter)
        log.addHandler(ch)


    #----------------------------------------------------------------------
    def write_log(self, data, prefix="[INFO]"):

        lines = ""
        if type(data) == type({}):
            line = key + ": " + kwargs[key]
            lines += line
        else:
            lines = data

        self.main.plainTextEdit_log.write(lines, prefix=prefix)
