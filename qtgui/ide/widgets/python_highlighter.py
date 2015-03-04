#! /usr/bin/python
#-*- coding: utf-8 -*-

#import os

from PySide import QtGui, QtCore

## Python3 compatibility
#if os.getenv("PINGUINO_PYTHON") is "3":
    ##Python3
    #import builtin
    #FUNCTIONS_PYTHON = "|".join(builtin.__dict__.keys())
#else:
    ##Python2
    #import __builtin__
    #FUNCTIONS_PYTHON = "|".join(__builtin__.__dict__.keys())

MESSAGES = [
    ("DEBUG", "#ffff00"),
    ("OUT", "#00ff00")
]


RESERVED_PYTHON = "def|class|for|while|pass|try|except|if|else|elif"

########################################################################
class Highlighter(QtGui.QSyntaxHighlighter):

    #----------------------------------------------------------------------
    def __init__(self, parent, extra):
        super(Highlighter, self).__init__(parent)
        color = QtGui.QColor

        self.highlightingRules = []

        operators = QtGui.QTextCharFormat()
        operators.setFontWeight(QtGui.QFont.Bold)
        self.highlightingRules.append(("[()\[\]{}<>=\-\+\*\\%#!~&^,/]", operators))

        reserved = QtGui.QTextCharFormat()
        reserved.setForeground(color("#8ae234"))
        #self.highlightingRules.append(("\\b(None|False|True|def|class|for|while|pass|try|except|print|if)\\b", reserved))
        self.highlightingRules.append(("\\b(%s)\\b" % RESERVED_PYTHON, reserved))

        #functions = QtGui.QTextCharFormat()
        #functions.setForeground(color("#aaffff"))
        #self.highlightingRules.append(("\\b(%s)\\b" % FUNCTIONS_PYTHON, functions))

        start_command = QtGui.QTextCharFormat()
        start_command.setForeground(color("#729fcf"))
        self.highlightingRules.append((extra[0].replace(".", "\."), start_command))

        #line_command = QtGui.QTextCharFormat()
        #line_command.setForeground(color("#ef292a"))
        #self.highlightingRules.append((extra[1].replace(".", "\."), line_command))

        sdcc_error_01 = QtGui.QTextCharFormat()
        sdcc_error_01.setForeground(color("#ef292a"))
        self.highlightingRules.append(("ERROR: .*", sdcc_error_01))

        #sdcc_error_02 = QtGui.QTextCharFormat()
        #sdcc_error_02.setForeground(color("#ef292a"))
        #self.highlightingRules.append(("\\b[\d]+: .*", sdcc_error_02))

        for msg, color_ in MESSAGES:
            debugger = QtGui.QTextCharFormat()
            debugger.setForeground(color(color_))
            self.highlightingRules.append(("\[%s\] .*"%msg, debugger))


    #----------------------------------------------------------------------
    def highlightBlock(self, text):
        for pattern, format_ in self.highlightingRules:
            expression = QtCore.QRegExp(pattern)
            index = expression.indexIn(text)
            while index >= 0:
                length = expression.matchedLength()
                self.setFormat(index, length, format_)
                index = expression.indexIn(text, index + length)
