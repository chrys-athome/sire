#!/bin/env python

import os
import string


def runqmake(startdir):

    for file in os.listdir(startdir):

        path = os.path.join(startdir,file)

        if (str(file) == "." or str(file) == ".."):
            return
        elif (os.path.isdir(path)):
            runqmake(path)
        elif (path.endswith(".pro")):
            olddir = os.path.abspath(os.path.curdir)
            os.chdir(startdir)
            print "chdir %s" % os.path.abspath(os.path.curdir)
            os.system("qmake %s" % file)
            os.chdir(olddir)

#only modify .pro files in the src directory            
runqmake("src")
