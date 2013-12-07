#!/usr/bin/env python
#-*- coding: utf-8 -*-

from ..events.events import PinguinoEvents

from ...gide.app.graphical import GraphicalIDE

########################################################################
class PinguinoFeatures(PinguinoEvents):
    """"""

    #----------------------------------------------------------------------
    def __initialize_features__(self):
        """"""
        self.PinguinoKIT = GraphicalIDE(self)
        self.__preconfig_kit__()
        
    #----------------------------------------------------------------------
    def __preconfig_kit__(self):
        """"""
        self.main.tabWidget_graphical.setVisible(False)
        self.main.dockWidget_blocks.setVisible(False)        
        
    #----------------------------------------------------------------------
    def switch_ide_mode(self, toogle):
        """"""
        self.main.tabWidget_graphical.setVisible(toogle and self.main.tabWidget_graphical.count()>0)
        self.main.tabWidget_files.setVisible(not toogle and self.main.tabWidget_files.count()>0)
        
        self.main.dockWidget_blocks.setVisible(toogle)  
        self.main.dockWidget_tools.setVisible(not toogle)  
        self.main.toolBar_search_replace.setVisible(not toogle)
        self.main.toolBar_edit.setVisible(not toogle)
        
        self.main.toolBar_graphical.setVisible(toogle)
        
        self.tab_changed()
        
    #----------------------------------------------------------------------
    def is_graphical(self):
        """"""
        return self.main.actionSwitch_ide.isChecked()