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

__atom_property_types__ = { "SireMol::AtomCoords" : "SireMaths_Vector",
                            "SireMol::AtomElements" : "SireMol_Element",
                            "SireMol::AtomCharges" : "SireUnits_Dimension_Charge",
                            "SireMol::AtomIntProperty" : "qint64",
                            "SireMol::AtomStringProperty" : "QString",
                            "SireMol::AtomFloatProperty" : "double",
                            "SireMol::AtomVariantProperty" : "QVariant" }

def __get_atom_property__(atom, property_name):
    property_type = atom.propertyType(property_name)
    property_type = __atom_property_types__[property_type]

    return getattr(atom, "_get_property_%s" % property_type)(property_name)

Atom.property = __get_atom_property__

