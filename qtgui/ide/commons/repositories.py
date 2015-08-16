#! /usr/bin/python2
#-*- coding: utf-8 -*-

import os
import logging

INFO = "{} is an optional dependence for Pinguino's Library Manager."

ErrorModules = {"gitpython": True,
                # "pysvn": True,
                "hgapi": True,
                }

try: import git
except ImportError:
    ErrorModules["gitpython"] = False
    logging.warning(INFO.format("gitpython"))

# try: import pysvn
# except ImportError:
    # ErrorModules["pysvn"] = False
    # logging.warning(INFO%"pysvn")

try: import hgapi
except ImportError:
    ErrorModules["hgapi"] = False
    logging.warning(INFO.format("hgapi"))


########################################################################
class Repository(object):

    #----------------------------------------------------------------------
    def __init__(self, repo_path, dir_path, repo_type):
        """"""
        self.repo_path = repo_path
        self.dir_path = dir_path

        self.REPOSITORIES = [
                             {"name": "git",
                              "file": ".git",
                              "cv": GitRepo},

                             {"name": "hg",
                              "file": ".hg",
                              "cv": HgRepo},

                             # {"name": "svn",
                              # "file": ".svn",
                              # "cv": SvnRepo},
                            ]

        self.set_repo(repo_type)


    #----------------------------------------------------------------------
    def search_repo(self):
        listdir = os.listdir(self.dir_path)

        for repo in self.REPOSITORIES:
            if repo["file"] in listdir:
                Repository = repo["cv"]()
        self.clone = Repository.clone
        self.update = Repository.update

    #----------------------------------------------------------------------
    def set_repo(self, repo_type):
        for repo in self.REPOSITORIES:
            if repo_type == repo["name"]:
                Repository = repo["cv"]()

        self.clone = Repository.clone
        self.update = Repository.update

    #----------------------------------------------------------------------
    def install_library(self):
        # try:
        self.clone(self.repo_path, self.dir_path)
        return True
        # except: return False

    #----------------------------------------------------------------------
    def update_library(self):
        try:
            self.search_repo()
            self.update(self.dir_path)
            return True
        except:
            return False



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
        head = repo.heads[0]
        head.checkout(force=True)
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


# Since Google code down, this feature is not usefull
# ########################################################################
# class SvnRepo(object):

    # #----------------------------------------------------------------------
    # def clone(self, url, path):
        # client = pysvn.Client()
        # client.checkout(url, path)

    # #----------------------------------------------------------------------
    # def update(self, path):
        # client = pysvn.Client()
        # client.update(path)
