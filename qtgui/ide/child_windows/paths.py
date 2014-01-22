#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

from PySide import QtGui, QtCore

from ..helpers.widgets_features import PrettyFeatures
from ..helpers.dialogs import Dialogs
from ...frames.paths import Ui_Paths

########################################################################
class Paths(QtGui.QMainWindow):
    """"""
    def __init__(self, parent):
        #QtGui.QMainWindow.__init__(self)
        super(Paths, self).__init__()
        #self.setWindowFlags(QtCore.Qt.WindowCloseButtonHint |
                            #QtCore.Qt.WindowSystemMenuHint |
                            #QtCore.Qt.WindowStaysOnTopHint)        
        
        self.set_paths = Ui_Paths()
        self.set_paths.setupUi(self)
        self.main = parent
        
        self.dialog_file = ((self.set_paths.lineEdit_gcc_bin, self.set_paths.pushButton_gcc_bin, "GCC_BIN"),
                            (self.set_paths.lineEdit_sdcc_bin, self.set_paths.pushButton_sdcc_bin, "SDCC_BIN"),
                            )
        
        self.dialog_dirs = (#(self.set_paths.lineEdit_gcc_libs, self.set_paths.pushButton_gcc_libs, "GCC_LIBS"),
                            #(self.set_paths.lineEdit_sdcc_libs, self.set_paths.pushButton_sdcc_libs, "SDCC_LIBS"),
                            #(self.set_paths.lineEdit_pinguino_config, self.set_paths.pushButton_pinguino_config, "PINGUINO_CONFIG"),
                            (self.set_paths.lineEdit_pinguino_8_libs, self.set_paths.pushButton_pinguino_8_libs, "PINGUINO_8_LIBS"),
                            (self.set_paths.lineEdit_pinguino_32_libs, self.set_paths.pushButton_pinguino_32_libs, "PINGUINO_32_LIBS"),
                            )
        
        default_paths = {"SDCC_BIN": self.main.configIDE.get_path("sdcc_bin"),
                         "GCC_BIN": self.main.configIDE.get_path("gcc_bin"),
                         "PINGUINO_8_LIBS": self.main.configIDE.get_path("pinguino_8_libs"),
                         "PINGUINO_32_LIBS": self.main.configIDE.get_path("pinguino_32_libs"),
                         }
        
        for lineEdit, pushButton, keyWord in self.dialog_file:
            self.connect(pushButton, QtCore.SIGNAL("clicked()"), self.set_file_dialog(lineEdit, keyWord))
            
        for lineEdit, pushButton, keyWord in self.dialog_dirs:
            self.connect(pushButton, QtCore.SIGNAL("clicked()"), self.set_dir_dialog(lineEdit, keyWord))         
        
        for lineEdit, pushButton, keyWord in self.dialog_file + self.dialog_dirs:
            lineEdit.setText(self.main.configIDE.config("Paths", keyWord, default_paths[keyWord]))
            lineEdit.setStyleSheet("")
        
        self.connect(self.set_paths.pushButton_close, QtCore.SIGNAL("clicked()"), self.close)
        
        #self.update_comobox_theme()        
        #self.connect(self.set_paths.comboBox, QtCore.SIGNAL("currentIndexChanged(int)"), self.change_theme)
        
        self.set_paths.pushButton_close.setFocus()
        self.centrar()
        
        Dialogs.warning_message(self, "This paths are very important don't try to edit it if you don't know what are you doing.")
        
    #----------------------------------------------------------------------
    def close(self):
        """"""
        if Dialogs.confirm_message(self, "Do you really want save these paths?."):
            for lineEdit, pushButton, keyWord in self.dialog_file + self.dialog_dirs:
                content = lineEdit.text()
                if content.isspace(): content = ""
                if not os.path.exists(content) and content:
                    Dialogs.error_message(self, keyWord + ": '" + content + "'\nThis path not exist.")
                    return
                else: self.main.configIDE.set("Paths", keyWord, content)  
        self.main.configIDE.save_config()
        self.main.update_pinguino_paths()
        super(Paths, self).close()

    #----------------------------------------------------------------------
    def set_file_dialog(self, lineEdit, KeyWord):
        """"""
        def dummy_func():
            path = lineEdit.text()
            if not os.path.isfile(path):
                path = QtCore.QDir.home().path()
            
            filename = QtGui.QFileDialog.getOpenFileName(self,
                    "Select file",
                    path, 
                    "All Files (*)")
            
            if filename:
                filename = filename[0]
                if filename:
                    lineEdit.setText(filename)
                    lineEdit.setStyleSheet("")     
                
        return dummy_func
                
    #----------------------------------------------------------------------
    def set_dir_dialog(self, lineEdit, KeyWord):
        """"""
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
        
    #----------------------------------------------------------------------
    def centrar(self):
        screen = QtGui.QDesktopWidget().screenGeometry()
        size =  self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)
        
        
    ##----------------------------------------------------------------------
    #def change_theme(self, index):
        #""""""
        #items = self.all_themes.keys()
        #items.sort()
        
        #QtGui.QIcon.setThemeName(items[index])
        #self.main.configIDE.set("THEME", "theme_name", items[index])
        
        
    ##----------------------------------------------------------------------
    #def update_comobox_theme(self):
        #""""""
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