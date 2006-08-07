
/**
  * This file contains the boost::python wrapping of the match helper classes
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireDB/matchhelpers.h"
#include "SireDB/matchmra.h"
#include "SireDB/matchmradata.h"
#include "SireDB/matchatomtype.h"
#include "SireDB/matchatomtypedata.h"
#include "SireDB/matchres.h"
#include "SireDB/matchmol.h"
#include "SireDB/matchatom.h"

using namespace boost::python;

namespace SireDB
{

void export_MatchHelpers2();

void export_MatchHelpers()
{
    class_<match_mol_name>("match_mol_name", init<>())
        .def(self == QString())
    ;

    class_<match_mol_group>("match_mol_group", init<>())
        .def(self == QString())
    ;

    class_<match_mol_state>("match_mol_state", init<>())
        .def(self == QString())
    ;
    
    class_<match_atom_name>("match_atom_name", init<uint>())
        .def(self == QString())
    ;
    
    class_<match_atom_num>("match_atom_num", init<uint>())
        .def(self == AtomNum())
    ;
    
    class_<match_single_atom_name>("match_single_atom_name", init<>())
        .def(self == QString())
    ;
    
    class_<match_single_atom_num>("match_single_atom_num", init<>())
        .def(self == AtomNum())
    ;
    
    class_<match_res_name>("match_res_name", init<uint>())
        .def(self == QString())
    ;
    
    class_<match_res_num>("match_res_num", init<uint>())
        .def(self == ResNum())
    ;
    
    class_<match_res_alias>("match_res_alias", init<uint>())
        .def(self == QString())
    ;
    
    class_<match_res_res_bond_via>("match_res_res_bond_via", init<uint>())
        .def(self == QString())
    ;
    
    class_<match_no_res_res_bond_via>("match_no_res_res_bond_via", init<uint>())
        .def(self == QString())
    ;
    
    export_MatchHelpers2();
}

}
