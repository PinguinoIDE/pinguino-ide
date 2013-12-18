#!/usr/bin/env python
#-*- coding: utf-8 -*-

import sys

from PySide import QtCore, QtGui

from ..helpers.python_shell import PythonShell

HEAD = "Welcome to Pinguino 11.0\n" + sys.version + " on " + sys.platform

START = ">>> "


########################################################################
class PinguinoTerminal(QtGui.QPlainTextEdit):
    """"""
    #----------------------------------------------------------------------
    def __init__(self, *args, **kwargs):
        super(PinguinoTerminal, self).__init__(*args, **kwargs)
        
        self.setStyleSheet("background-color: rgb(0, 0, 0);"\
                           "color: rgb(255, 255, 255);"\
                           "font-family: mono;"\
                           "font-size: 12px;")        
        
        self.appendPlainText(HEAD)
        self.appendPlainText(START)
        
        self.shell = PythonShell()
        
        

    #----------------------------------------------------------------------
    def keyPressEvent(self, event):
        """"""
        self.set_last_line()
        
        if event.key() in (QtCore.Qt.Key_Enter, QtCore.Qt.Key_Enter-1):
            super(PinguinoTerminal, self).keyPressEvent(event)
            command = self.get_command()
            if not command.isspace():
                self.moveCursor(QtGui.QTextCursor.End)
                self.insertPlainText(self.shell.run(command))
            self.insertPlainText(START)
            self.moveCursor(QtGui.QTextCursor.End)
    
        elif event.key() == QtCore.Qt.Key_Backspace:
            if not self.get_command(): return
            else: super(PinguinoTerminal, self).keyPressEvent(event)
            
        
        else:
            super(PinguinoTerminal, self).keyPressEvent(event)
            
            
    #----------------------------------------------------------------------
    def get_command(self):
        """"""
        plain = self.toPlainText()
        comand = plain[plain.rfind(START):]
        return comand[len(START):]
        

    #----------------------------------------------------------------------
    def wheelEvent(self, event):
        """"""
        if event.modifiers() == QtCore.Qt.ControlModifier:
            self.step_font_size(event.delta())
            
        else: super(PinguinoTerminal, self).wheelEvent(event)
                
                
    #----------------------------------------------------------------------
    def step_font_size(self, delta):
        """"""
        font = self.font()
        size = font.pointSize()
        if delta > 0: font.setPointSize(size+1)
        else: font.setPointSize(size-1)
        self.setFont(font)
                          
                          
    #----------------------------------------------------------------------
    def set_last_line(self):
        """"""
        cursor = self.textCursor()
        position = cursor.position()
        plain = self.toPlainText()
        index = plain.rfind("\n")
        if position < index: self.moveCursor(QtGui.QTextCursor.End)