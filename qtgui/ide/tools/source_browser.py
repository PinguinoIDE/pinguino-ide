#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import re
import codecs
import pickle

from ..methods.decorators import Decorator

from ...pinguino_core.pinguino_config import PinguinoConfig

from PySide import QtCore, QtGui

# data_types = ["int", "float", "char", "double", "u8", "u16", "u32", "u64", "BOOL", "byte", "word", "void", "short", "long"]  # Yes I know, void is NOT a data type.
# preprocessor_commands = ["define", "include", "error", "undef", "if", "else", "if", "elif", "ifdef", "ifndef", "line", "pragma"]  #endif


########################################################################
class SourceBrowser(object):

    #----------------------------------------------------------------------
    def __init__(self):

        self.connect(self.main.tableWidget_functions, QtCore.SIGNAL("doubleClicked(QModelIndex)"), self.jump_function)
        self.connect(self.main.tableWidget_directives, QtCore.SIGNAL("doubleClicked(QModelIndex)"), self.jump_directive)
        self.connect(self.main.tableWidget_variables, QtCore.SIGNAL("doubleClicked(QModelIndex)"), self.jump_variable)
        self.connect(self.main.tableWidget_functions.verticalHeader(), QtCore.SIGNAL("sectionClicked(int)"), self.jump_function)
        self.connect(self.main.tableWidget_directives.verticalHeader(), QtCore.SIGNAL("sectionClicked(int)"), self.jump_directive)
        self.connect(self.main.tableWidget_variables.verticalHeader(), QtCore.SIGNAL("sectionClicked(int)"), self.jump_variable)

        self.connect(self.main.label_library_edit, QtCore.SIGNAL("linkActivated(QString)"), self.open_file_from_link)
        # self.connect(self.main.label_headers_edit, QtCore.SIGNAL("linkActivated(QString)"), self.open_file_from_link)


    #----------------------------------------------------------------------
    def set_functions(self, functions):

        self.functions = functions
        # self.update_functions()


    #----------------------------------------------------------------------
    def get_functions(self):

        if hasattr(self, "functions"): return self.functions
        else: return []


    #----------------------------------------------------------------------
    def set_directives(self, directives):

        self.directives = directives
        # self.update_directives()


    #----------------------------------------------------------------------
    def get_directives(self):

        if hasattr(self, "directives"): return self.directives
        else: return []


    #----------------------------------------------------------------------
    def set_variables(self, variables):

        self.variables = variables
        # self.update_variables()


    #----------------------------------------------------------------------
    def get_variables(self):

        if hasattr(self, "variables"): return self.variables
        else: return []


    #----------------------------------------------------------------------
    @Decorator.clear_highlighted_lines()
    def jump_function(self, row):

        if type(row) == int:
            item = self.main.tableWidget_functions.verticalHeaderItem(row)
        else:
            item = self.main.tableWidget_functions.verticalHeaderItem(row.column())

        if getattr(item, "filename", False):
            self.ide_open_file_from_path(filename=item.filename)
        self.editor_highligh_line(item.line, "#DBFFE3")


    #----------------------------------------------------------------------
    @Decorator.clear_highlighted_lines()
    def jump_variable(self, row):

        if type(row) == int:
            item = self.main.tableWidget_variables.verticalHeaderItem(row)
        else:
            item = self.main.tableWidget_variables.verticalHeaderItem(row.column())

        if getattr(item, "filename", False):
            self.ide_open_file_from_path(filename=item.filename)
        self.editor_highligh_line(item.line, "#DBFFE3")


    #----------------------------------------------------------------------
    @Decorator.clear_highlighted_lines()
    def jump_directive(self, row):

        if type(row) == int:
            item = self.main.tableWidget_directives.verticalHeaderItem(row)
        else:
            item = self.main.tableWidget_directives.verticalHeaderItem(row.column())

        if getattr(item, "filename", False):
            self.ide_open_file_from_path(filename=item.filename)
        self.editor_highligh_line(item.line, "#DBFFE3")


    # #----------------------------------------------------------------------
    # @Decorator.clear_highlighted_lines()
    # def jump_directive(self, model_index):

        # column = model_index.column()
        # item = self.main.tableWidget_directives.itemFromIndex(model_index).text()
        # if column == 2:
            # line = item
            # self.editor_highligh_line(int(line), "#DBFFE3")




    #----------------------------------------------------------------------
    def get_files_to_explore(self):
        """"""
        if self.is_project() and not self.is_library():
            """"""
            # files = self.get_files_from_project()

            filenames = [s[0] for s in filter(lambda l:l[1], self.get_files_from_project().items())]
            filenames.extend([s[0] for s in filter(lambda l:l[1], self.get_inherits_from_project().items())])

            files = [{"filename": filename, "content": str(codecs.open(filename, encoding="utf-8").read()),} for filename in filenames]
            return files

        else:
            if self.is_graphical() is False:
                return [{"filename": "",
                         "content": str(self.get_current_editor().text_edit.toPlainText())}]
            else:
                return []



    #----------------------------------------------------------------------
    def update_assistant(self, name):
        """"""
        if not hasattr(self, "assistant"): self.set_assistant()

        if name in self.assistant:

            link = "<a href='{text}'><span style='text-decoration: none; color:#2980b9;'>{text}</span></a>"
            html = "<html><head/><body><p>{links}</p></body></html>"

            self.main.label_name_edit.setText("{} ({name})".format(name, **self.assistant[name]))
            self.main.label_args_edit.setText("({args})".format(**self.assistant[name]))
            self.main.label_library_edit.setText(html.format(links=link.format(text=self.assistant[name]["filename"])))

            return_ = self.assistant[name]["return"]
            if return_ == "void": return_ = "None"
            self.main.label_return_edit.setText("{}".format(return_))

            # return True
            self.main.label_source_browser.setVisible(False)


    #----------------------------------------------------------------------
    def set_assistant(self):
        """"""
        with open(os.path.join(os.getenv("PINGUINO_USER_PATH"), "reserved.pickle"), "rb") as file_reserved:
            self.assistant = pickle.load(file_reserved)["assistant"]


    #----------------------------------------------------------------------
    def open_file_from_link(self, name):
        """"""
        core = os.path.join(os.getenv("PINGUINO_INSTALL_PATH"), "p8", "include", "pinguino", "core")
        libraries = os.path.join(os.getenv("PINGUINO_INSTALL_PATH"), "p8", "include", "pinguino", "libraries")

        path = None
        if name in os.listdir(core):
            path =  os.path.join(core, name)
        elif name in os.listdir(libraries):
            path =  os.path.join(libraries, name)

        if path is None:
            for path_ in PinguinoConfig.get_p8_libraries() + PinguinoConfig.get_p32_libraries():
                if name in os.listdir(path_):
                    path = os.path.join(path_, name)
                    break

        if os.path.exists(path):
            self.ide_open_file_from_path(filename=path, readonly=True)



    # #----------------------------------------------------------------------
    # def update_code_navigator(self):
        # """"""
        # self.update_variables()
        # self.update_functions()
        # self.update_directives()

