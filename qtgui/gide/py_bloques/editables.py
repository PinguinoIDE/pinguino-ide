#!/usr/bin/env python
#-*- coding: utf-8 -*-

from . import constructor as Block
from .constructor import Kit

Kit = Kit()

########################################################################
class Editables(object):

    #----------------------------------------------------------------------
    def hack_linear(self):
        st = Block.Linear("Editable")
        st.addEdit("edit...", None, (255, 255, 255))
        st.addContextHelp("Any Communicationand here.")
        return st.getBlock()
    
    #----------------------------------------------------------------------
    def hack_function(self):
        st = Block.Function("Editable")
        st.addEdit("edit...", None, (255, 255, 255))
        st.addContextHelp("Any Communicationand here.")
        return st.getBlock()
    
    #----------------------------------------------------------------------
    def hack_nested(self):
        st = Block.Nested("Editable")
        st.addEdit("edit...", None, (255, 255, 255))
        st.addContextHelp("Any Communicationand here.")
        return st.getBlock()
    
    #----------------------------------------------------------------------
    def hack_nestednext(self):
        st = Block.NestedNext("Editable")
        st.addEdit("edit...", None, (255, 255, 255))
        st.addContextHelp("Any Communicationand here.")
        return st.getBlock()
    
    #----------------------------------------------------------------------
    def hack_output(self):
        st = Block.Output("Editable")
        st.addEdit("edit...", None, (255, 255, 255))
        st.addContextHelp("Any Communicationand here.")
        return st.getBlock()
    
    #----------------------------------------------------------------------
    def hack_output_b(self):
        st = Block.OutputBool("Editable")
        st.addEdit("edit...", None, (255, 255, 255))
        st.addContextHelp("Any Communicationand here.")
        return st.getBlock()

        
