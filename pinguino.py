#! /usr/bin/python
#-*- coding: utf-8 -*-

import sys
import os

sys.path.append(os.path.join("qtgui", "resources"))

python_path_modules = os.path.join("python_modules")
if os.path.isdir(python_path_modules):
    sys.path.append(python_path_modules)

from qtgui.ide import PinguinoIDE
from PySide.QtGui import QApplication

def main(argv):
    app = QApplication(argv)
    frame = PinguinoIDE()
    frame.show()
    app.exec_()

if __name__ == "__main__":
    if len(sys.argv) == 1:
        main(sys.argv)
    
    else:  #command line
        
        from qtgui.pinguino_api.pinguino import Pinguino
        from qtgui.ide.helpers.constants import VERSION
        
        pinguino = Pinguino()
        parser = pinguino.build_argparse()
        
        if parser.version:
            print("\t" + VERSION)
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
    