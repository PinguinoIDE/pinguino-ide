#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import requests

from PySide import QtGui, QtCore

from ...frames.patches import Ui_Patches

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from configparser import RawConfigParser
    from io import StringIO
else:
    #Python2
    from ConfigParser import RawConfigParser
    from cStringIO import StringIO


# BRANCH = "master"
BRANCH = "11.1"
PATCHES = "https://raw.githubusercontent.com/PinguinoIDE/pinguino-ide/%s/patches" % BRANCH


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
        self.get_patches()

    #----------------------------------------------------------------------
    def center_on_screen(self):

        screen = QtGui.QDesktopWidget().screenGeometry()
        size = self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)


    #----------------------------------------------------------------------
    def install(self):
        """"""



    #----------------------------------------------------------------------
    def get_patches(self):
        """"""
        r = requests.get(PATCHES)
        patches = r.text.decode(r.encoding)
        buf = StringIO(patches)
        parser = RawConfigParser()
        parser.readfp(buf)
        buf.close()

        index = 0
        for patch in parser.sections():

            item = QtGui.QTableWidgetItem(parser.get(patch, "patch"))
            item.setCheckState(QtCore.Qt.Checked)
            self.patches.tableWidget_patches.setItem(index, 0, item)
            item = QtGui.QTableWidgetItem(parser.get(patch, "summary"))
            self.patches.tableWidget_patches.setItem(index, 1, item)

            index += 1



        # https://raw.githubusercontent.com/PinguinoIDE/pinguino-ide/8c69b22ef68ed50fb1f3b524ae3bf5bb775aba56/qtgui/ide/ide.py
