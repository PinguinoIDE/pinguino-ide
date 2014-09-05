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

from qtgui.pinguino_api.pinguino import Pinguino, AllBoards
from qtgui.pinguino_api.pinguino_config import PinguinoConfig
from qtgui.ide.methods.config import Config


Pinguino = Pinguino()

PinguinoConfig.set_environ_vars()
PinguinoConfig.check_user_files()
config = Config()
PinguinoConfig.update_pinguino_paths(config, Pinguino)
PinguinoConfig.update_pinguino_extra_options(config, Pinguino)
PinguinoConfig.update_user_libs(Pinguino)

Pinguino.set_os_variables()


########################################################################
class TestEnvironment(unittest.TestCase):
    """"""

    #----------------------------------------------------------------------
    def test_python_version(self):

        self.assertTrue((3, 0) > sys.version_info > (2, 6), "Environment: Python version recommended > 2.6 and < 3.0")



########################################################################
class TestPreprocess(unittest.TestCase):
    """"""

    #----------------------------------------------------------------------
    def test_delete_comments(self):

        cases = (

            ("//Pinguino Rules!", ""),
            ("/*Pinguino Rules!*/", ""),
            ("/*Pinguino //Rules!*/", ""),
            ("///*Pinguino Rules!*/", ""),


            ("\n".join([
            "#define LED1 0\n",
            "//#define LED2 1\n",
            "/*\n",
            "1\n",
            "2\n",
            "3\n",
            "*/\n",
            "#include <math.h>\n",
            ]),
            "\n".join([
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

        )

        for case in cases:
            got = Pinguino.remove_comments(case[0])
            expected = case[1]
            self.assertMultiLineEqual(got, expected,
                                      "Remove comments: Failure\ngot: '%s'\nexpected: '%s'"%(got, expected))


    #----------------------------------------------------------------------
    def test_preprocess(self):

        libinstructions = Pinguino.get_regobject_libinstructions(8)

        for line, expected, include, define, regex in libinstructions:
            got = Pinguino.replace_word(line+"()", libinstructions)
            self.assertEqual(got, expected+"()",
                             "Preprocess: Failure\ngot: '%s'\nexpected: '%s'"%(got, expected))


        #Test extra words
        cases = (
            ("pinMode", "pinmode"),
            ("CDC.read", "CDCgets"),
            ("GLCD.init", "Init"),
        )

        for line, expected, in cases:
            got = Pinguino.replace_word(line+"()", libinstructions)
            expected += "()"
            self.assertEqual(got, expected,
                             "Preprocess: Failure\ngot: '%s'\nexpected: '%s'"%(got, expected))




########################################################################
class TestCompilation(unittest.TestCase):
    """"""

    #----------------------------------------------------------------------
    @classmethod
    def compilation(cls, board):
        """"""

        code = "void setup(){}; void loop(){}"

        def inter(self):

            try: Pinguino.set_board(board)
            except BaseException, msg:
                raise BaseException("Compilation: imposible set board %s\n%s" % (board.name, str(msg)))

            if board.arch == 8:
                for key in Pinguino.dict_boot.keys():
                    boot = Pinguino.dict_boot[key]
                    Pinguino.set_bootloader(boot)
                    try: Pinguino.compile_string(code)
                    except BaseException, msg:
                        self.fail("Compilation: impossible compile for %s, %sbits, boot:%s\n%s" % (board.name, board.arch, str(msg), key))

            if board.arch == 32:
                try: Pinguino.compile_string(code)
                except BaseException, msg:
                    #raise BaseException("Compilation: impossible compile for %s, %sbits\n%s" % (board.name, board.arch, str(msg)))
                    self.fail("Compilation: impossible compile for %s, %sbits\n%s" % (board.name, board.arch, str(msg)))

        return inter


for board in AllBoards:
    test_name = "test_compile_%s" % str(board.name.replace(" ", "_").replace(".", "_"))
    setattr(TestCompilation, test_name, TestCompilation.compilation(board))


if __name__ == "__main__":
    unittest.main()