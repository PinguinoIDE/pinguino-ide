#!/usr/bin/env python
#-*- coding: utf-8 -*-

from ..events.events import PinguinoEvents
from ..widgets.output_widget import PinguinoTerminal
from ...gide.app.graphical import GraphicalIDE


########################################################################
class PinguinoFeatures(PinguinoEvents):
    """"""

    #----------------------------------------------------------------------
    def __initialize_features__(self):
        """"""
        self.PinguinoKIT = GraphicalIDE(self)
        self.__preconfig_kit__()
        
        self.main.actionAutocomplete.setChecked(self.configIDE.config("Features", "autocomplete", True))
        
        self.main.actionSwitch_ide.setChecked(self.configIDE.config("Features", "graphical", False))
        self.switch_ide_mode(self.configIDE.config("Features", "graphical", False))
        
        self.recent_files = self.configIDE.get_recents()
        self.update_recents_menu()
        
        #self.main.dockWidget_output.add
        self.main.plainTextEdit_output = PinguinoTerminal(self.main.dockWidgetContents_2)
        self.main.plainTextEdit_output.shell.statement_module.pinguino_main = self
        self.main.gridLayout_3.addWidget(self.main.plainTextEdit_output, 0, 0, 1, 1)
        
        
    #----------------------------------------------------------------------
    def __preconfig_kit__(self):
        """"""
        self.main.tabWidget_graphical.setVisible(False)
        self.main.dockWidget_blocks.setVisible(False)        
        
    #----------------------------------------------------------------------
    def switch_ide_mode(self, toggle):
        """"""
        self.main.actionSwitch_ide.setChecked(toggle)
        self.main.tabWidget_graphical.setVisible(toggle and self.main.tabWidget_graphical.count()>0)
        self.main.tabWidget_files.setVisible(not toggle and self.main.tabWidget_files.count()>0)
        
        self.main.dockWidget_blocks.setVisible(toggle)  
        self.main.dockWidget_tools.setVisible(not toggle)  
        self.main.toolBar_search_replace.setVisible(not toggle)
        self.main.toolBar_edit.setVisible(not toggle)
        self.main.toolBar_graphical.setVisible(toggle)
        self.main.toolBar_undo_redo.setVisible(not toggle)
        self.main.actionView_Pinguino_code.setEnabled(toggle)
                                                      
        self.main.dockWidget_output.setVisible(not toggle)
        
        
        self.configIDE.set("Features", "graphical", toggle)
        self.configIDE.save_config()         
        
        self.tab_changed()
        
    #----------------------------------------------------------------------
    def is_graphical(self):
        """"""
        return self.main.actionSwitch_ide.isChecked()
    
    #----------------------------------------------------------------------
    def is_widget(self):
        """"""
        tab = self.get_tab()
        editor = tab.currentWidget()
        if editor is None: return False
        return getattr(editor, "is_widget", False)
    
    #----------------------------------------------------------------------
    def is_autocomplete_enable(self):
        """"""
        return self.main.actionAutocomplete.isChecked()
    
    #----------------------------------------------------------------------
    def switch_autocomplete(self):
        """"""
        enable = self.main.actionAutocomplete.isChecked()
        self.configIDE.set("Features", "autocomplete", enable)
        self.configIDE.save_config()        
        