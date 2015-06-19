#! /usr/bin/python2
#-*- coding: utf-8 -*-

import os
import re
import codecs

from ..methods.decorators import Decorator

from PySide import QtCore

data_types = ["int", "float", "char", "double", "u8", "u16", "u32", "u64", "BOOL", "byte", "word", "void"]
preprocessor_commands = ["define", "include", "error", "undef", "if", "else", "if", "elif", "ifdef", "ifndef", "line", "pragma"]  #endif


########################################################################
class CodeNavigator(object):

    #----------------------------------------------------------------------
    def __init__(self):

        self.connect(self.main.tableWidget_functions, QtCore.SIGNAL("doubleClicked(QModelIndex)"), self.jump_function)
        self.connect(self.main.tableWidget_directives, QtCore.SIGNAL("doubleClicked(QModelIndex)"), self.jump_directive)
        self.connect(self.main.tableWidget_variables, QtCore.SIGNAL("doubleClicked(QModelIndex)"), self.jump_variable)
        self.connect(self.main.tableWidget_functions.verticalHeader(), QtCore.SIGNAL("sectionClicked(int)"), self.jump_function)
        self.connect(self.main.tableWidget_directives.verticalHeader(), QtCore.SIGNAL("sectionClicked(int)"), self.jump_directive)
        self.connect(self.main.tableWidget_variables.verticalHeader(), QtCore.SIGNAL("sectionClicked(int)"), self.jump_variable)


    #----------------------------------------------------------------------
    def remove_comments(self, text):

        if type(text) == type([]):
            text = "".join(text)

        def replacer(match):
            s = match.group(0)

            if s.startswith('/'):
                #return "" #bug in line number in error info, multiline comments
                return "" + "\n" * (s.count("\n"))

            else:
                return s

        pattern = re.compile(
            r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
            re.DOTALL | re.MULTILINE
        )
        textout = re.sub(pattern, replacer, text)

        if type(text) == type([]):
            textout = textout.split("\n")

        return textout


    #----------------------------------------------------------------------
    def get_functions(self):

        regex_function = "[\s]*(unsigned|signed|long)*[\s]*(" + "|".join(data_types) + ")[\s]*(\*?)[\s]*([*\w]+)[\s]*\(([\w ,*.\[\]]*)\)[\s]*"
        #regex_function_content = "[\s]*%s[\s]*\{[\s\S]*\}[\s]*"

        funtions = []
        files = self.get_files_to_explore()
        for file_ in files:

            filename = file_["filename"]
            content = file_["content"]

            full_text = self.remove_comments(content)
            content = full_text.split("\n")

            for line in range(len(content)):

                match = re.match(regex_function, content[line])
                if match:
                    this_function = {}

                    #full_text = editor.text_edit.toPlainText()
                    index = full_text.find(content[line])
                    index_start = index
                    index_end = index
                    bracket = 1

                    try:
                        while full_text[index_end] != "{": index_end += 1
                    except IndexError:
                        bracket = 0

                    while bracket != 0:
                        index_end += 1
                        try:
                            if full_text[index_end] == "{": bracket += 1
                            elif full_text[index_end] == "}": bracket -= 1
                        except IndexError:
                            index_end = index_start - 1
                            break

                    count = full_text[index_start:index_end+1].count("\n")

                    this_function["content"] = full_text[index_start:index_end+1]
                    # this_function["line"] = (str(line + 1), str(line + 1 + count))
                    this_function["line"] = line + 1
                    this_function["name"] = match.groups()[2] + match.groups()[3]
                    this_function["return"] = ("" if not match.groups()[2] else "pointer to ") + match.groups()[1]
                    this_function["args"] = match.groups()[4]
                    this_function["filename"] = filename
                    funtions.append(this_function)

        return funtions


    #----------------------------------------------------------------------
    def get_directives(self, editor):

        regex_directive = "[\s]*#(" + "|".join(preprocessor_commands)+ ")[\s]+<?[\s]*([\w.]*)[\s]*>?[\s]*([\S]*)"

        directives = []

        files = self.get_files_to_explore()
        for file_ in files:

            filename = file_["filename"]
            content = file_["content"]

            content = self.remove_comments(content)
            content = content.split("\n")

            for line in range(len(content)):
                match = re.match(regex_directive, content[line])
                this_directive = {}
                if match:
                    this_directive["type"] = "#" + match.groups()[0]
                    this_directive["name"] = match.groups()[1]
                    this_directive["value"] = match.groups()[2]
                    this_directive["line"] = line + 1
                    this_directive["filename"] = filename
                    directives.append(this_directive)

        return directives




    #----------------------------------------------------------------------
    def get_variables(self, editor):

        regex_variables = "[\s]*(volatile|register|static|extern)*[\s]*(unsigned|signed)*[\s]*(short|long)*[\s]*(" + "|".join(data_types) + ")[\s]*([*])*[\s]*([ \w\[\]=,.{}\"'\*]*);"

        variables = []
        files = self.get_files_to_explore()
        for file_ in files:

            filename = file_["filename"]
            content = file_["content"]

            content = self.remove_comments(content)
            content = content.split("\n")
            for line in range(len(content)):
                match = re.match(regex_variables, content[line])
                if match:
                    args = match.groups()[5]
                    l = []
                    index = 0
                    while index < len(args):
                        if args[index] == "{":
                            start = index
                            bracket = 1

                            while bracket != 0:
                                index += 1
                                if args[index] == "{": bracket += 1
                                if args[index] == "}": bracket -= 1
                            end = index

                            l.append(args[start:end+1])

                        else: index += 1

                    for j in l: args = args.replace(j, "", 1)
                    args = args.split(",")
                    args = map(lambda a:a[:a.find("=")] if "=" in a else a, args)

                    type_ = " ".join([("" if not match.groups()[4] else "pointer to "),
                                    ("" if not match.groups()[0] else match.groups()[0]),
                                    ("" if not match.groups()[1] else match.groups()[1]),
                                    ("" if not match.groups()[2] else match.groups()[2]),
                                    match.groups()[3]])

                    while type_.startswith(" "): type_ = type_[1:]

                    for arg in args:
                        this_variable = {}
                        this_variable["type"] = type_
                        if re.match("(.*)(\[.*\])", arg):
                            this_variable["name"] = re.match("(.*)(\[.*\])", arg).groups()[0]
                        else:
                            this_variable["name"] = arg
                        #this_variable["value"] = match.groups()[6]
                        this_variable["line"] = line + 1
                        this_variable["filename"] = filename
                        variables.append(this_variable)

        return variables


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
        if os.environ["PINGUINO_PROJECT"]:
            """"""
            # files = self.get_files_from_project()
            filenames = [s[0] for s in filter(lambda l:l[1], self.get_files_from_project().items())]
            filenames.extend([s[0] for s in filter(lambda l:l[1], self.get_inherits_from_project().items())])


            files = [{"filename": filename, "content": str(codecs.open(filename, encoding="utf-8").read()),} for filename in filenames]

            return files




        else:
            if not self.is_graphical():
                return [{"filename": None,
                         "content": str(self.get_current_editor().text_edit.toPlainText())}]
            else:
                return []
