# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/python/dev/developing/pinguino/pinguino-ide/qtgui/frames/stdout.ui'
#
# Created: Sat Mar 29 19:41:54 2014
#      by: pyside-uic 0.2.14 running on PySide 1.2.1
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_PlainOut(object):
    def setupUi(self, PlainOut):
        PlainOut.setObjectName("PlainOut")
        PlainOut.resize(596, 475)
        self.gridLayout = QtGui.QGridLayout(PlainOut)
        self.gridLayout.setObjectName("gridLayout")
        self.textEdit = QtGui.QTextEdit(PlainOut)
        self.textEdit.setReadOnly(True)
        self.textEdit.setObjectName("textEdit")
        self.gridLayout.addWidget(self.textEdit, 0, 0, 1, 2)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem, 1, 0, 1, 1)
        self.pushButton_close = QtGui.QPushButton(PlainOut)
        self.pushButton_close.setMinimumSize(QtCore.QSize(165, 0))
        self.pushButton_close.setObjectName("pushButton_close")
        self.gridLayout.addWidget(self.pushButton_close, 1, 1, 1, 1)

        self.retranslateUi(PlainOut)
        QtCore.QMetaObject.connectSlotsByName(PlainOut)

    def retranslateUi(self, PlainOut):
        PlainOut.setWindowTitle(QtGui.QApplication.translate("PlainOut", "Plain Out", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_close.setText(QtGui.QApplication.translate("PlainOut", "Close", None, QtGui.QApplication.UnicodeUTF8))

