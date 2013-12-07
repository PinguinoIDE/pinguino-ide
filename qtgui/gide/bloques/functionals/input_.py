# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/python/dev/developing/Pinguino11/qtgui/gide/bloques/functionals/input_.ui'
#
# Created: Sat Dec  7 18:36:33 2013
#      by: pyside-uic 0.2.14 running on PySide 1.1.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(317, 33)
        Form.setStyleSheet("background-image: url(:/box/frame.svg);")
        self.gridLayout = QtGui.QGridLayout(Form)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setHorizontalSpacing(3)
        self.gridLayout.setObjectName("gridLayout")
        self.widget = QtGui.QWidget(Form)
        self.widget.setMinimumSize(QtCore.QSize(16, 16))
        self.widget.setMaximumSize(QtCore.QSize(16, 16))
        self.widget.setStyleSheet("background-image: url(:/box/out.svg);")
        self.widget.setObjectName("widget")
        self.gridLayout.addWidget(self.widget, 0, 0, 1, 1)
        spacerItem = QtGui.QSpacerItem(40, 1, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem, 1, 2, 1, 1)
        self.lineEdit = QtGui.QLineEdit(Form)
        self.lineEdit.setMinimumSize(QtCore.QSize(83, 26))
        self.lineEdit.setMaximumSize(QtCore.QSize(83, 26))
        font = QtGui.QFont()
        font.setFamily("Ubuntu Mono")
        self.lineEdit.setFont(font)
        self.lineEdit.setText("")
        self.lineEdit.setObjectName("lineEdit")
        self.gridLayout.addWidget(self.lineEdit, 0, 1, 1, 1)
        self.label = QtGui.QLabel(Form)
        font = QtGui.QFont()
        font.setFamily("Ubuntu Mono")
        font.setPointSize(12)
        self.label.setFont(font)
        self.label.setObjectName("label")
        self.gridLayout.addWidget(self.label, 0, 2, 1, 1)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(QtGui.QApplication.translate("Form", "Form", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Form", "TextLabel", None, QtGui.QApplication.UnicodeUTF8))

import images_rc
