#!/usr/bin/python2


import os


root_ = os.getcwd()

paths = ["frame_edit/arte",
         "inside/arte/bool",
         "inside/arte/in",
         "linear/arte",
         "nested/arte/function",
         "nested/arte/nested",
         "widgets/arte"]

for path in paths:
    root = os.path.join(root_, path)
    os.chdir(root)
    files = os.listdir(root)
    files = filter(lambda f:f.endswith(".svg"), files)

    for file in files:
        if os.path.exists(file.replace(".svg", ".png")):
            os.remove(file.replace(".svg", ".png"))
        os.system("convert -background transparent %s %s"%(file, file.replace(".svg", ".png")))
