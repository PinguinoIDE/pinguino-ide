# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/Desarrollo/Pinguino/GitHub/pinguino-ide/pinguino/qtgui/frames/insert_block.ui'
#
# Created: Wed Mar 16 13:19:35 2016
#      by: pyside-uic 0.2.15 running on PySide 1.2.4
#
# WARNING! All changes made in this file will be lost!

from PySide2 import QtCore, QtGui, QtWidgets

class Ui_InsertBlock(object):
    def setupUi(self, InsertBlock):
        InsertBlock.setObjectName("InsertBlock")
        InsertBlock.resize(400, 107)
        self.gridLayout = QtWidgets.QGridLayout(InsertBlock)
        self.gridLayout.setContentsMargins(5, 5, 5, 5)
        self.gridLayout.setHorizontalSpacing(0)
        self.gridLayout.setVerticalSpacing(2)
        self.gridLayout.setObjectName("gridLayout")
        self.listWidget = QtGui.QListWidget(InsertBlock)
        self.listWidget.setObjectName("listWidget")
        self.gridLayout.addWidget(self.listWidget, 1, 0, 1, 2)
        self.lineEdit = QtWidgets.QLineEdit(InsertBlock)
        self.lineEdit.setObjectName("lineEdit")
        self.gridLayout.addWidget(self.lineEdit, 0, 0, 1, 2)

        self.retranslateUi(InsertBlock)
        QtCore.QMetaObject.connectSlotsByName(InsertBlock)
        InsertBlock.setTabOrder(self.lineEdit, self.listWidget)

    def retranslateUi(self, InsertBlock):
        InsertBlock.setWindowTitle(QtWidgets.QApplication.translate("InsertBlock", "Insert Block", None, QtWidgets.QApplication.UnicodeUTF8))

