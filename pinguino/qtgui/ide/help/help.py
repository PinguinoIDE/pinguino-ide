#!/usr/bin/env python
#-*- coding: utf-8 -*-

from PySide import QtWebKit
from PySide import QtCore, QtGui
import codecs


########################################################################
class Help(object):
    """"""

    #----------------------------------------------------------------------
    def __init__(self):
        """Constructor"""

        self.connect(self.main.actionPinguino_ide_manual, QtCore.SIGNAL("triggered()"), self.pinguino_ide_manual)
        self.connect(self.main.actionLibraries_help, QtCore.SIGNAL("triggered()"), self.help_libraries)


    #----------------------------------------------------------------------
    def help_libraries(self):
        """"""
        # self.switch_ide_mode(graphical=False)
        self.add_tab("Libraries")
        self.web_view.load("pinguino/docs/index_pinguino/index.html")


    #----------------------------------------------------------------------
    def pinguino_ide_manual(self):
        """"""
        # self.switch_ide_mode(graphical=False)
        self.add_tab("Help")
        self.web_view.load("pinguino/docs/index/index.html")


    #----------------------------------------------------------------------
    def add_tab(self, name):
        tab = QtGui.QWidget()
        setattr(tab, "html", True)
        gridLayout = QtGui.QGridLayout(tab)
        gridLayout.setSpacing(0)
        gridLayout.setContentsMargins(0, 0, 0, 0)
        gridLayout.setObjectName("gridLayout")
        self.web_view = QtWebKit.QWebView(tab)
        gridLayout.addWidget(self.web_view, 0, 0, 1, 1)
        self.main.tabWidget_files.addTab(tab, name)
        self.main.tabWidget_files.setCurrentWidget(tab)

