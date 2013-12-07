#!/usr/bin/env python
#-*- coding: utf-8 -*-


from ...frames.about import Ui_About

from ..helpers.constants import TAB_NAME

from PySide import QtGui, QtCore

########################################################################
class About(QtGui.QMainWindow):
    """"""
    def __init__(self, IDE):
        super(About, self).__init__()
        self.setWindowFlags(QtCore.Qt.WindowCloseButtonHint |
                            QtCore.Qt.WindowSystemMenuHint |
                            QtCore.Qt.WindowStaysOnTopHint)        
        
    
        self.about = Ui_About()
        self.about.setupUi(self)
        
        self.setWindowTitle(TAB_NAME+" - About")
        
        
        self.connect(self.about.pushButton_credits, QtCore.SIGNAL("clicked()"), lambda :self.about.stackedWidget.setCurrentIndex(1))
        self.connect(self.about.pushButton_license, QtCore.SIGNAL("clicked()"), lambda :self.about.stackedWidget.setCurrentIndex(2))
        self.connect(self.about.pushButton_about, QtCore.SIGNAL("clicked()"), lambda :self.about.stackedWidget.setCurrentIndex(0))
        self.connect(self.about.pushButton_close, QtCore.SIGNAL("clicked()"), self.close)
        self.connect(self.about.pushButton_close_2, QtCore.SIGNAL("clicked()"), self.close)
        self.connect(self.about.pushButton_close_3, QtCore.SIGNAL("clicked()"), self.close)
        
        
    
        
        self.centrar()

    #----------------------------------------------------------------------
    def centrar(self):
        screen = QtGui.QDesktopWidget().screenGeometry()
        size =  self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)
        