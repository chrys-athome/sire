###############################################
#
# This file contains special code to help
# with the wrapping of SireMove classes
#
#

import sys
import pickle

from pyplusplus.module_builder import call_policies

sys.path.append("../AutoGenerate")
from scanheaders import *

def fix_Moves(c):
    
    for decl in c.decls():
        if str(decl).find("SimController") != -1:
            decl.exclude()

def fixMB(mb):
    mb.add_declaration_code("#include \"SireMol/bondid.h\"")
    mb.add_declaration_code("#include \"SireMol/angleid.h\"")
    mb.add_declaration_code("#include \"SireMol/dihedralid.h\"")
    mb.add_declaration_code("#include \"SireMove/movermove.h\"") 

special_code = { "SireMove::MovesBase" : fix_Moves,
                 "SireMove::SameMoves" : fix_Moves,
                 "SireMove::WeightedMoves" : fix_Moves
               }

implicitly_convertible = [ ("SireMol::BondID",  "SireMove::DofID"),
                           ("SireMol::AngleID", "SireMove::DofID"),
                           ("SireMol::DihedralID","SireMove::DofID") ] 
