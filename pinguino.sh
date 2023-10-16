#!/bin/sh
#
# Launch Pinguino's IDE
echo "Launch Pinguino's IDE ..."
cd ~/Pinguino/v13/pinguino-ide/
python3 -m pip install pipenv
python3 -m pipenv install
python3 -m pipenv run python pinguino-ide.py
