#!/usr/bin/env python
#-*- coding: utf-8 -*-

from .constructor import Kit

Kit = Kit()

########################################################################
class Communication(object):

    CDC_read = lambda n:Kit.addFunct("CDC", "CDC.read", 1, t="output", fill=[])
    CDC_write = lambda n:Kit.addFunct("CDC", "CDC.write", 1, t="linear", fill=["edit-c"])
    CDC_getstring = lambda n:Kit.addFunct("CDC", "CDC.getString", 0, t="output")
    CDC_getkey = lambda n:Kit.addFunct("CDC", "CDC.getKey", 0, t="output")
    CDC_print = lambda n:Kit.addFunct("CDC", "CDC.print", 1, t="linear", fill=["edit"])
    CDC_println = lambda n:Kit.addFunct("CDC", "CDC.println", 1, t="linear", fill=["edit"])
    CDC_printf = lambda n:Kit.addFunct("CDC", "CDC.printf", 2, t="linear", fill=["edit", "more-args"])

    Serial_available = lambda n:Kit.addFunct("Serial", "Serial.available", 0, t="output-bool", fill=[])
    Serial_init = lambda n:Kit.addFunct("Serial", "Serial.init", 1, t="linear", fill=["spin-int"])
    Serial_flush = lambda n:Kit.addFunct("Serial", "Serial.flush", 0, t="linear", fill=[])
    Serial_getKey = lambda n:Kit.addFunct("Serial", "Serial.getKey", 0, t="output", fill=[])
    Serial_getString = lambda n:Kit.addFunct("Serial", "Serial.getString", 0, t="output", fill=[])
    Serial_print = lambda n:Kit.addFunct("Serial", "Serial.print", 1, t="linear", fill=["edit"])
    Serial_println = lambda n:Kit.addFunct("Serial", "Serial.println", 1, t="linear", fill=["edit"])
    Serial_read = lambda n:Kit.addFunct("Serial", "Serial.read", 0, t="output", fill=[])
    Serial_write = lambda n:Kit.addFunct("Serial", "Serial.write", 1, t="linear", fill=["edit-c"])
    Serial_printf = lambda n:Kit.addFunct("Serial", "Serial.printf", 2, t="linear", fill=["edit", "more-args"])

    USB_read = lambda n:Kit.addFunct("USB", "USB.read", 0, t="output", fill=[])
    USB_write = lambda n:Kit.addFunct("USB", "USB.write", 1, t="linear", fill=["edit"])
    USB_printf = lambda n:Kit.addFunct("USB", "USB.printf", 2, t="linear", fill=["edit"])
    USB_available = lambda n:Kit.addFunct("USB", "USB.available", 0, t="output-bool", fill=[])
    USB_gets = lambda n:Kit.addFunct("USB", "USB.gets", 0, t="output", fill=[])
    USB_sets = lambda n:Kit.addFunct("USB", "USB.sets", 1, t="linear", fill=["edit-c"])

