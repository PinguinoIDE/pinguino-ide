#! /usr/bin/python2
#-*- coding: utf-8 -*-

import os
import sys
import shutil

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from configparser import RawConfigParser
else:
    #Python2
    from ConfigParser import RawConfigParser


from qtgui.pinguino_api.pinguino_config import PinguinoConfig

os.environ["PINGUINO_HOME"] = os.path.abspath(sys.path[0])

# For PyInstaller compatibility
if os.path.exists(os.path.abspath("pinguino_data")):
    os.environ["PINGUINO_DATA"] = os.path.abspath("pinguino_data")
else:
    os.environ["PINGUINO_DATA"] = os.getenv("PINGUINO_HOME")

PinguinoConfig.set_environ_vars()

#Remove files and directories
if os.path.isdir(os.path.join(os.getenv("PINGUINO_USER_PATH"), "source")):
    shutil.rmtree(os.path.join(os.getenv("PINGUINO_USER_PATH"), "source"))

if os.path.isdir(os.path.join(os.getenv("PINGUINO_USER_PATH"), "patches")):
    shutil.rmtree(os.path.join(os.getenv("PINGUINO_USER_PATH"), "patches"))

#Remove old files
#RB20150202 : each file must be checked before being deleted
if os.path.isfile(os.path.join(os.getenv("PINGUINO_USER_PATH"), "reserved.pickle")):
    os.remove(os.path.join(os.getenv("PINGUINO_USER_PATH"), "reserved.pickle"))
if not os.path.isfile(os.path.join(os.getenv("PINGUINO_USER_PATH"), "pinguino.conf")):
    os.remove(os.path.join(os.getenv("PINGUINO_USER_PATH"), "pinguino.conf"))
if os.path.isfile(os.path.join(os.getenv("PINGUINO_USER_PATH"), "wikidocs.pickle")):
    os.remove(os.path.join(os.getenv("PINGUINO_USER_PATH"), "wikidocs.pickle"))


#Check files and directories
PinguinoConfig.check_user_files()

#Remove patches
parser = RawConfigParser()
parser.readfp(open(os.path.join(os.getenv("PINGUINO_USER_PATH"), "pinguino.conf"), "r"))
parser.remove_section("Patches")
parser.write(open(os.path.join(os.getenv("PINGUINO_USER_PATH"), "pinguino.conf"), "w"))