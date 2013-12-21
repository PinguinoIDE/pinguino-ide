#!/usr/bin/env python
#-*- coding: utf-8 -*-

import time
import os

from ...ide.helpers.constants import IDE_HOME_DIR, TAB_NAME, NAME, VERSION

FONTS_DIR = os.path.join(IDE_HOME_DIR, "qtgui", "resources", "fonts")


INTRO_CODE = lambda :"""//------------------------------------------------------------------
//  Pinguino source code generated automatically.
// 
//  Created: %s
//      by: %s %s
// 
//  WARNING! All changes made in this file will be lost!
//------------------------------------------------------------------
"""%(time.asctime(), NAME, VERSION)

