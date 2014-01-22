# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/.virtualenvs/pinguino_env/pinguino/pinguino-ide/qtgui/frames/stdout.ui'
#
# Created: Wed Jan 22 12:27:11 2014
#      by: pyside-uic 0.2.15 running on PySide 1.2.1
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_PlainOut(object):
    def setupUi(self, PlainOut):
        PlainOut.setObjectName("PlainOut")
        PlainOut.resize(606, 432)
        PlainOut.setWindowTitle("Plain Out")
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/logo/art/windowIcon.svg"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        PlainOut.setWindowIcon(icon)
        self.centralwidget = QtGui.QWidget(PlainOut)
        self.centralwidget.setObjectName("centralwidget")
        self.gridLayout = QtGui.QGridLayout(self.centralwidget)
        self.gridLayout.setObjectName("gridLayout")
        self.buttonBox = QtGui.QDialogButtonBox(self.centralwidget)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Close)
        self.buttonBox.setObjectName("buttonBox")
        self.gridLayout.addWidget(self.buttonBox, 2, 0, 1, 1)
        self.textEdit = QtGui.QTextEdit(self.centralwidget)
        self.textEdit.setReadOnly(True)
        self.textEdit.setObjectName("textEdit")
        self.gridLayout.addWidget(self.textEdit, 1, 0, 1, 1)
        PlainOut.setCentralWidget(self.centralwidget)

        self.retranslateUi(PlainOut)
        QtCore.QMetaObject.connectSlotsByName(PlainOut)

    def retranslateUi(self, PlainOut):
        pass

import resources_rc
