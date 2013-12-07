#!/usr/bin/env python
#-*- coding: utf-8 -*-

import inspect
import os, sys, pickle

allBlocks = {}


#----------------------------------------------------------------------
def getName(block):
    name = block.__name__()
    if name != "<lambda>": return name
    else:
        source = inspect.getsource(block)
        return source[:source.find("=")].replace(" ", "")
    
#----------------------------------------------------------------------
def extractBlocks(mod):
    global allBlocks
    alls = inspect.getmembers(mod())
    for block in alls:
        
        if hasattr(block[1], "im_class"):
            if block[0] != "__init__" and block[1].im_class == mod().__init__.im_class:
                allBlocks[block[0]] = block[1]()
                

files = filter(lambda f:f.endswith("py") and f != "__init__.py" and (not f.startswith(".")), os.listdir(os.path.join(sys.path[0], os.path.join(sys.path[0], "qtgui", "gide", "user_bloques"))))

sys.path.append(os.path.join(sys.path[0], "user_bloques"))
for module in files:
    mod = __import__(module.replace(".py", ""))
    extractBlocks(getattr(mod, "KitBlocks"))
    
    
save_dir = os.path.join(sys.path[0], "qtgui", "gide", "user_bloques", "saved")
if not os.path.isdir(save_dir): os.mkdir(save_dir)
        
files = filter(lambda f:not f.startswith("."), os.listdir(save_dir))

for f in files:
    file = open(os.path.join(os.path.join(sys.path[0], "qtgui", "gide", "user_bloques"), "saved", f), "r")
    allBlocks[f] = pickle.load(file)