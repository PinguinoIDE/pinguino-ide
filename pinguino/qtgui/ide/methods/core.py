#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import codecs
import pickle
import logging
import webbrowser
import shutil
from math import ceil

from PySide import QtGui, QtCore
# import requests
# from urllib import request
# import urllib
# import json

from ...pinguino_core.pinguino_config import PinguinoConfig


from .core_threads import UpdateAutocompleter, PinguinoRequest
from .parser import PinguinoParser

from ..custom_widgets import PinguinoCodeEditor

from .decorators import Decorator
from .dialogs import Dialogs
from ..tools.files import Files
from ..tools.search import Search
from ..tools.project import Project
from ..tools.boards import Boards
from ..tools.source_browser import SourceBrowser
# from ..tools.library_manager import LibraryManager
# from ..methods.library_manager import Librarymanager
# from ..widgets.output_widget import START

from .timed_methods import TimedMethods
# from .event_methods import EventMethods

from ..child_windows.about import About
# from ..child_windows.libraries import LibManager
# from ..child_windows.paths import Paths
# from ..child_windows.hex_viewer import HexViewer
from ..child_windows.insert_block_dialog import InsertBlock
# from ..child_windows.environ_viewer import EnvironViewer
from ..child_windows.submit_bug import SubmitBug, send_old_submits
from ..child_windows.library_template import LibraryTemplate


########################################################################
class PinguinoComponents(TimedMethods, Search, Project, Files, Boards, SourceBrowser):
    """"""

    #----------------------------------------------------------------------
    def __init__(self):
        """"""
        Boards.__init__(self)
        Search.__init__(self)
        SourceBrowser.__init__(self)
        Files.__init__(self)
        Project.__init__(self)
        # LibraryManager.__init__(self)
        # super(BoardConfig, self).__init__()



########################################################################
class PinguinoChilds(object):
    """Frames."""


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_graphical_mode()
    def __show_insert_block__(self):
        self.frame_insert_block = InsertBlock(self.PinguinoKIT)
        self.frame_insert_block.show()


    #----------------------------------------------------------------------
    def __show_about__(self):
        self.frame_about = About()
        self.frame_about.show()


    # #----------------------------------------------------------------------
    # def __show_libmanager__(self):
        # self.frame_stdout = LibManager(self)
        # self.frame_stdout.show()


    # #----------------------------------------------------------------------
    # def __config_paths__(self):
        # self.frame_paths = Paths(self)
        # self.frame_paths.show()


    #----------------------------------------------------------------------
    def __show_submit_bug__(self):
        self.submit_bug = SubmitBug(self)
        self.submit_bug.show()


    #----------------------------------------------------------------------
    def __show_library_template__(self):
        """"""
        self.library_template = LibraryTemplate(self)
        self.library_template.show()



    # #----------------------------------------------------------------------
    # def __show_patches__(self):
        # self.patches = Patches(self)
        # patches = self.patches.get_patches()

        # if patches == 0:
            # Dialogs.info_message(self, "There are no new updates available.\n {PINGUINO_FULLNAME} is up to date".format(**os.environ))
            # self.patches.close()
        # elif patches is None:
            # self.patches.close()
        # else:
            # self.patches.show()


########################################################################
class PinguinoQueries(object):
    """"""

    #----------------------------------------------------------------------
    def get_tab(self):
        return self.main.tabWidget_files


    #----------------------------------------------------------------------
    def get_untitled_name(self, ext=".pde"):

        index = 1
        name = "untitled-{}".format(index) + ext
        #filenames = [self.main.tabWidget_files.tabText(i) for i in range(self.main.tabWidget_files.count())]
        filenames = [self.get_tab().tabText(i) for i in range(self.get_tab().count())]
        while name in filenames or name + "*" in filenames:
            index += 1
            name = "untitled-{}".format(index) + ext
        return name + "*"


    #----------------------------------------------------------------------
    def get_tabs_height(self):

        return self.main.tabWidget_bottom.tabBar().height()


    #----------------------------------------------------------------------
    def get_current_editor(self):

        return self.main.tabWidget_files.currentWidget()

    #----------------------------------------------------------------------
    def get_current_code(self):
        """"""
        editor = self.get_current_editor()
        return editor.text_edit.toPlainText()

    #----------------------------------------------------------------------
    def get_all_editors(self, text=True, blocks=False, html=False):

        count = self.main.tabWidget_files.count()
        editors = []
        for i in range(count):
            widget = self.main.tabWidget_files.widget(i)

            if hasattr(widget, "graphical_area") and blocks:
                editors.append(widget)

            elif hasattr(widget, "text_edit") and text:
                editors.append(widget)

            elif hasattr(widget, "html") and html:
                editors.append(widget)

        return editors



    #----------------------------------------------------------------------
    def is_graphical(self, editor=None):
        if editor is None:
            editor = self.get_current_editor()
        if hasattr(editor, "graphical_area"):
            return True
        elif hasattr(editor, "text_edit"):
            return False
        else:
            return None


        # return self.main.actionSwitch_ide.isChecked()


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
    def get_all_open_files(self):

        opens = []

        for tab in [self.main.tabWidget_files, self.main.tabWidget_files]:
            widgets = map(tab.widget, range(tab.count()))
            for widget in widgets:
                if not tab.tabText(tab.indexOf(widget)).endswith("(r/o)"):
                    path = getattr(widget, "path", False)
                    if path: opens.append(path)

        return opens



    #----------------------------------------------------------------------
    def get_description_board(self):

        board_config = []

        board = self.pinguinoAPI.get_board()
        board_config.append("Board: {}".format(board.name))
        board_config.append("Proc: {}".format(board.proc))
        board_config.append("Arch: {}".format(board.arch))

        if board.arch == 32:
            board_config.append("MIPS 16: {}".format(self.configIDE.config("Board", "mips16", True)))
            board_config.append("Heap size: {} bytes".format(self.configIDE.config("Board", "heapsize", 512)))
            board_config.append("Optimization: {}".format(self.configIDE.config("Board", "optimization", "-O3")))

        if board.bldr == "noboot":
            board_config.append("Mode: ICSP")
        else:
            board_config.append("Bootloader: {}".format(board.bldr))

        return "\n".join(board_config)


    #----------------------------------------------------------------------
    def get_status_board(self):

        self.set_board()
        board = self.pinguinoAPI.get_board()
        board_config = [board.name]

        if board.bldr == "noboot":
            board_config.append("Mode: ICSP")

        else:
            board_config.append("Bootloader: {}".format(board.bldr))
            # if board.arch == 8 and board.bldr == "boot2":
                # board_config += " | Bootloader: v1 & v2"

        if board.arch == 8:
            board_config.append("Compiler: {}".format(self.configIDE.config("Board", "compiler", "XC8")))
        elif board.arch == 32:
            board_config.append("Compiler: GCC")


        return " | ".join(board_config)


    #----------------------------------------------------------------------
    def get_current_filename(self):
        """"""
        filename = str(self.get_tab().tabText(self.get_tab().currentIndex()))
        if filename.endswith("*"):
            filename = filename[:-1]
        return filename

    # #----------------------------------------------------------------------
    # def get_curren_path_filename(self):
        # """"""
        # editor = self.get_current_editor



    #----------------------------------------------------------------------
    def get_current_hex(self):
        """"""
        editor = self.get_current_editor()
        if hasattr(editor, "path"):
            filename = editor.path.replace(".pde", ".hex").replace(".gpde", ".hex")
            if os.path.exists(filename):
                return filename
            else:
                return False
        else:
            return False


    #----------------------------------------------------------------------
    def get_default_dir(self):
        """"""
        if self.is_project() and self.is_library():
            return self.get_library_path()
        elif self.is_project():
            return self.get_default_project_dir()
        elif self.is_graphical() != None:
            editor = self.get_current_editor()
            return getattr(editor, "path", os.getenv("PINGUINO_DEFAULT_FILES"))
        else:
            return os.getenv("PINGUINO_DEFAULT_FILES")



########################################################################
class PinguinoSettings(object):
    """"""

    #----------------------------------------------------------------------
    def change_icon_theme(self, theme, action):

        def set_theme():
            QtGui.QIcon.setThemeName(theme)
            self.reload_toolbar_icons()
            self.configIDE.set("Main", "theme", theme)
            self.configIDE.save_config()

            [act.setChecked(False) for act in self.main.menuIcons_theme.actions()]
            action.setChecked(True)

        return set_theme


    #----------------------------------------------------------------------
    def clear_recents_menu(self):

        self.main.menuRecents.clear()
        self.main.menuRecents.addSeparator()
        self.main.menuRecents.addAction(QtGui.QApplication.translate("Dialogs", "Clear recent files"), self.clear_recents_menu)
        self.recent_files = []


    #----------------------------------------------------------------------
    @Decorator.call_later()
    def load_main_config(self):

        if self.configIDE.config("Main", "maximized", True):
            self.showMaximized()
            self.setWindowState(QtCore.Qt.WindowMaximized)

        else:
            pos = self.configIDE.config("Main", "position", "(0, 0)")
            self.move(*eval(pos))

            size = self.configIDE.config("Main", "size", "(1050, 550)")
            self.resize(*eval(size))


        visible = self.configIDE.config("Main", "menubar", True)
        self.toggle_menubar(visible)
        self.main.actionMenubar.setChecked(visible)

        # self.switch_ide_mode(self.configIDE.config("Features", "graphical", False))

        self.main.actionAutocomplete.setChecked(self.configIDE.config("Features", "autocomplete", True))

        self.toggle_side_vertical_area(load=True)

        self.load_tabs_config()




    #----------------------------------------------------------------------
    def load_tabs_config(self):
        """"""
        names = []
        exclude = ["Blocks"]

        tabs = self.main.tabWidget_bottom.tabBar()
        for index in range(tabs.count()):
            names.append(self.main.tabWidget_bottom.widget(index).objectName())

        tabs = self.main.tabWidget_tools.tabBar()
        for index in range(tabs.count()):
            names.append(self.main.tabWidget_tools.widget(index).objectName())

        for name in names:
            if name in exclude:
                continue
            if not self.configIDE.config("Tabs", name, True):
                name = self.configIDE.config("Tabs", "{}_name".format(name), None)
                if hasattr(self.main, name):
                    getattr(self.main, "actionTab{}".format(name)).setChecked(False)





    #----------------------------------------------------------------------
    def reload_toolbar_icons(self):

        self.toolbars = [self.main.toolBar,
                         self.main.toolBar_menu,
                         # self.main.toolBar_switch,
                         ]

        for toolbar in self.toolbars:
            toolbar.setToolButtonStyle(QtCore.Qt.ToolButtonIconOnly)  #explicit IconOnly for windows
            size = self.configIDE.config("Main", "icons_size", 24)
            self.resize_toolbar(size, getattr(self.main, "action{}x{}".format(size, size)))()
            getattr(self.main, "action{}x{}".format(size, size)).setChecked(True)

        icons_toolbar = [
                         (self.main.actionNew_file, "document-new"),
                         (self.main.actionNew_blocks_file, "insert-object"),
                         (self.main.actionOpen_file, "document-open"),
                         (self.main.actionSave_file, "document-save"),

                         (self.main.actionUndo, "edit-undo"),
                         (self.main.actionRedo, "edit-redo"),
                         (self.main.actionCut, "edit-cut"),
                         (self.main.actionCopy, "edit-copy"),
                         (self.main.actionPaste, "edit-paste"),

                         (self.main.actionSearch, "edit-find"),
                         # (self.main.actionSearch_and_replace, "edit-find"),

                         (self.main.actionSelect_board, "applications-electronics"),
                         (self.main.actionCompile, "system-run"),
                         (self.main.actionUpload, "emblem-downloads"),

                         (self.main.actionSave_image, "applets-screenshooter"),

                         (self.toolbutton_menutoolbar, "preferences-system"),

                        ]

        for action, icon_name in icons_toolbar:
            if QtGui.QIcon.hasThemeIcon(icon_name):
                icon = QtGui.QIcon.fromTheme(icon_name)
                action.setIcon(icon)
                action.setVisible(True)
            else:
                action.setVisible(False)

        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QIcon.fromTheme("insert-text").pixmap(size), QtGui.QIcon.Normal, QtGui.QIcon.On)
        icon.addPixmap(QtGui.QIcon.fromTheme("insert-object").pixmap(size), QtGui.QIcon.Normal, QtGui.QIcon.Off)

        # self.main.actionSwitch_ide.setIcon(icon)


    #----------------------------------------------------------------------
    def set_icon_theme(self):

        QtGui.QIcon.setThemeSearchPaths(QtGui.QIcon.themeSearchPaths()+[os.path.join(os.getenv("PINGUINO_LIB"), "qtgui", "resources", "themes")])
        #paths = filter(lambda path:os.path.isdir(path), QtGui.QIcon.themeSearchPaths())
        paths = [path for path in QtGui.QIcon.themeSearchPaths() if os.path.isdir(path)]
        themes = [(path, os.listdir(path)) for path in paths]

        valid_themes = []
        for path, list_themes in themes:
            for theme in list_themes:
                if os.path.isdir(os.path.join(path, theme)): valid_themes.append(theme)

        self.main.menuIcons_theme.clear()

        dict_themes = {}
        for theme in valid_themes:
            action = QtGui.QAction(self)
            action.setCheckable(True)
            action.setText(theme.capitalize().replace("-", " "))
            self.connect(action, QtCore.SIGNAL("triggered()"), self.change_icon_theme(theme, action))
            dict_themes[theme] = action
            self.main.menuIcons_theme.addAction(action)

        theme = self.configIDE.config("Main", "theme", "pinguino12")
        if not theme in valid_themes:
            theme = "pinguino12"
            self.configIDE.set("Main", "theme", "pinguino12")
        self.change_icon_theme(theme, dict_themes[theme])()
        dict_themes[theme].setChecked(True)



    #----------------------------------------------------------------------
    def resize_toolbar(self, size, action):

        def resize_icons():
            for toolbar in self.toolbars:
                toolbar.setToolButtonStyle(QtCore.Qt.ToolButtonIconOnly)
                toolbar.setIconSize(QtCore.QSize(size, size))

            [act.setChecked(False) for act in self.main.menuIcons_size.actions()]
            action.setChecked(True)
            self.configIDE.set("Main", "icons_size", size)

            self.toolbutton_menutoolbar.setIconSize(QtCore.QSize(size, size))

        return resize_icons




    #----------------------------------------------------------------------
    def switch_autocomplete(self):
        enable = self.main.actionAutocomplete.isChecked()
        self.configIDE.set("Features", "autocomplete", enable)
        self.configIDE.save_config()


    #----------------------------------------------------------------------
    def switch_color_theme(self, pinguino_color=True):
        default_pallete = ["toolBar", "toolBar_menu", "menubar", "statusBar"]
        pinguino_pallete = ["dockWidget_bottom", "dockWidget_right"]

        if pinguino_color:
            self.PinguinoPallete.set_background_pinguino(self.main.centralwidget.parent())
            for element in pinguino_pallete:
                self.PinguinoPallete.set_background_pinguino(getattr(self.main, element))
            for element in default_pallete:
                self.PinguinoPallete.set_default_palette(getattr(self.main, element))
            self.main.label_logo.setPixmap(QtGui.QPixmap(":/logo/art/banner.png"))
        else:
            self.PinguinoPallete.set_default_palette(self.main.centralwidget.parent())
            for element in default_pallete + pinguino_pallete:
                self.PinguinoPallete.set_default_palette(getattr(self.main, element))
            self.main.label_logo.setPixmap(QtGui.QPixmap(":/logo/art/banner_blue.png"))

        self.configIDE.set("Main", "color_theme", pinguino_color)
        self.main.actionColor_theme.setChecked(pinguino_color)


    #----------------------------------------------------------------------
    def switch_confirm_board(self, event=None):
        enable = self.main.actionConfirm_board.isChecked()
        self.configIDE.set("Features", "confirm_board", enable)
        self.configIDE.save_config()



    # #----------------------------------------------------------------------
    # def switch_ide_mode(self, graphical):
        # self.main.actionSwitch_ide.setChecked(graphical)
        # self.main.tabWidget_files.setVisible(graphical and self.main.tabWidget_files.count() > 0)
        # self.main.tabWidget_files.setVisible(not graphical and self.main.tabWidget_files.count() > 0)


        # menu = self.toolbutton_menutoolbar.menu()
        # if graphical:
            # self.update_actions_for_graphical()
            # # self.main.menubar.setVisible(False)
            # # self.toggle_toolbars(True)
            # # menu.insertMenu(self.main.menuHelp.menuAction(), self.main.menuGraphical)
        # else:
            # self.update_actions_for_text()
            # # self.main.menubar.setVisible(True)
            # # menu.removeAction(self.main.menuGraphical.menuAction())

        # self.ide_tab_changed()




    #----------------------------------------------------------------------
    def toggle_right_area(self, expand=None):

        if expand is None:
            expand = (self.main.dockWidgetContents_rigth.width() <= (self.get_tabs_height()) + 5)
        min_ = self.get_tabs_height()

        if expand:
            h = self.configIDE.config("Main", "right_area_width", 400)
            if h <= (self.get_tabs_height() + 5):
                h = 400
                self.configIDE.set("Main", "right_area_width", 400)

            self.main.dockWidgetContents_rigth.setMaximumWidth(1000)
            self.main.dockWidgetContents_rigth.setMinimumWidth(h)
            self.main.actionToggle_vertical_tool_area.setText("Hide vertical tool area")
            QtCore.QTimer.singleShot(100, lambda :self.main.dockWidgetContents_rigth.setMinimumWidth(0))
        else:
            self.configIDE.set("Main", "right_area_width", self.main.dockWidgetContents_rigth.width())
            self.main.dockWidgetContents_rigth.setMinimumWidth(0)
            self.main.dockWidgetContents_rigth.setMaximumWidth(min_)
            self.main.actionToggle_vertical_tool_area.setText("Show vertical tool area")
            QtCore.QTimer.singleShot(100, lambda :self.main.dockWidgetContents_rigth.setMaximumWidth(1000))
            QtCore.QTimer.singleShot(100, lambda :self.main.dockWidgetContents_rigth.setMinimumWidth(0))


    #----------------------------------------------------------------------
    def toggle_bottom_area(self, expand=None):

        if expand is None:
            expand = (self.main.dockWidgetContents_bottom.height() <= (self.get_tabs_height()) + 5)
        min_ = self.get_tabs_height()

        if expand:
            h = self.configIDE.config("Main", "bottom_area_height", 200)
            if h <= (self.get_tabs_height() + 5):
                h = 200
                self.configIDE.set("Main", "bottom_area_height", 200)

            self.main.dockWidgetContents_bottom.setMaximumHeight(1000)
            self.main.dockWidgetContents_bottom.setMinimumHeight(h)
            self.main.actionToggle_horizontal_tool_area.setText("Hide horizontal tool area")
            QtCore.QTimer.singleShot(100, lambda :self.main.dockWidgetContents_bottom.setMinimumHeight(0))
        else:
            self.configIDE.set("Main", "bottom_area_height", self.main.dockWidgetContents_bottom.height()-3)
            self.main.dockWidgetContents_bottom.setMinimumHeight(0)
            self.main.dockWidgetContents_bottom.setMaximumHeight(min_)
            self.main.actionToggle_horizontal_tool_area.setText("Show horizontal tool area")
            QtCore.QTimer.singleShot(100, lambda :self.main.dockWidgetContents_bottom.setMaximumHeight(1000))
            QtCore.QTimer.singleShot(100, lambda :self.main.dockWidgetContents_bottom.setMinimumHeight(0))


    #----------------------------------------------------------------------
    def tab_right_changed(self, event):

        self.toggle_right_area(expand=True)


    #----------------------------------------------------------------------
    def tab_bottoms_changed(self, event):

        self.toggle_bottom_area(expand=True)


    #----------------------------------------------------------------------
    def tab_tools_resize(self, event):

        self.configIDE.set("Main", "right_area_width", self.main.dockWidgetContents_rigth.width())
        self.configIDE.set("Main", "bottom_area_height", self.main.dockWidgetContents_bottom.height()-3)


    # #----------------------------------------------------------------------
    # def toggle_editor_area(self, expand):

        # self.toggle_right_area(not expand)
        # self.toggle_bottom_area(not expand)
        # self.toggle_toolbars(not expand)
        # self.main.actionToolbars.setChecked(not expand)

        # self.main.actionMenubar.setChecked(not expand)
        # self.main.menubar.setVisible(expand)
        # self.main.actionMenubar.setVisible(not expand)


    #----------------------------------------------------------------------
    def toggle_side_vertical_area(self, load=False):

        if load:
            position = self.configIDE.config("Main", "dock_tools", "RightDockWidgetArea")
            # position = getattr(QtCore.Qt, position)
        else:
            # Invert current position and saved in position
            side = self.dockWidgetArea(self.main.dockWidget_right)
            position = {"RightDockWidgetArea": "LeftDockWidgetArea",
                        "LeftDockWidgetArea": "RightDockWidgetArea",}[side.name.decode()]
            self.configIDE.set("Main", "dock_tools", position)

        self.addDockWidget(QtCore.Qt.DockWidgetArea(getattr(QtCore.Qt, position)), self.main.dockWidget_right)

        if position == "LeftDockWidgetArea":
            self.main.tabWidget_tools.setTabPosition(QtGui.QTabWidget.East)
            # self.main.tabWidget_blocks.setTabPosition(QtGui.QTabWidget.East)
            self.main.actionMove_vertical_tool_area.setText("Move vertical tool area to right")
        else:
            self.main.tabWidget_tools.setTabPosition(QtGui.QTabWidget.West)
            # self.main.tabWidget_blocks.setTabPosition(QtGui.QTabWidget.West)
            self.main.actionMove_vertical_tool_area.setText("Move vertical tool area to left")


    #----------------------------------------------------------------------
    def toggle_toolbars(self, visible):

        for toolbar in self.toolbars:
            toolbar.setVisible(visible)

        # if self.is_graphical():
            # self.update_actions_for_graphical()
        # else:
            # self.update_actions_for_text()

        self.update_actions()


    #----------------------------------------------------------------------
    def toggle_menubar(self, event=None):

        if event is None:
            event = not self.main.menubar.isVisible()

        self.main.menubar.setVisible(event)
        if not event:
            self.toggle_toolbars(True)
        self.main.toolBar_menu.setVisible(not event)
        self.configIDE.set("Main", "menubar", event)
        self.configIDE.save_config()




########################################################################
class PinguinoMain(object):
    """"""

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_file_saved()
    @Decorator.requiere_can_compile()
    def pinguino_compile(self, dialog_upload=True):

        self.ide_save_file()

        filename = self.get_tab().currentWidget().path

        if not os.path.exists(filename): return

        if self.is_graphical() is False:
            if self.is_project() and not self.is_library():
                self.ide_save_all()
                filenames = self.get_project_files()
                if not filenames:
                    Dialogs.add_files_to_project(self)
                    return
                compile_code = lambda :self.pinguinoAPI.compile_file(filenames)
            else:
                if self.is_project() and self.is_library():
                    self.ide_save_all()
                    self.refresh_libraries()
                    logging.info("Adding library to environment...")
                compile_code = lambda :self.pinguinoAPI.compile_file([filename])

        else:
            if self.is_project():
                self.ide_save_all()
                filenames = self.get_project_files()
                if not filenames:
                    Dialogs.add_files_to_project(self)
                    return
                compile_code = lambda :self.pinguinoAPI.compile_string(self.PinguinoKIT.get_pinguino_source_code(), filenames=filenames)
            else:
                compile_code = lambda :self.pinguinoAPI.compile_string(self.PinguinoKIT.get_pinguino_source_code())


        if self.is_project() and not self.is_library():
            self.ide_save_all()
            self.write_log(QtGui.QApplication.translate("Frame", "Compiling: {}".format(self.get_project_name())))
        else:
            self.write_log(QtGui.QApplication.translate("Frame", "Compiling: {}".format(filename)))

        self.write_log(self.get_description_board())

        compile_code()
        self.update_stdout()
        self.post_compile(dialog_upload)
        self.main.actionHex.setVisible(bool(self.get_current_hex()))


    #----------------------------------------------------------------------
    def post_compile(self, dialog_upload=True):

        self.main.actionUpload.setEnabled(self.pinguinoAPI.compiled())
        if not self.pinguinoAPI.compiled():

            if self.is_project() and not self.is_library():
                self.ide_show_user_c()

            # errors_preprocess = self.pinguinoAPI.get_errors_preprocess()
            # if errors_preprocess:
                # for error in errors_preprocess:
                    # self.write_log("ERROR: {}".format(errors_preprocess))

            errors_c = self.pinguinoAPI.get_errors_compiling_c()
            if errors_c:
                self.write_log("ERROR: {complete_message}".format(**errors_c))
                line_errors = errors_c["line_numbers"]
                for line_error in line_errors:
                    self.editor_highligh_line(line_error, "#ff7f7f")

            errors_asm = self.pinguinoAPI.get_errors_compiling_asm()
            if errors_asm:
                for error in errors_asm["error_symbols"]:
                    self.write_log("ERROR: {}".format(error))

            errors_linking = self.pinguinoAPI.get_errors_linking()
            if errors_linking:
                for error in errors_linking["linking"]:
                    self.write_log("ERROR: "+error)

                line_errors_l = errors_linking["line_numbers"]
                for line_error in line_errors_l:
                    self.editor_highligh_line(line_error, "#ff7f7f")


            if errors_asm or errors_c:
                Dialogs.error_message(self, "Error while compiling.")
                self.set_tab_stdout()
            elif errors_linking:
                Dialogs.error_message(self, "Error while linking.")
                self.set_tab_stdout()
            # elif errors_preprocess:
                # Dialogs.error_while_preprocess(self)
                # self.set_tab_stdout()

            else:
                Dialogs.error_message(self, "Unknow error.")
                self.set_tab_stdout()
                self.submit_stdout()

        else:
            result = self.pinguinoAPI.get_result()
            self.write_log(QtGui.QApplication.translate("Frame", "compilation done"))
            self.write_log(result["code_size"])
            self.write_log(QtGui.QApplication.translate("Frame", "{time} seconds process time".format(**result)))

            if dialog_upload:
                if Dialogs.compilation_done(self):
                    self.pinguino_upload()


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def pinguino_upload(self):
        uploaded, result = self.pinguinoAPI.upload()
        self.write_log(result)
        if uploaded:
            Dialogs.upload_done(self)
        elif Dialogs.upload_fail(self, result):
            self.pinguino_upload()


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_file_saved()
    def pinguino_compile_and_upload(self):
        self.pinguino_compile(dialog_upload=False)
        if self.pinguinoAPI.compiled():
            self.pinguino_upload()


    #----------------------------------------------------------------------
    def pinguino_upload_hex(self):

        Dialogs.warning_message(self, "Be careful with this feature, ensure that .hex file it is correct.")

        path = QtCore.QDir.home().path()
        filename = Dialogs.set_open_hex(self, path)

        if not filename: return

        self.set_board()
        # reply = Dialogs.confirm_board(self)

        # if reply == False:
            # self.set_tab_board()
            # return False
        # elif reply == None:
            # return False

        board = self.pinguinoAPI.get_board()
        reply = Dialogs.confirm_message(self, "Do you want upload '{}' to {}".format(filename, board.name))

        if reply:
            self.pinguinoAPI.__hex_file__ = filename
            self.pinguino_upload()



    #----------------------------------------------------------------------
    def set_board(self):

        # config data
        board_name = self.configIDE.config("Board", "board", "Pinguino 2550")
        arch = self.configIDE.config("Board", "arch", 8)
        mode = self.configIDE.config("Board", "mode", "boot")
        # bootloader = self.configIDE.config("Board", "bootloader", "v1_v2")


        # set board
        for board in self.pinguinoAPI._boards_:
            if board.name == board_name:
                self.pinguinoAPI.set_board(board)


        # set mode and bootloader
        # if arch == 8 and mode == "bootloader":
            # if bootloader == "v1_v2":
                # self.pinguinoAPI.set_bootloader(*self.pinguinoAPI.Boot2)
            # else:
                # self.pinguinoAPI.set_bootloader(*self.pinguinoAPI.Boot4)

        # no configuration bootloader for 32 bits

        if mode == "icsp":
            # if mode is icsp overwrite all configuration
            self.pinguinoAPI.set_icsp()


        # update environment
        os.environ["PINGUINO_BOARD_ARCH"] = str(arch)


        # set compilers and libraries for each arch
        # RB20150127 : modified until I can compile p32-gcc for mac os x
        #elif os.getenv("PINGUINO_OS_NAME") == "linux":
        if os.getenv("PINGUINO_OS_NAME") == "windows":
            ext = ".exe"
        else :
            ext = ""

        if arch == 8:
            compiler_path = os.path.join(self.configIDE.get_path("sdcc_bin"), "sdcc" + ext)
            libraries_path = self.configIDE.get_path("pinguino_8_libs")

        elif arch == 32:
            #RB20140615 + RB20141116 + RB20150127 :
            #- gcc toolchain has been renamed from mips-elf-gcc to p32-gcc except for MAC OS X
            if os.getenv("PINGUINO_OS_NAME") == "macosx":
                compiler_path = os.path.join(self.configIDE.get_path("gcc_bin"), "mips-elf-gcc" + ext)
            else:
                compiler_path = os.path.join(self.configIDE.get_path("gcc_bin"), "p32-gcc" + ext)
            #- except for 32-bit Windows
            #if os.getenv("PINGUINO_OS_NAME") == "windows":
            #    if os.getenv("PINGUINO_OS_ARCH") == "32bit":
            #        compiler_path = os.path.join(self.configIDE.get_path("gcc_bin"), "mips-gcc" + ext)
            libraries_path = self.configIDE.get_path("pinguino_32_libs")


        # generate messages
        status = ""
        if not os.path.exists(compiler_path):
            status = QtGui.QApplication.translate("Frame", "Missing compiler for {}-bit".format(arch))
            logging.warning("Missing compiler for {}-bit".format(arch))
            logging.warning("Not found: {}".format(compiler_path))

        if not os.path.exists(libraries_path):
            status = QtGui.QApplication.translate("Frame", "Missing libraries for {}-bit".format(arch))
            logging.warning("Missing libraries for {}-bit".format(arch))
            logging.warning("Not found: {}".format(libraries_path))

        if not os.path.exists(libraries_path) and not os.path.exists(compiler_path):
            status = QtGui.QApplication.translate("Frame", "Missing libraries and compiler for {}-bit".format(arch))

        if status:
            self.status_warnnig.setText(status)
            os.environ["PINGUINO_CAN_COMPILE"] = "False"
        else:
            os.environ["PINGUINO_CAN_COMPILE"] = "True"





########################################################################
class PinguinoCore(PinguinoComponents, PinguinoChilds, PinguinoQueries, PinguinoSettings, PinguinoMain):


    #----------------------------------------------------------------------
    def __startapp__(self):
        """This method will be runned with IDE starting."""

        # Search updates
        if os.getenv("PINGUINO_MODE") == "NORMAL":
            self.need_update()

        # Try to send failed submittes
        try:send_old_submits()
        except: pass


        # PythonShell.__init__(self)


    #----------------------------------------------------------------------
    def show_message(self, frame, msg):
        """"""
        if frame:
            Dialogs.info_message(self, msg)
        else:
            logging.info(msg)


    #----------------------------------------------------------------------
    def need_update(self):
        """"""
        url = "https://api.github.com/repos/PinguinoIDE/pinguino-ide/releases/latest"

        if not hasattr(self, "thread_response"):
            self.thread_response = PinguinoRequest(url)
            self.thread_response.signal_message.connect(self.show_message)
            self.thread_response.setTerminationEnabled(True)

        if not self.thread_response.isRunning():
            self.thread_response.start()


    #----------------------------------------------------------------------
    def ide_open_file_from_path(self, *args, **kwargs):
        filename = kwargs["filename"]
        readonly = kwargs.get("readonly", False)

        if self.ide_check_duplicate_file(filename): return

        self.update_recents(filename)

        if filename.endswith(".gpde"):
            # self.switch_ide_mode(True)
            self.PinguinoKIT.ide_open_file_from_path(filename=filename)
            editor = self.get_current_editor()
            pde_file = codecs.open(filename, "r", encoding="utf-8")
            content = "".join(pde_file.readlines())
            pde_file.close()
        elif filename.endswith(".ppde"):
            self.open_project_from_path(filename=filename)
            return

        else:
            self.ide_new_file(filename=filename)
            editor = self.get_current_editor()
            #pde_file = open(path, mode="r")
            pde_file = codecs.open(filename, "r", encoding="utf-8")
            content = "".join(pde_file.readlines())
            pde_file.close()
            editor.text_edit.setPlainText(content)
            editor.text_edit.setReadOnly(readonly)
            self.main.tabWidget_files.setTabToolTip(self.main.tabWidget_files.currentIndex(), filename)
            if readonly: extra_name = " (r/o)"
            else: extra_name = ""
            self.main.tabWidget_files.setTabText(self.main.tabWidget_files.currentIndex(), os.path.basename(filename)+extra_name)

        setattr(editor, "path", filename)
        setattr(editor, "last_saved", content)

        self.ide_check_backup_file(editor=editor)
        self.save_recents()
        # self.ide_tab_changed()



    #----------------------------------------------------------------------
    @Decorator.call_later()
    #@Decorator.debug_time()
    def ide_open_last_files(self):
        self.recent_files = self.configIDE.get_recents()
        self.update_recents_menu()

        self.recent_projects = self.configIDE.get_recents(section="RecentsProjects")
        self.update_recents_menu_project()

        opens = self.configIDE.get_recents_open()
        # if not opens:
            # self.pinguino_ide_manual()
            # return

        self.setCursor(QtCore.Qt.WaitCursor)
        for file_ in opens:
            if os.path.exists(file_):
                try: self.ide_open_file_from_path(filename=file_)
                except: pass

        # self.main.actionSwitch_ide.setChecked(file_.endswith(".gpde"))
        # self.switch_ide_mode(file_.endswith(".gpde"))
        self.setCursor(QtCore.Qt.ArrowCursor)
        # self.thread_variables()


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    def editor_select_block_edit(self):
        editor = self.get_current_editor()
        cursor = editor.text_edit.textCursor()
        prevCursor = editor.text_edit.textCursor()

        text = cursor.selectedText()
        selected = bool(text)

        if text == "":  #no selected, single line
            start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
            startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()
            endPosition = editor.text_edit.document().findBlockByLineNumber(start+1).position() - 1

            cursor.setPosition(startPosition)
            cursor.setPosition(endPosition, QtGui.QTextCursor.KeepAnchor)
            editor.text_edit.setTextCursor(cursor)

        else:
            start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
            startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()

            end = editor.text_edit.document().findBlock(cursor.selectionEnd()).firstLineNumber()

            endPosition = editor.text_edit.document().findBlockByLineNumber(end+1).position() - 1

            cursor.setPosition(startPosition)
            cursor.setPosition(endPosition, QtGui.QTextCursor.KeepAnchor)
            editor.text_edit.setTextCursor(cursor)


        text = cursor.selectedText()

        lines = text.split(u'\u2029')
        firstLine = False
        for line in lines:
            if not line.isspace() and not line == "":
                firstLine = line
                break
        return editor, cursor, prevCursor, selected, firstLine



    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    def editor_highligh_line(self, line=None, color="#ff0000", text_cursor=None):

        editor = self.get_current_editor()

        if line:
            content = editor.text_edit.toPlainText()
            #line_content = content.split("\n")[line-1]
            content = content.split("\n")[:line]
            position = len("\n".join(content))
            text_cur = editor.text_edit.textCursor()
            text_cur.setPosition(position)
            text_cur.clearSelection()
            editor.text_edit.setTextCursor(text_cur)
        else:
            text_cur = editor.text_edit.textCursor()
            text_doc = editor.text_edit.document()
            text_cur.clearSelection()
            editor.text_edit.setDocument(text_doc)
            editor.text_edit.setTextCursor(text_cur)

        selection = QtGui.QTextEdit.ExtraSelection()
        selection.format.setBackground(QtGui.QColor(color))
        selection.format.setProperty(QtGui.QTextFormat.FullWidthSelection, True)
        selection.cursor = editor.text_edit.textCursor()
        editor.text_edit.setExtraSelections(editor.text_edit.extraSelections()+[selection])

        selection.cursor.clearSelection()

        if text_cursor: editor.text_edit.setTextCursor(text_cursor)


    #----------------------------------------------------------------------
    # @Decorator.requiere_text_mode()
    def editor_clear_highlighted_lines(self):

        if os.getenv("PINGUINO_PROJECT"):
            for editor in self.get_all_editors(text=True):
                editor.text_edit.setExtraSelections([])
        else:
            editor = self.get_current_editor()
            editor.text_edit.setExtraSelections([])

    #----------------------------------------------------------------------
    # @Decorator.connect_features()
    def __save_file__(self, *args, **kwargs):

        editor = kwargs.get("editor", self.get_tab())
        if self.is_graphical(editor):
            self.PinguinoKIT.__save_file__(*args, **kwargs)
        elif self.is_graphical(editor) is None:
            return

        content = editor.text_edit.toPlainText()
        pde_file = codecs.open(editor.path, "w", encoding="utf-8")
        pde_file.write(content)
        pde_file.close()
        setattr(editor, "last_saved", content)
        self.ide_remove_backup_file(editor=editor)
        self.editor_saved()

    #----------------------------------------------------------------------
    def ide_remove_backup_file(self, *args, **kwargs):

        editor = kwargs.get("editor", self.get_tab())
        filename = getattr(editor, "path", None)
        filename_backup = filename + "~"
        if os.path.exists(filename_backup):
            os.remove(filename_backup)


    #----------------------------------------------------------------------
    def editor_changed(self, *args, **kwargs):

        index = self.main.tabWidget_files.currentIndex()
        filename = self.main.tabWidget_files.tabText(index)
        if not filename.endswith("*"):
            self.main.tabWidget_files.setTabText(index, filename+"*")
            self.main.actionSave_file.setEnabled(True)
        self.editor_clear_highlighted_lines()


    #----------------------------------------------------------------------
    def editor_saved(self, *args, **kwargs):

        index = self.get_tab().currentIndex()
        filename = self.get_tab().tabText(index)
        if filename.endswith("*"):
            self.get_tab().setTabText(index, filename[:-1])
        self.main.actionSave_file.setEnabled(False)


    #----------------------------------------------------------------------
    def editor_can_undo(self, available):

        self.main.actionUndo.setEnabled(available)


    #----------------------------------------------------------------------
    def editor_can_redo(self, available):

        self.main.actionRedo.setEnabled(available)


    #----------------------------------------------------------------------
    def editor_can_copy(self, available):

        self.main.actionCopy.setEnabled(available)
        self.main.actionCut.setEnabled(available)


    #----------------------------------------------------------------------
    def ide_check_duplicate_file(self, filename):

        filenames = [getattr(self.get_tab().widget(i), "path", None) for i in range(self.get_tab().count())]
        if filename in filenames:
            # Dialogs.file_duplicated(self, filename)
            self.get_tab().setCurrentIndex(filenames.index(filename))
            return True
        return False





    #----------------------------------------------------------------------
    def update_recents(self, filename):

        if filename in self.recent_files:
            self.recent_files.remove(filename)
        self.recent_files.insert(0, filename)
        self.recent_files = self.recent_files[:10]

        self.update_recents_menu()


    #----------------------------------------------------------------------
    def update_recents_menu(self):

        self.main.menuRecents.clear()
        for file_ in self.recent_files:
            action = QtGui.QAction(self)
            filename = os.path.split(file_)[1]

            len_ = 40
            if len(file_) > len_:
                file_path_1 = file_[:int(len_/2)]
                file_path_2 = file_[int(-len_/2):]
                file_path = file_path_1 + "..." + file_path_2
            else: file_path = file_

            if os.path.isfile(file_):
                action.setText(filename+" ("+file_path+")")
                self.connect(action, QtCore.SIGNAL("triggered()"), self.ide_menu_recent_event(file_))
                action.ActionEvent = self.ide_menu_recent_event

                self.main.menuRecents.addAction(action)

        self.main.menuRecents.addSeparator()
        self.main.menuRecents.addAction(QtGui.QApplication.translate("Dialogs", "Clear recent files"), self.clear_recents_menu)



    #----------------------------------------------------------------------
    def ide_menu_recent_event(self, file_):

        def menu():
            self.ide_open_file_from_path(filename=file_)
        return menu


    #----------------------------------------------------------------------
    def build_statusbar(self):

        #principal status
        self.status_info = QtGui.QLabel()

        #warning status
        self.status_warnnig = QtGui.QLabel()
        self.status_warnnig.setAlignment(QtCore.Qt.AlignRight)
        self.status_warnnig.setStyleSheet("""
        QLabel{
            color: red;
        }
        """)

        self.main.statusBar.addPermanentWidget(self.status_info, 1)
        self.main.statusBar.addPermanentWidget(self.status_warnnig, 2)



    # #----------------------------------------------------------------------
    # def statusbar_ide(self, status):

        # self.status_info.setText(status)

    # #----------------------------------------------------------------------
    # def statusbar_warnning(self, status):

        # self.status_warnnig.setText(status)


    #----------------------------------------------------------------------
    def editor_reload_file(self):

        editor = self.get_current_editor()
        filename = getattr(editor, "path", False)
        file_ = codecs.open(filename, "r", encoding="utf-8")
        editor.text_edit.clear()
        editor.text_edit.insertPlainText("".join(file_.readlines()))
        self.ide_save_file()


    #----------------------------------------------------------------------
    def update_reserved_words(self):
        """"""
        helpers = {}
        assistant = {}

        libinstructions8 = self.pinguinoAPI.read_lib(8)
        name_spaces_8 = list(map(lambda x:x["pinguino"], libinstructions8))

        libinstructions32 = self.pinguinoAPI.read_lib(32)
        name_spaces_32 = list(map(lambda x:x["pinguino"], libinstructions32))

        core = os.path.join(os.getenv("PINGUINO_INSTALL_PATH"), "p8", "include", "pinguino", "core")
        libraries = os.path.join(os.getenv("PINGUINO_INSTALL_PATH"), "p8", "include", "pinguino", "libraries")

        lib_paths = []

        # From core
        for filename in os.listdir(core):
            lib_paths.append((filename, core))

        # From libraries
        for filename in os.listdir(libraries):
            lib_paths.append((filename, libraries))

        # From user
        for path in PinguinoConfig.get_p8_libraries() + PinguinoConfig.get_p32_libraries():
            for filename in os.listdir(path):
                lib_paths.append((filename, path))

        files = []
        for filename, path in lib_paths:
            if filename.endswith(".c"):
                try:
                    files.append({"filename": filename,
                                  "content": open(os.path.join(path, filename), mode="r").read(),
                                  })
                except Exception as msg:
                    logging.error("{}: {}".format(filename, msg))

        parser = PinguinoParser(files)

        functions = {function["name"]:function for function in parser.get_functions()}

        directives = {}
        for directive in parser.get_directives():
            if directive["name"].endswith(".h"):
                if directive["filename"] in directives:
                    directives[directive["filename"]].append(directive)
                else:
                    directives[directive["filename"]] = [directive]


        for instruction in libinstructions8 + libinstructions32:
            if instruction["c"] in functions:
                args = ""
                if functions[instruction["c"]]["args"] != "void":
                    args = functions[instruction["c"]]["args"]
                helpers[instruction["pinguino"]] = "{}({{{{{}}}}});".format(instruction["pinguino"], args)
                try:
                    headers = [directive["name"] for directive in directives[functions[instruction["c"]]["filename"]]]
                except KeyError:
                    headers = []
                info_funtion = functions[instruction["c"]]
                info_funtion["headers"] = headers
                assistant[instruction["pinguino"]] = info_funtion

        reserved_filename = os.path.join(os.getenv("PINGUINO_USER_PATH"), "reserved.pickle")

        name_spaces_commun = []

        copy_32 = name_spaces_32[:]
        for name in name_spaces_8:
            if name in copy_32:
                name_spaces_8.remove(name)
                name_spaces_32.remove(name)
                name_spaces_commun.append(name)

        namespaces = {"arch8": name_spaces_8,
                      "arch32": name_spaces_32,
                      "all": name_spaces_commun,
                      "helpers": helpers,
                      "assistant": assistant,
                      }

        pickle.dump(namespaces, open(reserved_filename, "wb"), protocol=2)
        # logging.warning("Writing: " + reserved_filename)
        # return "Writing: {}" .format(reserved_filename)


    #----------------------------------------------------------------------
    @Decorator.requiere_text_mode()
    def ide_check_backup_file(self, *args, **kwargs):

        editor = kwargs.get("editor", self.get_tab())
        if editor:

            filename = getattr(editor, "path")
            filename_backup = filename + "~"

            if os.path.exists(filename_backup):
                backup_file = codecs.open(filename_backup, "r", encoding="utf-8")
                content = "".join(backup_file.readlines())
                backup_file.close()

                # if self.is_graphical() is True:
                    # content = self.PinguinoKIT.get_gpde()
                    # content = self.PinguinoKIT.get_raw_parser(content)

                if self.is_graphical() is False:
                    if editor.text_edit.toPlainText() == content:
                        os.remove(filename_backup)
                        return

                reply = Dialogs.confirm_message(self, "{}\n".format(filename) + QtGui.QApplication.translate("Dialogs",
                        "Pinguino IDE has found changes that were not saved during your last session.\nDo you want recover it?."))

                if reply:
                    editor.text_edit.setPlainText(content)
                    os.remove(filename_backup)



    #----------------------------------------------------------------------
    def ide_restart_now(self):

        import time
        import sys
        logging.warning("Restarting now...")
        time.sleep(1)

        # python = sys.executable
        file_ = os.path.join(os.getenv("PINGUINO_LIB"), "pinguino.py")

        os.execv(file_, sys.argv)



    # #----------------------------------------------------------------------
    # def drag_tab(self, *args, **kwargs):
        # """"""
        # logging.debug("DRAG")



    # #----------------------------------------------------------------------
    # def pop_out(self, widget):
        # """"""
        # index = self.main.tabWidget_bottom.indexOf(widget)
        # if index != -1:
            # tab_name = self.main.tabWidget_bottom.tabText(index)
            # self.main.tabWidget_bottom.removeTab(index)
            # self.floating_tab = Window()
            # self.floating_tab.tab.addTab(widget, tab_name)
            # self.floating_tab.show()


    #----------------------------------------------------------------------
    def editor_set_editable(self):
        """"""
        editor = self.get_current_editor()
        editor.text_edit.setReadOnly(False)
        self.main.tabWidget_files.setTabText(self.main.tabWidget_files.currentIndex(), os.path.basename(editor.path))


    # #----------------------------------------------------------------------
    # def open_as_blocks(self):
        # """"""
        # editor = self.get_current_editor()
        # code = editor.text_edit.toPlainText()
        # code = self.pinguinoAPI.remove_comments(code)
        # blocks = self.PinguinoKIT.code_to_blocks(code)
        # self.PinguinoKIT.open_from_source(blocks)
        # self.switch_ide_mode(graphical=True)


    #----------------------------------------------------------------------
    def ide_check_files(self):
        """"""
        if not os.path.exists(os.path.join(os.getenv("PINGUINO_USER_PATH"), "reserved.pickle")):
            self.update_reserved_words()


    #----------------------------------------------------------------------
    def ide_reset_instalation(self):
        """"""
        path = os.getenv("PINGUINO_USER_PATH")
        if Dialogs.confirm_message(self, "This function remove some files from {} and restart the IDE.\nthis could fix some bugs".format(path)):

            from pinguino import pinguino_reset
            self.ide_restart_now()




    #----------------------------------------------------------------------
    def update_actions(self):

        # actionname: (Blocks, Code, HTML)
        actions = {"actionSave_image":			(True, False, False),
                   "actionSave_file":			(True, True, False),
                   "actionUndo":				(False, True, False),
                   "actionRedo":				(False, True, False),
                   # "actionCopy":				(False, True, False),  #automatic
                   # "actionCut":				(False, True, False),  #automatic
                   "actionPaste":				(False, True, False),
                   "actionSearch":				(False, True, False),
                   "actionCompile":				(True, True, False),
                   "actionUpload":				(True, True, False),
                   "tabWidget_browser":			(True, True, False),
                   "frame_source_assistnant":	(True, True, False),
                   "Search":					(False, True, False),
                   "actionInsert_Block":		(True, False, False),

                   }

        if self.is_graphical() is True:
            self.update_actions_for_graphical(actions, 0)

        elif self.is_graphical() is False:
            self.update_actions_for_text(actions, 1)

        elif self.is_graphical() is None:
            self.update_actions_for_html(actions, 2)

        graph = False
        for i in range(self.get_tab().count()):
            if self.get_tab().tabText(i).endswith(".gpde") or self.get_tab().tabText(i).endswith(".gpde*"):
                graph = True
                break

        if graph:
            if not self.configIDE.config("Tabs", "Blocks", False):
                if hasattr(self.main.Blocks, "tab_parent"):
                    self.toggle_tab("Blocks", True)
                    # Decorator.alert_tab("Blocks")(lambda :None)
                    self.main.comboBox_blocks.setCurrentIndex(0)
                    self.main.stackedWidget_blocks.setCurrentIndex(0)
        else:
            # if self.configIDE.config("Tabs", "Blocks", True):
            self.toggle_tab("Blocks", False)

        self.main.actionHex.setVisible(bool(self.get_current_hex()))


    #----------------------------------------------------------------------
    def update_actions_for_text(self, actions, index):

        for action in actions:
            getattr(self.main, action).setEnabled(actions[action][index])


    #----------------------------------------------------------------------
    def update_actions_for_graphical(self, actions, index):

        # self.main.tabWidget_blocks.setVisible(True)

        for action in actions:
            getattr(self.main, action).setEnabled(actions[action][index])


    #----------------------------------------------------------------------
    def update_actions_for_html(self, actions, index):

        for action in actions:
            getattr(self.main, action).setEnabled(actions[action][index])


    #----------------------------------------------------------------------
    def build_menutoolbar(self):

        self.toolbutton_menutoolbar = QtGui.QToolButton(self)
        self.toolbutton_menutoolbar.setPopupMode(QtGui.QToolButton.MenuButtonPopup)
        menu = QtGui.QMenu()

        icon = QtGui.QIcon.fromTheme("preferences-system")
        self.toolbutton_menutoolbar.setIcon(icon)

        menu.addMenu(self.main.menuFile)
        menu.addMenu(self.main.menuEdit)
        menu.addMenu(self.main.menuProject)
        menu.addMenu(self.main.menuSettings)
        menu.addMenu(self.main.menuSource)
        menu.addMenu(self.main.menuPinguino)
        menu.addMenu(self.main.menuTools)
        menu.addMenu(self.main.menuHelp)

        menu.addSeparator()
        menu.addAction(self.main.actionMenubar)

        self.toolbutton_menutoolbar.setMenu(menu)
        self.main.toolBar_menu.addWidget(self.toolbutton_menutoolbar)

    #----------------------------------------------------------------------
    # @Decorator.update_toolbar()
    def ide_new_file(self, *args, **kwargs):

        path = kwargs.get("filename", self.get_untitled_name())
        filename = os.path.split(path)[1]
        highlighter = filename.endswith(".pde") or filename.endswith(".c") or filename.endswith(".h") or filename.endswith(".cpp") or \
                filename.endswith(".pde*") or filename.endswith(".c*") or filename.endswith(".h*") or filename.endswith(".cpp*") or \
                filename.endswith(".lib") or filename.endswith(".lib*") or filename.endswith(".pdl") or filename.endswith(".pdl*") or \
                filename.endswith(".lib32") or filename.endswith(".lib32*") or filename.endswith(".pdl32") or filename.endswith(".pdl32*") or \
                filename.lower() == "pinguino" or filename.lower() == "readme.md"
        autocompleter = highlighter
        editor = PinguinoCodeEditor(highlighter=highlighter, autocompleter=autocompleter)
        self.main.tabWidget_files.addTab(editor, filename)
        #editor.text_edit.insertPlainText(Snippet["file {snippet}"][1].replace("\t", ""))
        #editor.text_edit.insertPlainText("\n")
        #editor.text_edit.insertPlainText(Snippet["Bare minimum {snippet}"][1].replace("\t", ""))

        if highlighter:
            tc = editor.text_edit.textCursor()
            editor.text_edit.insert("file {snippet}")
            tc.movePosition(tc.End)
            tc.insertText("\n\n")
            editor.text_edit.setTextCursor(tc)
            editor.text_edit.insert("Bare minimum {snippet}")

        self.main.tabWidget_files.setCurrentWidget(editor)
        editor.text_edit.textChanged.connect(self.editor_changed)
        editor.text_edit.undoAvailable.connect(self.editor_can_undo)
        editor.text_edit.redoAvailable.connect(self.editor_can_redo)
        editor.text_edit.copyAvailable.connect(self.editor_can_copy)
        self.main.actionUndo.setEnabled(False)
        self.main.actionRedo.setEnabled(False)
        self.main.actionCopy.setEnabled(False)

        editor.text_edit.dropEvent = self.ide_drop_file
        editor.text_edit.keyPressEvent = self.editor_key_press
        editor.text_edit.contextMenuEvent = self.editor_file_context_menu
        editor.text_edit.setAcceptRichText(False)
        self.main.tabWidget_files.setTabText(self.main.tabWidget_files.currentIndex(), filename[:-1])
        editor.text_edit.setFocus()
        self.update_actions()

        # Decorator.update_toolbar()


    #----------------------------------------------------------------------
    def ide_open_files(self):

        editor = self.get_current_editor()
        dirpath = self.get_default_dir()

        filenames = Dialogs.set_open_files(self, dirpath)
        if filenames is None: return

        for filename in filenames:
            if self.ide_check_duplicate_file(filename): continue

            self.update_recents(filename)
            if filename.endswith(".gpde"):
                # self.switch_ide_mode(True)
                self.PinguinoKIT.ide_open_files(filename=filename)
                return
            # elif filename.endswith(".pde"):
                # self.switch_ide_mode(False)
            self.ide_open_file_from_path(filename=filename)
            # self.ide_new_file(os.path.split(filename)[1])
            # editor = self.get_current_editor()
            # pde_file = codecs.open(filename, "r", encoding="utf-8")
            # content = "".join(pde_file.readlines())
            # pde_file.close()
            # editor.text_edit.setPlainText(content)
            # setattr(editor, "path", filename)
            # setattr(editor, "last_saved", content)
            # self.main.tabWidget_files.setTabToolTip(self.main.tabWidget_files.currentIndex(), filename)
            # self.main.tabWidget_files.setTabText(self.main.tabWidget_files.currentIndex(), os.path.split(filename)[1])
            # #self.update_recents(filename)
            # self.ide_check_backup_file(editor=editor)

        # self.ide_tab_changed()


    #----------------------------------------------------------------------
    @Decorator.connect_features()
    def ide_save_file(self, *args, **kwargs):

        editor = kwargs.get("editor", None)
        if not editor: editor = self.get_tab().currentWidget()
        index = self.get_tab().indexOf(editor)
        filename = self.main.tabWidget_files.tabText(index)
        save_path = getattr(editor, "path", None)

        if not save_path:
            save_path, filename = Dialogs.set_save_file(self, filename)
            if not save_path: return False
            setattr(editor, "path", save_path)
            self.main.tabWidget_files.setTabText(index, filename)
            self.main.tabWidget_files.setTabToolTip(index, save_path)
            self.setWindowTitle(os.getenv("PINGUINO_FULLNAME")+" - "+save_path)

            self.update_recents(save_path)

        self.__save_file__(editor=editor)
        return True


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def ide_close_file(self, *args, **kwargs):
        editor = kwargs.get("editor", None)
        if not editor: editor = self.get_tab().currentWidget()
        index = self.get_tab().indexOf(editor)
        filename = self.get_tab().tabText(index)
        save_path = getattr(editor, "path", None)

        if not save_path and filename.endswith("*"):
            reply = Dialogs.set_no_saved_file(self, filename)

            if reply == True:
                save_path, filename = Dialogs.set_save_file(self, filename)
                if not save_path: return
                setattr(editor, "path", save_path)
                self.ide_save_file(editor=editor)

            elif reply == None: return

        elif filename.endswith("*"):
            reply = Dialogs.set_no_saved_file(self, filename)
            #print reply
            if reply == True: self.ide_save_file(editor=editor)
            elif reply == None: return

        self.get_tab().removeTab(index)
        self.save_recents()


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.connect_features()
    def ide_save_as(self, *args, **kwargs):
        editor = kwargs.get("editor", None)
        if not editor: editor = self.get_tab().currentWidget()
        index = self.get_tab().indexOf(editor)
        filename = self.main.tabWidget_files.tabText(index)
        save_path, filename = Dialogs.set_save_file(self, filename)
        if not save_path: return False
        setattr(editor, "path", save_path)
        self.main.tabWidget_files.setTabText(index, filename)
        self.main.tabWidget_files.setTabToolTip(index, save_path)
        self.setWindowTitle(os.getenv("PINGUINO_FULLNAME")+" - "+save_path)

        self.ide_save_file(editor=editor)
        return True


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def ide_save_all(self):
        tab = self.get_tab()
        for index in range(tab.count()):
            editor = tab.widget(index)
            if not self.is_graphical(editor) is None:
                self.ide_save_file(editor=editor)


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def ide_close_all(self):
        tab = self.get_tab()
        widgets = map(tab.widget, range(tab.count()))
        for widget in widgets:
            self.ide_close_file(editor=widget)

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def ide_close_others(self):
        tab = self.get_tab()
        current = tab.currentWidget()
        widgets = map(tab.widget, range(tab.count()))
        for widget in widgets:
            if widget == current: continue
            self.ide_close_file(editor=widget)

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    def editor_print_file(self):
        #Bug: no print to file, this is PySide bug.
        editor = self.get_tab().currentWidget()
        filename = self.get_tab().tabText(self.get_tab().currentIndex()).replace(".pde", ".pdf")
        QPrinter = QtGui.QPrinter
        printer = QPrinter(QPrinter.HighResolution)
        printer.setPageSize(QPrinter.Letter)
        printer.setOutputFileName(filename)
        printer.setDocName(filename)
        printer.setPageOrder(QPrinter.FirstPageFirst)
        printer.setOutputFormat(QPrinter.PdfFormat)
        preview = QtGui.QPrintDialog(printer)
        preview.setStyleSheet("""
        font-family: inherit;
        font-weight: normal;

        """)
        if preview.exec_():
            document = editor.text_edit.document()
            document.print_(printer)


    #----------------------------------------------------------------------
    def ide_close_ide(self, *args, **kwargs):

        self.configIDE.set("Main", "size", self.size().toTuple())
        self.configIDE.set("Main", "position", self.pos().toTuple())
        self.configIDE.set("Main", "maximized", self.isMaximized())
        self.configIDE.set("Main", "menubar", self.main.menubar.isVisible())
        self.configIDE.save_config()
        self.save_recents()

        self.close()


    #----------------------------------------------------------------------
    def save_recents(self):
        """"""
        count = 1
        self.configIDE.clear_recents()
        for file_ in self.recent_files:
            self.configIDE.set("Recents", "recent_"+str(count), file_)
            count += 1

        count = 1
        self.configIDE.clear_recents(section="RecentsProjects")
        for file_ in self.recent_projects:
            self.configIDE.set("RecentsProjects", "recent_"+str(count), file_)
            count += 1

        count = 1
        self.configIDE.clear_recents_open()
        for file_ in self.get_all_open_files():
            self.configIDE.set("Recents", "open_"+str(count), file_)
            count += 1

        self.configIDE.save_config()

        self.update_recents_menu()
        self.update_recents_menu_project()




    # Menu Edit
    #----------------------------------------------------------------------
    def editor_undo(self):
        editor = self.get_current_editor()
        #index = self.main.tabWidget_files.currentIndex()
        if hasattr(editor, "text_edit"):
            editor.text_edit.undo()


    #----------------------------------------------------------------------
    def editor_redo(self):
        editor = self.get_current_editor()
        #index = self.main.tabWidget_files.currentIndex()
        # editor = self.get_current_editor()
        if hasattr(editor, "text_edit"):
            editor.text_edit.redo()


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    def editor_cut(self):
        editor = self.get_current_editor()
        #index = self.main.tabWidget_files.currentIndex()
        if hasattr(editor, "text_edit"):
            editor.text_edit.cut()


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    def editor_copy(self):
        editor = self.get_current_editor()
        #index = self.main.tabWidget_files.currentIndex()
        if hasattr(editor, "text_edit"):
            editor.text_edit.copy()


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    def editor_paste(self):
        editor = self.get_current_editor()
        #index = self.main.tabWidget_files.currentIndex()
        if hasattr(editor, "text_edit"):
            editor.text_edit.paste()

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    def editor_delete(self):
        editor = self.get_current_editor()
        #index = self.main.tabWidget_files.currentIndex()
        if hasattr(editor, "text_edit"):
            tc = editor.text_edit.textCursor()
            if tc.selectedText(): tc.removeSelectedText()

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    def editor_select_all(self):
        editor = self.get_current_editor()
        #index = self.main.tabWidget_files.currentIndex()
        if hasattr(editor, "text_edit"):
            editor.text_edit.selectAll()


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.alert_tab("Search")
    @Decorator.show_tab("Search")
    def set_tab_search(self, mode):

        self.main.lineEdit_search.setFocus()
        editor = self.get_current_editor()
        cursor = editor.text_edit.textCursor()
        self.main.lineEdit_search.setText(cursor.selectedText())

        replace = (mode == "replace")
        self.main.lineEdit_replace.setVisible(replace)
        self.main.label_replace.setVisible(replace)
        self.main.pushButton_replace.setVisible(replace)
        self.main.pushButton_replace_all.setVisible(replace)

    # Menu Source

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def editor_comment_region(self):
        editor = self.get_current_editor()
        comment_wildcard = "// "

        #cursor is a COPY all changes do not affect the QPlainTextEdit's cursor!!!
        cursor = editor.text_edit.textCursor()

        text = cursor.selectedText()

        if text == "":  #no selected, single line
            start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
            startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()
            endPosition = editor.text_edit.document().findBlockByLineNumber(start+1).position() - 1

            cursor.setPosition(startPosition)
            cursor.setPosition(endPosition, QtGui.QTextCursor.KeepAnchor)
            editor.text_edit.setTextCursor(cursor)

        else:
            start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
            startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()


            end = editor.text_edit.document().findBlock(cursor.selectionEnd()).firstLineNumber()

            endPosition = editor.text_edit.document().findBlockByLineNumber(end+1).position() - 1

            cursor.setPosition(startPosition)
            cursor.setPosition(endPosition, QtGui.QTextCursor.KeepAnchor)
            editor.text_edit.setTextCursor(cursor)


        cursor = editor.text_edit.textCursor()

        start_ = cursor.selectionStart()
        end_ = cursor.selectionEnd()

        #selectionEnd = cursor.selectionEnd()
        start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
        end = editor.text_edit.document().findBlock(cursor.selectionEnd()).firstLineNumber()
        startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()

        #init=(start, end)
        #Start a undo block
        cursor.beginEditBlock()

        #Move the COPY cursor
        cursor.setPosition(startPosition)
        #Move the QPlainTextEdit Cursor where the COPY cursor IS!
        editor.text_edit.setTextCursor(cursor)
        editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)

        for i in comment_wildcard:
            editor.text_edit.moveCursor(QtGui.QTextCursor.Right, QtGui.QTextCursor.KeepAnchor)

        start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()

        editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)
        s = editor.text_edit.cursor()
        s.pos()
        for i in range(start, end + 1):
            editor.text_edit.textCursor().insertText(comment_wildcard)
            #cursor.insertText(comment_wildcard)
            editor.text_edit.moveCursor(QtGui.QTextCursor.Down)
            editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)

        editor.text_edit.moveCursor(QtGui.QTextCursor.EndOfLine)

        end_ += (end + 1 - start) * len(comment_wildcard)
        cursor.setPosition(start_)
        cursor.setPosition(end_, QtGui.QTextCursor.KeepAnchor)
        editor.text_edit.setTextCursor(cursor)

        #End a undo block
        cursor.endEditBlock()


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def editor_uncomment_region(self):

        editor = self.get_current_editor()
        comment_wildcard = "// "

        #cursor is a COPY all changes do not affect the QPlainTextEdit's cursor!!!
        cursor = editor.text_edit.textCursor()

        start_ = cursor.selectionStart()
        end_ = cursor.selectionEnd()

        start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
        end = editor.text_edit.document().findBlock(cursor.selectionEnd()).firstLineNumber()
        startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()

        #Start a undo block
        cursor.beginEditBlock()

        #Move the COPY cursor
        cursor.setPosition(startPosition)
        #Move the QPlainTextEdit Cursor where the COPY cursor IS!
        editor.text_edit.setTextCursor(cursor)
        editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)
        for i in range(start, end + 1):

            for i in comment_wildcard:
                editor.text_edit.moveCursor(QtGui.QTextCursor.Right, QtGui.QTextCursor.KeepAnchor)

            text = editor.text_edit.textCursor().selectedText()
            if text == comment_wildcard:
                editor.text_edit.textCursor().removeSelectedText()
            elif u'\u2029' in text:
                #\u2029 is the unicode char for \n
                #if there is a newline, rollback the selection made above.
                editor.text_edit.moveCursor(QtGui.QTextCursor.Left, QtGui.QTextCursor.KeepAnchor)

            editor.text_edit.moveCursor(QtGui.QTextCursor.Down)
            editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)

        end_ -= (end + 1 - start) * len(comment_wildcard)
        cursor.setPosition(start_)
        cursor.setPosition(end_, QtGui.QTextCursor.KeepAnchor)
        editor.text_edit.setTextCursor(cursor)

        #End a undo block
        cursor.endEditBlock()


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def editor_comment_uncomment(self):
        editor, cursor, prevCursor, selected, firstLine = self.editor_select_block_edit()

        if firstLine != False:
            if firstLine.startswith("//"): self.editor_uncomment_region()
            else: self.editor_comment_region()

        if not selected:
            cursor.clearSelection()
            editor.text_edit.setTextCursor(prevCursor)


    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    def editor_indent_region(self):
        editor, cursor, prevCursor, selected, firstLine = self.editor_select_block_edit()

        if firstLine != False:

            editor = self.get_current_editor()
            comment_wildcard = " " * 4

            #cursor is a COPY all changes do not affect the QPlainTextEdit's cursor!!!
            cursor = editor.text_edit.textCursor()

            text = cursor.selectedText()

            if text == "":  #no selected, single line
                start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
                startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()
                endPosition = editor.text_edit.document().findBlockByLineNumber(start+1).position() - 1

                cursor.setPosition(startPosition)
                cursor.setPosition(endPosition, QtGui.QTextCursor.KeepAnchor)
                editor.text_edit.setTextCursor(cursor)

            else:
                start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
                startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()


                end = editor.text_edit.document().findBlock(cursor.selectionEnd()).firstLineNumber()

                endPosition = editor.text_edit.document().findBlockByLineNumber(end+1).position() - 1

                cursor.setPosition(startPosition)
                cursor.setPosition(endPosition, QtGui.QTextCursor.KeepAnchor)
                editor.text_edit.setTextCursor(cursor)


            cursor = editor.text_edit.textCursor()

            start_ = cursor.selectionStart()
            end_ = cursor.selectionEnd()

            #selectionEnd = cursor.selectionEnd()
            start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
            end = editor.text_edit.document().findBlock(cursor.selectionEnd()).firstLineNumber()
            startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()

            #init=(start, end)
            #Start a undo block
            cursor.beginEditBlock()

            #Move the COPY cursor
            cursor.setPosition(startPosition)
            #Move the QPlainTextEdit Cursor where the COPY cursor IS!
            editor.text_edit.setTextCursor(cursor)
            editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)

            for i in comment_wildcard:
                editor.text_edit.moveCursor(QtGui.QTextCursor.Right, QtGui.QTextCursor.KeepAnchor)

            start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()

            editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)
            s = editor.text_edit.cursor()
            s.pos()
            for i in range(start, end + 1):
                editor.text_edit.textCursor().insertText(comment_wildcard)
                #cursor.insertText(comment_wildcard)
                editor.text_edit.moveCursor(QtGui.QTextCursor.Down)
                editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)

            editor.text_edit.moveCursor(QtGui.QTextCursor.EndOfLine)

            end_ += (end + 1 - start) * len(comment_wildcard)
            cursor.setPosition(start_)
            cursor.setPosition(end_, QtGui.QTextCursor.KeepAnchor)
            editor.text_edit.setTextCursor(cursor)

            #End a undo block
            cursor.endEditBlock()


        if not selected:
            cursor.clearSelection()
            editor.text_edit.setTextCursor(prevCursor)



    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    def editor_dedent_region(self):
        editor, cursor, prevCursor, selected, firstLine = self.editor_select_block_edit()

        if firstLine != False and firstLine.startswith(" "*4):

            editor = self.get_current_editor()
            comment_wildcard = " " * 4

            #cursor is a COPY all changes do not affect the QPlainTextEdit's cursor!!!
            cursor = editor.text_edit.textCursor()

            start_ = cursor.selectionStart()
            end_ = cursor.selectionEnd()

            start = editor.text_edit.document().findBlock(cursor.selectionStart()).firstLineNumber()
            end = editor.text_edit.document().findBlock(cursor.selectionEnd()).firstLineNumber()
            startPosition = editor.text_edit.document().findBlockByLineNumber(start).position()

            #Start a undo block
            cursor.beginEditBlock()

            #Move the COPY cursor
            cursor.setPosition(startPosition)
            #Move the QPlainTextEdit Cursor where the COPY cursor IS!
            editor.text_edit.setTextCursor(cursor)
            editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)
            for i in range(start, end + 1):

                for i in comment_wildcard:
                    editor.text_edit.moveCursor(QtGui.QTextCursor.Right, QtGui.QTextCursor.KeepAnchor)

                text = editor.text_edit.textCursor().selectedText()
                if text == comment_wildcard:
                    editor.text_edit.textCursor().removeSelectedText()
                elif u'\u2029' in text:
                    #\u2029 is the unicode char for \n
                    #if there is a newline, rollback the selection made above.
                    editor.text_edit.moveCursor(QtGui.QTextCursor.Left, QtGui.QTextCursor.KeepAnchor)

                editor.text_edit.moveCursor(QtGui.QTextCursor.Down)
                editor.text_edit.moveCursor(QtGui.QTextCursor.StartOfLine)

            end_ -= (end + 1 - start) * len(comment_wildcard)
            cursor.setPosition(start_)
            cursor.setPosition(end_, QtGui.QTextCursor.KeepAnchor)
            editor.text_edit.setTextCursor(cursor)

            #End a undo block
            cursor.endEditBlock()

        if not selected:
            cursor.clearSelection()
            editor.text_edit.setTextCursor(prevCursor)


    # Pinguino


    #----------------------------------------------------------------------
    @Decorator.show_tab("Boards")
    @Decorator.alert_tab("Boards")
    def set_tab_board(self):
        pass


    #----------------------------------------------------------------------
    @Decorator.show_tab("Stdout")
    @Decorator.alert_tab("Stdout")
    def set_tab_stdout(self):
        pass

    #----------------------------------------------------------------------
    def ide_show_main_c(self):

        source = os.path.join(os.getenv("PINGUINO_USER_PATH"), "source")
        board = self.configIDE.config("Board", "arch", 8)
        if board == 32: extra = "32"
        else: extra = ""
        filename = os.path.join(source, "main{}.c".format(extra))
        self.ide_open_file_from_path(filename=filename, readonly=True)


    #----------------------------------------------------------------------
    def ide_show_define_h(self):

        filename = os.path.join(os.getenv("PINGUINO_USER_PATH"), "source", "define.h")
        self.ide_open_file_from_path(filename=filename, readonly=True)


    #----------------------------------------------------------------------
    def ide_show_user_c(self):

        filename = os.path.join(os.getenv("PINGUINO_USER_PATH"), "source", "user.c")
        self.ide_open_file_from_path(filename=filename, readonly=True)


    #----------------------------------------------------------------------
    def ide_show_hex(self):

        filename = self.get_current_hex()
        self.ide_open_file_from_path(filename=filename, readonly=True)






    # Graphical

    # #----------------------------------------------------------------------
    # def __show_pinguino_code__(self):
        # name = getattr(self.get_tab().currentWidget(), "path", "")
        # if name: name = " - " + name
        # self.frame_pinguino_code = PlainOut(QtGui.QApplication.translate("Dialogs", "Pinguino code"))
        # self.frame_pinguino_code.show_text(self.PinguinoKIT.get_pinguino_source_code(), pde=True)
        # self.frame_pinguino_code.show()

    #----------------------------------------------------------------------
    def editor_export_pinguino_code(self):
        area = self.PinguinoKIT.get_work_area()
        area.export_code_to_pinguino_editor()


    # Options




    # Help

    #----------------------------------------------------------------------
    def open_web_site(self, url):
        webbrowser.open_new_tab(url)


    # Tools Files



    # Tools Source


    # Tools Search
    # see search_replace.py


    # Widgets

    #----------------------------------------------------------------------
    @Decorator.update_toolbar()
    # @Decorator.connect_features()
    def ide_tab_changed(self, *args, **kwargs):
        self.main.tabWidget_files.setVisible(self.main.tabWidget_files.count() > 0)
        self.main.frame_logo.setVisible(not self.main.tabWidget_files.count() > 0)
        self.main.actionClose_file.setEnabled(self.main.tabWidget_files.count() > 0)

        editor = self.get_current_editor()
        if getattr(editor, "path", None):

            if self.is_project():
                if self.is_library():
                    type_project = "Library: {}-{}".format(self.get_project_name(), self.get_library_version())
                else:
                    type_project = "Project: {}".format(self.get_project_name())

                self.setWindowTitle("{pinguino} | {type_project} - {path}".format(pinguino=os.getenv("PINGUINO_FULLNAME"), type_project=type_project, path=editor.path))
            else:
                self.setWindowTitle("{pinguino} - {path}".format(pinguino=os.getenv("PINGUINO_FULLNAME"), path=editor.path))


        # else: self.setWindowTitle(os.getenv("PINGUINO_FULLNAME"))
        else:
            self.setWindowTitle("{PINGUINO_FULLNAME}".format(**os.environ))



        index = self.main.tabWidget_files.currentIndex()
        filename = self.main.tabWidget_files.tabText(index)
        if filename.endswith("*"): self.main.actionSave_file.setEnabled(True)
        else: self.main.actionSave_file.setDisabled(True)

        # self.__update_current_dir_on_files__()

        self.update_actions()
        # self.thread_variables()
        # self.update_code_navigator()
        # # if not hasattr(self, "ii"):
            # # self.ii = 0
        # # self.ii += 1
        # # logging.debug("WTF-{}".format(self.ii))


    #----------------------------------------------------------------------
    def ide_tab_close(self, index):
        editor = self.get_tab().widget(index)
        self.ide_close_file(editor=editor)


    # Graphical Tool Bar

    #----------------------------------------------------------------------
    @Decorator.requiere_open_files()
    def editor_save_screen_image(self):
        editor = self.get_tab().currentWidget()
        scroll_area = editor.scroll_area
        image = QtGui.QPixmap.grabWidget(scroll_area,
                                         QtCore.QRect(0, 0,
                                                      scroll_area.width()-13,
                                                      scroll_area.height()-13))

        filename = self.get_tab().tabText(self.get_tab().currentIndex())
        filename = os.path.splitext(filename)[0] + ".png"
        filename = Dialogs.set_save_image(self, filename)
        if filename: image.save(filename, "png")


    #----------------------------------------------------------------------
    def editor_rename_file(self):
        """"""
        editor = self.get_tab().currentWidget()
        new_name = Dialogs.get_text(self, "Rename file", os.path.basename(editor.path))
        filename = os.path.basename(editor.path)
        logging.debug("Renamed {} for {}".format(filename, new_name) )


    #----------------------------------------------------------------------
    def editor_key_press(self, event):
        editor = self.get_current_editor()
        if hasattr(editor.text_edit, "completer"):
            if self.is_autocomplete_enable():
                editor.text_edit.__keyPressEvent__(event)
            else:
                editor.text_edit.force_keyPressEvent(event)


    #----------------------------------------------------------------------
    def ide_drop_file(self, event):
        mine = event.mimeData()
        if mine.hasUrls():
            for path in mine.urls():
                self.ide_open_file_from_path(filename=path.path())



    #----------------------------------------------------------------------
    def editor_tabfile_context_menu(self, event):
        menu = QtGui.QMenu()

        editor = self.get_current_editor()

        if hasattr(editor, "path"):
            if self.is_graphical() is False:
                if editor.text_edit.isReadOnly():
                    menu.addAction("Remove read only", self.editor_set_editable)
                else:
                    menu.addAction("Rename...", self.editor_rename_file)

            elif  self.is_graphical() is True:
                menu.addAction("Rename...", self.editor_rename_file)


        menu.addAction(self.main.actionSave_file)
        menu.addAction(self.main.actionSave_as)
        menu.addAction(self.main.actionSave_all)
        menu.addSeparator()
        menu.addAction(self.main.actionClose_file)
        menu.addAction(self.main.actionClose_all)
        menu.addAction(self.main.actionClose_others)

        menu.setStyleSheet("""
        font-family: inherit;
        font-weight: normal;

        """)

        menu.exec_(event.globalPos())


    #----------------------------------------------------------------------
    def editor_file_context_menu(self, event):
        menu = QtGui.QMenu()

        editor = self.get_current_editor()
        filename = getattr(editor, "path", False)
        if filename and (filename.startswith(os.path.join(os.getenv("PINGUINO_USER_PATH"), "examples")) or
                         filename.startswith(os.path.join(os.getenv("PINGUINO_USER_PATH"), "graphical_examples")) or
                         filename.startswith(os.path.join(os.getenv("PINGUINO_USER_PATH"), "examples_libs"))
                         ):
            menu.addAction(QtGui.QApplication.translate("Frame", "Restore example"), self.editor_restore_example)
            menu.addSeparator()


        menu.addAction(self.main.actionGenerate_blocks)
        menu.addSeparator()

        menu.addAction(self.main.actionUndo)
        menu.addAction(self.main.actionRedo)
        menu.addSeparator()
        menu.addAction(self.main.actionCut)
        menu.addAction(self.main.actionCopy)
        menu.addAction(self.main.actionPaste)
        menu.addAction(self.main.actionDelete)
        menu.addSeparator()
        menu.addAction(self.main.actionSelect_all)
        menu.addSeparator()
        menu.addAction(self.main.actionComment_out_region)
        menu.addAction(self.main.actionComment_Uncomment_region)
        menu.addSeparator()
        menu.addAction(self.main.actionIndent)
        menu.addAction(self.main.actionDedent)
        menu.addSeparator()
        menu.addAction(self.main.actionCompile)
        menu.addAction(self.main.actionUpload)
        menu.addAction(self.main.actionIf_Compile_then_Upload)
        menu.addSeparator()
        menu.addAction(self.main.actionAutocomplete)
        # menu.addAction("Generate blocks", self.open_as_blocks)

        menu.setStyleSheet("""
        QMenu {
            font-family: inherit;
            font-weight: normal;
            }

        """)

        menu.exec_(event.globalPos())


    #----------------------------------------------------------------------
    def ide_tabs_context_menu(self, event):
        """"""
        menu = QtGui.QMenu()
        menu.addAction(self.main.actionTabShell)
        menu.addAction(self.main.actionTabLog)
        menu.addAction(self.main.actionTabStdout)
        menu.addSeparator()
        menu.addAction(self.main.actionTabFiles)
        menu.addAction(self.main.actionTabProject)
        menu.addAction(self.main.actionTabSourceBrowser)
        menu.addAction(self.main.actionLibraryManager)
        menu.addAction(self.main.actionTabSearchReplace)
        menu.addAction(self.main.actionTabBoardConfig)
        menu.exec_(event.globalPos())


    #----------------------------------------------------------------------
    def editor_restore_example(self):
        editor = self.get_current_editor()
        filename = getattr(editor, "path", False)
        filename_install = filename.replace(os.getenv("PINGUINO_USER_PATH"), os.getenv("PINGUINO_INSTALL_PATH"))
        shutil.copyfile(filename_install, filename)
        self.editor_reload_file()


    #----------------------------------------------------------------------
    def toggle_tab(self, tab_name, force=None):
        """"""
        widget = getattr(self.main, tab_name)

        if force is None:

            if self.main.tabWidget_bottom.indexOf(widget) != -1:
                index = self.main.tabWidget_bottom.indexOf(widget)
                widget.tab_parent = self.main.tabWidget_bottom
                widget.index = index
                widget.label = self.main.tabWidget_bottom.tabText(index)
                self.main.tabWidget_bottom.removeTab(index)
                self.configIDE.set("Tabs", tab_name, False)
                self.configIDE.set("Tabs", "{}_name".format(tab_name), tab_name)

            elif self.main.tabWidget_tools.indexOf(widget) != -1:
                index = self.main.tabWidget_tools.indexOf(widget)
                widget.tab_parent = self.main.tabWidget_tools
                widget.index = index
                widget.label = self.main.tabWidget_tools.tabText(index)
                self.main.tabWidget_tools.removeTab(index)
                self.configIDE.set("Tabs", tab_name, False)
                self.configIDE.set("Tabs", "{}_name".format(tab_name), tab_name)

            else:
                widget.tab_parent.addTab(widget, widget.label)
                i = widget.tab_parent.indexOf(widget)
                widget.tab_parent.tabBar().moveTab(i, widget.index)
                widget.tab_parent.setCurrentIndex(widget.index)
                self.configIDE.set("Tabs", tab_name, True)
                self.configIDE.set("Tabs", "{}_name".format(tab_name), tab_name)

        else:
            if force is True:

                widget.tab_parent.addTab(widget, widget.label)
                i = widget.tab_parent.indexOf(widget)
                widget.tab_parent.tabBar().moveTab(i, widget.index)
                # widget.tab_parent.setCurrentIndex(widget.index)
                self.configIDE.set("Tabs", tab_name, True)
                self.configIDE.set("Tabs", "{}_name".format(tab_name), tab_name)


            elif force is False:

                if self.main.tabWidget_bottom.indexOf(widget) != -1:
                    index = self.main.tabWidget_bottom.indexOf(widget)
                    widget.tab_parent = self.main.tabWidget_bottom
                    widget.index = index
                    widget.label = self.main.tabWidget_bottom.tabText(index)
                    self.main.tabWidget_bottom.removeTab(index)
                    self.configIDE.set("Tabs", tab_name, False)
                    self.configIDE.set("Tabs", "{}_name".format(tab_name), tab_name)

                elif self.main.tabWidget_tools.indexOf(widget) != -1:
                    index = self.main.tabWidget_tools.indexOf(widget)
                    widget.tab_parent = self.main.tabWidget_tools
                    widget.index = index
                    widget.label = self.main.tabWidget_tools.tabText(index)
                    self.main.tabWidget_tools.removeTab(index)
                    self.configIDE.set("Tabs", tab_name, False)
                    self.configIDE.set("Tabs", "{}_name".format(tab_name), tab_name)

        self.configIDE.save_config()

    #----------------------------------------------------------------------
    def set_block_tab(self, name):
        """"""
        widget = self.PinguinoKIT.get_widget(name)
        self.main.stackedWidget_blocks.setCurrentWidget(widget)

        for i in range(self.main.comboBox_blocks.count()):
            if self.main.comboBox_blocks.itemText(i) == name:
                self.main.comboBox_blocks.setCurrentIndex(i)
                break


    #----------------------------------------------------------------------
    def submit_stdout(self):
        """"""
        file_ = os.path.join(os.getenv("PINGUINO_USER_PATH"), "source", "stdout")

        if os.path.exists(file_):
            stdout = codecs.open(file_, "r", encoding="utf-8")
            content = stdout.read()
            stdout.close()
        self.submit_bug = SubmitBug(self, content)
        self.submit_bug.show()


    #----------------------------------------------------------------------
    def refresh_libraries(self):
        """"""
        self.pinguinoAPI.force_reload_libs()
        PinguinoConfig.update_user_libs(self.pinguinoAPI)
        self.update_reserved_words()
        if hasattr(self, "assistant"): delattr(self, "assistant")
        self.change_dir_files(0)