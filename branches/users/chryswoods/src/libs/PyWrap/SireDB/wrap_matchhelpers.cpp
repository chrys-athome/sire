/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

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

void  
SIREDB_EXPORT
export_MatchHelpers()
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
