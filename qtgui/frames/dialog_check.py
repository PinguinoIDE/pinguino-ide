# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/.virtualenvs/pinguino_env/pinguino/pinguino-ide/qtgui/frames/dialog_check.ui'
#
# Created: Sat Jan 25 12:22:29 2014
#      by: pyside-uic 0.2.15 running on PySide 1.2.1
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_DialogCheck(object):
    def setupUi(self, DialogCheck):
        DialogCheck.setObjectName("DialogCheck")
        DialogCheck.resize(460, 191)
        self.gridLayout = QtGui.QGridLayout(DialogCheck)
        self.gridLayout.setObjectName("gridLayout")
        self.pushButton = QtGui.QPushButton(DialogCheck)
        self.pushButton.setMinimumSize(QtCore.QSize(165, 0))
        self.pushButton.setMaximumSize(QtCore.QSize(165, 16777215))
        self.pushButton.setObjectName("pushButton")
        self.gridLayout.addWidget(self.pushButton, 1, 2, 1, 1)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem, 1, 1, 1, 1)
        self.checkBox = QtGui.QCheckBox(DialogCheck)
        self.checkBox.setChecked(True)
        self.checkBox.setObjectName("checkBox")
        self.gridLayout.addWidget(self.checkBox, 1, 0, 1, 1)
        self.textBrowser = QtGui.QTextBrowser(DialogCheck)
        self.textBrowser.setOpenExternalLinks(True)
        self.textBrowser.setObjectName("textBrowser")
        self.gridLayout.addWidget(self.textBrowser, 0, 0, 1, 3)

        self.retranslateUi(DialogCheck)
        QtCore.QMetaObject.connectSlotsByName(DialogCheck)

    def retranslateUi(self, DialogCheck):
        DialogCheck.setWindowTitle(QtGui.QApplication.translate("DialogCheck", "Dialog", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton.setText(QtGui.QApplication.translate("DialogCheck", "Ok", None, QtGui.QApplication.UnicodeUTF8))
        self.checkBox.setText(QtGui.QApplication.translate("DialogCheck", "Show this message again", None, QtGui.QApplication.UnicodeUTF8))

