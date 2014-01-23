#!/usr/bin/env python
#-*- coding: utf-8 -*-

from PySide import QtGui, QtCore

########################################################################
class PrettyFeatures(object):
    

        
        
    #----------------------------------------------------------------------
    @classmethod
    def LineEdit_default_text(self, parent, lineEdit, default_text):
        
        
        def set_focus_out_color():
            lineEdit.setStyleSheet("color: #aaa;")    
            
        def set_focus_in_color():
            lineEdit.setStyleSheet("")     
        
        def focus_in(event):
            if lineEdit.text() == default_text:
                lineEdit.setText("")
                set_focus_in_color()
            QtGui.QLineEdit.focusInEvent(lineEdit, QtGui.QFocusEvent(QtCore.QEvent.FocusIn))
            
        def focus_out(event):
            if lineEdit.text() == "":
                lineEdit.setText(default_text)
                set_focus_out_color()
            QtGui.QLineEdit.focusInEvent(lineEdit, QtGui.QFocusEvent(QtCore.QEvent.FocusOut))
            
        lineEdit.focusInEvent = focus_in      
        lineEdit.focusOutEvent = focus_out     
        
        lineEdit.setText(default_text) 
        set_focus_out_color()
    
    