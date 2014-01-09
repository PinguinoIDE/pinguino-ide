#!/usr/bin/env python
#-*- coding: utf-8 -*-

import shutil
import os
from ConfigParser import RawConfigParser

from PySide.QtCore import QDir

CONFIG_FILENAME = "pinguino.conf"

########################################################################
class ConfigFirstTimePinguino(object):
    """"""

    #----------------------------------------------------------------------
    def __init__(self):
        """Constructor"""
        self.first_config = RawConfigParser()
        self.init_filenames()
        getattr(self, "config_"+os.name)() 
        
    #----------------------------------------------------------------------
    def init_filenames(self):
        """Initialize filename for firts pinguino.conf"""
        filenames = {
            "pinguino_stdout": "stdout",     
            "pinguino_ide_stdout": "pinguino.out", 
            "pinguino_ide_stderr": "pinguino.err", 
            "pinguino_ide_configlibs": "config_libs.conf", 
            "pinguino_ide_reserved": "reserved.pickle", 
            "pinguino_ide_wiki": "wiki.pickle",
            }
        
        self.first_config.add_section("Filenames") 
        for file_ in filenames.keys():
            self.first_config.set("Filenames", file_, filenames[file_])      
        
        
    #----------------------------------------------------------------------
    def config_posix(self):
        """"""
        # Path for all pinguinowriteable files
        PATH = QDir.home().path() + "/.local/share/pinguino_ide"
        
        writeable_path = PATH
        pinguino_user_libs = PATH + "/pinguino-libraries"
        pinguino_resources = "qtgui/resources"
        pinguino_examples = PATH + "/examples/examples"
        pinguino_examples_graphical = PATH + "/examples/examples_graphical"
        
        # create (if not exist) a work dir space
        if not os.path.isdir(writeable_path):
            os.makedirs(writeable_path, 0777)
            
        self.first_config.add_section("Paths")
        self.first_config.set("Paths", "pinguino_writeable_path", writeable_path)
        self.first_config.set("Paths", "pinguino_user_libs", pinguino_user_libs)
        self.first_config.set("Paths", "pinguino_resources", pinguino_resources)
        self.first_config.set("Paths", "pinguino_examples_graphical", pinguino_examples_graphical)
        self.first_config.set("Paths", "pinguino_examples", pinguino_examples)
        
        # set default paths
        self.first_config.set("Paths", "sdcc_bin", "/usr/bin/sdcc")
        #self.first_config.set("Paths", "sdcc_libs", "/usr/share/sdcc")
        self.first_config.set("Paths", "gcc_bin", "")
        #self.first_config.set("Paths", "gcc_libs", "")
        self.first_config.set("Paths", "pinguino_8_libs", "pinguino/p8")
        self.first_config.set("Paths", "pinguino_32_libs", "pinguino/p32")   
        self.first_config.set("Paths", "source", PATH + "/source")
        
        config_location = os.path.join(writeable_path, CONFIG_FILENAME)
        
        self.first_config.write(file(config_location, "w"))
        
        local_config = file("config", "w")
        local_config.write(config_location)
        
        # Remove examples
        if os.path.exists(pinguino_examples): shutil.rmtree(pinguino_examples)
        if os.path.exists(pinguino_examples_graphical): shutil.rmtree(pinguino_examples_graphical)
        
        # Copy examples to writeable path
        shutil.copytree("pinguino/examples", pinguino_examples)
        shutil.copytree("qtgui/gide/examples", pinguino_examples_graphical)
        
        # Copy other configs
        shutil.copy("qtgui/config/reserved.pickle", PATH + "/reserved.pickle")
        shutil.copy("qtgui/config/wikidocs.pickle", PATH + "/wikidocs.pickle")
        
        # Copy pinguino source dir
        if os.path.exists(PATH + "/source"): shutil.rmtree(PATH + "/source")
        shutil.copytree("pinguino/source", PATH + "/source")
        
        
        
if __name__ == "__main__":
    ConfigFirstTimePinguino()
    