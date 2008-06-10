#############################
##
## The SireMol module
##
## (C) Christopher Woods
##

import Sire.Maths
import Sire.Base
import Sire.ID

from Sire.Mol._Mol import *

def __get_atom_property__(atom, key):
    property_type = atom.propertyType(key).replace("::","_")

    return getattr(atom, "_get_property_%s" % property_type)(key)

def __get_atom_metadata__(atom, *args):

    if len(args) == 1:
        metakey = args[0]
        property_type = atom.metadataType(metakey).replace("::","_")
        return getattr(atom, "_get_metadata_%s" % property_type)(metakey)

    elif len(args) == 2:
         (key, metakey) = args
         property_type = atom.metadataType(key, metakey).replace("::","_")
         return getattr(atom, "_get_metadata_%s" % property_type)(key, metakey)

    else:
        raise "ERROR"

Atom.property = __get_atom_property__
Atom.metadata = __get_atom_metadata__

