#!/bin/env python

import sys
import os

# Relative path to the api doc directory
docdir = "docs/api"

# get the top-level directory (directory to build docs from)
topdir = os.getcwd()

#get the template doxyfile
doxyfile = "%s/Doxyfile" % topdir

# List the modules whose documentation should
# be compiled, together with the location of the 
# source code, and where to place the docs
modules = [
            ["SireStream", "src/libs/SireStream", "libs/SireStream"],
            ["SireError", "src/libs/SireError","libs/SireError"],
            ["SireUnits", "src/libs/SireUnits","libs/SireUnits"],
            ["SireMaths", "src/libs/SireMaths","libs/SireMaths"],
            ["SireCAS", "src/libs/SireCAS","libs/SireCAS"],
            ["SireBase", "src/libs/SireBase", "libs/SireBase"],
            ["SireMol", "src/libs/SireMol", "libs/SireMol"],
            ["SireVol", "src/libs/SireVol", "libs/SireVol"],
            ["SireDB", "src/libs/SireDB", "libs/SireDB"],
            ["SireFF", "src/libs/SireFF", "libs/SireFF"],
            ["SireMM", "src/libs/SireMM", "libs/SireMM"],
            ["SireIO", "src/libs/SireIO", "libs/SireIO"],
            ["SireSystem", "src/libs/SireSystem", "libs/SireSystem"],
            ["Spier", "src/libs/Spier", "libs/Spier"],
            ["SireTest", "src/libs/SireTest", "libs/SireTest"],
            ["SireUnitTest", "src/libs/SireUnitTest", "libs/SireUnitTest"],
            ["SirePy", "src/libs/SirePy", "libs/SirePy"]
            
          ]

#first, parse the Doxyfile and save each key-value pair
template = file(doxyfile,"r").readlines()
          
#variable used to save the paths and names of all of the
#generated tag files
tagfiles = {}
          
#make the docs for each directory in turn
for moduledata in modules:
    #change to the top-level directory
    os.chdir(topdir)
    
    #get the information about the module whose documentation
    #is to be generated
    module = moduledata[0]
    indir = moduledata[1]
    outdir = moduledata[2]
    
    print "Making documentation for %s" % (module)
    
    #change into the input directory
    os.chdir(indir)
    
    #get the relative path back to the output directory
    outdir = "../../../%s/%s" % (docdir,outdir)
    
    #create the output directory
    try:
        os.makedirs(outdir)
    except OSError:
        print "Directory %s already exists!" % outdir
    
    print "input from %s, output to %s" % (os.getcwd(),outdir)

    pipe = os.popen("doxygen -","w")
    #pipe = file("test.txt","w")
    
    #write the template configuration commands
    for line in template:
       pipe.write(line)
    
    #now write the local configuration for this module
    pipe.write("PROJECT_NAME = %s\n" % module)
    pipe.write("INPUT = .\n")
    pipe.write("OUTPUT_DIRECTORY = %s\n" % outdir)
    
    #what is the name of this modules tag file?
    tagfile = "%s/%s.tag" % (outdir,module)
    
    pipe.write("GENERATE_TAGFILE = %s\n" % tagfile)
    
    if len(tagfiles) > 0:
       #link to existing tag files
       pipe.write("TAGFILES = ")
       
       needslash = False
       
       for taglink in tagfiles:
           pipe.write(" \\ \n      %s=%s" % (taglink,tagfiles[taglink]))
           
       pipe.write("\n")
    
    pipe.close()
    
    tagfiles[tagfile] = "../../../%s/html" % moduledata[2]
    
