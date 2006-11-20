
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

void  
SIREDB_EXPORT
export_MatchHelpers2()
{
    class_<match_single_res_name>("match_single_res_name", init<>())
        .def(self == QString())
    ;
    
    class_<match_single_res_num>("match_single_res_num", init<>())
        .def(self == ResNum())
    ;
    
    class_<match_single_res_alias>("match_single_res_alias", init<>())
        .def(self == QString())
    ;
    
    class_<match_single_res_res_bond_via>("match_single_res_res_bond_via", init<>())
        .def(self == QString())
    ;
    
    class_<match_single_no_res_res_bond_via>("match_single_no_res_res_bond_via", init<>())
        .def(self == QString())
    ;
    
    class_<match_atom_type>("match_atom_type", init<uint>())
        .def(self == AtomType())
        .def(self == QString())
    ;
    
    class_<match_atom_element>("match_atom_element", init<uint>())
        .def(self == QString())
        .def(self == Element())
        .def(self == uint())
    ;
    
    class_<match_single_atom_type>("match_single_atom_type", init<>())
        .def(self == AtomType())
        .def(self == QString())
    ;
    
    class_<match_single_atom_element>("match_single_atom_element", init<>())
        .def(self == QString())
        .def(self == Element())
        .def(self == uint())
    ;
    
}

}
