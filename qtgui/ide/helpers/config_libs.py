#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import re
from ConfigParser import RawConfigParser

from .constants import IDE_CONFIGLIBS_FILE

########################################################################
class ConfigLibs(RawConfigParser, object):
    """"""
    
    #----------------------------------------------------------------------
    def __init__(self):
        super(ConfigLibs, self).__init__()
        
        self.verify_config_file()
        self.load_config()
        #self.readfp(file(IDE_CONFIG_FILE, "r")) 
        
    #----------------------------------------------------------------------
    def get_format_config(self, section, option):
        """"""
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
        """"""
        if self.has_section(section) and self.has_option(section, option):
            return self.get_format_config(section, option)
        else:
            self.set(section, option, default)
            return default
        
    #----------------------------------------------------------------------
    def set(self, section, option, value):
        """"""
        if not self.has_section(section): self.add_section(section)
        super(ConfigLibs, self).set(section, option, value)
    
    #----------------------------------------------------------------------
    def verify_config_file(self):
        """"""
        if not os.path.isfile(IDE_CONFIGLIBS_FILE):
            file(IDE_CONFIGLIBS_FILE, "w").close()
        
    #----------------------------------------------------------------------
    def save_config(self):
        """"""
        self.write(file(IDE_CONFIGLIBS_FILE, "w"))
        
    #----------------------------------------------------------------------
    def load_config(self):
        """"""
        self.readfp(file(IDE_CONFIGLIBS_FILE, "r")) 
        
        
    #----------------------------------------------------------------------
    def get_recents(self):
        """"""
        if not self.has_section("Recents"): self.add_section("Recents")
        options = self.options("Recents")
        
        files = []
        for option in options: files.append(self.get("Recents", option))
        
        return files
    
    #----------------------------------------------------------------------
    def get_all_sources(self):
        """"""
        sources = {}
        for section in self.sections():
            sources[section] = {"repository": self.get(section, "repository"),
                                "description": self.config(section, "description", ""),
                                "author": self.config(section, "author", ""),
                                "arch": self.config(section, "arch", ""),
                                "active": self.config(section, "active", False),
                                "installed": self.config(section, "installed", False)}
                
        return sources
            
                
    #----------------------------------------------------------------------
    def check_duplicated(self, source):
        """"""
        self.load_config()
        sources = self.get_all_sources()
        
        return source in [sources[key]["repository"] for key in sources.keys()]
        
            
            
        
        