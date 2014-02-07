# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/.virtualenvs/pinguino_env/pinguino/pinguino-ide/qtgui/frames/wiki_doc_widget.ui'
#
# Created: Thu Feb  6 21:00:53 2014
#      by: pyside-uic 0.2.15 running on PySide 1.2.1
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_WikiDocs(object):
    def setupUi(self, WikiDocs):
        WikiDocs.setObjectName("WikiDocs")
        WikiDocs.resize(817, 454)
        WikiDocs.setWindowTitle("Wiki documentation")
        self.centralwidget = QtGui.QWidget(WikiDocs)
        self.centralwidget.setObjectName("centralwidget")
        self.gridLayout_2 = QtGui.QGridLayout(self.centralwidget)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.tabWidget = QtGui.QTabWidget(self.centralwidget)
        self.tabWidget.setTabsClosable(True)
        self.tabWidget.setObjectName("tabWidget")
        self.tab = QtGui.QWidget()
        self.tab.setObjectName("tab")
        self.gridLayout = QtGui.QGridLayout(self.tab)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.textBrowser_doc = QtGui.QTextBrowser(self.tab)
        self.textBrowser_doc.setFrameShape(QtGui.QFrame.NoFrame)
        self.textBrowser_doc.setTextInteractionFlags(QtCore.Qt.LinksAccessibleByKeyboard|QtCore.Qt.LinksAccessibleByMouse|QtCore.Qt.TextBrowserInteraction|QtCore.Qt.TextSelectableByKeyboard|QtCore.Qt.TextSelectableByMouse)
        self.textBrowser_doc.setOpenExternalLinks(True)
        self.textBrowser_doc.setObjectName("textBrowser_doc")
        self.gridLayout.addWidget(self.textBrowser_doc, 0, 0, 1, 1)
        self.tabWidget.addTab(self.tab, "")
        self.gridLayout_2.addWidget(self.tabWidget, 0, 0, 1, 1)
        WikiDocs.setCentralWidget(self.centralwidget)

        self.retranslateUi(WikiDocs)
        self.tabWidget.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(WikiDocs)

    def retranslateUi(self, WikiDocs):
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), QtGui.QApplication.translate("WikiDocs", "Libraries", None, QtGui.QApplication.UnicodeUTF8))

