# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/Desarrollo/Pinguino/GitHub/pinguino-ide/qtgui/frames/paths.ui'
#
# Created: Fri Jul 10 13:53:22 2015
#      by: pyside-uic 0.2.15 running on PySide 1.2.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Paths(object):
    def setupUi(self, Paths):
        Paths.setObjectName("Paths")
        Paths.resize(737, 152)
        self.gridLayout_5 = QtGui.QGridLayout(Paths)
        self.gridLayout_5.setObjectName("gridLayout_5")
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        spacerItem = QtGui.QSpacerItem(304, 23, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem)
        self.pushButton_close = QtGui.QPushButton(Paths)
        self.pushButton_close.setMinimumSize(QtCore.QSize(165, 0))
        self.pushButton_close.setObjectName("pushButton_close")
        self.horizontalLayout_2.addWidget(self.pushButton_close)
        self.gridLayout_5.addLayout(self.horizontalLayout_2, 3, 0, 1, 1)
        self.label = QtGui.QLabel(Paths)
        self.label.setObjectName("label")
        self.gridLayout_5.addWidget(self.label, 4, 0, 1, 1)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setSpacing(0)
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.label_6 = QtGui.QLabel(Paths)
        self.label_6.setObjectName("label_6")
        self.horizontalLayout_3.addWidget(self.label_6)
        self.lineEdit_pinguino_8_libs = QtGui.QLineEdit(Paths)
        self.lineEdit_pinguino_8_libs.setObjectName("lineEdit_pinguino_8_libs")
        self.horizontalLayout_3.addWidget(self.lineEdit_pinguino_8_libs)
        self.pushButton_clear_8_libs = QtGui.QPushButton(Paths)
        self.pushButton_clear_8_libs.setMaximumSize(QtCore.QSize(26, 16777215))
        self.pushButton_clear_8_libs.setText("")
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/icons/icons/clear.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.pushButton_clear_8_libs.setIcon(icon)
        self.pushButton_clear_8_libs.setFlat(True)
        self.pushButton_clear_8_libs.setObjectName("pushButton_clear_8_libs")
        self.horizontalLayout_3.addWidget(self.pushButton_clear_8_libs)
        self.pushButton_pinguino_8_libs = QtGui.QPushButton(Paths)
        self.pushButton_pinguino_8_libs.setObjectName("pushButton_pinguino_8_libs")
        self.horizontalLayout_3.addWidget(self.pushButton_pinguino_8_libs)
        self.gridLayout_5.addLayout(self.horizontalLayout_3, 0, 0, 1, 1)
        self.horizontalLayout_5 = QtGui.QHBoxLayout()
        self.horizontalLayout_5.setSpacing(0)
        self.horizontalLayout_5.setObjectName("horizontalLayout_5")
        self.label_8 = QtGui.QLabel(Paths)
        self.label_8.setObjectName("label_8")
        self.horizontalLayout_5.addWidget(self.label_8)
        self.lineEdit_pinguino_32_libs = QtGui.QLineEdit(Paths)
        self.lineEdit_pinguino_32_libs.setObjectName("lineEdit_pinguino_32_libs")
        self.horizontalLayout_5.addWidget(self.lineEdit_pinguino_32_libs)
        self.pushButton_clear_32_libs = QtGui.QPushButton(Paths)
        self.pushButton_clear_32_libs.setMaximumSize(QtCore.QSize(26, 16777215))
        self.pushButton_clear_32_libs.setText("")
        self.pushButton_clear_32_libs.setIcon(icon)
        self.pushButton_clear_32_libs.setFlat(True)
        self.pushButton_clear_32_libs.setObjectName("pushButton_clear_32_libs")
        self.horizontalLayout_5.addWidget(self.pushButton_clear_32_libs)
        self.pushButton_pinguino_32_libs = QtGui.QPushButton(Paths)
        self.pushButton_pinguino_32_libs.setObjectName("pushButton_pinguino_32_libs")
        self.horizontalLayout_5.addWidget(self.pushButton_pinguino_32_libs)
        self.gridLayout_5.addLayout(self.horizontalLayout_5, 1, 0, 1, 1)

        self.retranslateUi(Paths)
        QtCore.QMetaObject.connectSlotsByName(Paths)

    def retranslateUi(self, Paths):
        Paths.setWindowTitle(QtGui.QApplication.translate("Paths", "Paths", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_close.setText(QtGui.QApplication.translate("Paths", "Close", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Paths", "<html><head/><body><p>Pinguino IDE requires a restart to reinitialize its preferences.</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.label_6.setText(QtGui.QApplication.translate("Paths", "8-bit libraries:", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_pinguino_8_libs.setText(QtGui.QApplication.translate("Paths", "Change...", None, QtGui.QApplication.UnicodeUTF8))
        self.label_8.setText(QtGui.QApplication.translate("Paths", "32-bit libraries:", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_pinguino_32_libs.setText(QtGui.QApplication.translate("Paths", "Change...", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
