# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/python/developing/pinguino/pinguino-ide/qtgui/frames/insert_block.ui'
#
# Created: Thu Oct 23 00:06:12 2014
#      by: pyside-uic 0.2.15 running on PySide 1.2.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_InsertBlock(object):
    def setupUi(self, InsertBlock):
        InsertBlock.setObjectName("InsertBlock")
        InsertBlock.resize(400, 107)
        self.gridLayout = QtGui.QGridLayout(InsertBlock)
        self.gridLayout.setContentsMargins(5, 5, 5, 5)
        self.gridLayout.setHorizontalSpacing(0)
        self.gridLayout.setVerticalSpacing(2)
        self.gridLayout.setObjectName("gridLayout")
        self.listWidget = QtGui.QListWidget(InsertBlock)
        self.listWidget.setObjectName("listWidget")
        self.gridLayout.addWidget(self.listWidget, 1, 0, 1, 2)
        self.lineEdit = QtGui.QLineEdit(InsertBlock)
        self.lineEdit.setObjectName("lineEdit")
        self.gridLayout.addWidget(self.lineEdit, 0, 0, 1, 2)

        self.retranslateUi(InsertBlock)
        QtCore.QMetaObject.connectSlotsByName(InsertBlock)
        InsertBlock.setTabOrder(self.lineEdit, self.listWidget)

    def retranslateUi(self, InsertBlock):
        InsertBlock.setWindowTitle(QtGui.QApplication.translate("InsertBlock", "Insert Block", None, QtGui.QApplication.UnicodeUTF8))

