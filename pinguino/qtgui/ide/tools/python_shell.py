#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

from ..custom_widgets import PinguinoTextEdit


########################################################################
class PythonShell(object):

    def __init__(self):

        self.main.plainTextEdit_output = PinguinoTextEdit()
        self.main.plainTextEdit_output.set_extra_args(**{"pinguino": self})
        self.main.gridLayout_shell.addWidget(self.main.plainTextEdit_output, 0, 0, 1, 1)

# import inspect
# import types
# from ..methods.decorators import Decorator
# for name, fn in inspect.getmembers(PinguinoTextEdit):
    # if isinstance(fn, types.UnboundMethodType):
        # setattr(PinguinoTextEdit, name, Decorator.debug_method()(fn))