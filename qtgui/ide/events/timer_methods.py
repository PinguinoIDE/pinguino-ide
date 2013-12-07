#!/usr/bin/env python
#-*- coding: utf-8 -*-

from PySide import QtGui

from ..helpers.decorators import Decorator
from ..tools.code_navigator import CodeNavigator
from ..tools.files import Files

########################################################################
class TimerMethods(object):
    """"""

    #----------------------------------------------------------------------
    @Decorator.timer(1000)
    @Decorator.requiere_main_focus()
    @Decorator.requiere_open_files()
    @Decorator.requiere_browser_tab("Functions")
    @Decorator.requiere_tools_tab("Source Browser")
    def update_functions(self):
        """"""
        print("Source Browser: Functions")
        editor = self.main.tabWidget_files.currentWidget()
        functions_parse = CodeNavigator.get_functions(editor)
        
        index = 0
        self.main.tableWidget_functions.setRowCount(len(functions_parse))
        for funtion in functions_parse:
            item = QtGui.QTableWidgetItem()
            self.main.tableWidget_functions.setVerticalHeaderItem(index, item) 
            item.setText(funtion["name"])
            
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
    @Decorator.requiere_open_files()
    @Decorator.requiere_browser_tab("Directives")
    @Decorator.requiere_tools_tab("Source Browser")
    def update_directives(self):
        """"""
        print("Source Browser: Directives.")
        editor = self.main.tabWidget_files.currentWidget()
        directives_parse = CodeNavigator.get_directives(editor)
        
        index = 0
        self.main.tableWidget_directives.setRowCount(len(directives_parse))
        for directive in directives_parse:
            item = QtGui.QTableWidgetItem()
            self.main.tableWidget_directives.setVerticalHeaderItem(index, item)
            item.setText(directive["name"])
            
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
    @Decorator.requiere_open_files()
    @Decorator.requiere_browser_tab("Variables")
    @Decorator.requiere_tools_tab("Source Browser")
    def update_variables(self):
        """"""
        print("Source Browser: Variables.")
        editor = self.main.tabWidget_files.currentWidget()
        variables_parse = CodeNavigator.get_variables(editor)
        
        
        index = 0
        self.main.tableWidget_variables.setRowCount(len(variables_parse))
        for variable in variables_parse:
            item = QtGui.QTableWidgetItem()
            self.main.tableWidget_variables.setVerticalHeaderItem(index, item)
            item.setText(variable["name"])
            
            self.main.tableWidget_variables.setItem(index, 0, QtGui.QTableWidgetItem())
            self.main.tableWidget_variables.setItem(index, 1, QtGui.QTableWidgetItem())
            #self.main.tableWidget_variables.setItem(index, 2, QtGui.QTableWidgetItem())
            
            self.main.tableWidget_variables.item(index, 0).setText(variable["type"])
            #self.main.tableWidget_variables.item(index, 1).setText(variable["value"])
            self.main.tableWidget_variables.item(index, 1).setText(variable["line"])
            index += 1
            
