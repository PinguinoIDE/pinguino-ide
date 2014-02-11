#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import re
from ConfigParser import RawConfigParser

########################################################################
class Config(RawConfigParser, object):
    
    
    #----------------------------------------------------------------------
    def __init__(self):
        super(Config, self).__init__()
        
        self.ide_config_file = os.path.join(os.environ.get("PINGUINO_USER_PATH"), "pinguino.conf")
        self.verify_config_file()
        self.load_config()
        
        
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
        super(Config, self).set(section, option, value)
    
    #----------------------------------------------------------------------
    def verify_config_file(self):
        if not os.path.isfile(self.ide_config_file):
            open(self.ide_config_file, "w").close()
        
    #----------------------------------------------------------------------
    def save_config(self):
        self.write(open(self.ide_config_file, "w"))
        
    #----------------------------------------------------------------------
    def load_config(self):
        self.readfp(open(self.ide_config_file, "r")) 
        
    
    #----------------------------------------------------------------------
    def get_path(self, name):
        
        return self.get("Paths", name)
        
        
    #----------------------------------------------------------------------
    def get_recents(self):
        if not self.has_section("Recents"): self.add_section("Recents")
        options = self.options("Recents")
        
        files = []
        for option in options:
            if option.startswith("recent_"):
                files.append(self.get("Recents", option))
        
        return files
    
    #----------------------------------------------------------------------
    def get_recents_open(self):
        if not self.has_section("Recents"): self.add_section("Recents")
        options = self.options("Recents")
        
        files = []
        for option in options:
            if option.startswith("open_"):
                files.append(self.get("Recents", option))
        
        return files
    
    #----------------------------------------------------------------------
    def clear_recents_open(self):
        if not self.has_section("Recents"): self.add_section("Recents")
        options = self.options("Recents")
        
        files = []
        for option in options:
            if option.startswith("open_"):
                files.append(self.set("Recents", option, ""))
        
        return files
        