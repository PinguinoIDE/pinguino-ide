# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/Desarrollo/Pinguino/GitHub/pinguino-ide/qtgui/frames/patches.ui'
#
# Created: Fri Jul  3 21:14:56 2015
#      by: pyside-uic 0.2.15 running on PySide 1.2.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Patches(object):
    def setupUi(self, Patches):
        Patches.setObjectName("Patches")
        Patches.resize(457, 324)
        self.gridLayout = QtGui.QGridLayout(Patches)
        self.gridLayout.setObjectName("gridLayout")
        spacerItem = QtGui.QSpacerItem(20, 20, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Fixed)
        self.gridLayout.addItem(spacerItem, 1, 0, 1, 1)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem1)
        self.pushButton_install = QtGui.QPushButton(Patches)
        self.pushButton_install.setObjectName("pushButton_install")
        self.horizontalLayout.addWidget(self.pushButton_install)
        self.pushButton_cancel = QtGui.QPushButton(Patches)
        self.pushButton_cancel.setObjectName("pushButton_cancel")
        self.horizontalLayout.addWidget(self.pushButton_cancel)
        self.gridLayout.addLayout(self.horizontalLayout, 3, 0, 1, 1)
        self.tableWidget_patches = QtGui.QTableWidget(Patches)
        self.tableWidget_patches.setObjectName("tableWidget_patches")
        self.tableWidget_patches.setColumnCount(2)
        self.tableWidget_patches.setRowCount(0)
        item = QtGui.QTableWidgetItem()
        self.tableWidget_patches.setHorizontalHeaderItem(0, item)
        item = QtGui.QTableWidgetItem()
        self.tableWidget_patches.setHorizontalHeaderItem(1, item)
        self.tableWidget_patches.horizontalHeader().setDefaultSectionSize(200)
        self.tableWidget_patches.horizontalHeader().setStretchLastSection(True)
        self.tableWidget_patches.verticalHeader().setVisible(False)
        self.tableWidget_patches.verticalHeader().setStretchLastSection(False)
        self.gridLayout.addWidget(self.tableWidget_patches, 2, 0, 1, 1)
        self.label = QtGui.QLabel(Patches)
        self.label.setObjectName("label")
        self.gridLayout.addWidget(self.label, 0, 0, 1, 1)

        self.retranslateUi(Patches)
        QtCore.QMetaObject.connectSlotsByName(Patches)

    def retranslateUi(self, Patches):
        Patches.setWindowTitle(QtGui.QApplication.translate("Patches", "Patches", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_install.setText(QtGui.QApplication.translate("Patches", "Install", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_cancel.setText(QtGui.QApplication.translate("Patches", "Cancel", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget_patches.horizontalHeaderItem(0).setText(QtGui.QApplication.translate("Patches", "Patch", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget_patches.horizontalHeaderItem(1).setText(QtGui.QApplication.translate("Patches", "Summary", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Patches", "This patches are minimal bug fixing.", None, QtGui.QApplication.UnicodeUTF8))

