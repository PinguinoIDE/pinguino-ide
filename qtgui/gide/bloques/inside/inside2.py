# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/Pinguino/pinguino-ide/qtgui/gide/bloques/inside/inside2.ui'
#
# Created: Tue Dec 17 17:45:43 2013
#      by: pyside-uic 0.2.14 running on PySide 1.1.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(228, 34)
        self.gridLayout = QtGui.QGridLayout(Form)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setObjectName("gridLayout")
        self.frame_2 = QtGui.QFrame(Form)
        self.frame_2.setMinimumSize(QtCore.QSize(0, 34))
        self.frame_2.setMaximumSize(QtCore.QSize(16777215, 34))
        self.frame_2.setObjectName("frame_2")
        self.gridLayout.addWidget(self.frame_2, 0, 1, 1, 1)
        self.frame = QtGui.QFrame(Form)
        self.frame.setMinimumSize(QtCore.QSize(8, 34))
        self.frame.setMaximumSize(QtCore.QSize(8, 34))
        self.frame.setObjectName("frame")
        self.gridLayout.addWidget(self.frame, 0, 0, 1, 1)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(QtGui.QApplication.translate("Form", "Form", None, QtGui.QApplication.UnicodeUTF8))

