#!/usr/bin/env python
#-*- coding: utf-8 -*-

from PySide import QtGui
#from .constants import a

########################################################################
class BackgroundPallete(object):
    

    #----------------------------------------------------------------------
    def __init__(self):
        """Constructor"""
        
        #self.PinguinoColor = (175, 200, 225)
        self.PinguinoColor = "#afc8e1"
        
    
    #----------------------------------------------------------------------
    def set_background_pinguino(self, widget):
        
        palette = QtGui.QPalette(widget.palette())
        widget.setAutoFillBackground(True)
        palette.setColor(QtGui.QPalette.Window, QtGui.QColor(self.PinguinoColor))
        widget.setPalette(palette)
        
        
    ##----------------------------------------------------------------------
    #def set_active_background(self, widget):
        #
        #palette = QtGui.QPalette(widget.palette())
        #widget.setAutoFillBackground(True)
        #palette.setColor(QtGui.QPalette.Active, QtGui.QColor(self.PinguinoColor))
        #widget.setPalette(palette)
        
        
        
    #----------------------------------------------------------------------
    def save_palette(self, widget):
        
        self.default_palette = QtGui.QPalette(widget.palette())
        
        
    #----------------------------------------------------------------------
    def get_default_palette(self):
        
        return self.default_palette
    
    
    #----------------------------------------------------------------------
    def set_default_palette(self, widget):
        
        widget.setAutoFillBackground(True)
        widget.setPalette(self.get_default_palette())

            
    #----------------------------------------------------------------------
    def set_palette(self, widget, palette):
        
        widget.setAutoFillBackground(True)
        widget.setPalette(palette)
            
    