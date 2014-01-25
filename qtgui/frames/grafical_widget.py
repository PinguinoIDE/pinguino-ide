# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/.virtualenvs/pinguino_env/pinguino/pinguino-ide/qtgui/frames/grafical_widget.ui'
#
# Created: Sat Jan 25 17:07:45 2014
#      by: pyside-uic 0.2.15 running on PySide 1.2.1
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Form_graphical(object):
    def setupUi(self, Form_graphical):
        Form_graphical.setObjectName("Form_graphical")
        Form_graphical.resize(450, 406)
        Form_graphical.setWindowTitle("Graphical Widget")
        self.gridLayout = QtGui.QGridLayout(Form_graphical)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.scrollArea = QtGui.QScrollArea(Form_graphical)
        self.scrollArea.setFrameShape(QtGui.QFrame.NoFrame)
        self.scrollArea.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOn)
        self.scrollArea.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOn)
        self.scrollArea.setWidgetResizable(True)
        self.scrollArea.setObjectName("scrollArea")
        self.scrollAreaWidgetContents = QtGui.QWidget()
        self.scrollAreaWidgetContents.setGeometry(QtCore.QRect(0, 0, 437, 393))
        self.scrollAreaWidgetContents.setObjectName("scrollAreaWidgetContents")
        self.gridLayout_3 = QtGui.QGridLayout(self.scrollAreaWidgetContents)
        self.gridLayout_3.setSpacing(0)
        self.gridLayout_3.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_3.setObjectName("gridLayout_3")
        self.widget = QtGui.QWidget(self.scrollAreaWidgetContents)
        self.widget.setObjectName("widget")
        self.gridLayout_3.addWidget(self.widget, 0, 0, 1, 1)
        self.scrollArea.setWidget(self.scrollAreaWidgetContents)
        self.gridLayout.addWidget(self.scrollArea, 0, 0, 1, 1)

        self.retranslateUi(Form_graphical)
        QtCore.QMetaObject.connectSlotsByName(Form_graphical)

    def retranslateUi(self, Form_graphical):
        pass

