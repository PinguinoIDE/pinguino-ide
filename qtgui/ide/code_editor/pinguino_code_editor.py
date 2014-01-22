#!/usr/bin/env python
#-*- coding: utf-8 -*-

from PySide.QtGui import QWidget
from PySide import QtCore, QtGui

from ..code_editor.line_number import LineNumber
from ..code_editor.editor import CustomTextEdit

########################################################################
class PinguinoCodeEditor(QWidget):
    

    #----------------------------------------------------------------------
    def __init__(self, *args, **kwargs):
        super(PinguinoCodeEditor, self).__init__(*args, **kwargs)
        
        self.gridLayout_0 = QtGui.QGridLayout(self)
        self.gridLayout_0.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_0.setSpacing(0)
        
        self.line_number = LineNumber(self)
        self.line_number.setMinimumSize(QtCore.QSize(51, 0))
        self.line_number.setMaximumSize(QtCore.QSize(51, 16777215))
        self.gridLayout_0.addWidget(self.line_number, 0, 0, 1, 1)
        
        self.text_edit = CustomTextEdit(self)
        self.text_edit.setFrameShape(QtGui.QFrame.NoFrame)
        self.gridLayout_0.addWidget(self.text_edit, 0, 3, 1, 1)
        
        self.widget = QtGui.QWidget(self)
        self.widget.setMinimumSize(QtCore.QSize(10, 0))
        self.gridLayout_0.addWidget(self.widget, 0, 2, 1, 1)
        

        palette = QtGui.QPalette(self.widget.palette())
        self.widget.setAutoFillBackground(True)
        palette.setColor(QtGui.QPalette.Window, QtGui.QColor("#E7E7E7"))
        self.widget.setPalette(palette)   
        
        palette = QtGui.QPalette(self.palette())
        self.setAutoFillBackground(True)
        palette.setColor(QtGui.QPalette.Base, QtGui.QColor("#FFFFFF"))
        self.setPalette(palette)        
        
        self.line_number.setTextEdit(self.text_edit)    
        self.text_edit.viewport().installEventFilter(self)
        self.initialize()
        
        self.tool_bar_state = {"undo": True,
                               "redo": False,
                               #"cut": False,
                               "copy": False,
                               }
        
        

        
    #----------------------------------------------------------------------
    def initialize(self):
        
        self.set_font_size(12)
        self.set_font_family("Ubuntu Mono")
        
        
    #----------------------------------------------------------------------
    def eventFilter(self, object, event):
        self.line_number.update()
        return False
    
    
    #----------------------------------------------------------------------
    def set_font_size(self, font_size):
        font = self.text_edit.font()
        font.setPointSize(font_size)
        self.text_edit.setFont(font)
        
    #----------------------------------------------------------------------
    def set_font_family(self, font_family, fixed_pitch=True):
        font = self.text_edit.font()
        font.setFamily(font_family)
        font.setFixedPitch = fixed_pitch
        self.text_edit.setFont(font)
        
        
  