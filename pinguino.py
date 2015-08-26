#! /usr/bin/python2
#-*- coding: utf-8 -*-

from version import NAME, VERSION

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

import sys
import os
import argparse

from PySide import QtCore, QtGui
from PySide.QtGui import QApplication, QSplashScreen, QPixmap

#----------------------------------------------------------------------
def build_argparse():

    #IDE args
    parsergui = argparse.ArgumentParser(description="*** {PINGUINO_NAME}:GUI ***".format(**os.environ))
    parsergui.add_argument("--lang", dest="lang", nargs=1, default=False, help="set IDE language")
    parsergui.add_argument("--dev", dest="devmode", action="store_true", default=False, help="set IDE developer mode")
    parsergui.add_argument("-style")

    parsed_g, others_g = parsergui.parse_known_args()


    if others_g:
        parsergui.print_help()
        sys.exit()
    else: return parsed_g

parser = build_argparse()

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    python_path_modules = os.path.join(os.getenv("PINGUINO_DATA"), "batteries", "py3")
else:
    #Python2
    python_path_modules = os.path.join(os.getenv("PINGUINO_DATA"), "batteries", "py2")

if parser.devmode:
    os.environ["PINGUINO_MODE"] = "DEV"
else:
    os.environ["PINGUINO_MODE"] = "NORMAL"

if os.path.isdir(python_path_modules): sys.path.append(python_path_modules)

sys.path.append(os.path.join(os.getenv("PINGUINO_DATA"), "qtgui", "resources"))
# import resources_rc

from qtgui.ide.ide import PinguinoIDE

if __name__ == "__main__":

    if parser.lang:
        sys_locale = parser.lang[0]
    else:
        sys_locale = QtCore.QLocale.system().name()
    translator = QtCore.QTranslator()

    #load intern dialogs translations
    qtTranslator = QtCore.QTranslator()
    qtTranslator.load("qt_" + sys_locale, QtCore.QLibraryInfo.location(QtCore.QLibraryInfo.TranslationsPath))

    #load translations files
    translations_path = os.path.join(os.getenv("PINGUINO_DATA"), "multilanguage")
    trasnlations = os.path.exists(translations_path)

    if trasnlations and (os.getenv("PINGUINO_MODE") == "NORMAL"):
        translations_file = "pinguino_" + sys_locale

        if translations_file + ".qm" in os.listdir(translations_path):
            translator.load(os.path.join(os.getenv("PINGUINO_DATA"), "multilanguage", "pinguino_{}.qm".format(sys_locale)))

        elif "_" in sys_locale:
            sys_locale = sys_locale[:sys_locale.find("_")]
            translations_file = "pinguino_" + sys_locale
            if translations_file + ".qm" in os.listdir(translations_path):
                translator.load(os.path.join(os.getenv("PINGUINO_DATA"), "multilanguage", "pinguino_{}.qm".format(sys_locale)))

    app = QApplication(sys.argv)

    #Splash
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
            splash.showMessage("\t"+msg, color=QtGui.QColor("#4d4d4d"), alignment=QtCore.Qt.AlignBottom)

    splash_write(NAME+" "+VERSION)
    app.processEvents()

    app.installTranslator(qtTranslator)
    if trasnlations:
        app.installTranslator(translator)

    frame = PinguinoIDE(splash_write=splash_write)
    frame.show()

    if not splash is None:
        splash.finish(frame)

    app.exec_()

