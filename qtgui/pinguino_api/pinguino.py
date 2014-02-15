#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import shutil
import argparse
import re
import codecs

from .pinguino_tools import PinguinoTools
from .boards import boardlist as AllBoards
#from ..ide.methods.constants import NAME

########################################################################
class Pinguino(PinguinoTools):
    
    
    #----------------------------------------------------------------------
    def __init__(self):
        
        super(Pinguino, self).__init__()
        
        
    #----------------------------------------------------------------------
    def compile_file(self, file_name):
        
        if os.path.isfile(file_name):
            data = self.verify(file_name)
            self.__compiled__ = data["verified"]
            self.__data__ = data
        else:
            raise Exception, "No such file or directory: %s" % file_name
        
        
    #----------------------------------------------------------------------
    def compile_string(self, code):
        
        temp_dir = os.path.join(os.getenv("PINGUINO_USER_PATH"), "temp")
        if os.path.exists(temp_dir):
            shutil.rmtree(temp_dir)
        os.mkdir(temp_dir)
        temp_file = os.path.join(temp_dir, "temp_filename.pde")
        file_ = codecs.open(temp_file, "w", "utf-8")
        file_.write(code)
        file_.close()
        self.compile_file(temp_file)
        
        
    #----------------------------------------------------------------------
    def upload(self):
        
        data = self.__upload__()
        data = "\n".join(data)
        if data.find("Pinguino not found") != -1 or data.find("Error") != -1:
            if data.find("Error") != -1: data = data[data.find("Error"):]
            return False, data
        return True, data
        
    #----------------------------------------------------------------------
    def set_bootloader(self, boot):
        
        if self.__current_board__.arch == 8:
            self.__current_board__.bldr = boot[0]
            self.__current_board__.memstart = boot[1]
        else:
            raise Exception, "No bootloader options for 32 bits."
            
    #----------------------------------------------------------------------
    def compiled(self):
        
        return self.__compiled__
       
    #----------------------------------------------------------------------
    def get_errors(self):
        
        errors = {}
        errors["preprocess"] = self.__data__["preprocess"]
        errors["compiling"] = self.__data__["compiling"]
        errors["linking"] = self.__data__["linking"]
        return errors
    
    #----------------------------------------------------------------------
    def get_errors_preprocess(self):
        
        errors = self.__data__["preprocess"]
        if not errors: return None

        report = {}
        report["preprocess"] = errors
        return report        
    
    #----------------------------------------------------------------------
    def get_errors_linking(self):
        
        errors = self.__data__["linking"]
        if not errors: return None

        report = {}
        report["linking"] = errors
        report["line_numbers"] = []
        for error in errors:
            match = re.match(".*:([\d]+):[\d]+: error.*", error)
            if not match is None: report["line_numbers"] += [int(match.groups()[0])]
        return report        
        
        
    #----------------------------------------------------------------------
    def get_errors_compiling_c(self):
        
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
        
        errors = self.__data__["compiling"]["asm"]
        if not errors: return None
        
        report = {}
        report["error_symbols"] = errors
        return report
    
    #----------------------------------------------------------------------
    def get_result(self):
        
        assert self.__compiled__
        result = {}
        result["time"] = self.__data__["time"]
        result["hex_file"] = self.__data__["hex_file"]
        result["code_size"] = self.__data__["code_size"]
        return result
        
    #----------------------------------------------------------------------
    def get_hex(self):
        assert self.__compiled__
        hex_file = open(self.__data__["hex_file"], "r")
        #content = hex_file.readlines()
        hex_file.close()
        return hex_file
    
    #----------------------------------------------------------------------
    def build_argparse(self):
        
        parser = argparse.ArgumentParser(description="*** %s ***"%os.getenv("NAME"))
        parser.add_argument("-v", "--version", dest="version", action="store_true", default=False, help="show %s version and exit"%os.getenv("NAME"))
        parser.add_argument("-a", "--author", dest="author", action="store_true", default=False, help="show authors of this %s version and exit"%os.getenv("NAME"))
        parser.add_argument("-f", "--filename", dest="filename", nargs=1, default=False, help="filename to process")
        parser.add_argument("-l", "--boot", dest="bootloader", nargs=1, default=False, help="set bootloader option")
        parser.add_argument("-x", "--upload", dest="upload", action="store_true", default=False, help="upload code")
        parser.add_argument("-g", "--hex", dest="hex_file", action="store_true", default=False, help="print hex_file")
        
        for board in AllBoards:
            parser.add_argument(board.shortarg, board.longarg, dest="board", const=board, action="store_const", default=False, 
                                help="compile code for " + board.board + " board")
            
        return parser.parse_args()
    