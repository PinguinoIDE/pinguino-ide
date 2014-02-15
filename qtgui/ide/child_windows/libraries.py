#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import shutil
#import re
from zipfile import ZipFile
from ConfigParser import RawConfigParser
import webbrowser

from PySide import QtGui, QtCore

from ..methods.config_libs import ConfigLibsGroup
#from ..methods.constants import os.getenv("NAME"), NAME
from ..methods.widgets_features import PrettyFeatures
from ..methods.dialogs import Dialogs
from ..methods.repositories import PinguinoLibrary
from ...frames.libraries_widget import Ui_LibraryManager
from ...pinguino_api.pinguino_config import PinguinoConfig

########################################################################
class LibManager(QtGui.QMainWindow):
    
    def __init__(self, IDE):
        super(LibManager, self).__init__()
        
        self.libframe = Ui_LibraryManager()
        self.libframe.setupUi(self)
        
        self.main = IDE
        
        self.user_libraries_dir = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "libraries")        
        
        if not os.path.isdir(os.path.dirname(self.user_libraries_dir)):
            os.mkdir(os.path.dirname(self.user_libraries_dir))
            self.main.write_log(QtGui.QApplication.translate("frame", "Created :")+os.path.dirname(self.user_libraries_dir))
        if not os.path.isdir(self.user_libraries_dir):
            os.mkdir(self.user_libraries_dir)
            self.main.write_log(QtGui.QApplication.translate("frame", "Created :")+self.user_libraries_dir)
        
        self.ConfigLibs = ConfigLibsGroup()
        
        self.setWindowTitle(os.getenv("NAME")+" - "+self.windowTitle())
        
        self.connect(self.libframe.pushButton_add, QtCore.SIGNAL("clicked()"), self.add_source)
        #self.connect(self.libframe.lineEdit_source, QtCore.SIGNAL("editingFinished()"), self.add_source)
        self.connect(self.libframe.tableWidget_sources, QtCore.SIGNAL("itemClicked(QTableWidgetItem*)"), self.handleItemClicked_source)
        self.connect(self.libframe.tableWidget_libs, QtCore.SIGNAL("itemClicked(QTableWidgetItem*)"), self.handleItemClicked_libs)
        
        self.connect(self.libframe.pushButton_apply, QtCore.SIGNAL("clicked()"), self.update_changes_install_libs)   
        self.connect(self.libframe.pushButton_update, QtCore.SIGNAL("clicked()"), self.update_libraries)
        self.connect(self.libframe.pushButton_remove, QtCore.SIGNAL("clicked()"), self.remove_instaled_libraries)
        self.connect(self.libframe.pushButton_close, QtCore.SIGNAL("clicked()"), self.close)
        self.connect(self.libframe.pushButton_from_zip, QtCore.SIGNAL("clicked()"), self.install_from_zip)
        
        self.connect(self.libframe.checkBox_sources, QtCore.SIGNAL("clicked(bool)"), self.check_all_sources)    
        self.connect(self.libframe.checkBox_libs, QtCore.SIGNAL("clicked(bool)"), self.check_all_libs)        
        
        self.connect(self.libframe.commandLinkButton_how, QtCore.SIGNAL("clicked()"), self.open_tutorial)


        self.connect(self.libframe.tableWidget_sources, QtCore.SIGNAL("clicked(QModelIndex)"), self.open_link)         
        
            
        self.default_line_edit_source = "e.g. git@github.com:repo/pinguinolib-library.git"
        PrettyFeatures.LineEdit_default_text(self.libframe.lineEdit_source, self.default_line_edit_source)
        
        bg_color = IDE.configIDE.config("Styles", "background_color", "#FFFFFF")
        alternate_bg_color = IDE.configIDE.config("Styles", "alternate_background_color", "#DDE8FF")        
        self.libframe.tableWidget_libs.setStyleSheet("QTableWidget {background-color: %s;\nalternate-background-color: %s;}"%(bg_color, alternate_bg_color))
        self.libframe.tableWidget_sources.setStyleSheet("QTableWidget {background-color: %s;\nalternate-background-color: %s;}"%(bg_color, alternate_bg_color))
        
        self.handleItemClicked_source_count = 0
        
        self.closeEvent = self.close
        
        self.libframe.tabWidget.setCurrentIndex(0)
        
        self.update_sources_view()
        self.update_libraries_view()
        self.centrar()
        
    
    #----------------------------------------------------------------------
    def open_link(self, model_index):
        #column = model_index.column()
        item = self.libframe.tableWidget_sources.itemFromIndex(model_index)
        url = getattr(item, "url", None)
        if not url: return
        
        reply = Dialogs.confirm_message(self, QtGui.QApplication.translate("Dialogs", "Do you want open this URL in a new tab?")+"\n"+url)
        if reply: webbrowser.open_new_tab(url)
        
        
    #----------------------------------------------------------------------
    def post_install(self, lib_name):
        
        for dir_ in ["examples", "blocks"]:
            path_user_examples_libraries = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), dir_)
            if not os.path.exists(path_user_examples_libraries):
                os.mkdir(path_user_examples_libraries)
                self.main.write_log(QtGui.QApplication.translate("frame", "Created :")+path_user_examples_libraries)
            
        lista = []
        
        path_examples = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "libraries", lib_name, "lib", "examples")
        dest_examples = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "examples", lib_name)
        lista.append([path_examples, dest_examples])
        
        path_blocks = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "libraries", lib_name, "lib", "blocks")
        dest_blocks = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "blocks", lib_name)
        lista.append([path_blocks, dest_blocks])
        
        for src, dest in lista:
            if os.path.exists(src):
                if os.path.exists(dest):
                    shutil.rmtree(dest)
                    self.main.write_log(QtGui.QApplication.translate("frame", "Removed :")+dest)
                shutil.copytree(src, dest)
                self.main.write_log(QtGui.QApplication.translate("frame", "Created :")+dest)
                for x in os.listdir(dest):
                    self.main.write_log(QtGui.QApplication.translate("frame", "Copied :")+x)
            
        
    #----------------------------------------------------------------------
    def open_tutorial(self):
        webbrowser.open_new_tab("https://github.com/PinguinoIDE/pinguino-lib_template/wiki")
        
        
    
    #----------------------------------------------------------------------
    def install_from_zip(self):
        open_files = QtGui.QFileDialog.getOpenFileNames(self,
                os.getenv("NAME")+QtGui.QApplication.translate("Dialogs", " - Open"),
                QtCore.QDir.home().path(),
                QtGui.QApplication.translate("Dialogs", "Zip Files (*.zip)"))
        
        if not open_files[0]: return
        libraries = open_files[0]
        
        for lib in libraries:
            #dir_, name = os.path.split(lib)
            #name, ext = os.path.splitext(name)
            name = os.path.split(lib)[1]
            name = os.path.splitext(name)[0]
            
            path_dirlib = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "libraries", name)
            #path_dirlib_source = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "libraries", name, "lib")
            path_dirlib_source = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "libraries", name)
            path_PINGUINO = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "libraries", name, "lib", "PINGUINO")
            path_config = os.path.join(self.user_libraries_dir, name, "config")
            
            if os.path.exists(path_dirlib):
                Dialogs.error_message(self, QtGui.QApplication.translate("Dialogs", "Library duplicated (name conflict)."))
                return
            
            libZip = ZipFile(lib)
            os.mkdir(path_dirlib)
            self.main.write_log(QtGui.QApplication.translate("frame", "Created :")+path_dirlib)
            libZip.extractall(path_dirlib_source)
            
            try:
                os.rename(os.path.join(path_dirlib_source, name), os.path.join(path_dirlib_source, "lib"))
            except OSError:
                Dialogs.error_message(self, QtGui.QApplication.translate("Dialogs", "This is not a Pinguino library or are not packaged correctly."))
                shutil.rmtree(path_dirlib_source)
                self.main.write_log(QtGui.QApplication.translate("frame", "Removed :")+path_dirlib_source)
                continue
            
            file_config = open(path_PINGUINO)
            content = "".join(file_config.readlines())
            file_config.close()
            
            content = content.replace("[library]", "[LIB]")
            
            file_config = open(path_config, "w")
            file_config.write(content)
            file_config.close()
            
            self.post_install(name)
            
        self.update_sources_view()
        self.update_libraries_view()


    #----------------------------------------------------------------------
    def centrar(self):
        screen = QtGui.QDesktopWidget().screenGeometry()
        size = self.geometry()
        self.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)
        

    #----------------------------------------------------------------------
    def close(self, event=None):
        self.ConfigLibs.save_config()
        #self.main.update_user_libs()
        PinguinoConfig.update_user_libs(self.main.pinguinoAPI)
        self.main.update_instaled_reserved_words()
        super(LibManager, self).close()
        
        
    #----------------------------------------------------------------------
    def add_source(self, source=None):
        if not source:
            source = self.libframe.lineEdit_source.text()
            if source == self.default_line_edit_source: return
    
        if self.ConfigLibs.check_duplicated(source):
            Dialogs.error_message(self, source+"\n"+QtGui.QApplication.translate("Dialogs", "already exist."))
            return
            
        name = "temp_lib_name"
        if not name: return
        
        temp_dir = os.path.join(self.user_libraries_dir, name)
        if os.path.exists(temp_dir):
            shutil.rmtree(temp_dir)
            self.main.write_log(QtGui.QApplication.translate("frame", "Removed :")+temp_dir)
        os.mkdir(temp_dir)
        self.main.write_log(QtGui.QApplication.translate("frame", "Created :")+temp_dir)
        
        name_config = os.path.join(self.user_libraries_dir, name, "config") 
        new_lib = self.ConfigLibs.new(name_config)
        new_lib.set("LIB", "repository", source)
        new_lib.save_config() 
        
        if self.install_library(name):
            data = self.get_data_from_lib(name)
            new_lib = self.ConfigLibs.new(name_config)
            source = self.ConfigLibs.all_libs[name]
            source.set("LIB", "description", data["description"])
            source.set("LIB", "author", data["author"])
            source.set("LIB", "arch", data["arch"])
            source.set("LIB", "url", data["url"])
            source.set("LIB", "name", data["name"])
            source.save_config()
            os.rename(temp_dir, os.path.join(os.path.split(temp_dir)[0], data["name"]))
            
        self.ConfigLibs = ConfigLibsGroup()
        self.ConfigLibs.load_config()
        
        self.update_libraries_view()
        self.update_sources_view()
        
        
    #----------------------------------------------------------------------
    def update_sources_view(self):
        sources = self.ConfigLibs.get_all_sources()
        
        index = 0
        self.libframe.tableWidget_sources.setRowCount(0)
        
        #self.libframe.tableWidget_sources.set
        
        for key in sources.keys():
            
            name = key
            repo = sources[key]["repository"]
            url = sources[key]["url"]
            description = sources[key]["description"]
            
            #ins = "Yes" if sources[key]["installed"] else "No"

            if not repo:
                repo == "None"
            
            self.libframe.tableWidget_sources.setRowCount(self.libframe.tableWidget_sources.rowCount() + 1)            
            
            item = QtGui.QTableWidgetItem()
            self.libframe.tableWidget_sources.setVerticalHeaderItem(index, item)
            
            #name
            checkable = QtGui.QTableWidgetItem()
            checkable.setFlags(QtCore.Qt.ItemIsEnabled|QtCore.Qt.ItemIsUserCheckable)
            checkable.setCheckState(QtCore.Qt.Unchecked)
            checkable.setTextAlignment(QtCore.Qt.AlignHCenter|QtCore.Qt.AlignVCenter)
            checkable.setToolTip(description)
            self.libframe.tableWidget_sources.setItem(index, 0, checkable)
            
            #repo
            repository = QtGui.QTableWidgetItem()
            repository.setFlags(QtCore.Qt.ItemIsEnabled)
            repository.setTextAlignment(QtCore.Qt.AlignHCenter|QtCore.Qt.AlignVCenter)
            self.libframe.tableWidget_sources.setItem(index, 1, repository)
            
            #url
            web_page = QtGui.QTableWidgetItem()
            web_page.setFlags(QtCore.Qt.ItemIsEnabled)
            brush = QtGui.QBrush(QtGui.QColor(0, 0, 255))
            brush.setStyle(QtCore.Qt.NoBrush)
            web_page.setForeground(brush)
            font = web_page.font()
            font.setUnderline(True)
            web_page.setFont(font)
            web_page.setTextAlignment(QtCore.Qt.AlignHCenter|QtCore.Qt.AlignVCenter)
            web_page.setToolTip(url)
            setattr(web_page, "url", url)
            self.libframe.tableWidget_sources.setItem(index, 2, web_page)
            
        
            
            self.libframe.tableWidget_sources.item(index, 0).setText(name)
            self.libframe.tableWidget_sources.item(index, 1).setText(repo)
            self.libframe.tableWidget_sources.item(index, 2).setText("Link")
            
            index += 1
            
    
        hh = self.libframe.tableWidget_sources.horizontalHeader()
        hh.resizeSection(0, 250)        
        hh.resizeSection(1, 400)        
        #hh.resizeSection(2, 50)         
                        
            
            
            
    #----------------------------------------------------------------------
    def update_libraries_view(self):
        self.ConfigLibs.save_config()
        self.ConfigLibs.load_config()
        sources = self.ConfigLibs.get_all_sources()
        
        index = 0
        self.libframe.tableWidget_libs.setRowCount(0)
        
        hh = self.libframe.tableWidget_libs.horizontalHeader()
        hh.resizeSection(0, 250)        

        for key in sources.keys():
            
            name = key
            #repo = sources[key]["repository"]
            #installed = sources[key]["installed"]
            arch = sources[key]["arch"]
            author = sources[key]["author"]
            description = sources[key]["description"]
            active = sources[key]["active"]
            
            #if not installed: continue
            self.libframe.tableWidget_libs.setRowCount(self.libframe.tableWidget_libs.rowCount()+1)
                
            
            #name            
            checkable = QtGui.QTableWidgetItem()
            checkable.setFlags(QtCore.Qt.ItemIsEnabled|QtCore.Qt.ItemIsUserCheckable)
            if active:
                checkable.setCheckState(QtCore.Qt.Checked)
                checkable.setToolTip(QtGui.QApplication.translate("ToolTip", "This library is enable (can be used)."))
            else:
                checkable.setCheckState(QtCore.Qt.Unchecked)
                checkable.setToolTip(QtGui.QApplication.translate("ToolTip", "This library is not enable."))                
            checkable.setTextAlignment(QtCore.Qt.AlignHCenter|QtCore.Qt.AlignVCenter)
            self.libframe.tableWidget_libs.setItem(index, 0, checkable)
            
            #arch
            arch_ = QtGui.QTableWidgetItem()
            arch_.setFlags(QtCore.Qt.ItemIsEnabled)
            arch_.setTextAlignment(QtCore.Qt.AlignHCenter|QtCore.Qt.AlignVCenter)
            self.libframe.tableWidget_libs.setItem(index, 1, arch_)
            
            #author
            auth = QtGui.QTableWidgetItem()
            auth.setFlags(QtCore.Qt.ItemIsEnabled)
            auth.setTextAlignment(QtCore.Qt.AlignHCenter|QtCore.Qt.AlignVCenter)
            self.libframe.tableWidget_libs.setItem(index, 2, auth)

            #description
            desc = QtGui.QTableWidgetItem()
            desc.setFlags(QtCore.Qt.ItemIsEnabled)
            #desc.setTextAlignment(QtCore.Qt.AlignHCenter|QtCore.Qt.AlignVCenter)
            self.libframe.tableWidget_libs.setItem(index, 3, desc)
            
            self.libframe.tableWidget_libs.item(index, 0).setText(name)
            self.libframe.tableWidget_libs.item(index, 1).setText(arch)  
            self.libframe.tableWidget_libs.item(index, 2).setText(author)  
            self.libframe.tableWidget_libs.item(index, 3).setText(description)

            index += 1
        
        
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
        #sources = self.ConfigLibs.get_all_sources()
        selected = []        
        
        for index in range(self.libframe.tableWidget_sources.rowCount()):
            if self.libframe.tableWidget_sources.item(index, 0).checkState() == QtCore.Qt.Checked:
                selected.append(self.libframe.tableWidget_sources.item(index, 0).text())
                
        if not Dialogs.confirm_message(self, QtGui.QApplication.translate("Dialogs", "Are you sure you want to delete these libraries?")+
                                       "\n"+"\n".join(selected)):
            return
                
        selected_l = map(lambda sel:os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "libraries", sel), selected)
        selected_b = map(lambda sel:os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "blocks", sel), selected)
        selected_e = map(lambda sel:os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "examples", sel), selected)
        
        for sel in selected_l + selected_b + selected_e:
            if os.path.exists(sel):
                shutil.rmtree(sel)
                self.main.write_log(QtGui.QApplication.translate("frame", "Removed :")+sel)
                
        self.update_sources_view()
        self.update_libraries_view()
        
    #----------------------------------------------------------------------
    def get_repository_for_install(self):
        repos = {"git": self.libframe.radioButton_repo_git, 
                 "hg": self.libframe.radioButton_repo_hg,
                 "svn": self.libframe.radioButton_repo_svn,
                 }
        for key in repos.keys():
            if repos[key].isChecked(): return key
            
            
    #----------------------------------------------------------------------
    def update_libraries(self):
        self.setCursor(QtCore.Qt.WaitCursor)
        #sources = self.ConfigLibs.get_all_sources()
        selected = []
        
        for index in range(self.libframe.tableWidget_sources.rowCount()):
            if self.libframe.tableWidget_sources.item(index, 0).checkState() == QtCore.Qt.Checked:
                selected.append(self.libframe.tableWidget_sources.item(index, 0).text())
                
        work = []
        for sel in selected:
            Repository = PinguinoLibrary(sel)
            if Repository is None:
                work.append(sel + ": Nothing to do")
                continue
            
            reply = Repository.update_library()
            if reply:
                work.append(sel+": "+QtGui.QApplication.translate("Dialogs", "Updated"))
                self.post_install(sel)
            else:
                work.append(sel+": "+QtGui.QApplication.translate("Dialogs", "Failed"))
                
        self.setCursor(QtCore.Qt.ArrowCursor)
            
        Dialogs.info_message(self, "\n".join(work))
        self.update_libraries_view()
        
        
    #----------------------------------------------------------------------
    def install_library(self, sel):
        self.setCursor(QtCore.Qt.WaitCursor)
        #sources = self.ConfigLibs.get_all_sources()
        Repository = PinguinoLibrary(sel)
        reply = Repository.install_library(self.get_repository_for_install())
        
        if reply:
            Dialogs.info_message(self, sel+": "+QtGui.QApplication.translate("Dialogs", "Installed"))
            self.post_install(sel)
        else:
            Dialogs.error_message(self, QtGui.QApplication.translate("Dialogs", "Problems with")+" %s."%sel)
            
        self.setCursor(QtCore.Qt.ArrowCursor)

        
        return reply



        
    #----------------------------------------------------------------------
    def get_data_from_lib(self, lib):
        lib_data = RawConfigParser()
        filename = os.path.join(self.user_libraries_dir, lib, "lib", "PINGUINO")

        lib_data.readfp(open(filename, "r"))
        
        return {"arch": lib_data.get("library", "arch"),
                "author": lib_data.get("library", "author"),
                "description": lib_data.get("library", "description"),
                "url": lib_data.get("library", "url"),
                "name": lib_data.get("library", "name"),
                }
        
        
        
    #----------------------------------------------------------------------
    def update_changes_install_libs(self):
        selected = []
        for index in range(self.libframe.tableWidget_libs.rowCount()):
            selected.append((self.libframe.tableWidget_libs.item(index, 0).text(), self.libframe.tableWidget_libs.item(index, 0).checkState() == QtCore.Qt.Checked))
            
        message = ""   
        sources = self.ConfigLibs.all_libs
        for sel, state in selected:
            sources[sel].set("LIB", "active", state)
            message += sel + ": " + (QtGui.QApplication.translate("Dialogs", "Enable") if state else QtGui.QApplication.translate("Dialogs", "Disable")) + "\n"
        
        Dialogs.info_message(self, message)
        
        self.ConfigLibs.save_config()
        
        self.libframe.pushButton_apply.setEnabled(False)        
        self.update_libraries_view()
        
        
    ##----------------------------------------------------------------------
    #def get_repo_type(self, sel, sources):
        
        #cfg = sources[sel]
        
        #if cfg["repository"].endswith(".git"):
            #return PinguinoLibrary()
    
    
    #----------------------------------------------------------------------
    def check_all_sources(self, check):
        for index in range(self.libframe.tableWidget_sources.rowCount()):
            if check:
                self.libframe.tableWidget_sources.item(index, 0).setCheckState(QtCore.Qt.Checked)
            else:
                self.libframe.tableWidget_sources.item(index, 0).setCheckState(QtCore.Qt.Unchecked)                
        self.libframe.pushButton_update.setEnabled(check)
        self.libframe.pushButton_remove.setEnabled(check)            
                
    #----------------------------------------------------------------------
    def check_all_libs(self, check):
        for index in range(self.libframe.tableWidget_sources.rowCount()):
            if check:
                self.libframe.tableWidget_libs.item(index, 0).setCheckState(QtCore.Qt.Checked)
            else:
                self.libframe.tableWidget_libs.item(index, 0).setCheckState(QtCore.Qt.Unchecked)
        #self.libframe.pushButton_apply.setEnabled(check)
        