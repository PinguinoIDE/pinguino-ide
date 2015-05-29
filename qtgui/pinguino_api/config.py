#! /usr/bin/python2
#-*- coding: utf-8 -*-

import os
import re
import logging

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from configparser import RawConfigParser
else:
    #Python2
    from ConfigParser import RawConfigParser

########################################################################
class Config(RawConfigParser, object):


    #----------------------------------------------------------------------
    def __init__(self):
        super(Config, self).__init__()

        self.ide_config_file = os.path.join(os.getenv("PINGUINO_USER_PATH"), "pinguino.conf")
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
    def get_path(self, name, prefix=None):
        if self.has_option("Paths", name):
            if prefix:
                return os.path.normpath(os.path.join(prefix, self.get("Paths", name)[1:]))
            else:
                return os.path.normpath(self.get("Paths", name))
        else:
            logging.warning("Missing '%s' for 'Paths' in config file: '%s'"%(name, self.ide_config_file))
            return ""

    #----------------------------------------------------------------------
    def get_recents(self, section="Recents"):
        if not self.has_section(section): self.add_section(section)
        options = self.options(section)

        files = []
        for option in options:
            if option.startswith("recent_") and self.get(section, option):
                files.append(self.get(section, option))

        return files

    #----------------------------------------------------------------------
    def get_recents_open(self, section="Recents"):
        if not self.has_section(section): self.add_section(section)
        options = self.options(section)

        files = []
        for option in options:
            if option.startswith("open_") and self.get(section, option):
                files.append(self.get(section, option))

        return files

    #----------------------------------------------------------------------
    def clear_recents_open(self, section="Recents"):
        if not self.has_section(section): self.add_section(section)
        options = self.options(section)

        files = []
        for option in options:
            if option.startswith("open_"):
                files.append(self.set(section, option, ""))

        return files

    #----------------------------------------------------------------------
    def clear_recents(self, section="Recents"):
        if not self.has_section(section): self.add_section(section)
        options = self.options(section)

        files = []
        for option in options:
            if option.startswith("recent_"):
                files.append(self.set(section, option, ""))

        return files
