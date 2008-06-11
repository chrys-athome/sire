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
chainprops = Properties()
cgprops = Properties()
resprops = Properties()
segprops = Properties()

active_headers = pickle.load( open("active_headers.data") )

return_const = "bp::return_value_policy<bp::copy_const_reference>()"
return_self = "bp::return_self< >()"

def fix_MolView(c, molview, props):
   #now add in all of the header files
   for header in props.dependencies():
       c.add_declaration_code( "#include %s" % header )

   #add accessor functions for all of the view properties
   for property in props.properties():
       p = property[0]
       prop = property[1].replace("::","_")

       c.add_registration_code( "def( \"_get_property_%s\", &%s::property<%s>, %s)" \
                                      % (prop, molview, p, return_const) )
       c.add_registration_code( "def( \"_get_metadata_%s\", get_Metadata_%s_function1, %s)" \
                                      % (prop, prop, return_const) )
       c.add_registration_code( "def( \"_get_metadata_%s\", &get_Metadata_%s_function2, %s)" \
                                      % (prop, prop, return_const) )

       c.add_declaration_code( """ const %s& get_Metadata_%s_function1(const %s &atom,
                                   const QString &metakey){ return atom.metadata<%s>(metakey); }""" \
                                      % (p, prop, molview, p) )
 
       c.add_declaration_code( """ const %s& get_Metadata_%s_function2(const %s &atom,
                                   const QString &key, const QString &metakey){
                                        return atom.metadata<%s>(key, metakey); }""" \
                                      % (p, prop, molview, p) )

def fix_Atom(c):
   fix_MolView(c, "SireMol::Atom", atomprops)

def fix_Chain(c):
   fix_MolView(c, "SireMol::Chain", chainprops)

def fix_CutGroup(c):
   fix_MolView(c, "SireMol::CutGroup", cgprops)

def fix_Residue(c):
   fix_MolView(c, "SireMol::Residue", resprops)

def fix_Segment(c):
   fix_MolView(c, "SireMol::Segment", segprops)

def fix_MolViewEditorBase(c, molview, props):
   c.decls( "removeProperty" ).call_policies = call_policies.return_self()
   c.decls( "removeMetadata" ).call_policies = call_policies.return_self()

   #now add the code to set properties and metadata
   for header in props.dependencies():
       c.add_declaration_code( "#include %s" % header )

   #add accessor functions for all of the atom properties
   for property in props.properties():
       p = property[0]
       prop = property[1].replace("::","_")

       c.add_registration_code( """def( \"_set_property_%s\", 
                                   &%s::setProperty<%s>, %s )""" \
                                   % (p.replace("::","_"), molview, p, return_self ) )

       c.add_registration_code( "def( \"_set_metadata_%s\", &set_Metadata_%s_function1, %s)" \
                                   % (p.replace("::","_"), prop, return_self) )

       c.add_registration_code( "def( \"_set_metadata_%s\", &set_Metadata_%s_function2, %s)" \
                                   % (p.replace("::","_"), prop, return_self) )

       c.add_declaration_code( """%s& set_Metadata_%s_function1(
                                  %s &molview,
                                   const QString &metakey, const %s &p)
                                   { return molview.setMetadata<%s>(metakey, p); }""" \
                                      % (molview, prop, molview, p, p) )

       c.add_declaration_code( """%s& set_Metadata_%s_function2(
                                  %s &molview,
                                   const QString &key, const QString &metakey, const %s &p)
                                   { return molview.setMetadata<%s>(key, metakey, p); }""" \
                                      % (molview, prop, molview, p, p) )

def fix_AtomEditorBase(c):
    fix_MolViewEditorBase(c, "SireMol::AtomEditorBase", atomprops)

def fix_ChainEditorBase(c):
    fix_MolViewEditorBase(c, "SireMol::ChainEditorBase", chainprops)

def fix_CGEditorBase(c):
    fix_MolViewEditorBase(c, "SireMol::CGEditorBase", cgprops)

def fix_ResEditorBase(c):
    fix_MolViewEditorBase(c, "SireMol::ResEditorBase", resprops)

def fix_SegEditorBase(c):
    fix_MolViewEditorBase(c, "SireMol::SegEditorBase", segprops)

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

def fix_Mover(c):
    c.decls("mapInto").call_policies = call_policies.return_self()
    c.decls("translate").call_policies = call_policies.return_self()
    c.decls("rotate").call_policies = call_policies.return_self()
    c.decls("changeFrame").call_policies = call_policies.return_self()
    c.decls("change").call_policies = call_policies.return_self()
    c.decls("set").call_policies = call_policies.return_self()
    c.decls("setAll").call_policies = call_policies.return_self()
    c.decls("alignTo").call_policies = call_policies.return_self()

    #also include all of the header files included in mover.cpp
    for header in active_headers["mover.h"].dependencies():
        c.add_declaration_code( "#include %s" % header )    

special_code = { "SireMol::Atom" : fix_Atom,
                 "SireMol::Editor<SireMol::AtomEditor, SireMol::Atom>" : fix_AtomEditorBase,
                 "SireMol::AtomEditor" : fix_AtomEditor,
                 "SireMol::AtomSelection" : fix_AtomSelection,
                 "SireMol::AtomStructureEditor" : fix_AtomStructureEditor,
                 "SireMol::Mover<SireMol::Atom>" : fix_Mover,
                 "SireMol::Mover<SireMol::Selector<SireMol::Atom> >" : fix_Mover,

                 "SireMol::CutGroup" : fix_CutGroup,
                 "SireMol::Editor<SireMol::CGEditor, SireMol::CutGroup>" : fix_CGEditorBase,
                 "SireMol::CGEditor" : fix_CGEditor,
                 "SireMol::CGStructureEditor" : fix_CGStructureEditor,
                 "SireMol::Mover<SireMol::CutGroup>" : fix_Mover,
                 "SireMol::Mover<SireMol::Selector<SireMol::CutGroup> >" : fix_Mover,

                 "SireMol::Chain" : fix_Chain,
                 "SireMol::Editor<SireMol::ChainEditor, SireMol::Chain>" : fix_ChainEditorBase,
                 "SireMol::ChainEditor" : fix_ChainEditor,
                 "SireMol::ChainStructureEditor" : fix_ChainStructureEditor,
                 "SireMol::Mover<SireMol::Chain>" : fix_Mover,
                 "SireMol::Mover<SireMol::Selector<SireMol::Chain> >" : fix_Mover,

                 "SireMol::Residue" : fix_Residue,
                 "SireMol::Editor<SireMol::ResEditor, SireMol::Residue>" : fix_ResEditorBase,
                 "SireMol::ResEditor" : fix_ResEditor,
                 "SireMol::ResStructureEditor" : fix_ResStructureEditor,
                 "SireMol::Mover<SireMol::Residue>" : fix_Mover,
                 "SireMol::Mover<SireMol::Selector<SireMol::Residue> >" : fix_Mover,

                 "SireMol::Segment" : fix_Segment,
                 "SireMol::Editor<SireMol::SegEditor, SireMol::Segment>" : fix_SegEditorBase,
                 "SireMol::SegEditor" : fix_SegEditor,
                 "SireMol::SegStructureEditor" : fix_SegStructureEditor,
                 "SireMol::Mover<SireMol::Segment>" : fix_Mover,
                 "SireMol::Mover<SireMol::Selector<SireMol::Segment> >" : fix_Mover,

                 "SireMol::MolEditor" : fix_MolEditor,
                 "SireMol::MolStructureEditor" : fix_MolStructureEditor,
                 "SireMol::Mover<SireMol::Molecule>" : fix_Mover,
                 "SireMol::Mover<SireMol::PartialMolecule>" : fix_Mover,
                 "SireMol::Mover<SireMol::ViewsOfMol>" : fix_Mover,

                 "SireMol::ConnectivityEditor" : fix_ConnectivityEditor,
                 "SireMol::MGName" : fix_MGName,
                 "SireMol::MGIdx" : fix_MGIdx,
                 "SireMol::MGNum" : fix_MGNum,
                 "SireMol::MolNum" : fix_MolNum,
                 "SireMol::MolName" : fix_MolName,
                 "SireMol::MolIdx" : fix_MolIdx,
                 "SireMol::MolInfo" : fix_MolInfo }

