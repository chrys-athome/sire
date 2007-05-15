
import os
import sys

from pyplusplus.module_builder import module_builder_t
from pyplusplus.module_builder import call_policies

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("..")
from sireutils import *

wrap_classes = [ "CheckPoint",
                 "LocalSimSystem",
                 "MoveBase",
                 "Move",
                 "MovesBase",
                 "SameMoves",
                 "Moves",
                 "QuerySystem",
                 "SimSystem",
                 "SystemData",
                 "System",
                 "SystemID",
                 "SystemMonitors"
               ]

huge_classes = []

aliases = {}

extra_includes = [ "SireMol/molecule.h",
                   "SireMol/molecules.h",
                   "SireMol/residue.h",
                   "SireMol/newatom.h",
                   "SireFF/ffgroupid.h" ]

def remove_next_move(c):
    c.decls( "nextMove", allow_empty=False ).exclude()

def expose_clone(c):
    c.decls( "clone" ).include()
    c.decls( "clone" ).call_policies = \
             call_policies.return_value_policy(call_policies.manage_new_object)

def fix_noncopyable(c):
    c.noncopyable = False

special_code = { "MovesBase" : remove_next_move,
                 "SameMoves" : remove_next_move,
                 "Move" : expose_clone,
                 "Moves" : fix_noncopyable }

implicitly_convertible = [ ("const SireSystem::MoveBase&",
                            "SireSystem::Move"),
                           ("SireSystem::Move", "SireSystem::Moves"),
                           ("const SireSystem::MoveBase&",
                            "SireSystem::Moves")
                         ]

incpaths = sys.argv[1:]
incpaths.insert(0, "../../")

#give the namespace and header files for the module to export
namespace = "SireSystem"
modulename = "System"

headerfiles = ["siresystem_headers.h"]

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
mb.add_declaration_code( "#include \"siresystem_containers.h\"" )
mb.add_registration_code( "register_SireSystem_containers();", tail=False )

mb.calldefs().create_with_signature = True

#export each class in turn
for classname in wrap_classes:
   #tell the program to write wrappers for this class
   export_class(mb, classname, aliases, special_code)

#export the free functions and free variables
def export_free(things):
   for thing in things:
       if thing.parent.name == namespace:
           thing.include()
           
export_free( mb.free_functions() )
mb.free_operators().include()
export_free( mb.variables() )

register_implicit_conversions(mb, implicitly_convertible)

write_wrappers(mb, modulename, extra_includes, huge_classes)
