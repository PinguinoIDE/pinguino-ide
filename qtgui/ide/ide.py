#!/usr/bin/env python
#-*- coding: utf-8 -*-

import sys
import os
import platform
import inspect
import types

from PySide import QtGui, QtCore

from .commons.backgrounds import BackgroundPallete
from .events import PinguinoEvents
from .methods.decorators import Decorator
from .custom_widgets.code_editor.autocomplete_icons import CompleteIcons
# from .custom_widgets import PinguinoTextEdit
from .help.help import Help
# from .methods.widgets_features import PrettyFeatures
from ..gide.app.graphical import GraphicalIDE
from ..pinguino_core.pinguino import Pinguino, AllBoards
from ..pinguino_core.pinguino_config import PinguinoConfig
from ..pinguino_core.config import Config
from ..frames.main import Ui_PinguinoIDE

import logging
#import debugger

from .tools.python_shell import PythonShell
from .tools.log import PinguinoLog
from .tools.stdout import Stdout

########################################################################
class PinguinoIDE(QtGui.QMainWindow, PinguinoEvents, Help, PythonShell, PinguinoLog, Stdout):

    #@Decorator.debug_time()
    def __init__(self, splash_write):
        super(PinguinoIDE, self).__init__()

        os.environ["PINGUINO_PROJECT"] = ""

        QtCore.QTextCodec.setCodecForCStrings(QtCore.QTextCodec.codecForName("UTF-8"))
        QtCore.QTextCodec.setCodecForLocale(QtCore.QTextCodec.codecForName("UTF-8"))
        QtCore.QTextCodec.setCodecForTr(QtCore.QTextCodec.codecForName("UTF-8"))

        self.main = Ui_PinguinoIDE()
        self.main.setupUi(self)

        PythonShell.__init__(self)
        PinguinoLog.__init__(self)
        Help.__init__(self)

        # self.argvs = argvs

        # if not self.argvs.devmode:
            # self.main.menubar.removeAction(self.main.menuDevelopment.menuAction())

        splash_write(QtGui.QApplication.translate("Splash", "Setting enviroment values"))
        PinguinoConfig.set_environ_vars()
        splash_write(QtGui.QApplication.translate("Splash", "Checking user files"))
        PinguinoConfig.check_user_files()

        splash_write(QtGui.QApplication.translate("Splash", "Loading Pinguino Core"))
        self.pinguinoAPI = Pinguino()
        self.pinguinoAPI._boards_ = AllBoards

        splash_write(QtGui.QApplication.translate("Splash", "Loading configuration"))
        self.configIDE = Config()

        splash_write(QtGui.QApplication.translate("Splash", "Loading blocks programming"))
        self.PinguinoKIT = GraphicalIDE(self)
        self.main.tabWidget_files.setVisible(False)
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
        # self.pinguinoAPI.set_os_variables()

        self.setWindowTitle(os.getenv("PINGUINO_FULLNAME"))

        splash_write(QtGui.QApplication.translate("Splash", "Opening last files"))
        self.ide_open_last_files()

        splash_write(QtGui.QApplication.translate("Splash", "Starting widgets features"))
        self.init_widgets()
        splash_write(QtGui.QApplication.translate("Splash", "Building status bar"))
        self.build_statusbar()
        # splash_write(QtGui.QApplication.translate("Splash", "Building tabs"))
        # self.build_tabs()

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
        self.update_path_files([os.path.join(os.getenv("PINGUINO_USER_PATH"), "examples"),
                                os.path.join(os.getenv("PINGUINO_USER_PATH"), "graphical_examples")])
        # self.__update_graphical_path_files__(os.path.join(os.getenv("PINGUINO_USER_PATH"), "graphical_examples"))

        splash_write(QtGui.QApplication.translate("Splash", "Loading boards configuration"))
        #self.set_board() #called in self.get_status_board()
        self.status_info.setText(self.get_status_board())

        splash_write(QtGui.QApplication.translate("Splash", "Connecting events"))
        self.connect_events()
        splash_write(QtGui.QApplication.translate("Splash", "Loading configuration"))
        self.load_main_config()
        self.update_project_status(None)
        splash_write(QtGui.QApplication.translate("Splash", "Checking configuration files"))
        self.ide_check_files()

        os_name = os.getenv("PINGUINO_OS_NAME")
        if os_name == "windows":
            os.environ["PATH"] = os.environ["PATH"] + ";" + self.configIDE.get_path("sdcc_bin")

        elif os_name == "linux":
            os.environ["LD_LIBRARY_PATH"]="/usr/lib32:/usr/lib:/usr/lib64"

        splash_write(QtGui.QApplication.translate("Splash", "Welcome to {PINGUINO_NAME} {PINGUINO_VERSION}".format(**os.environ)))

        self.enable_debugger()
        logging.info("Welcome to {PINGUINO_FULLNAME}".format(**os.environ))

        self.setCorner(QtCore.Qt.BottomRightCorner, QtCore.Qt.RightDockWidgetArea)
        self.setCorner(QtCore.Qt.BottomLeftCorner, QtCore.Qt.LeftDockWidgetArea)
        self.set_sise_hint()

        PinguinoEvents.__init__(self)

        if os.getenv("PINGUINO_MODE") == "NORMAL":
            self.need_update(silent=True)

    #----------------------------------------------------------------------
    def set_sise_hint(self):
        """"""
        elements = ["dockWidget_bottom", "dockWidget_right", "plainTextEdit_log", "plainTextEdit_output", "tabWidget_bottom",
                    "tabWidget_tools", "comboBox_files", "treeWidget_explorer", "lineEdit_blocks_search",
                    "label_search_block", "pushButton_openproject", "pushButton_newproject", "treeWidget_projects",
                    "tableWidget_variables", "lineEdit_search", "lineEdit_replace", "checkBox_case_sensitive", "label_search",
                    "label_replace", "pushButton_search_previous", "pushButton_search_next", "pushButton_replace",
                    "pushButton_replace_all", "label_replace_info", "plainTextEdit_stdout"]

        for element in elements:
            getattr(self.main, element).minimumSizeHint = lambda :QtCore.QSize(0, 0)


    #----------------------------------------------------------------------
    def init_widgets(self):

        self.main.tabWidget_tools.setCurrentWidget(self.main.Files)
        self.main.tabWidget_bottom.setCurrentWidget(self.main.Log)

        self.main.dockWidget_right.setTitleBarWidget(QtGui.QWidget())
        self.main.dockWidget_bottom.setTitleBarWidget(QtGui.QWidget())


    #----------------------------------------------------------------------
    def set_styleSheet(self):
        #self.load_fonts()

        self.PinguinoPallete = BackgroundPallete()
        self.PinguinoPallete.save_palette(self.main.centralwidget.parent())
        self.switch_color_theme(self.configIDE.config("Main", "color_theme", False))

        bg_color = self.configIDE.config("Styles", "background_color", "#FFFFFF")
        alternate_bg_color = self.configIDE.config("Styles", "alternate_background_color", "#DDE8FF")
        selection_color = self.configIDE.config("Styles", "selection_color", "#FFFFFF")
        selection_bg_color = self.configIDE.config("Styles", "selection_foreground_color", "#57AAFF")

        self.main.tableWidget_functions.setStyleSheet("""
        QTableWidget {{
            background-color: {};
            alternate-background-color: {};
        }}
        """.format(bg_color, alternate_bg_color))

        self.main.tableWidget_directives.setStyleSheet("""
        QTableWidget {{
            background-color: {};
            alternate-background-color: {};
        }}
        """.format(bg_color, alternate_bg_color))

        self.main.tableWidget_variables.setStyleSheet("""
        QTableWidget {{
            background-color: {};
            alternate-background-color: {};
        }}
        """.format(bg_color, alternate_bg_color))


        self.main.statusBar.setStyleSheet("""
        font-family: inherit;
        font-weight: normal;
        selection-color: {};
        selection-background-color: {};
        """.format(selection_color, selection_bg_color))


        #Python shell CSS styles
        self.main.plainTextEdit_output.setStyleSheet("""
        QPlainTextEdit {
            background-color: #000;
            color: #FFFFFF;
            font-family: mono;
            font-weight: normal;
            font-size: 10pt;
        }
        """)

        #Log CSS styles
        self.main.plainTextEdit_log.setStyleSheet("""
        QPlainTextEdit {
            background-color: #000;
            color: #FFFFFF;
            font-family: mono;
            font-weight: normal;
            font-size: 10pt;
        }
        """)

        #StdOut CSS styles
        self.main.plainTextEdit_stdout.setStyleSheet("""
        QPlainTextEdit {
            background-color: #FFFFFF;
            font-family: mono;
            font-weight: normal;
            font-size: 10pt;
        }
        """)

        #Assistant CSS styles
        self.main.plainTextEdit_assistant.setStyleSheet("""
        QPlainTextEdit {
            background-color: #FFFFFF;
            font-family: mono;
            font-weight: normal;
            font-size: 9pt;
        }
        """)




if os.getenv("PINGUINO_MODE") == "NORMAL":
    for name, fn in inspect.getmembers(PinguinoIDE):
        if isinstance(fn, types.UnboundMethodType):
            setattr(PinguinoIDE, name, Decorator.debug_method()(fn))