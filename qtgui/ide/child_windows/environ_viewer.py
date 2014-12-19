#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
from PySide import QtGui, QtCore


from ...frames.environ_viewer import Ui_EnvironViewer


########################################################################
class EnvironViewer(QtGui.QMainWindow):

    def __init__(self):
        #QtGui.QMainWindow.__init__(self)
        super(EnvironViewer, self).__init__()
        #self.setWindowFlags(QtCore.Qt.Tool)

        self.env_viewer = Ui_EnvironViewer()
        self.env_viewer.setupUi(self)

        self.setStyleSheet("""
        font-family: inherit;
        font-weight: normal;

        """)

        self.setWindowTitle(os.getenv("NAME")+" - "+self.windowTitle())

        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/logo/art/windowIcon.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.setWindowIcon(icon)

        self.show_env()


        self.center_on_screen()

    #----------------------------------------------------------------------
    def center_on_screen(self):

        screen = QtGui.QDesktopWidget().screenGeometry()
        size = self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)




    #----------------------------------------------------------------------
    def show_env(self):
        """"""
        self.env_viewer.tableWidget.setRowCount(len(os.environ))

        index = 0
        for key in os.environ.keys():
            if not key.startswith("PINGUINO"): continue

            name = key
            value = os.environ[name]

            self.env_viewer.tableWidget.setItem(index, 0, QtGui.QTableWidgetItem())
            self.env_viewer.tableWidget.setItem(index, 1, QtGui.QTableWidgetItem())
            self.env_viewer.tableWidget.item(index, 0).setText(name)
            self.env_viewer.tableWidget.item(index, 0).setForeground(QtGui.QColor("red"))
            self.env_viewer.tableWidget.item(index, 1).setText(value)
            index += 1

        keys = list(os.environ.keys())
        keys.sort()

        for key in keys:
            if key.startswith("PINGUINO"): continue

            name = key
            value = os.environ[name]

            self.env_viewer.tableWidget.setItem(index, 0, QtGui.QTableWidgetItem())
            self.env_viewer.tableWidget.setItem(index, 1, QtGui.QTableWidgetItem())
            self.env_viewer.tableWidget.item(index, 0).setText(name)
            self.env_viewer.tableWidget.item(index, 1).setText(value)
            index += 1


