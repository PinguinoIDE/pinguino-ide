#! /usr/bin/python2
#-*- coding: utf-8 -*-

import re

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
        self.connect(self.main.tableWidget_functions.verticalHeader(), QtCore.SIGNAL("sectionClicked(int)"), self.jump_function_header)
        self.connect(self.main.tableWidget_directives.verticalHeader(), QtCore.SIGNAL("sectionClicked(int)"), self.jump_directive_header)
        self.connect(self.main.tableWidget_variables.verticalHeader(), QtCore.SIGNAL("sectionClicked(int)"), self.jump_variable_header)


    #----------------------------------------------------------------------
    def remove_comments(cls, text):

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
    def get_functions(cls, editor):

        regex_function = "[\s]*(unsigned|signed|long)*[\s]*(" + "|".join(data_types) + ")[\s]*(\*?)[\s]*([*\w]+)[\s]*\(([\w ,*.\[\]]*)\)[\s]*"
        #regex_function_content = "[\s]*%s[\s]*\{[\s\S]*\}[\s]*"

        funtions = []
        full_text = cls.remove_comments(editor.text_edit.toPlainText())
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
                this_function["line"] = (str(line + 1), str(line + 1 + count))
                this_function["name"] = match.groups()[2] + match.groups()[3]
                this_function["return"] = ("" if not match.groups()[2] else "pointer to ") + match.groups()[1]
                this_function["args"] = match.groups()[4]
                funtions.append(this_function)

        return funtions


    #----------------------------------------------------------------------
    def get_directives(cls, editor):

        regex_directive = "[\s]*#(" + "|".join(preprocessor_commands)+ ")[\s]+<?[\s]*([\w.]*)[\s]*>?[\s]*([\S]*)"

        directives = []
        content = cls.remove_comments(editor.text_edit.toPlainText())
        content = content.split("\n")

        for line in range(len(content)):
            match = re.match(regex_directive, content[line])
            this_directive = {}
            if match:
                this_directive["type"] = "#" + match.groups()[0]
                this_directive["name"] = match.groups()[1]
                this_directive["value"] = match.groups()[2]
                this_directive["line"] = str(line + 1)
                directives.append(this_directive)

        return directives




    #----------------------------------------------------------------------
    def get_variables(cls, editor):

        regex_variables = "[\s]*(volatile|register|static|extern)*[\s]*(unsigned|signed)*[\s]*(short|long)*[\s]*(" + "|".join(data_types) + ")[\s]*([*])*[\s]*([ \w\[\]=,.{}\"'\*]*);"

        variables = []
        content = cls.remove_comments(editor.text_edit.toPlainText())
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
                    this_variable["line"] = str(line + 1)
                    variables.append(this_variable)

        return variables


    #----------------------------------------------------------------------
    @Decorator.clear_highlighted_lines()
    def jump_function(self, model_index):
        column = model_index.column()
        item = self.main.tableWidget_functions.itemFromIndex(model_index).text()
        if column == 2:
            line = item[:item.find("-")]
            self.editor_highligh_line(int(line), "#DBFFE3")


    #----------------------------------------------------------------------
    @Decorator.clear_highlighted_lines()
    def jump_directive(self, model_index):
        column = model_index.column()
        item = self.main.tableWidget_directives.itemFromIndex(model_index).text()
        if column == 2:
            line = item
            self.editor_highligh_line(int(line), "#DBFFE3")



    #----------------------------------------------------------------------
    @Decorator.clear_highlighted_lines()
    def jump_variable(self, model_index):
        column = model_index.column()
        item = self.main.tableWidget_variables.itemFromIndex(model_index).text()
        if column == 1:
            line = item
            self.editor_highligh_line(int(line), "#DBFFE3")


    #----------------------------------------------------------------------
    @Decorator.clear_highlighted_lines()
    def jump_function_header(self, row):
        item = self.main.tableWidget_functions.verticalHeaderItem(row).text()
        line = item[item.find(":")+1:][:item[item.find(":")+1:].find("-")]
        self.highligh_line(int(line), "#DBFFE3")


    #----------------------------------------------------------------------
    @Decorator.clear_highlighted_lines()
    def jump_directive_header(self, row):
        item = self.main.tableWidget_directives.verticalHeaderItem(row).text()
        line = item[item.find(":")+1:]
        self.editor_highligh_line(int(line), "#DBFFE3")


    #----------------------------------------------------------------------
    @Decorator.clear_highlighted_lines()
    def jump_variable_header(self, row):
        item = self.main.tableWidget_variables.verticalHeaderItem(row).text()
        line = item[item.find(":")+1:]
        self.editor_highligh_line(int(line), "#DBFFE3")