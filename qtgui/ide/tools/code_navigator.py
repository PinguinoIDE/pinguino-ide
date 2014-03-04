#!/usr/bin/env python
#-*- coding: utf-8 -*-

import re

data_types = ["int", "float", "char", "double", "u8", "u16", "u32", "u64", "BOOL", "byte", "word", "void"]
preprocessor_commands = ["define", "include", "error", "undef", "if", "else", "if", "elif", "ifdef", "ifndef", "line", "pragma"]  #endif


########################################################################
class CodeNavigator(object):

    #----------------------------------------------------------------------
    @classmethod
    def remove_comments(cls, content):
        #FIXME: needed revision
        for i in range(content.count("/*")):
            start_pos = content.find("/*")
            end_pos = content.find("*/")
            comment = content[start_pos:end_pos]
            content = content[:start_pos] + "\n"*comment.count("\n") + content[end_pos+2:]

        return content


    #----------------------------------------------------------------------
    @classmethod
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
    @classmethod
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
    @classmethod
    def get_variables(cls, editor):

        regex_variables = "[\s]*(volatile|register|static|extern)*[\s]*(unsigned|signed)*[\s]*(short|long)*[\s]*(" + "|".join(data_types) + ")[\s]*([*])*[\s]*([ \w\[\]=,{}\"'\*]*);"

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
