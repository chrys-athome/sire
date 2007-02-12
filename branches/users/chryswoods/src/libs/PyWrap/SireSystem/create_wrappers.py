
import os
import sys

from pyplusplus.module_builder import module_builder_t

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("..")
from sireutils import *

wrap_classes = [ "MoveBase",
                 "Move",
                 "MovesBase",
                 "SameMoves",
                 "Moves",
                 "SimSystem",
                 "SystemData",
                 "System",
                 "SystemID"
               ]

huge_classes = []

aliases = {}

extra_includes = [ "SireMol/molecule.h",
                   "SireMol/residue.h",
                   "SireMol/newatom.h" ]

def remove_next_move(c):
    c.decls( "nextMove", allow_empty=False ).exclude()

special_code = { "MovesBase" : remove_next_move,
                 "SameMoves" : remove_next_move }

incpaths = sys.argv[1:]
incpaths.insert(0, "../../")

#give the namespace and header files for the module to export
namespace = "SireSystem"
modulename = "System"

headerfiles = ["siresystem_headers.h"]

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
