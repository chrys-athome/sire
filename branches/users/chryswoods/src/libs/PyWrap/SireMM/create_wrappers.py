
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
                 "assign_angles",
                 "assign_bonds",
                 "assign_dihedrals",
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
                 "InterGroupLJFF",
                 "InterLJFF",
                 "BondResID",
                 "BondAtomID",
                 "AngleResID",
                 "AngleAtomID",
                 "DihedralResID",
                 "DihedralAtomID",
                 "LJDB",
                 "LJFF",
                 "LJPair",
                 "LJParameter",
                 "LJTable",
                 "MolAngleInfo",
                 "MolBondInfo",
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
                 
                 "assign_internals<SireMM::MolAngleInfo>",
                 "assign_internals<SireMM::MolBondInfo>",
                 "assign_internals<SireMM::MolDihedralInfo>",
                 
                 "AtomTableT<SireMM::ChargeParameter>",
                 "AtomTableT<SireMM::LJParameter>",
                 
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
                 "InternalTable<SireMM::MolDihedralInfo, SireCAS::Expression>",
                 
                 "MolInternalInfo<SireMol::Angle>",
                 "MolInternalInfo<SireMol::Bond>",
                 "MolInternalInfo<SireMol::Dihedral>",
                 
                 "ResInternalInfo<SireMol::Angle>",
                 "ResInternalInfo<SireMol::Bond>",
                 "ResInternalInfo<SireMol::Dihedral>",
                 
                 "InternalInfo<SireMol::Angle>",
                 "InternalInfo<SireMol::Bond>",
                 "InternalInfo<SireMol::Dihedral>",
                 
                 "UsePassedInternals<SireMM::MolAngleInfo>",
                 "UsePassedInternals<SireMM::MolBondInfo>",
                 "UsePassedInternals<SireMM::MolDihedralInfo>"
                 
               ]

huge_classes = []

aliases = { "assign_internals<SireMM::MolAngleInfo>" :
                           "assign_internals_MolAngleInfo_",
            "assign_internals<SireMM::MolBondInfo>" :
                           "assign_internals_MolBondInfo_",
            "assign_internals<SireMM::MolDihedralInfo>" :
                           "assign_internals_MolDihedralInfo_",
            
            "AtomTableT<SireMM::ChargeParameter>" : "AtomTable_ChargeParameter_",
            "AtomTableT<SireMM::LJParameter>" : "AtomTable_LJParameter_",

            "AngleTableT<SireCAS::Expression>" : "AngleTable_Expression_",
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
                              "InternalTable_MolDihedralInfo_Expression_",
                              
            "MolInternalInfo<SireMol::Angle>" : "MolInternalInfo_Angle_",
            "MolInternalInfo<SireMol::Bond>" : "MolInternalInfo_Bond_",
            "MolInternalInfo<SireMol::Dihedral>" : "MolInternalInfo_Dihedral_",
            
            "ResInternalInfo<SireMol::Angle>" : "ResInternalInfo_Angle_",
            "ResInternalInfo<SireMol::Bond>" : "ResInternalInfo_Bond_",
            "ResInternalInfo<SireMol::Dihedral>" : "ResInternalInfo_Dihedral_",
            
            "InternalInfo<SireMol::Angle>" : "InternalInfo_Angle_",
            "InternalInfo<SireMol::Bond>" : "InternalInfo_Bond_",
            "InternalInfo<SireMol::Dihedral>" : "InternalInfo_Dihedral_",
            
            "UsePassedInternals<SireMM::MolAngleInfo>" :
                       "UsePassedInternals_MolAngleInfo_",
            "UsePassedInternals<SireMM::MolBondInfo>" :
                       "UsePassedInternals_MolBondInfo_",
            "UsePassedInternals<SireMM::MolDihedralInfo>" :
                       "UsePassedInternals_MolDihedralInfo_"
          }

extra_includes = [ "SireMol/molecule.h",
                   "SireMol/residue.h",
                   "SireMol/newatom.h",
                   "SireMol/atom.h" ]

def remove_create(c):
    c.decls("create").exclude()

def fix_noncopyable(c):
    c.noncopyable = False

def fix_assigninternals(c):
    c.add_declaration_code("#include \"SireDB/using_database.h\"")
    c.add_declaration_code("using namespace SireMM;")

special_code = { "assign_angles" : fix_assigninternals,
                 "assign_bonds" : fix_assigninternals,
                 "assign_dihedrals" : fix_assigninternals,
                 
                 "AngleTable" : remove_create,
                 "BondTable" : remove_create,
                 "ChargeTable" : remove_create,
                 "DihedralTable" : remove_create,
                 "LJTable" : remove_create,
                 "InterLJFF" : fix_noncopyable,
                 "InterGroupLJFF" : fix_noncopyable,
                 "InterCLJFF" : fix_noncopyable,
                 "Tip4PFF" : fix_noncopyable }

implicitly_convertible = [ ("QVector< QVector<SireMM::ChargeParameter> >",
                            "SireMM::AtomicCharges"),
                           ("QVector< SireMM::ChargeParameter >",
                            "SireMM::AtomicCharges"),
                           ("QVector< QVector<SireMM::LJParameter> >",
                            "SireMM::AtomicLJs"),
                           ("QVector< SireMM::LJParameter >",
                            "SireMM::AtomicLJs"),
                           ("double", "SireMM::ChargeParameter"),
                           ("const SireMM::CombiningRuleBase&",
                            "SireMM::CombiningRules"),
                           ("SireMM::SwitchFuncBase",
                            "SireMM::SwitchingFunction"),
                           ("QSet<SireMol::Angle>",
                            "SireMM::UsePassedAngles"),
                           ("const SireMM::AngleGeneratorBase&",
                            "SireMM::assign_angles"),
                           ("SireMM::UsePassedAngles",
                            "SireMM::assign_angles"),
                           ("QSet<SireMol::Bond>",
                            "SireMM::UsePassedBonds"),
                           ("const SireMM::BondGeneratorBase&",
                            "SireMM::assign_bonds"),
                           ("SireMM::UsePassedBonds",
                            "SireMM::assign_bonds"),
                           ("QSet<SireMol::Dihedral>",
                            "SireMM::UsePassedDihedrals"),
                           ("const SireMM::DihedralGeneratorBase&",
                            "SireMM::assign_dihedrals"),
                           ("SireMM::UsePassedDihedrals",
                            "SireMM::assign_dihedrals")
                         ]

incpaths = sys.argv[1:]
incpaths.insert(0, "../../")

#give the namespace and header files for the module to export
namespace = "SireMM"
modulename = "MM"

headerfiles = ["siremm_headers.h"]

#construct a module builder that will build the module's wrappers
mb = module_builder_t( files=headerfiles, 
                       include_paths=incpaths,
                       define_symbols=["SKIP_BROKEN_GCCXML_PARTS",
                                       "SKIP_TEMPLATE_DEFINITIONS"] )


populateNamespaces(mb)

for calldef in mb.calldefs():
    try:
      calldef.virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL
    except:
      pass

mb.calldefs().create_with_signature = True

#add calls to register hand-written wrappers
mb.add_declaration_code( "#include \"siremm_containers.h\"" )
mb.add_registration_code( "register_SireMM_containers();", tail=False )

#export each class in turn
for classname in wrap_classes:
   #tell the program to write wrappers for this class
   export_class(mb, classname, aliases, special_code)

register_implicit_conversions(mb, implicitly_convertible)

write_wrappers(mb, modulename, extra_includes, huge_classes)
