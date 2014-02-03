#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import codecs

from PySide import QtGui

from ..methods.decorators import Decorator
from ..tools.code_navigator import CodeNavigator
from ..tools.files import Files
from ..methods.dialogs import Dialogs
from ..code_editor.autocomplete_icons import CompleteIcons

########################################################################
class TimerMethods(object):
    

    #----------------------------------------------------------------------
    @Decorator.timer(1000)
    @Decorator.requiere_main_focus()
    @Decorator.requiere_text_mode()
    @Decorator.requiere_open_files()
    @Decorator.requiere_browser_tab("Functions")
    @Decorator.requiere_tools_tab("Source Browser")
    def update_functions(self):
        
        #print("Source Browser: Functions")
        editor = self.main.tabWidget_files.currentWidget()
        functions_parse = CodeNavigator.get_functions(editor)
        
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
            
            self.main.tableWidget_functions.item(index, 0).setText(funtion["return"])
            self.main.tableWidget_functions.item(index, 1).setText(funtion["args"])
            self.main.tableWidget_functions.item(index, 2).setText("-".join(funtion["line"]))
            
            
            index += 1
        
        
    #----------------------------------------------------------------------
    @Decorator.timer(1000)
    @Decorator.requiere_main_focus()
    @Decorator.requiere_text_mode()
    @Decorator.requiere_open_files()
    @Decorator.requiere_browser_tab("Directives")
    @Decorator.requiere_tools_tab("Source Browser")
    def update_directives(self):
        
        #print("Source Browser: Directives.")
        editor = self.main.tabWidget_files.currentWidget()
        directives_parse = CodeNavigator.get_directives(editor)
        
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
            
            self.main.tableWidget_directives.item(index, 0).setText(directive["type"])
            self.main.tableWidget_directives.item(index, 1).setText(directive["value"])
            self.main.tableWidget_directives.item(index, 2).setText(directive["line"])
            index += 1
            

        
    #----------------------------------------------------------------------
    @Decorator.timer(1000)
    @Decorator.requiere_main_focus()
    @Decorator.requiere_text_mode()
    @Decorator.requiere_open_files()
    @Decorator.requiere_browser_tab("Variables")
    @Decorator.requiere_tools_tab("Source Browser")
    def update_variables(self):
        
        #print("Source Browser: Variables.")
        editor = self.main.tabWidget_files.currentWidget()
        variables_parse = CodeNavigator.get_variables(editor)
        
        
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
            #self.main.tableWidget_variables.setItem(index, 2, QtGui.QTableWidgetItem())
            
            self.main.tableWidget_variables.item(index, 0).setText(variable["type"])
            #self.main.tableWidget_variables.item(index, 1).setText(variable["value"])
            self.main.tableWidget_variables.item(index, 1).setText(variable["line"])
            index += 1
            
            
    #----------------------------------------------------------------------
    @Decorator.timer(1000)
    @Decorator.requiere_main_focus()
    @Decorator.requiere_text_mode()
    @Decorator.requiere_open_files()
    @Decorator.if_autocomplete_is_enable()
    def update_autocompleter(self):
        
        editor = self.main.tabWidget_files.currentWidget()
        
        if not getattr(self, "completer_funtions", False):
            self.completer_funtions = []
            functions_parse = CodeNavigator.get_functions(editor)
            for funtion in functions_parse:
                self.completer_funtions.append(funtion["name"])
                
        if not getattr(self, "completer_variables", False):
            self.completer_variables = []
            variables_parse = CodeNavigator.get_variables(editor)
            for variable in variables_parse:
                self.completer_variables.append([variable["name"], variable["type"]])
                
        if not getattr(self, "completer_directives", False):
            self.completer_directives = []
            directives_parse = CodeNavigator.get_directives(editor)
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
    @Decorator.timer(1000)
    @Decorator.requiere_main_focus()
    @Decorator.requiere_text_mode()
    @Decorator.requiere_open_files()
    def check_external_changes(self):
        editor = self.main.tabWidget_files.currentWidget()
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
            
            if reload_: self.save_file()
            else: self.reload_file()
                