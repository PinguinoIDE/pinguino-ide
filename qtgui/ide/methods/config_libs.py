#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import re
from ConfigParser import RawConfigParser

#from .constants import IDE_CONFIGLIBS_FILE, IDE_LIBRARY_INSTALLED


########################################################################
class ConfigLibsGroup(object):
    
    
    #----------------------------------------------------------------------
    def __init__(self):
        
        IDE_LIBRARY_INSTALLED = os.path.join(os.environ.get("PINGUINO_USER_PATH"), "user_libraries") 
        libs = os.listdir(IDE_LIBRARY_INSTALLED)
    
        self.all_libs = {}
        for lib in libs:
            config = os.path.join(IDE_LIBRARY_INSTALLED, lib, "config")
            self.all_libs[lib] = ConfigLibs(config)
            
    #----------------------------------------------------------------------
    def update_libs(self):
        
        self.__init__()
            
    #----------------------------------------------------------------------
    def new(self, name):
        
        return ConfigLibs(name)
            
    #----------------------------------------------------------------------
    def get_all_sources(self):
        
        self.update_libs()
        sources = {}
        for lib_key in self.all_libs.keys():
            sources[lib_key] = {"repository": self.all_libs[lib_key].config("LIB", "repository", ""),
                                "description": self.all_libs[lib_key].config("LIB", "description", ""),
                                "author": self.all_libs[lib_key].config("LIB", "author", ""),
                                "arch": self.all_libs[lib_key].config("LIB", "arch", ""),
                                "active": self.all_libs[lib_key].config("LIB", "active", False),
                                "installed": self.all_libs[lib_key].config("LIB", "installed", False)}
                
        return sources
    
    
                
    #----------------------------------------------------------------------
    def check_duplicated(self, source):
        
        #self.load_config()
        sources = self.get_all_sources()
        
        return source in sources.keys()
        
            
                
    
    
    #----------------------------------------------------------------------
    def save_config(self):
        
        for lib_key in self.all_libs.keys():
            #if os.path.isdir(os.path.join(IDE_CONFIGLIBS_FILE, lib_key)):
            self.all_libs[lib_key].save_config()
            #else:
                #self.all_libs.pop(lib_key)
            
    #----------------------------------------------------------------------
    def load_config(self):
        
        for lib_key in self.all_libs.keys():
            self.all_libs[lib_key].load_config()




########################################################################
class ConfigLibs(RawConfigParser, object):
    
    
    #----------------------------------------------------------------------
    def __init__(self, filename):
        super(ConfigLibs, self).__init__()
        
        self.config_filename = filename
        self.verify_config_file()
        self.load_config()
        #self.readfp(file(IDE_CONFIG_FILE, "r")) 
        
    #----------------------------------------------------------------------
    def get_format_config(self, section, option):
        
        value = self.get(section, option)
        if type(value) != type(""): return value
        
        if re.match("([-\d]+)$", value):
            return int(value)
        
        if re.match("([.\d*e-]+)$", value):
            return float(value)
        
        elif re.match("(true)$", value.lower()):
            return True
        
        elif re.match("(false)$", value.lower()):
            return False
    
        else: return value
            
    #---------------------------------------------------------------------- 
    def config(self, section, option, default):
        
        if self.has_section(section) and self.has_option(section, option):
            return self.get_format_config(section, option)
        else:
            self.set(section, option, default)
            return default
        
    #----------------------------------------------------------------------
    def set(self, section, option, value):
        
        if not self.has_section(section): self.add_section(section)
        super(ConfigLibs, self).set(section, option, value)
    
    #----------------------------------------------------------------------
    def verify_config_file(self):
        
        if not os.path.isfile(self.config_filename):
            file(self.config_filename, "w").close()
        
    #----------------------------------------------------------------------
    def save_config(self):
        
        self.write(file(self.config_filename, "w"))
        
    #----------------------------------------------------------------------
    def load_config(self):
        
        self.readfp(file(self.config_filename, "r")) 
        
        