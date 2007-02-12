
import os
import sys

from pyplusplus.module_builder import module_builder_t

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("..")
from sireutils import *

wrap_classes = [ "Angle",
                 "Atom",
                 "AtomCutting",
                 "AtomicProperties",
                 "AtomID",
                 "AtomIDGroup",
                 "AtomIndex",
                 "AtomInfo",
                 "AtomInfoGroup",
                 "AtomNum",
                 "AtomSelection",
                 "Bond",
                 "BondAddingFunction",
                 "CGAtomID",
                 "CGNumAtomID",
                 "CutGroup",
                 "CutGroupID",
                 "CutGroupNum",
                 "CuttingFunction",
                 "CuttingFunctionBase",
                 "Dihedral",
                 "EditMol",
                 "EditRes",
                 "Element",
                 "GroupID",
                 "GroupIndexID",
                 "IDBase",
                 "IDMolAtom",
                 "Improper",
                 "Index",
                 "MolCGAtomID",
                 "MolCutGroupID",
                 "Molecule",
                 "MoleculeBonds",
                 "MoleculeCutting",
                 "MoleculeGroup",
                 "MoleculeGroupID",
                 "MoleculeGroups",
                 "MoleculeID",
                 "MoleculeInfo",
                 "MoleculeVersion",
                 "NewAtom",
                 "Property",
                 "PropertyBase",
                 "ResID",
                 "ResIDAtomID",
                 "Residue",
                 "ResidueBonds",
                 "ResidueCutting",
                 "ResidueInfo",
                 "ResNum",
                 "ResNumAtomID",
                 "ResNumIndexID",
                 "VariantProperty",
                 "WeightFunction"
               ]

huge_classes = [ "EditMol",
                 "EditRes",
                 "Molecule",
                 "NewAtom",
                 "Residue" ]

aliases = {}

extra_includes = [ "SireMaths/angle.h",
                   "SireMaths/quaternion.h",
                   "SireMaths/matrix.h",
                   "SireMaths/triangle.h",
                   "SireMaths/line.h",
                   "SireMaths/torsion.h" ]

def fix_atominfo(c):
    c.decls( "element", allow_empty=True ).exclude()
    c.decls( "index", allow_empty=True ).exclude()
    c.decls( "info", allow_empty=True ).exclude()

def fix_atom(c):
    c.decls( "info", allow_empty=True ).exclude()
    c.decls( "vector", allow_empty=True ).exclude()

def fix_atominfogroup(c):
    c.decls( "constData", allow_empty=True ).exclude()

special_code = { "AtomInfo" : fix_atominfo,
                 "Atom" : fix_atom,
                 "AtomInfoGroup" : fix_atominfogroup
               }

incpaths = sys.argv[1:]
incpaths.insert(0, "../../")

#give the namespace and header files for the module to export
namespace = "SireMol"
modulename = "Mol"

headerfiles = ["siremol_headers.h"]

#construct a module builder that will build the module's wrappers
mb = module_builder_t( files=headerfiles, 
                       include_paths=incpaths,
                       define_symbols=["SKIP_BROKEN_GCCXML_PARTS"],
                       start_with_declarations = [namespace] )

#add calls to register hand-written wrappers
mb.add_declaration_code( "#include \"QSet_AtomIndex_.py.h\"" )
mb.add_registration_code( "register_QSet_AtomIndex_class();", tail=False )

#export each class in turn
for classname in wrap_classes:
   #tell the program to write wrappers for this class
   export_class(mb, classname, aliases, special_code)

#write all of the wrappers
write_wrappers(mb, modulename, extra_includes, huge_classes)

