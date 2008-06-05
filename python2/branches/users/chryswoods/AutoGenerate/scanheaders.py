##########################################
#
#  This script scans all of the header
#  files in a specified directory to 
#  get the list of classes that should
#  be exposed in Python
#

import sys
import os
import re
import pickle

from glob import glob

def getFiles(dir, pattern):
    files = glob("%s/%s" % (dir,pattern))

    trimmed_files = []

    for file in files:
        trimmed_files.append( file[len(dir)+1:] )

    return trimmed_files

def getIncludes(file, lines):
    
    includes = { "\"%s\"" % file : 1 }

    for line in lines:
        if line.find("CONDITIONAL_INCLUDE") == -1:
            m = re.search(r"#include\s+([\"|<].*[\"|>])", line)

            if m:
                includes[m.groups()[0]] = 1

    ret = includes.keys()
    ret.sort()
    return ret

def getDependencies(dir, file):
    """Return the list of header files included by the .cpp or .c file
       that corresponds to 'file'"""

    try:
        #is there a corresponding .cpp file?
        file_cpp = re.sub(r"h(p*)$", "cpp", file)
        
        lines = open("%s/%s" % (dir,file_cpp), "r").readlines()
        return getIncludes(file, lines)

    except:
        pass

    try:
        #is there a corresponding .c file?
        file_c = re.sub(r"h(p*)$", "c", file)
    
        lines = open("%s/%s" % (dir,file_c), "r").readlines()
        return getIncludes(file, lines)

    except:
        pass

    return [ "\"%s\"" % file ]

def scanFiles(dir, module_dir):
    """Scan the header files in the passed directory to get information
       about all of the exposed classes, returning a list of all of 
       the classes that are being exposed, and placing meta information
       into the directory 'module_dir'""" 

    h_files = getFiles(dir, "*.h")
    hpp_files = getFiles(dir, "*.hpp")

    #dictionary mapping files to exposed classes
    active_files = {}

    #the list of exposed classes
    exposed_classes = []

    #read each file, looking for SIRE_EXPOSE_FUNCTION or SIRE_EXPOSE_CLASS
    for file in h_files + hpp_files:
        lines = open("%s/%s" % (dir,file), "r").readlines()

        for line in lines:
            m = re.search(r"SIRE_EXPOSE_CLASS\(\s*([<>\w\d:]+)\s*\)", line)

            if not m:
               m = re.search(r"SIRE_EXPOSE_ALIAS\(\s*([<>\w\d:]+)\s*,\s*([<>\w\d:]+)\s*\)", line)

            if m:
                if file not in active_files:
                    active_files[file] = ( [], [], getDependencies(dir, file), {} )

                active_files[file][0].append(m.groups()[0])
                exposed_classes.append(m.groups()[0])

                try:
                    active_files[file][3][m.groups()[0]] = m.groups()[1]
                except:
                    pass

            else:
                m = re.search(r"SIRE_EXPOSE_FUNCTION\(\s*([\w\d:]+)\s*\)", line)
                if m:
                    if file not in active_files:
                        active_files[file] = ( [], [], getDependencies(dir, file), {} )

                    active_files[file][1].append(m.groups()[0])


    #now add each active file to a single header file that can be parsed by Py++
    FILE = open("%s/active_headers.h" % module_dir, "w")

    print >>FILE,"#ifndef ACTIVE_HEADERS_H\n" + \
                 "#define ACTIVE_HEADERS_H\n\n" + \
                 "#ifdef GCCXML_PARSE\n"

    files = active_files.keys()
    files.sort()

    for file in files:
        print >>FILE,"#include \"%s\"" % file

    print >>FILE,"\n#endif\n\n#endif"

    FILE.close()

    #now write out the active_files data structure so it can be
    #used by other scripts
    FILE = open("%s/active_headers.data" % module_dir,"w")
    pickle.dump(active_files, FILE)
    FILE.close()

    return exposed_classes

if __name__ == "__main__":
    
    modules = { "Base" : "SireBase",
                "CAS" : "SireCAS",
                "Maths" : "SireMaths",
                "Vol" : "SireVol" }
        
    if len(sys.argv) < 3:
        print "USAGE: python scanheaders.py input_path output_path"
        sys.exit(-1)
    
    siredir = sys.argv[1]
    outdir = sys.argv[2]

    exposed_classes = {}

    for module in modules:
        
        try:
            os.makedirs( "%s/%s" % (outdir,module) )
        except:
            pass

        FILE = open("%s/%s/module_info" % (outdir,module), "w")

        print >>FILE,"Module %s" % module
        print >>FILE,"Source %s" % modules[module]
        print >>FILE,"Root %s" % siredir

        FILE.close()

        module_classes = scanFiles( "%s/%s" % (siredir,modules[module]),
                                    "%s/%s" % (outdir,module) ) 

        for clas in module_classes:
            exposed_classes[clas] = 1

    #write the set of exposed classes to a data file to be used
    #by other scripts
    pickle.dump( exposed_classes, open("%s/classdb.data" % outdir, "w") )
