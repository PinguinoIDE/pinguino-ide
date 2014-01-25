# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/.virtualenvs/pinguino_env/pinguino/pinguino-ide/qtgui/frames/blocks_widget.ui'
#
# Created: Sat Jan 25 17:07:44 2014
#      by: pyside-uic 0.2.15 running on PySide 1.2.1
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_widgetBlock(object):
    def setupUi(self, widgetBlock):
        widgetBlock.setObjectName("widgetBlock")
        widgetBlock.resize(306, 360)
        widgetBlock.setWindowTitle("Tool Area")
        self.gridLayout = QtGui.QGridLayout(widgetBlock)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.scrollArea = QtGui.QScrollArea(widgetBlock)
        self.scrollArea.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAsNeeded)
        self.scrollArea.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAsNeeded)
        self.scrollArea.setWidgetResizable(True)
        self.scrollArea.setObjectName("scrollArea")
        self.scrollAreaWidgetContents = QtGui.QWidget()
        self.scrollAreaWidgetContents.setGeometry(QtCore.QRect(0, 0, 304, 358))
        self.scrollAreaWidgetContents.setObjectName("scrollAreaWidgetContents")
        self.gridLayout_3 = QtGui.QGridLayout(self.scrollAreaWidgetContents)
        self.gridLayout_3.setSpacing(0)
        self.gridLayout_3.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_3.setObjectName("gridLayout_3")
        self.gridLayout_2 = QtGui.QGridLayout()
        self.gridLayout_2.setSpacing(0)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.gridLayout_3.addLayout(self.gridLayout_2, 0, 0, 1, 1)
        spacerItem = QtGui.QSpacerItem(278, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.gridLayout_3.addItem(spacerItem, 0, 1, 1, 1)
        spacerItem1 = QtGui.QSpacerItem(20, 332, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.gridLayout_3.addItem(spacerItem1, 1, 0, 1, 1)
        self.scrollArea.setWidget(self.scrollAreaWidgetContents)
        self.gridLayout.addWidget(self.scrollArea, 0, 0, 1, 1)

        self.retranslateUi(widgetBlock)
        QtCore.QMetaObject.connectSlotsByName(widgetBlock)

    def retranslateUi(self, widgetBlock):
        pass

