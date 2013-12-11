#!/usr/bin/env python
#-*- coding: utf-8 -*-

from PySide import QtCore

from .event_methods import EventMethods
from .timer_methods import TimerMethods


########################################################################
class PinguinoEvents(EventMethods, TimerMethods):
    """"""
        
    #----------------------------------------------------------------------
    def connect_events(self):
        
        self.connect(self.main.actionNew_file, QtCore.SIGNAL("triggered()"), self.new_file)
        self.connect(self.main.actionClose_file, QtCore.SIGNAL("triggered()"), self.close_file)
        self.connect(self.main.actionSave_file, QtCore.SIGNAL("triggered()"), self.save_file)
        self.connect(self.main.actionOpen_file, QtCore.SIGNAL("triggered()"), self.open_files)
        self.connect(self.main.actionSave_all, QtCore.SIGNAL("triggered()"), self.save_all)
        self.connect(self.main.actionSave_as, QtCore.SIGNAL("triggered()"), self.save_as)
        self.connect(self.main.actionClose_all, QtCore.SIGNAL("triggered()"), self.close_all)
        self.connect(self.main.actionQuit, QtCore.SIGNAL("triggered()"), self.__close_ide__)
        
        self.connect(self.main.actionUndo, QtCore.SIGNAL("triggered()"), self.undo)
        self.connect(self.main.actionRedo, QtCore.SIGNAL("triggered()"), self.redo)
        
        self.connect(self.main.actionCut, QtCore.SIGNAL("triggered()"), self.cut)
        self.connect(self.main.actionCopy, QtCore.SIGNAL("triggered()"), self.copy)
        self.connect(self.main.actionPaste, QtCore.SIGNAL("triggered()"), self.paste)
        
        self.connect(self.main.actionComment_Uncomment_region, QtCore.SIGNAL("triggered()"), self.comment_uncomment)
        self.connect(self.main.actionComment_out_region, QtCore.SIGNAL("triggered()"), self.commentregion)
        
        self.connect(self.main.actionSearch, QtCore.SIGNAL("triggered()"), self.set_tab_search)
        self.connect(self.main.actionSearch_and_replace, QtCore.SIGNAL("triggered()"), self.set_tab_search)
        
        self.connect(self.main.actionAbout, QtCore.SIGNAL("triggered()"), self.__show_about__)
        self.connect(self.main.actionSelect_board, QtCore.SIGNAL("triggered()"), self.__show_board_config__)
        
        self.connect(self.main.actionStdout, QtCore.SIGNAL("triggered()"), self.__show_stdout__)
        
        self.connect(self.main.tabWidget_files, QtCore.SIGNAL("currentChanged(int)"), self.tab_changed)
        self.connect(self.main.tabWidget_files, QtCore.SIGNAL("tabCloseRequested(int)"), self.tab_close)
        
        self.connect(self.main.tabWidget_graphical, QtCore.SIGNAL("currentChanged(int)"), self.tab_changed)
        self.connect(self.main.tabWidget_graphical, QtCore.SIGNAL("tabCloseRequested(int)"), self.tab_close)
        
        self.connect(self.main.tableWidget_functions, QtCore.SIGNAL("doubleClicked(QModelIndex)"), self.jump_function)
        self.connect(self.main.tableWidget_directives, QtCore.SIGNAL("doubleClicked(QModelIndex)"), self.jump_directive) 
        self.connect(self.main.tableWidget_variables, QtCore.SIGNAL("doubleClicked(QModelIndex)"), self.jump_variable)
        self.connect(self.main.tableWidget_functions.verticalHeader(), QtCore.SIGNAL("sectionClicked(int)"), self.jump_function_header)
        self.connect(self.main.tableWidget_directives.verticalHeader(), QtCore.SIGNAL("sectionClicked(int)"), self.jump_directive_header)
        self.connect(self.main.tableWidget_variables.verticalHeader(), QtCore.SIGNAL("sectionClicked(int)"), self.jump_variable_header)        
        
        self.connect(self.main.listWidget_files, QtCore.SIGNAL("itemActivated(QListWidgetItem*)"), self.jump_dir_files) 
        self.connect(self.main.comboBox_files, QtCore.SIGNAL("currentIndexChanged(QString)"), self.change_dir_files)
        
        self.connect(self.main.listWidget_filesg, QtCore.SIGNAL("itemActivated(QListWidgetItem*)"), self.jump_dir_filesg) 
        self.connect(self.main.comboBox_filesg, QtCore.SIGNAL("currentIndexChanged(QString)"), self.change_dir_filesg)        

        self.connect(self.main.pushButton_search_previous, QtCore.SIGNAL("clicked()"), self.search_previous)
        self.connect(self.main.pushButton_search_next, QtCore.SIGNAL("clicked()"), self.search_next)
        self.connect(self.main.pushButton_replace, QtCore.SIGNAL("clicked()"), self.replace)
        self.connect(self.main.pushButton_replace_all, QtCore.SIGNAL("clicked()"), self.replaceall) 
        self.connect(self.main.lineEdit_search, QtCore.SIGNAL("textChanged(QString)"), self.search_instantaneous)
        
        
        self.connect(self.main.actionCompile, QtCore.SIGNAL("triggered()"), self.pinguino_compile)   
        self.connect(self.main.actionUpload, QtCore.SIGNAL("triggered()"), self.pinguino_upload)
        
        
        self.connect(self.main.actionSave_image, QtCore.SIGNAL("triggered()"), self.save_screen_image)        
        
        self.connect(self.main.actionSwitch_ide, QtCore.SIGNAL("toggled(bool)"), self.switch_ide_mode)
        
        self.connect(self.main.actionView_Pinguino_code, QtCore.SIGNAL("triggered()"), self.__show_pinguino_code__)           
        
        
        #links
        self.connect(self.main.actionWebsite, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("http://www.pinguino.cc/")) 
        self.connect(self.main.actionGitHub, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("https://github.com/PinguinoIDE"))   
        self.connect(self.main.actionWiki, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("http://wiki.pinguino.cc/"))
        self.connect(self.main.actionForum, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("http://forum.pinguino.cc/"))  
        self.connect(self.main.actionBlog, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("http://blog.pinguino.cc/"))    
        self.connect(self.main.actionShop, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("http://shop.pinguino.cc/"))         
        self.connect(self.main.actionGroup, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("https://groups.google.com/forum/#!forum/pinguinocard"))        