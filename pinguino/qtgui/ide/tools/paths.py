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


        #self.OPTIMIZATION = "-O2 -O3 -Os".split()
        #self.main.comboBox_optimization.clear()
        #self.main.comboBox_optimization.addItems(self.OPTIMIZATION)
        #self.init_groups()


        #self.connect(self.main.pushButton_paths_set_default, QtCore.SIGNAL("clicked()"), self.set_default_values)


        self.connect(self.main.lineEdit_path_sdcc_bin, QtCore.SIGNAL("editingFinished()"), self.save_paths)
        self.connect(self.main.lineEdit_path_gcc_bin, QtCore.SIGNAL("editingFinished()"), self.save_paths)
        self.connect(self.main.lineEdit_path_xc8_bin, QtCore.SIGNAL("editingFinished()"), self.save_paths)
        self.connect(self.main.lineEdit_path_8_libs, QtCore.SIGNAL("editingFinished()"), self.save_paths)
        self.connect(self.main.lineEdit_path_32_libs, QtCore.SIGNAL("editingFinished()"), self.save_paths)





    #----------------------------------------------------------------------
    def save_paths(self):
        """"""
        sdcc_bin = self.main.lineEdit_path_sdcc_bin.text()
        gcc_bin = self.main.lineEdit_path_gcc_bin.text()
        xc8_bin = self.main.lineEdit_path_xc8_bin.text()
        p8libs = self.main.lineEdit_path_8_libs.text()
        p32libs = self.main.lineEdit_path_32_libs.text()

        self.configIDE.set("Paths", "sdcc_bin", sdcc_bin)
        self.configIDE.set("Paths", "gcc_bin", gcc_bin)
        self.configIDE.set("Paths", "xc8_bin", xc8_bin)
        self.configIDE.set("Paths", "pinguino_8_libs", p8libs)
        self.configIDE.set("Paths", "pinguino_32_libs", p32libs)

        self.configIDE.save_config()



    #----------------------------------------------------------------------
    def load_paths(self, config=None):
        """"""

        if config is None:
            self.configIDE.load_config()
            config = self.configIDE

        sdcc_bin = config.get("Paths", "sdcc_bin")
        gcc_bin = config.get("Paths", "gcc_bin")
        xc8_bin = config.get("Paths", "xc8_bin")
        p8libs = config.get("Paths", "pinguino_8_libs")
        p32libs = config.get("Paths", "pinguino_32_libs")


        self.main.lineEdit_path_sdcc_bin.setText(sdcc_bin)
        self.main.lineEdit_path_gcc_bin.setText(gcc_bin)
        self.main.lineEdit_path_xc8_bin.setText(xc8_bin)
        self.main.lineEdit_path_8_libs.setText(p8libs)
        self.main.lineEdit_path_32_libs.setText(p32libs)



    ##----------------------------------------------------------------------
    #def set_default_values(self):
        #""""""
        #config_paths = RawConfigParser()
        #config_paths.readfp(open(os.path.join(os.getenv("PINGUINO_LIB"), "qtgui", "config", "pinguino.{PINGUINO_OS_NAME}.conf".format(**os.environ)), "r"))
        #self.load_paths(config_paths)

        #self.save_paths()






