
import os
import sys
import re

from pyplusplus.module_builder import module_builder_t

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("..")
from sireutils import *

wrap_classes = [ "TempBase",
                 "Celsius",
                 "Fahrenheit",
                 "Unit",
                 "GeneralUnit" ]

huge_classes = []

aliases = {}

extra_includes = []

special_code = {}

implicitly_convertible = [ ("SireUnits::Dimension::TempBase",
                            "SireUnits::Dimension::Temperature"),
                           ("SireUnits::Dimension::TempBase",
                            "double"),
                         ]
 

def findGlobals():
    lines = open("../../SireUnits/units.h", "r").readlines()
    
    FILE = open("_Units_global_variables.pyman.hpp", "w")
    
    print >>FILE, "#ifndef _Units_global_variables_hpp"
    print >>FILE, "#define _Units_global_variables_hpp"
    print >>FILE, "\nvoid register_man_global_variables();\n"
    print >>FILE, "#endif"
     
    FILE.close()

    FILE = open("_Units_global_variables.pyman.cpp", "w")
    
    print >>FILE, "\n#include \"_Units_global_variables.pyman.hpp\""
    print >>FILE, "#include <boost/python.hpp>"
    print >>FILE, "#include \"sireunits_headers.h\""
    print >>FILE, "\nusing namespace boost::python;"
    print >>FILE, "using namespace SireUnits;"
    print >>FILE, "using namespace SireUnits::Dimension;\n"
    
    print >>FILE, "void register_man_global_variables()"
    print >>FILE, "{"
    
    
    for line in lines:
        match = re.search(r"const Dimension::([\w\d\-<,>]+)\s+(\w+)", line)
    
        if match:
            name = match.group(2)
            print >>FILE, "    scope().attr(\"%s\") = %s;\n" % (name,name)
        else:
            match = re.search(r"const double\s+(\w+)", line)
            
            if match:
                name = match.group(1)
                print >>FILE, "    scope().attr(\"%s\") = %s;\n" % (name,name)


    print >>FILE, "}\n"

findGlobals()

incpaths = sys.argv[1:]
incpaths.insert(0, "../../")

#give the namespace and header files for the module to export
namespace = "SireUnits"
modulename = "Units"

headerfiles = ["sireunits_headers.h"]

#construct a module builder that will build the module's wrappers
mb = module_builder_t( files=headerfiles, 
                       include_paths=incpaths,
                       define_symbols=["SKIP_BROKEN_GCCXML_PARTS",
                                       "SKIP_TEMPLATE_DEFINITIONS"] )

populateNamespaces(mb)

for calldef in mb.calldefs():
    try:
      calldef.virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL
    except:
      pass

#add calls to register hand-written wrappers

#export each class in turn
for classname in wrap_classes:
   #tell the program to write wrappers for this class
   export_class(mb, classname, aliases, special_code)

#export the converter functions...
mb.free_functions("convert").include()
mb.free_functions("convertFrom").include()
mb.free_functions("convertTo").include()

mb.free_operators().include()

for v in mb.variables():
    if v.parent.name == "SireUnits":
        v.include()

register_implicit_conversions(mb, implicitly_convertible)

#add the hand-generated dimension wrappers
mb.add_declaration_code( "#include \"_Units_global_variables.pyman.hpp\"" )
mb.add_declaration_code( "#include \"sireunits_dimensions.h\"" )

mb.add_registration_code("register_SireUnits_dimensions();")
mb.add_registration_code("register_man_global_variables();")

#write all of the wrappers
write_wrappers(mb, modulename, extra_includes, huge_classes)

