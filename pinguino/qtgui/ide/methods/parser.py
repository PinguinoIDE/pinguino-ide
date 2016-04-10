#!/usr/bin/env python
#-*- coding: utf-8 -*-

import re

data_types = ["int", "float", "char", "double", "u8", "u16", "u32", "u64", "BOOL", "byte", "word", "void", "short", "long"]  # Yes I know, void is NOT a data type.
preprocessor_commands = ["define", "include", "error", "undef", "if", "else", "if", "elif", "ifdef", "ifndef", "line", "pragma"]  #endif

#----------------------------------------------------------------------
def remove_comments(textinput, ignore_spaces=False):

    if type(textinput) == list:
        text = "".join(textinput)
    else:
        text = textinput

    def replacer(match):
        s = match.group(0)

        if s.startswith('/'):
            if not ignore_spaces:
                # return " "*(len(s)-s.count("\n")) + "\n" * (s.count("\n"))
                return "" + "\n" * (s.count("\n"))
            else:
                return ""
        else:
            return s

    pattern = re.compile(r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"', re.DOTALL|re.MULTILINE)
    textout = re.sub(pattern, replacer, text)

    if type(textinput) == list:
        textout = textout.split("\n")
        textout = list(map(lambda x:x+"\n", textout))

    return textout



########################################################################
class PinguinoParser(object):
    """"""
    #----------------------------------------------------------------------
    def __init__(self, files):
        """"""
        self.files = files


    #----------------------------------------------------------------------
    def get_variables(self):
        """"""
        regex_variables = "[\s]*(volatile|register|static|extern)*[\s]*(unsigned|signed)*[\s]*(" + "|".join(data_types) + ")+[\s]*( \* )*[\s]*([ \w\[\]=,.{}\"']*);"
        # regex_variables = "[\s]*(volatile|register|static|extern)*[\s]*(unsigned|signed)*[\s]*(short|long)*[\s]*(" + "|".join(data_types) + ")[\s]*+([*])*([ \w\[\]=,.{}\"'\*]*);"

        variables = []

        # self.signal_set_variables.emit(variables)

        # # if files is None:
        # files = self.ide.get_files_to_explore()

        for file_ in self.files:

            filename = file_["filename"]
            content = file_["content"]

            content = remove_comments(content)
            content = content.split("\n")

            for line in range(len(content)):
                match = re.match(regex_variables, content[line])
                if match:
                    args = match.groups()[4]
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

                    type_ = " ".join([("" if not match.groups()[3] else "pointer to "),
                                    ("" if not match.groups()[0] else match.groups()[0]),
                                    ("" if not match.groups()[1] else match.groups()[1]),
                                    (match.groups()[2]),
                                    # match.groups()[3]
                                    ])

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
    def get_directives(self):

        regex_directive = "[\s]*#(" + "|".join(preprocessor_commands)+ ")[\s]+<?[\s]*([\w.]*)[\s]*>?[\s]*([\S]*)"

        directives = []

        # files = self.ide.get_files_to_explore()

        for file_ in self.files:

            filename = file_["filename"]
            content = file_["content"]

            content = remove_comments(content)
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
    def get_functions(self):

        regex_function = "[\s]*(unsigned|signed|long|PUBLIC)*[\s]*(" + "|".join(data_types) + ")[\s]*(\*?)[\s]*([*\w]+)[\s]*\(([\w ,*.\[\]]*)\)[\s]*"
        # regex_function = "[\s]*(unsigned|signed|long|PUBLIC)*[\s]*(" + "|".join(data_types) + ")[\s]*(\*?)[\s]*([*\w]+)[\s]*\(([\s\w ,*.\[\]]*)\)[\s]*"
        #regex_function_content = "[\s]*{}[\s]*\{[\s\S]*\}[\s]*

        functions = []
        # files = self.ide.get_files_to_explore()
        ignore_spaces = bool(len(self.files) > 1)

        for file_ in self.files:

            filename = file_["filename"]
            content = file_["content"]


            # content_comments = content

            content_no_comments = remove_comments(content, ignore_spaces=ignore_spaces)
            content_list = remove_comments(content).split("\n")

            for line in range(len(content_list)):

                match = re.match(regex_function, content_list[line])
                if match:
                    this_function = {}

                    # content_no_comments = editor.text_edit.toPlainText()
                    index =  content_no_comments.find(content_list[line])
                    index_start = index
                    index_end = index
                    bracket = 1

                    try:
                        while  content_no_comments[index_end] != "{": index_end += 1
                    except IndexError:
                        bracket = 0

                    while bracket != 0:
                        index_end += 1
                        try:
                            if  content_no_comments[index_end] == "{": bracket += 1
                            elif  content_no_comments[index_end] == "}": bracket -= 1
                        except IndexError:
                            index_end = index_start - 1
                            break

                    count =  content_no_comments[index_start:index_end+1].count("\n")

                    # this_function["content"] = content_comments[index_start:index_end+1]
                    # this_function["line"] = (str(line + 1), str(line + 1 + count))
                    this_function["line"] = line + 1
                    this_function["name"] = match.groups()[2] + match.groups()[3]
                    this_function["return"] = ("" if not match.groups()[2] else "pointer to ") + ((match.groups()[0] + " " + match.groups()[1]) if match.groups()[0] else match.groups()[1])
                    this_function["return_true"] = ((match.groups()[0] + " " + match.groups()[1]) if match.groups()[0] else match.groups()[1])
                    this_function["return_true"] = this_function["return_true"].replace("PUBLIC ", "")
                    this_function["args"] = match.groups()[4]
                    this_function["filename"] = filename
                    functions.append(this_function)

        return functions
