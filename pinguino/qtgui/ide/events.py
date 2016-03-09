#!/usr/bin/env python
#-*- coding: utf-8 -*-

from PySide import QtCore

from .methods.core import PinguinoCore

########################################################################
class PinguinoEvents(PinguinoCore):

    #----------------------------------------------------------------------
    def __init__(self):
        """"""
        PinguinoCore.__init__(self)


    #----------------------------------------------------------------------
    def connect_events(self):
        """"""

        # Triggers
        self.connect(self.main.actionNew_file, QtCore.SIGNAL("triggered()"), self.ide_new_file)
        self.connect(self.main.actionNew_blocks_file, QtCore.SIGNAL("triggered()"), self.PinguinoKIT.ide_new_file)
        self.connect(self.main.actionOpen_file, QtCore.SIGNAL("triggered()"), self.ide_open_files)
        self.connect(self.main.actionSave_file, QtCore.SIGNAL("triggered()"), self.ide_save_file)
        self.connect(self.main.actionSave_as, QtCore.SIGNAL("triggered()"), self.ide_save_as)
        self.connect(self.main.actionSave_all, QtCore.SIGNAL("triggered()"), self.ide_save_all)
        self.connect(self.main.actionClose_file, QtCore.SIGNAL("triggered()"), self.ide_close_file)
        self.connect(self.main.actionClose_all, QtCore.SIGNAL("triggered()"), self.ide_close_all)
        self.connect(self.main.actionClose_others, QtCore.SIGNAL("triggered()"), self.ide_close_others)
        self.connect(self.main.actionPrint, QtCore.SIGNAL("triggered()"), self.editor_print_file)
        self.connect(self.main.actionQuit, QtCore.SIGNAL("triggered()"), self.ide_close_ide)
        self.connect(self.main.actionUndo, QtCore.SIGNAL("triggered()"), self.editor_undo)
        self.connect(self.main.actionRedo, QtCore.SIGNAL("triggered()"), self.editor_redo)
        self.connect(self.main.actionCut, QtCore.SIGNAL("triggered()"), self.editor_cut)
        self.connect(self.main.actionCopy, QtCore.SIGNAL("triggered()"), self.editor_copy)
        self.connect(self.main.actionPaste, QtCore.SIGNAL("triggered()"), self.editor_paste)
        self.connect(self.main.actionDelete, QtCore.SIGNAL("triggered()"), self.editor_delete)
        self.connect(self.main.actionSelect_all, QtCore.SIGNAL("triggered()"), self.editor_select_all)
        self.connect(self.main.actionComment_out_region, QtCore.SIGNAL("triggered()"), self.editor_comment_region)
        self.connect(self.main.actionComment_Uncomment_region, QtCore.SIGNAL("triggered()"), self.editor_comment_uncomment)
        self.connect(self.main.actionIndent, QtCore.SIGNAL("triggered()"), self.editor_indent_region)
        self.connect(self.main.actionDedent, QtCore.SIGNAL("triggered()"), self.editor_dedent_region)
        # self.connect(self.main.actionHex_code, QtCore.SIGNAL("triggered()"), self.__show_hex_code__)
        self.connect(self.main.actionMain_c, QtCore.SIGNAL("triggered()"), self.ide_show_main_c)
        self.connect(self.main.actionUser_c, QtCore.SIGNAL("triggered()"), self.ide_show_user_c)
        self.connect(self.main.actionDefine_h, QtCore.SIGNAL("triggered()"), self.ide_show_define_h)
        self.connect(self.main.actionHex, QtCore.SIGNAL("triggered()"), self.ide_show_hex)
        self.connect(self.main.actionReset_IDE_instalation, QtCore.SIGNAL("triggered()"), self.ide_reset_instalation)


        # Toggle Tabs
        self.connect(self.main.actionTabLog, QtCore.SIGNAL("toggled(bool)"), lambda :self.toggle_tab("Log"))
        self.connect(self.main.actionTabFiles, QtCore.SIGNAL("toggled(bool)"), lambda :self.toggle_tab("Files"))
        self.connect(self.main.actionTabBoards, QtCore.SIGNAL("toggled(bool)"), lambda :self.toggle_tab("Boards"))
        self.connect(self.main.actionTabStdout, QtCore.SIGNAL("toggled(bool)"), lambda :self.toggle_tab("Stdout"))
        self.connect(self.main.actionTabSearch, QtCore.SIGNAL("toggled(bool)"), lambda :self.toggle_tab("Search"))
        self.connect(self.main.actionTabProject, QtCore.SIGNAL("toggled(bool)"), lambda :self.toggle_tab("Project"))
        self.connect(self.main.actionTabShell, QtCore.SIGNAL("toggled(bool)"), lambda :self.toggle_tab("Shell"))
        self.connect(self.main.actionTabSourceBrowser, QtCore.SIGNAL("toggled(bool)"), lambda :self.toggle_tab("SourceBrowser"))
        self.connect(self.main.actionLibraryManager, QtCore.SIGNAL("toggled(bool)"), lambda :self.toggle_tab("LibraryManager"))
        self.connect(self.main.actionTabPaths, QtCore.SIGNAL("toggled(bool)"), lambda :self.toggle_tab("Paths"))
        #self.connect(self.main.actionTabICSP, QtCore.SIGNAL("toggled(bool)"), lambda :self.toggle_tab("ICSP"))


        # Perspective related events
        self.connect(self.main.actionMenubar, QtCore.SIGNAL("toggled(bool)"), self.toggle_menubar)
        self.connect(self.main.actionToolbars, QtCore.SIGNAL("toggled(bool)"), self.toggle_toolbars)
        self.connect(self.main.actionToggle_horizontal_tool_area, QtCore.SIGNAL("triggered()"), self.toggle_bottom_area)
        self.connect(self.main.actionToggle_vertical_tool_area, QtCore.SIGNAL("triggered()"), self.toggle_right_area)
        self.connect(self.main.tabWidget_tools, QtCore.SIGNAL("currentChanged(int)"), self.tab_right_changed)
        self.connect(self.main.tabWidget_bottom, QtCore.SIGNAL("currentChanged(int)"), self.tab_bottoms_changed)
        # self.connect(self.main.actionToggle_editor_area, QtCore.SIGNAL("toggled(bool)"), self.toggle_editor_area)
        self.connect(self.main.actionMove_vertical_tool_area, QtCore.SIGNAL("triggered()"), self.toggle_side_vertical_area)


        # Settings related events
        self.connect(self.main.actionAutocomplete, QtCore.SIGNAL("triggered()"), self.switch_autocomplete)
        self.connect(self.main.actionColor_theme, QtCore.SIGNAL("toggled(bool)"), self.switch_color_theme)
        self.connect(self.main.action16x16, QtCore.SIGNAL("triggered(bool)"), self.resize_toolbar(16, self.main.action16x16))
        self.connect(self.main.action24x24, QtCore.SIGNAL("triggered(bool)"), self.resize_toolbar(24, self.main.action24x24))
        self.connect(self.main.action32x32, QtCore.SIGNAL("triggered(bool)"), self.resize_toolbar(32, self.main.action32x32))
        self.connect(self.main.action48x48, QtCore.SIGNAL("triggered(bool)"), self.resize_toolbar(48, self.main.action48x48))


        # Child windows
        # self.connect(self.main.actionLibrary_manager, QtCore.SIGNAL("triggered()"), self.__show_libmanager__)
        # self.connect(self.main.actionSet_paths, QtCore.SIGNAL("triggered()"), self.__config_paths__)
        self.connect(self.main.actionSelect_board, QtCore.SIGNAL("triggered()"), self.set_tab_board)
        # self.connect(self.main.actionView_Pinguino_code, QtCore.SIGNAL("triggered()"), self.__show_pinguino_code__)
        self.connect(self.main.actionInsert_Block, QtCore.SIGNAL("triggered()"), self.__show_insert_block__)
        self.connect(self.main.actionSubmit_bug_report, QtCore.SIGNAL("triggered()"), self.__show_submit_bug__)
        # self.connect(self.main.actionCheck_for_patches, QtCore.SIGNAL("triggered()"), self.__show_patches__)


        # Pinguinguino related events
        self.connect(self.main.actionCompile, QtCore.SIGNAL("triggered()"), self.pinguino_compile)
        self.connect(self.main.actionUpload, QtCore.SIGNAL("triggered()"), self.pinguino_upload)
        self.connect(self.main.actionUpload_hex_directly, QtCore.SIGNAL("triggered()"), self.pinguino_upload_hex)
        self.connect(self.main.actionIf_Compile_then_Upload, QtCore.SIGNAL("triggered()"), self.pinguino_compile_and_upload)


        # Help
        self.connect(self.main.actionGitPinguinoIde, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("https://github.com/PinguinoIDE/pinguino-ide/releases/latest"))
        self.connect(self.main.actionGitPinguinoLibraries, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("https://github.com/PinguinoIDE/pinguino-libraries"))
        self.connect(self.main.actionGitPinguinoCompilers, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("https://github.com/PinguinoIDE/pinguino-compilers"))
        self.connect(self.main.actionWebsite, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("http://www.pinguino.cc/"))
        self.connect(self.main.actionWiki, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("http://wiki.pinguino.cc/"))
        self.connect(self.main.actionForum, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("http://forum.pinguino.cc/"))
        self.connect(self.main.actionBlog, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("http://blog.pinguino.cc/"))
        self.connect(self.main.actionGroup, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("https://groups.google.com/forum/#!forum/pinguinocard"))
        self.connect(self.main.actionShop, QtCore.SIGNAL("triggered()"), lambda :self.open_web_site("http://shop.pinguino.cc/"))
        self.connect(self.main.actionAbout, QtCore.SIGNAL("triggered()"), self.__show_about__)
        # self.connect(self.main.actionCheck_for_updates, QtCore.SIGNAL("triggered()"), self.need_update)


        # Events
        self.closeEvent = self.ide_close_ide
        # self.connect(self.main.actionSwitch_ide, QtCore.SIGNAL("toggled(bool)"), self.switch_ide_mode)
        self.connect(self.main.tabWidget_files, QtCore.SIGNAL("currentChanged(int)"), self.ide_tab_changed)
        self.connect(self.main.tabWidget_files, QtCore.SIGNAL("tabCloseRequested(int)"), self.ide_tab_close)
        # self.connect(self.main.tabWidget_bottom, QtCore.SIGNAL("customContextMenuRequested(QPoint)"), self.ide_tabs_context_menu)
        # self.connect(self.main.tabWidget_tools, QtCore.SIGNAL("customContextMenuRequested(QPoint)"), self.ide_tabs_context_menu)
        # self.connect(self.main.tabWidget_files, QtCore.SIGNAL("tabCloseRequested(int)"), self.ide_tab_close)


        # Graphical mode
        self.connect(self.main.actionSave_image, QtCore.SIGNAL("triggered()"), self.editor_save_screen_image)
        self.connect(self.main.lineEdit_blocks_search, QtCore.SIGNAL("textChanged(QString)"), self.PinguinoKIT.update_blocks_search_tab)
        self.connect(self.main.actionExport_code_to_editor, QtCore.SIGNAL("triggered()"), self.editor_export_pinguino_code)
        self.connect(self.main.comboBox_blocks, QtCore.SIGNAL("activated(QString)"), self.set_block_tab)
        self.connect(self.main.actionGenerate_blocks, QtCore.SIGNAL("triggered()"), self.PinguinoKIT.code_to_blocks)


        # Context menu
        self.main.tabWidget_files.contextMenuEvent = self.editor_tabfile_context_menu
        self.main.tabWidget_files.contextMenuEvent = self.editor_tabfile_context_menu


        # Initialize
        self.main.actionToolbars.setChecked(True)
        # self.main.actionConfirm_board.setChecked(self.configIDE.config("Features", "confirm_board", True))

        self.main.dockWidget_right.resizeEvent = self.tab_tools_resize
        self.main.dockWidget_bottom.resizeEvent = self.tab_tools_resize

        self.main.tabWidget_bottom.contextMenuEvent = self.ide_tabs_context_menu
        self.main.tabWidget_tools.contextMenuEvent = self.ide_tabs_context_menu
        self.main.toolBar.contextMenuEvent = self.ide_tabs_context_menu
        self.main.menubar.contextMenuEvent = self.ide_tabs_context_menu
