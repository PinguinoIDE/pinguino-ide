#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import re
import logging

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") == "3":
    #Python3
    from configparser import RawConfigParser
else:
    #Python2
    from ConfigParser import RawConfigParser


########################################################################
class Config(RawConfigParser, object):

    #----------------------------------------------------------------------
    def __init__(self):
        """RawConfigParser based.

        A set of sections and options for storage configurations.

        [SECTION1]
        option11 = value11
        option12 = value12

        [SECTION2]
        option21 = (0, 1, 2)
        option22 = 3
        option23 = False

        [SECTION3]
        option3 = value3
        """

        super(Config, self).__init__()

        self.ide_config_file = os.path.join(os.getenv("PINGUINO_USER_PATH"), "pinguino.conf")
        self.verify_config_file()
        self.load_config()


    #----------------------------------------------------------------------
    def get_format_config(self, section, option):
        """Return formated configuration option.

        Parameters
        ----------
        section: str
            Section name.
        option: str
            Option name.

        Returns
        -------
        value: str, float, bool
            Return corret data type.
        """

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
        """Read option or return default.

        Parameters
        ----------
        section: str
            Section name.
        option: str
            Option name.
        default: str, float, bool

        Returns
        -------
        option: str, float, bool
            If option exist then return them, otherwise return default value.
        """

        if self.has_section(section) and self.has_option(section, option):
            return self.get_format_config(section, option)
        else:
            self.set(section, option, default)
            return default


    #----------------------------------------------------------------------
    def set(self, section, option, value):
        """save option value.

        Parameters
        ----------
        section: str
            Section name.
        option: str
            Option name.
        value: str
            Value for option.
        """

        if not self.has_section(section): self.add_section(section)
        super(Config, self).set(section, option, value)


    #----------------------------------------------------------------------
    def verify_config_file(self):
        """Check if exist configuration file. otherwise create it.
        """

        if not os.path.isfile(self.ide_config_file):
            open(self.ide_config_file, "w").close()


    #----------------------------------------------------------------------
    def save_config(self):
        """Write configuration on configuration file.
        """

        self.write(open(self.ide_config_file, "w"))


    #----------------------------------------------------------------------
    def load_config(self):
        """Read configuration from configuration file.
        """

        self.readfp(open(self.ide_config_file, "r"))


    #----------------------------------------------------------------------
    def get_path(self, name, prefix=None):
        """Get corret formated path from config.

        Parameters
        ----------
        name: str
            Option name from configuration file.
        prefix: str, optional
            Path to add before the getted from configuration file, default is original path.

        Returns
        -------
        path: str
            Absolute path.
        """

        paths = self.config("Features", "pathstouse", "Paths")

        if self.has_option(paths, name):
            if prefix:
                return os.path.normpath(os.path.join(prefix, self.get(paths, name)[1:]))
            else:
                return os.path.normpath(self.get(paths, name))
        else:
            logging.warning("Missing '{}' for 'Paths' or 'PathsCustom' in config file: '{}'".format(name, self.ide_config_file))
            return ""


    #----------------------------------------------------------------------
    def get_recents(self, section="Recents"):
        """Getter for recents files from configuration.

        Parameters
        ----------
        section: str, optional
            Section for search recents.

        Returns
        -------
        files: list
            A list of paths for recents files.
        """

        if not self.has_section(section): self.add_section(section)
        options = self.options(section)

        files = []
        for option in options:
            if option.startswith("recent_") and self.get(section, option):
                files.append(self.get(section, option))

        return files


    #----------------------------------------------------------------------
    def get_recents_open(self, section="Recents"):
        """Getter for recents opened files from configuration.

        Parameters
        ----------
        section: str, optional
            Section for search recents opened.

        Returns
        -------
        files: list
            A list of paths for recents opened files.
        """

        if not self.has_section(section): self.add_section(section)
        options = self.options(section)

        files = []
        for option in options:
            if option.startswith("open_") and self.get(section, option):
                files.append(self.get(section, option))

        return files


    #----------------------------------------------------------------------
    def clear_recents_open(self, section="Recents"):
        """Remove recents opened files from configuration.

        Parameters
        ----------
        section: str, optional
            Section for remove recents opened.
        """

        if not self.has_section(section): self.add_section(section)
        options = self.options(section)

        for option in options:
            if option.startswith("open_"):
                self.remove_option(section, option)


    #----------------------------------------------------------------------
    def clear_recents(self, section="Recents"):
        """Remove recents files from configuration.

        Parameters
        ----------
        section: str, optional
            Section for remove recents.
        """

        if not self.has_section(section): self.add_section(section)
        options = self.options(section)

        for option in options:
            if option.startswith("recent_"):
                self.remove_option(section, option)
