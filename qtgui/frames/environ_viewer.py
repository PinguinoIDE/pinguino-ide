# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/python/developing/pinguino/pinguino-ide/qtgui/frames/environ_viewer.ui'
#
# Created: Fri Dec 19 15:43:19 2014
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
        self.tableWidget.setRowCount(6)
        item = QtGui.QTableWidgetItem()
        self.tableWidget.setVerticalHeaderItem(0, item)
        item = QtGui.QTableWidgetItem()
        self.tableWidget.setVerticalHeaderItem(1, item)
        item = QtGui.QTableWidgetItem()
        self.tableWidget.setVerticalHeaderItem(2, item)
        item = QtGui.QTableWidgetItem()
        self.tableWidget.setVerticalHeaderItem(3, item)
        item = QtGui.QTableWidgetItem()
        self.tableWidget.setVerticalHeaderItem(4, item)
        item = QtGui.QTableWidgetItem()
        self.tableWidget.setVerticalHeaderItem(5, item)
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
        self.tableWidget.verticalHeaderItem(0).setText(QtGui.QApplication.translate("EnvironViewer", "New Row", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget.verticalHeaderItem(1).setText(QtGui.QApplication.translate("EnvironViewer", "New Row", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget.verticalHeaderItem(2).setText(QtGui.QApplication.translate("EnvironViewer", "New Row", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget.verticalHeaderItem(3).setText(QtGui.QApplication.translate("EnvironViewer", "New Row", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget.verticalHeaderItem(4).setText(QtGui.QApplication.translate("EnvironViewer", "New Row", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget.verticalHeaderItem(5).setText(QtGui.QApplication.translate("EnvironViewer", "New Row", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget.horizontalHeaderItem(0).setText(QtGui.QApplication.translate("EnvironViewer", "Name", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget.horizontalHeaderItem(1).setText(QtGui.QApplication.translate("EnvironViewer", "Value", None, QtGui.QApplication.UnicodeUTF8))

