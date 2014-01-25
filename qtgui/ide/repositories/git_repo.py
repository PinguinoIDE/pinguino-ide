#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import shutil

import git

#from ..methods.constants import self.ide_library_installed
from ..methods.config_libs import ConfigLibsGroup

########################################################################
class GitRepo(object):
    

    #----------------------------------------------------------------------
    def __init__(self):
        
        self.config = ConfigLibsGroup()   
        self.ide_library_installed = os.path.join(os.environ.get("PINGUINO_USER_PATH"), "user_libraries")
        
        
    #----------------------------------------------------------------------
    def install_library(self, lib):
        name = lib
             
        lib = self.config.get_all_sources()[lib]
        repository = lib["repository"]
        
        git.Repo.clone_from(repository, os.path.join(self.ide_library_installed, name, "lib"))
            
            
    #----------------------------------------------------------------------
    def update_library(self, lib):
        name = lib       
        lib = self.config.get_all_sources()[lib]
        repository = lib["repository"]
        
        repo = git.Repo(os.path.join(self.ide_library_installed, name, "lib"))
        repo.config_writer()
        remo = repo.remote()
        remo.pull()
        
        
    #----------------------------------------------------------------------
    def check_library(self, lib):
        name = lib  
        lib = self.config.get_all_sources()[lib]
        
        if os.path.exists(os.path.join(self.ide_library_installed, name, "lib")):
            if not os.path.exists(os.path.join(self.ide_library_installed, name, "lib", ".git")):
                shutil.rmtree(os.path.join(self.ide_library_installed, name, "lib"))
                return False
            else:
                if os.path.exists(os.path.join(self.ide_library_installed, name, "lib", ".git", "config.lock")):
                    os.remove(os.path.join(self.ide_library_installed, name, "lib", ".git", "config.lock")) 
                return True
                
        return False
        
        
    
    