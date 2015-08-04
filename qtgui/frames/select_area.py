# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/Desarrollo/Pinguino/GitHub/pinguino-ide/qtgui/frames/select_area.ui'
#
# Created: Mon Aug  3 17:12:12 2015
#      by: pyside-uic 0.2.15 running on PySide 1.2.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Selection(object):
    def setupUi(self, Selection):
        Selection.setObjectName("Selection")
        Selection.resize(360, 145)
        Selection.setWindowTitle("SelectArea")
        self.gridLayout = QtGui.QGridLayout(Selection)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setObjectName("gridLayout")
        self.border_w = QtGui.QFrame(Selection)
        self.border_w.setMinimumSize(QtCore.QSize(1, 0))
        self.border_w.setMaximumSize(QtCore.QSize(1, 16777215))
        self.border_w.setStyleSheet("background-image: url(:/selection/selection/border.png);")
        self.border_w.setObjectName("border_w")
        self.horizontalLayout_7 = QtGui.QHBoxLayout(self.border_w)
        self.horizontalLayout_7.setSpacing(0)
        self.horizontalLayout_7.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_7.setObjectName("horizontalLayout_7")
        self.gridLayout.addWidget(self.border_w, 1, 0, 1, 1)
        self.background = QtGui.QFrame(Selection)
        self.background.setStyleSheet("background-image: url(:/selection/selection/background.png);")
        self.background.setObjectName("background")
        self.horizontalLayout_2 = QtGui.QHBoxLayout(self.background)
        self.horizontalLayout_2.setSpacing(0)
        self.horizontalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.gridLayout.addWidget(self.background, 1, 1, 1, 1)
        self.border_e = QtGui.QFrame(Selection)
        self.border_e.setMinimumSize(QtCore.QSize(1, 0))
        self.border_e.setMaximumSize(QtCore.QSize(1, 16777215))
        self.border_e.setStyleSheet("background-image: url(:/selection/selection/border.png);")
        self.border_e.setObjectName("border_e")
        self.horizontalLayout_9 = QtGui.QHBoxLayout(self.border_e)
        self.horizontalLayout_9.setSpacing(0)
        self.horizontalLayout_9.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_9.setObjectName("horizontalLayout_9")
        self.gridLayout.addWidget(self.border_e, 1, 2, 1, 1)
        self.border_n = QtGui.QFrame(Selection)
        self.border_n.setMinimumSize(QtCore.QSize(0, 1))
        self.border_n.setMaximumSize(QtCore.QSize(16777215, 1))
        self.border_n.setStyleSheet("background-image: url(:/selection/selection/border.png);")
        self.border_n.setObjectName("border_n")
        self.horizontalLayout_11 = QtGui.QHBoxLayout(self.border_n)
        self.horizontalLayout_11.setSpacing(0)
        self.horizontalLayout_11.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_11.setObjectName("horizontalLayout_11")
        self.gridLayout.addWidget(self.border_n, 0, 0, 1, 3)
        self.border_s = QtGui.QFrame(Selection)
        self.border_s.setMinimumSize(QtCore.QSize(0, 1))
        self.border_s.setMaximumSize(QtCore.QSize(16777215, 1))
        self.border_s.setStyleSheet("background-image: url(:/selection/selection/border.png);")
        self.border_s.setObjectName("border_s")
        self.horizontalLayout_5 = QtGui.QHBoxLayout(self.border_s)
        self.horizontalLayout_5.setSpacing(0)
        self.horizontalLayout_5.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_5.setObjectName("horizontalLayout_5")
        self.gridLayout.addWidget(self.border_s, 2, 0, 1, 3)

        self.retranslateUi(Selection)
        QtCore.QMetaObject.connectSlotsByName(Selection)

    def retranslateUi(self, Selection):
        pass

import resources_rc
