#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import shutil
import re
from ConfigParser import RawConfigParser

from PySide import QtGui, QtCore

from ..helpers.config_libs import ConfigLibs
from ..helpers.constants import TAB_NAME, IDE_LIBRARY_INSTALLED
from ..helpers.widgets_features import PrettyFeatures
from ..helpers.dialogs import Dialogs
from ...frames.libraries_widget import Ui_LibraryManager

from ..repositories.git_repo import GitRepo

########################################################################
class LibManager(QtGui.QMainWindow):
    """"""
    def __init__(self, IDE):
        super(LibManager, self).__init__()
        #self.setWindowFlags(QtCore.Qt.WindowCloseButtonHint |
                            #QtCore.Qt.WindowSystemMenuHint |
                            #QtCore.Qt.WindowStaysOnTopHint)        
        
    
        self.libframe = Ui_LibraryManager()
        self.libframe.setupUi(self)
        
        #self.libframe.progressBar_install.setVisible(False)
        
        self.ConfigLibs = ConfigLibs()
        

        #self.GitRepo = GitRepo()       
        
        self.setWindowTitle(TAB_NAME+" - Library manager")
        
        self.connect(self.libframe.pushButton_add, QtCore.SIGNAL("clicked()"), self.add_source)
        self.connect(self.libframe.lineEdit_source, QtCore.SIGNAL("editingFinished()"), self.add_source)
        self.connect(self.libframe.tableWidget_sources, QtCore.SIGNAL("itemClicked(QTableWidgetItem*)"), self.handleItemClicked_source)
        self.connect(self.libframe.tableWidget_libs, QtCore.SIGNAL("itemClicked(QTableWidgetItem*)"), self.handleItemClicked_libs)
        
        
        #self.connect(self.libframe.pushButton_apply, QtCore.SIGNAL("clicked()"), self.remove_libs)   
        self.connect(self.libframe.pushButton_update, QtCore.SIGNAL("clicked()"), self.update_install_libs)
        self.connect(self.libframe.pushButton_close, QtCore.SIGNAL("clicked()"), self.close_manager)
            
                
        self.default_line_edit_source = "e.g. git@github.com:repo/pinguinolib-library.git"
        PrettyFeatures.LineEdit_default_text(self.libframe, self.libframe.lineEdit_source, self.default_line_edit_source)
        
        
        bg_color = IDE.configIDE.config("Styles", "background_color", "#FFFFFF")
        alternate_bg_color = IDE.configIDE.config("Styles", "alternate_background_color", "#DDE8FF")        
        self.libframe.tableWidget_libs.setStyleSheet("QTableWidget {background-color: %s;\nalternate-background-color %s);}"%(bg_color, alternate_bg_color))
        self.libframe.tableWidget_sources.setStyleSheet("QTableWidget {background-color: %s;\nalternate-background-color: %s;}"%(bg_color, alternate_bg_color))
        
        
        
        #self.libframe.tableWidget_libs.setStyleSheet("QTableWidget {background-color: rgb(255, 255, 222);\nalternate-background-color: rgb(255, 255, 191);}")
        #self.libframe.tableWidget_sources.setStyleSheet("QTableWidget {background-color: rgb(255, 255, 222);\nalternate-background-color: rgb(255, 255, 191);}")
        
        self.handleItemClicked_source_count = 0
        
        self.update_sources_view()
        self.update_libraries_view()
        self.centrar()
        
        if not os.path.isdir(IDE_LIBRARY_INSTALLED): os.mkdir(IDE_LIBRARY_INSTALLED)
        

    #----------------------------------------------------------------------
    def centrar(self):
        screen = QtGui.QDesktopWidget().screenGeometry()
        size =  self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)
        

    #----------------------------------------------------------------------
    def close_manager(self):
        """"""
        self.ConfigLibs.save_config()
        self.close()
        
        
    #----------------------------------------------------------------------
    def add_source(self, source=None):
        """"""
        if not source:
            source = self.libframe.lineEdit_source.text()
            if source == self.default_line_edit_source: return
    
        if self.ConfigLibs.check_duplicated(source):
            Dialogs.error_message(self, source+"\n already exist.")
            return
            
        name= self.get_name_from_source(source)
        if not name: return
        
        self.ConfigLibs.set(name, "repository", source)
        self.ConfigLibs.set(name, "name", name)
        self.ConfigLibs.save_config()
        
        index = self.libframe.tableWidget_sources.rowCount()
        self.libframe.tableWidget_sources.setRowCount(index+1)
        
        item = QtGui.QTableWidgetItem()
        self.libframe.tableWidget_sources.setVerticalHeaderItem(index, item) 
        
        checkable = QtGui.QTableWidgetItem()
        checkable.setCheckState(QtCore.Qt.Checked)
        
        self.libframe.tableWidget_sources.setItem(index, 0, checkable)
        self.libframe.tableWidget_sources.setItem(index, 1, QtGui.QTableWidgetItem())
        
        self.libframe.tableWidget_sources.item(index, 0).setText(name)
        self.libframe.tableWidget_sources.item(index, 1).setText(source)
        
        self.libframe.pushButton_update.setEnabled(True)
        
        
    #----------------------------------------------------------------------
    def update_sources_view(self):
        """"""
        sources = self.ConfigLibs.get_all_sources()
        
        index = 0
        self.libframe.tableWidget_sources.setRowCount(0)
        
        for key in sources.keys():
            
            name = key
            repo = sources[key]["repository"]
            
            
            self.libframe.tableWidget_sources.setRowCount(self.libframe.tableWidget_sources.rowCount()+1)            
            
            
            item = QtGui.QTableWidgetItem()
            self.libframe.tableWidget_sources.setVerticalHeaderItem(index, item)             
            
            checkable = QtGui.QTableWidgetItem()
            checkable.setFlags(QtCore.Qt.ItemIsEnabled|QtCore.Qt.ItemIsUserCheckable)
            checkable.setCheckState(QtCore.Qt.Unchecked)
            self.libframe.tableWidget_sources.setItem(index, 0, checkable)
            
            repository = QtGui.QTableWidgetItem()
            repository.setFlags(QtCore.Qt.ItemIsEnabled)
            self.libframe.tableWidget_sources.setItem(index, 1, repository)
            
            self.libframe.tableWidget_sources.item(index, 0).setText(name)
            self.libframe.tableWidget_sources.item(index, 1).setText(repo)
            
            index += 1
            
            
    #----------------------------------------------------------------------
    def update_libraries_view(self):
        """"""
        self.ConfigLibs.save_config()
        self.ConfigLibs.load_config()
        sources = self.ConfigLibs.get_all_sources()
        
        index = 0
        self.libframe.tableWidget_libs.setRowCount(0)
        
        for key in sources.keys():
            
            name = key
            repo = sources[key]["repository"]
            installed = sources[key]["installed"]
            arch = sources[key]["arch"]
            author = sources[key]["author"]
            description = sources[key]["description"]
            active = sources[key]["active"]
            
            if not installed: continue
            self.libframe.tableWidget_libs.setRowCount(self.libframe.tableWidget_libs.rowCount()+1)
                
            checkable = QtGui.QTableWidgetItem()
            checkable.setFlags(QtCore.Qt.ItemIsEnabled|QtCore.Qt.ItemIsUserCheckable)
            checkable.setCheckState(QtCore.Qt.Unchecked)
            self.libframe.tableWidget_libs.setItem(index, 0, checkable)
            
            arch_ = QtGui.QTableWidgetItem()
            arch_.setFlags(QtCore.Qt.ItemIsEnabled)            
            self.libframe.tableWidget_libs.setItem(index, 1, arch_)
            
            auth = QtGui.QTableWidgetItem()
            auth.setFlags(QtCore.Qt.ItemIsEnabled)            
            self.libframe.tableWidget_libs.setItem(index, 2, auth)

            desc = QtGui.QTableWidgetItem()
            desc.setFlags(QtCore.Qt.ItemIsEnabled)      
            self.libframe.tableWidget_libs.setItem(index, 3, desc)
            
            self.libframe.tableWidget_libs.item(index, 0).setText(name)
            self.libframe.tableWidget_libs.item(index, 1).setText(arch)  
            self.libframe.tableWidget_libs.item(index, 2).setText(author)  
            self.libframe.tableWidget_libs.item(index, 3).setText(description)

            index += 1
        
        

    #----------------------------------------------------------------------
    def get_name_from_source(self, source):
        """"""
        match = re.match(".*/([\w-]*)(.git)", source)
        
        if match:
            name = match.groups()[0]
            Dialogs.info_message(self, name+" from "+source+"\nsuccessfully added.")
            return name
        else:
            Dialogs.error_message(self, source+" is not recognized as a valid source.")
            return False
        
        
    #----------------------------------------------------------------------
    def build_menu(self, event):
        """"""
        menu = QtGui.QMenu()
        if self.isSelecting:
            if self.getUnderSelection():
                menu.addAction("Delete selected blocks", self.dele_blocks)
            menu.addAction("Take Screenshot from selected area", lambda :None)
            menu.addAction("Export code to pinguino editor", lambda :None)
        menu.exec_(event.globalPos())
        
        
        
    #----------------------------------------------------------------------
    def handleItemClicked_source(self, item):
        """"""
        for index in range(self.libframe.tableWidget_sources.rowCount()):
            if self.libframe.tableWidget_sources.item(index, 0).checkState() == QtCore.Qt.Checked:
                self.libframe.pushButton_update.setEnabled(True)
                return
        self.libframe.pushButton_update.setEnabled(False)
        
        
    #----------------------------------------------------------------------
    def handleItemClicked_libs(self, item):
        """"""
        #for index in range(self.libframe.tableWidget_libs.rowCount()):
            #if self.libframe.tableWidget_libs.item(index, 0).checkState() == QtCore.Qt.Checked:
        self.libframe.pushButton_apply.setEnabled(True)
                #return
        #self.libframe.pushButton_uninstall.setEnabled(False)
        
        
        
    #----------------------------------------------------------------------
    def update_install_libs(self):
        """"""
        self.setCursor(QtCore.Qt.WaitCursor)
        
        sources = self.ConfigLibs.get_all_sources()
        selected = []
        
        for index in range(self.libframe.tableWidget_sources.rowCount()):
            if self.libframe.tableWidget_sources.item(index, 0).checkState() == QtCore.Qt.Checked:
                selected.append(self.libframe.tableWidget_sources.item(index, 0).text())
                
        
        work = []
        for sel in selected:
            if GitRepo.check_library(sel):
                try:
                    GitRepo.update_library(sel)
                except:
                    if Dialogs.confirm_message(self, "Error", "Problems with "+sel+".\nTry again?"):
                        shutil.rmtree(os.path.join(IDE_LIBRARY_INSTALLED, sel))
                    try:
                        GitRepo.install_library(sel)
                    except:
                        Dialogs.info_message(self, "Problems with "+sel+".")
                work.append(sel + ": Updated")
                
            else:
                try:
                    GitRepo.install_library(sel)
                except:
                    if Dialogs.confirm_message(self, "Error", "Problems with "+sel+".\nTry again?"):
                        shutil.rmtree(os.path.join(IDE_LIBRARY_INSTALLED, sel))
                    try:
                        GitRepo.install_library(sel)
                    except:
                        Dialogs.info_message(self, "Problems with "+sel+".")
                work.append(sel + ": Installed")
                
                
            if not self.update_lib_config(sel):
                work.pop(-1)
                work.append(sel + ": Failed")
                
                
        self.setCursor(QtCore.Qt.ArrowCursor)
            
        Dialogs.info_message(self, "\n".join(work))
        self.update_libraries_view()
        
        
    #----------------------------------------------------------------------
    def update_lib_config(self, lib):
        """"""
        lib_data = RawConfigParser()
        filename = os.path.join(IDE_LIBRARY_INSTALLED, lib, "PINGUINO")
        
        if os.path.exists(filename):
            lib_data.readfp(file(filename, "r"))
            for arg in "arch author description url repository name".split():
                self.ConfigLibs.set(lib, arg, lib_data.get("library", arg))
            self.ConfigLibs.set(lib, "installed", True)
            self.ConfigLibs.set(lib, "active", False)
            self.ConfigLibs.save_config()
            return True
        else:
            Dialogs.info_message(self, lib+" has no valid configuration file.\n\nmissing "+filename)
            shutil.rmtree(os.path.join(IDE_LIBRARY_INSTALLED, lib))
            return False