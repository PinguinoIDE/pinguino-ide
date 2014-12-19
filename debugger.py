#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from configparser import RawConfigParser
else:
    #Python2
    from ConfigParser import RawConfigParser

import sys
import codecs
from datetime import datetime


########################################################################
class sys_redirect(object):
    """"""

    #----------------------------------------------------------------------
    def __init__(self, debug, clear):
        """Constructor"""


        config_paths = RawConfigParser()
        config_paths.readfp(open(os.path.join(os.path.abspath(sys.path[0]), "paths.cfg"), "r"))

        if os.name == "posix": #GNU/Linux
            os_name = "linux"

        #Mac could return posix :/
        elif os.name == "os2":  #Mac OS X
            os_name = "macosx"

        elif os.name == "nt":  #Windows
            os_name = "windows"

        #load path from paths.conf
        user_dir = os.path.expanduser(config_paths.get("paths-%s"%os_name, "user_path"))
        if not os.path.exists(user_dir):
            os.mkdir(user_dir)


        if debug == "stdout":
            self.filename = os.path.join(user_dir, "pinguino.stdout")

        if debug == "stderr":
            self.filename = os.path.join(user_dir, "pinguino.stderr")

        if clear:
            if os.path.isfile(self.filename): os.remove(self.filename)


    #----------------------------------------------------------------------
    def write(self, out):
        if not out or out.isspace(): return
        file = codecs.open(self.filename, "a", "utf-8")
        file.write(datetime.now().ctime())
        file.write("\t\t"+out)
        if not out.endswith("\n"): file.write("\n")
        file.close()

        #if self.gui_output: self.gui_output(out)



########################################################################
class Debugger(object):
    """"""

    #----------------------------------------------------------------------
    def __init__(self, sys_, clear=False):
        """Constructor"""
        # sys.stderr = sys_redirect("stderr", clear)
        # sys.stdout = sys_redirect("stdout", clear)



