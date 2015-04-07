#!/usr/bin/env python
# -*- coding: iso-8859-15 -*-

"""---------------------------------------------------------------------
    Pinguino Boards List

    author:			Regis Blanchot
    contact:		rblanchot@gmail.com
    first release:	2011-10-23
    last release:	2014-09-05

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
---------------------------------------------------------------------"""

# vendors id
MICROCHIP_ID = 0x04D8

# products id (Pinguino's Microchip sub-license)
P8_ID  = 0xFEAA
P32_ID = 0x003C

# RB 17/09/2013
"""
class PinguinoNoBoot(object):
    name = 'Pinguino (no Bootloader)'
    arch = 8
    bldr = 'noboot'
    proc = ''
    board = ''
    vendor = ''
    product = ''
    memstart = 0x0000
    memend = 0xFFF8
    shortarg = '-b'
    longarg = '--noboot'
"""

"""---------------------------------------------------------------------
    8-bit Boards
---------------------------------------------------------------------"""

class Pinguino2455(object):
    name = 'Pinguino 2455'
    arch = 8
    bldr = 'boot4'
    proc = '18f2455'
    board = 'PINGUINO2455'
    vendor = MICROCHIP_ID
    product = P8_ID
    memstart = 0x0C00       # bootloader offset
    memend   = 0x6000
    shortarg = '-p2455'
    longarg = '--pinguino2455'

class Pinguino2550(object):
    name = 'Pinguino 2550'
    arch = 8
    bldr = 'boot2'
    proc = '18f2550'
    board = 'PINGUINO2550'
    vendor = MICROCHIP_ID
    product = P8_ID
    memstart = 0x2000       # bootloader offset
    memend   = 0x8000
    shortarg = '-p2550'
    longarg = '--pinguino2550'

class FreeJALduino(object):
    name = 'FreeJALduino'
    arch = 8
    bldr = 'boot2'
    proc = '18f2550'
    board = 'FREEJALDUINO'
    vendor = MICROCHIP_ID
    product = P8_ID
    memstart = 0x2000
    memend   = 0x8000
    config = 0x300000
    shortarg = '-J'
    longarg = '--freejalduino'

class Pinguino25k50(object):
    name = 'Pinguino 25k50'
    arch = 8
    bldr = 'boot4'
    proc = '18f25k50'
    board = 'PINGUINO25K50'
    vendor = MICROCHIP_ID
    product = P8_ID
    memstart = 0x0C00       # bootloader offset
    memend   = 0x8000
    shortarg = '-p25k50'
    longarg = '--pinguino25k50'

class Pinguino26J50(object):
    name = 'Pinguino 26J50'
    arch = 8
    bldr = 'boot4'
    proc = '18f26j50'
    board = 'PINGUINO26J50'
    vendor = MICROCHIP_ID
    product = P8_ID
    memstart = 0x0C00       # bootloader offset
    memend   = 0x10000
    shortarg = '-p26j50'
    longarg = '--pinguino26j50'

class Pinguino27J53(object):
    name = 'Pinguino 27J53'
    arch = 8
    bldr = 'boot4'
    proc = '18f27j53'
    board = 'PINGUINO27J53'
    vendor = MICROCHIP_ID
    product = P8_ID
    memstart = 0x0C00       # bootloader offset
    memend   = 0x20000
    shortarg = '-p27j53'
    longarg = '--pinguino27j53'

class Pinguino4455(object):
    name = 'Pinguino 4455'
    arch = 8
    bldr = 'boot4'
    proc = '18f4455'
    board = 'PINGUINO4455'
    vendor = MICROCHIP_ID
    product = P8_ID
    memstart = 0x0C00       # bootloader offset
    memend   = 0x6000
    shortarg = '-p4455'
    longarg = '--pinguino4455'

class Pinguino4550(object):
    name = 'Pinguino 4550'
    arch = 8
    bldr = 'boot2'
    proc = '18f4550'
    board = 'PINGUINO4550'
    vendor = MICROCHIP_ID
    product = P8_ID
    memstart = 0x2000       # bootloader offset
    memend   = 0x8000
    shortarg = '-p4550'
    longarg = '--pinguino4550'

class PICuno_Equo(object):
    name = 'PICuno Equo'
    arch = 8
    bldr = 'boot2'
    proc = '18f4550'
    board = 'PICUNO_EQUO'
    vendor = MICROCHIP_ID
    product = P8_ID
    memstart = 0x2000
    memend   = 0x8000
    config = 0x300000
    shortarg = '-eq'
    longarg = '--picunoequo'

class Pinguino45k50(object):
    name = 'Pinguino 45k50'
    arch = 8
    bldr = 'boot4'
    proc = '18f45k50'
    board = 'PINGUINO45K50'
    vendor = MICROCHIP_ID
    product = P8_ID
    memstart = 0x0C00       # bootloader offset
    memend   = 0x8000
    shortarg = '-p45k50'
    longarg = '--pinguino45k50'

class CHRP3(object):
    name = 'CHRP3.0'
    arch = 8
    bldr = 'boot4'
    proc = '18f25k50'
    board = 'CHRP3'
    vendor = MICROCHIP_ID
    product = P8_ID
    memstart = 0x0C00       # bootloader offset
    memend   = 0x8000
    shortarg = '-c'
    longarg = '--chrp3'

class Pinguino46J50(object):
    name = 'Pinguino 46J50'
    arch = 8
    bldr = 'boot4'
    proc = '18f46j50'
    board = 'PINGUINO46J50'
    vendor = MICROCHIP_ID
    product = P8_ID
    memstart = 0x0C00       # bootloader offset
    memend   = 0x10000
    shortarg = '-p46j50'
    longarg = '--pinguino46j50'

class Pinguino47J53(object):
    name = 'Pinguino 47J53'
    arch = 8
    bldr = 'boot4'
    proc = '18f47j53'
    board = 'PINGUINO47J53'
    vendor = MICROCHIP_ID
    product = P8_ID
    memstart = 0x0C00       # bootloader offset
    memend   = 0x20000
    shortarg = '-p47j53'
    longarg = '--pinguino47j53'

"""---------------------------------------------------------------------
    32-bit Boards
---------------------------------------------------------------------"""

class Pinguino32MX220(object):
    name = 'Pinguino 32MX220'
    arch = 32
    bldr = 'microchip'
    proc = '32MX220F032B'
    board = 'PINGUINO32MX220'
    vendor = MICROCHIP_ID
    product = P32_ID
    ivtstart = 0x9D003000           # _ebase_address (exception_mem)
    memstart = 0x9D004010           # kseg0_program_mem
    memend   = 0x9D008000
    shortarg = '-p220'
    longarg = '--pinguino32mx220'

class Pinguino32MX250(object):
    name = 'Pinguino 32MX250'
    arch = 32
    bldr = 'microchip'
    proc = '32MX250F128B'
    board = 'PINGUINO32MX250'
    vendor = MICROCHIP_ID
    product = P32_ID
    ivtstart = 0x9D003000           # _ebase_address (exception_mem)
    memstart = 0x9D004010           # kseg0_program_mem
    memend   = 0x9D020000
    shortarg = '-p250'
    longarg = '--pinguino32mx250'

class Pinguino32MX270(object):
    name = 'Pinguino 32MX270'
    arch = 32
    bldr = 'microchip'
    proc = '32MX270F256B'
    board = 'PINGUINO32MX270'
    vendor = MICROCHIP_ID
    product = P32_ID
    ivtstart = 0x9D002000           # _ebase_address (exception_mem)
    memstart = 0x9D003010           # kseg0_program_mem
    memend   = 0x9D040000
    shortarg = '-p270'
    longarg = '--pinguino32mx270'

class PIC32_Pinguino_220(object):
    name = 'PIC32 Pinguino MX220'
    arch = 32
    bldr = 'microchip'
    proc = '32MX220F032D'
    board = 'PIC32_PINGUINO_220'
    vendor = MICROCHIP_ID
    product = P32_ID
    ivtstart = 0x9D003000           # _ebase_address (exception_mem)
    memstart = 0x9D004010           # kseg0_program_mem
    memend   = 0x9D008000
    shortarg = '-m'
    longarg = '--olimex220'

class PIC32_Pinguino(object):
    name = 'PIC32 Pinguino'
    arch = 32
    bldr = 'microchip'
    proc = '32MX440F256H'
    board = 'PIC32_PINGUINO'
    vendor = MICROCHIP_ID
    product = P32_ID
    ivtstart = 0x9D005000           # _ebase_address (exception_mem)
    memstart = 0x9D006010           # kseg0_program_mem
    memend   = 0x9D040000
    shortarg = '-o'
    longarg = '--olimex440'

class PIC32_Pinguino_OTG(object):
    name = 'PIC32 Pinguino OTG'
    arch = 32
    bldr = 'microchip'
    proc = '32MX440F256H'
    board = 'PIC32_PINGUINO_OTG'
    vendor = MICROCHIP_ID
    product = P32_ID
    ivtstart = 0x9D005000           # _ebase_address (exception_mem)
    memstart = 0x9D006010           # kseg0_program_mem
    memend   = 0x9D040000
    shortarg = '-O'
    longarg = '--olimex440OTG'

class PIC32_Pinguino_Micro(object):
    name = 'PIC32 Pinguino Micro'
    arch = 32
    bldr = 'microchip'
    proc = '32MX440F256H'
    board = 'PIC32_PINGUINO_MICRO'
    vendor = MICROCHIP_ID
    product = P32_ID
    ivtstart = 0x9D005000           # _ebase_address (exception_mem)
    memstart = 0x9D006010           # kseg0_program_mem
    memend   = 0x9D040000
    shortarg = '-M'
    longarg = '--olimex440Micro'

class PIC32_Pinguino_T795(object):
    name = 'PIC32 Pinguino T795'
    arch = 32
    bldr = 'microchip'
    proc = '32MX795F512H'
    board = 'PIC32_PINGUINO_T795'
    vendor = MICROCHIP_ID
    product = P32_ID
    ivtstart = 0x9D005000           # _ebase_address (exception_mem)
    memstart = 0x9D006010           # kseg0_program_mem
    memend   = 0x9D080000
    shortarg = '-T'
    longarg = '--olimexT795'

class Emperor_460(object):
    name = 'Emperor 460'
    arch = 32
    bldr = 'microchip'
    proc = '32MX460F512L'
    board = 'EMPEROR460'
    vendor = MICROCHIP_ID
    product = P32_ID
    ivtstart = 0x9D005000           # _ebase_address (exception_mem)
    memstart = 0x9D006010           # kseg0_program_mem
    memend   = 0x9D080000
    shortarg = '-e'
    longarg = '--emperor460'

class Emperor_795(object):
    name = 'Emperor 795'
    arch = 32
    bldr = 'microchip'
    proc = '32MX795F512L'
    board = 'EMPEROR795'
    vendor = MICROCHIP_ID
    product = P32_ID
    ivtstart = 0x9D005000           # _ebase_address (exception_mem)
    memstart = 0x9D006010           # kseg0_program_mem
    memend   = 0x9D080000
    shortarg = '-E'
    longarg = '--emperor795'

class UBW32_460(object):
    name = 'UBW32 460'
    arch = 32
    bldr = 'microchip'
    proc = '32MX460F512L'
    board = 'UBW32_460'
    vendor = MICROCHIP_ID
    product = P32_ID
    ivtstart = 0x9D005000           # _ebase_address (exception_mem)
    memstart = 0x9D006010           # kseg0_program_mem
    memend   = 0x9D080000
    shortarg = '-u'
    longarg = '--ubw460'

class UBW32_795(object):
    name = 'UBW32 795'
    arch = 32
    bldr = 'microchip'
    proc = '32MX795F512L'
    board = 'UBW32_795'
    vendor = MICROCHIP_ID
    product = P32_ID
    ivtstart = 0x9D005000           # _ebase_address (exception_mem)
    memstart = 0x9D006010           # kseg0_program_mem
    memend   = 0x9D080000
    shortarg = '-U'
    longarg = '--ubw795'

"""---------------------------------------------------------------------
    Board list
---------------------------------------------------------------------"""

    # RB 17/09/2013
    #PinguinoNoBoot,
boardlist = [
    Pinguino2455,  Pinguino4455,
    Pinguino2550,  Pinguino4550,
    FreeJALduino,  PICuno_Equo, CHRP3,
    Pinguino25k50, Pinguino45k50,
    Pinguino26J50, Pinguino46J50,
    Pinguino27J53, Pinguino47J53,
    Pinguino32MX220, Pinguino32MX250, Pinguino32MX270,
    PIC32_Pinguino, PIC32_Pinguino_OTG,
    PIC32_Pinguino_Micro, PIC32_Pinguino_T795,
    PIC32_Pinguino_220,
    Emperor_460, Emperor_795,
    UBW32_460, UBW32_795
]
