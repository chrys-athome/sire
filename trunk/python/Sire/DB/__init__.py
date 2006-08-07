#############################
##
## The SireDB library.
##
## This contains all of the classes that are used to 
## provide database type functionality, e.g. 
## this provides the base of the forcefield databases
##

import Sire.CAS
import Sire.Base

# Import all of the classes and functions from the C++ library
from Sire.DB._DB import *

# Add some functionality to classes derived from ParameterDB
def __ParameterDB__deepcopy__(self, memo):
    """Return a deep copy of the database"""
    new_db = self.__class__()

    #dump the contents of the database to a string
    dump = self.dumpToString()

    #load the contents into the new database
    new_db.loadFromString(dump)

    #return the new database
    return new_db

ParameterDB.__deepcopy__ = __ParameterDB__deepcopy__

def __ParameterDB__isA(self, class_type):
    """Return whether or not the database contains the component 'class_type', 
       e.g. db.isA( ChargeDB ) would return whether or not the database
       contains the ChargeDB component. You can also pass a tuple of classes
       to this function, e.g. db.isA( (ChargeDB,BondDB) ) will return whether
       or not this database has a ChargeDB component *or* a BondDB component."""
    return isinstance(self, class_type)

ParameterDB.isA = __ParameterDB__isA

def __MatchData__isA(self, class_type):
    """Return whether or not the MatchData is an instance of the type 'class_type',
       e.g. match.isA( MatchAtom ) would return whether or not the match 
       was a set of atom criteria."""
    return isinstance(self, class_type)

MatchData.isA = __MatchData__isA

# Expose the MatchHelper classes
mol_name = match_mol_name()
mol_group = match_mol_group()
mol_state = match_mol_state()

res_name = match_single_res_name()
res0_name = match_res_name(0)
res1_name = match_res_name(1)
res2_name = match_res_name(2)
res3_name = match_res_name(3)

res_num = match_single_res_num()
res0_num = match_res_num(0)
res1_num = match_res_num(1)
res2_num = match_res_num(2)
res3_num = match_res_num(3)

res_alias = match_single_res_alias()
res0_alias = match_res_alias(0)
res1_alias = match_res_alias(1)
res2_alias = match_res_alias(2)
res3_alias = match_res_alias(3)

res_res_bond_via = match_single_res_res_bond_via()
res0_res_bond_via = match_res_res_bond_via(0)
res1_res_bond_via = match_res_res_bond_via(1)
res2_res_bond_via = match_res_res_bond_via(2)
res3_res_bond_via = match_res_res_bond_via(3)

no_res_res_bond_via = match_single_no_res_res_bond_via()
no_res0_res_bond_via = match_no_res_res_bond_via(0)
no_res1_res_bond_via = match_no_res_res_bond_via(1)
no_res2_res_bond_via = match_no_res_res_bond_via(2)
no_res3_res_bond_via = match_no_res_res_bond_via(3)

atom_name = match_single_atom_name()
atom0_name = match_atom_name(0)
atom1_name = match_atom_name(1)
atom2_name = match_atom_name(2)
atom3_name = match_atom_name(3)

atom_num = match_single_atom_num()
atom0_num = match_atom_num(0)
atom1_num = match_atom_num(1)
atom2_num = match_atom_num(2)
atom3_num = match_atom_num(3)

atom_type = match_single_atom_type()
atom0_type = match_atom_type(0)
atom1_type = match_atom_type(1)
atom2_type = match_atom_type(2)
atom3_type = match_atom_type(3)

atom_element = match_single_atom_element()
atom0_element = match_atom_element(0)
atom1_element = match_atom_element(1)
atom2_element = match_atom_element(2)
atom3_element = match_atom_element(3)

def __to_table_type_name(table):
    """Get and return the type name of 'table'. This 
       will return the type name if this is an object or
       class type, or will return the object itself
       if it cannot determine whether it is a table type"""
       
    is_subclass = False
    
    try:
        is_subclass = issubclass(table, TableBase)
    except:
        is_subclass = isinstance(table, TableBase)
        
    if is_subclass:
        return table.typeName()
    else:
        return table

# add functionality to the ParameterTable - allows easy casting
# via use of the Python class
__ParameterTable_old_asA = ParameterTable.asA
__ParameterTable_old_isA = ParameterTable.isA
__ParameterTable_old_addTable = ParameterTable.addTable
__ParameterTable_old_setTable = ParameterTable.setTable
__ParameterTable_old_removeTable = ParameterTable.removeTable

def __new_asA(table, arg):
    return __ParameterTable_old_asA(table, __to_table_type_name(arg))

def __new_isA(table, arg):
    return __ParameterTable_old_isA(table, __to_table_type_name(arg))

def __new_addTable(table, arg):

    if isinstance(arg, TableBase):
        __ParameterTable_old_addTable(table, arg)
    else:
        __ParameterTable_old_addTable(table, __to_table_type_name(arg))

def __new_setTable(table, arg):

    if isinstance(arg, TableBase):
        __ParameterTable_old_setTable(table, arg)
    else:
        __ParameterTable_old_setTable(table, __to_table_type_name(arg))

def __new_removeTable(table, arg):
    __ParameterTable_old_removeTable(table, __to_table_type_name(arg))
    
ParameterTable.asA = __new_asA
ParameterTable.isA = __new_isA
ParameterTable.removeTable = __new_removeTable
ParameterTable.addTable = __new_addTable
ParameterTable.setTable = __new_setTable

__old_using_parameters_init = using_parameters.__init__

def __new_using_parameters_init(self, *args):
    
    #turn the argument list into a list of class names
    typenames = Sire.Qt.QStringList()
    
    for arg in args:
        typenames.append( arg.typeName() )
        
    __old_using_parameters_init(self, typenames)
    
using_parameters.__init__ = __new_using_parameters_init

__old_using_relationships_init = using_relationships.__init__

def __new_using_relationships_init(self, *args):
    
    #turn the argument list into a list of class names
    typenames = Sire.Qt.QStringList()
    
    for arg in args:
        typenames.append( arg.typeName() )
        
    __old_using_relationships_init(self, typenames)

using_relationships.__init__ = __new_using_relationships_init
