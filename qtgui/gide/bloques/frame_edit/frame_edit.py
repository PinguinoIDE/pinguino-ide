# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/python/dev/developing/pinguino/pinguino-ide/qtgui/gide/bloques/frame_edit/frame_edit.ui'
#
# Created: Mon Sep  1 22:08:59 2014
#      by: pyside-uic 0.2.14 running on PySide 1.2.1
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(380, 125)
        Form.setWindowTitle("")
        self.gridLayout = QtGui.QGridLayout(Form)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.plainTextEdit = QtGui.QPlainTextEdit(Form)
        font = QtGui.QFont()
        font.setPointSize(12)
        self.plainTextEdit.setFont(font)
        self.plainTextEdit.setFrameShape(QtGui.QFrame.NoFrame)
        self.plainTextEdit.setPlainText("")
        self.plainTextEdit.setObjectName("plainTextEdit")
        self.gridLayout.addWidget(self.plainTextEdit, 1, 2, 1, 1)
        self.frame_8 = QtGui.QFrame(Form)
        self.frame_8.setMinimumSize(QtCore.QSize(0, 10))
        self.frame_8.setObjectName("frame_8")
        self.gridLayout.addWidget(self.frame_8, 0, 1, 1, 2)
        self.frame_7 = QtGui.QFrame(Form)
        self.frame_7.setMinimumSize(QtCore.QSize(10, 0))
        self.frame_7.setCursor(QtCore.Qt.OpenHandCursor)
        self.frame_7.setObjectName("frame_7")
        self.gridLayout.addWidget(self.frame_7, 1, 3, 1, 1)
        self.frame_6 = QtGui.QFrame(Form)
        self.frame_6.setMinimumSize(QtCore.QSize(0, 10))
        self.frame_6.setObjectName("frame_6")
        self.gridLayout.addWidget(self.frame_6, 2, 1, 1, 2)
        self.frame_4 = QtGui.QFrame(Form)
        self.frame_4.setObjectName("frame_4")
        self.gridLayout.addWidget(self.frame_4, 2, 3, 1, 1)
        self.frame_3 = QtGui.QFrame(Form)
        self.frame_3.setObjectName("frame_3")
        self.gridLayout.addWidget(self.frame_3, 2, 0, 1, 1)
        self.frame_2 = QtGui.QFrame(Form)
        self.frame_2.setObjectName("frame_2")
        self.gridLayout.addWidget(self.frame_2, 0, 3, 1, 1)
        self.frame_11 = QtGui.QFrame(Form)
        self.frame_11.setMinimumSize(QtCore.QSize(10, 0))
        self.frame_11.setObjectName("frame_11")
        self.gridLayout.addWidget(self.frame_11, 1, 1, 1, 1)
        self.frame_10 = QtGui.QFrame(Form)
        self.frame_10.setMinimumSize(QtCore.QSize(10, 0))
        self.frame_10.setObjectName("frame_10")
        self.gridLayout.addWidget(self.frame_10, 1, 0, 1, 1)
        self.frame = QtGui.QFrame(Form)
        self.frame.setObjectName("frame")
        self.gridLayout.addWidget(self.frame, 0, 0, 1, 1)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        pass

