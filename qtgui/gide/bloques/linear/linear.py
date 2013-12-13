# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/Pinguino/pinguino-ide/qtgui/gide/bloques/linear/linear.ui'
#
# Created: Thu Dec 12 20:11:11 2013
#      by: pyside-uic 0.2.14 running on PySide 1.1.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(200, 46)
        self.gridLayout = QtGui.QGridLayout(Form)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setObjectName("gridLayout")
        self.frame = QtGui.QFrame(Form)
        self.frame.setMinimumSize(QtCore.QSize(26, 46))
        self.frame.setMaximumSize(QtCore.QSize(26, 46))
        self.frame.setObjectName("frame")
        self.gridLayout.addWidget(self.frame, 0, 0, 1, 1)
        self.frame_3 = QtGui.QFrame(Form)
        self.frame_3.setMinimumSize(QtCore.QSize(6, 46))
        self.frame_3.setMaximumSize(QtCore.QSize(6, 46))
        self.frame_3.setObjectName("frame_3")
        self.gridLayout.addWidget(self.frame_3, 0, 2, 1, 1)
        self.area = QtGui.QFrame(Form)
        self.area.setMinimumSize(QtCore.QSize(0, 46))
        self.area.setMaximumSize(QtCore.QSize(16777215, 46))
        self.area.setAutoFillBackground(False)
        self.area.setObjectName("area")
        self.gridLayout_2 = QtGui.QGridLayout(self.area)
        self.gridLayout_2.setSpacing(0)
        self.gridLayout_2.setContentsMargins(0, 0, 0, 7)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setSpacing(0)
        self.horizontalLayout.setContentsMargins(0, -1, -1, -1)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.gridLayout_2.addLayout(self.horizontalLayout, 0, 0, 1, 1)
        self.gridLayout.addWidget(self.area, 0, 1, 1, 1)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(QtGui.QApplication.translate("Form", "Form", None, QtGui.QApplication.UnicodeUTF8))

