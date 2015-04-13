#! /usr/bin/python2
#-*- coding: utf-8 -*-

from PySide import QtWebKit
from PySide import QtCore, QtGui


########################################################################
class Help(object):
    """"""

    #----------------------------------------------------------------------
    def __init__(self):
        """Constructor"""


    #----------------------------------------------------------------------
    def help(self):
        """"""
        self.home_help = self.add_tab("Help")

        bootstrap = self.get_bootstrap()
        html = self.get_html_page("help").replace("{bootstrap}", bootstrap)
        self.home_help.setHtml(html)


    #----------------------------------------------------------------------
    def add_tab(self, name):
        tab = QtGui.QWidget()
        gridLayout = QtGui.QGridLayout(tab)
        gridLayout.setSpacing(0)
        gridLayout.setContentsMargins(0, 0, 0, 0)
        gridLayout.setObjectName("gridLayout")
        web_view = QtWebKit.QWebView(tab)
        gridLayout.addWidget(web_view, 0, 0, 1, 1)
        self.main.tabWidget_files.addTab(tab, name)
        self.main.tabWidget_files.setCurrentWidget(tab)
        return web_view


    #----------------------------------------------------------------------
    def get_bootstrap(self):

        s = QtCore.QFile(":/bootstrap/bootstrap/css/bootstrap.min.css")
        s.open(QtCore.QIODevice.ReadOnly|QtCore.QIODevice.Text)
        content = str(s.readAll())
        return content


    #----------------------------------------------------------------------
    def get_html_page(self, name):

        s = QtCore.QFile(":/html/html/%s.html" % name)
        s.open(QtCore.QIODevice.ReadOnly|QtCore.QIODevice.Text)
        content = str(s.readAll())
        return content

