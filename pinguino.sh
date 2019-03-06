#!/bin/sh

PYTHON_VERSION=3

cd ~/Dvpt/pinguino/pinguino-ide

python${PYTHON_VERSION} -m pip install --upgrade --user pip
python${PYTHON_VERSION} -m pip install --upgrade --user pyside2
python${PYTHON_VERSION} -m pip install --upgrade --user pyusb
python${PYTHON_VERSION} -m pip install --upgrade --user wheel
python${PYTHON_VERSION} -m pip install --upgrade --user beautifulsoup4
python${PYTHON_VERSION} -m pip install --upgrade --user setuptools
python${PYTHON_VERSION} -m pip install --upgrade --user requests
# pybluez needs Windows Platform SDK
# bluepy is for Linux only and then failed to compile on Windows
# python${PYTHON_VERSION} -m pip install --upgrade --user pybluez

python${PYTHON_VERSION} pinguino-ide.py
