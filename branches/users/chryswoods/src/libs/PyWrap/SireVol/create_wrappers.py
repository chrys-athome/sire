
import os
import sys

from pyplusplus.module_builder import module_builder_t

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("..")
from sireutils import *

wrap_classes = [ "AABox",
                 "Cartesian",
                 "CoordGroupBase",
                 "CoordGroup",
                 "CoordGroupEditor",
                 "PeriodicBox",
                 "SpaceBase",
                 "Space" 
               ]

huge_classes = []

aliases = {}

extra_includes = [ "SireMaths/vector.h",
                   "SireMaths/quaternion.h",
                   "SireMaths/matrix.h" ]

def fix_coordgroup(c):
   c.decl("data").exclude()
   c.decl("constData").exclude()

def fix_coordgroupeditor(c):
   c.decl("data").exclude()

special_code = { "CoordGroupBase" : fix_coordgroup,
                 "CoordGroupEditor" : fix_coordgroupeditor }

incpaths = sys.argv[1:]
incpaths.insert(0, "../../")

#give the namespace and header files for the module to export
namespace = "SireVol"
modulename = "Vol"

headerfiles = ["sirevol_headers.h"]

#construct a module builder that will build the module's wrappers
mb = module_builder_t( files=headerfiles, 
                       include_paths=incpaths,
                       define_symbols=["SKIP_BROKEN_GCCXML_PARTS"],
                       start_with_declarations = [namespace] )


populateNamespaces(mb)

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
