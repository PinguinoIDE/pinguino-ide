#!/bin/sh

#cd ~/Dvpt/pinguino/git-local/pinguino-ide
python3 -m pip install --upgrade pip pyside pyusb
python3 -m pip install --upgrade wheel beautifulsoup4 setuptools requests
# pybluez needs Windows Platform SDK
# bluepy is for Linux only and then failed to compile on Windows
# python -m pip install --upgrade pybluez
python3 pinguino-ide.py
