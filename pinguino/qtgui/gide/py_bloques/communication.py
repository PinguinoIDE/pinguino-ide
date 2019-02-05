#!/usr/bin/env python
#-*- coding: utf-8 -*-

from .constructor import Kit

Kit = Kit()

########################################################################
class Communication(object):


    USB_read = lambda n:Kit.addFunct("USB", "USB.read", 0, t="output", fill=[])
    USB_write = lambda n:Kit.addFunct("USB", "USB.write", 1, t="linear", fill=["edit"])
    USB_printf = lambda n:Kit.addFunct("USB", "USB.printf", 2, t="linear", fill=["edit"])
    USB_available = lambda n:Kit.addFunct("USB", "USB.available", 0, t="output-bool", fill=[])
    USB_gets = lambda n:Kit.addFunct("USB", "USB.gets", 0, t="output", fill=[])
    USB_sets = lambda n:Kit.addFunct("USB", "USB.sets", 1, t="linear", fill=["edit-c"])

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

    I2C1_master = lambda n:Kit.addFunct("I2C", "I2C1.master", 1, t="linear", fill=["edit"])
    I2C1_slave = lambda n:Kit.addFunct("I2C", "I2C1.slave", 1, t="linear", fill=["edit"])  
    I2C1_init = lambda n:Kit.addFunct("I2C", "I2C1.init", 1, t="linear", fill=["edit"])
    I2C1_write = lambda n:Kit.addFunct("I2C", "I2C1.write", 1, t="linear", fill=["edit"])
    I2C1_writeChar = lambda n:Kit.addFunct("I2C", "I2C1.writeChar", 1, t="linear", fill=["edit"])
    I2C1_writeBytes = lambda n:Kit.addFunct("I2C", "I2C1.writeBytes", 1, t="linear", fill=["edit"])
    I2C1_read = lambda n:Kit.addFunct("I2C", "I2C1.read", 1, t="linear", fill=["edit"])
    I2C1_readChar = lambda n:Kit.addFunct("I2C", "I2C1.readChar", 1, t="linear", fill=["edit"])
    I2C1_readBytes = lambda n:Kit.addFunct("I2C", "I2C1.readBytes", 1, t="linear", fill=["edit"])
    I2C1_printf = lambda n:Kit.addFunct("I2C", "I2C1.printf", 1, t="linear", fill=["edit"])
    I2C1_wait = lambda n:Kit.addFunct("I2C", "I2C1.wait", 1, t="linear", fill=["edit"])
    I2C1_start = lambda n:Kit.addFunct("I2C", "I2C1.start", 1, t="linear", fill=["edit"])
    I2C1_stop = lambda n:Kit.addFunct("I2C", "I2C1.stop", 1, t="linear", fill=["edit"])
    I2C1_restart = lambda n:Kit.addFunct("I2C", "I2C1.restart", 1, t="linear", fill=["edit"])
    I2C1_sendNack = lambda n:Kit.addFunct("I2C", "I2C1.sendNack", 1, t="linear", fill=["edit"])
    I2C1_sendAck = lambda n:Kit.addFunct("I2C", "I2C1.sendAck", 1, t="linear", fill=["edit"])
    I2C1_onRequest = lambda n:Kit.addFunct("I2C", "I2C1.onRequest", 1, t="linear", fill=["edit"])#define I2CINT
    I2C1_onReceive = lambda n:Kit.addFunct("I2C", "I2C1.onReceive", 1, t="linear", fill=["edit"])#define I2CINT

    SPI1_begin = lambda n:Kit.addFunct("SPI", "SPI1.begin", 1, t="linear", fill=["edit"])#define SPIBEGIN
    SPI1_deselect = lambda n:Kit.addFunct("SPI", "SPI1.deselect", 1, t="linear", fill=["edit"])#define SPIDESELECT
    SPI1_select = lambda n:Kit.addFunct("SPI", "SPI1.select", 1, t="linear", fill=["edit"])#define SPISELECT
    SPI1_setBitOrder = lambda n:Kit.addFunct("SPI", "SPI1.setBitOrder", 1, t="linear", fill=["edit"])#define SPISETBITORDER
    SPI1_setMode = lambda n:Kit.addFunct("SPI", "SPI1.setMode", 1, t="linear", fill=["edit"])#define SPISETMODE
    SPI1_setDataMode = lambda n:Kit.addFunct("SPI", "SPI1.setDataMode", 1, t="linear", fill=["edit"])#define SPISETDATAMODE
    SPI1_setClock = lambda n:Kit.addFunct("SPI", "SPI1.setClock", 1, t="linear", fill=["edit"])#define SPISETCLOCK
    SPI1_setClockDivider = lambda n:Kit.addFunct("SPI", "SPI1.setClockDivider", 1, t="linear", fill=["edit"])#define SPISETCLOCKDIVIDER
    SPI1_write = lambda n:Kit.addFunct("SPI", "SPI1.write", 1, t="linear", fill=["edit"])#define SPIWRITE
    SPI1_read = lambda n:Kit.addFunct("SPI", "SPI1.read", 1, t="linear", fill=["edit"])#define SPIREAD
    SPI1_transfer = lambda n:Kit.addFunct("SPI", "SPI1.write", 1, t="linear", fill=["edit"])#define SPI1TRANS
