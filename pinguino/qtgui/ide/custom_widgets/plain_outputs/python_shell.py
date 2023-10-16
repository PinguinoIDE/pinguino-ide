#!/usr/bin/env python
#-*- coding: utf-8 -*-

import sys
import os

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") == "3":
    #Python3
    # import builtins
    from io import StringIO
else:
    #Python2
    # import __builtin__
    from cStringIO import StringIO

import traceback
import types

########################################################################
class PythonShell(object):

    #----------------------------------------------------------------------
    def __init__(self):

        self.statement_module = types.ModuleType("__main__")
        # self.statement_module.__builtins__ = builtins.__builtins__

        sys.modules['__main__'] = self.statement_module
        self.statement_module.__name__ = '__main__'

    #----------------------------------------------------------------------
    def run(self, command, log=None):

        if log is None: log = StringIO()
        sys.stdout = log
        sys.stderr = log

        try:
            compiled = compile(command, '<string>', 'single')
            exec(compiled, self.statement_module.__dict__)

        except: log.write(traceback.format_exc())
        # return log.getvalue()

    #----------------------------------------------------------------------
    def restart(self):

        self.__init__()
