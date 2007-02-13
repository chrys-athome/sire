
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
                 "UreyBradleyDB",
                 
                 "AngleTableT<SireCAS::Expression>",
                 "BondTableT<SireCAS::Expression>",
                 "DihedralTableT<SireCAS::Expression>",
                 
                 "InternalDB<SireMM::MolAngleInfo>",
                 "InternalDB<SireMM::MolBondInfo>",
                 "InternalDB<SireMM::MolDihedralInfo>",
                 
                 "InternalGenerator<SireMM::MolAngleInfo>",
                 "InternalGenerator<SireMM::MolBondInfo>",
                 "InternalGenerator<SireMM::MolDihedralInfo>",
                 
                 "InternalTableBase<SireMM::MolAngleInfo>",
                 "InternalTableBase<SireMM::MolBondInfo>",
                 "InternalTableBase<SireMM::MolDihedralInfo>",
                 
                 "InternalTable<SireMM::MolAngleInfo, SireCAS::Expression>",
                 "InternalTable<SireMM::MolBondInfo, SireCAS::Expression>",
                 "InternalTable<SireMM::MolDihedralInfo, SireCAS::Expression>"
                 
               ]

huge_classes = []

aliases = { "AngleTableT<SireCAS::Expression>" : "AngleTable_Expression_",
            "BondTableT<SireCAS::Expression>" : "BondTable_Expression_",
            "DihedralTableT<SireCAS::Expression>" : "DihedralTable_Expression_",
            
            "InternalDB<SireMM::MolAngleInfo>" : "InternalDB_MolAngleInfo_",
            "InternalDB<SireMM::MolBondInfo>" : "InternalDB_MolBondInfo_",
            "InternalDB<SireMM::MolDihedralInfo>" : "InternalDB_MolDihedralInfo_",

            "InternalGenerator<SireMM::MolAngleInfo>" : "InternalGenerator_MolAngleInfo_",
            "InternalGenerator<SireMM::MolBondInfo>" : "InternalGenerator_MolBondInfo_",
            "InternalGenerator<SireMM::MolDihedralInfo>" : "InternalGenerator_MolDihedralInfo_",

            "InternalTableBase<SireMM::MolAngleInfo>" :
                          "InternalTableBase_MolAngleInfo_",
            "InternalTableBase<SireMM::MolBondInfo>" :
                          "InternalTableBase_MolBondInfo_",
            "InternalTableBase<SireMM::MolDihedralInfo>" :
                          "InternalTableBase_MolDihedralInfo_",

            "InternalTable<SireMM::MolAngleInfo, SireCAS::Expression>" :
                              "InternalTable_MolAngleInfo_Expression_",
            "InternalTable<SireMM::MolBondInfo, SireCAS::Expression>" :
                              "InternalTable_MolBondInfo_Expression_",
            "InternalTable<SireMM::MolDihedralInfo, SireCAS::Expression>" :
                              "InternalTable_MolDihedralInfo_Expression_"
          }

extra_includes = [ "SireMol/molecule.h",
                   "SireMol/residue.h",
                   "SireMol/newatom.h",
                   "SireMol/atom.h" ]

def remove_create(c):
    c.decls("create").exclude()

def fix_noncopyable(c):
    c.noncopyable = False

special_code = { "AngleTable" : remove_create,
                 "BondTable" : remove_create,
                 "ChargeTable" : remove_create,
                 "DihedralTable" : remove_create,
                 "LJTable" : remove_create,
                 "InterCLJFF" : fix_noncopyable,
                 "Tip4PFF" : fix_noncopyable }

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


populateNamespaces(mb)

for calldef in mb.calldefs():
    try:
      calldef.virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL
    except:
      pass

mb.calldefs().create_with_signature = True

#add calls to register hand-written wrappers
mb.add_declaration_code( "#include \"QVector_ChargeParameter_.py.h\"" )
mb.add_registration_code( "register_QVector_ChargeParameter_class();", tail=False )

mb.add_declaration_code( "#include \"QVector_LJParameter_.py.h\"" )
mb.add_registration_code( "register_QVector_LJParameter_class();", tail=False )

#export each class in turn
for classname in wrap_classes:
   #tell the program to write wrappers for this class
   export_class(mb, classname, aliases, special_code)

write_wrappers(mb, modulename, extra_includes, huge_classes)
