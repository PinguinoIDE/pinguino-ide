#!/usr/bin/python

import sys
import os
import shutil

root = os.path.dirname(__file__)
files = os.listdir(".")
files = filter(lambda f:f.endswith(".svg"), files)

for size in ["16", "24", "32", "48"]:
    path = "../{}".format(size)

    if not os.path.exists(path):
        os.mkdir(path)
    else:
        shutil.rmtree(path)
        os.mkdir(path)


    for file in files:

        filename_svg = file
        filename_png = os.path.join(path, file.replace(".svg", ".png"))

        command = "inkscape {filename_svg} -e={filename_png} -C -w={size} -h={size}".format(**locals())

        print(command)
        os.system(command)