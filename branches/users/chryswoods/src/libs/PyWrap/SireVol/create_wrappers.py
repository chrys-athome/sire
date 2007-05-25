
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
                 "MapFromCartesianFunction",
                 "MapFromSelfFunction",
                 "MappingFunctionBase",
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

implicitly_convertible = [ ("QVector<SireMaths::Vector>","SireVol::CoordGroup"),
                           ("const SireVol::SpaceBase&","SireVol::Space"),
                           ("const SireVol::MappingFunctionBase&","SireVol::MappingFunction")
                         ]

incpaths = sys.argv[1:]
incpaths.insert(0, "../../")

#give the namespace and header files for the module to export
namespace = "SireVol"
modulename = "Vol"

headerfiles = ["sirevol_headers.h"]

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
mb.add_declaration_code( "#include \"sirevol_containers.h\"" )
mb.add_registration_code( "register_SireVol_containers();", tail=False )

mb.calldefs().create_with_signature = True

#export each class in turn
for classname in wrap_classes:
   #tell the program to write wrappers for this class
   export_class(mb, classname, aliases, special_code)

register_implicit_conversions(mb, implicitly_convertible)

write_wrappers(mb, modulename, extra_includes, huge_classes)
