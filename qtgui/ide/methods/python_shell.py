#!/usr/bin/env python
#-*- coding: utf-8 -*-

import __builtin__
import sys
import traceback
import types
from cStringIO import StringIO

########################################################################
class PythonShell(object):

    #----------------------------------------------------------------------
    def __init__(self):

        self.statement_module = types.ModuleType("__main__")
        self.statement_module.__builtins__ = __builtin__

        sys.modules['__main__'] = self.statement_module
        self.statement_module.__name__ = '__main__'

    #----------------------------------------------------------------------
    def run(self, command):

        log = StringIO()
        sys.stdout = log
        sys.stderr = log

        try:
            compiled = compile(command, '<string>', 'single')
            exec compiled in self.statement_module.__dict__
        except: log.write(traceback.format_exc())
        return log.getvalue()

    #----------------------------------------------------------------------
    def restart(self):

        self.__init__()
