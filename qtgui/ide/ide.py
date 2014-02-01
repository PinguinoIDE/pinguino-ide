#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import sys
import codecs
import shutil
import logging
from ConfigParser import RawConfigParser

from PySide import QtGui, QtCore

from .methods.backgrounds import BackgroundPallete
from .events.events import PinguinoEvents
from .methods import constants as Constants
from .methods.dialogs import Dialogs
from .methods.pyhton_debug import Stderr
from .methods.config import Config
from .code_editor.autocomplete_icons import CompleteIcons
from .widgets.output_widget import PinguinoTerminal
from .methods.dev_tools import DevTools
from .methods.widgets_features import PrettyFeatures
from ..gide.app.graphical import GraphicalIDE
from ..frames.main import Ui_PinguinoIDE
from ..pinguino_api.pinguino import Pinguino, AllBoards


########################################################################
class PinguinoIDE(QtGui.QMainWindow, PinguinoEvents):
    
    def __init__(self):
        super(PinguinoIDE, self).__init__()
        
        
        QtCore.QTextCodec.setCodecForCStrings(QtCore.QTextCodec.codecForName("UTF-8"))
        QtCore.QTextCodec.setCodecForLocale(QtCore.QTextCodec.codecForName("UTF-8"))
        QtCore.QTextCodec.setCodecForTr(QtCore.QTextCodec.codecForName("UTF-8"))
        
        self.check_user_files()
        self.pinguinoAPI = Pinguino()
        self.pinguinoAPI._boards_ = AllBoards
        self.configIDE = Config()
        self.ICONS = CompleteIcons()
        self.update_pinguino_paths()
        self.update_user_libs()        
        
        self.main = Ui_PinguinoIDE()          
        self.main.setupUi(self)
        self.setWindowTitle(Constants.TAB_NAME)
        self.build_statusbar()
        self.fix_styles()
        self.init_graphical_mode()
        self.open_last_files()
        
        #self.connect_events()
        self.init_widgets()
    
        self.update_functions()
        self.update_directives()
        self.update_variables()
        self.update_autocompleter()
        self.__update_path_files__(os.path.join(os.getenv("PINGUINO_USER_PATH"), "examples"))
        self.__update_graphical_path_files__(os.path.join(os.getenv("PINGUINO_USER_PATH"), "graphical_examples"))
        
        self.set_board()
        self.statusbar_ide(self.get_status_board())
        
        self.load_main_config()
        
        self.connect_events()
    

        
    #----------------------------------------------------------------------
    def init_widgets(self):
        self.main.tabWidget_files.setVisible(False)
        self.main.toolBar_graphical.setVisible(False)
        
        self.main.tabWidget_tools.setCurrentIndex(0)
        self.main.tabWidget_blocks_tools.setCurrentIndex(0)
        
        PrettyFeatures.LineEdit_default_text(self, self.main.lineEdit_search, QtGui.QApplication.translate("Frame", "Search..."))
        PrettyFeatures.LineEdit_default_text(self, self.main.lineEdit_replace, QtGui.QApplication.translate("Frame", "Replace..."))
        PrettyFeatures.LineEdit_default_text(self, self.main.lineEdit_blocks_search, QtGui.QApplication.translate("Frame", "Search block..."))
        
        
    #----------------------------------------------------------------------
    def fix_styles(self):
        self.PinguinoPallete = BackgroundPallete()
        self.PinguinoPallete.save_palette(self.main.centralwidget.parent())   
        self.switch_color_theme(self.configIDE.config("Main", "color_theme", True))
        
        bg_color = self.configIDE.config("Styles", "background_color", "#FFFFFF")
        alternate_bg_color = self.configIDE.config("Styles", "alternate_background_color", "#DDE8FF")
        
        self.main.tableWidget_functions.setStyleSheet("QTableWidget {background-color: %s;\nalternate-background-color: %s;}"%(bg_color, alternate_bg_color))
        self.main.tableWidget_directives.setStyleSheet("QTableWidget {background-color: %s;\nalternate-background-color: %s;}"%(bg_color, alternate_bg_color))
        self.main.tableWidget_variables.setStyleSheet("QTableWidget {background-color: %s;\nalternate-background-color: %s;}"%(bg_color, alternate_bg_color))

    
    #----------------------------------------------------------------------
    def check_user_files(self):
        
        config_paths = RawConfigParser()
        config_paths.readfp(open("paths.conf", "r"))
        
        if os.name == "posix": #GNU/Linux
            os.environ["PINGUINO_OS_NAME"] = "linux"
        
        elif os.name == "nt":  #Windows
            os.environ["PINGUINO_OS_NAME"] = "windows"
        
        os.environ["PINGUINO_USER_PATH"] = os.path.expanduser(config_paths.get("paths-%s"%os.environ["PINGUINO_OS_NAME"], "user_path"))
        os.environ["PINGUINO_INSTALL_PATH"] = os.path.expanduser(config_paths.get("paths-%s"%os.environ["PINGUINO_OS_NAME"], "install_path"))        
            
        #create ~/.pinguino
        if not os.path.exists(os.environ.get("PINGUINO_USER_PATH")):
            os.mkdir(os.environ.get("PINGUINO_USER_PATH"))
            
        self.check_examples_dirs()
        self.check_config_files()
            
            
    #----------------------------------------------------------------------
    def check_examples_dirs(self):
        
        self.if_not_exist_then_copy(src=os.path.join(os.environ.get("PINGUINO_INSTALL_PATH"), "examples"),
                                    dst=os.path.join(os.environ.get("PINGUINO_USER_PATH"), "examples"),
                                    default_dir=True)
        
        self.if_not_exist_then_copy(src=os.path.join(os.environ.get("PINGUINO_INSTALL_PATH"), "graphical_examples"),
                                    dst=os.path.join(os.environ.get("PINGUINO_USER_PATH"), "graphical_examples"),
                                    default_dir=True)
        
        
    #----------------------------------------------------------------------
    def check_config_files(self):
        
        self.if_not_exist_then_copy(src=os.path.join(os.getcwd(), "qtgui", "config", "pinguino.%s.conf"%os.environ.get("PINGUINO_OS_NAME")),
                                    dst=os.path.join(os.environ.get("PINGUINO_USER_PATH"), "pinguino.conf"))
        
        self.if_not_exist_then_copy(src=os.path.join(os.getcwd(), "qtgui", "config", "reserved.pickle"),
                                    dst=os.path.join(os.environ.get("PINGUINO_USER_PATH"), "reserved.pickle"))
        
        self.if_not_exist_then_copy(src=os.path.join(os.getcwd(), "qtgui", "config", "wikidocs.pickle"),
                                    dst=os.path.join(os.environ.get("PINGUINO_USER_PATH"), "wikidocs.pickle"))
        
        
        
    #----------------------------------------------------------------------
    def if_not_exist_then_copy(self, src, dst, default_dir=False):
        if not os.path.exists(src):
            logging.warning("Missing: " + src)
            if not os.path.exists(dst) and default_dir:
                os.mkdir(dst)
            return
        
        if not os.path.exists(dst):
            if os.path.isdir(src):
                shutil.copytree(src, dst)
            else:
                shutil.copy(src, dst)
        
        
    #----------------------------------------------------------------------
    def init_graphical_mode(self):
        self.PinguinoKIT = GraphicalIDE(self)
        self.main.tabWidget_graphical.setVisible(False)
        self.main.dockWidget_blocks.setVisible(False) 
        
        self.main.actionAutocomplete.setChecked(self.configIDE.config("Features", "autocomplete", True))
        self.main.plainTextEdit_output = PinguinoTerminal(self.main.dockWidgetContents_2)
        self.main.plainTextEdit_output.set_extra_args(**{"pinguino_main": self, "devmode": DevTools(),})
        self.main.gridLayout_3.addWidget(self.main.plainTextEdit_output, 0, 0, 1, 1)
        
        
    #----------------------------------------------------------------------
    def is_graphical(self):
        return self.main.actionSwitch_ide.isChecked()
    
    
    #----------------------------------------------------------------------
    def is_widget(self):
        tab = self.get_tab()
        editor = tab.currentWidget()
        if editor is None: return False
        return getattr(editor, "is_widget", False)
    
    #----------------------------------------------------------------------
    def is_autocomplete_enable(self):
        return self.main.actionAutocomplete.isChecked()
    
        
    #----------------------------------------------------------------------
    def update_actions_for_text(self):
        normal = False
        #self.main.menuGraphical.setEnabled(normal)
        self.main.actionExport_code_to_editor.setEnabled(normal)
        self.main.actionView_Pinguino_code.setEnabled(normal)
        
        self.main.actionComment_out_region.setEnabled(not normal)   
        self.main.actionComment_Uncomment_region.setEnabled(not normal) 
        self.main.actionRedo.setEnabled(not normal)         
        self.main.actionUndo.setEnabled(not normal)         
        self.main.actionCut.setEnabled(not normal)         
        self.main.actionCopy.setEnabled(not normal)         
        self.main.actionPaste.setEnabled(not normal)         
        self.main.actionSearch.setEnabled(not normal)        
        self.main.actionSearch_and_replace.setEnabled(not normal)   
        self.main.dockWidget_blocks.setVisible(normal)
        self.main.dockWidget_tools.setVisible(not normal)  
        self.main.toolBar_search_replace.setVisible(not normal)
        self.main.toolBar_edit.setVisible(not normal)
        self.main.toolBar_graphical.setVisible(normal)
        self.main.toolBar_undo_redo.setVisible(not normal)
        
        self.configIDE.set("Features", "terminal_on_graphical", self.main.dockWidget_output.isVisible())         
        self.main.dockWidget_output.setVisible(self.configIDE.config("Features", "terminal_on_text", True))
        self.configIDE.save_config()
        
        
    #----------------------------------------------------------------------
    def update_actions_for_graphical(self):
        normal = True
        #self.main.menuGraphical.setEnabled(normal)
        self.main.actionExport_code_to_editor.setEnabled(normal)
        self.main.actionView_Pinguino_code.setEnabled(normal)
        
        self.main.actionRedo.setEnabled(not normal)          
        self.main.actionComment_out_region.setEnabled(not normal)   
        self.main.actionComment_Uncomment_region.setEnabled(not normal)         
        self.main.actionUndo.setEnabled(not normal)         
        self.main.actionCut.setEnabled(not normal)         
        self.main.actionCopy.setEnabled(not normal)         
        self.main.actionPaste.setEnabled(not normal)         
        self.main.actionSearch.setEnabled(not normal)        
        self.main.actionSearch_and_replace.setEnabled(not normal)   
        self.main.dockWidget_blocks.setVisible(normal)
        self.main.dockWidget_tools.setVisible(not normal)  
        self.main.toolBar_search_replace.setVisible(not normal)
        self.main.toolBar_edit.setVisible(not normal)
        self.main.toolBar_graphical.setVisible(normal)
        self.main.toolBar_undo_redo.setVisible(not normal)       
        
        self.configIDE.set("Features", "terminal_on_text", self.main.dockWidget_output.isVisible())        
        self.main.dockWidget_output.setVisible(self.configIDE.config("Features", "terminal_on_graphical", False))
        self.configIDE.save_config()
        
        
