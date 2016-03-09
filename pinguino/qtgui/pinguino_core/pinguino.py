#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import shutil
import argparse
import re
import codecs

from .pinguino_tools import PinguinoTools
from .boards import boardlist as AllBoards


########################################################################
class Pinguino(PinguinoTools):


    #----------------------------------------------------------------------
    def __init__(self):
        """Pinguino

        Class for handle Pinguino from Python.
        """

        super(Pinguino, self).__init__()


    #----------------------------------------------------------------------
    def compile_file(self, file_path):
        """Compile file from path.

        Parameters
        ----------
        file_path: str, list
            Full absolute or relative path of file, list of files is project.

        See Also
        --------
        compile_string: Compile from string.
        """

        if type(file_path) == str:
            file_path = [file_path]

        data = self.verify(file_path)
        self.__compiled__ = data["verified"]
        self.__data__ = data


    #----------------------------------------------------------------------
    def compile_string(self, code, tmp_filename="temp_filename.pde", filenames=[]):
        """Compile from string.

        Parameters
        ----------
        code: str
            String with Pinguino Code for compile
        tmp_filename: str, optional
            Temporal filename used for compile the code.
        filenames: list of strings, optional
            Environment files for projects.

        See Also
        --------
        compile_file: Compile from path.

        Examples
        --------
        >>> code = "setup(){pinmode(0,OUTPUT);}loop(){toggle(0);delay(100);}"
        >>> compile_string(code)
        """

        tmp_dir = os.path.join(os.getenv("PINGUINO_USER_PATH"), "temp")
        if not os.path.exists(tmp_dir):
            os.mkdir(tmp_dir)
        tmp_file = os.path.join(tmp_dir, tmp_filename)
        file_ = codecs.open(tmp_file, "w", encoding="utf-8")
        file_.write(code)
        file_.close()
        self.compile_file([tmp_file] + filenames)


    #----------------------------------------------------------------------
    def upload(self):
        """Upload previous successfully compiled file.


        Returns
        -------
         uploaded: bool
            Is True if file was uploaded succesffully, otherwise is False.

        data: list
            List of messages reports.
        """

        data = self.__upload__()
        data = "\n".join(data)
        NO_UPLOADED_KEY_TEXT = ["Pinguino not found",
                                "Error",
                                "Upload not possible",
                                "No target found",
                                "Resource busy",
                                "Entity not found",
                                ]

        if False in [data.find(key) == -1 for key in NO_UPLOADED_KEY_TEXT]:
            if data.find("Error") != -1: data = data[data.find("Error"):]
            return False, data
        return True, data


    # #----------------------------------------------------------------------
    # def set_bootloader(self, bldr, memstart):
        # """Configure bootloader for current board selected.

        # Parameters
        # ----------
        # bldr: str
            # "noboot", "boot2" or "boot4"
        # memstart: int
            # 0, 0x2000 or 0x0C00

        # See Also
        # --------
        # set_board: Set the board to use.
        # """

        # if self.__current_board__.arch == 8:
            # self.__current_board__.bldr = bldr
            # self.__current_board__.memstart = memstart

    #----------------------------------------------------------------------
    def set_icsp(self):
        """"""
        if self.__current_board__.arch == 8:
            self.__current_board__.bldr = "noboot"
            self.__current_board__.memstart = 0


    #----------------------------------------------------------------------
    def set_8bit_compiler(self, compiler):
        """"""
        assert compiler.lower() in ["sdcc", "xc8"], "Compiler must be SDCC or XC8"
        self.SET_COMPILER_8BIT = compiler.lower()


    #----------------------------------------------------------------------
    def compiled(self):
        """Check if file was successfully compiled.

        Returns
        -------
        compiled: bool
            True if file was successfully compiled, otherwise is False.
        """

        return self.__compiled__


    #----------------------------------------------------------------------
    def get_errors(self):
        """Getter for errors while compiling.

        Returns
        -------
        errors: dict
            Dictionary with 3 types o errors (keys): compiling and linking


        See Also
        --------
        get_errors_linking: Get linking errors.
        get_errors_compiling_c: Get compiling C/C++ errors.
        get_errors_compiling_asm: Get compiling assembler errors.
        """

        errors = {}
        errors["compiling"] = self.__data__["compiling"]
        errors["linking"] = self.__data__["linking"]
        return errors


    #----------------------------------------------------------------------
    def get_errors_linking(self):
        """Getter for linking errors.

        Returns
        -------
        error: dict, None
            Dictionary with error message or None.

        See Also
        --------
        get_errors: Get errors while compiling.
        get_errors_compiling_c: Get compiling C/C++ errors.
        get_errors_compiling_asm: Get compiling assembler errors.
        """

        errors = self.__data__["linking"]
        if not errors: return None

        error = {}
        error["linking"] = errors
        error["line_numbers"] = []
        for err in errors:
            match = re.match(".*user.c:([\d]+):[\d]+: error.*", err)
            if not match is None: error["line_numbers"] += [int(match.groups()[0])]
        return error


    #----------------------------------------------------------------------
    def get_errors_compiling_c(self):
        """Getter for compiling C/C++ errors.

        Returns
        -------
        error: dict
            Dictionary with error message or None.

        See Also
        --------
        get_errors: Get errors while compiling.
        get_errors_linking: Get linking errors.
        get_errors_compiling_asm: Get compiling assembler errors.
        """

        errors = self.__data__["compiling"]["c"]
        if not errors: return None

        error = {}
        error["line_numbers"] = []
        error["complete_message"] = ""
        error["messages"] = {}
        for err in errors:
            error["line_numbers"] += [int(err["line_number"])]
            error["complete_message"] += err["line_number"] + ": " + err["message"]
            error["messages"][err["line_number"]] = err["message"]
        return error


    #----------------------------------------------------------------------
    def get_errors_compiling_asm(self):
        """Getter for compiling assembler errors.

        Returns
        -------
        error: str, None
            String with error message or None.

        See Also
        --------
        get_errors: Get errors while compiling.
        get_errors_linking: Get linking errors.
        get_errors_compiling_asm: Get compiling assembler errors.
        """

        errors = self.__data__["compiling"]["asm"]
        if not errors: return None

        if (type(errors) is dict) and ("error_symbols" in errors):
            return errors["error_symbols"]
        else:
            return errors


    #----------------------------------------------------------------------
    def get_result(self):
        """Getter log for post-compiling process.

        Returns
        -------
        result: dict
            Dictionary with info (keys) about: time, hex_file, code_size
        """

        assert self.__compiled__, "Must compile first."
        result = {}
        result["time"] = self.__data__["time"]
        result["hex_file"] = self.__data__["hex_file"]
        result["code_size"] = self.__data__["code_size"]
        return result


    #----------------------------------------------------------------------
    def get_hex(self):
        """Getter for hex file.

        Returns
        -------
        file: file
            Readonly hex file object.
        """

        assert self.__compiled__, "Must compile first."
        return open(self.__data__["hex_file"], "r")

