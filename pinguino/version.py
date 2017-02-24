#! /usr/bin/python
#-*- coding: utf-8 -*-

"""
Semantic Versioning 2.0.0 - http://semver.org/

Given a version number MAJOR.MINOR.PATCH, increment the:

    MAJOR version when you make incompatible API changes,
    MINOR version when you add functionality in a backwards-compatible manner, and
    PATCH version when you make backwards-compatible bug fixes.

Additional labels for pre-release and build metadata are available as extensions to the MAJOR.MINOR.PATCH format.
"""

NAME = "Pinguino IDE"

MAJOR = "12"
MINOR = "0"
PATCH = "39"
PRE_RELEASE = "beta.4"

if PRE_RELEASE:
    VERSION = "{MAJOR}.{MINOR}.{PATCH}-{PRE_RELEASE}".format(**locals())
else:
    VERSION = "{MAJOR}.{MINOR}.{PATCH}".format(**locals())


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
os.environ["PINGUINO_FULLNAME"] = "{PINGUINO_NAME} {PINGUINO_VERSION}".format(**os.environ)
os.environ["PINGUINO_LIB"] = os.path.abspath(os.path.dirname(__file__))

