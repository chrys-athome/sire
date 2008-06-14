###############################################
#
# This file contains special code to help
# with the wrapping of SireMM classes
#
#

import sys
import pickle

from pyplusplus.module_builder import call_policies

def fix_MolViewProperty(c):
    c.decls("set").call_policies = call_policies.return_self()

def fix_AtomLJs(c):
    fix_MolViewProperty(c)

def fix_AtomFunctions(c):
    c.add_declaration_code("#include \"SireMol/moleculedata.h\"")


special_code = { "AtomLJs" : fix_AtomLJs,
                 "SireMM::FourAtomFunctions" : fix_AtomFunctions,
                 "SireMM::ThreeAtomFunctions" : fix_AtomFunctions,
                 "SireMM::TwoAtomFunctions" : fix_AtomFunctions }

