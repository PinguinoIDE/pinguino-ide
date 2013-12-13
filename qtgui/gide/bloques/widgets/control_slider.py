# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/Pinguino/pinguino-ide/qtgui/gide/bloques/widgets/control_slider.ui'
#
# Created: Thu Dec 12 20:11:11 2013
#      by: pyside-uic 0.2.14 running on PySide 1.1.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Frame(object):
    def setupUi(self, Frame):
        Frame.setObjectName("Frame")
        Frame.resize(237, 36)
        self.gridLayout = QtGui.QGridLayout(Frame)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setObjectName("gridLayout")
        self.lineEdit_2 = QtGui.QLineEdit(Frame)
        self.lineEdit_2.setMaximumSize(QtCore.QSize(46, 16777215))
        font = QtGui.QFont()
        font.setFamily("Ubuntu Mono")
        font.setPointSize(15)
        font.setWeight(75)
        font.setBold(True)
        self.lineEdit_2.setFont(font)
        self.lineEdit_2.setStyleSheet("color: rgb(255, 255, 255);\n"
"background-color: rgba(255, 255, 255, 0);")
        self.lineEdit_2.setFrame(False)
        self.lineEdit_2.setReadOnly(True)
        self.lineEdit_2.setObjectName("lineEdit_2")
        self.gridLayout.addWidget(self.lineEdit_2, 0, 1, 1, 1)
        self.horizontalSlider = QtGui.QSlider(Frame)
        self.horizontalSlider.setCursor(QtCore.Qt.PointingHandCursor)
        self.horizontalSlider.setFocusPolicy(QtCore.Qt.NoFocus)
        self.horizontalSlider.setMaximum(1023)
        self.horizontalSlider.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider.setInvertedAppearance(False)
        self.horizontalSlider.setTickPosition(QtGui.QSlider.NoTicks)
        self.horizontalSlider.setTickInterval(128)
        self.horizontalSlider.setObjectName("horizontalSlider")
        self.gridLayout.addWidget(self.horizontalSlider, 0, 2, 1, 1)

        self.retranslateUi(Frame)
        QtCore.QMetaObject.connectSlotsByName(Frame)

    def retranslateUi(self, Frame):
        Frame.setWindowTitle(QtGui.QApplication.translate("Frame", "Frame", None, QtGui.QApplication.UnicodeUTF8))
        self.lineEdit_2.setText(QtGui.QApplication.translate("Frame", "0000", None, QtGui.QApplication.UnicodeUTF8))

