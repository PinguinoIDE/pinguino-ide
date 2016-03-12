#!/usr/bin/env python

from . import version
from .qtgui.pinguino_core import boards
from .qtgui.pinguino_core.config import Config
from .qtgui.pinguino_core.pinguino import Pinguino
from .qtgui.pinguino_core.pinguino_config import PinguinoConfig


#----------------------------------------------------------------------
def lazy(pinguino):
    """"""
    PinguinoConfig.set_environ_vars()
    PinguinoConfig.check_user_files()
    config = Config()
    PinguinoConfig.update_pinguino_paths(config, pinguino)
    PinguinoConfig.update_pinguino_extra_options(config, pinguino)
    PinguinoConfig.update_user_libs(pinguino)
