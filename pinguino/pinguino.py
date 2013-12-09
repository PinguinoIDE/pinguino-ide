#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import sys

parent_dir = os.path.split(os.path.dirname(__file__))[0]
sys.path.append(os.path.join(parent_dir, "python_modules"))

from python_api.pinguino import PinguinoTools
from python_api import boards as Board
from python_api.boards import boardlist as AllBoards
import argparse
import datetime
import os

########################################################################
class Pinguino(PinguinoTools):
    """"""
    
    #----------------------------------------------------------------------
    def __init__(self):
        """"""
        super(Pinguino, self).__init__()
        
        
    #----------------------------------------------------------------------
    def compile_file(self, file_name):
        """"""
        if os.path.isfile(file_name):
            data = self.verify(file_name)
            self.__compiled__ = data["verified"]
            self.__data__ = data
        else:
            raise Exception, "No such file or directory: %s" % file_name
        
    #----------------------------------------------------------------------
    def upload(self):
        """"""
        data = self.__upload__()
        data = "\n".join(data)
        if data.find("Pinguino not found") != -1 or data.find("Error") != -1:
            if data.find("Error") != -1: data = data[data.find("Error"):]
            return False, data
        return True, data
        
    #----------------------------------------------------------------------
    def set_bootloader(self, boot):
        """"""
        if self.__current_board__.arch == 8:
            self.__current_board__.bldr = boot[0]
            self.__current_board__.memstart = boot[1]
        else:
            raise Exception, "No bootloader options for 32 bits."
            
    #----------------------------------------------------------------------
    def compiled(self):
        """"""
        return self.__compiled__
       
    #----------------------------------------------------------------------
    def get_errors(self):
        """"""
        errors = {}
        errors["preprocess"] = self.__data__["preprocess"]
        errors["compiling"] = self.__data__["compiling"]
        errors["linking"] = self.__data__["linking"]
        return errors
    
    #----------------------------------------------------------------------
    def get_errors_preprocess(self):
        """"""
        errors = self.__data__["preprocess"]
        if not errors: return None

        report = {}
        report["preprocess"] = errors
        return report        
    
    #----------------------------------------------------------------------
    def get_errors_linking(self):
        """"""
        errors = self.__data__["linking"]
        if not errors: return None

        report = {}
        report["linking"] = errors
        return report        
        
        
    #----------------------------------------------------------------------
    def get_errors_compiling_c(self):
        """"""
        errors = self.__data__["compiling"]["c"]
        if not errors: return None
        
        report = {}
        report["line_numbers"] = []
        report["complete_message"] = ""
        report["messages"] = {}
        for error in errors:
            report["line_numbers"] += [int(error["line_number"])]
            report["complete_message"] += error["line_number"] + ": " + error["message"]
            report["messages"][error["line_number"]] = error["message"]
        return report
    
    
    #----------------------------------------------------------------------
    def get_errors_compiling_asm(self):
        """"""
        errors = self.__data__["compiling"]["asm"]
        if not errors: return None
        
        report = {}
        report["error_symbols"] = errors
        return report
    
    #----------------------------------------------------------------------
    def get_result(self):
        """"""
        assert self.__compiled__
        result = {}
        result["time"] = self.__data__["time"]
        result["hex_file"] = self.__data__["hex_file"]
        result["code_size"] = self.__data__["code_size"]
        return result
        
    #----------------------------------------------------------------------
    def get_hex(self):
        """"""
        assert self.__compiled__
        hex_file = file(self.__data__["hex_file"], "r")
        content = hex_file.readlines()
        hex_file.close()
        return hex_file
    
    #----------------------------------------------------------------------
    def build_argparse(self):
        """"""
        parser = argparse.ArgumentParser(description="*** Pinguino API ***")
        parser.add_argument("-v", "--version", dest="version", action="store_true", default=False, help="show Pinguino IDE version and exit")
        parser.add_argument("-a", "--author", dest="author", action="store_true", default=False, help="show authors of this Pinguino IDE version and exit")
        parser.add_argument("-f", "--filename", dest="filename", nargs=1, default=False, help="filename to process")
        parser.add_argument("-l", "--boot", dest="bootloader", nargs=1, default=False, help="set bootloader option")
        parser.add_argument("-x", "--upload", dest="upload", action="store_true", default=False, help="upload code")
        parser.add_argument("-g", "--hex", dest="hex_file", action="store_true", default=False, help="print hex_file")
        
        for board in AllBoards:
            parser.add_argument(board.shortarg, board.longarg, dest="board", const=board, action="store_const", default=False, 
                                help="compile code for " + board.board + " board")
            
        return parser.parse_args()
        
        
        
    
if "__main__" == __name__:
    
    pinguino = Pinguino()
    parser = pinguino.build_argparse()
    
    if parser.version:
        print("\t11.0")
        sys.exit()
        
    if parser.author:
        print("\tJean-Pierre Mandon")
        print("\tRegis Blanchot")
        print("\tYeison Cardona")
        sys.exit()
        
    if parser.board:
        pinguino.set_board(parser.board)
        print("using %s board" % parser.board.name)
        
        if parser.bootloader:
            bootloader = pinguino.dict_boot.get(parser.bootloader[0].lower(), parser.board.bldr)
            pinguino.set_bootloader(bootloader)
        print("using %s bootloader" % pinguino.get_board().bldr)
        
        if not parser.filename:
            print("ERROR: missing filename")
            sys.exit(1)
            
        else:
            filename = parser.filename[0]
            
            fname, extension = os.path.splitext(filename)
            if extension != ".pde":
                print "ERROR: bad file extension, it should be .pde"
                sys.exit()
            del fname, extension
            
            pinguino.compile_file(filename)
            
            if not pinguino.compiled():
                print "\nERROR: no compiled\n"
                
                errors_proprocess = pinguino.get_errors_preprocess()
                if errors_proprocess:
                    for error in errors_proprocess["preprocess"]: print(error)                
                
                errors_c = pinguino.get_errors_compiling_c()
                if errors_c:
                    print(errors_c["complete_message"])
                
                errors_asm = pinguino.get_errors_compiling_asm()
                if errors_asm:
                    for error in errors_asm["error_symbols"]: print(error)
                
                errors_link = pinguino.get_errors_linking()
                if errors_link:
                    for error in errors_link["linking"]: print(error)
                
                sys.exit()
                
            else:
                result = pinguino.get_result()
                print("compilation time: %s" % result["time"])
                print("compiled to: %s" % result["hex_file"])
                
                if parser.hex_file:
                    hex_file = file(result["hex_file"], "r")
                    content_hex = hex_file.readlines()
                    hex_file.close()
                    print("\n" + "*" * 70)
                    print(result["hex_file"])                    
                    print("*" * 70)
                    for line in content_hex: print(line), 
                    print("*" * 70 + "\n")
        
        
            if parser.upload:
                try:
                    uploaded, result = pinguino.upload()
                    if result:
                        print(result)
                except Exception:
                    if pinguino.get_board().arch == 8:
                        print("ERROR: is possible that a parameter is incorrect, try another bootloader option.")
                        print("Boloader options: "),
                        print(", ".join(pinguino.dict_boot.keys()))