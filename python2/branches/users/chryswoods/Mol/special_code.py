###############################################
#
# This file contains special code to help
# with the wrapping of SireVol classes
#
#

import sys
import pickle

from pyplusplus.module_builder import call_policies

sys.path.append("../AutoGenerate")
from scanheaders import *

atomprops = pickle.load( open("atomprops.data") )

return_policy = "bp::return_value_policy<bp::copy_const_reference>()"

def fix_Atom(c):
   #add accessor functions for all of the atom properties
   for atomprop in atomprops.properties():
       p = atomprop[0]
       c.add_registration_code( "def( \"_get_property_%s\", &SireMol::Atom::property<%s>, %s)" \
                                      % (p.replace("::","_"), p, return_policy) )

   #now add in all of the header files
   for header in atomprops.dependencies():
       c.add_declaration_code( "#include %s" % header )

def fix_AtomEditor(c):
   c.decls( "rename" ).call_policies = call_policies.return_self()
   c.decls( "renumber" ).call_policies = call_policies.return_self()

def fix_AtomStructureEditor(c):
   fix_AtomEditor(c)

   c.decls( "reindex" ).call_policies = call_policies.return_self()
   c.decls( "reparent" ).call_policies = call_policies.return_self()   

def fix_AtomSelection(c):
   c.decls( "selectAll" ).call_policies = call_policies.return_self()
   c.decls( "deselectAll" ).call_policies = call_policies.return_self()
   c.decls( "selectNone" ).call_policies = call_policies.return_self()
   c.decls( "select" ).call_policies = call_policies.return_self()
   c.decls( "deselect" ).call_policies = call_policies.return_self()
   c.decls( "selectOnly" ).call_policies = call_policies.return_self()
   c.decls( "invert" ).call_policies = call_policies.return_self()
   c.decls( "intersect" ).call_policies = call_policies.return_self()
   c.decls( "unite" ).call_policies = call_policies.return_self()
   c.decls( "subtract" ).call_policies = call_policies.return_self()
   c.decls( "mask" ).call_policies = call_policies.return_self()

def fix_CGEditor(c):
   c.decls( "rename" ).call_policies = call_policies.return_self()

def fix_CGStructureEditor(c):
   fix_CGEditor(c)
   
   c.decls( "reindex" ).call_policies = call_policies.return_self()
   c.decls( "remove" ).call_policies = call_policies.return_self()
   c.decls( "transfer" ).call_policies = call_policies.return_self()
   c.decls( "transferAll" ).call_policies = call_policies.return_self()

fix_ChainEditor = fix_CGEditor
fix_ChainStructureEditor = fix_CGStructureEditor

fix_SegEditor = fix_CGEditor
fix_SegStructureEditor = fix_CGStructureEditor

def fix_ResEditor(c):
    c.decls( "renumber" ).call_policies = call_policies.return_self()
    c.decls( "rename" ).call_policies = call_policies.return_self()

def fix_ResStructureEditor(c):
    fix_ResEditor(c)

    c.decls( "reindex" ).call_policies = call_policies.return_self()
    c.decls( "reparent" ).call_policies = call_policies.return_self()
    c.decls( "remove" ).call_policies = call_policies.return_self()
    c.decls( "transfer" ).call_policies = call_policies.return_self()
    c.decls( "transferAll" ).call_policies = call_policies.return_self()

def fix_MolEditor(c):
    c.decls( "renumber" ).call_policies = call_policies.return_self()
    c.decls( "rename" ).call_policies = call_policies.return_self()

def fix_MolStructureEditor(c):
    fix_MolEditor(c)

    c.decls( "remove" ).call_policies = call_policies.return_self()
    c.decls( "removeAllAtoms" ).call_policies = call_policies.return_self()
    c.decls( "removeAllCutGroups" ).call_policies = call_policies.return_self()
    c.decls( "removeAllResidues" ).call_policies = call_policies.return_self()
    c.decls( "removeAllChains" ).call_policies = call_policies.return_self()
    c.decls( "removeAllSegments" ).call_policies = call_policies.return_self()

def fix_ConnectivityEditor(c):
    c.decls( "connect" ).call_policies = call_policies.return_self()
    c.decls( "disconnect" ).call_policies = call_policies.return_self()
    c.decls( "disconnectAll" ).call_policies = call_policies.return_self()

def fix_MGNum(c):
    c.add_declaration_code( "#include \"mgid.h\"" )
    c.add_declaration_code( "#include \"mgidx.h\"" )
    c.add_declaration_code( "#include \"mgname.h\"" )
    c.add_declaration_code( "#include \"mgnum.h\"" )
    c.add_declaration_code( "#include \"molgroups.h\"")

fix_MGIdx = fix_MGNum
fix_MGName = fix_MGNum

def fix_MolNum(c):
    c.add_declaration_code( "#include \"molid.h\"" )
    c.add_declaration_code( "#include \"molidx.h\"" )
    c.add_declaration_code( "#include \"molnum.h\"" )
    c.add_declaration_code( "#include \"molname.h\"" )
    c.add_declaration_code( "#include \"molgroup.h\"" )
    c.add_declaration_code( "#include \"molgroups.h\"" )
    c.add_declaration_code( "#include \"mover.hpp\"" )

fix_MolName = fix_MolNum
fix_MolIdx = fix_MolNum

def fix_MolInfo(c):
    c.add_declaration_code( "#include \"moleculeinfodata.h\"" )
    c.add_declaration_code( "#include \"atomselection.h\"" )

special_code = { "Atom" : fix_Atom,
                 "AtomEditor" : fix_AtomEditor,
                 "AtomSelection" : fix_AtomSelection,
                 "AtomStructureEditor" : fix_AtomStructureEditor,
                 "CGEditor" : fix_CGEditor,
                 "CGStructureEditor" : fix_CGStructureEditor,
                 "ChainEditor" : fix_ChainEditor,
                 "ChainStructureEditor" : fix_ChainStructureEditor,
                 "ResEditor" : fix_ResEditor,
                 "ResStructureEditor" : fix_ResStructureEditor,
                 "SegEditor" : fix_SegEditor,
                 "SegStructureEditor" : fix_SegStructureEditor,
                 "MolEditor" : fix_MolEditor,
                 "MolStructureEditor" : fix_MolStructureEditor,
                 "ConnectivityEditor" : fix_ConnectivityEditor,
                 "MGName" : fix_MGName,
                 "MGIdx" : fix_MGIdx,
                 "MGNum" : fix_MGNum,
                 "MolNum" : fix_MolNum,
                 "MolName" : fix_MolName,
                 "MolIdx" : fix_MolIdx,
                 "MolInfo" : fix_MolInfo }

