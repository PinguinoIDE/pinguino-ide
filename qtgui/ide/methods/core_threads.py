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

# data_types = ["int", "float", "char", "double", "u8", "u16", "u32", "u64", "BOOL", "byte", "word", "void", "short", "long"]  # Yes I know, void is NOT a data type.
# preprocessor_commands = ["define", "include", "error", "undef", "if", "else", "if", "elif", "ifdef", "ifndef", "line", "pragma"]  #endif


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
        super(UpdateAutocompleter, self).__init__(parent)
        # QtCore.QThread.__init__(parent)
        # QtCore.QThread.__init__(self, parent)


    #----------------------------------------------------------------------
    def set_files(self, files):
        """"""
        # self.files = files
        self.parser = PinguinoParser(files)

    # #----------------------------------------------------------------------
    # def __del__(self):
        # self.wait()

    #----------------------------------------------------------------------
    def run(self):

        # if not getattr(self, "completer_funtions", False):
        self.completer_functions = []
        self.functions_args = {}
        functions_parse = self.parser.get_functions()
        self.signal_set_functions.emit(functions_parse)

        for funtion in functions_parse:
            self.completer_functions.append(funtion["name"])
            self.functions_args[funtion["name"]] = "{name}({{{{{args}}}}});".format(**funtion)

        # if not getattr(self, "completer_variables", False):
        self.completer_variables = []
        variables_parse = self.parser.get_variables()
        self.signal_set_variables.emit(variables_parse)

        for variable in variables_parse:
            self.completer_variables.append([variable["name"], variable["type"]])

        # if not getattr(self, "completer_directives", False):
        self.completer_directives = []
        directives_parse = self.parser.get_directives()
        self.signal_set_directives.emit(directives_parse)

        for directive in directives_parse:
            self.completer_directives.append(directive["name"])

        # editor.text_edit.completer.removeTemporalItems("directives")
        # editor.text_edit.completer.removeTemporalItems("variables")
        # editor.text_edit.completer.removeTemporalItems("functions")

        self.signal_rm_autocompleter.emit("directives")
        self.signal_rm_autocompleter.emit("variables")
        self.signal_rm_autocompleter.emit("functions")

        # editor.text_edit.completer.update_local_functions(self.functions_args)

        for item in self.completer_directives:
            # editor.text_edit.completer.addTemporalItem("directives", item, self.ICONS.iconDirectives)
            self.signal_add_autocompleter.emit("directives", item, str("iconDirectives"))

        for item in self.completer_functions:
            # editor.text_edit.completer.addTemporalItem("functions", item, self.ICONS.iconFunction)
            self.signal_add_autocompleter.emit("functions", item, str("iconFunction"))

        for item, type_ in self.completer_variables:
            type_ = type_.split()[-1]
            if type_ in "int float bool byte bool double struct union".split():
                icon = str("icon"+type_.capitalize())
            else:
                icon = str("")

            # editor.text_edit.completer.addTemporalItem("variables", item, icon)
            self.signal_add_autocompleter.emit("variables", item, icon)

        # self.terminate()



# ########################################################################
# class :
    # """"""

    # #----------------------------------------------------------------------
    # def get_variables(self):
        # """"""
        # regex_variables = "[\s]*(volatile|register|static|extern)*[\s]*(unsigned|signed)*[\s]*(" + "|".join(data_types) + ")+[\s]*( \* )*[\s]*([ \w\[\]=,.{}\"']*);"
        # # regex_variables = "[\s]*(volatile|register|static|extern)*[\s]*(unsigned|signed)*[\s]*(short|long)*[\s]*(" + "|".join(data_types) + ")[\s]*+([*])*([ \w\[\]=,.{}\"'\*]*);"

        # variables = []

        # # self.signal_set_variables.emit(variables)

        # # # if files is None:
        # # files = self.ide.get_files_to_explore()

        # for file_ in self.files:

            # filename = file_["filename"]
            # content = file_["content"]

            # content = remove_comments(content)
            # content = content.split("\n")
            # for line in range(len(content)):
                # match = re.match(regex_variables, content[line])
                # if match:
                    # args = match.groups()[4]
                    # l = []
                    # index = 0
                    # while index < len(args):
                        # if args[index] == "{":
                            # start = index
                            # bracket = 1

                            # while bracket != 0:
                                # index += 1
                                # if args[index] == "{": bracket += 1
                                # if args[index] == "}": bracket -= 1
                            # end = index

                            # l.append(args[start:end+1])

                        # else: index += 1

                    # for j in l: args = args.replace(j, "", 1)
                    # args = args.split(",")
                    # args = map(lambda a:a[:a.find("=")] if "=" in a else a, args)

                    # type_ = " ".join([("" if not match.groups()[3] else "pointer to "),
                                    # ("" if not match.groups()[0] else match.groups()[0]),
                                    # ("" if not match.groups()[1] else match.groups()[1]),
                                    # (match.groups()[2]),
                                    # # match.groups()[3]
                                    # ])

                    # while type_.startswith(" "): type_ = type_[1:]

                    # for arg in args:
                        # this_variable = {}
                        # this_variable["type"] = type_
                        # if re.match("(.*)(\[.*\])", arg):
                            # this_variable["name"] = re.match("(.*)(\[.*\])", arg).groups()[0]
                        # else:
                            # this_variable["name"] = arg
                        # #this_variable["value"] = match.groups()[6]
                        # this_variable["line"] = line + 1
                        # this_variable["filename"] = filename
                        # variables.append(this_variable)


        # return variables


    # #----------------------------------------------------------------------
    # def get_directives(self):

        # regex_directive = "[\s]*#(" + "|".join(preprocessor_commands)+ ")[\s]+<?[\s]*([\w.]*)[\s]*>?[\s]*([\S]*)"

        # directives = []

        # # files = self.ide.get_files_to_explore()

        # for file_ in self.files:

            # filename = file_["filename"]
            # content = file_["content"]

            # content = remove_comments(content)
            # content = content.split("\n")

            # for line in range(len(content)):
                # match = re.match(regex_directive, content[line])
                # this_directive = {}
                # if match:
                    # this_directive["type"] = "#" + match.groups()[0]
                    # this_directive["name"] = match.groups()[1]
                    # this_directive["value"] = match.groups()[2]
                    # this_directive["line"] = line + 1
                    # this_directive["filename"] = filename
                    # directives.append(this_directive)

        # return directives


    # #----------------------------------------------------------------------
    # def get_functions(self):

        # regex_function = "[\s]*(unsigned|signed|long|PUBLIC)*[\s]*(" + "|".join(data_types) + ")[\s]*(\*?)[\s]*([*\w]+)[\s]*\(([\w ,*.\[\]]*)\)[\s]*"
        # # regex_function = "[\s]*(unsigned|signed|long|PUBLIC)*[\s]*(" + "|".join(data_types) + ")[\s]*(\*?)[\s]*([*\w]+)[\s]*\(([\s\w ,*.\[\]]*)\)[\s]*"
        # #regex_function_content = "[\s]*{}[\s]*\{[\s\S]*\}[\s]*

        # functions = []
        # # files = self.ide.get_files_to_explore()
        # ignore_spaces = bool(len(self.files) > 1)

        # for file_ in self.files:

            # filename = file_["filename"]
            # content = file_["content"]


            # # content_comments = content

            # content_no_comments = remove_comments(content, ignore_spaces=ignore_spaces)
            # content_list = remove_comments(content).split("\n")

            # for line in range(len(content_list)):

                # match = re.match(regex_function, content_list[line])
                # if match:
                    # this_function = {}

                    # # content_no_comments = editor.text_edit.toPlainText()
                    # index =  content_no_comments.find(content_list[line])
                    # index_start = index
                    # index_end = index
                    # bracket = 1

                    # try:
                        # while  content_no_comments[index_end] != "{": index_end += 1
                    # except IndexError:
                        # bracket = 0

                    # while bracket != 0:
                        # index_end += 1
                        # try:
                            # if  content_no_comments[index_end] == "{": bracket += 1
                            # elif  content_no_comments[index_end] == "}": bracket -= 1
                        # except IndexError:
                            # index_end = index_start - 1
                            # break

                    # count =  content_no_comments[index_start:index_end+1].count("\n")

                    # # this_function["content"] = content_comments[index_start:index_end+1]
                    # # this_function["line"] = (str(line + 1), str(line + 1 + count))
                    # this_function["line"] = line + 1
                    # this_function["name"] = match.groups()[2] + match.groups()[3]
                    # this_function["return"] = ("" if not match.groups()[2] else "pointer to ") + ((match.groups()[0] + " " + match.groups()[1]) if match.groups()[0] else match.groups()[1])
                    # this_function["args"] = match.groups()[4]
                    # this_function["filename"] = filename
                    # functions.append(this_function)

        # return functions


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

            if version.startswith("v"):
                version = version[1:]

            if version:
                local_version = "{PINGUINO_VERSION}.{PINGUINO_SUBVERSION}".format(**os.environ)

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
            # logging.error(self, "Unable to connect to server. Please check your network connection and try again.")
            self.signal_message.emit(False, "ConnectionError")
            return
