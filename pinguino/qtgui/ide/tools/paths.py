#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

from PySide import QtGui, QtCore


# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from configparser import RawConfigParser
else:
    #Python2
    from ConfigParser import RawConfigParser


########################################################################
class Paths(object):

    def __init__(self):
        """"""

        self.load_paths()

        self.connect(self.main.lineEdit_path_sdcc_bin, QtCore.SIGNAL("editingFinished()"), self.save_paths)
        self.connect(self.main.lineEdit_path_gcc_bin, QtCore.SIGNAL("editingFinished()"), self.save_paths)
        self.connect(self.main.lineEdit_path_xc8_bin, QtCore.SIGNAL("editingFinished()"), self.save_paths)
        self.connect(self.main.lineEdit_path_8_libs, QtCore.SIGNAL("editingFinished()"), self.save_paths)
        self.connect(self.main.lineEdit_path_32_libs, QtCore.SIGNAL("editingFinished()"), self.save_paths)

        self.connect(self.main.checkBox_paths_default, QtCore.SIGNAL("toggled(bool)"), self.set_default_values)





    #----------------------------------------------------------------------
    def save_paths(self):
        """"""
        sdcc_bin = self.main.lineEdit_path_sdcc_bin.text()
        gcc_bin = self.main.lineEdit_path_gcc_bin.text()
        xc8_bin = self.main.lineEdit_path_xc8_bin.text()
        p8libs = self.main.lineEdit_path_8_libs.text()
        p32libs = self.main.lineEdit_path_32_libs.text()

        #self.configIDE.set("Paths", "sdcc_bin", sdcc_bin)
        #self.configIDE.set("Paths", "gcc_bin", gcc_bin)
        #self.configIDE.set("Paths", "xc8_bin", xc8_bin)
        #self.configIDE.set("Paths", "pinguino_8_libs", p8libs)
        #self.configIDE.set("Paths", "pinguino_32_libs", p32libs)

        self.configIDE.set("PathsCustom", "sdcc_bin", sdcc_bin)
        self.configIDE.set("PathsCustom", "gcc_bin", gcc_bin)
        self.configIDE.set("PathsCustom", "xc8_bin", xc8_bin)
        self.configIDE.set("PathsCustom", "pinguino_8_libs", p8libs)
        self.configIDE.set("PathsCustom", "pinguino_32_libs", p32libs)

        self.configIDE.save_config()



    #----------------------------------------------------------------------
    def load_paths(self, section=None):
        """"""
        self.configIDE.load_config()
        if section is None:
            section = self.configIDE.config("Features", "pathstouse", "Paths")

        self.main.checkBox_paths_default.setChecked(section=="Paths")

        def short(section, option):
            if section == "Paths":
                return self.configIDE.get(section, option)
            elif section == "PathsCustom":
                return self.configIDE.config(section, option, self.configIDE.get("Paths", option))

        sdcc_bin = short(section, "sdcc_bin")
        gcc_bin = short(section, "gcc_bin")
        xc8_bin = short(section, "xc8_bin")
        p8libs = short(section, "pinguino_8_libs")
        p32libs = short(section, "pinguino_32_libs")

        self.main.lineEdit_path_sdcc_bin.setText(sdcc_bin)
        self.main.lineEdit_path_gcc_bin.setText(gcc_bin)
        self.main.lineEdit_path_xc8_bin.setText(xc8_bin)
        self.main.lineEdit_path_8_libs.setText(p8libs)
        self.main.lineEdit_path_32_libs.setText(p32libs)



    #----------------------------------------------------------------------
    def set_default_values(self, default):
        """"""

        self.configIDE.load_config()

        if default:
            self.load_paths(section="Paths")
            self.configIDE.set("Features", "pathstouse", "Paths")

        else:
            self.load_paths(section="PathsCustom")
            self.configIDE.set("Features", "pathstouse", "PathsCustom")

        self.configIDE.save_config()


