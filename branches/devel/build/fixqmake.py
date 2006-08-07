#!/bin/env python

import os
import string

def rootDir(ndirs):
    """Return the path to the root directory, which is 'ndirs' about this
       directory (e.g. ndirs=2 would return ../..)"""
    
    rootdir = ""
    
    for i in range(0,ndirs):
        rootdir = os.path.join(rootdir,"..")
        
    return rootdir

def fixProFile(qfile, depth):
    """This parses a qmake .pro file and ensures that the 
       'include' line is in the file after the 'template' variable"""

    rootdir = rootDir(depth)
    builddir = os.path.join('$$sire.root',"build")
    globalconfig = os.path.join('$$sire.build',"global.pri")
    configfile = os.path.join('$$sire.root',"configure.pri")
    
    if (len(rootdir) == 0):
        builddir = "build"
        configfile = "configure.pri"
    
    lines = open(qfile,"r").readlines()
    
    incstrings = []
    
    incstrings.append("sire.root = %s" % rootdir)
    incstrings.append("sire.build = %s" % builddir)
    incstrings.append("sire.config = %s" % configfile)
    incstrings.append("sire.global = %s" % globalconfig)
    incstrings.append("!include($$sire.global): error(Cannot find global configure file!)")
    
    qfile = open(qfile,"w")
    for line in lines:
    
        foundold = False
    
        if (line.find("# AUTO_GENERATED_LINE") != -1):
                foundold = True

        if (not foundold):
            qfile.write(line)
                
            if (line.find("TEMPLATE =") != -1):
                for incstring in incstrings:
                    qfile.write(incstring + " # AUTO_GENERATED_LINE\n")


def fixProFiles(startdir, depth=0):

    for file in os.listdir(startdir):

        path = os.path.join(startdir,file)

        if (os.path.isdir(path)):
            fixProFiles(path, depth+1)
        elif (path.endswith(".pro")):
            fixProFile(path, depth)

#only modify .pro files in the src directory            
fixProFiles("src",1)
#and of course the top sire.pro file!
fixProFile("sire.pro",0)
