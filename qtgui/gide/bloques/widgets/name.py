# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/Pinguino/pinguino-ide/qtgui/gide/bloques/widgets/name.ui'
#
# Created: Tue Dec 31 12:23:58 2013
#      by: pyside-uic 0.2.14 running on PySide 1.2.1
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Frame(object):
    def setupUi(self, Frame):
        Frame.setObjectName("Frame")
        Frame.resize(94, 33)
        self.gridLayout = QtGui.QGridLayout(Frame)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.lineEdit = QtGui.QLineEdit(Frame)
        font = QtGui.QFont()
        font.setFamily("Ubuntu Mono")
        font.setPointSize(15)
        font.setWeight(75)
        font.setBold(True)
        self.lineEdit.setFont(font)
        self.lineEdit.setFrame(False)
        self.lineEdit.setObjectName("lineEdit")
        self.gridLayout.addWidget(self.lineEdit, 0, 0, 1, 1)

        self.retranslateUi(Frame)
        QtCore.QMetaObject.connectSlotsByName(Frame)

    def retranslateUi(self, Frame):
        Frame.setWindowTitle(QtGui.QApplication.translate("Frame", "Frame", None, QtGui.QApplication.UnicodeUTF8))
        self.lineEdit.setText(QtGui.QApplication.translate("Frame", "Bloq", None, QtGui.QApplication.UnicodeUTF8))

