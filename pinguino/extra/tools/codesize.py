#!/usr/bin/env python
#  -*- coding: UTF-8 -*-

"""-----------------------------------------------------------------------------
	codesize
	calculate code size from .hex file
	usage: ./codesize.py filename (without .hex extension)
	2011 - regis blanchot <rblanchot@gmail.com> 

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
	--------------------------------------------------------------------------"""

import sys

codesize = 0

if (len(sys.argv) > 1):
	filename = sys.argv[1]
	fichier = open(filename + ".hex", 'r')
	lines = fichier.readlines()
	for line in lines:
		# count only data record
		if line[7:9:1] == "00":
			codesize = codesize + int(line[1:3:1], 16)
	fichier.close()
	print "code size: " + str(codesize) + " bytes"
else:
	print "No file to proceed"
	print "usage: ./codesize.py filename (without .hex extension)"

