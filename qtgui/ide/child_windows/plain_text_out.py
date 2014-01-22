#!/usr/bin/env python
#-*- coding: utf-8 -*-

import codecs
import os

from PySide import QtGui, QtCore

from ..helpers.constants import TAB_NAME
#from ..helpers.constants import PINGUINO_STDOUT_FILE
from ...frames.stdout import Ui_Stdout
from ..code_editor.syntaxhighlighter import Highlighter

########################################################################
class PlainOut(QtGui.QMainWindow):
    """"""
    def __init__(self, title):
        #QtGui.QMainWindow.__init__(self)
        super(PlainOut, self).__init__()
        self.setWindowFlags(QtCore.Qt.WindowCloseButtonHint |
                            QtCore.Qt.WindowSystemMenuHint |
                            QtCore.Qt.WindowStaysOnTopHint)        
        
    
        self.plain_out = Ui_Stdout()
        self.plain_out.setupUi(self)
        
        font = self.plain_out.textEdit.font()
        font.setFamily("mono")
        font.setPointSize(font.pointSize()-1)
        self.plain_out.textEdit.setFont(font)
        
        self.setWindowTitle(TAB_NAME+" - "+title)
        
        self.connect(self.plain_out.buttonBox, QtCore.SIGNAL("clicked(QAbstractButton*)"), self.getButton)
        
        palette = QtGui.QPalette(self.palette())
        self.setAutoFillBackground(True)
        palette.setColor(QtGui.QPalette.Base, QtGui.QColor("#FFFFFF"))
        self.setPalette(palette)
        
        PINGUINO_STDOUT_FILE = os.path.join(os.environ.get("PINGUINO_USER_PATH"), "source", "stdout")

        if os.path.exists(PINGUINO_STDOUT_FILE):
            stdout = codecs.open(PINGUINO_STDOUT_FILE, "r", "utf-8")
            content = stdout.readlines()
            stdout.close()
            self.show_text("".join(content))
            
        self.plain_out.buttonBox.setFocus()
        self.centrar()
        
        
    #----------------------------------------------------------------------
    def getButton(self, button):
        if self.plain_out.buttonBox.standardButton(button) == self.plain_out.buttonBox.Close: self.close()
        #elif  self.ventana.buttonBox.standardButton(button) == self.ventana.buttonBox.Cancel: self.close()
                    

    #----------------------------------------------------------------------
    def centrar(self):
        screen = QtGui.QDesktopWidget().screenGeometry()
        size =  self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)
        
    #----------------------------------------------------------------------
    def show_text(self, text, pde=False):
        """"""
        if pde: Highlighter(self.plain_out.textEdit)
        self.plain_out.textEdit.setPlainText(text)