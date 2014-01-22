#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

LANGUAGES = (
            ("Spanish", "es"),
            ("French", "fr"),
            #...
            )

EXCLUDE = ("__init__.py", "__pycache__")

pylupdate4 = "pylupdate4 -noobsolete %s -ts translations/pinguino_%s.ts"  #%(files, lang)

files = ["pinguino.py"]
for root, dirnames, filenames in os.walk("qtgui"):
    if root in EXCLUDE: continue
    filenames = filter(lambda filename: filename.endswith(".py") or filename.endswith(".ui"), filenames)
    for filename in filenames:
        if filename in EXCLUDE: continue
        files.append(os.path.join(root, filename))
                
if not os.path.exists("translations"):
    os.mkdir("translations")

for language, ext in LANGUAGES:
    os.system(pylupdate4%(" ".join(files), ext))
