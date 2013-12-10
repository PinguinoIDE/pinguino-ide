#!/usr/bin/env python
#-*- coding: utf-8 -*-

import time
import os

from ...ide.helpers.constants import IDE_HOME_DIR, TAB_NAME, NAME, VERSION

FONTS_DIR = os.path.join(IDE_HOME_DIR, "qtgui", "resources", "fonts")


INTRO_CODE = lambda :"""/*
Pinguino source code generated automatically.
 
Created: %s
     by: %s %s

WARNING! All changes made in this file will be lost!
*/
"""%(time.asctime(), NAME, VERSION)

INTRO_CODE_PY = lambda :"""#!/usr/bin/env python
#-*- coding: utf-8 -*-

# Python source code generated automatically.
# 
# Created: %s
#      by: %s %s
#
# WARNING! All changes made in this file will be lost!
"""%(time.asctime(), NAME, VERSION)

GLOBAL_CODE_PYNGUINO = lambda :"""

from math import *

from pynguino import *

pinguino = Pinguino()

pinMode = pinguino.pinMode
digitalRead = pinguino.digitalRead
digitalWrite = pinguino.digitalWrite
analogRead = pinguino.analogRead
analogWrite = pinguino.analogWrite
delay = pinguino.delay
toggle = pinguino.toggle
"""


GLOBAL_CODE_PYTHON = lambda :"""from math import *"""

GLOBAL_CODE = lambda :"""int __i__ = 0;"""

FORMAT = lambda :"""#
# %s %s
#
## Format:
## ['Name', [posX, posY], [['ID-widget', [...]],], 'Type-Name', 'ID', ['To',], 'From', ['Inside',], ['Nested',]]
#
# %s
# 
""" %(NAME, VERSION, time.asctime())