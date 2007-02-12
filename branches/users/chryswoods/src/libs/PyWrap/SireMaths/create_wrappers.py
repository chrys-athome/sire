
import os
import sys

from pyplusplus.module_builder import module_builder_t

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("..")
from sireutils import *

wrap_classes = [ "Angle",
                 "AxisSet",
                 "Complex",
                 "Line",
                 "Matrix4",
                 "Matrix",
                 "Plane",
                 "Quaternion",
                 "RanGenerator",
                 "Sphere",
                 "Torsion",
                 "Triangle",
                 "Vector"
               ]

huge_classes = []

aliases = {}

extra_includes = []

def fix_matrix4(c):
   #remove 16 argument constructor!
   for d in c.constructors():
      if len(d.arguments) >= 16:
         d.exclude()

special_code = { "Matrix4" : fix_matrix4 }

incpaths = sys.argv[1:]
incpaths.insert(0, "../../")

#give the namespace and header files for the module to export
namespace = "SireMaths"
modulename = "Maths"

headerfiles = ["siremaths_headers.h"]

#construct a module builder that will build the module's wrappers
mb = module_builder_t( files=headerfiles, 
                       include_paths=incpaths,
                       define_symbols=["SKIP_BROKEN_GCCXML_PARTS"],
                       start_with_declarations = [namespace] )


for calldef in mb.calldefs():
    try:
      calldef.virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL
    except:
      pass

mb.calldefs().create_with_signature = True

#export each class in turn
for classname in wrap_classes:
   #tell the program to write wrappers for this class
   export_class(mb, classname, aliases, special_code)

write_wrappers(mb, modulename, extra_includes, huge_classes)
