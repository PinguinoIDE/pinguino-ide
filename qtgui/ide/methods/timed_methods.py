#! /usr/bin/python2
#-*- coding: utf-8 -*-

import os
import codecs

from PySide import QtGui

from ..methods.decorators import Decorator
# from ..tools.code_navigator import self
from ..methods.dialogs import Dialogs

########################################################################
class TimedMethods(object):


    #----------------------------------------------------------------------
    @Decorator.timer(1000)
    @Decorator.requiere_open_files()
    # @Decorator.requiere_text_mode()
    @Decorator.requiere_browser_tab("Functions")
    @Decorator.requiere_tools_tab("SourceBrowser")
    @Decorator.requiere_main_focus()
    def update_functions(self):

        functions_parse = self.get_functions()

        index = 0
        self.main.tableWidget_functions.setRowCount(len(functions_parse))
        self.completer_funtions = []
        for funtion in functions_parse:
            item = QtGui.QTableWidgetItem()
            self.main.tableWidget_functions.setVerticalHeaderItem(index, item)
            item.setText(funtion["name"])
            self.completer_funtions.append(funtion["name"])

            self.main.tableWidget_functions.setItem(index, 0, QtGui.QTableWidgetItem())
            self.main.tableWidget_functions.setItem(index, 1, QtGui.QTableWidgetItem())
            self.main.tableWidget_functions.setItem(index, 2, QtGui.QTableWidgetItem())

            if funtion["filename"]:
                self.main.tableWidget_functions.item(index, 0).setText(os.path.split(funtion["filename"])[1])
            else:
                self.main.tableWidget_functions.item(index, 0).setText(self.get_current_filename())


            self.main.tableWidget_functions.item(index, 1).setText(funtion["return"])
            self.main.tableWidget_functions.item(index, 2).setText(funtion["args"])

            setattr(item, "filename", funtion["filename"])
            setattr(item, "line", funtion["line"])

            index += 1


    #----------------------------------------------------------------------
    @Decorator.timer(1000)
    @Decorator.requiere_open_files()
    # @Decorator.requiere_text_mode()
    @Decorator.requiere_browser_tab("Directives")
    @Decorator.requiere_tools_tab("SourceBrowser")
    @Decorator.requiere_main_focus()
    def update_directives(self):

        directives_parse = self.get_directives()

        index = 0
        self.main.tableWidget_directives.setRowCount(len(directives_parse))
        self.completer_directives = []
        for directive in directives_parse:
            item = QtGui.QTableWidgetItem()
            self.main.tableWidget_directives.setVerticalHeaderItem(index, item)
            item.setText(directive["name"])
            self.completer_directives.append(directive["name"])

            self.main.tableWidget_directives.setItem(index, 0, QtGui.QTableWidgetItem())
            self.main.tableWidget_directives.setItem(index, 1, QtGui.QTableWidgetItem())
            self.main.tableWidget_directives.setItem(index, 2, QtGui.QTableWidgetItem())

            if directive["filename"]:
                self.main.tableWidget_directives.item(index, 0).setText(os.path.split(directive["filename"])[1])
            else:
                self.main.tableWidget_directives.item(index, 0).setText(self.get_current_filename())

            self.main.tableWidget_directives.item(index, 1).setText(directive["type"])
            self.main.tableWidget_directives.item(index, 2).setText(directive["value"])

            setattr(item, "filename", directive["filename"])
            setattr(item, "line", directive["line"])
            index += 1



    #----------------------------------------------------------------------
    @Decorator.timer(1000)
    @Decorator.requiere_open_files()
    # @Decorator.requiere_text_mode()
    @Decorator.requiere_browser_tab("Variables")
    @Decorator.requiere_tools_tab("SourceBrowser")
    @Decorator.requiere_main_focus()
    def update_variables(self):

        variables_parse = self.get_variables()

        index = 0
        self.main.tableWidget_variables.setRowCount(len(variables_parse))
        self.completer_variables = []
        for variable in variables_parse:
            item = QtGui.QTableWidgetItem()
            self.main.tableWidget_variables.setVerticalHeaderItem(index, item)
            item.setText(variable["name"])
            self.completer_variables.append([variable["name"], variable["type"]])

            self.main.tableWidget_variables.setItem(index, 0, QtGui.QTableWidgetItem())
            self.main.tableWidget_variables.setItem(index, 1, QtGui.QTableWidgetItem())

            if variable["filename"]:
                self.main.tableWidget_variables.item(index, 0).setText(os.path.split(variable["filename"])[1])
            else:
                self.main.tableWidget_variables.item(index, 0).setText(self.get_current_filename())

            self.main.tableWidget_variables.item(index, 1).setText(variable["type"])

            setattr(item, "filename", variable["filename"])
            setattr(item, "line", variable["line"])
            index += 1


    #----------------------------------------------------------------------
    @Decorator.timer(5000)
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    @Decorator.if_autocomplete_is_enable()
    @Decorator.requiere_main_focus()
    def update_autocompleter(self):

        editor = self.get_current_editor()

        if not hasattr(editor.text_edit, "completer"):
            return

        if not getattr(self, "completer_funtions", False):
            self.completer_funtions = []
            functions_parse = self.get_functions()
            for funtion in functions_parse:
                self.completer_funtions.append(funtion["name"])

        if not getattr(self, "completer_variables", False):
            self.completer_variables = []
            variables_parse = self.get_variables()
            for variable in variables_parse:
                self.completer_variables.append([variable["name"], variable["type"]])

        if not getattr(self, "completer_directives", False):
            self.completer_directives = []
            directives_parse = self.get_directives()
            for directive in directives_parse:
                self.completer_directives.append(directive["name"])

        editor.text_edit.completer.removeTemporalItems("directives")
        editor.text_edit.completer.removeTemporalItems("variables")
        editor.text_edit.completer.removeTemporalItems("functions")

        for item in self.completer_directives:
            editor.text_edit.completer.addTemporalItem("directives", item, self.ICONS.iconDirectives)

        for item in self.completer_funtions:
            editor.text_edit.completer.addTemporalItem("functions", item, self.ICONS.iconFunction)

        for item, type_ in self.completer_variables:
            type_ = type_.split()[-1]
            if type_ in "int float bool byte bool double struct union".split():
                icon = getattr(self.ICONS, "icon"+type_.capitalize())
            else:
                icon = None

            editor.text_edit.completer.addTemporalItem("variables", item, icon)

        self.completer_funtions = []
        self.completer_directives = []
        self.completer_variables = []


    #----------------------------------------------------------------------
    @Decorator.timer(3000)
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    @Decorator.requiere_main_focus()
    def check_external_changes(self):

        editor = self.get_current_editor()
        filename = getattr(editor, "path", None)
        if not filename: return
        if os.path.exists(filename):
            file_ = codecs.open(filename, "r", "utf-8")
            content_file = "".join(file_.readlines())
            file_.close()
            exist = True
        else:
            content_file = ""
            exist = False

        last_saved = getattr(editor, "last_saved")

        if content_file != last_saved:
            reload_ = Dialogs.overwrite_file(self, filename, exist)

            if reload_: self.ide_save_file()
            else: self.reload_file()


    #----------------------------------------------------------------------
    @Decorator.timer(3000)
    @Decorator.requiere_open_files()
    # @Decorator.requiere_text_mode()
    @Decorator.requiere_main_focus()
    def save_backup_file(self):

        editor = self.get_current_editor()
        index = self.main.tabWidget_files.indexOf(editor)
        filename_tab = self.main.tabWidget_files.tabText(index)
        filename = getattr(editor, "path", None)
        if not filename: return
        content_saved = getattr(editor, "last_saved", None)

        if self.is_graphical() is True:

            content = self.PinguinoKIT.get_gpde()
            self.PinguinoKIT.save_raw_parser(content, editor.path+"~")

        elif self.is_graphical() is False:
            content = editor.text_edit.toPlainText()
            filename_backup = filename + "~"
            #if os.path.exists(filename) and filename_tab.endswith("*"):
            if os.path.exists(filename) and (content_saved != content):
                file_ = codecs.open(filename_backup, "w", "utf-8")
                file_.write(content)
                file_.close()

        elif content_saved == content and os.path.exists(filename_backup):
            os.remove(filename_backup)
