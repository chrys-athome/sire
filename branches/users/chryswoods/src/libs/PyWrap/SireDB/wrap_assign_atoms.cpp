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

#include "SireDB/assign_atoms.h"
#include "SireDB/parametertable.h"
#include "SireDB/parameterdb.h"
#include "SireDB/using_database.h"

#include "SireMol/molecule.h"

using namespace boost::python;

using namespace SireMol;

namespace SireDB
{

void wrap_assignParameters3( const AssignBase &assignbase,
                             const Molecule &molecule,
                             ParameterTable &param_table,
                             ParameterDB &database,
                             const MatchMRData &matchmr )
{
    return assignbase.assignParameters(molecule, param_table, database, matchmr);
}

void wrap_assignParameters2( const AssignBase &assignbase,
                             const Molecule &molecule,
                             ParameterTable &param_table,
                             ParameterDB &database )
{
    return assignbase.assignParameters(molecule, param_table, database);
}

void  
SIREDB_EXPORT
export_assign_atoms()
{

    class_<AssignBase, bases<AssignInstruction>, boost::noncopyable>
                                      ("AssignBase", no_init)

        .def( "addDataBase", (void (AssignBase::*)(const using_database&))
                        &AssignBase::addDataBase )

        .def( "parameterDataBases", &AssignBase::parameterDataBases )
        .def( "relationshipDataBases", &AssignBase::relationshipDataBases )

        .def( "assignParameters", &wrap_assignParameters3 )
        .def( "assignParameters", &wrap_assignParameters2 )

    ;

    class_< assign_atoms, bases<AssignBase> >( "assign_atoms", init<>() )

        .def( init< optional<const using_database&, const using_database&,
                             const using_database&, const using_database&,
                             const using_database&, const using_database&,
                             const using_database&, const using_database&,
                             const using_database&, const using_database&> >() )

        .def( init< const QSet<AtomIndex>&,
                    optional<const using_database&, const using_database&,
                             const using_database&, const using_database&,
                             const using_database&, const using_database&,
                             const using_database&, const using_database&,
                             const using_database&, const using_database&> >() )

        .def( "typeName", &assign_atoms::typeName )
    ;

}

}
