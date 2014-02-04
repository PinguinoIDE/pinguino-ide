#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import shutil
import logging

INFO = "%s is an optional dependence for Pinguino's Library Manager."

try: import git
except: logging.warning(INFO%"gitpython")

try: import pysvn
except: logging.warning(INFO%"pysvn")

try: from mercurial import ui, hg
except: logging.warning(INFO%"mercurial")

from ..methods.config_libs import ConfigLibsGroup



########################################################################
class PinguinoLibrary(object):
 
    
    #----------------------------------------------------------------------
    def __init__(self, lib, source):
        self.config = ConfigLibsGroup()   
        self.ide_library_installed = os.path.join(os.environ.get("PINGUINO_USER_PATH"), "user_libraries")
        
        self.name = lib
        self.url = self.config.get_all_sources()[lib]["repository"]
        self.path = os.path.join(self.ide_library_installed, self.name, "lib")
        
        
        self.REPOSYTORIES = [
                             {"name": "git",
                              "file": ".git",
                              "cv": GitRepo},
                            
                             {"name": "hg",
                              "file": ".hg",
                              "cv": HgRepo},
                            
                             {"name": "svn",
                              "file": ".svn",
                              "cv": SvnRepo},
                            ]           
            
        
    #----------------------------------------------------------------------
    def search_repo(self):
        listdir = os.listdir(self.path)
        
        for repo in self.REPOSYTORIES:
            if repo["file"] in listdir:
                Repository = repo["cv"]()
        self.clone = Repository.clone
        self.update = Repository.update

    #----------------------------------------------------------------------
    def set_repo(self, repo_):
        for repo in self.REPOSYTORIES:
            if repo_ == repo["name"]:
                Repository = repo["cv"]()
            
        self.clone = Repository.clone
        self.update = Repository.update
        
    #----------------------------------------------------------------------
    def install_library(self, repo):
        self.set_repo(repo)
        self.clone(self.url, self.path)
        return True
            
    #----------------------------------------------------------------------
    def update_library(self):
        self.search_repo()
        self.update(self.path)
        return True

        
    
    
########################################################################
class GitRepo(object):

    #----------------------------------------------------------------------
    def clone(self, url, path):
        git.Repo.clone_from(url, path)
    
    #----------------------------------------------------------------------
    def update(self, path):
        if os.path.exists(os.path.join(path, ".git", "config.lock")): os.remove(os.path.join(path, ".git", "config.lock"))         
        
        repo = git.Repo(path)
        repo.config_writer()
        remo = repo.remote()
        remo.pull()

    
########################################################################
class HgRepo(object):
    
    #----------------------------------------------------------------------
    def clone(self, url, path):
        """"""
    
    #----------------------------------------------------------------------
    def update(self, path):
        """"""

    
    
########################################################################
class SvnRepo(object):

    #----------------------------------------------------------------------
    def clone(self, url, path):
        client = pysvn.Client()
        client.checkout(url, path)        
        
    #----------------------------------------------------------------------
    def update(self, path):
        client = pysvn.Client()
        client.update(path)