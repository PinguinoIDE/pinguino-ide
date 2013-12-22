# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/Pinguino/pinguino-ide/qtgui/frames/wiki_doc_widget.ui'
#
# Created: Sun Dec 22 10:37:12 2013
#      by: pyside-uic 0.2.14 running on PySide 1.1.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Form_wiki(object):
    def setupUi(self, Form_wiki):
        Form_wiki.setObjectName("Form_wiki")
        Form_wiki.resize(575, 337)
        self.gridLayout = QtGui.QGridLayout(Form_wiki)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setObjectName("gridLayout")
        self.textBrowser_doc = QtGui.QTextBrowser(Form_wiki)
        self.textBrowser_doc.setTextInteractionFlags(QtCore.Qt.LinksAccessibleByKeyboard|QtCore.Qt.LinksAccessibleByMouse|QtCore.Qt.TextBrowserInteraction|QtCore.Qt.TextSelectableByKeyboard|QtCore.Qt.TextSelectableByMouse)
        self.textBrowser_doc.setOpenExternalLinks(True)
        self.textBrowser_doc.setObjectName("textBrowser_doc")
        self.gridLayout.addWidget(self.textBrowser_doc, 1, 0, 1, 2)

        self.retranslateUi(Form_wiki)
        QtCore.QMetaObject.connectSlotsByName(Form_wiki)

    def retranslateUi(self, Form_wiki):
        Form_wiki.setWindowTitle(QtGui.QApplication.translate("Form_wiki", "Form", None, QtGui.QApplication.UnicodeUTF8))

