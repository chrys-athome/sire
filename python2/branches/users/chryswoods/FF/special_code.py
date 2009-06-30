###############################################
#
# This file contains special code to help
# with the wrapping of SireFF classes
#
#

from pyplusplus.module_builder import call_policies

def fix_FFID(c):
   c.add_declaration_code("#include \"forcefields.h\"")
   c.add_declaration_code("#include \"ffidx.h\"")

def fix_ForceTable(c):
   c.decls( "getTable" )[0].exclude()

def fix_ForceFields(c):
   c.add_declaration_code("#include \"forcetable.h\"")

special_code = { "SireFF::ForceTable" : fix_ForceTable,
                 "SireFF::FFID" : fix_FFID,
                 "SireFF::FFIdx" : fix_FFID,
                 "SireFF::FFName" : fix_FFID,
                 "SireFF::ForceFields" : fix_ForceFields }

implicitly_convertible = [ ("SireMaths::Vector", "SireFF::PointRef"),
                           ("SireMol::Atom", "SireFF::PointRef"),
                           ("SireFF::Point", "SireFF::PointRef") ]

def fixMB(mb):
    mb.add_declaration_code("#include \"SireFF/point.h\"")
    mb.add_declaration_code("#include \"SireMol/molecules.h\"")
    mb.add_declaration_code("#include \"SireMol/atom.h\"")
    mb.add_declaration_code("#include \"SireMaths/vector.h\"")

