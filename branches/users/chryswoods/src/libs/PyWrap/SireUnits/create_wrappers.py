
import os
import sys
import re

from pyplusplus.module_builder import module_builder_t

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("..")
from sireutils import *

wrap_classes = [ "Unit",
                 "TempBase",
                 "Celsius",
                 "Fahrenheit" ]

huge_classes = []

aliases = {}

extra_includes = []

special_code = {}

implicitly_convertible = [ ("SireUnits::Dimension::TempBase", 
                            "SireUnits::Dimension::Temperature") ]

def findDimensions():
   
   lines = open("../../SireUnits/dimensions.h","r").readlines()
   
   dims = []
   
   for line in lines:
       match = re.search(r"typedef (PhysUnit<[\d,-]+>) (\w+)", line)
       
       if match:
           dims.append( (match.group(1), match.group(2)) )
           
   return dims

def createDimensionWrapper(name, _class):

     print "Creating %s.pyman.cpp and %s.pyman.hpp" % (name,name)
     
     FILE = open("%s.pyman.hpp" % name, "w")
     
     print >>FILE, "#ifndef %s_hpp__pyplusplus_wrapper" % name
     print >>FILE, "#define %s_hpp__pyplusplus_wrapper" % name
     print >>FILE, "\nvoid register_%s_class();\n" % name
     print >>FILE, "#endif"
     
     FILE.close()

     FILE = open("%s.pyman.cpp" % name, "w")
     
     print >>FILE, "\n#include \"%s.pyman.hpp\"" % name
     print >>FILE, "#include <boost/python.hpp>"
     print >>FILE, "#include \"sireunits_headers.h\""
     print >>FILE, "\nusing namespace boost::python;"
     print >>FILE, "using namespace SireUnits;"
     print >>FILE, "using namespace SireUnits::Dimension;\n"
     
     print >>FILE, "str _pvt_get_name(const %s &val){ return str(double(val));}\n" \
                          % (name) 
                          
     print >>FILE, "void register_%s_class()" % name
     print >>FILE, "{"
     
     print >>FILE, "    class_< %s, bases<Unit> >(\"%s\", init<>())" % (name,name)
     print >>FILE, "        .def( init<double>() )"
     print >>FILE, "        .def( init<const %s&>() )" % name
     print >>FILE, "        .def( \"__str__\", &_pvt_get_name )"
     print >>FILE, "        .def( self == self )"
     print >>FILE, "        .def( self != self )"
     print >>FILE, "        .def( -self )"
     print >>FILE, "        .def( self + self )"
     print >>FILE, "        .def( self - self )"
     print >>FILE, "        .def( self += self )"
     print >>FILE, "        .def( self -= self )"
     print >>FILE, "        .def( self * other<double>() )"
     print >>FILE, "        .def( self / other<double>() )"
     print >>FILE, "        .def( other<double>() * self )"
     print >>FILE, "        .def( other<double>() / self )"
     print >>FILE, "        .def( \"to\", (double (%s::*)(const %s&) const) &%s::in )" \
                                         % (name,name,name) 
     
     print >>FILE, "    ;\n"
     
     print >>FILE, "}\n"
     
     return ("%s.pyman.hpp" % name, "register_%s_class()" % name)

additional_funcs = []

dimensions = findDimensions()

for dimension in dimensions:
    additional_funcs.append( createDimensionWrapper(dimension[1], 
                                                    dimension[0]) )

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
        match = re.search(r"const Dimension::([\w\d\-<>]+)\s(\w+)", line)
    
        if match:
            name = match.group(2)
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
for func in additional_funcs:
    mb.add_declaration_code( "#include \"%s\"" % func[0] )
    mb.add_registration_code( "%s;" % func[1], tail=True )

mb.add_declaration_code( "#include \"_Units_global_variables.pyman.hpp\"" )
mb.add_registration_code("register_man_global_variables();")

#write all of the wrappers
write_wrappers(mb, modulename, extra_includes, huge_classes)

