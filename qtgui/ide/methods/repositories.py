#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
#import shutil
import logging

INFO = "%s is an optional dependence for Pinguino's Library Manager."

ErrorModules = {"gitpython": True, "pysvn": True, "hgapi": True,}

try: import git
except ImportError:
    ErrorModules["gitpython"] = False
    logging.warning(INFO%"gitpython")

try: import pysvn
except ImportError:
    ErrorModules["pysvn"] = False
    logging.warning(INFO%"pysvn")

try: import hgapi
except ImportError:
    ErrorModules["hgapi"] = False
    logging.warning(INFO%"hgapi")

from ..methods.config_libs import ConfigLibsGroup



########################################################################
class PinguinoLibrary(object):


    #----------------------------------------------------------------------
    def __init__(self, lib):
        self.config = ConfigLibsGroup()
        self.ide_library_installed = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), "libraries")

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
        try:
            self.set_repo(repo)
            self.clone(self.url, self.path)
            return True
        except: return False

    #----------------------------------------------------------------------
    def update_library(self):
        try:
            self.search_repo()
            self.update(self.path)
            return True
        except: return False



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
        hgapi.hg_clone(url, path)

    #----------------------------------------------------------------------
    def update(self, path):
        repo = hgapi.Repo(path)
        repo.hg_pull()
        repo.hg_update("default", clean=True)


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
