#! /usr/bin/python2
#-*- coding: utf-8 -*-

import inspect
import os
import sys
#import pickle

allBlocks = {}

########################################################################
class UserBlocks(object):

    #----------------------------------------------------------------------
    def __init__(self):

        path = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "blocks")
        if not os.path.exists(path): return
        sets = os.listdir(os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "blocks"))
        sys.path.append(os.path.join(os.getenv("PINGUINO_HOME"), "qtgui", "gide", "py_bloques"))

        for dir_ in sets:
            files = os.listdir(os.path.join(path, dir_))
            files = filter(lambda f:f.endswith("py") and f != "__init__.py" and (not f.startswith(".")), files)
            sys.path.append(os.path.join(path, dir_))
            for module in files:
                mod = __import__(module.replace(".py", ""))
                self.extractBlocks(getattr(mod, "PinguinoBlock"))
            sys.path.remove(os.path.join(path, dir_))
        sys.path.remove(os.path.join(os.getenv("PINGUINO_HOME"), "qtgui", "gide", "py_bloques"))


    #----------------------------------------------------------------------
    def getBlocks(self):

        return allBlocks


    #----------------------------------------------------------------------
    def extractBlocks(self, mod):

        alls = inspect.getmembers(mod())
        for block in alls:
            if hasattr(block[1], "im_class"):
                if block[0] != "__init__" and block[1].im_class == mod().__init__.im_class:
                    allBlocks[block[0]] = block[1]()
