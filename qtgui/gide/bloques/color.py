#!/usr/bin/env python
#-*- coding: utf-8 -*-

import re
import os
import sys

extractFrom = lambda dir: [dir + path for path in os.listdir(dir) if path.endswith(".svg") and not path.startswith("__")]

linear = extractFrom(os.path.join(sys.path[0], "qtgui/gide/bloques/linear/arte/"))
inside = extractFrom(os.path.join(sys.path[0], "qtgui/gide/bloques/inside/arte/in/"))
insidebool = extractFrom(os.path.join(sys.path[0], "qtgui/gide/bloques/inside/arte/bool/"))
nested = extractFrom(os.path.join(sys.path[0], "qtgui/gide/bloques/nested/arte/nested/"))
function = extractFrom(os.path.join(sys.path[0], "qtgui/gide/bloques/nested/arte/function/"))
frame = extractFrom(os.path.join(sys.path[0], "qtgui/gide/bloques/frame_edit/arte/"))

#----------------------------------------------------------------------
def setColor(color, block):
    """"""
    block = eval(block)
    for parte in block:
        file = open(parte, "r")
        lines = file.readlines()
        file.close()
        
        file = open(parte, "w")
        for line in lines:
            regex = re.findall(".*fill:#(......)", line)
            if len(regex) > 0 and regex[0] != "ffffff":
                file.write(line.replace(regex[0], color))
            else: file.write(line)
        file.close()
    #delay(0.5)