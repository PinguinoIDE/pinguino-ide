#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
from setuptools import setup, find_packages

if os.name in ["posix", "os2"]: #GNU/Linux and MacOS
    os_kwargs = {"data_files": [
                                ("/usr/share/applications", ["pinguino12.desktop"]),
                                ("/usr/share/icons/hicolor/256x256", ["pinguino/qtgui/resources/art/pinguino_logo-256x256.png"]),
                                ("/usr/share/icons/hicolor/256x256", ["pinguino/qtgui/resources/art/pinguino_logo_background_blue-256x256.png"]),
                                ],
                }

# elif os.name == "os2":  #Mac OS X
    # os_kwargs = {}

elif os.name == "nt":  #Windows
    os_kwargs = {"data_files": [
                                # ("C:\pinguino", ["pinguino12.desktop"]),
                                ("C:\pinguino\icons", ["pinguino/qtgui/resources/art/pinguino_logo-256x256.ico"]),
                                ("C:\pinguino\icons", ["pinguino/qtgui/resources/art/pinguino_logo_background_blue-256x256.ico"]),
                                ],
                }

from pinguino import version

setup(name = "pinguino",
      version = "{MAJOR}.{MINOR}.{PATCH}".format(version.MAJOR, version.MINOR, version.PATCH),
      description = "Open Hardware Electronics Prototyping Platform, Open Source Integrated Development Environment (IDE)",
      description_file = "README.rst",

      author = "",
      author_email = "",
      maintainer = "Yeison Cardona",
      maintainer_email = "yeisoneng@gmail.com",

      url = "http://www.pinguino.cc/",
      download_url = "",

      license = "GPLv2",
      keywords = "microchip, electronic, prototyping, IDE",

      classifiers=[#list of classifiers in https://pypi.python.org/pypi?:action=list_classifiers
                   "Environment :: X11 Applications :: Qt",
                   "Intended Audience :: Developers",
                   "License :: OSI Approved :: GNU General Public License v2 (GPLv2)",
                   "Operating System :: POSIX :: Linux",
                   "Operating System :: Microsoft :: Windows",
                   "Operating System :: MacOS",
                   "Programming Language :: Python",
                   "Topic :: Software Development",
                   "Topic :: Software Development :: Code Generators",
                   "Topic :: Software Development :: Compilers",
                   "Topic :: Software Development :: Debuggers",
                   ],

      packages = find_packages(),
      include_package_data = True,

      install_requires = [
                          #"gitpython",
                          #"hgapi",
                          "pyusb==1.0.0b2",
                          #"setuptools",
                          #"wheel"
                          #"pyside",
                          ],

      extras_require = {
          "gui":  ["pyside"]
      },

      scripts = [
                 "cmd/pinguinoide",			# pinguino ide launcher
                 "cmd/pinguinoide.pyw",		# pinguino ide launcher with extension for windows
                 "cmd/pinguino-cmd",		# pinguino command line
                 "cmd/pinguino-cmd.py",		# pinguino command line with extension for windows
                 "cmd/pinguino-reset",
                 "cmd/pinguino-reset.py",
                 "cmd/pinguino-test",
                 "cmd/pinguino-test.py",
                 ],

      zip_safe = False,

      **os_kwargs
      )
