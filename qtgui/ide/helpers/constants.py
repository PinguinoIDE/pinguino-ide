#!/usr/bin/env python
#-*- coding: utf-8 -*-


import os
import sys

from PySide.QtCore import QDir


IDE_HOME_DIR = sys.path[0]

PINGUINO_EXAMPLES_DIR = os.path.join(IDE_HOME_DIR, "pinguino", "examples")
HOME_DIR = QDir.home().absolutePath()
DESKTOP_DIR = QDir.AllDirs


PINGUINOG_EXAMPLES_DIR = os.path.join(IDE_HOME_DIR, "qtgui", "gide", "examples")

PINGUINO_STDOUT_FILE = os.path.join(IDE_HOME_DIR, "pinguino", "source", "stdout")

IDE_STDOUT_FILE = os.path.join(IDE_HOME_DIR, "pinguino.panic")
IDE_CONFIG_FILE = os.path.join(IDE_HOME_DIR, "qtgui", "config", "pinguino.conf")

NAME = "Pinguino IDE"
VERSION = "11.0"
TAB_NAME = "Pinguino 11.0"
NAME_LONG = " ".join([NAME, VERSION])
