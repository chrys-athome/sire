
import os
import sys

from pyplusplus.module_builder import module_builder_t

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("..")
from sireutils import *

wrap_classes = [ "MolproCalculator",
                 "MolproFF",
                 "MolproProcessor",
                 "MolproSession"
               ]

huge_classes = []

aliases = {}

extra_includes = [ "SireMol/molecule.h",
                   "SireMol/residue.h",
                   "SireMol/newatom.h"
                 ]

def fix_MolproFF(c):
    c.noncopyable = False

special_code = { "MolproFF" : fix_MolproFF }

implicitly_convertible = []

incpaths = sys.argv[1:]
incpaths.insert(0, "../../")

#give the namespace and header files for the module to export
namespace = "Squire"
modulename = "Squire"

headerfiles = ["squire_headers.h"]

#construct a module builder that will build the module's wrappers
mb = module_builder_t( files=headerfiles, 
                       include_paths=incpaths,
                       define_symbols=["SKIP_BROKEN_GCCXML_PARTS"] )

populateNamespaces(mb)

for calldef in mb.calldefs():
    try:
      calldef.virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL
    except:
      pass

#add calls to register hand-written wrappers
mb.add_declaration_code( "#include \"squire_containers.h\"" )
mb.add_registration_code( "register_Squire_containers();", tail=False )

mb.calldefs().create_with_signature = True

#export each class in turn
for classname in wrap_classes:
   #tell the program to write wrappers for this class
   export_class(mb, classname, aliases, special_code)

register_implicit_conversions(mb, implicitly_convertible)

write_wrappers(mb, modulename, extra_includes, huge_classes)
