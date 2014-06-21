#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import re
import pickle

#from . import constants as Constants

P32_DIR = os.path.join(Constants.IDE_HOME_DIR, "pinguino", "p32")
P8_DIR = os.path.join(Constants.IDE_HOME_DIR, "pinguino", "p8")

########################################################################
class Namespaces(object):
    
    #----------------------------------------------------------------------
    def get_pinguino_namespaces(self, arch):
        
        namespaces = []
        
        if arch == "8":
            libext = ".pdl"
            libdir = P8_DIR
        elif arch == "32":
            libext = ".pdl32"
            libdir = P32_DIR
            
            
        all_libdir_pdls = filter(lambda name:name.endswith(libext), os.listdir(os.path.join(libdir, 'pdl')))
        for fichier in all_libdir_pdls:
            
            # check content of the PDL file
            lib_file = open(os.path.join(libdir, "pdl", fichier), "r")
            lines = lib_file.readlines()
            lib_file.close()
            
            regex_pdl = "[\s]*([.\w]*)[\s]*([\w]*)[\s]*(#.*)*(#.*)*[\s]*"
            
            for line in lines:
                line = line[:line.find('//')] 
                if line.isspace() or not line: continue
                
                reg = re.match(regex_pdl, line)
                instruction, cnvinstruction, include, define = reg.groups()
                
                if not instruction: continue
                namespaces.append(instruction)
               
        
        return namespaces
    

    #----------------------------------------------------------------------
    def save_namespaces(self):
        
        name_spaces_8 = self.get_pinguino_namespaces("8")
        name_spaces_32 = self.get_pinguino_namespaces("32")
        
        name_spaces_commun = []
        
        copy_32 = name_spaces_32[:]        
        for name in name_spaces_8:
            if name in copy_32:
                name_spaces_8.remove(name)
                name_spaces_32.remove(name)
                name_spaces_commun.append(name)
        
        namespaces = {"arch8": name_spaces_8, "arch32": name_spaces_32, "all": name_spaces_commun,}   
        #pickle.dump(namespaces, open(Constants.IDE_NAMESPACES_FILE, "w"))
    
