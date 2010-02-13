###############################################
#
# This file contains special code to help
# with the wrapping of SireSec classes
#
#

import sys
import pickle

from pyplusplus.module_builder import call_policies

def fixMB(mb):
    mb.add_declaration_code("#include \"SireSec/key.h\"")

    for enum in mb.enums():
        if str(enum).find("SireSec::Ciphers") != -1 or \
           str(enum).find("SireSec::KeyTypes") != -1 or \
           str(enum).find("SireSec::CertTypes") != -1 or \
           str(enum).find("SireSec::MACTypes") != -1:
            enum.include()
            enum.include_files.append( "SireSec/key.h" )


