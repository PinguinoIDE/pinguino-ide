#!/usr/bin/env python
#-*- coding: utf-8 -*-

import constructor
all_sets = {}

#----------------------------------------------------------------------
def extractBlocks(model):
    """"""
    blocks = filter(lambda x:not x.startswith("__"), model.__dict__.keys())
    instanced = model()
    for key in blocks: all_sets[key] = getattr(instanced, key)()
    

from pinguino import Pinguino
extractBlocks(Pinguino)

from math_ import Math_
extractBlocks(Math_)

from control import Control
extractBlocks(Control)

from widgets import Widgets
extractBlocks(Widgets)

from .invisible import NoVisible
extractBlocks(NoVisible)
 
from user import User
extractBlocks(User)
