# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/python/developing/pinguino/pinguino-ide/qtgui/gide/bloques/inside/part_bool.ui'
#
# Created: Thu Oct  2 13:25:31 2014
#      by: pyside-uic 0.2.15 running on PySide 1.2.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(94, 34)
        Form.setWindowTitle("")
        self.gridLayout = QtGui.QGridLayout(Form)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setObjectName("gridLayout")
        self.frame = QtGui.QFrame(Form)
        self.frame.setMinimumSize(QtCore.QSize(10, 34))
        self.frame.setMaximumSize(QtCore.QSize(10, 34))
        self.frame.setObjectName("frame")
        self.gridLayout.addWidget(self.frame, 0, 0, 1, 1)
        self.frame_2 = QtGui.QFrame(Form)
        self.frame_2.setMinimumSize(QtCore.QSize(0, 34))
        self.frame_2.setMaximumSize(QtCore.QSize(16777215, 34))
        self.frame_2.setObjectName("frame_2")
        self.gridLayout_2 = QtGui.QGridLayout(self.frame_2)
        self.gridLayout_2.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_2.setSpacing(0)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setSpacing(0)
        self.horizontalLayout.setContentsMargins(-1, 0, 0, -1)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.gridLayout_2.addLayout(self.horizontalLayout, 0, 0, 1, 1)
        self.gridLayout.addWidget(self.frame_2, 0, 1, 1, 1)
        self.frame_3 = QtGui.QFrame(Form)
        self.frame_3.setMinimumSize(QtCore.QSize(10, 34))
        self.frame_3.setMaximumSize(QtCore.QSize(10, 34))
        self.frame_3.setObjectName("frame_3")
        self.gridLayout.addWidget(self.frame_3, 0, 2, 1, 1)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        pass

