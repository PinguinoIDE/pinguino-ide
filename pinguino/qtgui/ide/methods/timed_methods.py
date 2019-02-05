#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import codecs

from PySide2 import QtGui

from .core_threads import UpdateAutocompleter
from ..methods.decorators import Decorator
# from ..tools.code_navigator import self
from ..methods.dialogs import Dialogs

########################################################################
class TimedMethods(object):



    #----------------------------------------------------------------------
    @Decorator.timer(1500)
    @Decorator.requiere_open_files()
    @Decorator.requiere_browser_tab("Functions")
    @Decorator.requiere_tools_tab("SourceBrowser")
    @Decorator.requiere_main_focus()
    def timer_update_functions(self):

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
    @Decorator.timer(1500)
    @Decorator.requiere_open_files()
    @Decorator.requiere_browser_tab("Directives")
    @Decorator.requiere_tools_tab("SourceBrowser")
    @Decorator.requiere_main_focus()
    def timer_update_directives(self):
        """"""

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
    @Decorator.timer(1500)
    @Decorator.requiere_open_files()
    @Decorator.requiere_browser_tab("Variables")
    @Decorator.requiere_tools_tab("SourceBrowser")
    @Decorator.requiere_main_focus()
    def timer_update_variables(self):

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
    @Decorator.timer(3000)
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    @Decorator.requiere_main_focus()
    def timer_check_changes(self):

        editor = self.get_current_editor()
        filename = getattr(editor, "path", None)
        if not filename:
            return

        if os.path.exists(filename):
            file_ = codecs.open(filename, "r", encoding="utf-8")
            content_file = "".join(file_.readlines())
            file_.close()
            exist = True
        else:
            content_file = ""
            exist = False

        last_saved = getattr(editor, "last_saved")

        # if content_file != last_saved:
            # self.thread_variables()

        if self.get_current_filename().endswith("(r/o)"):
            if content_file != last_saved:
                self.editor_reload_file()
            self.ide_save_file()
            return

        if self.is_library():
            name = self.get_project_name()
            if self.get_current_filename().startswith(name) and (self.get_current_filename().endswith(".c")\
                                                                 or self.get_current_filename().endswith(".h")\
                                                                 or self.get_current_filename().endswith(".pdl")\
                                                                 or self.get_current_filename().endswith(".pdl32")):
                if content_file != last_saved:
                    self.editor_reload_file()
                self.ide_save_file()
                return




        if content_file != last_saved:
            reload_ = Dialogs.overwrite_file(self, filename, exist)

            if reload_:
                self.ide_save_file()
            else:
                self.editor_reload_file()


    #----------------------------------------------------------------------
    @Decorator.timer(3000)
    @Decorator.requiere_open_files()
    # @Decorator.requiere_text_mode()
    @Decorator.requiere_main_focus()
    def timer_backup_file(self):

        editor = self.get_current_editor()
        # index = self.main.tabWidget_files.indexOf(editor)
        # filename_tab = self.main.tabWidget_files.tabText(index)
        filename = getattr(editor, "path", None)
        if not filename:
            return
        content_saved = getattr(editor, "last_saved", None)

        if self.is_graphical() is True:
            content = self.PinguinoKIT.get_gpde()
            self.PinguinoKIT.save_raw_parser(content, editor.path+"~")

        elif self.is_graphical() is False:
            content = editor.text_edit.toPlainText()
            filename_backup = filename + "~"
            #if os.path.exists(filename) and filename_tab.endswith("*"):
            if os.path.exists(filename) and (content_saved != content):
                file_ = codecs.open(filename_backup, "w", encoding="utf-8")
                file_.write(content)
                file_.close()

        elif content_saved == content and os.path.exists(filename_backup):
            os.remove(filename_backup)


    #----------------------------------------------------------------------
    @Decorator.timer(1000)
    @Decorator.requiere_open_files()
    @Decorator.requiere_tools_tab("SourceBrowser")
    @Decorator.requiere_main_focus()
    def timer_update_assiatant(self):

        if not self.is_graphical() is None:
            editor = self.get_current_editor()
            if not hasattr(editor, "text_edit"): return
            tc = editor.text_edit.textCursor()
            tc.movePosition(tc.EndOfWord, tc.MoveAnchor)
            editor.text_edit.smart_under_selection(tc)
            selected = tc.selectedText()
            self.update_assistant(selected)


    #----------------------------------------------------------------------
    @Decorator.timer(3000)
    @Decorator.requiere_open_files()
    @Decorator.requiere_text_mode()
    @Decorator.if_autocomplete_is_enable()
    @Decorator.requiere_main_focus()
    def timer_update_autocompleter(self):
        """"""

        if not self.is_graphical() is False: return

        editor = self.get_current_editor()

        if (not hasattr(self, "thread_autocompleter")) and hasattr(editor.text_edit, "completer"):
            self.thread_autocompleter = UpdateAutocompleter()
            self.thread_autocompleter.signal_set_variables.connect(self.set_variables)
            self.thread_autocompleter.signal_set_directives.connect(self.set_directives)
            self.thread_autocompleter.signal_set_functions.connect(self.set_functions)
            self.thread_autocompleter.signal_add_autocompleter.connect(editor.text_edit.completer.addTemporalItem)
            self.thread_autocompleter.signal_rm_autocompleter.connect(editor.text_edit.completer.removeTemporalItems)

        if not hasattr(self, "thread_autocompleter"):
            return

        if not self.thread_autocompleter.isRunning():
            self.thread_autocompleter.set_files(self.get_files_to_explore())
            self.thread_autocompleter.setTerminationEnabled(True)
            self.thread_autocompleter.start()
        ##else:
            ##logging.debug("Working...")
            ##self.thread_autocompleter.terminate()
            ##self.thread_autocompleter.set_files(self.get_files_to_explore())
            ##self.thread_autocompleter.setTerminationEnabled(True)
            ##self.thread_autocompleter.start()

