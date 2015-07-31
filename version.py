#!/usr/bin/env python
#-*- coding: utf-8 -*-

NAME = "Pinguino IDE"
VERSION = "12"
SUBVERSION = "0"

"""-------------------------------------------------------------------------
    Pinguino IDE

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
-------------------------------------------------------------------------"""

import os, sys

if sys.version_info >= (3, ):
    #Python3
    os.environ["PINGUINO_PYTHON"] = "3"
else:
    #Python2
    os.environ["PINGUINO_PYTHON"] = "2"

# Python3 compatibility
if os.getenv("PINGUINO_PYTHON") is "3":
    import imp
    imp.reload(sys)
else:
    reload(sys)
    sys.setdefaultencoding("utf-8")

os.environ["PINGUINO_NAME"] = NAME
os.environ["PINGUINO_VERSION"] = VERSION
os.environ["PINGUINO_SUBVERSION"] = SUBVERSION
if SUBVERSION != "0":
    os.environ["PINGUINO_FULLNAME"] = "{PINGUINO_NAME} {PINGUINO_VERSION}.{PINGUINO_SUBVERSION}".format(**os.environ)
else:
    os.environ["PINGUINO_FULLNAME"] = "{PINGUINO_NAME} {PINGUINO_VERSION}".format(**os.environ)
os.environ["PINGUINO_HOME"] = os.path.abspath(sys.path[0])


# For PyInstaller compatibility
if os.path.exists(os.path.abspath("pinguino_data")):
    os.environ["PINGUINO_DATA"] = os.path.abspath("pinguino_data")
else:
    os.environ["PINGUINO_DATA"] = os.getenv("PINGUINO_HOME")
