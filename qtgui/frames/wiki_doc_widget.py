# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/yeison/Documentos/Desarrollo/Pinguino/GitHub/pinguino-ide/qtgui/frames/wiki_doc_widget.ui'
#
# Created: Wed Apr  8 03:20:58 2015
#      by: pyside-uic 0.2.15 running on PySide 1.2.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_WikiDocs(object):
    def setupUi(self, WikiDocs):
        WikiDocs.setObjectName("WikiDocs")
        WikiDocs.resize(817, 482)
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
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setObjectName("gridLayout")
        self.textBrowser_doc = QtGui.QTextBrowser(self.tab)
        self.textBrowser_doc.setFrameShape(QtGui.QFrame.NoFrame)
        self.textBrowser_doc.setTextInteractionFlags(QtCore.Qt.LinksAccessibleByKeyboard|QtCore.Qt.LinksAccessibleByMouse|QtCore.Qt.TextBrowserInteraction|QtCore.Qt.TextSelectableByKeyboard|QtCore.Qt.TextSelectableByMouse)
        self.textBrowser_doc.setOpenExternalLinks(True)
        self.textBrowser_doc.setObjectName("textBrowser_doc")
        self.gridLayout.addWidget(self.textBrowser_doc, 0, 0, 1, 1)
        self.tabWidget.addTab(self.tab, "")
        self.gridLayout_2.addWidget(self.tabWidget, 0, 0, 1, 1)
        self.label_error = QtGui.QLabel(self.centralwidget)
        self.label_error.setStyleSheet("color: rgb(255, 0, 0);")
        self.label_error.setText("")
        self.label_error.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.label_error.setObjectName("label_error")
        self.gridLayout_2.addWidget(self.label_error, 1, 0, 1, 1)
        self.progressBar = QtGui.QProgressBar(self.centralwidget)
        self.progressBar.setProperty("value", 24)
        self.progressBar.setObjectName("progressBar")
        self.gridLayout_2.addWidget(self.progressBar, 2, 0, 1, 1)
        WikiDocs.setCentralWidget(self.centralwidget)

        self.retranslateUi(WikiDocs)
        self.tabWidget.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(WikiDocs)

    def retranslateUi(self, WikiDocs):
        WikiDocs.setWindowTitle(QtGui.QApplication.translate("WikiDocs", "Wiki documentation", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), QtGui.QApplication.translate("WikiDocs", "Libraries", None, QtGui.QApplication.UnicodeUTF8))

