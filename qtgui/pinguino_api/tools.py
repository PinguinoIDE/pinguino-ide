#!/usr/bin/env python
#-*- coding: utf-8 -*-


import logging
import time
import traceback


########################################################################
class Debugger(object):
    """"""


    #----------------------------------------------------------------------
    @classmethod
    def debug_method(self, f):

        def inset(*args, **kwaargs):
            logging.info("Starting '%s' from '%s'" % (f.__name__, f.__module__))
            t1 = time.time()
            try:
                ret = f(*args, **kwaargs)
            except:
                logging.info(traceback.format_exc())
                return
            t2 = time.time()
            logging.info("Time spent for '%s': %.2f s" % (f.__name__, t2-t1))
            logging.info("Ending '%s'" % f.__name__)
            return ret
        return inset





