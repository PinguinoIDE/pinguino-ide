#!/usr/bin/env python
#-*- coding: utf-8 -*-


from constructor import * 
kit = Kit()

########################################################################
class Interface():
    
    #----------------------------------------------------------------------
    def __init__(self):
        """constructor"""
    
    #----------------------------------------------------------------------
    def interfaz_function(self):
        pm = Function("Interface")
        pm.addEdit("function", None, (255, 255, 255))
        pm.addSyntax("()")
        pm.addButton("Run", False, False)
        return pm.getBlock()
        
    #----------------------------------------------------------------------    
    def frame_plot(self):
        pm = FramePlot("Tools")
        pm.bloque.append(["edit", "plot_0"])
        pm.bloque.append(["resize", 350, 200])
        return pm.getBlock()
    
    #----------------------------------------------------------------------    
    def frame_debug(self):
        pm = FrameDebug("Tools")
        pm.bloque.append(["label", "Output:"])
        pm.bloque.append(["resize", 350, 200])
        return pm.getBlock()
    
    #----------------------------------------------------------------------
    def print_(self):
        pm = Linear("Interface")
        pm.addDecorator("print")
        pm.addSyntax("__print_output___(")
        pm.addSpace()
        pm.addSyntax(")")
        pm.bloque.append(["resize", 350, 200])
        pm.fillWith(["edit"])
        return pm.getBlock()


#['call-function-plot_sin_X', [344, 77],
 
 #[['syntax', '__update_plot__'],
  #['label', 'plot_sin_X'],
  #['syntax', '('],
  #['syntax', '9, '],
  #['space'],
  #['syntax', ')']],
 
 #'linear', '10', ['12'], ['3'], ['1', '1', '1'], []]
