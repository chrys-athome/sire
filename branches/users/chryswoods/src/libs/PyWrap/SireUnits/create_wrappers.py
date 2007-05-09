
import os
import sys

from pyplusplus.module_builder import module_builder_t

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("..")
from sireutils import *

wrap_classes = [ "Celsius",
                 "Charge",
                 "DerivedUnit",
                 "Energy",
                 "Fahrenheit",
                 "Kelvin",
                 "Length", 
                 "Mass",
                 "Quantity",
                 "Temperature",
                 "Time",
                 "Unit"
               ]

huge_classes = []

aliases = {}

extra_includes = []

def add_operators(c):
   c.add_registration_code( "def( bp::self * bp::other<SireUnits::Dimension::Unit>() )" )
   c.add_registration_code( "def( bp::self / bp::other<SireUnits::Dimension::Unit>() )" )

special_code = { "Celsius" : add_operators,
                 "Charge" : add_operators,
                 "DerivedUnit" : add_operators,
                 "Energy" : add_operators,
                 "Fahrenheit" : add_operators,
                 "Kelvin" : add_operators,
                 "Length" : add_operators,
                 "Mass" : add_operators,
                 "Time" : add_operators
               }

implicitly_convertible = []

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

#write all of the wrappers
write_wrappers(mb, modulename, extra_includes, huge_classes)

