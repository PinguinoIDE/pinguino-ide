#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import sys
import shutil

from qtgui.pinguino_api.pinguino_config import PinguinoConfig

os.environ["PINGUINO_HOME"] = os.path.abspath(sys.path[0])

# For PyInstaller compatibility
if os.path.exists(os.path.abspath("pinguino_data")):
    os.environ["PINGUINO_DATA"] = os.path.abspath("pinguino_data")
else:
    os.environ["PINGUINO_DATA"] = os.getenv("PINGUINO_HOME")

PinguinoConfig.set_environ_vars()

#Remove files and directories
shutil.rmtree(os.path.join(os.environ["PINGUINO_USER_PATH"], "source"))

#Check files and directories
PinguinoConfig.check_user_files()

#Remove library names
os.remove(os.path.join(os.environ["PINGUINO_USER_PATH"], "reserved.pickle"))
