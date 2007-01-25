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

#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#include "wrap_SireDB.h"

namespace SireDB
{

void export_AtomType();
void export_Match();
void export_MatchHelpers();
void export_DB();
void export_ParameterDB();
void export_AtomTypeDB();
void export_RelateDB();
void export_ParameterTable();
void export_AtomTable();
void export_AtomTypeTable();

void export_using_database();
void export_assign_atoms();
void export_assign_parameters();

void SIREDB_EXPORT export_SireDB()
{
    export_AtomType();
    export_Match();
    export_MatchHelpers();
    export_DB();
    export_ParameterDB();
    export_AtomTypeDB();
    export_RelateDB();
    export_ParameterTable();
    export_AtomTable();
    export_AtomTypeTable();

    export_using_database();
    export_assign_atoms();
    export_assign_parameters();
}

}
