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

#include "SirePy/str.hpp"
#include "SireQt/qdatastream.hpp"

#include "SireDB/parameterdb.h"
#include "SireDB/tablebase.h"
#include "SireDB/parametertable.h"
#include "SireDB/assign_parameters.h"

#include "SireMol/molecule.h"

using namespace boost::python;
using namespace SirePy;
using namespace SireQt;
using namespace SireMol;

namespace SireDB
{

ParameterTable wrap_assign3(ParameterDB &db, const Molecule &molecule,
                            const assign_parameters &assign_instruct,
                            const MatchMRData &matchmr)
{
    return db.assign(molecule, assign_instruct, matchmr);
}

ParameterTable wrap_assign2(ParameterDB &db, const Molecule &molecule,
                            const assign_parameters &assign_instruct)
{
    return db.assign(molecule, assign_instruct);
}

void  
SIREDB_EXPORT
export_ParameterDB()
{
    
    class_<ParameterDB>( "ParameterDB", init<>() )
    
        .def( init<const ParameterDB&>() )
    
        .def( "__str__",  &__str__<ParameterDB> )
        
        .def( "__rlshift__", &__rlshift__QDataStream<ParameterDB>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<ParameterDB>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )

        .def( "addComponent", (DBBase& (ParameterDB::*)(const char*))
                    &ParameterDB::addComponent,
                        return_internal_reference<1, with_custodian_and_ward<1,2> >() )
    
        .def( "addLog", &ParameterDB::addLog )
        
        .def( "asA", (DBBase& (ParameterDB::*)(const QString&))
                    &ParameterDB::asA,
                        return_internal_reference<1, with_custodian_and_ward<1,2> >() )

        .def( "assign", &wrap_assign3 )
        .def( "assign", &wrap_assign2 )

        .def( "clear", &ParameterDB::clear )
                  
        .def( "clearLog", &ParameterDB::clearLog )
        
        .def( "dump", (void (ParameterDB::*)(const QString&))
                    &ParameterDB::dump )
        .def( "dump", (void (ParameterDB::*)(QTextStream&))
                    &ParameterDB::dump )
    
        .def( "dumpToString", &ParameterDB::dumpToString )

        .def( "isA", (bool (ParameterDB::*)(const QString&) const)
                    &ParameterDB::isA )
        
        .def( "getLog", &ParameterDB::getLog )
    
        .def( "load", (void (ParameterDB::*)(QTextStream&))
                    &ParameterDB::load )

        .def( "load", (void (ParameterDB::*)(const QString&))
                    &ParameterDB::load )
        .def( "loadFromString", &ParameterDB::loadFromString )

        .def( "nParameters", &ParameterDB::nParameters )
        .def( "nRelationships", &ParameterDB::nRelationships )
        
        .def( "toString", &ParameterDB::toString )
        
        .def( "typeName", &ParameterDB::typeName ).staticmethod("typeName")

        .def( "types", &ParameterDB::types )
    
        .def( "what", &ParameterDB::what )
    ;

}

}
