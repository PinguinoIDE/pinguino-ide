# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/Pinguino/pinguino-ide/qtgui/gide/bloques/widgets/control_spin.ui'
#
# Created: Sun Dec 22 08:31:36 2013
#      by: pyside-uic 0.2.14 running on PySide 1.1.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Frame(object):
    def setupUi(self, Frame):
        Frame.setObjectName("Frame")
        Frame.resize(94, 32)
        self.gridLayout = QtGui.QGridLayout(Frame)
        self.gridLayout.setContentsMargins(0, 0, 3, 0)
        self.gridLayout.setHorizontalSpacing(3)
        self.gridLayout.setVerticalSpacing(0)
        self.gridLayout.setObjectName("gridLayout")
        self.lineEdit = QtGui.QLineEdit(Frame)
        font = QtGui.QFont()
        font.setFamily("Ubuntu Mono")
        font.setPointSize(15)
        font.setWeight(75)
        font.setBold(True)
        self.lineEdit.setFont(font)
        self.lineEdit.setFrame(False)
        self.lineEdit.setAlignment(QtCore.Qt.AlignCenter)
        self.lineEdit.setObjectName("lineEdit")
        self.gridLayout.addWidget(self.lineEdit, 0, 0, 1, 1)
        self.frame_4 = QtGui.QFrame(Frame)
        self.frame_4.setObjectName("frame_4")
        self.gridLayout_4 = QtGui.QGridLayout(self.frame_4)
        self.gridLayout_4.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_4.setSpacing(0)
        self.gridLayout_4.setObjectName("gridLayout_4")
        self.verticalLayout_2 = QtGui.QVBoxLayout()
        self.verticalLayout_2.setSpacing(0)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.frame = QtGui.QFrame(self.frame_4)
        self.frame.setMinimumSize(QtCore.QSize(15, 15))
        self.frame.setMaximumSize(QtCore.QSize(15, 15))
        self.frame.setObjectName("frame")
        self.verticalLayout_2.addWidget(self.frame)
        self.frame_2 = QtGui.QFrame(self.frame_4)
        self.frame_2.setMinimumSize(QtCore.QSize(15, 15))
        self.frame_2.setMaximumSize(QtCore.QSize(15, 15))
        self.frame_2.setObjectName("frame_2")
        self.verticalLayout_2.addWidget(self.frame_2)
        self.gridLayout_4.addLayout(self.verticalLayout_2, 0, 0, 1, 1)
        self.gridLayout.addWidget(self.frame_4, 0, 1, 1, 1)

        self.retranslateUi(Frame)
        QtCore.QMetaObject.connectSlotsByName(Frame)

    def retranslateUi(self, Frame):
        Frame.setWindowTitle(QtGui.QApplication.translate("Frame", "Frame", None, QtGui.QApplication.UnicodeUTF8))
        self.lineEdit.setText(QtGui.QApplication.translate("Frame", "Bloq", None, QtGui.QApplication.UnicodeUTF8))

