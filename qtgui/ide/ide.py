#!/usr/bin/env python
#-*- coding: utf-8 -*-

import sys
import os
import platform
from PySide import QtGui, QtCore

from .methods.backgrounds import BackgroundPallete
from .events.events import PinguinoEvents
from .methods.decorators import Decorator
from .code_editor.autocomplete_icons import CompleteIcons
from .widgets.output_widget import PinguinoTerminal
# from .methods.widgets_features import PrettyFeatures
from ..gide.app.graphical import GraphicalIDE
from ..frames.main import Ui_PinguinoIDE
from ..pinguino_api.pinguino import Pinguino, AllBoards
from ..pinguino_api.pinguino_config import PinguinoConfig
from ..pinguino_api.config import Config

import logging
#import debugger

########################################################################
class PinguinoIDE(QtGui.QMainWindow, PinguinoEvents):

    #@Decorator.debug_time()
    def __init__(self, splash_write, argvs):
        super(PinguinoIDE, self).__init__()

        os.environ["PINGUINO_PROJECT"] = ""

        QtCore.QTextCodec.setCodecForCStrings(QtCore.QTextCodec.codecForName("UTF-8"))
        QtCore.QTextCodec.setCodecForLocale(QtCore.QTextCodec.codecForName("UTF-8"))
        QtCore.QTextCodec.setCodecForTr(QtCore.QTextCodec.codecForName("UTF-8"))

        self.main = Ui_PinguinoIDE()
        self.main.setupUi(self)

        self.argvs = argvs

        if not self.argvs.devmode:
            self.main.menubar.removeAction(self.main.menuDevelopment.menuAction())

        splash_write(QtGui.QApplication.translate("Splash", "Setting enviroment values"))
        PinguinoConfig.set_environ_vars()
        splash_write(QtGui.QApplication.translate("Splash", "Checking user files"))
        PinguinoConfig.check_user_files()

        splash_write(QtGui.QApplication.translate("Splash", "Loading Pinguino API"))
        self.pinguinoAPI = Pinguino()
        self.pinguinoAPI._boards_ = AllBoards

        splash_write(QtGui.QApplication.translate("Splash", "Loading configuration"))
        self.configIDE = Config()

        splash_write(QtGui.QApplication.translate("Splash", "Loading graphical mode"))
        self.PinguinoKIT = GraphicalIDE(self)
        self.main.tabWidget_graphical.setVisible(False)
        # self.main.dockWidget_blocks.setVisible(False)

        splash_write(QtGui.QApplication.translate("Splash", "Loading icons"))
        self.ICONS = CompleteIcons()

        splash_write(QtGui.QApplication.translate("Splash", "Setting theme"))
        self.build_menutoolbar()
        self.set_icon_theme()
        self.reload_toolbar_icons()

        splash_write(QtGui.QApplication.translate("Splash", "Linking paths for libraries and compilers"))
        PinguinoConfig.update_pinguino_paths(self.configIDE, self.pinguinoAPI)
        PinguinoConfig.update_pinguino_extra_options(self.configIDE, self.pinguinoAPI)
        splash_write(QtGui.QApplication.translate("Splash", "Searching user libraries"))
        PinguinoConfig.update_user_libs(self.pinguinoAPI)

        # RB 2015-01-27 : Still useful ? See also methods.py/set_board
        self.pinguinoAPI.set_os_variables()

        self.setWindowTitle(os.getenv("PINGUINO_FULLNAME"))

        splash_write(QtGui.QApplication.translate("Splash", "Opening last files"))
        self.open_last_files()

        splash_write(QtGui.QApplication.translate("Splash", "Starting widgets features"))
        self.init_widgets()
        splash_write(QtGui.QApplication.translate("Splash", "Building status bar"))
        self.build_statusbar()
        splash_write(QtGui.QApplication.translate("Splash", "Building Python Shell"))
        self.build_output()

        splash_write(QtGui.QApplication.translate("Splash", "Overwriting stylesheets"))
        self.set_styleSheet()

        #timer events
        splash_write(QtGui.QApplication.translate("Splash", "Starting timers"))
        self.update_functions()
        self.update_directives()
        self.update_variables()
        self.update_autocompleter()
        self.check_external_changes()
        self.save_backup_file()

        splash_write(QtGui.QApplication.translate("Splash", "Loading examples"))
        self.__update_path_files__(os.path.join(os.getenv("PINGUINO_USER_PATH"), "examples"))
        # self.__update_graphical_path_files__(os.path.join(os.getenv("PINGUINO_USER_PATH"), "graphical_examples"))

        splash_write(QtGui.QApplication.translate("Splash", "Loading boards configuration"))
        #self.set_board() #called in self.get_status_board()
        self.statusbar_ide(self.get_status_board())

        splash_write(QtGui.QApplication.translate("Splash", "Connecting events"))
        self.connect_events()
        splash_write(QtGui.QApplication.translate("Splash", "Loading configuration"))
        self.load_main_config()
        self.update_project_status(None)

        os_name = os.getenv("PINGUINO_OS_NAME")
        if os_name == "windows":
            os.environ["PATH"] = os.environ["PATH"] + ";" + self.configIDE.get_path("sdcc_bin")

        elif os_name == "linux":
            os.environ["LD_LIBRARY_PATH"]="/usr/lib32:/usr/lib:/usr/lib64"

        splash_write(QtGui.QApplication.translate("Splash", "Welcome to %s %s")%(os.getenv("PINGUINO_NAME"), os.getenv("PINGUINO_VERSION")))
        print("Pinguino IDE started!")

        self.enable_debugger()


    #----------------------------------------------------------------------
    def enable_debugger(self):

        root = logging.getLogger()
        root.setLevel(logging.DEBUG)

        ch = logging.StreamHandler(self.main.plainTextEdit_output)
        #ch.setLevel(logging.INFO)
        # formatter = logging.Formatter("[DEBUG] %(message)s")
        formatter = logging.Formatter("%(message)s")
        ch.setFormatter(formatter)

        root.addHandler(ch)


    #----------------------------------------------------------------------
    def build_menutoolbar(self):

        self.toolbutton_menutoolbar = QtGui.QToolButton(self)
        self.toolbutton_menutoolbar.setPopupMode(QtGui.QToolButton.MenuButtonPopup)
        menu = QtGui.QMenu()

        icon = QtGui.QIcon.fromTheme("preferences-system")
        self.toolbutton_menutoolbar.setIcon(icon)

        menu.addMenu(self.main.menuFile)
        menu.addMenu(self.main.menuEdit)
        menu.addMenu(self.main.menuView)
        menu.addMenu(self.main.menuProject)
        menu.addMenu(self.main.menuSettings)
        menu.addMenu(self.main.menuSource)
        menu.addMenu(self.main.menuPinguino)
        menu.addMenu(self.main.menuGraphical)
        menu.addMenu(self.main.menuHelp)
        if self.argvs.devmode:
            menu.addMenu(self.main.menuDevelopment)

        menu.addSeparator()
        menu.addAction(self.main.actionMenubar)

        self.toolbutton_menutoolbar.setMenu(menu)
        self.main.toolBar_system.addWidget(self.toolbutton_menutoolbar)


    #----------------------------------------------------------------------
    def get_systeminfo(self):

        data = {}
        try: data["os.name"] = str(os.name)
        except: pass
        try: data["os.environ"] = str(os.environ)
        except: pass
        try: data["os.uname"] = str(os.uname())
        except: pass
        try: data["sys.argv"] = str(sys.argv)
        except: pass
        try: data["sys.flags"] = str(sys.flags)
        except: pass
        try: data["sys.platform"] = str(sys.platform)
        except: pass
        try: data["sys.version"] = str(sys.version)
        except: pass
        try: data["platform.architecture"] = str(platform.architecture())
        except: pass
        try: data["platform.dist"] = str(platform.dist())
        except: pass
        try: data["platform.linux_distribution"] = str(platform.linux_distribution())
        except: pass
        try: data["platform.mac_ver"] = str(platform.mac_ver())
        except: pass
        try: data["platform.system"] = str(platform.system())
        except: pass
        try: data["platform.win32_ver"] = str(platform.win32_ver())
        except: pass
        try: data["platform.libc_ver"] = str(platform.libc_ver())
        except: pass
        try: data["platform.machine"] = str(platform.machine())
        except: pass
        try: data["platform.platform"] = str(platform.platform())
        except: pass
        try: data["platform.release"] = str(platform.release())
        except: pass

        return "\n" + "#" + "-" * 80 + "\n#" + "-" * 80 + "\n" + "\n".join([": ".join(item) for item in data.items()]) + "\n#" + "-" * 80 + "\n#" + "-" * 80


    #----------------------------------------------------------------------
    def init_widgets(self):

        self.TAB_FILES = 0
        self.TAB_PROJECT = 1
        self.TAB_SOURCE = 2
        self.TAB_SEARCH = 3

        self.main.tabWidget_files.setVisible(False)
        self.main.toolBar_graphical.setVisible(False)

        self.main.tabWidget_tools.setCurrentIndex(self.TAB_FILES)


        self.main.dockWidget_tools.setTitleBarWidget(QtGui.QWidget())
        self.main.dockWidget_output.setTitleBarWidget(QtGui.QWidget())

        # self.main.tabWidget_blocks_tools.setCurrentIndex(0)

        # side = self.configIDE.config("Main", "dock_tools", "RightDockWidgetArea")
        # self.addDockWidget(QtCore.Qt.DockWidgetArea(getattr(QtCore.Qt, side)), self.main.dockWidget_tools)
        # self.update_tab_position(self.main.tabWidget_tools, self.dockWidgetArea(self.main.dockWidget_tools))

        # side = self.configIDE.config("Main", "dock_blocks", "RightDockWidgetArea")
        # # self.addDockWidget(QtCore.Qt.DockWidgetArea(getattr(QtCore.Qt, side)), self.main.dockWidget_blocks)
        # # self.update_tab_position(self.main.tabWidget_blocks, self.dockWidgetArea(self.main.dockWidget_blocks))

        # side = self.configIDE.config("Main", "dock_shell", "BottomDockWidgetArea")
        # self.addDockWidget(QtCore.Qt.DockWidgetArea(getattr(QtCore.Qt, side)), self.main.dockWidget_output)

        # PrettyFeatures.LineEdit_default_text(self.main.lineEdit_search, QtGui.QApplication.translate("Frame", "Search..."))
        # PrettyFeatures.LineEdit_default_text(self.main.lineEdit_replace, QtGui.QApplication.translate("Frame", "Replace..."))
        # PrettyFeatures.LineEdit_default_text(self.main.lineEdit_blocks_search, QtGui.QApplication.translate("Frame", "Search block..."))


    #----------------------------------------------------------------------
    def set_styleSheet(self):
        #self.load_fonts()

        self.PinguinoPallete = BackgroundPallete()
        self.PinguinoPallete.save_palette(self.main.centralwidget.parent())
        self.switch_color_theme(self.configIDE.config("Main", "color_theme", True))

        bg_color = self.configIDE.config("Styles", "background_color", "#FFFFFF")
        alternate_bg_color = self.configIDE.config("Styles", "alternate_background_color", "#DDE8FF")
        selection_color = self.configIDE.config("Styles", "selection_color", "#FFFFFF")
        selection_bg_color = self.configIDE.config("Styles", "selection_foreground_color", "#57AAFF")

        self.main.tableWidget_functions.setStyleSheet("""
        QTableWidget {
            background-color: %s;
            alternate-background-color: %s;
        }
        """%(bg_color, alternate_bg_color))

        self.main.tableWidget_directives.setStyleSheet("""
        QTableWidget {
            background-color: %s;
            alternate-background-color: %s;
        }
        """%(bg_color, alternate_bg_color))

        self.main.tableWidget_variables.setStyleSheet("""
        QTableWidget {
            background-color: %s;
            alternate-background-color: %s;
        }
        """%(bg_color, alternate_bg_color))


        # #Global CSS styles
        # self.setStyleSheet("""
        # font-family: inherit;
        # font-weight: normal;
        # selection-color: %s;
        # selection-background-color: %s;
        # """%(selection_color, selection_bg_color))


        # self.main.groupBox_replace.setStyleSheet("""
        # QGroupBox{
            # font-family: inherit;
            # font-weight: bold;
        # }
        # """)

        # self.main.groupBox_search.setStyleSheet("""
        # QGroupBox{
            # font-family: inherit;
            # font-weight: bold;
        # }
        # """)

        #Python shell CSS styles
        self.main.plainTextEdit_output.setStyleSheet("""
        QPlainTextEdit {
            background-color: #333;
            color: #FFFFFF;
            font-family: mono;
            font-weight: normal;
            font-size: 10pt;
        }
        """)



    #----------------------------------------------------------------------
    def build_output(self):
        self.main.actionAutocomplete.setChecked(self.configIDE.config("Features", "autocomplete", True))  #FIXME: move this
        self.main.checkBox_output_debug.setChecked(self.configIDE.config("Features", "debug_in_output", True))
        self.main.checkBox_output_messages.setChecked(self.configIDE.config("Features", "out_in_output", True))

        checkbox = {"checkbox_debug": self.main.checkBox_output_debug,
                    "checkbox_out": self.main.checkBox_output_messages,
                    }

        self.main.plainTextEdit_output = PinguinoTerminal(widget=self.main.widget_output, checkbox=checkbox)

        self.main.frame.setStyleSheet("""
        QFrame {
            background-color: #333;
            color: #FFFFFF;
        }
        """)

        self.main.plainTextEdit_output.set_extra_args(**{"pinguino": self})
        self.main.gridLayout_17.addWidget(self.main.plainTextEdit_output, 0, 0, 1, 1)


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
        # self.main.menuGraphical.setEnabled(normal
        self.main.menubar.removeAction(self.main.menuGraphical.menuAction())

        self.main.lineEdit_blocks_search.setVisible(normal)
        self.main.label_search_block.setVisible(normal)
        self.main.tabWidget_blocks.setVisible(normal)
        self.main.tabWidget_tools.setVisible(not normal)

        # self.main.dockWidget_blocks.setVisible(normal)
        # self.main.dockWidget_tools.setVisible(not normal)
        self.main.toolBar_search_replace.setVisible(not normal)
        self.main.toolBar_edit.setVisible(not normal)
        self.main.toolBar_graphical.setVisible(normal)
        self.main.toolBar_undo_redo.setVisible(not normal)

        #self.configIDE.set("Features", "terminal_on_graphical", self.main.dockWidget_output.isVisible())
        self.main.dockWidget_output.setVisible(self.configIDE.config("Features", "terminal_on_text", True))
        self.main.actionPython_shell.setChecked(self.configIDE.config("Features", "terminal_on_text", True))
        self.configIDE.save_config()


    #----------------------------------------------------------------------
    def update_actions_for_graphical(self):
        normal = True
        # self.main.menuGraphical.setEnabled(normal)
        self.main.menubar.insertMenu(self.main.menuHelp.menuAction(), self.main.menuGraphical)

        self.main.lineEdit_blocks_search.setVisible(normal)
        self.main.label_search_block.setVisible(normal)
        self.main.tabWidget_blocks.setVisible(normal)
        self.main.tabWidget_tools.setVisible(not normal)

        # self.main.dockWidget_blocks.setVisible(normal)
        # self.main.dockWidget_tools.setVisible(not normal)
        self.main.toolBar_search_replace.setVisible(not normal)
        self.main.toolBar_edit.setVisible(not normal)
        self.main.toolBar_graphical.setVisible(normal)
        self.main.toolBar_undo_redo.setVisible(not normal)

        #self.configIDE.set("Features", "terminal_on_text", self.main.dockWidget_output.isVisible())
        self.main.dockWidget_output.setVisible(self.configIDE.config("Features", "terminal_on_graphical", False))
        self.main.actionPython_shell.setChecked(self.configIDE.config("Features", "terminal_on_graphical", False))
        self.configIDE.save_config()


    #----------------------------------------------------------------------
    def reload_toolbar_icons(self):

        self.toolbars = [self.main.toolBar_edit,
                         self.main.toolBar_files,
                         self.main.toolBar_graphical,
                         self.main.toolBar_pinguino,
                         self.main.toolBar_search_replace,
                         self.main.toolBar_switch,
                         self.main.toolBar_undo_redo,
                         self.main.toolBar_system,
                         ]

        for toolbar in self.toolbars:
            toolbar.setToolButtonStyle(QtCore.Qt.ToolButtonIconOnly)  #explicit IconOnly for windows
            size = self.configIDE.config("Main", "icons_size", 24)
            self.resize_toolbar(size, getattr(self.main, "action%dx%d"%(size, size)))()
            getattr(self.main, "action%dx%d"%(size, size)).setChecked(True)

        icons_toolbar = [
                         (self.main.actionNew_file, "document-new"),
                         (self.main.actionOpen_file, "document-open"),
                         (self.main.actionSave_file, "document-save"),

                         (self.main.actionUndo, "edit-undo"),
                         (self.main.actionRedo, "edit-redo"),
                         (self.main.actionCut, "edit-cut"),
                         (self.main.actionCopy, "edit-copy"),
                         (self.main.actionPaste, "edit-paste"),

                         (self.main.actionSearch, "edit-find"),
                         (self.main.actionSearch_and_replace, "edit-find-replace"),

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

        self.main.actionSwitch_ide.setIcon(icon)


    #----------------------------------------------------------------------
    def set_icon_theme(self):

        QtGui.QIcon.setThemeSearchPaths(QtGui.QIcon.themeSearchPaths()+[os.path.join(os.getenv("PINGUINO_DATA"), "qtgui", "resources", "themes")])
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

        theme = self.configIDE.config("Main", "theme", "pinguino11")
        if not theme in valid_themes:
            theme = "pinguino11"
            self.configIDE.set("Main", "theme", "pinguino11")
        self.change_icon_theme(theme, dict_themes[theme])()
        dict_themes[theme].setChecked(True)


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

