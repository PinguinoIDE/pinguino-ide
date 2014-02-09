# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/.virtualenvs/pinguino_env/pinguino/pinguino-ide/qtgui/gide/bloques/inside/inside2_bool.ui'
#
# Created: Sat Feb  8 23:39:15 2014
#      by: pyside-uic 0.2.15 running on PySide 1.2.1
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(94, 35)
        Form.setWindowTitle("")
        self.gridLayout = QtGui.QGridLayout(Form)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.frame = QtGui.QFrame(Form)
        self.frame.setMinimumSize(QtCore.QSize(8, 34))
        self.frame.setMaximumSize(QtCore.QSize(8, 34))
        self.frame.setObjectName("frame")
        self.gridLayout.addWidget(self.frame, 0, 0, 1, 1)
        self.frame_2 = QtGui.QFrame(Form)
        self.frame_2.setMinimumSize(QtCore.QSize(0, 34))
        self.frame_2.setMaximumSize(QtCore.QSize(16777215, 34))
        self.frame_2.setObjectName("frame_2")
        self.gridLayout.addWidget(self.frame_2, 0, 1, 1, 1)
        self.frame_3 = QtGui.QFrame(Form)
        self.frame_3.setMinimumSize(QtCore.QSize(8, 34))
        self.frame_3.setMaximumSize(QtCore.QSize(8, 34))
        self.frame_3.setObjectName("frame_3")
        self.gridLayout.addWidget(self.frame_3, 0, 2, 1, 1)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        pass

