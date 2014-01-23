#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import shutil

import git

#from ..methods.constants import IDE_LIBRARY_INSTALLED
from ..methods.config_libs import ConfigLibsGroup

########################################################################
class GitRepo(object):
    

    ##----------------------------------------------------------------------
    #def __init__(self):
        #super(GitRepo, self).__init__()
        
        
        
    #----------------------------------------------------------------------
    @classmethod
    def install_library(self, lib):
        
        name = lib
        config = ConfigLibsGroup()        
        lib = config.get_all_sources()[lib]
        repository = lib["repository"]
        
        git.Repo.clone_from(repository, os.path.join(IDE_LIBRARY_INSTALLED, name, "lib"))
            
            
    #----------------------------------------------------------------------
    @classmethod
    def update_library(self, lib):
        
        name = lib
        config = ConfigLibsGroup()        
        lib = config.get_all_sources()[lib]
        repository = lib["repository"]
        
        repo = git.Repo(os.path.join(IDE_LIBRARY_INSTALLED, name, "lib"))
        repo.config_writer()
        remo = repo.remote()
        remo.pull()
        
        
    #----------------------------------------------------------------------
    @classmethod
    def check_library(self, lib):
        
        name = lib
        config = ConfigLibsGroup()       
        lib = config.get_all_sources()[lib]
        
        if os.path.exists(os.path.join(IDE_LIBRARY_INSTALLED, name, "lib")):
            if not os.path.exists(os.path.join(IDE_LIBRARY_INSTALLED, name, "lib", ".git")):
                shutil.rmtree(os.path.join(IDE_LIBRARY_INSTALLED, name, "lib"))
                return False
            else:
                if os.path.exists(os.path.join(IDE_LIBRARY_INSTALLED, name, "lib", ".git", "config.lock")):
                    os.remove(os.path.join(IDE_LIBRARY_INSTALLED, name, "lib", ".git", "config.lock")) 
                return True
                
        return False
        
        
    
    