# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/Pinguino/pinguino-ide/qtgui/frames/stdout.ui'
#
# Created: Sun Dec  8 20:57:10 2013
#      by: pyside-uic 0.2.14 running on PySide 1.1.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Stdout(object):
    def setupUi(self, Stdout):
        Stdout.setObjectName("Stdout")
        Stdout.resize(606, 432)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/logo/art/windowIcon.svg"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        Stdout.setWindowIcon(icon)
        self.centralwidget = QtGui.QWidget(Stdout)
        self.centralwidget.setObjectName("centralwidget")
        self.gridLayout = QtGui.QGridLayout(self.centralwidget)
        self.gridLayout.setObjectName("gridLayout")
        self.plainTextEdit = QtGui.QPlainTextEdit(self.centralwidget)
        self.plainTextEdit.setReadOnly(True)
        self.plainTextEdit.setObjectName("plainTextEdit")
        self.gridLayout.addWidget(self.plainTextEdit, 1, 0, 1, 1)
        self.buttonBox = QtGui.QDialogButtonBox(self.centralwidget)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Close)
        self.buttonBox.setObjectName("buttonBox")
        self.gridLayout.addWidget(self.buttonBox, 2, 0, 1, 1)
        Stdout.setCentralWidget(self.centralwidget)

        self.retranslateUi(Stdout)
        QtCore.QMetaObject.connectSlotsByName(Stdout)

    def retranslateUi(self, Stdout):
        Stdout.setWindowTitle(QtGui.QApplication.translate("Stdout", "Stdout", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
