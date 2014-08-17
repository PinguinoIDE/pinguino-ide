#!/usr/bin/env python
#-*- coding: utf-8 -*-

from PySide import QtCore

from .event_methods import EventMethods
from .timer_methods import TimerMethods

########################################################################
class PinguinoEvents(EventMethods, TimerMethods):

    #----------------------------------------------------------------------
    def connect_events(self):

        # Menu File
        self.connect(self.main.actionNew_file, QtCore.SIGNAL("triggered()"), self.new_file)
        self.connect(self.main.actionOpen_file, QtCore.SIGNAL("triggered()"), self.open_files)
        self.connect(self.main.actionSave_file, QtCore.SIGNAL("triggered()"), self.save_file)
        self.connect(self.main.actionClose_file, QtCore.SIGNAL("triggered()"), self.close_file)
        self.connect(self.main.actionSave_as, QtCore.SIGNAL("triggered()"), self.save_as)
        self.connect(self.main.actionSave_all, QtCore.SIGNAL("triggered()"), self.save_all)
        self.connect(self.main.actionClose_all, QtCore.SIGNAL("triggered()"), self.close_all)
        self.connect(self.main.actionClose_others, QtCore.SIGNAL("triggered()"), self.close_others)
        self.connect(self.main.actionQuit, QtCore.SIGNAL("triggered()"), self.__close_ide__)
        self.connect(self.main.actionPrint, QtCore.SIGNAL("triggered()"), self.print_file)

        # Menu Edit
        self.connect(self.main.actionUndo, QtCore.SIGNAL("triggered()"), self.undo)
        self.connect(self.main.actionRedo, QtCore.SIGNAL("triggered()"), self.redo)
        self.connect(self.main.actionCut, QtCore.SIGNAL("triggered()"), self.cut)
        self.connect(self.main.actionCopy, QtCore.SIGNAL("triggered()"), self.copy)
        self.connect(self.main.actionPaste, QtCore.SIGNAL("triggered()"), self.paste)
        self.connect(self.main.actionDelete, QtCore.SIGNAL("triggered()"), self.delete)
        self.connect(self.main.actionSelect_all, QtCore.SIGNAL("triggered()"), self.select_all)
        self.connect(self.main.actionSearch, QtCore.SIGNAL("triggered()"), self.set_tab_search)
        self.connect(self.main.actionSearch_and_replace, QtCore.SIGNAL("triggered()"), self.set_tab_search)

        # Menu View
        self.connect(self.main.actionMaximize_editor, QtCore.SIGNAL("toggled(bool)"), self.exapand_editor)
        self.connect(self.main.actionToolbars, QtCore.SIGNAL("toggled(bool)"), self.toggle_toolbars)
        self.connect(self.main.actionTools_2, QtCore.SIGNAL("toggled(bool)"), self.main.dockWidget_tools.setVisible)
        self.connect(self.main.actionBlocks_2, QtCore.SIGNAL("toggled(bool)"), self.main.dockWidget_blocks.setVisible)
        self.connect(self.main.actionPython_shell, QtCore.SIGNAL("toggled(bool)"), self.main.dockWidget_output.setVisible)

        #Menu Settings
        self.connect(self.main.actionAutocomplete, QtCore.SIGNAL("triggered()"), self.switch_autocomplete)
        self.connect(self.main.actionColor_theme, QtCore.SIGNAL("toggled(bool)"), self.switch_color_theme)
        self.connect(self.main.actionConfirm_board, QtCore.SIGNAL("toggled(bool)"), self.switch_confirm_board)
        self.connect(self.main.action16x16, QtCore.SIGNAL("triggered(bool)"), self.resize_toolbar(16, self.main.action16x16))
        self.connect(self.main.action22x22, QtCore.SIGNAL("triggered(bool)"), self.resize_toolbar(22, self.main.action22x22))
        self.connect(self.main.action24x24, QtCore.SIGNAL("triggered(bool)"), self.resize_toolbar(24, self.main.action24x24))
        self.connect(self.main.action32x32, QtCore.SIGNAL("triggered(bool)"), self.resize_toolbar(32, self.main.action32x32))
        self.connect(self.main.action48x48, QtCore.SIGNAL("triggered(bool)"), self.resize_toolbar(48, self.main.action48x48))

        # Menu Source
        self.connect(self.main.actionComment_out_region, QtCore.SIGNAL("triggered()"), self.commentregion)
        self.connect(self.main.actionComment_Uncomment_region, QtCore.SIGNAL("triggered()"), self.comment_uncomment)
        self.connect(self.main.actionIndent, QtCore.SIGNAL("triggered()"), self.indentregion)
        self.connect(self.main.actionDedent, QtCore.SIGNAL("triggered()"), self.dedentregion)

        # Pinguino
        self.connect(self.main.actionLibrary_manager, QtCore.SIGNAL("triggered()"), self.__show_libmanager__)
        self.connect(self.main.actionSet_paths, QtCore.SIGNAL("triggered()"), self.__config_paths__)
        self.connect(self.main.actionSelect_board, QtCore.SIGNAL("triggered()"), self.__show_board_config__)
        self.connect(self.main.actionCompile, QtCore.SIGNAL("triggered()"), self.pinguino_compile)
        self.connect(self.main.actionUpload, QtCore.SIGNAL("triggered()"), self.pinguino_upload)
        self.connect(self.main.actionIf_Compile_then_Upload, QtCore.SIGNAL("triggered()"), self.pinguino_compile_and_upload)
        self.connect(self.main.actionHex_code, QtCore.SIGNAL("triggered()"), self.__show_hex_code__)
        self.connect(self.main.actionStdout, QtCore.SIGNAL("triggered()"), self.__show_stdout__)
        self.connect(self.main.actionMain_c, QtCore.SIGNAL("triggered()"), self.__show_main_c__)

        # Graphical
        self.connect(self.main.actionView_Pinguino_code, QtCore.SIGNAL("triggered()"), self.__show_pinguino_code__)
        self.connect(self.main.actionExport_code_to_editor, QtCore.SIGNAL("triggered()"), self.__export_pinguino_code__)
        self.connect(self.main.actionInsert_Block, QtCore.SIGNAL("triggered()"), self.__insert_block__)

        # Help
        self.connect(self.main.actionWiki_docs, QtCore.SIGNAL("triggered()"), self.show_wiki_docs)
        self.connect(self.main.actionIDE, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("https://github.com/PinguinoIDE/pinguino-ide/releases/latest"))
        self.connect(self.main.actionCompilers, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("https://github.com/PinguinoIDE/pinguino-compilers/releases/latest"))
        self.connect(self.main.actionLibraries, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("https://github.com/PinguinoIDE/pinguino-libraries/releases/latest"))
        self.connect(self.main.actionWebsite, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("http://www.pinguino.cc/"))
        self.connect(self.main.actionWiki, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("http://wiki.pinguino.cc/"))
        self.connect(self.main.actionForum, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("http://forum.pinguino.cc/"))
        self.connect(self.main.actionBlog, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("http://blog.pinguino.cc/"))
        self.connect(self.main.actionGroup, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("https://groups.google.com/forum/#!forum/pinguinocard"))
        self.connect(self.main.actionShop, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("http://shop.pinguino.cc/"))
        self.connect(self.main.actionAbout, QtCore.SIGNAL("triggered()"), self.__show_about__)

        # Tools Files
        self.connect(self.main.listWidget_files, QtCore.SIGNAL("itemActivated(QListWidgetItem*)"), self.jump_dir_files)
        self.connect(self.main.listWidget_filesg, QtCore.SIGNAL("itemActivated(QListWidgetItem*)"), self.jump_dir_filesg)
        self.connect(self.main.comboBox_files, QtCore.SIGNAL("currentIndexChanged(int)"), self.change_dir_files)
        self.connect(self.main.comboBox_filesg, QtCore.SIGNAL("currentIndexChanged(int)"), self.change_dir_filesg)

        # Tools Source
        self.connect(self.main.tableWidget_functions, QtCore.SIGNAL("doubleClicked(QModelIndex)"), self.jump_function)
        self.connect(self.main.tableWidget_directives, QtCore.SIGNAL("doubleClicked(QModelIndex)"), self.jump_directive)
        self.connect(self.main.tableWidget_variables, QtCore.SIGNAL("doubleClicked(QModelIndex)"), self.jump_variable)
        self.connect(self.main.tableWidget_functions.verticalHeader(), QtCore.SIGNAL("sectionClicked(int)"), self.jump_function_header)
        self.connect(self.main.tableWidget_directives.verticalHeader(), QtCore.SIGNAL("sectionClicked(int)"), self.jump_directive_header)
        self.connect(self.main.tableWidget_variables.verticalHeader(), QtCore.SIGNAL("sectionClicked(int)"), self.jump_variable_header)

        # Tools Search
        self.connect(self.main.pushButton_search_previous, QtCore.SIGNAL("clicked()"), self.search_previous)
        self.connect(self.main.pushButton_search_next, QtCore.SIGNAL("clicked()"), self.search_next)
        self.connect(self.main.pushButton_replace, QtCore.SIGNAL("clicked()"), self.replace)
        self.connect(self.main.pushButton_replace_all, QtCore.SIGNAL("clicked()"), self.replaceall)
        self.connect(self.main.lineEdit_search, QtCore.SIGNAL("textChanged(QString)"), self.search_instantaneous)
        self.connect(self.main.lineEdit_replace, QtCore.SIGNAL("textChanged(QString)"), self.replace_instantaneous)

        # Widgets
        self.connect(self.main.tabWidget_files, QtCore.SIGNAL("currentChanged(int)"), self.tab_changed)
        self.connect(self.main.tabWidget_files, QtCore.SIGNAL("tabCloseRequested(int)"), self.tab_close)
        self.connect(self.main.tabWidget_graphical, QtCore.SIGNAL("currentChanged(int)"), self.tab_changed)
        self.connect(self.main.tabWidget_graphical, QtCore.SIGNAL("tabCloseRequested(int)"), self.tab_close)

        # Graphical Tool Bar
        self.connect(self.main.actionSave_image, QtCore.SIGNAL("triggered()"), self.save_screen_image)
        self.connect(self.main.actionSwitch_ide, QtCore.SIGNAL("toggled(bool)"), self.switch_ide_mode)

        # Events
        self.closeEvent = self.__close_ide__
        self.connect(self.main.lineEdit_blocks_search, QtCore.SIGNAL("textChanged(QString)"), self.PinguinoKIT.update_blocks_search_tab)

        self.main.tabWidget_files.contextMenuEvent = self.tab_files_context_menu
        self.main.tabWidget_graphical.contextMenuEvent = self.tab_files_context_menu

        self.connect(self.main.dockWidget_output, QtCore.SIGNAL("visibilityChanged(bool)"), self.update_mode_output)
        self.connect(self.main.dockWidget_tools, QtCore.SIGNAL("visibilityChanged(bool)"), self.main.actionTools_2.setChecked)
        self.connect(self.main.dockWidget_blocks, QtCore.SIGNAL("visibilityChanged(bool)"), self.main.actionBlocks_2.setChecked)
        self.connect(self.main.dockWidget_output, QtCore.SIGNAL("visibilityChanged(bool)"), self.main.actionPython_shell.setChecked)

        self.main.actionTools_2.setChecked(self.main.dockWidget_tools.isVisible())
        self.main.actionPython_shell.setChecked(self.main.dockWidget_output.isVisible())
        self.main.actionBlocks_2.setChecked(self.main.dockWidget_blocks.isVisible())
        self.main.actionToolbars.setChecked(True)
        self.main.actionPython_shell.setChecked(True)
        self.main.actionConfirm_board.setChecked(self.configIDE.config("Features", "confirm_board", True))
