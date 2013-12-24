#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

#----------------------------------------------------------------------
def go_to(back, path):
    for dot in back: path = os.path.dirname(path)
    return path

HOME_DIR = go_to("..", os.path.abspath(__file__))

#HOME_DIR = os.path.split(os.path.dirname(os.path.realpath(__file__)))[0]
SOURCE_DIR = os.path.join(HOME_DIR, 'source')
LOCALE_DIR = os.path.join(HOME_DIR, 'locale')
P32_DIR = os.path.join(HOME_DIR, 'p32')
P8_DIR = os.path.join(HOME_DIR, 'p8')
APP_CONFIG = os.path.join(HOME_DIR, '.config')
TEMP_DIR = os.path.join(HOME_DIR, '.temp')
STDOUT_FILE = os.path.join(HOME_DIR, 'source', "stdout")

COMPILERS_DIR = os.path.join(go_to("....", os.path.abspath(__file__)), "pinguino-compilers")