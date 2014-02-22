#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import logging
import shutil
import sys
from ConfigParser import RawConfigParser

from ..ide.methods.library_manager import Librarymanager

########################################################################
class PinguinoConfig(object):

    #----------------------------------------------------------------------
    @classmethod
    def set_environ_vars(cls):
        if not os.path.exists(os.path.join(os.getenv("PINGUINO_DATA"), "paths.cfg")):
            logging.error("Missing: "+os.path.join(os.getenv("PINGUINO_DATA"), "paths.cfg"))
            sys.exit()

        config_paths = RawConfigParser()
        config_paths.readfp(open(os.path.join(os.getenv("PINGUINO_DATA"), "paths.cfg"), "r"))

        if os.name == "posix": #GNU/Linux
            os.environ["PINGUINO_OS_NAME"] = "linux"

        elif os.name == "nt":  #Windows
            os.environ["PINGUINO_OS_NAME"] = "windows"

        #load path from paths.conf
        os.environ["PINGUINO_USER_PATH"] = os.path.expanduser(config_paths.get("paths-%s"%os.getenv("PINGUINO_OS_NAME"), "user_path"))
        os.environ["PINGUINO_INSTALL_PATH"] = os.path.expanduser(config_paths.get("paths-%s"%os.getenv("PINGUINO_OS_NAME"), "install_path"))
        os.environ["PINGUINO_USERLIBS_PATH"] = os.path.join(os.getenv("PINGUINO_USER_PATH"), "library_manager")


    #----------------------------------------------------------------------
    @classmethod
    def check_user_files(cls):

        #check instalation path
        if not os.path.exists(os.getenv("PINGUINO_INSTALL_PATH")):
            logging.warning("Missing Pinguino libraries instalation. "
                            "Make sure the paths in %s are correct."%os.path.join(os.getenv("PINGUINO_DATA"), "paths.conf"))

        #create ~/.pinguino
        if not os.path.exists(os.getenv("PINGUINO_USER_PATH")):
            os.mkdir(os.getenv("PINGUINO_USER_PATH"))

        #Check files
        cls.check_dirs()
        cls.check_config_files()


    #----------------------------------------------------------------------
    @classmethod
    def check_dirs(cls):

        cls.if_not_exist_then_copy(src=os.path.join(os.getenv("PINGUINO_INSTALL_PATH"), "examples"),
                                    dst=os.path.join(os.getenv("PINGUINO_USER_PATH"), "examples"),
                                    default_dir=True)

        cls.if_not_exist_then_copy(src=os.path.join(os.getenv("PINGUINO_INSTALL_PATH"), "graphical_examples"),
                                    dst=os.path.join(os.getenv("PINGUINO_USER_PATH"), "graphical_examples"),
                                    default_dir=True)

        cls.if_not_exist_then_copy(src=os.path.join(os.getenv("PINGUINO_HOME"), "pinguino_source", "source"),
                                    dst=os.path.join(os.getenv("PINGUINO_USER_PATH"), "source"))

        #FIXME: wath to do with this dir?
        cls.if_not_exist_then_copy(src=os.path.join(os.getenv("PINGUINO_INSTALL_PATH"), "p32", "obj"),
                                    dst=os.path.join(os.getenv("PINGUINO_USER_PATH"), "source", "obj"))


    #----------------------------------------------------------------------
    @classmethod
    def check_config_files(cls):

        cls.if_not_exist_then_copy(src=os.path.join(os.getenv("PINGUINO_DATA"), "qtgui", "config", "pinguino.%s.conf"%os.getenv("PINGUINO_OS_NAME")),
                                    dst=os.path.join(os.getenv("PINGUINO_USER_PATH"), "pinguino.conf"))

        cls.if_not_exist_then_copy(src=os.path.join(os.getenv("PINGUINO_DATA"), "qtgui", "config", "reserved.pickle"),
                                    dst=os.path.join(os.getenv("PINGUINO_USER_PATH"), "reserved.pickle"))

        cls.if_not_exist_then_copy(src=os.path.join(os.getenv("PINGUINO_DATA"), "qtgui", "config", "wikidocs.pickle"),
                                    dst=os.path.join(os.getenv("PINGUINO_USER_PATH"), "wikidocs.pickle"))



    #----------------------------------------------------------------------
    @classmethod
    def if_not_exist_then_copy(cls, src, dst, default_dir=False):
        if not os.path.exists(src):
            logging.warning("Missing: " + src)
            if not os.path.exists(dst) and default_dir:
                os.mkdir(dst)
            return

        if not os.path.exists(dst):
            if os.path.isdir(src):
                shutil.copytree(src, dst)
            else:
                shutil.copy(src, dst)

    #----------------------------------------------------------------------
    @classmethod
    def update_pinguino_paths(cls, config, pinguino_object):
        user_sdcc_bin = config.get_path("sdcc_bin")
        if user_sdcc_bin: pinguino_object.P8_BIN = user_sdcc_bin
        else: logging.warning("Missing path to 'sdcc_bin': '%s'"%user_sdcc_bin)

        user_gcc_bin = config.get_path("gcc_bin")
        if user_gcc_bin: pinguino_object.P32_BIN = user_gcc_bin
        else: logging.warning("Missing path to 'gcc_bin': '%s'"%user_gcc_bin)

        pinguino_source = os.path.join(os.getenv("PINGUINO_USER_PATH"), "source")
        if pinguino_source: pinguino_object.SOURCE_DIR = pinguino_source
        else: logging.warning("Missing path to 'source': '%s'"%pinguino_source)

        pinguino_8_libs = config.get_path("pinguino_8_libs")
        if pinguino_8_libs: pinguino_object.P8_DIR = pinguino_8_libs
        else: logging.warning("Missing path to '8_libs': '%s'"%pinguino_8_libs)

        pinguino_32_libs = config.get_path("pinguino_32_libs")
        if pinguino_32_libs: pinguino_object.P32_DIR = pinguino_32_libs
        else: logging.warning("Missing path to '32_libs': '%s'"%pinguino_32_libs)


    #----------------------------------------------------------------------
    @classmethod
    def update_user_libs(cls, pinguino_object):
        libs = Librarymanager()

        all_p8 = libs.get_p8_libraries()
        all_p8 = map(lambda lib:lib["p8"], all_p8)
        pinguino_object.USER_P8_LIBS = all_p8

        all_p32 = libs.get_p32_libraries()
        all_p32 = map(lambda lib:lib["p32"], all_p32)
        pinguino_object.USER_P32_LIBS = all_p32

        pinguino_object.USER_PDL = libs.get_pdls()