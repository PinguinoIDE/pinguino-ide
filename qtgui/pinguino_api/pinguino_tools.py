#!/usr/bin/env python
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

from .boards import boardlist as Boardlist
from .uploader.uploader import Uploader

HOME_DIR = os.path.split(os.path.dirname(os.path.realpath(__file__)))[0]



########################################################################
class PinguinoTools(object):

    #----------------------------------------------------------------------
    def __init__(self):

        self.NoBoot = ("noboot", 0)
        self.Boot2 = ("boot2", 0x2000)
        self.Boot4 = ("boot4", 0x0C00)

        self.dict_boot = {"v2": self.Boot2,
                          "v4": self.Boot4,
                          "no": self.NoBoot,
                          }

        #self.P32_DIR = "p32"
        #self.P8_DIR = "p8"


    #----------------------------------------------------------------------
    def set_os_variables(self):

        #if sys.platform == 'darwin':
            #self.c8 = 'sdcc'
            #self.u32 = 'mphidflash'
            #self.make = 'make'

        if os.getenv("PINGUINO_OS_NAME") == "windows":
            self.COMPILER_8BIT = os.path.join(self.P8_BIN, "sdcc.exe")
            #self.p8 = 'picpgm.exe'
            self.UPLOADER_32 = os.path.join(self.P32_BIN, "mphidflash.exe")
            self.MAKE = os.path.join(self.P32_BIN, "make.exe")

        elif os.getenv("PINGUINO_OS_NAME") == "linux":
            self.COMPILER_8BIT = os.path.join(self.P8_BIN, "sdcc")
            #self.p8 = 'picpgm'
            self.UPLOADER_32 = os.path.join(self.P32_BIN, "ubw32")
            self.MAKE = "make"



    #----------------------------------------------------------------------
    def set_board(self, board):

        self.__current_board__ = board
        #self.get_regobject_libinstructions(board.arch)

    #----------------------------------------------------------------------
    def get_board(self):

        return self.__current_board__

    #----------------------------------------------------------------------
    def get_filename(self):

        return self.__filename__

    #----------------------------------------------------------------------
    def get_hex_file(self):

        return self.__hex_file__


    #----------------------------------------------------------------------
    def verify(self, filename):
        DATA_RETURN = {}
        DATA_RETURN["compiling"] = {"c":[], "asm":[]}
        DATA_RETURN["linking"] = []
        DATA_RETURN["preprocess"] = []

        self.__filename__ = filename

        #self.in_verify=1
        t0 = time.time()

        filename = os.path.splitext(filename)[0]
        if os.path.exists(filename + ".hex"): os.remove(filename + ".hex")
        if os.path.exists(os.path.join(os.path.expanduser(self.SOURCE_DIR), "user.c")): os.remove(os.path.join(os.path.expanduser(self.SOURCE_DIR), "user.c"))

        cur_board = self.get_board()
        retour, error_preprocess = self.preprocess(filename)

        if not retour:
            DATA_RETURN["verified"] = False
            DATA_RETURN["preprocess"] = error_preprocess
            return DATA_RETURN


        # compilation
        if cur_board.arch == 8: MAIN_FILE="main.hex"
        else: MAIN_FILE="main32.hex"


        retour, error_compile = self.compile(filename)
        if retour!=0:
            DATA_RETURN["verified"] = False
            DATA_RETURN["compiling"] = error_compile
            return DATA_RETURN
            #self.displaymsg(_("error while compiling"),0)
            #self.displaymsg(_("check highlighted lines in your code"),0)
            #self.displaymsg(_("You can review the file stdout (F8) for more information."),0)
        else:
            retour, error_link = self.link(filename)
            if os.path.exists(os.path.join(os.path.expanduser(self.SOURCE_DIR), MAIN_FILE)) != True:
                DATA_RETURN["verified"] = False
                DATA_RETURN["linking"] = error_link
                #self.displaymsg(_("error while linking")+" "+filename+".o",0)
                #self.displaymsg(_("You can review the file stdout (F8) for more information."),0)
                return DATA_RETURN
            else:
                shutil.copy(os.path.join(os.path.expanduser(self.SOURCE_DIR), MAIN_FILE), filename+".hex")
                #self.displaymsg(_("compilation done"),0)
                #self.displaymsg(self.__get_code_size__(filename, self.curBoard),0)
                #t = "%.1f" % ( time.time() - t0 )
                #self.displaymsg( t + " "+_("seconds process time"),0)
                os.remove(os.path.join(os.path.expanduser(self.SOURCE_DIR), MAIN_FILE))
                self.__hex_file__ = filename+".hex"

                DATA_RETURN["verified"] = True
                DATA_RETURN["time"] = "%.3f" % ( time.time() - t0 )
                DATA_RETURN["filename"] = self.get_filename()
                DATA_RETURN["hex_file"] = filename+".hex"
                DATA_RETURN["code_size"] = self.get_code_size()


                return DATA_RETURN


    #----------------------------------------------------------------------
    def __upload__(self):

        hex_file = self.get_hex_file()
        board = self.get_board()

        if board.arch == 8:
            uploader = Uploader(hex_file, board)
            result = uploader.write_hex()

        elif board.arch == 32:
            fichier = open(os.path.join(os.path.expanduser(self.SOURCE_DIR), 'stdout'), 'w+')

            sortie=Popen([os.path.join(os.path.dirname(self.P32_BIN), self.UPLOADER_32),
                          "-w",
                          hex_file,
                          "-r",
                          "-n"],
                         stdout=fichier, stderr=STDOUT)
            sortie.communicate()
            fichier.seek(0)
            result = fichier.readlines()
            fichier.close()

        result = filter(lambda line:not line.isspace(), result)
        return result


    #----------------------------------------------------------------------
    def get_regobject_libinstructions(self, arch):
        """Return regobject and libinstructions for each architecture."""
        if arch == 8:
            if getattr(self, "regobject_8", False) and getattr(self, "libinstructions_8", False):
                return self.regobject_8,  self.libinstructions_8
            else:
                self.regobject_8, self.libinstructions_8 = self.read_lib(8)
                return self.regobject_8,  self.libinstructions_8

        elif arch == 32:
            if getattr(self, "regobject_32", False) and getattr(self, "libinstructions_32", False):
                return self.regobject_32, self.libinstructions_32
            else:
                self.regobject_32, self.libinstructions_32 = self.read_lib(32)
                return self.regobject_32, self.libinstructions_32


    #----------------------------------------------------------------------
    def read_lib(self, arch, include_default=True):
        """Load .pdl or .pdl32 files (keywords and libraries)
         trying to find PDL files to store reserved words."""

        regobject = []
        libinstructions = []

        all_pdls = self.USER_PDL

        if arch == 8:
            libext = ".pdl"
            libdir = self.P8_DIR
        else:
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
                line = line[:line.find('//')]
                if line.isspace() or not line: continue

                reg = re.match(regex_pdl, line)
                instruction, cnvinstruction, include, define = reg.groups()
                include = "" if include is None else include
                define = "" if define is None else define
                cnvinstruction = instruction if cnvinstruction is "" else cnvinstruction
                libinstructions.append([instruction, cnvinstruction, include, define])

                if not instruction: continue

                regex = re.compile(r"(^|[' ']|['=']|['{']|[',']|[\t]|['(']|['!'])%s\W" % re.escape(instruction))

                regobject.append(regex)

        return regobject[:], libinstructions[:]


    #----------------------------------------------------------------------
    def preprocess(self, filename):
        """Read Pinguino File (.pde) and translate it into C language"""

        error = []
        #defineword = {}
        #index = 0

        # delete old define.h and create a new one
        if os.path.exists(os.path.join(os.path.expanduser(self.SOURCE_DIR), "define.h")):
            os.remove(os.path.join(os.path.expanduser(self.SOURCE_DIR), "define.h"))
        fichier = open(os.path.join(os.path.expanduser(self.SOURCE_DIR), "define.h"), "a")
        fichier.close()

        # rename .pde in user.c
        #name = os.path.split(filename)[1]
        shutil.copy(filename + ".pde", os.path.join(os.path.expanduser(self.SOURCE_DIR), "user.c"))
        #fichier = open(os.path.join(os.path.expanduser(self.SOURCE_DIR), "user.c"), "a")
        #fichier.close()


        fichier = open(os.path.join(os.path.expanduser(self.SOURCE_DIR), "user.c"), "r")
        i=0
        file_line = {}
        for line in fichier.readlines():
            if line.find("#include")!=-1 or line.find("#define")!=-1:
                line = line[:line.find('//')]   # Ignores C++ comments, fixing Issue 11
                self.add_define(line)    # add to define.h
                file_line[i] = " \n"   # delete from user.c
                i += 1
            else:
                file_line[i] = line
                i += 1
        fichier.close()

        # rewrite file user.c without #include and #define
        fichier = open(os.path.join(os.path.expanduser(self.SOURCE_DIR), "user.c"), "w")
        for cpt in range(i):
            fichier.write(file_line[cpt])
        fichier.close()

        # search and replace arduino keywords in file
        fichier = open(os.path.join(os.path.expanduser(self.SOURCE_DIR), "user.c"), "r")
        content = fichier.read()
        content = self.remove_comments(content)
        content = content.split('\n')
        nblines = 0
        for line in content:
            if not line.isspace():
                resultline = self.replace_word(line)
            else: resultline = "\n"
            #FIXME: error line
            #if resultline.find("error") == 1:
                ##line = resultline
                ##print "error " + resultline
                ##self.displaymsg("error "+resultline,1)
                #error.append(resultline)
                #return False
            file_line[nblines] = resultline
            nblines += 1
        fichier.close()


        # save new tmp file
        fichier = open(os.path.join(os.path.expanduser(self.SOURCE_DIR), "user.c"), "w")
        for i in range(0, nblines):
            fichier.writelines(file_line[i])
        fichier.writelines("\r\n")
        fichier.close()

        # sort define.h
        fichier = open(os.path.join(os.path.expanduser(self.SOURCE_DIR), "define.h"), "r")
        lignes = fichier.readlines()
        lignes.sort()
        fichier.close()

        # save sorted lines
        fichier = open(os.path.join(os.path.expanduser(self.SOURCE_DIR), "define.h"), "w")
        fichier.writelines(lignes)
        fichier.close()

        return True, error


    #----------------------------------------------------------------------
    def add_define(self, chaine):
        """ add #define in define.h file """
        fichier = open(os.path.join(os.path.expanduser(self.SOURCE_DIR), "define.h"), "a")
        fichier.writelines(chaine)
        fichier.writelines("\r\n")
        fichier.close()

    #----------------------------------------------------------------------
    def not_in_define(self, chaine):
        """ verify if #define exists in define.h file """
        fichier = open(os.path.join(os.path.expanduser(self.SOURCE_DIR), "define.h"), "r")
        for line in fichier.readlines():
            # chaine has been found ?
            if line.find(chaine) != -1:
                fichier.close()
                return False
        fichier.close()
        return True

    #----------------------------------------------------------------------
    def replace_word(self, line):
        """ convert pinguino language in C language """

        regobject, libinstructions = self.get_regobject_libinstructions(self.get_board().arch)

        # replace arduino/pinguino language and add #define or #include to define.h
        for i in range(len(libinstructions)):
            if re.search(regobject[i], line):
                line = line.replace(libinstructions[i][0], libinstructions[i][1])
                #print (str(self.libinstructions[i][0]), str(self.libinstructions[i][1]))
                #print (str(self.libinstructions[i][2]), str(self.libinstructions[i][3]))
                if self.not_in_define(libinstructions[i][2]):
                    self.add_define(libinstructions[i][2])
                if self.not_in_define(libinstructions[i][3]):
                    self.add_define(libinstructions[i][3])
        return line+"\n"


    #----------------------------------------------------------------------
    def remove_comments(self, text):
        #FIXME: replace comment with white lines for debugger
        #def replacer(match):
            #s = match.group(0)

            #if s.startswith('/'):
                ##return "" #bug in line number in error info, multiline comments
                #return "" + "\n" * (s.count("\n"))

            #else:
                #return s

        #pattern = re.compile(
            #r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
            #re.DOTALL | re.MULTILINE
        #)
        #return re.sub(pattern, replacer, text)
        return text

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
    def compile(self, filename):
        """ Compile.

        NB :  "--opt-code-size"   deprecated
              "--use-non-free"    implicit -I and -L options for non-free headers and libs
        """

        ERROR = {"c": {},
                 "asm": {},}

        board = self.get_board()

        if board.arch == 32: return 0, None

        fichier = open(os.path.join(os.path.expanduser(self.SOURCE_DIR), "stdout"), "w+")

        user_imports = self.get_user_imports_p8()
        #for lib_dir in self.USER_P8_LIBS:
            #user_imports.append("-I" + lib_dir)

        if board.bldr == 'boot2':
            sortie = Popen([self.COMPILER_8BIT,
                "--verbose",\
                "-mpic16",\
                "--denable-peeps",\
                "--obanksel=9",\
                "--optimize-cmp",\
                "--optimize-df",\
                "-p" + board.proc,\
                "-D" + board.board,\
                "-D" + board.bldr,\
                "-DBOARD=\"" + board.board + "\"",\
                "-DPROC=\"" + board.proc + "\"",\
                "-DBOOT_VER=2",\
                "-I" + os.path.join(self.P8_DIR, 'sdcc', 'include', 'pic16'),\
                "-I" + os.path.join(self.P8_DIR, 'sdcc', 'non-free', 'include', 'pic16'),\
                "-I" + os.path.join(self.P8_DIR, 'pinguino', 'core'),\
                "-I" + os.path.join(self.P8_DIR, 'pinguino', 'libraries'),\
                "-I" + os.path.dirname(filename),\
                "--compile-only",\
                "-o" + os.path.join(os.path.expanduser(self.SOURCE_DIR), 'main.o'),\
                os.path.join(os.path.expanduser(self.SOURCE_DIR), 'main.c')] + user_imports,\
                stdout=fichier, stderr=STDOUT)


        elif board.bldr == 'boot4':
            sortie = Popen([self.COMPILER_8BIT,
                "--verbose",\
                "-mpic16",\
                "--denable-peeps",\
                "--obanksel=9",\
                "--optimize-cmp",\
                "--optimize-df",\
                # Do not remove --ivt-loc option
                "--ivt-loc=" + str(board.memstart),\
                "-p" + board.proc,\
                "-D" + board.board,\
                "-D" + board.bldr,\
                "-DBOARD=\"" + board.board + "\"",\
                "-DPROC=\"" + board.proc + "\"",\
                "-DBOOT_VER=4",\
                "-I" + os.path.join(self.P8_DIR, 'sdcc', 'include', 'pic16'),\
                "-I" + os.path.join(self.P8_DIR, 'sdcc', 'non-free', 'include', 'pic16'),\
                "-I" + os.path.join(self.P8_DIR, 'pinguino', 'core'),\
                "-I" + os.path.join(self.P8_DIR, 'pinguino', 'libraries'),\
                "-I" + os.path.dirname(filename),\
                "--compile-only",\
                os.path.join(os.path.expanduser(self.SOURCE_DIR), 'main.c'),\
                "-o" + os.path.join(os.path.expanduser(self.SOURCE_DIR), 'main.o')] + user_imports,\
                stdout=fichier, stderr=STDOUT)

        elif board.bldr == 'noboot':
            sortie = Popen([self.COMPILER_8BIT,
                "--verbose",\
                "-mpic16",\
                "--denable-peeps",\
                "--obanksel=9",\
                "--optimize-cmp",\
                "--optimize-df",\
                "-p" + board.proc,\
                "-D" + board.board,\
                "-D" + board.bldr,\
                "-DBOARD=\"" + board.board + "\"",\
                "-DPROC=\"" + board.proc + "\"",\
                "-DBOOT_VER=0",\
                "-I" + os.path.join(self.P8_DIR, 'sdcc', 'include', 'pic16'),\
                "-I" + os.path.join(self.P8_DIR, 'sdcc', 'non-free', 'include', 'pic16'),\
                "-I" + os.path.join(self.P8_DIR, 'pinguino', 'core'),\
                "-I" + os.path.join(self.P8_DIR, 'pinguino', 'libraries'),\
                "-I" + os.path.dirname(filename),\
                "--compile-only",\
                os.path.join(os.path.expanduser(self.SOURCE_DIR), 'main.c'),\
                "-o" + os.path.join(os.path.expanduser(self.SOURCE_DIR), 'main.o')] + user_imports,\
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


    #----------------------------------------------------------------------
    def link(self, filename):
        """Link.

        NB :  "--opt-code-size"   deprecated
              "--use-non-free"    implicit -I and -L options for non-free headers and libs"""

        error = []
        board = self.get_board()
        fichier = open(os.path.join(os.path.expanduser(self.SOURCE_DIR), "stdout"), "w+")

        user_imports = self.get_user_imports_p8()
        #for lib_dir in self.USER_P8_LIBS:
            #user_imports.append("-I" + lib_dir)

        if board.arch == 8:

            if board.bldr == 'boot2':
                sortie = Popen([self.COMPILER_8BIT,
                    "--verbose",\
                    "-mpic16",\
                    "--denable-peeps",\
                    "--obanksel=9",\
                    "--optimize-cmp",\
                    "--optimize-df",\
                    "--no-crt",\
                    "-Wl-s" + os.path.join(self.P8_DIR, 'lkr', board.bldr + '.' + board.proc + '.lkr') + ",-m",\
                    "-p" + board.proc,\
                    "-D" + board.bldr,\
                    "-D" + board.board,\
                    "-DBOARD=\"" + board.board + "\"",\
                    "-DPROC=\"" + board.proc + "\"",\
                    "-DBOOT_VER=2",\
                    "-I" + os.path.join(self.P8_DIR, 'sdcc', 'include', 'pic16'),\
                    "-I" + os.path.join(self.P8_DIR, 'sdcc', 'non-free', 'include', 'pic16'),\
                    "-I" + os.path.join(self.P8_DIR, 'pinguino', 'core'),\
                    "-I" + os.path.join(self.P8_DIR, 'pinguino', 'libraries'),\
                    "-L" + os.path.join(self.P8_DIR, 'sdcc', 'lib', 'pic16'),\
                    "-L" + os.path.join(self.P8_DIR, 'sdcc', 'non-free', 'lib', 'pic16'),\
                    'libio' + board.proc + '.lib',\
                    'libdev' + board.proc + '.lib',\
                    'libc18f.lib',\
                    'libm18f.lib',\
                    'libsdcc.lib',\
                    "-o" + os.path.join(os.path.expanduser(self.SOURCE_DIR), 'main.hex'),\
                    os.path.join(self.P8_DIR, 'obj', 'application_iface.o'),\
                    os.path.join(self.P8_DIR, 'obj', 'boot_iface.o'),\
                    os.path.join(self.P8_DIR, 'obj', 'usb_descriptors.o'),\
                    os.path.join(self.P8_DIR, 'obj', 'crt0ipinguino.o'),\
                    os.path.join(os.path.expanduser(self.SOURCE_DIR), 'main.o')] + user_imports,\
                    stdout=fichier, stderr=STDOUT)

            elif board.bldr == 'boot4':
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
                    "-I" + os.path.join(self.P8_DIR, 'sdcc', 'include', 'pic16'),\
                    "-I" + os.path.join(self.P8_DIR, 'sdcc', 'non-free', 'include', 'pic16'),\
                    "-I" + os.path.join(self.P8_DIR, 'pinguino', 'core'),\
                    "-I" + os.path.join(self.P8_DIR, 'pinguino', 'libraries'),\
                    "-L" + os.path.join(self.P8_DIR, 'sdcc', 'lib', 'pic16'),\
                    "-L" + os.path.join(self.P8_DIR, 'sdcc', 'non-free', 'lib', 'pic16'),\
                    os.path.join(os.path.expanduser(self.SOURCE_DIR), 'main.o'),\
                    'libio' + board.proc + '.lib',\
                    'libdev' + board.proc + '.lib',\
                    'libc18f.lib',\
                    'libm18f.lib',\
                    # link the default run-time module (crt0i.o)
                    # except when "-no-crt" option is used
                    'libsdcc.lib',\
                    "-o" + os.path.join(os.path.expanduser(self.SOURCE_DIR), 'main.hex'),\
                    ] + user_imports,\
                    stdout=fichier, stderr=STDOUT)

            elif board.bldr == 'noboot':
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
                    "-I" + os.path.join(self.P8_DIR, 'sdcc', 'include', 'pic16'),\
                    "-I" + os.path.join(self.P8_DIR, 'sdcc', 'non-free', 'include', 'pic16'),\
                    "-I" + os.path.join(self.P8_DIR, 'pinguino', 'core'),\
                    "-I" + os.path.join(self.P8_DIR, 'pinguino', 'libraries'),\
                    "-L" + os.path.join(self.P8_DIR, 'sdcc', 'lib', 'pic16'),\
                    "-L" + os.path.join(self.P8_DIR, 'sdcc', 'non-free', 'lib', 'pic16'),\
                    'libio' + board.proc + '.lib',\
                    'libdev' + board.proc + '.lib',\
                    'libc18f.lib',\
                    'libm18f.lib',\
                    # link the default run-time module
                    'libsdcc.lib',\
                    "-o" + os.path.join(os.path.expanduser(self.SOURCE_DIR), 'main.hex'),\
                    os.path.join(os.path.expanduser(self.SOURCE_DIR), 'main.o')] + user_imports,\
                    stdout=fichier, stderr=STDOUT)

        else:#if board.arch == 32:

            makefile = os.path.join(os.path.expanduser(self.SOURCE_DIR), 'Makefile32.'+os.getenv("PINGUINO_OS_NAME"))

            user_imports32 = self.get_user_imports_p32()

            sortie = Popen([self.MAKE,
                            "--makefile=" + makefile,
                            "PDEDIR=" + os.path.dirname(filename),
                            "PROC=" + board.proc,
                            "BOARD=" + board.board,
                            "BINDIR=" + self.P32_BIN,  #default /usr/bin
                            "P32DIR=" + self.P32_DIR,  #default /usr/share/pinguino-11.0/p32
                            "SRCDIR=" + self.SOURCE_DIR,

                            "USERLIBS=" + user_imports32,
                         ],

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
            if board.board in ["PIC32_PINGUINO_220", "GENERIC32MX250F128", "GENERIC32MX220F032"]:
                badrecord = ":040000059D0040001A\n"
            else:
                badrecord = ":040000059D006000FA\n"
            if os.path.exists(os.path.join(os.path.expanduser(self.SOURCE_DIR), "main32tmp.hex")):
                fichiersource = open(os.path.join(os.path.expanduser(self.SOURCE_DIR), "main32tmp.hex"), "r")
                fichierdest = open(os.path.join(os.path.expanduser(self.SOURCE_DIR), "main32.hex"), "w+")
                for line in fichiersource:
                    if line != badrecord:
                        fichierdest.writelines(line)
                fichiersource.close()
                fichierdest.close()
                os.remove(os.path.join(os.path.expanduser(self.SOURCE_DIR), "main32tmp.hex"))

        return sortie.poll(), error


    #----------------------------------------------------------------------
    def get_code_size(self):

        board = self.get_board()
        filename = self.get_hex_file()

        codesize = 0
        address_Hi = 0
        memfree = board.memend - board.memstart
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
        return "code size: " + str(codesize) + " / " + str(memfree) + " " + "bytes" + " (" + str(100*codesize/memfree) + "% " + "used"+ ")"



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
