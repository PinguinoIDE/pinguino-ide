#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import json

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from urllib.request import urlopen
else:
    #Python2
    from urllib import urlopen

from PySide import QtCore, QtGui

from .parser import PinguinoParser


########################################################################
class UpdateAutocompleter(QtCore.QThread):
    """"""
    signal_add_autocompleter = QtCore.Signal(str, str, str)
    signal_rm_autocompleter = QtCore.Signal(str)

    signal_set_variables = QtCore.Signal(list)
    signal_set_directives = QtCore.Signal(list)
    signal_set_functions = QtCore.Signal(list)


    #----------------------------------------------------------------------
    def __init__(self, parent=None):
        """Constructor"""

        # Python3 compatibility
        if os.getenv("PINGUINO_PYTHON") is "3":
            #Python3
            super().__init__(parent)
        else:
            #Python2
            super(UpdateAutocompleter, self).__init__(parent)


    #----------------------------------------------------------------------
    def set_files(self, files):
        """"""
        self.parser = PinguinoParser(files)


    #----------------------------------------------------------------------
    def run(self):

        self.completer_functions = []
        self.functions_args = {}
        functions_parse = self.parser.get_functions()
        self.signal_set_functions.emit(functions_parse)

        for funtion in functions_parse:
            self.completer_functions.append(funtion["name"])
            self.functions_args[funtion["name"]] = "{name}({{{{{args}}}}});".format(**funtion)

        self.completer_variables = []
        variables_parse = self.parser.get_variables()
        self.signal_set_variables.emit(variables_parse)

        for variable in variables_parse:
            self.completer_variables.append([variable["name"], variable["type"]])

        self.completer_directives = []
        directives_parse = self.parser.get_directives()
        self.signal_set_directives.emit(directives_parse)

        for directive in directives_parse:
            self.completer_directives.append(directive["name"])

        self.signal_rm_autocompleter.emit("directives")
        self.signal_rm_autocompleter.emit("variables")
        self.signal_rm_autocompleter.emit("functions")

        for item in self.completer_directives:
            self.signal_add_autocompleter.emit("directives", item, "iconDirectives")

        for item in self.completer_functions:
            self.signal_add_autocompleter.emit("functions", item, "iconFunction")

        for item, type_ in self.completer_variables:
            type_ = type_.split()[-1]
            if type_ in "int float bool byte bool double struct union".split():
                icon = "icon{}".format(type_.capitalize())
            else:
                icon = ""

            self.signal_add_autocompleter.emit("variables", item, icon)



########################################################################
class PinguinoRequest(QtCore.QThread):
    """"""
    signal_message = QtCore.Signal(bool, str)


    #----------------------------------------------------------------------
    def __init__(self, url):
        """Constructor"""
        super(PinguinoRequest, self).__init__(None)
        self.url = url


    #----------------------------------------------------------------------
    def run(self):
        """"""
        try:
            response = urlopen(self.url)
            response = response.read().decode("utf8")
            version = json.loads(response).get("tag_name", None)

            if version.lower().startswith("v"):
                version = version[1:]

            if version:
                local_version = os.environ.get("PINGUINO_VERSION")

                html = """
                    <html>
                      <head/>
                      <body>
                        <p>
                          <b>{}</b><br>
                          A new version of Pinguino IDE is available at:<br>
                          <a href="http://www.pinguino.cc/download"><span style=" text-decoration: underline; color:#2980b9;">http://www.pinguino.cc/download</span></a>
                        </p>
                      </body>
                    </html>""".format(version)

                if version > local_version:
                    self.signal_message.emit(True, html)
                    return

                else:
                    self.signal_message.emit(False, "Your copy of Pinguino IDE is up to date.")

        except Exception as msg:
            self.signal_message.emit(False, "ConnectionError")
            return
