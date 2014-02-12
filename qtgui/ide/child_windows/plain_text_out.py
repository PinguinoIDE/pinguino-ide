#!/usr/bin/env python
#-*- coding: utf-8 -*-

import codecs
import os

from PySide import QtGui, QtCore

#from ..methods.constants import os.getenv("NAME")
#from ..methods.constants import PINGUINO_STDOUT_FILE
from ...frames.stdout import Ui_PlainOut
from ..code_editor.syntaxhighlighter import Highlighter

########################################################################
class PlainOut(QtGui.QDialog):
    
    def __init__(self, title):
        super(PlainOut, self).__init__()     
        
    
        self.plain_out = Ui_PlainOut()
        self.plain_out.setupUi(self)
        
        font = self.plain_out.textEdit.font()
        font.setFamily("mono")
        font.setPointSize(font.pointSize()-1)
        self.plain_out.textEdit.setFont(font)
        
        self.setWindowTitle(os.getenv("NAME")+" - "+title)
        
        self.connect(self.plain_out.pushButton_close, QtCore.SIGNAL("clicked()"), self.close)
        
        palette = QtGui.QPalette(self.palette())
        self.setAutoFillBackground(True)
        palette.setColor(QtGui.QPalette.Base, QtGui.QColor("#FFFFFF"))
        self.setPalette(palette)
        
        PINGUINO_STDOUT_FILE = os.path.join(os.getenv("PINGUINO_USER_PATH"), "source", "stdout")

        if os.path.exists(PINGUINO_STDOUT_FILE):
            stdout = codecs.open(PINGUINO_STDOUT_FILE, "r", "utf-8")
            content = stdout.readlines()
            stdout.close()
            self.show_text("".join(content))
            
        self.plain_out.pushButton_close.setFocus()
        
        
    #----------------------------------------------------------------------
    def show_text(self, text, pde=False):
        
        if pde: Highlighter(self.plain_out.textEdit)
        self.plain_out.textEdit.setPlainText(text)
        