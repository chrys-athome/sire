
import os
import sys

from pyplusplus.module_builder import module_builder_t

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("..")
from sireutils import *

wrap_classes = [ "AngleDB",
                 "AngleGenerator",
                 "UsePassedAngles",
                 "AngleTable",
                 #"assign_angles",
                 #"assign_bonds",
                 #"assign_dihedrals",
                 "AtomicCharges",
                 "AtomicLJs",
                 "BondDB",
                 "BondGenerator",
                 "UsePassedBonds",
                 "BondTable",
                 "ChargeDB",
                 "ChargeParameter",
                 "ChargeTable",
                 "CLJFF",
                 "CLJPair",
                 "CLJParameter",
                 "CombiningRuleBase",
                 "ArithmeticCombiningRules",
                 "GeometricCombiningRules",
                 "CombiningRules",
                 "DihedralDB",
                 "DihedralGenerator",
                 "UsePassedDihedrals",
                 "DihedralTable",
                 "InterCLJFF",
                 "BondResID",
                 "BondAtomID",
                 "AngleResID",
                 "AngleAtomID",
                 "DihedralResID",
                 "DihedralAtomID",
                 "LJDB",
                 "LJParameter",
                 "LJTable",
                 "MolAngleInfo",
                 "MolBondInfo",
                 "MolAngleInfo",
                 "MolDihedralInfo",
                 "ResAngleInfo",
                 "ResBondInfo",
                 "ResDihedralInfo",
                 "SwitchFuncBase",
                 "NoCutoff",
                 "HarmonicSwitchingFunction",
                 "SwitchingFunction",
                 "Tip4PFF",
                 "UreyBradleyDB"
               ]

huge_classes = []

aliases = {}

extra_includes = [ "SireMol/molecule.h",
                   "SireMol/residue.h",
                   "SireMol/newatom.h",
                   "SireMol/atom.h" ]

def remove_create(c):
    c.decls("create").exclude()

special_code = { "AngleTable" : remove_create,
                 "BondTable" : remove_create,
                 "ChargeTable" : remove_create,
                 "DihedralTable" : remove_create,
                 "LJTable" : remove_create }

incpaths = sys.argv[1:]
incpaths.insert(0, "../../")

#give the namespace and header files for the module to export
namespace = "SireMM"
modulename = "MM"

headerfiles = ["siremm_headers.h"]

#construct a module builder that will build the module's wrappers
mb = module_builder_t( files=headerfiles, 
                       include_paths=incpaths,
                       define_symbols=["SKIP_BROKEN_GCCXML_PARTS"],
                       start_with_declarations = [namespace] )


for calldef in mb.calldefs():
    try:
      calldef.virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL
    except:
      pass

mb.calldefs().create_with_signature = True

#export each class in turn
for classname in wrap_classes:
   #tell the program to write wrappers for this class
   export_class(mb, classname, aliases, special_code)

write_wrappers(mb, modulename, extra_includes, huge_classes)
