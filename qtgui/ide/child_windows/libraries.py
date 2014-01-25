#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import shutil
import re
from zipfile import ZipFile
from ConfigParser import RawConfigParser
import webbrowser

from PySide import QtGui, QtCore

from ..methods.config_libs import ConfigLibsGroup
from ..methods.constants import TAB_NAME, NAME
from ..methods.widgets_features import PrettyFeatures
from ..methods.dialogs import Dialogs
from ...frames.libraries_widget import Ui_LibraryManager

from ..repositories.git_repo import GitRepo

########################################################################
class LibManager(QtGui.QMainWindow):
    
    def __init__(self, IDE):
        super(LibManager, self).__init__()
        
        self.libframe = Ui_LibraryManager()
        self.libframe.setupUi(self)
        
        
        self.user_libraries_dir = os.path.join(os.environ.get("PINGUINO_USER_PATH"), "user_libraries")        
        
        if not os.path.isdir(self.user_libraries_dir): os.mkdir(self.user_libraries_dir)
        
        self.ConfigLibs = ConfigLibsGroup()
        
        self.setWindowTitle(TAB_NAME+" - "+self.windowTitle())
        
        self.connect(self.libframe.pushButton_add, QtCore.SIGNAL("clicked()"), self.add_source)
        self.connect(self.libframe.lineEdit_source, QtCore.SIGNAL("editingFinished()"), self.add_source)
        self.connect(self.libframe.tableWidget_sources, QtCore.SIGNAL("itemClicked(QTableWidgetItem*)"), self.handleItemClicked_source)
        self.connect(self.libframe.tableWidget_libs, QtCore.SIGNAL("itemClicked(QTableWidgetItem*)"), self.handleItemClicked_libs)
        
        self.connect(self.libframe.pushButton_apply, QtCore.SIGNAL("clicked()"), self.update_changes_install_libs)   
        self.connect(self.libframe.pushButton_update, QtCore.SIGNAL("clicked()"), self.update_instaled_libraries)
        self.connect(self.libframe.pushButton_remove, QtCore.SIGNAL("clicked()"), self.remove_instaled_libraries)
        self.connect(self.libframe.pushButton_close, QtCore.SIGNAL("clicked()"), self.close_manager)
        self.connect(self.libframe.pushButton_from_zip, QtCore.SIGNAL("clicked()"), self.install_from_zip)
        
        self.connect(self.libframe.commandLinkButton_how, QtCore.SIGNAL("clicked()"), self.open_tutorial)
            
        self.default_line_edit_source = "e.g. git@github.com:repo/pinguinolib-library.git"
        PrettyFeatures.LineEdit_default_text(self.libframe, self.libframe.lineEdit_source, self.default_line_edit_source)
        
        bg_color = IDE.configIDE.config("Styles", "background_color", "#FFFFFF")
        alternate_bg_color = IDE.configIDE.config("Styles", "alternate_background_color", "#DDE8FF")        
        self.libframe.tableWidget_libs.setStyleSheet("QTableWidget {background-color: %s;\nalternate-background-color: %s;}"%(bg_color, alternate_bg_color))
        self.libframe.tableWidget_sources.setStyleSheet("QTableWidget {background-color: %s;\nalternate-background-color: %s;}"%(bg_color, alternate_bg_color))
        
        self.handleItemClicked_source_count = 0
        
        self.update_sources_view()
        self.update_libraries_view()
        self.centrar()
        
    #----------------------------------------------------------------------
    def open_tutorial(self):
        webbrowser.open_new_tab("https://github.com/PinguinoIDE/pinguino-lib_template/wiki")
        
        
    
    #----------------------------------------------------------------------
    def install_from_zip(self):
        open_files = QtGui.QFileDialog.getOpenFileNames(self,
                NAME+QtGui.QApplication.translate("Dialogs", " - Open"),
                QtCore.QDir.home().path(),
                QtGui.QApplication.translate("Dialogs", "Zip Files (*.zip)"))
        
        if not open_files[0]: return
        libraries = open_files[0]
        
        for lib in libraries:
            dir_, name = os.path.split(lib)
            name, ext = os.path.splitext(name)
            
            path_dirlib = os.path.join(os.environ.get("PINGUINO_USER_PATH"), "user_libraries", name)
            path_dirlib_source = os.path.join(os.environ.get("PINGUINO_USER_PATH"), "user_libraries", name, "lib")
            path_PINGUINO = os.path.join(os.environ.get("PINGUINO_USER_PATH"), "user_libraries", name, "lib", "PINGUINO")
            path_config = os.path.join(self.user_libraries_dir, name, "config")
            
            if os.path.exists(path_dirlib):
                Dialogs.error_message(self, "Library duplicated (name conflict)")
                return
            
            libZip = ZipFile(lib)
            os.mkdir(path_dirlib)
            libZip.extractall(path_dirlib_source)
            
            file_config = open(path_PINGUINO)
            content = "".join(file_config.readlines())
            file_config.close()
            
            content = content.replace("[library]", "[LIB]")
            
            file_config = open(path_config, "w")
            file_config.write(content)
            file_config.close()            
            
        self.update_sources_view()


    #----------------------------------------------------------------------
    def centrar(self):
        screen = QtGui.QDesktopWidget().screenGeometry()
        size =  self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)
        

    #----------------------------------------------------------------------
    def close_manager(self):
        self.ConfigLibs.save_config()
        self.close()
        
        
    #----------------------------------------------------------------------
    def add_source(self, source=None):
        if not source:
            source = self.libframe.lineEdit_source.text()
            if source == self.default_line_edit_source: return
    
        if self.ConfigLibs.check_duplicated(source):
            Dialogs.error_message(self, source+"\n already exist.")
            return
            
        name= self.get_name_from_source(source)
        if not name: return
        
        name_config = os.path.join(self.user_libraries_dir, name, "config")
        
        if os.path.exists(os.path.join(self.user_libraries_dir, name)):
            Dialogs.error_message(self, "Library duplicated (name conflict)")
            return
        
        os.mkdir(os.path.join(self.user_libraries_dir, name))
        
        new_lib = self.ConfigLibs.new(name_config)
        
        new_lib.set("LIB", "repository", source)
        new_lib.set("LIB", "name", name)
        new_lib.save_config()
        
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
        
        self.ConfigLibs.load_config()
        
        
    #----------------------------------------------------------------------
    def update_sources_view(self):
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
            checkable.setFlags(QtCore.Qt.ItemIsEnabled|QtCore.Qt.ItemIsUserCheckable|QtCore.Qt.ItemIsSelectable)
            checkable.setCheckState(QtCore.Qt.Unchecked)
            self.libframe.tableWidget_sources.setItem(index, 0, checkable)
            
            repository = QtGui.QTableWidgetItem()
            repository.setFlags(QtCore.Qt.ItemIsEnabled|QtCore.Qt.ItemIsSelectable)
            self.libframe.tableWidget_sources.setItem(index, 1, repository)
            
            self.libframe.tableWidget_sources.item(index, 0).setText(name)
            self.libframe.tableWidget_sources.item(index, 1).setText(repo)
            
            index += 1
            
            
    #----------------------------------------------------------------------
    def update_libraries_view(self):
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
            checkable.setFlags(QtCore.Qt.ItemIsEnabled|QtCore.Qt.ItemIsUserCheckable|QtCore.Qt.ItemIsSelectable)
            if active:
                checkable.setCheckState(QtCore.Qt.Checked)
            else:
                checkable.setCheckState(QtCore.Qt.Unchecked)
                
            self.libframe.tableWidget_libs.setItem(index, 0, checkable)
            
            arch_ = QtGui.QTableWidgetItem()
            arch_.setFlags(QtCore.Qt.ItemIsEnabled|QtCore.Qt.ItemIsSelectable)            
            self.libframe.tableWidget_libs.setItem(index, 1, arch_)
            
            auth = QtGui.QTableWidgetItem()
            auth.setFlags(QtCore.Qt.ItemIsEnabled|QtCore.Qt.ItemIsSelectable)            
            self.libframe.tableWidget_libs.setItem(index, 2, auth)

            desc = QtGui.QTableWidgetItem()
            desc.setFlags(QtCore.Qt.ItemIsEnabled|QtCore.Qt.ItemIsSelectable)      
            self.libframe.tableWidget_libs.setItem(index, 3, desc)
            
            self.libframe.tableWidget_libs.item(index, 0).setText(name)
            self.libframe.tableWidget_libs.item(index, 1).setText(arch)  
            self.libframe.tableWidget_libs.item(index, 2).setText(author)  
            self.libframe.tableWidget_libs.item(index, 3).setText(description)

            index += 1
        
        

    #----------------------------------------------------------------------
    def get_name_from_source(self, source):
        match = re.match(".*/([\w-]*)(.git)", source)
        
        if match:
            name = match.groups()[0]
            Dialogs.info_message(self, name+" from "+source+"\nsuccessfully added.")
            return name
        else:
            Dialogs.error_message(self, source+" is not recognized as a valid source.")
            return False
        
        
    #----------------------------------------------------------------------
    def handleItemClicked_source(self, item):
        for index in range(self.libframe.tableWidget_sources.rowCount()):
            if self.libframe.tableWidget_sources.item(index, 0).checkState() == QtCore.Qt.Checked:
                self.libframe.pushButton_update.setEnabled(True)
                self.libframe.pushButton_remove.setEnabled(True)
                return
        self.libframe.pushButton_update.setEnabled(False)
        self.libframe.pushButton_remove.setEnabled(False)
        
        
    #----------------------------------------------------------------------
    def handleItemClicked_libs(self, item):
        self.libframe.pushButton_apply.setEnabled(True)
        
        
    #----------------------------------------------------------------------
    def remove_instaled_libraries(self):
        sources = self.ConfigLibs.get_all_sources()
        selected = []        
        
        for index in range(self.libframe.tableWidget_sources.rowCount()):
            if self.libframe.tableWidget_sources.item(index, 0).checkState() == QtCore.Qt.Checked:
                selected.append(self.libframe.tableWidget_sources.item(index, 0).text())
                
        if not Dialogs.confirm_message(self, QtGui.QApplication.translate("Dialogs", "Are you sure you want to delete these libraries?")+
                                       "\n"+"\n".join(selected)):
            return
                
        selected = map(lambda sel:os.path.join(os.environ.get("PINGUINO_USER_PATH"), "user_libraries", sel), selected)
        
        for sel in selected:
            if os.path.exists(sel):
                shutil.rmtree(sel)
                
        self.update_sources_view()
        self.update_libraries_view()
        
                
        
    #----------------------------------------------------------------------
    def update_instaled_libraries(self):
        self.setCursor(QtCore.Qt.WaitCursor)
        
        sources = self.ConfigLibs.get_all_sources()
        selected = []
        
        for index in range(self.libframe.tableWidget_sources.rowCount()):
            if self.libframe.tableWidget_sources.item(index, 0).checkState() == QtCore.Qt.Checked:
                selected.append(self.libframe.tableWidget_sources.item(index, 0).text())
                
        Repository = GitRepo()
                
        work = []
        for sel in selected:
            if Repository.check_library(sel):
                #print("Updating...")
                try:
                    Repository.update_library(sel)
                except:
                    #if Dialogs.confirm_message(self, "Error", "Problems with "+sel+".\nTry again?"):
                    if os.path.exists(os.path.join(self.user_libraries_dir, sel, "lib")):
                        shutil.rmtree(os.path.join(self.user_libraries_dir, sel, "lib"))
                    try:
                        Repository.install_library(sel)
                    except:
                        Dialogs.error_message(self, "Problems with "+sel+".")
                work.append(sel + ": Updated")
                
            else:
                #print("Installing...")
                try:
                    Repository.install_library(sel)
                except:
                    #if Dialogs.confirm_message(self, "Error", "Problems with "+sel+".\nTry again?"):
                    if os.path.isdir(os.path.join(self.user_libraries_dir, sel, "lib")):
                        shutil.rmtree(os.path.join(self.user_libraries_dir, sel, "lib"))
                    try:
                        Repository.install_library(sel)
                    except:
                        Dialogs.error_message(self, "Problems with "+sel+".")
                work.append(sel + ": Installed")
                
            if not self.update_lib_config(sel):
                work.pop(-1)
                work.append(sel + ": Failed")
                
        self.setCursor(QtCore.Qt.ArrowCursor)
            
        Dialogs.info_message(self, "\n".join(work))
        self.update_libraries_view()
        
        
    #----------------------------------------------------------------------
    def update_lib_config(self, lib):
        lib_data = RawConfigParser()
        filename = os.path.join(self.user_libraries_dir, lib, "lib", "PINGUINO")

        sources = self.ConfigLibs.all_libs
        
        if os.path.exists(filename):
            lib_data.readfp(file(filename, "r"))
            for arg in "arch author description url repository name".split():
                sources[lib].set("LIB", arg, lib_data.get("library", arg))
            sources[lib].set("LIB", "installed", True)
            sources[lib].set("LIB", "active", False)
            sources[lib].save_config()
            return True
        else:
            #Dialogs.info_message(self, lib+" has no valid configuration file.\n\nmissing "+filename)
            #if os.path.isdir(os.path.join(self.user_libraries_dir, lib)):
                #shutil.rmtree(os.path.join(self.user_libraries_dir, lib))
            return False
        
        
    #----------------------------------------------------------------------
    def update_changes_install_libs(self):
        selected = []
        for index in range(self.libframe.tableWidget_libs.rowCount()):
            selected.append((self.libframe.tableWidget_libs.item(index, 0).text(), self.libframe.tableWidget_libs.item(index, 0).checkState() == QtCore.Qt.Checked))
            
        message = ""   
        sources = self.ConfigLibs.all_libs
        for sel, state in selected:
            sources[sel].set("LIB", "active", state)
            message += sel + ": " + "Actived" if state else "Deactived\n"
        
        Dialogs.info_message(self, message)
        
        self.ConfigLibs.save_config()