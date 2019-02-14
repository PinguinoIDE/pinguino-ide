#! /usr/bin/python
#-*- coding: utf-8 -*-

import os
import sys
import shutil

try:
    import version
except:
    from . import version

try:
    from qtgui.pinguino_core.pinguino_config import PinguinoConfig
except:
    from .qtgui.pinguino_core.pinguino_config import PinguinoConfig

os.environ["PINGUINO_LIB"] = os.path.abspath(os.path.dirname(__file__))

PinguinoConfig.set_environ_vars()

#Check files and directories
PinguinoConfig.check_user_files()

#Remove files and directories
#if os.path.isdir(os.path.join(os.getenv("PINGUINO_USER_PATH"), "source")):
#    shutil.rmtree(os.path.join(os.getenv("PINGUINO_USER_PATH"), "source"))

# Deprecated (never released)
# if os.path.isdir(os.path.join(os.getenv("PINGUINO_USER_PATH"), "patches")):
    # shutil.rmtree(os.path.join(os.getenv("PINGUINO_USER_PATH"), "patches"))

#Remove old files
#RB20150202 : each file must be checked before being deleted
if os.path.exists(os.path.join(os.getenv("PINGUINO_USER_PATH"), "reserved.pickle")):
    os.remove(os.path.join(os.getenv("PINGUINO_USER_PATH"), "reserved.pickle"))
if os.path.exists(os.path.join(os.getenv("PINGUINO_USER_PATH"), "pinguino.conf")):
    os.remove(os.path.join(os.getenv("PINGUINO_USER_PATH"), "pinguino.conf"))

