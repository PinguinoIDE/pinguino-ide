#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import sys
import re

from PySide2 import QtGui, QtCore, QtWidgets

from ..methods.dialogs import Dialogs
from ...frames.library_template import Ui_LibraryTemplate


########################################################################
class LibraryTemplate(QtWidgets.QDialog):

    def __init__(self, parent):
        super(LibraryTemplate, self).__init__()

        self.lib_template = Ui_LibraryTemplate()
        self.lib_template.setupUi(self)
        self.main = parent

        self.setWindowTitle(os.getenv("PINGUINO_NAME")+" - "+self.windowTitle())

        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/logo/art/windowIcon.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.setWindowIcon(icon)

        self.lib_template.lineEdit_lib_author.setText(os.environ.get("USER", ""))

        self.connect(self.lib_template.pushButton_accept, QtCore.SIGNAL("clicked()"), self.accept)
        self.connect(self.lib_template.pushButton_cancel, QtCore.SIGNAL("clicked()"), self.close)
        self.connect(self.lib_template.pushButton_lib_path, QtCore.SIGNAL("clicked()"), self.set_directory)
        self.connect(self.lib_template.checkBox_lib32, QtCore.SIGNAL("clicked(bool)"), self.can_generate)
        self.connect(self.lib_template.checkBox_lib8, QtCore.SIGNAL("clicked(bool)"), self.can_generate)
        self.connect(self.lib_template.lineEdit_lib_name, QtCore.SIGNAL("textChanged(QString)"), self.can_generate)
        self.connect(self.lib_template.lineEdit_lib_path, QtCore.SIGNAL("textChanged(QString)"), self.can_generate)

        self.center_on_screen()


    #----------------------------------------------------------------------
    def center_on_screen(self):

        screen = QtGui.QDesktopWidget().screenGeometry()
        size = self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)


    #----------------------------------------------------------------------
    def set_directory(self):

        target_dir = Dialogs.set_open_dir(self.main)
        if target_dir:
            self.lib_template.lineEdit_lib_path.setText(target_dir)


    #----------------------------------------------------------------------
    def accept(self):

        author = self.lib_template.lineEdit_lib_author.text()
        libname = self.lib_template.lineEdit_lib_name.text()
        libpath = self.lib_template.lineEdit_lib_path.text()
        description = self.lib_template.plainTextEdit_lib_description.toPlainText()
        _8bit = self.lib_template.checkBox_lib8.isChecked()
        _32bit = self.lib_template.checkBox_lib32.isChecked()

        kwargs = locals().copy()
        kwargs.pop("self")
        self.main.new_library(**kwargs)
        self.close()


    #----------------------------------------------------------------------
    def can_generate(self, event):

        libname = self.lib_template.lineEdit_lib_name.text()
        libpath = self.lib_template.lineEdit_lib_path.text()
        _8bit = self.lib_template.checkBox_lib8.isChecked()
        _32bit = self.lib_template.checkBox_lib32.isChecked()

        if re.match("[a-zA-Z_]+[_0-9a-zA-Z]*", libname):
            self.lib_template.pushButton_accept.setEnabled(libname and os.path.isdir(libpath) and (_8bit or _32bit))
        else:
            self.lib_template.pushButton_accept.setEnabled(False)
