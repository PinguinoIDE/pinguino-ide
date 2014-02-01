#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
from ConfigParser import RawConfigParser
import logging


########################################################################
class Librarymanager(object):
    
    #----------------------------------------------------------------------
    def __init__(self):
        self.libraries = self.get_libraries()
        
    
    #----------------------------------------------------------------------
    def get_libraries(self):
        path = os.path.join(os.environ.get("PINGUINO_USER_PATH"), "user_libraries")
        if not os.path.exists(path): return []
        dirs = os.listdir(path)
        
        libraries = []
        for dir_ in dirs:
            if os.path.isdir(os.path.join(path, dir_)):
                dict_ = self.parser_to_dict(os.path.join(path, dir_, "config"))
                
                if dict_.get("active", "False") == "False": continue
                
                dict_["pdl"] = os.path.join(path, dir_, "lib", "pdl")
                
                if os.path.isdir(os.path.join(path, dir_, "lib", "p8")):
                    dict_["p8"] = os.path.join(path, dir_, "lib", "p8")
                    
                if os.path.isdir(os.path.join(path, dir_, "lib", "p32")):
                    dict_["p32"] = os.path.join(path, dir_, "lib", "p32")
                    
                libraries.append(dict_)
        
        return libraries
            
            
    #----------------------------------------------------------------------
    def get_p8_libraries(self):
        return filter(lambda lib:lib.get("p8", False), self.libraries)
    
    
    #----------------------------------------------------------------------
    def get_p32_libraries(self):
        return filter(lambda lib:lib.get("p32", False), self.libraries)
    
    #----------------------------------------------------------------------
    def get_pdls(self):
        #_list_pdls = map(lambda lib:map(lambda pdl_file:os.path.join(lib["pdl"], pdl_file) , os.listdir(lib["pdl"])), self.libraries)
        
        list_pdls = []
        for lib in self.libraries:
            if os.path.exists(lib["pdl"]):
                list_pdls.extend(map(lambda pdl_file:os.path.join(lib["pdl"], pdl_file) , os.listdir(lib["pdl"])))
            else:
                logging.warning("Missing: "+lib["pdl"])
        
        #pdl = []
        #for list_pdl in list_pdls:
            #pdl.extend(list_pdl)
        return list_pdls
    
    #----------------------------------------------------------------------
    def parser_to_dict(self, filename):
        parser = RawConfigParser()
        parser.readfp(open(filename, "r"))
        dict_ = {}
        options = parser.options("LIB")
        for option in options: dict_[option] = parser.get("LIB", option)
        return dict_