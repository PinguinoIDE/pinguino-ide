# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/python/developing/pinguino/pinguino-ide/qtgui/frames/board_config_advance.ui'
#
# Created: Wed Mar  4 01:39:55 2015
#      by: pyside-uic 0.2.15 running on PySide 1.2.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_AdvanceBoardConfig(object):
    def setupUi(self, AdvanceBoardConfig):
        AdvanceBoardConfig.setObjectName("AdvanceBoardConfig")
        AdvanceBoardConfig.resize(280, 174)
        self.gridLayout = QtGui.QGridLayout(AdvanceBoardConfig)
        self.gridLayout.setObjectName("gridLayout")
        self.checkBox_mips16 = QtGui.QCheckBox(AdvanceBoardConfig)
        self.checkBox_mips16.setCursor(QtCore.Qt.WhatsThisCursor)
        self.checkBox_mips16.setObjectName("checkBox_mips16")
        self.gridLayout.addWidget(self.checkBox_mips16, 0, 0, 1, 1)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.label = QtGui.QLabel(AdvanceBoardConfig)
        self.label.setCursor(QtCore.Qt.WhatsThisCursor)
        self.label.setObjectName("label")
        self.horizontalLayout_2.addWidget(self.label)
        self.comboBox_heapsize = QtGui.QComboBox(AdvanceBoardConfig)
        self.comboBox_heapsize.setObjectName("comboBox_heapsize")
        self.horizontalLayout_2.addWidget(self.comboBox_heapsize)
        self.gridLayout.addLayout(self.horizontalLayout_2, 1, 0, 1, 1)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.label_2 = QtGui.QLabel(AdvanceBoardConfig)
        self.label_2.setCursor(QtCore.Qt.WhatsThisCursor)
        self.label_2.setObjectName("label_2")
        self.horizontalLayout_3.addWidget(self.label_2)
        self.comboBox_optimization = QtGui.QComboBox(AdvanceBoardConfig)
        self.comboBox_optimization.setObjectName("comboBox_optimization")
        self.comboBox_optimization.addItem("")
        self.comboBox_optimization.addItem("")
        self.comboBox_optimization.addItem("")
        self.horizontalLayout_3.addWidget(self.comboBox_optimization)
        self.gridLayout.addLayout(self.horizontalLayout_3, 2, 0, 1, 1)
        spacerItem = QtGui.QSpacerItem(188, 18, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.gridLayout.addItem(spacerItem, 3, 0, 1, 1)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.pushButton_defaul = QtGui.QPushButton(AdvanceBoardConfig)
        self.pushButton_defaul.setObjectName("pushButton_defaul")
        self.horizontalLayout.addWidget(self.pushButton_defaul)
        self.pushButton_accept = QtGui.QPushButton(AdvanceBoardConfig)
        self.pushButton_accept.setObjectName("pushButton_accept")
        self.horizontalLayout.addWidget(self.pushButton_accept)
        self.gridLayout.addLayout(self.horizontalLayout, 4, 0, 1, 1)

        self.retranslateUi(AdvanceBoardConfig)
        QtCore.QMetaObject.connectSlotsByName(AdvanceBoardConfig)

    def retranslateUi(self, AdvanceBoardConfig):
        AdvanceBoardConfig.setWindowTitle(QtGui.QApplication.translate("AdvanceBoardConfig", "Advance Board Config", None, QtGui.QApplication.UnicodeUTF8))
        self.checkBox_mips16.setToolTip(QtGui.QApplication.translate("AdvanceBoardConfig", "<html><head/><body><p>Checked: Less memory but also less speed.<br>Unchecked: More memory but also increase speed.</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.checkBox_mips16.setText(QtGui.QApplication.translate("AdvanceBoardConfig", "Mips16", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setToolTip(QtGui.QApplication.translate("AdvanceBoardConfig", "<html><head/><body><p>The amount of memory allocated to objects that are being defined.</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("AdvanceBoardConfig", "Heap Size:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setToolTip(QtGui.QApplication.translate("AdvanceBoardConfig", "<html><head/><body><p>-O2: The compiler will attempt to increase code performance without compromising on size, and without taking too much compilation time.</p><p>-O3: This is the highest level of optimization possible. It enables optimizations that are expensive in terms of compile time and memory usage, in many cases can slow down a system due to larger binaries and increased memory usage.</p><p>-Os: This option will optimize your code for size.</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("AdvanceBoardConfig", "Optimization:", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox_optimization.setItemText(0, QtGui.QApplication.translate("AdvanceBoardConfig", "-O2", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox_optimization.setItemText(1, QtGui.QApplication.translate("AdvanceBoardConfig", "-O3", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox_optimization.setItemText(2, QtGui.QApplication.translate("AdvanceBoardConfig", "-Os", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_defaul.setText(QtGui.QApplication.translate("AdvanceBoardConfig", "Restore default", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_accept.setText(QtGui.QApplication.translate("AdvanceBoardConfig", "Accept", None, QtGui.QApplication.UnicodeUTF8))

