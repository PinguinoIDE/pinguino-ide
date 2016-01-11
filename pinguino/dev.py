#!/usr/bin/env python
#-*- coding: utf-8 -*

import os
import sys
from setuptools import setup
from . import version

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    #Python3
    from configparser import RawConfigParser
else:
    #Python2
    from ConfigParser import RawConfigParser


from .qtgui.pinguino_core.pinguino_config import PinguinoConfig
PinguinoConfig.set_environ_vars()


########################################################################
class PinguinoLib:
    """"""

    #----------------------------------------------------------------------
    def __init__(self):
        """Constructor"""

        config_file = os.path.join(sys.path[0], "PINGUINO")

        self.PINGUINO = RawConfigParser()
        self.PINGUINO.readfp(open(config_file, "r"))
        self.PINGUINO = dict(self.PINGUINO.items("PINGUINO"))

        self.BASE_DIR = sys.path[0]



    #----------------------------------------------------------------------
    def get_data_files(self):

        PINGUINOLIBS_DIR = os.path.join(os.getenv("PINGUINO_USERLIBS_PATH"), self.PINGUINO["name"])
        data_files = []
        mainlib = [(PINGUINOLIBS_DIR, "PINGUINO")]
        for dir_ in ["p8", "p32", "pdl", "examples"]:
            path = os.path.join(self.BASE_DIR, dir_)
            if os.path.exists(path):
                for file_ in os.listdir(path):
                    # mainlib.append(os.path.join(dir_, file_))
                    data_files.append((os.path.join(PINGUINOLIBS_DIR, dir_), [os.path.join(dir_, file_)]))

        return data_files


    #----------------------------------------------------------------------
    def pinguino_setup(self, **kwargs):
        """"""
        return setup(install_requires = ["pinguino",
                                         # "pyside",
                                         ],

                    # include_package_data = True,
                    data_files = self.get_data_files(),
                    zip_safe = False,


                    **kwargs
                    )


    #----------------------------------------------------------------------
    @classmethod
    def get_setup_template(cls):
        return """
#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
from pinguino.dev import PinguinoLib

library = PinguinoLib()
PINGUINO = library.PINGUINO

library.pinguino_setup(

    name = "pinguino-{name}".format(**PINGUINO),
    version = PINGUINO["version"],
    description = PINGUINO["description"],

    author = PINGUINO["author"],
    author_email = PINGUINO["author_email"],
    maintainer = "",
    maintainer_email = "",

    url = PINGUINO["url"],
    download_url = "",

    license = "",
    keywords = "microchip, electronic, prototyping, pinguino",

    classifiers=[#list of classifiers in https://pypi.python.org/pypi?:action=list_classifiers

                 ],
)"""


    #----------------------------------------------------------------------
    @classmethod
    def get_manifest_template(cls):
        return """
recursive-include p8 *
recursive-include p32 *
recursive-include pdl *
recursive-include examples *
recursive-exclude * __pycache__
recursive-exclude * *.py[co]"""

