###############################################
#
# This file contains special code to help
# with the wrapping of SireUnits classes
#
#

import re

def findGlobals():
    #read in the information about this module
    lines = open("module_info", "r").readlines()
    root = lines[2].split()[1]
    sourcedir = lines[1].split()[1]

    lines = open("%s/%s/units.h" % (root,sourcedir), "r").readlines()
    
    FILE = open("_Units_global_variables.pyman.hpp", "w")
    
    print >>FILE, "#ifndef _Units_global_variables_hpp"
    print >>FILE, "#define _Units_global_variables_hpp"
    print >>FILE, "\nvoid register_man_global_variables();\n"
    print >>FILE, "#endif"
     
    FILE.close()

    FILE = open("_Units_global_variables.pyman.cpp", "w")
    
    print >>FILE, "\n#include \"_Units_global_variables.pyman.hpp\""
    print >>FILE, "#include <boost/python.hpp>"
    print >>FILE, "#include \"SireUnits/units.h\""
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

def fixMB(mb):
   mb.add_declaration_code("#include \"SireUnits/temperature.h\"")
   mb.add_declaration_code("#include \"sireunits_dimensions.h\"")
   mb.add_declaration_code("#include \"_Units_global_variables.pyman.hpp\"")
  

   mb.add_registration_code("register_SireUnits_dimensions();")
   mb.add_registration_code("register_man_global_variables();")

   #add all of the global physical constants to the module
   findGlobals()


implicitly_convertible = [ ("SireUnits::Dimension::TempBase",
                            "SireUnits::Dimension::Temperature"),
                           ("SireUnits::Dimension::TempBase",
                            "double"),
                         ]
 
