#! /usr/bin/python
#-*- coding: utf-8 -*-

try:
    from version import VERSION
except:
    from .version import VERSION


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

class BColors:
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

try:
    from qtgui.pinguino_core.boards import boardlist
    from qtgui.pinguino_core.pinguino import Pinguino
    from qtgui.pinguino_core.pinguino_config import PinguinoConfig
    from qtgui.pinguino_core.config import Config
except:
    from .qtgui.pinguino_core.boards import boardlist
    from .qtgui.pinguino_core.pinguino import Pinguino
    from .qtgui.pinguino_core.pinguino_config import PinguinoConfig
    from .qtgui.pinguino_core.config import Config

pinguino = Pinguino()
PinguinoConfig.set_environ_vars()
PinguinoConfig.check_user_files()
config = Config()
PinguinoConfig.update_pinguino_paths(config, pinguino)
PinguinoConfig.update_pinguino_extra_options(config, pinguino)
PinguinoConfig.update_user_libs(pinguino)

def printb(text, color):
    print(color + text + BColors.ENDC)

#----------------------------------------------------------------------
def build_argparse():

    parser = argparse.ArgumentParser(description="*** {PINGUINO_NAME}:Command line ***".format(**os.environ))

    #command line args
    parser.add_argument("-v", "--version", dest="version", action="store_true", default=False, help="show {PINGUINO_NAME} version and exit".format(**os.environ))
    parser.add_argument("-a", "--author", dest="author", action="store_true", default=False, help="show authors of the {PINGUINO_NAME} and exit".format(**os.environ))
    parser.add_argument("-f", "--filename", dest="filename", nargs=1, default=False, help="filename to process")
    #parser.add_argument("-8c", "--8bit_compiler", dest="compiler", nargs=1, default=None, help="8-bit compiler: SDCC or XC8")
    parser.add_argument("--sdcc", dest="compiler_sdcc", action="store_true", default=None, help="Set SDCC as 8-bit compiler")
    parser.add_argument("--xc8", dest="compiler_xc8", action="store_true", default=None, help="Set XC8 as 8-bit compiler")
    #parser.add_argument("-l", "--boot", dest="bootloader", nargs=1, default=False, help="set bootloader option")
    parser.add_argument("--icsp", dest="icsp", action="store_true", default=False, help="use ICSP (default use USB)")
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
    printb("\t" + VERSION, BColors.Green)
    sys.exit()

if parser.author:
    printb("\tJean-Pierre Mandon", BColors.Green)
    printb("\tRegis Blanchot", BColors.Green)
    printb("\tYeison Cardona", BColors.Green)
    sys.exit()

if parser.board:
    pinguino.set_board(parser.board)
    printb("using {} board".format(parser.board.name), BColors.Green)

    #if parser.bootloader:
        #bootloader = pinguino.dict_boot.get(parser.bootloader[0].lower(), parser.board.connect)
        #pinguino.set_bootloader(*bootloader)
        #printb("using {} bootloader".format(pinguino.get_board().connect), BColors.Green)

    if parser.icsp:
        #do nothing
        pinguino.set_icsp()  #use ICSP instead of bootloader
        printb("ICSP mode used", BColors.Green)
    else:
        printb("Bootloader {} used".format(pinguino.get_board().connect), BColors.Green)


    if parser.board.arch == 8:
        if parser.compiler_sdcc:
            pinguino.set_8bit_compiler("sdcc")
        elif parser.compiler_xc8:
            pinguino.set_8bit_compiler("xc8")
        else:
            pinguino.set_8bit_compiler("xc8")


    if not parser.filename:
        printb("ERROR: missing filename", BColors.Red)
        sys.exit(1)

    else:
        filename = parser.filename

        if not (len(filename) == 1 and filename[0].endswith(".hex")):

            if len(filename) == 1:
                fname, extension = os.path.splitext(filename[0])
                if extension != ".pde":
                    printb("ERROR: bad file extension, it should be .pde", BColors.Red)
                    sys.exit()
                del fname, extension

            pinguino.compile_file(filename)

            if not pinguino.compiled():
                printb("\nERROR: not compiled\n", BColors.Red)

                ##errors_proprocess = pinguino.get_errors_preprocess()
                ##if errors_proprocess:
                    ##for error in errors_proprocess["preprocess"]: printb(error, BColors.Red)

                #errors_c = pinguino.get_errors_compiling_c()
                #if errors_c:
                    #printb(errors_c["complete_message"], BColors.Red)

                #errors_asm = pinguino.get_errors_compiling_asm()
                #if errors_asm:
                    #for error in errors_asm["error_symbols"]:
                        #printb(error, BColors.Red)

                #errors_link = pinguino.get_errors_linking()
                #if errors_link:
                    #for error in errors_link["linking"]:
                        #printb(error, BColors.Red)

                errors_c = pinguino.get_errors_compiling_c()
                if errors_c:
                    printb("ERROR: {complete_message}".format(**errors_c), BColors.Red)

                errors_asm = pinguino.get_errors_compiling_asm()
                if errors_asm:
                    printb("ERROR: {}".format("\n".join(errors_asm)), BColors.Red)

                errors_linking = pinguino.get_errors_linking()
                if errors_linking:
                    if "linking" in  errors_linking:
                        for error in errors_linking["linking"]:
                            printb("ERROR: {}".format(error), BColors.Red)

                sys.exit()

            else:
                result = pinguino.get_result()
                printb("compilation time: {time}".format(**result), BColors.Yellow)
                printb("compiled to: {hex_file}".format(**result), BColors.Yellow)

                if parser.hex_file:
                    hex_file = open(result["hex_file"], "r")
                    content_hex = hex_file.readlines()
                    hex_file.close()
                    printb("\n" + "*" * 70, BColors.Cyan)
                    printb(result["hex_file"], BColors.Cyan)
                    printb("*" * 70, BColors.Cyan)
                    for line in content_hex: printb(line, BColors.Cyan),
                    printb("*" * 70 + "\n", BColors.Cyan)

                print "OK"
        else:
            pinguino.__hex_file__ = filename[0]

        if parser.upload:
            try:
                uploaded, result = pinguino.upload()
                if result:
                    printb(result, BColors.Green)
            except Exception as err:
                printb(err, BColors.Red)
                #if pinguino.get_board().arch == 8:
                    #printb("ERROR: bootloader option could be incorrect.", BColors.Red)
                    #printb("Bootloader options: ", BColors.Green),
                    #printb(", ".join(pinguino.dict_boot.keys()), BColors.Green)
