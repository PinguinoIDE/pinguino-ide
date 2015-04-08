#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

from PySide import QtGui, QtCore

from ...frames.patches import Ui_Patches

########################################################################
class Patches(QtGui.QDialog):

    def __init__(self, parent):
        super(Patches, self).__init__()
        self.setWindowFlags(QtCore.Qt.WindowCloseButtonHint |
                            QtCore.Qt.WindowSystemMenuHint |
                            QtCore.Qt.WindowStaysOnTopHint |
                            QtCore.Qt.Tool)

        self.patches = Ui_Patches()
        self.patches.setupUi(self)
        self.main = parent

        self.setWindowTitle(os.getenv("PINGUINO_NAME")+" - "+self.windowTitle())

        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/logo/art/windowIcon.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.setWindowIcon(icon)

        self.connect(self.patches.pushButton_install, QtCore.SIGNAL("clicked()"), self.install)
        self.connect(self.patches.pushButton_cancel, QtCore.SIGNAL("clicked()"), self.close)

        self.setStyleSheet("""
        font-family: inherit;
        font-weight: normal;

        """)

        self.center_on_screen()

    #----------------------------------------------------------------------
    def center_on_screen(self):

        screen = QtGui.QDesktopWidget().screenGeometry()
        size = self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)


    #----------------------------------------------------------------------
    def install(self):
        """"""



    #----------------------------------------------------------------------
    def get_patch(self):
        """"""

        # https://raw.githubusercontent.com/PinguinoIDE/pinguino-ide/8c69b22ef68ed50fb1f3b524ae3bf5bb775aba56/qtgui/ide/ide.py
