
import os
import sys

from pyplusplus.module_builder import module_builder_t

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("..")
from sireutils import *

wrap_classes = [ "assign_atoms",
                 "AssignBase",
                 "AssignInstruction",
                 "assign_parameters",
                 "AtomDB",
                 "AtomTable",
                 "AtomTypeDB",
                 "AtomType",
                 "AtomTypeTable",
                 "DBBase",
                 "FuncDB",
                 "MatchAtom",
                 "MatchAtomType",
                 "MatchData",
                 "match_mol_name",
                 "match_mol_group",
                 "match_mol_state",
                 "match_atom_name",
                 "match_atom_num",
                 "match_single_atom_name",
                 "match_single_atom_num",
                 "match_res_name",
                 "match_res_num",
                 "match_res_alias",
                 "match_res_res_bond_via",
                 "match_no_res_res_bond_via",
                 "match_single_res_name",
                 "match_single_res_num",
                 "match_single_res_alias",
                 "match_single_res_res_bond_via",
                 "match_single_no_res_res_bond_via",
                 "match_atom_type",
                 "match_atom_element",
                 "match_single_atom_type",
                 "match_single_atom_element",
                 "MatchMol",
                 "MatchMRA",
                 "MatchMRAData",
                 "MatchMR",
                 "MatchMRData",
                 "MatchAtomTypeData",
                 "MatchRes",
                 "NameDB",
                 "OverWriteParams",
                 "ParameterDB",
                 "ParameterTable",
                 "RelateAtomTypeDB",
                 "RelateDB",
                 "RelateMRADB",
                 "RelateMRDB",
                 "RelationshipDB",
                 "Smarts",
                 "TableBase",
                 "Term12DB",
                 "Term13DB",
                 "Term14DB",
                 "using_database",
                 "using_parameters_base",
                 "using_relationships_base"
               ]

huge_classes = []

aliases = {}

extra_includes = [ "SireCAS/function.h",
                   "SireCAS/expression.h" ]

def fix_atomtypetable(c):
    c.decls("create").exclude()

def fix_parameterdb(c):
    c.decls("addComponent").exclude()
    c.decls("asA").exclude()

def fix_tablebase(c):
    c.decls("create").exclude()

special_code = { "AtomTypeTable" : fix_atomtypetable,
                 "ParameterDB" : fix_parameterdb,
                 "TableBase" : fix_tablebase }

incpaths = sys.argv[1:]
incpaths.insert(0, "../../")

#give the namespace and header files for the module to export
namespace = "SireDB"
modulename = "DB"

headerfiles = ["siredb_headers.h"]

#construct a module builder that will build the module's wrappers
mb = module_builder_t( files=headerfiles, 
                       include_paths=incpaths,
                       define_symbols=["SKIP_BROKEN_GCCXML_PARTS"],
                       start_with_declarations = [namespace] )

#export each class in turn
for classname in wrap_classes:
   #tell the program to write wrappers for this class
   export_class(mb, classname, aliases, special_code)

write_wrappers(mb, modulename, extra_includes, huge_classes)
