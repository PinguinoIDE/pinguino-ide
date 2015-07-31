#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import logging
import shutil
import platform
import sys

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from configparser import RawConfigParser
else:
    #Python2
    from ConfigParser import RawConfigParser

########################################################################
class PinguinoConfig(object):

    #----------------------------------------------------------------------
    @classmethod
    def set_environ_vars(cls):
        """Configure some environ variables about OS and architecture.
        """

        if not os.path.exists(os.path.join(os.getenv("PINGUINO_DATA"), "paths.cfg")):
            logging.error("Missing: "+os.path.join(os.getenv("PINGUINO_DATA"), "paths.cfg"))
            sys.exit()

        config_paths = RawConfigParser()
        config_paths.readfp(open(os.path.join(os.getenv("PINGUINO_DATA"), "paths.cfg"), "r"))

        #RB20141116 : get the “bitness” of the current OS
        bitness, linkage = platform.architecture()
        os.environ["PINGUINO_OS_ARCH"] = bitness

        if os.name == "posix": #GNU/Linux
            os.environ["PINGUINO_OS_NAME"] = "linux"

        #Mac could return posix :/
        elif os.name == "os2":  #Mac OS X
            os.environ["PINGUINO_OS_NAME"] = "macosx"

        elif os.name == "nt":  #Windows
            os.environ["PINGUINO_OS_NAME"] = "windows"

        #load path from paths.conf
        os.environ["PINGUINO_USER_PATH"] = os.path.expandvars(os.path.expanduser(config_paths.get("paths-%s"%os.getenv("PINGUINO_OS_NAME"), "user_path")))
        os.environ["PINGUINO_INSTALL_PATH"] = os.path.expandvars(os.path.expanduser(config_paths.get("paths-%s"%os.getenv("PINGUINO_OS_NAME"), "install_path")))
        os.environ["PINGUINO_USERLIBS_PATH"] = os.path.expandvars(os.path.join(os.getenv("PINGUINO_USER_PATH"), "library_manager"))


    #----------------------------------------------------------------------
    @classmethod
    def check_user_files(cls):
        """Check existence of depends directories and created if missed."""

        #check instalation path
        if not os.path.exists(os.getenv("PINGUINO_INSTALL_PATH")):
            logging.warning("Missing Pinguino libraries instalation. "
                            "Make sure the paths in %s are correct."%os.path.join(os.getenv("PINGUINO_DATA"), "paths.conf"))

        #create ~/.pinguino
        if not os.path.exists(os.getenv("PINGUINO_USER_PATH")):
            os.mkdir(os.getenv("PINGUINO_USER_PATH"))

        #Check files and directories
        cls.check_locations()


    #----------------------------------------------------------------------
    @classmethod
    def check_locations(cls):
        """If not exist some directory then copy them from installation path."""

        cls.if_not_exist_then_copy(src=os.path.join(os.getenv("PINGUINO_INSTALL_PATH"), "examples"),
                                   dst=os.path.join(os.getenv("PINGUINO_USER_PATH"), "examples"))

        cls.if_not_exist_then_copy(src=os.path.join(os.getenv("PINGUINO_INSTALL_PATH"), "graphical_examples"),
                                   dst=os.path.join(os.getenv("PINGUINO_USER_PATH"), "graphical_examples"))

        cls.if_not_exist_then_copy(src=os.path.join(os.getenv("PINGUINO_INSTALL_PATH"), "source"),
                                   dst=os.path.join(os.getenv("PINGUINO_USER_PATH"), "source"))

        cls.if_not_exist_then_copy(src=os.path.join(os.getenv("PINGUINO_INSTALL_PATH"), "p32", "obj"),
                                   dst=os.path.join(os.getenv("PINGUINO_USER_PATH"), "source", "obj"))

        cls.if_not_exist_then_copy(src=os.path.join(os.getenv("PINGUINO_DATA"), "qtgui", "config", "pinguino.%s.conf"%os.getenv("PINGUINO_OS_NAME")),
                                   dst=os.path.join(os.getenv("PINGUINO_USER_PATH"), "pinguino.conf"))


    #----------------------------------------------------------------------
    @classmethod
    def if_not_exist_then_copy(cls, src, dst):
        """The method name say all.

        Parameters
        ----------
        src: str
            Source file or directory.

        dst: str
            Destiny file or directory.
        """

        #When src is a file
        if os.path.isfile(src):
            if not os.path.exists(src): logging.warning("Missing: " + src)
            else:
                if not os.path.exists(dst): shutil.copy(src, dst)
            return

        #Create parent directories
        def prepare_directory(file_path):
            dir_, filename = os.path.split(file_path)
            all_dirs = []
            while not os.path.exists(dir_):
                all_dirs.append(dir_)
                dir_, dir2 = os.path.split(dir_)
            all_dirs.reverse()
            for dir_ in all_dirs:  os.mkdir(dir_)

        #When src is a directory, copy file by file
        all_files = []
        for path, dirs, files in os.walk(src):
            for f in files:
                file_path_src = os.path.join(path, f)
                file_path_dst = dst + os.path.join(path, f).replace(src, "")

                if not os.path.exists(file_path_dst):
                    logging.warning("Copying: " + file_path_dst)
                    prepare_directory(file_path_dst)
                    shutil.copy(file_path_src, file_path_dst)


    #----------------------------------------------------------------------
    @classmethod
    def update_pinguino_extra_options(self, config, pinguino_object):
        """Load on Pinguino Object the advance configuration for 32 bit.

        Parameters
        ----------
        config: RawConfigParser
            Config file.
        pinguino_object: Pinguino()
            Pinguino
        """

        pinguino_object.MIPS16 = str(config.config("Board", "mips16", True)).lower()
        pinguino_object.HEAPSIZE = str(config.config("Board", "heapsize", 512))
        pinguino_object.OPTIMIZATION = config.config("Board", "optimization", "-O3")


    #----------------------------------------------------------------------
    @classmethod
    def update_pinguino_paths(cls, config, pinguino_object, prefix=None, workspace=None):
        """Load paths of libraries and compilers.

        Parameters
        ----------
        config: RawConfigParser
            Config file.
        pinguino_object: Pinguino()
            Pinguino
        prefix: str, optional
            Path for search libraries and compilers, default is absolute path from root.
        workspace: str,optional
            Path for makefiles and temp files, default is ~/.pinguino/source

        """

        user_sdcc_bin = config.get_path("sdcc_bin", prefix)
        if user_sdcc_bin: pinguino_object.P8_SDCC_BIN = user_sdcc_bin
        else: logging.warning("Missing path to 'sdcc_bin': '%s'"%user_sdcc_bin)

        user_sdcc_bin = config.get_path("xc8_bin", prefix)
        if user_sdcc_bin: pinguino_object.P8_XC8_BIN = user_sdcc_bin
        else: logging.warning("Missing path to 'xc8_bin': '%s'"%user_sdcc_bin)

        user_gcc_bin = config.get_path("gcc_bin", prefix)
        if user_gcc_bin: pinguino_object.P32_GCC_BIN = user_gcc_bin
        else: logging.warning("Missing path to 'gcc_bin': '%s'"%user_gcc_bin)

        if workspace:
            pinguino_object.SOURCE_DIR = workspace
        else:
            pinguino_source = os.path.join(os.getenv("PINGUINO_USER_PATH"), "source")
            if pinguino_source: pinguino_object.SOURCE_DIR = pinguino_source
            else: logging.warning("Missing path to 'source': '%s'"%pinguino_source)

        pinguino_8_libs = config.get_path("pinguino_8_libs", prefix)
        if pinguino_8_libs: pinguino_object.P8_DIR = pinguino_8_libs
        else: logging.warning("Missing path to '8_libs': '%s'"%pinguino_8_libs)

        pinguino_32_libs = config.get_path("pinguino_32_libs", prefix)
        if pinguino_32_libs: pinguino_object.P32_DIR = pinguino_32_libs
        else: logging.warning("Missing path to '32_libs': '%s'"%pinguino_32_libs)


    #----------------------------------------------------------------------
    @classmethod
    def update_user_libs(cls, pinguino_object):
        """Reload paths for user installed libraries.

        Parameters
        ----------
        pinguino_object: Pinguino()
            Pinguino
        """

        from ..ide.commons.library_manager import Librarymanager

        libs = Librarymanager()

        all_p8 = libs.get_p8_libraries()
        all_p8 = map(lambda lib:lib["p8"], all_p8)
        pinguino_object.USER_P8_LIBS = all_p8

        all_p32 = libs.get_p32_libraries()
        all_p32 = map(lambda lib:lib["p32"], all_p32)
        pinguino_object.USER_P32_LIBS = all_p32

        pinguino_object.USER_PDL = libs.get_pdls()

