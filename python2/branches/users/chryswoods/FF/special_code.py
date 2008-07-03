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

special_code = { "SireFF::ForceTable" : fix_ForceTable,
                 "SireFF::FFID" : fix_FFID,
                 "SireFF::FFIdx" : fix_FFID,
                 "SireFF::FFName" : fix_FFID }
