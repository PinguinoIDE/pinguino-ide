#!/usr/bin/env python
#-*- coding: utf-8 -*-

import unittest
import os
import sys

NAME = "Pinguino IDE"
VERSION = "11.0"
SUBVERSION = "beta.1"

os.environ["NAME"] = NAME
os.environ["VERSION"] = VERSION
os.environ["SUBVERSION"] = SUBVERSION
os.environ["PINGUINO_HOME"] = os.path.abspath(sys.path[0])

# For PyInstaller compatibility
if os.path.exists(os.path.abspath("pinguino_data")):
    os.environ["PINGUINO_DATA"] = os.path.abspath("pinguino_data")
else:
    os.environ["PINGUINO_DATA"] = os.getenv("PINGUINO_HOME")

sys.path.append(os.path.join(os.getenv("PINGUINO_DATA"), "qtgui", "resources"))

from qtgui.pinguino_api.pinguino import Pinguino
from qtgui.pinguino_api.pinguino_config import PinguinoConfig
from qtgui.ide.methods.config import Config


Pinguino = Pinguino()

PinguinoConfig.set_environ_vars()
PinguinoConfig.check_user_files()
config = Config()
PinguinoConfig.update_pinguino_paths(config, Pinguino)
PinguinoConfig.update_pinguino_extra_options(config, Pinguino)
PinguinoConfig.update_user_libs(Pinguino)


########################################################################
class TestPinguinoIDE(unittest.TestCase):
    """"""

    #----------------------------------------------------------------------
    def test_delete_comments(self):

        cases = (

            ("//Pinguino Rules!", ""),
            ("/*Pinguino Rules!*/", ""),
            ("/*Pinguino //Rules!*/", ""),
            ("///*Pinguino Rules!*/", ""),


            ([
            "#define LED1 0\n",
            "//#define LED2 1\n",
            "/*\n",
            "1\n",
            "2\n",
            "3\n",
            "*/\n",
            "#include <math.h>\n",
            ],[
            "#define LED1 0\n",
            "\n",
            "\n",
            "\n",
            "\n",
            "\n",
            "\n",
            "#include <math.h>\n",
            ]),

        )

        for case in cases:
            got = Pinguino.remove_comments(case[0])
            expected = case[1]
            self.assertEqual(got, expected,
                             "Remove comments: Failure\ngot: '%s'\nexpected: '%s'"%(got, expected))


    #----------------------------------------------------------------------
    def test_preprocess(self):

        regobject, libinstructions = Pinguino.get_regobject_libinstructions(8)

        for line, expected, include, define in libinstructions:
            got = Pinguino.replace_word(line+"()", regobject, libinstructions)
            self.assertEqual(got, expected+"()",
                             "Preprocess: Failure\ngot: '%s'\nexpected: '%s'"%(got, expected))


        #Test extra words
        cases = (
            ("pinMode", "pinmode"),
            ("CDC.read", "CDCgets"),
            ("GLCD.init", "Init"),
        )

        for line, expected, in cases:
            got = Pinguino.replace_word(line+"()", regobject, libinstructions)
            expected += "()"
            self.assertEqual(got, expected,
                             "Preprocess: Failure\ngot: '%s'\nexpected: '%s'"%(got, expected))




if __name__ == "__main__":

    unittest.main()