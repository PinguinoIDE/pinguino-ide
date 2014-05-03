#! /usr/bin/python
#-*- coding: utf-8 -*-

from PySide import QtGui, QtCore

########################################################################
class Highlighter(QtGui.QSyntaxHighlighter):

    #----------------------------------------------------------------------
    def __init__(self, parent, START):
        super(Highlighter, self).__init__(parent)
        color = QtGui.QColor

        self.highlightingRules = []

        operators = QtGui.QTextCharFormat()
        operators.setFontWeight(QtGui.QFont.Bold)
        self.highlightingRules.append(("[()\[\]{}<>=\-\+\*\\%#!~&^,/]", operators))

        start_command = QtGui.QTextCharFormat()
        start_command.setForeground(color("#729fcf"))
        self.highlightingRules.append((START, start_command))

        sdcc_error_01 = QtGui.QTextCharFormat()
        sdcc_error_01.setForeground(color("#ef292a"))
        self.highlightingRules.append(("ERROR: .*", sdcc_error_01))

        #sdcc_error_02 = QtGui.QTextCharFormat()
        #sdcc_error_02.setForeground(color("#ef292a"))
        #self.highlightingRules.append(("\\b[\d]+: .*", sdcc_error_02))

        reserved = QtGui.QTextCharFormat()
        reserved.setForeground(color("#8ae234"))
        self.highlightingRules.append(("\\b(None|False|True|def|class)\\b", reserved))


    #----------------------------------------------------------------------
    def highlightBlock(self, text):
        for pattern, format_ in self.highlightingRules:
            expression = QtCore.QRegExp(pattern)
            index = expression.indexIn(text)
            while index >= 0:
                length = expression.matchedLength()
                self.setFormat(index, length, format_)
                index = expression.indexIn(text, index + length)
