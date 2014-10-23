#!/usr/bin/env python
#-*- coding: utf-8 -*-

from ConfigParser import RawConfigParser
import os
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

    argvs = ["-style"]

    #----------------------------------------------------------------------
    def __init__(self, sys_, clear=False):
        """Constructor"""
        if len(sys.argv) == 1 or self.ignore(sys.argv):
            sys.stderr = sys_redirect("stderr", clear)
            sys.stdout = sys_redirect("stdout", clear)



    #----------------------------------------------------------------------
    def ignore(self, sys_argv):
        """"""
        for argv in self.argvs:
            if argv in sys_argv: return True
