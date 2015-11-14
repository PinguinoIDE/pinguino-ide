#! /usr/bin/python
#-*- coding: utf-8 -*-

from PySide import QtGui, QtCore

MESSAGES = [
    ("CRITICAL", "#FF0000"),
    ("DEBUG", "#00FF00"),
    ("ERROR", "#FF0000"),
    ("INFO", "#1E90FF"),
    ("WARNING", "#FFA500"),
]

exceptions = [
    "GeneratorExit",
    "ImportError",
    "ReferenceError",
    "RuntimeError",
    "UnicodeTranslateError",
    "KeyError",
    "StopIteration",
    "PendingDeprecationWarning",
    "EnvironmentError",
    "LookupError",
    "BufferError",
    "OSError",
    "DeprecationWarning",
    "UnicodeError",
    "UnicodeEncodeError",
    "FloatingPointError",
    "UnicodeWarning",
    "NameError",
    "BytesWarning",
    "IOError",
    "SyntaxError",
    "FutureWarning",
    "ImportWarning",
    "SystemExit",
    "Exception",
    "EOFError",
    "StandardError",
    "ValueError",
    "TabError",
    "ZeroDivisionError",
    "SystemError",
    "IndentationError",
    "AssertionError",
    "UnicodeDecodeError",
    "TypeError",
    "IndexError",
    "RuntimeWarning",
    "KeyboardInterrupt",
    "UserWarning",
    "SyntaxWarning",
    "MemoryError",
    "UnboundLocalError",
    "ArithmeticError",
    "Warning",
    "NotImplementedError",
    "AttributeError",
    "OverflowError",
    "BaseException",
]

EXCEPTIONS = [(f,"#FF0000") for f in exceptions]
RESERVED_PYTHON = "def|class|for|while|pass|try|except|if|else|elif"


########################################################################
class Highlighter(QtGui.QSyntaxHighlighter):

    #----------------------------------------------------------------------
    def __init__(self, parent, extra=[], python=True):
        super(Highlighter, self).__init__(parent)
        color = QtGui.QColor

        self.highlightingRules = []

        if python:
            operators = QtGui.QTextCharFormat()
            operators.setFontWeight(QtGui.QFont.Bold)
            self.highlightingRules.append(("[()\[\]{}<>=\-\+\*\\%#!~&^,/]", operators))

            reserved = QtGui.QTextCharFormat()
            reserved.setForeground(color("#8ae234"))
            #self.highlightingRules.append(("\\b(None|False|True|def|class|for|while|pass|try|except|print|if)\\b", reserved))
            self.highlightingRules.append(("\\b({})\\b".format(RESERVED_PYTHON), reserved))

        if extra:
            start_command = QtGui.QTextCharFormat()
            start_command.setForeground(color("#729fcf"))
            self.highlightingRules.append((extra[0].replace(".", "\."), start_command))

        sdcc_error_01 = QtGui.QTextCharFormat()
        sdcc_error_01.setForeground(color("#ef292a"))
        self.highlightingRules.append(("ERROR: .*", sdcc_error_01))

        for msg, color_ in MESSAGES:
            debugger = QtGui.QTextCharFormat()
            debugger.setForeground(color(color_))
            self.highlightingRules.append(("\[{}\] .*".format(msg), debugger))

        for msg, color_ in EXCEPTIONS:
            debugger = QtGui.QTextCharFormat()
            debugger.setForeground(color(color_))
            self.highlightingRules.append(("\\b({})\\b".format(msg), debugger))


    #----------------------------------------------------------------------
    def highlightBlock(self, text):
        for pattern, format_ in self.highlightingRules:
            expression = QtCore.QRegExp(pattern)
            index = expression.indexIn(text)
            while index >= 0:
                length = expression.matchedLength()
                self.setFormat(index, length, format_)
                index = expression.indexIn(text, index + length)
