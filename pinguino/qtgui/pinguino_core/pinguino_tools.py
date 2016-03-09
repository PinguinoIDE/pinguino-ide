#! /usr/bin/python2
#  -*- coding: UTF-8 -*-

"""-------------------------------------------------------------------------
    pinguino

    (c) 2008-2009-2010-2011 Jean-Pierre MANDON <jp.mandon@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
    -------------------------------------------------------------------------"""

import sys
import os
from subprocess import Popen, STDOUT
import re
import shutil
import time
import argparse
import logging
import codecs
import string
import logging

from .config import Config

from .boards import boardlist as Boardlist
from .uploader.uploader import Uploader

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from io import StringIO
else:
    #Python2
    from cStringIO import StringIO

HOME_DIR = os.path.split(os.path.dirname(os.path.realpath(__file__)))[0]


########################################################################
class PinguinoTools(Uploader):

    #----------------------------------------------------------------------
    def __init__(self):
        """Pinguino Tools.

        Helper methods for preprocesing, compiling and updloading code to Pinguino.
        """

        # v2 dropped
        # self.NoBoot = ("noboot", 0)
        # self.Boot2 = ("boot2", 0x2000)
        # self.Boot4 = ("boot4", 0x0C00)

        # self.dict_boot = {"v2": self.Boot2,
                          # "v4": self.Boot4,
                          # "no": self.NoBoot,
                          # }


    #----------------------------------------------------------------------
    def set_os_variables(self):
        """Set the compiler and makefile for each OS.
        """

        compiler = self.get_8bit_compiler()

        if os.getenv("PINGUINO_OS_NAME") == "windows":
            if compiler == "sdcc":
                self.COMPILER_8BIT = os.path.join(self.P8_SDCC_BIN, "sdcc.exe")
            elif compiler == "xc8":
                self.COMPILER_8BIT = os.path.join(self.P8_XC8_BIN, "xc8.exe")  #change for xc8

            #self.p8 = 'picpgm.exe'
            #self.UPLOADER_32 = os.path.join(self.P32_GCC_BIN, "mphidflash.exe")

            # RB : 2014-11-14
            # Windows installer should download and install GnuWin32
            # and add path to the System Path, something like :
            # set PATH=%PATH%;C:\Program Files\GnuWin32\bin
            #self.MAKE = "make.exe"

            self.MAKE = os.path.join(self.P32_GCC_BIN, "make.exe")

            if not os.path.exists(self.MAKE):
                logging.warning("Missing '{}'".format(self.MAKE))


        elif os.getenv("PINGUINO_OS_NAME") == "linux":
            if compiler.lower() == "sdcc":
                self.COMPILER_8BIT = os.path.join(self.P8_SDCC_BIN, "sdcc")
            elif compiler.lower() == "xc8":
                self.COMPILER_8BIT = os.path.join(self.P8_XC8_BIN, "xc8")

            #self.p8 = 'picpgm'
            #self.UPLOADER_32 = os.path.join(self.P32_GCC_BIN, "ubw32")
            #self.UPLOADER_32 = os.path.join(self.P32_GCC_BIN, "pic32prog")
            self.MAKE = "make"

        elif os.getenv("PINGUINO_OS_NAME") == "macosx":
            if compiler.lower() == "sdcc":
                self.COMPILER_8BIT = os.path.join(self.P8_SDCC_BIN, "sdcc")
            elif compiler.lower() == "xc8":
                self.COMPILER_8BIT = os.path.join(self.P8_XC8_BIN, "xc8")
            #self.p8 = 'picpgm'
            #self.UPLOADER_32 = os.path.join(self.P32_GCC_BIN, "mphidflash")
            self.MAKE = "make"


        if not os.path.exists(self.P8_SDCC_BIN):
            logging.warning("Missing '{}'".format(self.P8_SDCC_BIN))

        if not os.path.exists(self.COMPILER_8BIT):
            logging.warning("Missing '{}'".format(self.COMPILER_8BIT))


    #----------------------------------------------------------------------
    def get_8bit_compiler(self):
        """"""
        if hasattr(self, "SET_COMPILER_8BIT"):  #if no setted
            return getattr(self, "SET_COMPILER_8BIT").lower()
        else:
            config = Config()  #then, select from config
            return config.config("Board", "compiler", "sdcc").lower()


    #----------------------------------------------------------------------
    def set_board(self, board):
        """Select Pinguino board for compiling and uploading.

        Parameters
        ----------
        board: boards.BoardClass
            Board class from .boards.
        """

        # Board reference
        class Board(board):
            pass


        self.__current_board__ = Board
        #self.get_regobject_libinstructions(board.arch)


    #----------------------------------------------------------------------
    def get_board(self):
        """Getter for current board selected.

        Returns
        -------
        board: boards.BoardClass
            Board class from .boards.
        """

        return self.__current_board__


    #----------------------------------------------------------------------
    def get_filename(self):
        """Getter for the current *.pde file.

        Returns
        -------
        filename: str
            Absolute path for current *.pde used.
        """

        return self.__filename__


    #----------------------------------------------------------------------
    def get_hex_file(self):
        """Getter for the *.hex generated.

        Returns
        -------
        filename: str
            Absolute path for *.hex generated.
        """
        return self.__hex_file__


    #----------------------------------------------------------------------
    def verify(self, filenames):
        """Start process for compiling files.

        1. Preprocess
        2. Compile
        3. Linking

        Parameters
        ----------
        filenames: list of str
            A list of files to compile.

        Returns
        -------
        data_msg: dict
            Dictionary with data about process result.
        """

        self.set_os_variables()

        data_msg = {}
        data_msg["compiling"] = {"c":[], "asm":[]}
        data_msg["linking"] = []
        data_msg["verified"] = False

        if type(filenames) == str:
            filenames = [filenames]

        filename = filenames[0]
        filenames.reverse()
        self.__filename__ = filename
        filename = os.path.splitext(filename)[0]

        t0 = time.time()

        src_dir = os.path.expanduser(self.SOURCE_DIR)

        if os.path.exists("{}.hex".format(filename)):
            os.remove("{}.hex".format(filename))

        cur_board = self.get_board()
        self.preprocess(filenames)

        if cur_board.arch == 8: MAIN_FILE="main.hex"
        else: MAIN_FILE="main32.hex"

        retour, error_compile = self.compile()
        if retour != 0:
            data_msg["verified"] = False
            data_msg["compiling"] = error_compile
            return data_msg
        else:
            retour, error_link = self.link()
            if os.path.exists(os.path.join(src_dir, MAIN_FILE)) != True:
                data_msg["verified"] = False
                data_msg["linking"] = error_link
                return data_msg
            else:
                shutil.copy(os.path.join(src_dir, MAIN_FILE), filename+".hex")
                os.remove(os.path.join(src_dir, MAIN_FILE))
                self.__hex_file__ = filename+".hex"

                data_msg["verified"] = True
                data_msg["time"] = "{:.3f}".format( time.time() - t0)
                data_msg["filename"] = self.get_filename()
                data_msg["hex_file"] = "{}.hex".format(filename)
                data_msg["code_size"] = self.get_code_size()


                return data_msg


    #----------------------------------------------------------------------
    def __upload__(self):
        """Upload a compiled file to Pinguino.

        Returns
        -------
        result: list of strings
            A list with debug information about upload process.
        """

        hex_file = self.get_hex_file()
        board = self.get_board()

        # uploader = Uploader(hex_file, board)
        # result = uploader.write_hex()

        # Since Pinguino IDE 11.1 Uploader is an inherited class, so now we must use method configure_uploader
        self.configure_uploader(hex_file, board)
        result = self.upload_hex()
        # result = uploader.report

        # Weed out blank lines with filter
        if result:
            return filter(lambda line: not line.isspace(), result)
        else:
            return []


    #----------------------------------------------------------------------
    def get_regobject_libinstructions(self, arch):
        """Return regobject and libinstructions for each architecture.

        If libraries are not loaded yet then generate it, otherwise reuse existing.

        Parameters
        ----------
        arch: int
            Architecture, 8 or 32 bit.

        Returns
        -------
        libinstructions: list
            A list of libraries and functions from .pdl or .pdl32.
        """

        if arch == 8:
            if getattr(self, "libinstructions_8", False):
                return self.libinstructions_8
            else:
                self.libinstructions_8 = self.read_lib(8)
                return self.libinstructions_8

        elif arch == 32:
            if getattr(self, "libinstructions_32", False):
                return self.libinstructions_32
            else:
                self.libinstructions_32 = self.read_lib(32)
                return self.libinstructions_32


    #----------------------------------------------------------------------
    def read_lib(self, arch, include_default=True):
        """Load .pdl or .pdl32 files (keywords and libraries).

        Trying to find PDL files to store reserved words, store defines and includes.

        Parameters
        ----------
        arch: int
            Architecture, 8 or 32 bit.
        include_default: bool, optional
            Include default pdl for each arch or only instaled, default is True.

        Returns
        -------
        libinstructions: list
            A list of libraries and functions from .pdl or .pdl32.
        """

        regobject = []
        libinstructions = []

        all_pdls = self.USER_PDL

        if arch == 8:
            libext = ".pdl"
            libdir = self.P8_DIR
        else:  #32
            libext = ".pdl32"
            libdir = self.P32_DIR

        if include_default:
            all_pdls.extend(map(lambda pdl:os.path.join(libdir, "pdl", pdl), os.listdir(os.path.join(libdir, "pdl"))))

        all_pdls = filter(lambda name:name.endswith(libext), all_pdls)


        for fichier in all_pdls:

            # check content of the PDL file
            lib_file = open(fichier, "r")
            lines = lib_file.readlines()
            lib_file.close()

            regex_pdl = "[\s]*([.\w]*)[\s]*([\w]*)[\s]*(#include[\w\s\.\<\>/]*)*(#define.*)*[\s]*"

            for line in lines:
                line = line[:line.find('//')]  #ignore comments
                if line.isspace() or not line: continue

                reg = re.match(regex_pdl, line)
                instruction, cnvinstruction, include, define = reg.groups()
                include = "" if include is None else include  #if has includes
                define = "" if define is None else define  #if has directives
                cnvinstruction = instruction if cnvinstruction is "" else cnvinstruction

                if not instruction: continue

                # https://regex101.com/r/nH9nS9
                regex_str = r"([^.\w])({})([^.\w])"

                # Python3 compatibility
                if os.getenv("PINGUINO_PYTHON") is "3":
                    #Python3
                    regex = re.compile("{}".format(regex_str).format(re.escape(instruction)), re.MULTILINE | re.DOTALL)
                    # regex = re.compile("{}".format(regex_str).format(re.escape(instruction)))

                else:
                    #Python2
                    regex = re.compile(u"{}".format(regex_str).format(re.escape(instruction)), re.MULTILINE | re.DOTALL)
                    # regex = re.compile(u"{}".format(regex_str).format(re.escape(instruction)))


                # libinstructions.append([instruction, cnvinstruction, include, define, regex])
                libinstructions.append({"pinguino": instruction,
                                        "c": cnvinstruction,
                                        "include": include,
                                        "define": define,
                                        "regex": regex,
                                        })


        # libinstructions.sort(lambda x,y: cmp(len(x[0]), len(y[0])))
        libinstructions = sorted(libinstructions, key=lambda x: len(x["pinguino"]))  #Python3 style
        libinstructions.reverse()

        return libinstructions[:]


    #----------------------------------------------------------------------
    def remove_strings(self, content):
        """Remove strings from code.

        This is necesary for exclude strings from preprocess.

        Parameters
        ----------
        content: str
            Code with strings.

        Returns
        -------
        content: str
            Content with keys instead strings.
        keys: dict
            Dictionay with key:string for all strings.

        See Also
        --------
        recove_strings: Recove strings with code and keys.
        """

        strings = re.findall(r'"[^"]*"', content)
        content = re.sub(r'"[^"]*"', '"<PINGUINO_STRING>"', content)

        index = 0
        keys = {}
        for string in strings:
            content = content.replace('"<PINGUINO_STRING>"', '"<PINGUINO_STRING:%d>"' % index, 1)
            keys['"<PINGUINO_STRING:%d>"' % index] = string
            index += 1

        return content, keys


    #----------------------------------------------------------------------
    def recove_strings(self, content, keys):
        """Recove strings with code and keys.

        Parameters
        ----------
        content: str
            Code without strings.
        keys: dict
            Dictionary with original strings.

        Returns
        -------
        content: str
            Code with original strings.

        See Also
        --------
        remove_strings: Remove strings from code.
        """

        for key in keys.keys():
            content = content.replace(key, keys[key])
        return content


    #----------------------------------------------------------------------
    def generate_functions_declarations(self):
        """"""
        if not hasattr(self, "functions_declarations"):
            return ""

        declarations = []
        for func in self.functions_declarations:
            decl = "{return} {name}({args});".format(**func)
            declarations.append(decl)

        if declarations:
            return "\n/* Declarations */\n"+"\n".join(declarations)
        else:
            return ""


    #----------------------------------------------------------------------
    def set_functions_declarations(self, obj):
        """"""
        self.functions_declarations = obj


    #----------------------------------------------------------------------
    def preprocess(self, file_path, libinstructions=None, ignore_spaces=False, define_output=None, userc_output=None):
        """Read Pinguino File (.pde) and translate it into C language.

        Parameters
        ----------
        file_path: list of str
            List of absolute paths of *.pde files to preprocess.
        libinstructions: list
            Custom libinstructions
        ignore_spaces: bool
            Ignore spaces like comments in user.c file. Used in projects.
        define_output: str
            Path for destination of user.c. Used in projects.
        userc_output: str
            Path for destination of define.h. Used in projects.
        """

        src_dir = os.path.expanduser(self.SOURCE_DIR)

        defines = []
        # user_content = ""
        user_content = self.generate_functions_declarations()

        if len(file_path) > 1:
            user_content += "\n\n/* Merged\n"
            for path in file_path:
                user_content += "{}\n".format(path)
            user_content += "*/"

        for path in file_path:

            file_pde = codecs.open("{}".format(path), "r", encoding="utf-8")
            user_c = StringIO(file_pde.read())
            file_pde.close()

            # readlines = user_c.readlines()
            # readlines = self.remove_comments(readlines, ignore_spaces=ignore_spaces)
            # user_c = StringIO()
            # for line in readlines:
                # if line.find("#include")!=-1 or line.find("#define")!=-1:
                    # defines.append(line+"\n")
                    # user_c.write("\n")
                # else:
                    # user_c.write(line)

            # search and replace arduino keywords in file
            content = user_c.getvalue()
            content = self.format_code(content)
            content = self.remove_comments(content, ignore_spaces=ignore_spaces)
            content_nostrings, keys = self.remove_strings(content)
            nblines = 0
            # libinstructions = self.get_regobject_libinstructions(self.get_board().arch)

            content_nostrings, defines_lib = self.replace_word(content_nostrings, libinstructions=libinstructions)
            content = self.recove_strings(content_nostrings+"\n", keys)

            defines.extend(defines_lib)
            user_content += content

        if userc_output is None:
            userc_output = os.path.join(src_dir, "user.c")

        if define_output is None:
            define_output = os.path.join(src_dir, "define.h")

        # Generate files
        self.save_define(defines, define_output)
        self.save_userc(user_content, userc_output)


    #----------------------------------------------------------------------
    def format_code(self, code):
        """"""
        # for sym in list('!#$&()*+,-/:;<=>?@[\\]^`{|}~'):
            # code = code.replace(sym, " {} ".format(sym))

        return code


    #----------------------------------------------------------------------
    def save_define(self, defines, file_path):
        """Save headers file.

        Parameters
        ----------
        defines: set of strings
            A list of defines to write.
        file_path: str
            Destinty to write headers.

        See Also
        --------
        save_userc: Save preprocessed code in file.
        """

        fichier = open(file_path, "w")
        #defines = sorted(list(defines))
        fichier.writelines(defines)
        fichier.close()


    #----------------------------------------------------------------------
    def save_userc(self, content, file_path):
        """Save preprocessed code in file.

        Parameters
        ----------
        content: str
            Code after preprocess.
        file_path: str
            Destinty to write preprocess code.

        See Also
        --------
        save_define: Save headers file.
        """

        fichier = open(file_path, "w")
        fichier.write(content)
        fichier.close()


    #----------------------------------------------------------------------
    def replace_word(self, content, libinstructions=None):
        """Convert Pinguino language in C language.

        Parameters
        ----------
        content: str
            Pinguino code.
        libinstructions: list
            Custom libinstructions.

        Returns
        -------
        content: str
            Pinguino code with real libraries/functions name.
        defines: list
            List with directives needed and finded.
        """
        if libinstructions is None:
            libinstructions = self.get_regobject_libinstructions(self.get_board().arch)

        preprocessor_commands = ["if", "ifdef", "elif", "endif", "define", "include"]
        regex_directive = "[\s]*#(" + "|".join(preprocessor_commands)+ ")[\s\S]*"
        defines = []
        keys = {}
        index = 0

        # replace in lines
        content = content.split("\n")
        for line in range(len(content)):

            if not content[line]:
                continue

            #hack!!, very important line!! #truth
            #content[line] = " {} ".format(content[line])

            for instruction in libinstructions:
                if re.search(instruction["regex"], content[line]):
                    content[line] = re.sub(instruction["regex"], '\g<1><PINGUINO_RESERVED:%d>\g<3>' % index, content[line])  #safe
                    #content[line] = content[line][1:-1] #truth

                    keys['<PINGUINO_RESERVED:%d>' % index] = instruction["c"]
                    index += 1

                    if not instruction["define"]+"\n" in defines:
                        defines.append(instruction["define"]+"\n")
                    if not instruction["include"]+"\n" in defines:
                        defines.append(instruction["include"]+"\n")

            match = re.match(regex_directive, content[line])
            if match:
                defines.append(content[line]+"\n")
                if match.group(1) in ["define", "include"]: content[line] = ""


        # content = "\n".join(content)
        content = self.recove_strings("\n".join(content), keys)

        return content, self.sort_directives(defines)


    #----------------------------------------------------------------------
    def sort_directives(self, list_directives):
        """"""
        directives = "\n".join(list_directives) + "\n"

        start = 0
        conditionals = []
        while directives.find("#if", start) != -1:
            start = directives.find("#if", start)
            end = directives.find("#endif", start)
            end_= directives.find("\n", end)
            conditionals.append(directives[start:end_])
            start = end


        for conditional in conditionals:
            directives = directives.replace(conditional, "")
            conditional = conditional.split("\n")

        directives = list(filter(None, sorted(directives.split("\n"))))
        [directives.extend(cond.split("\n")) for cond in conditionals]

        return [directive + "\n" for directive in list(filter(None, directives))]



    #----------------------------------------------------------------------
    def remove_comments(self, textinput, ignore_spaces=False):
        """Remove comments of a code.

        Multiline comments are replaced with white lines, for correct interpretation of debug messages.

        Parameters
        ----------
        textinput: str, list
            Code or lines with code

        Returns
        -------
        textout: str, list
            Code without comments, in the same data type that textinput.
        """

        if type(textinput) == list:
            text = "".join(textinput)
        else:
            text = textinput

        def replacer(match):
            s = match.group(0)

            if s.startswith('/'):
                if not ignore_spaces:
                    return "" + "\n" * (s.count("\n"))
                else:
                    return ""
            else:
                return s

        pattern = re.compile(r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"', re.DOTALL|re.MULTILINE)
        textout = re.sub(pattern, replacer, text)

        if type(textinput) == list:
            textout = textout.split("\n")
            textout = map(lambda x:x+"\n", textout)

        return textout


    #----------------------------------------------------------------------
    def get_user_imports_p8(self):
        user_imports = []
        for lib_dir in self.USER_P8_LIBS:
            user_imports.append("-I" + lib_dir)
        return user_imports

    #----------------------------------------------------------------------
    def get_user_imports_p32(self):
        user_imports = []
        for lib_dir in self.USER_P32_LIBS:
            user_imports.append("-I" + lib_dir)
        return " ".join(user_imports)

    #----------------------------------------------------------------------
    def compile(self):
        """ Compile.

            NB :    "--opt-code-size"   deprecated
                    "--use-non-free"    implicit -I and -L options for non-free headers and libs
                    "-I" + os.path.join(self.P8_DIR, '..', 'sdcc', 'include', 'pic16'),\
                    "-I" + os.path.join(self.P8_DIR, '..', 'sdcc', 'non-free', 'include', 'pic16'),\
            """

        filename = self.__filename__

        src_dir = os.path.expanduser(self.SOURCE_DIR)

        mainc_output = os.path.join(src_dir, "main.c")

        ERROR = {"c": {},
                 "asm": {},}

        board = self.get_board()
        compiler = self.get_8bit_compiler()

        # GCC and XC8 compile and link at the "same time"
        # so nothing to do here
        if board.arch == 32 or compiler == 'xc8':
            return 0, None

        fichier = open(os.path.join(src_dir, "stdout"), "w+")

        user_imports = self.get_user_imports_p8()
        #for lib_dir in self.USER_P8_LIBS:
            #user_imports.append("-I" + lib_dir)

        if board.bldr == 'noboot':

            sortie = Popen([self.COMPILER_8BIT,
                            "--verbose",
                            "-mpic16",
                            "--denable-peeps",
                            "--obanksel=9",
                            "--optimize-cmp",
                            "--optimize-df",
                            "-p" + board.proc,
                            "-D" + board.board,
                            "-D" + board.bldr,
                            "-DBOARD=\"" + board.board + "\"",
                            "-DPROC=\"" + board.proc + "\"",
                            "-DBOOT_VER=0",
                            "--use-non-free",
                            "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'core'),
                            "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'libraries'),
                            "-I" + os.path.dirname(filename),
                            "--compile-only",
                            mainc_output,
                            "-o" + os.path.join(src_dir, 'main.o')] + user_imports,
                           stdout=fichier, stderr=STDOUT)


        # if board.bldr == 'boot2':

            # sortie = Popen([self.COMPILER_8BIT,
                            # "--verbose",
                            # "-mpic16",
                            # "--denable-peeps",
                            # "--obanksel=9",
                            # "--optimize-cmp",
                            # "--optimize-df",
                            # "-p" + board.proc,
                            # "-D" + board.board,
                            # "-D" + board.bldr,
                            # "-DBOARD=\"" + board.board + "\"",
                            # "-DPROC=\"" + board.proc + "\"",
                            # "-DBOOT_VER=2",
                            # "--use-non-free",
                            # "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'core'),
                            # "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'libraries'),
                            # "-I" + os.path.dirname(filename),
                            # "--compile-only",
                            # "-o" + os.path.join(, 'main.o'),
                            # mainc_output] + user_imports,
                           # stdout=fichier, stderr=STDOUT)


        #if board.bldr == 'boot4':
        else:

            sortie = Popen([self.COMPILER_8BIT,
                            "--verbose",
                            "-mpic16",
                            "--denable-peeps",
                            "--obanksel=9",
                            "--optimize-cmp",
                            "--optimize-df",
                            # Do not remove --ivt-loc option
                            "--ivt-loc=" + str(board.memstart),
                            "-p" + board.proc,
                            "-D" + board.board,
                            "-D" + board.bldr,
                            "-DBOARD=\"" + board.board + "\"",
                            "-DPROC=\"" + board.proc + "\"",
                            "-DBOOT_VER=4",
                            "--use-non-free",
                            "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'core'),
                            "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'libraries'),
                            "-I" + os.path.dirname(filename),
                            "--compile-only",
                            mainc_output,
                            "-o" + os.path.join(src_dir, 'main.o')] + user_imports,
                           stdout=fichier, stderr=STDOUT)



        sortie.communicate()
        if sortie.poll()!=0:
            #
            # Error treatment (RB: fixed 2012-11-15)
            #

            # set the file pointer to the beginning of stdout
            fichier.seek(0)

            # read lines until 'error' or 'Error' is found
            lines = fichier.readlines()
            errors_c = []
            errors_asm = []
            for ligne in lines:
                # C errors
                error_pos = ligne.find('error')
                if (error_pos != -1):
                    if os.name == "nt": err = 2
                    else: err = 1
                    error_line_number = ligne.split(":")[err]
                    error_message = ligne.split(":")[-1]
                    if error_line_number.isdigit():
                        errors_c.append({"line_number": error_line_number,
                                         "message": error_message,})

                # ASM errors
                error_pos = ligne.find('Error')
                if (error_pos != -1):
                    # do not display error line number since they are from the ASM file
                    # display error symbol instead
                    error_symbol = ligne[ligne.find("(_") + 2 : ligne.find(").")]
                    error_message = ligne[ error_pos + 13 : ligne.find("(_") - 1]
                    errors_asm.append(error_symbol)

            ERROR["c"] = errors_c
            ERROR["asm"] = errors_asm

        fichier.close()
        return sortie.poll(), ERROR



    # ------------------------------------------------------------------------------
    def report(self, message):
        #import sys
        #reload(sys)
        #sys.stdout.write("DEBUG : " + message + "\r\n")
        logging.info(message)

    #----------------------------------------------------------------------
    def link(self):
        """Link.

        NB :  "--opt-code-size"   deprecated
              "--use-non-free"    implicit -I and -L options for non-free headers and libs
                    "-I" + os.path.join(self.P8_DIR, 'sdcc', 'include', 'pic16'),\
                    "-I" + os.path.join(self.P8_DIR, 'sdcc', 'non-free', 'include', 'pic16'),\
                    "-I" + os.path.join(self.P8_DIR, 'pinguino', 'core'),\
                    "-I" + os.path.join(self.P8_DIR, 'pinguino', 'libraries'),\
                    "-L" + os.path.join(self.P8_DIR, 'sdcc', 'lib', 'pic16'),\
                    "-L" + os.path.join(self.P8_DIR, 'sdcc', 'non-free', 'lib', 'pic16'),\
        """

        error = []
        board = self.get_board()
        compiler = self.get_8bit_compiler()

        src_dir = os.path.expanduser(self.SOURCE_DIR)

        userc_output = os.path.join(src_dir, "user.c")

        fichier = open(os.path.join(src_dir, "stdout"), "w+")
        user_imports = self.get_user_imports_p8()

        file_dir = os.path.dirname(self.__filename__)

        if board.arch == 8 and board.bldr == 'noboot' and compiler == 'xc8':

            sortie = Popen([self.COMPILER_8BIT,
                # Define device
                "--CHIP=" + board.proc,
                # -Verbose, -Preprocess Assembly Files, -Map file,
                "-P", "-N64", "-V", "-M", "-G",
                # Compiler Optimizations
                "--OPT=default,+asm,+asmfile,-speed,+space,-debug",
                # We use the 32-bit format for both float and double values
                "--FLOAT=24", "--DOUBLE=24",
                # Reject any non-standard memory qualifiers (near, far, ...) in C source code
                "--ADDRQUAL=reject",
                # TODO : in Board Selector, adding 2 more options
                # to XC8 compiler: free and pro
                "--MODE=free",
                # Set Warning Level (form -9 to 9)
                "--WARN=0",
                # Shows the assembly output (.lst)
                "--ASMLIST",
                # Select the type of memory summary that is displayed after compilation
                "--SUMMARY=default,+psect,-class,+mem,-hex,-file",
                # Enable all software workarounds
                "--ERRATA=default",
                # --RUNTIME=-plib -> don't include plib.h
                "--RUNTIME=+init,+clib,+clear,-config,-download,-flp,-no_startup,-osccal,-keep,-plib,-resetbits,-stackcall",
                # Allows the type of the output files
                "--OUTPUT=intel",
                # Options to the Linker
                "-L-AUSB5=0500h-05FFh",
                "-L-Pusbram5=USB5",
                # Macros
                "-D" + board.board,
                "-D" + board.bldr,
                # To use in Pinguino programs
                "-DBOARD=\"" + board.board + "\"",
                "-DPROC=\"" + board.proc + "\"",
                "-DBOOT_VER=0",
                # Path
                "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'core'),
                "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'libraries'),
                "-I" + file_dir,
                os.path.join(src_dir, 'main.c'),
                "-O" + os.path.join(src_dir, 'main.hex'),
                ] + user_imports,
               stdout=fichier, stderr=STDOUT)

        elif board.arch == 8 and board.bldr == 'noboot' and compiler == 'sdcc':

            sortie = Popen([self.COMPILER_8BIT,
                "--verbose",\
                "-mpic16",\
                "--denable-peeps",\
                "--obanksel=9",\
                "--optimize-cmp",\
                "--optimize-df",\
                #"--no-crt",\ we use default run-time module inside libsdcc.lib
                "-Wl-s" + os.path.join(self.P8_DIR, 'lkr', board.proc + '_g.lkr') + ",-m",\
                "-p" + board.proc,\
                "-D" + board.bldr,\
                "-D" + board.board,\
                "-DBOARD=\"" + board.board + "\"",\
                "-DPROC=\"" + board.proc + "\"",\
                "-DBOOT_VER=0",\
                "--use-non-free",\
                "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'core'),\
                "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'libraries'),\
                "-I" + file_dir,\
                'libio' + board.proc + '.lib',\
                'libdev' + board.proc + '.lib',\
                'libc18f.lib',\
                'libm18f.lib',\
                # link the default run-time module
                'libsdcc.lib',\
                "-o" + os.path.join(src_dir, 'main.hex'),\
                os.path.join(src_dir, 'main.o')] + user_imports,\
                stdout=fichier, stderr=STDOUT)


        #if board.arch == 8 and board.bldr == 'boot2' and compiler == 'xc8':
            #
            # ERROR : XC8 and Boot2 are not compatible and will never be
            #
            #return

        #if board.arch == 8 and board.bldr == 'boot2' and compiler == 'sdcc':
            # sortie = Popen([self.COMPILER_8BIT,
                # "--verbose",\
                # "-mpic16",\
                # "--denable-peeps",\
                # "--obanksel=9",\
                # "--optimize-cmp",\
                # "--optimize-df",\
                # "--no-crt",\
                # "-Wl-s" + os.path.join(self.P8_DIR, 'lkr', board.bldr + '.' + board.proc + '.lkr') + ",-m",\
                # "-p" + board.proc,\
                # "-D" + board.bldr,\
                # "-D" + board.board,\
                # "-DBOARD=\"" + board.board + "\"",\
                # "-DPROC=\"" + board.proc + "\"",\
                # "-DBOOT_VER=2",\
                # "--use-non-free",\
                # "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'core'),\
                # "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'libraries'),\
                # 'libio' + board.proc + '.lib',\
                # 'libdev' + board.proc + '.lib',\
                # 'libc18f.lib',\
                # 'libm18f.lib',\
                # 'libsdcc.lib',\
                # "-o" + os.path.join(, 'main.hex'),\
                # os.path.join(self.P8_DIR, 'obj', 'application_iface.o'),\
                # os.path.join(self.P8_DIR, 'obj', 'boot_iface.o'),\
                # os.path.join(self.P8_DIR, 'obj', 'usb_descriptors.o'),\
                # os.path.join(self.P8_DIR, 'obj', 'crt0ipinguino.o'),\
                # os.path.join(, 'main.o')] + user_imports,\
                # stdout=fichier, stderr=STDOUT)


        #if board.arch == 8 and board.bldr == 'boot4' and compiler == 'xc8':
        elif board.arch == 8 and board.bldr != 'noboot' and compiler == 'xc8':

            sortie = Popen([self.COMPILER_8BIT,
                # Define device
                "--CHIP=" + board.proc,
                # -Verbose, -Preprocess Assembly Files, -Map file,
                "-P", "-N64", "-V", "-M", "-G",
                # Compiler Optimizations
                "--OPT=default,+asm,+asmfile,-speed,+space,-debug",
                # We use the 32-bit format for both float and double values
                "--FLOAT=24", "--DOUBLE=24",
                # Reject any non-standard memory qualifiers (near, far, ...) in C source code
                "--ADDRQUAL=reject",
                # TODO : in Board Selector, adding 2 more options
                # to XC8 compiler: free and pro
                "--MODE=free",
                # Set Warning Level (form -9 to 9)
                "--WARN=0",
                # Shows the assembly output (.lst)
                "--ASMLIST",
                # Select the type of memory summary that is displayed after compilation
                "--SUMMARY=default,+psect,-class,+mem,-hex,-file",
                # Enable all software workarounds
                "--ERRATA=default",
                # --RUNTIME=-plib -> don't include plib.h
                "--RUNTIME=+init,+clib,+clear,-config,-download,-flp,-no_startup,-osccal,-keep,-plib,-resetbits,-stackcall",
                # Allows the type of the output files
                "--OUTPUT=intel",
                # Move reset and interrupts vectors after bootloader code
                "--CODEOFFSET=" + hex(board.memstart),
                # Options to the Linker
                "-L-AUSB5=0500h-05FFh",
                "-L-Pusbram5=USB5",
                # Macros
                "-D" + board.board,
                "-D" + board.bldr,
                # To use in Pinguino programs
                "-DBOARD=\"" + board.board + "\"",
                "-DPROC=\"" + board.proc + "\"",
                "-DBOOT_VER=4",
                # Path
                "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'core'),
                "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'libraries'),
                "-I" + file_dir,
                os.path.join(src_dir, 'main.c'),
                "-O" + os.path.join(src_dir, 'main.hex'),
                ] + user_imports,
               stdout=fichier, stderr=STDOUT)

        #if board.arch == 8 and board.bldr == 'boot4' and compiler == 'sdcc':
        elif board.arch == 8 and board.bldr != 'noboot' and compiler == 'sdcc':

            sortie = Popen([self.COMPILER_8BIT,
                "--verbose", "-V",\
                "-mpic16",\
                # optimization
                "--denable-peeps",\
                "--obanksel=9",\
                "--optimize-cmp",\
                "--optimize-df",\
                # don't want to link default crt0i.o but crt0i.c
                "--no-crt",\
                # move all int. vectors after bootloader code
                "--ivt-loc=" + str(board.memstart),\
                # link memory map
                "-Wl-s" + os.path.join(self.P8_DIR, 'lkr', board.bldr + '.' + board.proc + '.lkr') + ",-m",\
                "-p" + board.proc,\
                "-D" + board.bldr,\
                "-D" + board.board,\
                "-DBOARD=\"" + board.board + "\"",\
                "-DPROC=\"" + board.proc + "\"",\
                "-DBOOT_VER=4",\
                "--use-non-free",\
                "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'core'),\
                "-I" + os.path.join(self.P8_DIR, 'include', 'pinguino', 'libraries'),\
                "-I" + file_dir, \
                os.path.join(src_dir, 'main.o'),\
                'libio' + board.proc + '.lib',\
                'libdev' + board.proc + '.lib',\
                'libc18f.lib',\
                'libm18f.lib',\
                # link the default run-time module (crt0i.o)
                # except when "-no-crt" option is used
                'libsdcc.lib',\
                "-o" + os.path.join(src_dir, 'main.hex'),\
                ] + user_imports,\
                stdout=fichier, stderr=STDOUT)


        elif board.arch == 32:

            makefile = os.path.join(src_dir, 'Makefile32.'+os.getenv("PINGUINO_OS_NAME"))

            user_imports32 = self.get_user_imports_p32()

            if user_imports32: _IDE_USERLIBS_ = ["_IDE_USERLIBS_=" + user_imports32]
            else: _IDE_USERLIBS_ = []

            #self.report(makefile)

            sortie = Popen([self.MAKE,
                "--makefile=" + makefile,
                "_IDE_PDEDIR_=" + file_dir,
                "_IDE_PROC_=" + board.proc,
                "_IDE_BOARD_=" + board.board,
                "_IDE_BINDIR_=" + self.P32_GCC_BIN,  #default /usr/bin
                "_IDE_P32DIR_=" + self.P32_DIR,  #default /usr/share/pinguino-11.0/p32
                "_IDE_SRCDIR_=" + self.SOURCE_DIR,
                "_IDE_USERHOMEDIR_=" + os.getenv("PINGUINO_USER_PATH"),  #default ~/.pinguino
                "_IDE_OSARCH_=" + os.getenv("PINGUINO_OS_ARCH"),
                "_IDE_HEAP_SIZE_=" + self.HEAPSIZE,
                "_IDE_MIPS16_ENABLE_=" + self.MIPS16,
                "_IDE_OPTIMIZATION_=" + self.OPTIMIZATION,
                ] + _IDE_USERLIBS_,

                         stdout=fichier, stderr=STDOUT)

        sortie.communicate()

        fichier.seek(0)
        # Check if child process has terminated
        if sortie.poll() != 0:
            for ligne in fichier:
                if ligne.find("error") != -1:
                    error.append(ligne)
        fichier.close()

        if sys.platform == "win32":

            if board.board in ["PIC32_PINGUINO_220", "Pinguino32MX220", "Pinguino32MX250", "Pinguino32MX270"]:
                badrecord = ":040000059D0040001A\n"
            else:
                badrecord = ":040000059D006000FA\n"

            if os.path.exists(os.path.join(src_dir, "main32tmp.hex")):
                fichiersource = open(os.path.join(src_dir, "main32tmp.hex"), "r")
                fichierdest = open(os.path.join(src_dir, "main32.hex"), "w+")
                for line in fichiersource:
                    if line != badrecord:
                        fichierdest.writelines(line)
                fichiersource.close()
                fichierdest.close()
                os.remove(os.path.join(src_dir, "main32tmp.hex"))

        return sortie.poll(), error


    #----------------------------------------------------------------------
    def get_code_size(self):

        board = self.get_board()
        filename = self.get_hex_file()

        codesize = 0
        address_Hi = 0

        memfree = board.memend - board.memstart

        #print "%X" % board.memstart
        #print "%X" % board.memend

        fichier = open(filename, 'r')
        lines = fichier.readlines()

        for line in lines:

            byte_count = int(line[1:3], 16)
            address_Lo = int(line[3:7], 16)
            record_type= int(line[7:9], 16)

            # extended linear address record
            if record_type == 4:
                address_Hi = int(line[9:13], 16) << 16

            # code size
            if record_type == 0:

                # address calculation
                address = address_Hi + address_Lo
                #self.displaymsg(_("address = %X" % address),0)

                #if address >= board.memstart:
                if (address >= board.memstart) and (address < board.memend):
                    codesize = codesize + byte_count

        fichier.close()
        # return "Code size: " + str(codesize) + " / " + str(memfree) + " " + "bytes" + " (" + str(100*codesize/memfree) + "% " + "used"+ ")"
        return "Code size: {}/{} bytes ({:.2f}% used)".format(codesize, memfree, 100*codesize/memfree)  #Python3 rocks!


    #----------------------------------------------------------------------
    def force_reload_libs(self):
        """"""
        if hasattr(self, "libinstructions_32"):
            delattr(self, "libinstructions_32")

        if hasattr(self, "libinstructions_8"):
            delattr(self, "libinstructions_8")


# ------------------------------------------------------------------------------
# getOptions
# ------------------------------------------------------------------------------
def getOptions():
    parser = argparse.ArgumentParser(description='*** Pinguino IDE ***')
    parser.add_argument('-v', '--version', dest='version', action='store_true', default=False, help='show Pinguino IDE version and exit')
    parser.add_argument('-a', '--author', dest='author', action='store_true', default=False, help='show authors of this Pinguino IDE version and exit')
    parser.add_argument('-f', '--filename', dest='filename', nargs=1, default=False, help='filename to process')
    parser.add_argument('-dv', '--dev', dest='dev', nargs=1, default=False, help='set developer mode')
    parser.add_argument('-ul', '--upload', dest='upload', const=True, action='store_const', default=False, help='upload code')
    parser.add_argument('-bt', '--boot', dest='bootloader', nargs=1, default=False, help='set bootloader option')

    for b in range(len(Boardlist)):
        parser.add_argument(    Boardlist[b].shortarg,
                                Boardlist[b].longarg,
                                dest='board',
                                const=b,
                                action='store_const',
                                default=False,
                                help='compile code for ' + Boardlist[b].board + ' board')
    return parser.parse_args()

