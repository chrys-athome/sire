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
