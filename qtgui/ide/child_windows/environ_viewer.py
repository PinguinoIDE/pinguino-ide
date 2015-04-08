#! /usr/bin/python2
#-*- coding: utf-8 -*-

import inspect
import os
from PySide import QtGui, QtCore


from ...frames.environ_viewer import Ui_EnvironViewer


########################################################################
class EnvironViewer(QtGui.QMainWindow):

    def __init__(self, parent, debug):
        #QtGui.QMainWindow.__init__(self)
        super(EnvironViewer, self).__init__()
        #self.setWindowFlags(QtCore.Qt.Tool)

        self.main = parent

        self.env_viewer = Ui_EnvironViewer()
        self.env_viewer.setupUi(self)

        self.setStyleSheet("""
        font-family: inherit;
        font-weight: normal;

        """)

        self.setWindowTitle(os.getenv("PINGUINO_NAME")+" - "+self.windowTitle())

        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/logo/art/windowIcon.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.setWindowIcon(icon)

        if debug == "environ":
            self.show_env()
        if debug == "variables":
            self.show_variables()


        self.center_on_screen()

    #----------------------------------------------------------------------
    def center_on_screen(self):

        screen = QtGui.QDesktopWidget().screenGeometry()
        size = self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)




    #----------------------------------------------------------------------
    def show_env(self):

        data = os.environ
        self.env_viewer.tableWidget.setRowCount(len(data))

        index = 0
        for key in data.keys():
            if not key.startswith("PINGUINO"): continue

            name = key
            value = data[name]

            self.env_viewer.tableWidget.setItem(index, 0, QtGui.QTableWidgetItem())
            self.env_viewer.tableWidget.setItem(index, 1, QtGui.QTableWidgetItem())
            self.env_viewer.tableWidget.item(index, 0).setText(name)
            self.env_viewer.tableWidget.item(index, 0).setForeground(QtGui.QColor("red"))
            self.env_viewer.tableWidget.item(index, 1).setText(value)
            index += 1

        keys = list(data.keys())
        keys.sort()

        for key in keys:
            if key.startswith("PINGUINO"): continue

            name = key
            value = data[name]

            self.env_viewer.tableWidget.setItem(index, 0, QtGui.QTableWidgetItem())
            self.env_viewer.tableWidget.setItem(index, 1, QtGui.QTableWidgetItem())
            self.env_viewer.tableWidget.item(index, 0).setText(name)
            self.env_viewer.tableWidget.item(index, 1).setText(value)
            index += 1



    #----------------------------------------------------------------------
    def show_variables(self):

        # data = self.main.__dict__
        data = dict(inspect.getmembers(self.main, predicate=inspect.ismethod))

        self.env_viewer.tableWidget.setRowCount(len(data))

        index = 0
        for key in data.keys():
            name = key
            value = data[name]

            self.env_viewer.tableWidget.setItem(index, 0, QtGui.QTableWidgetItem())
            self.env_viewer.tableWidget.setItem(index, 1, QtGui.QTableWidgetItem())
            self.env_viewer.tableWidget.item(index, 0).setText(name)
            self.env_viewer.tableWidget.item(index, 1).setText(str(value))
            index += 1
