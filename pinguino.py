#! /usr/bin/python
#-*- coding: utf-8 -*-

NAME = "Pinguino IDE"
VERSION = "11.0"
SUBVERSION = "beta.3"

#DESCRIPTION = ""
#LONG_DESCRIPTION = ""

################################################################################

"""-------------------------------------------------------------------------
    Pinguino IDE

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
-------------------------------------------------------------------------"""

################################################################################

import sys
reload(sys)
sys.setdefaultencoding("utf-8")
import os

os.environ["NAME"] = NAME
os.environ["VERSION"] = VERSION
os.environ["SUBVERSION"] = SUBVERSION
os.environ["PINGUINO_HOME"] = os.path.abspath(sys.path[0])

# For PyInstaller compatibility
if os.path.exists(os.path.abspath("pinguino_data")):
    os.environ["PINGUINO_DATA"] = os.path.abspath("pinguino_data")
else:
    os.environ["PINGUINO_DATA"] = os.getenv("PINGUINO_HOME")


class bcolors:
    Black = "\033[0;30m"
    Red = "\033[0;31m"
    Green = "\033[0;32m"
    Yellow = "\033[0;33m"
    Blue = "\033[0;34m"
    Purple = "\033[0;35m"
    Cyan = "\033[0;36m"
    LightGray = "\033[0;37m"
    ENDC = "\033[0m"


if __name__ == "__main__":

    sys.path.append(os.path.join(os.getenv("PINGUINO_DATA"), "qtgui", "resources"))

    python_path_modules = os.path.join(os.getenv("PINGUINO_DATA"), "python_requirements")
    if os.path.isdir(python_path_modules): sys.path.append(python_path_modules)

    #from qtgui.ide import PinguinoIDE #overwrite locale ¿?
    #from PySide.QtGui import QApplication, QSplashScreen, QPixmap, QPainter
    from PySide import QtCore
    #import locale

    sys_locale = QtCore.QLocale.system().name()
    #sys_locale = "en"  #force language
    translator = QtCore.QTranslator()

    #load intern dialogs translations
    qtTranslator = QtCore.QTranslator()
    qtTranslator.load("qt_" + sys_locale, QtCore.QLibraryInfo.location(QtCore.QLibraryInfo.TranslationsPath))

    #load translations files
    translations_path = os.path.join(os.getenv("PINGUINO_DATA"), "multilanguage")
    trasnlations = os.path.exists(translations_path)

    if trasnlations:
        translations_file = "pinguino_" + sys_locale

        if translations_file + ".qm" in os.listdir(translations_path):
            translator.load(os.path.join(os.getenv("PINGUINO_DATA"), "multilanguage", "pinguino_%s.qm" % sys_locale))

        elif "_" in sys_locale:
            sys_locale = sys_locale[:sys_locale.find("_")]
            translations_file = "pinguino_" + sys_locale
            if translations_file + ".qm" in os.listdir(translations_path):
                translator.load(os.path.join(os.getenv("PINGUINO_DATA"), "multilanguage", "pinguino_%s.qm" % sys_locale))

    if len(sys.argv) == 1:
        from qtgui.ide import PinguinoIDE
        from PySide.QtGui import QApplication, QSplashScreen, QPixmap, QPainter

        app = QApplication(sys.argv)

        #from PySide import QtGui

        pixmap = QPixmap(":/logo/art/splash.png")
        splash = QSplashScreen(pixmap, QtCore.Qt.WindowStaysOnTopHint)

        splash.show()
        splash.setStyleSheet("""
            font-family: inherit;
            font-weight: normal;
            font-size: 11pt;

        """)

        def splash_write(msg):
            if not splash is None:
                splash.showMessage("\t"+msg+"\n", color=QtCore.Qt.white, alignment=QtCore.Qt.AlignBottom)

        splash_write(NAME+" "+VERSION)
        app.processEvents()

        app.installTranslator(qtTranslator)
        if trasnlations: app.installTranslator(translator)

        frame = PinguinoIDE(splash_write=splash_write)
        frame.show()

        if not splash is None:
            splash.finish(frame)

        #For PyInstaller compatibility
        if app is None:
            from PySide.QtGui import QApplication
            QApplication.instance().exec_()
        else:
            #sys.exit(app.exec_())
            app.exec_()


    else:  #command line

        from qtgui.pinguino_api.pinguino import Pinguino
        from qtgui.pinguino_api.pinguino_config import PinguinoConfig
        from qtgui.ide.methods.config import Config

        pinguino = Pinguino()
        PinguinoConfig.set_environ_vars()
        PinguinoConfig.check_user_files()
        config = Config()
        PinguinoConfig.update_pinguino_paths(config, Pinguino)
        PinguinoConfig.update_pinguino_extra_options(config, Pinguino)
        PinguinoConfig.update_user_libs(pinguino)

        def printb(text, color):
            print(color + text + bcolors.ENDC)

        parser = pinguino.build_argparse()

        if parser.version:
            printb("\t" + VERSION, bcolors.Green)
            sys.exit()

        if parser.author:
            printb("\tJean-Pierre Mandon", bcolors.Green)
            printb("\tRegis Blanchot", bcolors.Green)
            printb("\tYeison Cardona", bcolors.Green)
            sys.exit()

        if parser.board:
            pinguino.set_board(parser.board)
            printb("using %s board" % parser.board.name, bcolors.Green)

            if parser.bootloader:
                bootloader = pinguino.dict_boot.get(parser.bootloader[0].lower(), parser.board.bldr)
                pinguino.set_bootloader(bootloader)
            printb("using %s bootloader" % pinguino.get_board().bldr, bcolors.Green)

            if not parser.filename:
                printb("ERROR: missing filename", bcolors.Red)
                sys.exit(1)

            else:
                filename = parser.filename[0]

                fname, extension = os.path.splitext(filename)
                if extension != ".pde":
                    printb("ERROR: bad file extension, it should be .pde", bcolors.Red)
                    sys.exit()
                del fname, extension

                pinguino.compile_file(filename)

                if not pinguino.compiled():
                    printb("\nERROR: no compiled\n", bcolors.Red)

                    errors_proprocess = pinguino.get_errors_preprocess()
                    if errors_proprocess:
                        for error in errors_proprocess["preprocess"]: printb(error, bcolors.Red)

                    errors_c = pinguino.get_errors_compiling_c()
                    if errors_c:
                        printb(errors_c["complete_message"], bcolors.Red)

                    errors_asm = pinguino.get_errors_compiling_asm()
                    if errors_asm:
                        for error in errors_asm["error_symbols"]: printb(error, bcolors.Red)

                    errors_link = pinguino.get_errors_linking()
                    if errors_link:
                        for error in errors_link["linking"]: printb(error, bcolors.Red)

                    sys.exit()

                else:
                    result = pinguino.get_result()
                    printb("compilation time: %s" % result["time"], bcolors.Yellow)
                    printb("compiled to: %s" % result["hex_file"], bcolors.Yellow)

                    if parser.hex_file:
                        hex_file = open(result["hex_file"], "r")
                        content_hex = hex_file.readlines()
                        hex_file.close()
                        printb("\n" + "*" * 70, bcolors.Cyan)
                        printb(result["hex_file"], bcolors.Cyan)
                        printb("*" * 70, bcolors.Cyan)
                        for line in content_hex: printb(line, bcolors.Cyan),
                        printb("*" * 70 + "\n", bcolors.Cyan)

                if parser.upload:
                    try:
                        uploaded, result = pinguino.upload()
                        if result:
                            printb(result, bcolors.Green)
                    except:
                        if pinguino.get_board().arch == 8:
                            printb("ERROR: bootloader option might be incorrect.", bcolors.Red)
                            printb("Bootloader options: ", bcolors.Green),
                            printb(", ".join(pinguino.dict_boot.keys()), bcolors.Green)
