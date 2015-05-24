#! /usr/bin/python2
#-*- coding: utf-8 -*-

from version import NAME, SUBVERSION, VERSION

"""-------------------------------------------------------------------------
    Pinguino IDE

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
-------------------------------------------------------------------------"""

import sys
import os

# if sys.version_info >= (3, ):
    # #Python3
    # os.environ["PINGUINO_PYTHON"] = "3"
# else:
    # #Python2
    # os.environ["PINGUINO_PYTHON"] = "2"


# # Python3 compatibility
# if os.getenv("PINGUINO_PYTHON") is "3":
    # import imp
    # imp.reload(sys)
# else:
    # reload(sys)
    # sys.setdefaultencoding("utf-8")

# os.environ["PINGUINO_NAME"] = NAME
# os.environ["PINGUINO_VERSION"] = VERSION
# os.environ["PINGUINO_SUBVERSION"] = SUBVERSION
# os.environ["PINGUINO_FULLNAME"] = " ".join([NAME, VERSION, SUBVERSION])
# os.environ["PINGUINO_HOME"] = os.path.abspath(sys.path[0])


# # For PyInstaller compatibility
# if os.path.exists(os.path.abspath("pinguino_data")):
    # os.environ["PINGUINO_DATA"] = os.path.abspath("pinguino_data")
# else:
    # os.environ["PINGUINO_DATA"] = os.getenv("PINGUINO_HOME")


class bcolors:
    Black = "\033[0;30m"
    Red = "\033[0;31m"
    Green = "\033[0;32m"
    Yellow = "\033[0;33m"
    Blue = "\033[0;34m"
    Purple = "\033[0;35m"
    Cyan = "\033[0;36m"
    LightGray = "\033[0;37m"
    ENDC = "\033[0m"

import argparse
from qtgui.pinguino_api.boards import boardlist

from qtgui.pinguino_api.pinguino import Pinguino
from qtgui.pinguino_api.pinguino_config import PinguinoConfig
from qtgui.pinguino_api.config import Config

pinguino = Pinguino()
PinguinoConfig.set_environ_vars()
PinguinoConfig.check_user_files()
config = Config()
PinguinoConfig.update_pinguino_paths(config, Pinguino)
PinguinoConfig.update_pinguino_extra_options(config, Pinguino)
PinguinoConfig.update_user_libs(pinguino)

def printb(text, color):
    print(color + text + bcolors.ENDC)

#----------------------------------------------------------------------
def build_argparse():

    parser = argparse.ArgumentParser(description="*** %s:Command line ***"%os.getenv("PINGUINO_NAME"))

    #command line args
    parser.add_argument("-v", "--version", dest="version", action="store_true", default=False, help="show %s version and exit"%os.getenv("PINGUINO_NAME"))
    parser.add_argument("-a", "--author", dest="author", action="store_true", default=False, help="show authors of this %s version and exit"%os.getenv("PINGUINO_NAME"))
    parser.add_argument("-f", "--filename", dest="filename", nargs=1, default=False, help="filename to process")
    parser.add_argument("-l", "--boot", dest="bootloader", nargs=1, default=False, help="set bootloader option")
    parser.add_argument("-x", "--upload", dest="upload", action="store_true", default=False, help="upload code")
    parser.add_argument("-g", "--hex", dest="hex_file", action="store_true", default=False, help="print hex_file")

    for board in boardlist:
        parser.add_argument(board.shortarg, board.longarg, dest="board", const=board, action="store_const", default=False,
                            help="compile code for " + board.board + " board")

    parsed_c, others_c = parser.parse_known_args()

    if others_c:
        parser.print_help()
        sys.exit()
    else: return parsed_c

parser = build_argparse()

if parser.version:
    printb("\t" + VERSION, bcolors.Green)
    sys.exit()

if parser.author:
    printb("\tJean-Pierre Mandon", bcolors.Green)
    printb("\tRegis Blanchot", bcolors.Green)
    printb("\tYeison Cardona", bcolors.Green)
    sys.exit()

if parser.board:
    pinguino.set_board(parser.board)
    printb("using %s board" % parser.board.name, bcolors.Green)

    if parser.bootloader:
        bootloader = pinguino.dict_boot.get(parser.bootloader[0].lower(), parser.board.bldr)
        pinguino.set_bootloader(bootloader)
    printb("using %s bootloader" % pinguino.get_board().bldr, bcolors.Green)

    if not parser.filename:
        printb("ERROR: missing filename", bcolors.Red)
        sys.exit(1)

    else:
        filename = parser.filename[0]

        fname, extension = os.path.splitext(filename)
        if extension != ".pde":
            printb("ERROR: bad file extension, it should be .pde", bcolors.Red)
            sys.exit()
        del fname, extension

        pinguino.compile_file(filename)

        if not pinguino.compiled():
            printb("\nERROR: no compiled\n", bcolors.Red)

            errors_proprocess = pinguino.get_errors_preprocess()
            if errors_proprocess:
                for error in errors_proprocess["preprocess"]: printb(error, bcolors.Red)

            errors_c = pinguino.get_errors_compiling_c()
            if errors_c:
                printb(errors_c["complete_message"], bcolors.Red)

            errors_asm = pinguino.get_errors_compiling_asm()
            if errors_asm:
                for error in errors_asm["error_symbols"]: printb(error, bcolors.Red)

            errors_link = pinguino.get_errors_linking()
            if errors_link:
                for error in errors_link["linking"]: printb(error, bcolors.Red)

            sys.exit()

        else:
            result = pinguino.get_result()
            printb("compilation time: %s" % result["time"], bcolors.Yellow)
            printb("compiled to: %s" % result["hex_file"], bcolors.Yellow)

            if parser.hex_file:
                hex_file = open(result["hex_file"], "r")
                content_hex = hex_file.readlines()
                hex_file.close()
                printb("\n" + "*" * 70, bcolors.Cyan)
                printb(result["hex_file"], bcolors.Cyan)
                printb("*" * 70, bcolors.Cyan)
                for line in content_hex: printb(line, bcolors.Cyan),
                printb("*" * 70 + "\n", bcolors.Cyan)

        if parser.upload:
            try:
                uploaded, result = pinguino.upload()
                if result:
                    printb(result, bcolors.Green)
            except:
                if pinguino.get_board().arch == 8:
                    printb("ERROR: bootloader option might be incorrect.", bcolors.Red)
                    printb("Bootloader options: ", bcolors.Green),
                    printb(", ".join(pinguino.dict_boot.keys()), bcolors.Green)
