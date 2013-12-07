#!/usr/bin/env python
#-*- coding: utf-8 -*-


from ...frames.stdout import Ui_Stdout
from ..helpers.constants import TAB_NAME

from ..helpers.constants import PINGUINO_STDOUT_FILE

from PySide import QtGui, QtCore

########################################################################
class Stdout(QtGui.QMainWindow):
    """"""
    def __init__(self, parent):
        #QtGui.QMainWindow.__init__(self)
        super(Stdout, self).__init__()
        self.setWindowFlags(QtCore.Qt.WindowCloseButtonHint |
                            QtCore.Qt.WindowSystemMenuHint |
                            QtCore.Qt.WindowStaysOnTopHint)        
        
    
        self.stdout = Ui_Stdout()
        self.stdout.setupUi(self)
        self.main = parent
        
        self.setWindowTitle(TAB_NAME+" - Stdout")
        
        self.connect(self.stdout.buttonBox, QtCore.SIGNAL("clicked(QAbstractButton*)"), self.getButton)
        

        
        stdout = file(PINGUINO_STDOUT_FILE, "r")
        content = stdout.readlines()
        stdout.close()
        self.show_text("".join(content))
        
        self.stdout.buttonBox.setFocus()
        
        self.centrar()
        
        
    #----------------------------------------------------------------------
    def getButton(self, button):
        if self.stdout.buttonBox.standardButton(button) == self.stdout.buttonBox.Close: self.close()
        #elif  self.ventana.buttonBox.standardButton(button) == self.ventana.buttonBox.Cancel: self.close()
                    

    #----------------------------------------------------------------------
    def centrar(self):
        screen = QtGui.QDesktopWidget().screenGeometry()
        size =  self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)
        
    #----------------------------------------------------------------------
    def show_text(self, text):
        """"""
        self.stdout.plainTextEdit.setPlainText(text)
        
        