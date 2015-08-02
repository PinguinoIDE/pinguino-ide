# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/Desarrollo/Pinguino/GitHub/pinguino-ide/qtgui/frames/environ_viewer.ui'
#
# Created: Sat Aug  1 23:10:06 2015
#      by: pyside-uic 0.2.15 running on PySide 1.2.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_EnvironViewer(object):
    def setupUi(self, EnvironViewer):
        EnvironViewer.setObjectName("EnvironViewer")
        EnvironViewer.resize(476, 614)
        self.centralwidget = QtGui.QWidget(EnvironViewer)
        self.centralwidget.setObjectName("centralwidget")
        self.gridLayout = QtGui.QGridLayout(self.centralwidget)
        self.gridLayout.setObjectName("gridLayout")
        self.tableWidget = QtGui.QTableWidget(self.centralwidget)
        self.tableWidget.setObjectName("tableWidget")
        self.tableWidget.setColumnCount(2)
        self.tableWidget.setRowCount(0)
        item = QtGui.QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(0, item)
        item = QtGui.QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(1, item)
        self.tableWidget.horizontalHeader().setDefaultSectionSize(216)
        self.tableWidget.horizontalHeader().setStretchLastSection(True)
        self.tableWidget.verticalHeader().setVisible(False)
        self.gridLayout.addWidget(self.tableWidget, 0, 0, 1, 1)
        EnvironViewer.setCentralWidget(self.centralwidget)

        self.retranslateUi(EnvironViewer)
        QtCore.QMetaObject.connectSlotsByName(EnvironViewer)

    def retranslateUi(self, EnvironViewer):
        EnvironViewer.setWindowTitle(QtGui.QApplication.translate("EnvironViewer", "Environ Viewer", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget.horizontalHeaderItem(0).setText(QtGui.QApplication.translate("EnvironViewer", "Name", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget.horizontalHeaderItem(1).setText(QtGui.QApplication.translate("EnvironViewer", "Value", None, QtGui.QApplication.UnicodeUTF8))

