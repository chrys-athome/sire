###############################################
#
# This file contains special code to help
# with the wrapping of SireFF classes
#
#

from pyplusplus.module_builder import call_policies

def fix_ForceTable(c):
   c.decls( "getTable" )[0].exclude()

special_code = { "SireFF::ForceTable" : fix_ForceTable }
