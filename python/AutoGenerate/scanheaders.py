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
import string

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

class Properties:
    def __init__(self):
        self._dependencies = {}
        self._properties = []

    def addProperty(self, property, alias):
        self._properties.append( (property,alias) )

    def properties(self):
        return self._properties

    def addDependency(self, headerfile, dir, module_dir):
        deps = getDependencies(dir, headerfile) + getDependencies(module_dir, headerfile)
        
        for dep in deps:
            self._dependencies[dep] = 1

    def dependencies(self):
        return self._dependencies.keys()

skip_metatypes = [ "QVariant", 
                   "SireCAS::ExpressionBase",
                   "SireMaths::Rational",
                   "SireCluster::Node",
                   "SireCluster::Nodes" ]

class HeaderInfo:
    def __init__(self, filename, dir, module_dir):
        self._filename = filename
        self._dependencies = getDependencies(dir, filename) + getDependencies(module_dir, filename)
        self._classes = []
        self._functions = []
        self._aliases = {}
        self._properties = []
        self._metatypes = []

    def addClass(self, classname):
        self._classes.append(classname)

    def addFunction(self, func):
        self._functions.append(func)

    def addMetaType(self, classname):
        #don't register some types
        if classname in skip_metatypes:
            return        

        self._metatypes.append(classname)

    def addAlias(self, classname, alias):
        self._aliases[classname] = alias

    def addProperty(self, prop, propbase):
        self._properties.append( (prop, propbase) )

    def dependencies(self):
        return self._dependencies

    def classes(self):
        return self._classes

    def functions(self):
        return self._functions

    def metaTypes(self):
        return self._metatypes

    def aliases(self):
        return self._aliases

    def properties(self):
        return self._properties

    def hasProperties(self):
        return len(self._properties) > 0

match_class = r"SIREN*_EXPOSE_CLASS\(\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*\)"
match_alias = r"SIREN*_EXPOSE_ALIAS\(\s*\n*\s*\(?([<>,\-\s\w\d:]+)\)?\s*\n*,\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*\)"
match_function = r"SIREN*_EXPOSE_FUNCTION\(\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*\)"
match_property = r"SIREN*_EXPOSE_PROPERTY\(\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*,\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*\)"
match_atom_property = r"SIRE_EXPOSE_ATOM_PROPERTY\(\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*,\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*\)"
match_res_property = r"SIRE_EXPOSE_RESIDUE_PROPERTY\(\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*,\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*\)"
match_cg_property = r"SIRE_EXPOSE_CUTGROUP_PROPERTY\(\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*,\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*\)"
match_chain_property = r"SIRE_EXPOSE_CHAIN_PROPERTY\(\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*,\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*\)"
match_seg_property = r"SIRE_EXPOSE_SEGMENT_PROPERTY\(\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*,\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*\)"
match_bead_property = r"SIRE_EXPOSE_BEAD_PROPERTY\(\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*,\s*\n*\s*\(?([<>,\s\w\d:]+)\)?\s*\n*\s*\)"
match_metatype = r"Q_DECLARE_METATYPE\(\s*\n*\s*\(?([<>.\s\w\d:]+)\)?\s*\n*\s*\)"

def scanFiles(dir, module_dir, atom_properties, cg_properties,
                               res_properties, chain_properties, seg_properties,
                               bead_properties):
    """Scan the header files in the passed directory to get information
       about all of the exposed classes, returning a list of all of 
       the classes that are being exposed, and placing meta information
       into the directory 'module_dir'""" 

    h_files = getFiles(dir, "*.h") + getFiles(module_dir, "*.h")
    hpp_files = getFiles(dir, "*.hpp") + getFiles(module_dir, "*.hpp")
    cpp_files = getFiles(dir, "*.cpp")

    #dictionary mapping files to exposed classes
    active_files = {}

    #the list of exposed classes
    exposed_classes = []

    #the list of classes that have been registered with QMetaType
    meta_classes = []

    #read each file, looking for SIRE_EXPOSE_FUNCTION or SIRE_EXPOSE_CLASS
    for file in h_files + hpp_files:
        if file.find("sirenglobal.h") != -1:
            continue

        try:
            lines = open("%s/%s" % (dir,file), "r").readlines()
        except:
            lines = open("%s/%s" % (module_dir,file), "r").readlines()

        text = string.join(lines)

        for m in re.finditer(match_class, text):
            if file not in active_files:
                active_files[file] = HeaderInfo(file, dir, module_dir)

            active_files[file].addClass(m.groups()[0].strip())
            exposed_classes.append(m.groups()[0].strip())

        for m in re.finditer(match_alias, text):
            if file not in active_files:
                active_files[file] = HeaderInfo(file, dir, module_dir)

            active_files[file].addClass(m.groups()[0].strip())
            active_files[file].addAlias(m.groups()[0].strip(), m.groups()[1].strip())
            exposed_classes.append(m.groups()[0].strip())

        for m in re.finditer(match_function, text):
            if file not in active_files:
                active_files[file] = HeaderInfo(file, dir, module_dir)

            active_files[file].addFunction(m.groups()[0].strip())

        for m in re.finditer(match_metatype, text):
            #don't match the 'errors.h' files, as these are wrapped separately
            if file == "errors.h":
                continue

            if file not in active_files:
                active_files[file] = HeaderInfo(file, dir, module_dir)

            active_files[file].addMetaType(m.groups()[0].strip())

        for m in re.finditer(match_property, text):
            if file not in active_files:
                active_files[file] = HeaderInfo(file, dir, module_dir)

            active_files[file].addProperty(m.groups()[0].strip(), m.groups()[1].strip())

        for m in re.finditer(match_atom_property, text):
            atom_properties.addDependency(file, dir, module_dir)
            atom_properties.addProperty(m.groups()[0].strip(), m.groups()[1].strip())

            if file not in active_files:
                active_files[file] = HeaderInfo(file, dir, module_dir)

            active_files[file].addClass(m.groups()[1].split("::")[-1].strip())
            exposed_classes.append(m.groups()[1].split("::")[-1].strip())

        for m in re.finditer(match_cg_property, text):
            cg_properties.addDependency(file, dir, module_dir)
            cg_properties.addProperty(m.groups()[0].strip(), m.groups()[1].strip())

            if file not in active_files:
                active_files[file] = HeaderInfo(file, dir, module_dir)

            active_files[file].addClass(m.groups()[1].split("::")[-1].strip())
            exposed_classes.append(m.groups()[1].split("::")[-1].strip())

        for m in re.finditer(match_res_property, text):
            res_properties.addDependency(file, dir, module_dir)
            res_properties.addProperty(m.groups()[0].strip(), m.groups()[1].strip())

            if file not in active_files:
                active_files[file] = HeaderInfo(file, dir, module_dir)

            active_files[file].addClass(m.groups()[1].split("::")[-1].strip())
            exposed_classes.append(m.groups()[1].split("::")[-1].strip())

        for m in re.finditer(match_chain_property, text):
            chain_properties.addDependency(file, dir, module_dir)
            chain_properties.addProperty(m.groups()[0].strip(), m.groups()[1].strip())

            if file not in active_files:
                active_files[file] = HeaderInfo(file, dir, module_dir)

            active_files[file].addClass(m.groups()[1].split("::")[-1].strip())
            exposed_classes.append(m.groups()[1].split("::")[-1].strip())

        for m in re.finditer(match_seg_property, text):
            seg_properties.addDependency(file, dir, module_dir)
            seg_properties.addProperty(m.groups()[0].strip(), m.groups()[1].strip())

            if file not in active_files:
                active_files[file] = HeaderInfo(file, dir, module_dir)

            active_files[file].addClass(m.groups()[1].split("::")[-1].strip())
            exposed_classes.append(m.groups()[1].split("::")[-1].strip())

        for m in re.finditer(match_bead_property, text):
            bead_properties.addDependency(file, dir, module_dir)
            bead_properties.addProperty(m.groups()[0].strip(), m.groups()[1].strip())

            if file not in active_files:
                active_files[file] = HeaderInfo(file, dir, module_dir)

            active_files[file].addClass(m.groups()[1].split("::")[-1].strip())
            exposed_classes.append(m.groups()[1].split("::")[-1].strip())

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
    
    modules = { "Analysis" : "SireAnalysis",
                "Base" : "SireBase",
                "CAS" : "SireCAS",
                "Cluster" : "SireCluster",
                "FF" : "SireFF",
                "ID" : "SireID",
                "IO" : "SireIO",
                "Maths" : "SireMaths",
                "MM" : "SireMM",
                "Mol" : "SireMol",
                "Move" : "SireMove",
                "Squire" : "Squire",
                "Stream" : "SireStream",
                "System" : "SireSystem",
                "Units" : "SireUnits",
                "Vol" : "SireVol" }
        
    if len(sys.argv) < 3:
        print "USAGE: python scanheaders.py input_path output_path"
        sys.exit(-1)
    
    siredir = sys.argv[1]
    outdir = sys.argv[2]

    exposed_classes = {}

    atom_properties = Properties()
    cg_properties = Properties()
    res_properties = Properties()
    chain_properties = Properties()
    seg_properties = Properties()
    bead_properties = Properties()

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
                                    "%s/%s" % (outdir,module),
                                    atom_properties, cg_properties, res_properties,
                                    chain_properties, seg_properties, bead_properties ) 

        for clas in module_classes:
            exposed_classes[clas] = 1


    #write the set of exposed classes to a data file to be used
    #by other scripts
    pickle.dump( exposed_classes, open("%s/classdb.data" % outdir, "w") )

    pickle.dump( atom_properties, open("%s/Mol/atomprops.data" % outdir, "w") )
    pickle.dump( cg_properties, open("%s/Mol/cgprops.data" % outdir, "w") )
    pickle.dump( res_properties, open("%s/Mol/resprops.data" % outdir, "w") )
    pickle.dump( chain_properties, open("%s/Mol/chainprops.data" % outdir, "w") )
    pickle.dump( seg_properties, open("%s/Mol/segprops.data" % outdir, "w") )
    pickle.dump( bead_properties, open("%s/Mol/beadprops.data" % outdir, "w") )
