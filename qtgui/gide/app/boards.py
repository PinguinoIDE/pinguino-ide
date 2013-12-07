#!/usr/bin/env python
# -*- coding: iso-8859-15 -*-

"""-------------------------------------------------------------------------
    Pinguino Boards List

    author:			Regis Blanchot
    contact:		rblanchot@gmail.com 
    first release:	2011-10-23
    last release:	2012-05-04

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

#import wx	# wx.NewId()

# vendors id
MICROCHIP_ID = 0x04D8

# products id
PIC8_ID = 0xFEAA
PIC32_ID = 0x003C

class PinguinoNoBoot:
    name='Pinguino (no Bootloader)'
    #id=wx.NewId()
    arch=8
    bldr='noboot'
    proc=''
    board=''
    vendor=''
    product=''
    memstart=0x0000
    memend=0xFFF8
    shortarg='-b'
    longarg='--noboot'

class Pinguino2550:
    name='Pinguino 2550'
    #id=wx.NewId()
    arch=8
    bldr='boot2'
    proc='18f2550'
    board='PINGUINO2550'
    vendor=MICROCHIP_ID
    product=PIC8_ID
    memstart=0x2000       # bootloader offset
    memend=0x7fff
    shortarg='-p'
    longarg='--pinguino2550'

class Pinguino25k50:
    name='Pinguino 25k50'
    #id=wx.NewId()
    arch=8
    bldr='boot4'
    proc='18f25k50'
    board='PINGUINO25K50'
    vendor=MICROCHIP_ID
    product=PIC8_ID
    memstart=0x0C00       # bootloader offset
    memend=0x7fff
    shortarg='-k'
    longarg='--pinguino25k50'

class Pinguino45k50:
    name='Pinguino 45k50'
    #id=wx.NewId()
    arch=8
    bldr='boot4'
    proc='18f45k50'
    board='PINGUINO45K50'
    vendor=MICROCHIP_ID
    product=PIC8_ID
    memstart=0x0C00       # bootloader offset
    memend=0x7fff
    shortarg='-l'
    longarg='--pinguino45k50'

class CHRP3:
    name='CHRP3.0'
    #id=wx.NewId()
    arch=8
    bldr='boot4'
    proc='18f25k50'
    board='CHRP3'
    vendor=MICROCHIP_ID
    product=PIC8_ID
    memstart=0x0C00       # bootloader offset
    memend=0x7fff
    shortarg='-c'
    longarg='--chrp3'

class Pinguino4550:
    name='Pinguino 4550'
    #id=wx.NewId()
    arch=8
    bldr='boot2'
    proc='18f4550'
    board='PINGUINO4550'
    vendor=MICROCHIP_ID
    product=PIC8_ID
    memstart=0x2000       # bootloader offset
    memend=0x7fff
    shortarg='-P'
    longarg='--pinguino4550'

class Pinguino26J50:
    name='Pinguino 26J50'
    #id=wx.NewId()
    arch=8
    bldr='boot4'
    proc='18f26j50'
    board='PINGUINO26J50'
    vendor=MICROCHIP_ID
    product=PIC8_ID
    memstart=0x0C00       # bootloader offset
    memend=0xFFF8
    shortarg='-j'
    longarg='--pinguino26j50'

class Pinguino27J53:
    name='Pinguino 27J53'
    #id=wx.NewId()
    arch=8
    bldr='boot4'
    proc='18f27j53'
    board='PINGUINO27J53'
    vendor=MICROCHIP_ID
    product=PIC8_ID
    memstart=0x0C00       # bootloader offset
    memend=0x1FFF8
    shortarg='-j53'
    longarg='--pinguino27j53'

class PICuno_Equo:
    name='PICuno Equo'
    #id=wx.NewId()
    arch=8
    bldr='boot2'
    proc='18f4550'
    board='PICUNO_EQUO'
    vendor=MICROCHIP_ID
    product=PIC8_ID
    memstart=0x2000
    memend=0x7fff
    config=0x300000
    shortarg='-eq'
    longarg='--picunoequo'

class FreeJALduino:
    name='FreeJALduino'
    #id=wx.NewId()
    arch=8
    bldr='boot2'
    proc='18f2550'
    board='FREEJALDUINO'
    vendor=MICROCHIP_ID
    product=PIC8_ID
    memstart=0x2000
    memend=0x7fff
    config=0x300000
    shortarg='-J'
    longarg='--freejalduino'

class PIC32_Pinguino:
    name='PIC32 Pinguino'
    #id=wx.NewId()
    arch=32
    bldr='microchip'
    proc='32MX440F256H'
    board='PIC32_PINGUINO'
    vendor=MICROCHIP_ID
    product=PIC32_ID
    memstart=0x00000
    memend=0x40000
    shortarg='-o'
    longarg='--olimex440'

class PIC32_Pinguino_OTG:
    name='PIC32 Pinguino OTG'
    #id=wx.NewId()
    arch=32
    bldr='microchip'
    proc='32MX440F256H'
    board='PIC32_PINGUINO_OTG'
    vendor=MICROCHIP_ID
    product=PIC32_ID
    memstart=0x00000 #0x9D000000
    memend=0x40000
    shortarg='-O'
    longarg='--olimex440OTG'

class PIC32_Pinguino_Micro:
    name='PIC32 Pinguino Micro'
    #id=wx.NewId()
    arch=32
    bldr='microchip'
    proc='32MX440F256H'
    board='PIC32_PINGUINO_MICRO'
    vendor=MICROCHIP_ID
    product=PIC32_ID
    memstart=0x00000
    memend=0x40000
    shortarg='-M'
    longarg='--olimex440Micro'

class PIC32_Pinguino_T795:
    name='PIC32 Pinguino T795'
    #id=wx.NewId()
    arch=32
    bldr='microchip'
    proc='32MX795F512H'
    board='PIC32_PINGUINO_T795'
    vendor=MICROCHIP_ID
    product=PIC32_ID
    memstart=0x00000
    memend=0x80000
    shortarg='-T'
    longarg='--olimexT795'

class PIC32_Pinguino_220:
    name='PIC32 Pinguino 220'
    #id=wx.NewId()
    arch=32
    bldr='microchip'
    proc='32MX220F032D'
    board='PIC32_PINGUINO_220'
    vendor=MICROCHIP_ID
    product=PIC32_ID
    memstart=0x00000
    memend=0x07FFF
    shortarg='-m'
    longarg='--olimex220'

class GENERIC32MX250F128:
    name='GENERIC32MX250F128'
    #id=wx.NewId()
    arch=32
    bldr='microchip'
    proc='32MX250F128B'
    board='GENERIC32MX250F128'
    vendor=MICROCHIP_ID
    product=PIC32_ID
    memstart=0x00000
    memend=0x20000
    shortarg='-N'
    longarg='--generic250'

class GENERIC32MX220F032:
    name='GENERIC32MX220F032'
    #id=wx.NewId()
    arch=32
    bldr='microchip'
    proc='32MX220F032B'
    board='GENERIC32MX220F032'
    vendor=MICROCHIP_ID
    product=PIC32_ID
    memstart=0x00000
    memend=0x07FFF
    shortarg='-n'
    longarg='--generic220'

class Emperor_460:
    name='Emperor 460'
    #id=wx.NewId()
    arch=32
    bldr='microchip'
    proc='32MX460F512L'
    board='EMPEROR460'
    vendor=MICROCHIP_ID
    product=PIC32_ID
    memstart=0x00000
    memend=0x80000
    shortarg='-e'
    longarg='--emperor460'

class Emperor_795:
    name='Emperor 795'
    #id=wx.NewId()
    arch=32
    bldr='microchip'
    proc='32MX795F512L'
    board='EMPEROR795'
    vendor=MICROCHIP_ID
    product=PIC32_ID
    memstart=0x00000
    memend=0x80000
    shortarg='-E'
    longarg='--emperor795'

class UBW32_460:
    name='UBW32 460'
    #id=wx.NewId()
    arch=32
    bldr='microchip'
    proc='32MX460F512L'
    board='UBW32_460'
    vendor=MICROCHIP_ID
    product=PIC32_ID
    memstart=0x00000
    memend=0x80000
    shortarg='-u'
    longarg='--ubw460'

class UBW32_795:
    name='UBW32 795'
    #id=wx.NewId()
    arch=32
    bldr='microchip'
    proc='32MX795F512L'
    board='UBW32_795'
    vendor=MICROCHIP_ID
    product=PIC32_ID
    memstart=0x00000
    memend=0x80000
    shortarg='-U'
    longarg='--ubw795'

boardlist = [   
    PinguinoNoBoot,
    Pinguino2550,  Pinguino4550,
    Pinguino25k50, CHRP3, Pinguino45k50,
    Pinguino26J50,
    Pinguino27J53, 
    PIC32_Pinguino, PIC32_Pinguino_OTG,
    PIC32_Pinguino_Micro, PIC32_Pinguino_T795, PIC32_Pinguino_220,
    GENERIC32MX250F128, GENERIC32MX220F032,
    Emperor_460, Emperor_795,
    UBW32_460, UBW32_795
]
