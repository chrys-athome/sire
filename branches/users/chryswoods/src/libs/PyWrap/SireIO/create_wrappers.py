
import os
import sys

from pyplusplus.module_builder import module_builder_t

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("..")
from sireutils import *

wrap_classes = [ "IOBase",
                 "PDB",
                 "ProtoMS"
               ]

huge_classes = []

aliases = {}

extra_includes = [ "SireMol/molecule.h",
                   "SireMol/editmol.h"
                 ]

def fix_iobase(c):
    c.add_declaration_code("using namespace SireMol;")

special_code = { "IOBase" : fix_iobase }

incpaths = sys.argv[1:]
incpaths.insert(0, "../../")

#give the namespace and header files for the module to export
namespace = "SireIO"
modulename = "IO"

headerfiles = ["sireio_headers.h"]

#construct a module builder that will build the module's wrappers
mb = module_builder_t( files=headerfiles, 
                       include_paths=incpaths,
                       define_symbols=["SKIP_BROKEN_GCCXML_PARTS"],
                       start_with_declarations = [namespace] )

mb.calldefs().virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL
mb.calldefs().create_with_signature = True

#export each class in turn
for classname in wrap_classes:
   #tell the program to write wrappers for this class
   export_class(mb, classname, aliases, special_code)

write_wrappers(mb, modulename, extra_includes, huge_classes)
