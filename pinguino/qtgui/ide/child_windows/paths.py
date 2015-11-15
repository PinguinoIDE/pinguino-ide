#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import sys

from PySide import QtGui, QtCore

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from configparser import RawConfigParser
else:
    #Python2
    from ConfigParser import RawConfigParser


from ..methods.dialogs import Dialogs
from ...frames.paths import Ui_Paths
from ...pinguino_core.pinguino_config import PinguinoConfig


########################################################################
class Paths(QtGui.QDialog):

    def __init__(self, parent):
        super(Paths, self).__init__()

        self.set_paths = Ui_Paths()
        self.set_paths.setupUi(self)
        self.main = parent

        self.setWindowTitle(os.getenv("PINGUINO_NAME")+" - "+self.windowTitle())

        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/logo/art/windowIcon.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.setWindowIcon(icon)

        self.dialog_dirs = (
                            #(self.set_paths.lineEdit_sdcc_bin, self.set_paths.pushButton_sdcc_bin, "SDCC_BIN"),
                            #(self.set_paths.lineEdit_gcc_bin, self.set_paths.pushButton_gcc_bin, "GCC_BIN"),
                            (self.set_paths.lineEdit_pinguino_8_libs, self.set_paths.pushButton_pinguino_8_libs, "PINGUINO_8_LIBS"),
                            (self.set_paths.lineEdit_pinguino_32_libs, self.set_paths.pushButton_pinguino_32_libs, "PINGUINO_32_LIBS"),
                            )

        default_paths = {
                         #"SDCC_BIN": self.main.configIDE.get_path("sdcc_bin"),
                         #"GCC_BIN": self.main.configIDE.get_path("gcc_bin"),
                         "PINGUINO_8_LIBS": self.main.configIDE.get_path("pinguino_8_libs"),
                         "PINGUINO_32_LIBS": self.main.configIDE.get_path("pinguino_32_libs"),
                         }

        #for lineEdit, pushButton, keyWord in self.dialog_file:
            #self.connect(pushButton, QtCore.SIGNAL("clicked()"), self.set_file_dialog(lineEdit))

        for lineEdit, pushButton, keyWord in self.dialog_dirs:
            self.connect(pushButton, QtCore.SIGNAL("clicked()"), self.set_dir_dialog(lineEdit))

        for lineEdit, pushButton, keyWord in self.dialog_dirs:
            lineEdit.setText(self.main.configIDE.config("Paths", keyWord, default_paths[keyWord]))
            #lineEdit.setStyleSheet("")

        self.connect(self.set_paths.pushButton_close, QtCore.SIGNAL("clicked()"), self.close)
        #self.connect(self.set_paths.pushButton_clear_sdcc_bin, QtCore.SIGNAL("clicked()"), lambda :self.reset_value("sdcc_bin"))
        self.connect(self.set_paths.pushButton_clear_8_libs, QtCore.SIGNAL("clicked()"), lambda :self.reset_value("pinguino_8_libs"))
        #self.connect(self.set_paths.pushButton_clear_gcc_bin, QtCore.SIGNAL("clicked()"), lambda :self.reset_value("gcc_bin"))
        self.connect(self.set_paths.pushButton_clear_32_libs, QtCore.SIGNAL("clicked()"), lambda :self.reset_value("pinguino_32_libs"))

        #self.update_comobox_theme()
        #self.connect(self.set_paths.comboBox, QtCore.SIGNAL("currentIndexChanged(int)"), self.change_theme)

        self.set_paths.pushButton_close.setFocus()

        self.setStyleSheet("""
        font-family: inherit;
        font-weight: normal;

        """)

        Dialogs.warning_message(self, QtGui.QApplication.translate("Dialogs", "This paths are very important don't try to edit it if you don't know what are you doing."))

        self.center_on_screen()


    #----------------------------------------------------------------------
    def center_on_screen(self):

        screen = QtGui.QDesktopWidget().screenGeometry()
        size = self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)


    #----------------------------------------------------------------------
    def close(self):

        if Dialogs.confirm_message(self, QtGui.QApplication.translate("Dialogs", "Do you really want save these paths?.")):
            for lineEdit, pushButton, keyWord in self.dialog_dirs:
                content = lineEdit.text()
                if content.isspace(): content = ""
                if not os.path.exists(content) and content:
                    Dialogs.error_message(self, keyWord + ": '" + content + "'\n"+QtGui.QApplication.translate("Dialogs", "This path not exist."))
                    return
                else: self.main.configIDE.set("Paths", keyWord, content)
        self.main.configIDE.save_config()
        #self.main.update_pinguino_paths()
        PinguinoConfig.update_pinguino_paths(self.main.configIDE, self.main.pinguinoAPI)
        super(Paths, self).close()

    ##----------------------------------------------------------------------
    #def set_file_dialog(self, lineEdit):

        #def dummy_func():
            #path = lineEdit.text()
            #if not os.path.isfile(path):
                #path = QtCore.QDir.home().path()

            #filename = QtGui.QFileDialog.getOpenFileName(self,
                    #"Select file",
                    #path,
                    #"All Files (*)")

            #if filename:
                #filename = filename[0]
                #if filename:
                    #lineEdit.setText(filename)
                    #lineEdit.setStyleSheet("")

        #return dummy_func

    #----------------------------------------------------------------------
    def set_dir_dialog(self, lineEdit):

        def dummy_func():
            path = lineEdit.text()
            if not os.path.isdir(path):
                path = QtCore.QDir.home().path()
            dirname = QtGui.QFileDialog.getExistingDirectory(self,
                      "Select directory",
                      path)

            if dirname:
                lineEdit.setText(dirname)
                lineEdit.setStyleSheet("")
        return dummy_func


    ##----------------------------------------------------------------------
    #def change_theme(self, index):
        #
        #items = self.all_themes.keys()
        #items.sort()

        #QtGui.QIcon.setThemeName(items[index])
        #self.main.configIDE.set("THEME", "theme_name", items[index])


    ##----------------------------------------------------------------------
    #def update_comobox_theme(self):
        #
        #current_theme = QtGui.QIcon.themeName()
        #theme_dirs = QtGui.QIcon.themeSearchPaths()
        #self.all_themes = {}
        #for dir_ in theme_dirs:
            #if os.path.exists(dir_):
                #for theme in os.listdir(dir_):
                    #self.all_themes[theme] = os.path.join(dir_, theme)

        #items = self.all_themes.keys()
        #items.sort()
        #self.set_paths.comboBox.addItems(items)

        #if current_theme in self.all_themes.keys():
            #self.set_paths.comboBox.setCurrentIndex(items.index(current_theme))


    #----------------------------------------------------------------------
    def reset_value(self, option):
        default = RawConfigParser()
        default.readfp(open(os.path.join(os.getenv("PINGUINO_LIB"), "qtgui", "config", "pinguino."+os.getenv("PINGUINO_OS_NAME")+".conf"), "r"))
        getattr(self.set_paths, "lineEdit_"+option).setText(default.get("Paths", option))

