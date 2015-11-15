#!/usr/bin/env python
#-*- coding: utf-8 -*-

import unittest
import os
import sys

from . import version
sys.path.append(os.path.join(os.getenv("PINGUINO_LIB"), "qtgui", "resources"))

from .qtgui.pinguino_core.pinguino import Pinguino, AllBoards
from .qtgui.pinguino_core.pinguino_config import PinguinoConfig
from .qtgui.pinguino_core.config import Config

Pinguino = Pinguino()
PinguinoConfig.set_environ_vars()
PinguinoConfig.check_user_files()
config = Config()
PinguinoConfig.update_pinguino_paths(config, Pinguino)
PinguinoConfig.update_pinguino_extra_options(config, Pinguino)
PinguinoConfig.update_user_libs(Pinguino)


########################################################################
class TestPreprocess(unittest.TestCase):

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
                                      "Remove comments: Failure\ngot: '{}'\nexpected: '{}'".format(got, expected))


    #----------------------------------------------------------------------
    @classmethod
    def preprocess(cls, lib, libinstructions):

        def inter(self):
            got, d = Pinguino.replace_word(lib["pinguino"], libinstructions)
            self.assertEqual(got, lib["c"],
                             "Preprocess: Failure\ngot: '{}'\nexpected: '{}'".format(got, lib["c"]))
        return inter



########################################################################
class TestBareMinumumCompilation(unittest.TestCase):

    #----------------------------------------------------------------------
    @classmethod
    def compilation(cls, board):

        code = "void setup(){}; void loop(){}"

        def inter(self):

            try:
                Pinguino.set_board(board)
            except BaseException as msg:
                raise BaseException("Compilation: imposible set board {}\n{}".format(board.name, str(msg)))

            if board.arch == 8:
                for key in Pinguino.dict_boot.keys():
                    boot = Pinguino.dict_boot[key]
                    Pinguino.set_bootloader(*boot)
                    try:
                        Pinguino.compile_string(code)
                    except BaseException as msg:
                        self.fail("Compilation: impossible compile for {}, {}-bit, boot:{}\n{}".format(board.name, board.arch, key, str(msg)))

            if board.arch == 32:
                try:
                    Pinguino.compile_string(code)
                except BaseException as msg:
                    self.fail("Compilation: impossible compile for {}, {}-bit\n{}".format(board.name, board.arch, str(msg)))

        return inter


libs8 = Pinguino.get_regobject_libinstructions(8)
for lib in libs8:
    test_name = "test_preprocess_8_{}".format(lib["pinguino"].replace(".", "_"))
    setattr(TestPreprocess, test_name, TestPreprocess.preprocess(lib, libs8))


libs32 = Pinguino.get_regobject_libinstructions(8)
for lib in libs32:
    test_name = "test_preprocess_32_{}".format(lib["pinguino"].replace(".", "-"))
    setattr(TestPreprocess, test_name, TestPreprocess.preprocess(lib, libs32))


for board in AllBoards:
    test_name = "test_compile_{}".format(board.name.replace(" ", "_").replace(".", "_"))
    setattr(TestBareMinumumCompilation, test_name, TestBareMinumumCompilation.compilation(board))


if __name__ == "__main__":
    unittest.main()
