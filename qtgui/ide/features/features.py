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
        
        self.recent_files = self.configIDE.get_recents()
        self.update_recents_menu()
        self.open_last_files()
        
        self.main.actionAutocomplete.setChecked(self.configIDE.config("Features", "autocomplete", True))
        
        #self.main.actionSwitch_ide.setChecked(self.configIDE.config("Features", "graphical", False))
        #self.switch_ide_mode(self.configIDE.config("Features", "graphical", False))
        
        self.main.plainTextEdit_output = PinguinoTerminal(self.main.dockWidgetContents_2)
        #self.main.plainTextEdit_output.shell.statement_module.pinguino_main = self
        self.main.plainTextEdit_output.set_extra_args(**{"pinguino_main": self})
        self.main.gridLayout_3.addWidget(self.main.plainTextEdit_output, 0, 0, 1, 1)
        
        
    #----------------------------------------------------------------------
    def __preconfig_kit__(self):
        """"""
        self.main.tabWidget_graphical.setVisible(False)
        self.main.dockWidget_blocks.setVisible(False)        
        
    #----------------------------------------------------------------------
    def switch_ide_mode(self, graphical):
        """"""
        self.main.actionSwitch_ide.setChecked(graphical)
        self.main.tabWidget_graphical.setVisible(graphical and self.main.tabWidget_graphical.count()>0)
        self.main.tabWidget_files.setVisible(not graphical and self.main.tabWidget_files.count()>0)
        
        if graphical:
            self.update_actions_for_graphical()
        else:
            self.update_actions_for_text()
        
        
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
        
        
    #----------------------------------------------------------------------
    def update_actions_for_text(self):
        """"""
        normal = False
        
        self.main.actionView_Pinguino_code.setEnabled(normal)   
        self.main.actionComment_out_region.setEnabled(normal)   
        self.main.actionComment_Uncomment_region.setEnabled(normal) 
        self.main.actionRedo.setEnabled(not normal)         
        self.main.actionUndo.setEnabled(not normal)         
        self.main.actionCut.setEnabled(not normal)         
        self.main.actionCopy.setEnabled(not normal)         
        self.main.actionPaste.setEnabled(not normal)         
        self.main.actionSearch.setEnabled(not normal)        
        self.main.actionSearch_and_replace.setEnabled(not normal)   
        self.main.dockWidget_blocks.setVisible(normal)
        self.main.dockWidget_tools.setVisible(not normal)  
        self.main.toolBar_search_replace.setVisible(not normal)
        self.main.toolBar_edit.setVisible(not normal)
        self.main.toolBar_graphical.setVisible(normal)
        self.main.toolBar_undo_redo.setVisible(not normal)
        
        
        self.configIDE.set("Features", "terminal_on_graphical", self.main.dockWidget_output.isVisible())         
        self.main.dockWidget_output.setVisible(self.configIDE.config("Features", "terminal_on_text", True))
        self.configIDE.save_config()
        
        
    #----------------------------------------------------------------------
    def update_actions_for_graphical(self):
        """"""
        normal = True
        
        self.main.actionView_Pinguino_code.setEnabled(normal)   
        self.main.actionRedo.setEnabled(not normal)          
        self.main.actionComment_out_region.setEnabled(not normal)   
        self.main.actionComment_Uncomment_region.setEnabled(not normal)         
        self.main.actionUndo.setEnabled(not normal)         
        self.main.actionCut.setEnabled(not normal)         
        self.main.actionCopy.setEnabled(not normal)         
        self.main.actionPaste.setEnabled(not normal)         
        self.main.actionSearch.setEnabled(not normal)        
        self.main.actionSearch_and_replace.setEnabled(not normal)   
        self.main.dockWidget_blocks.setVisible(normal)
        self.main.dockWidget_tools.setVisible(not normal)  
        self.main.toolBar_search_replace.setVisible(not normal)
        self.main.toolBar_edit.setVisible(not normal)
        self.main.toolBar_graphical.setVisible(normal)
        self.main.toolBar_undo_redo.setVisible(not normal)       
        
        self.configIDE.set("Features", "terminal_on_text", self.main.dockWidget_output.isVisible())        
        self.main.dockWidget_output.setVisible(self.configIDE.config("Features", "terminal_on_graphical", False))
        self.configIDE.save_config()
        
        