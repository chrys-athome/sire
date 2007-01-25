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

#include "SireDB/using_database.h"
#include "SireDB/using_parameters.hpp"
#include "SireDB/using_relationships.hpp"
#include "SireDB/overwriteparams.h"

#include "SirePy/str.hpp"

using namespace boost::python;
using namespace SirePy;

namespace SireDB
{

void  
SIREDB_EXPORT
export_using_database()
{
    class_<AssignInstruction, boost::noncopyable>("AssignInstruction", no_init)
    ;
    
    class_<OverWriteParams, bases<AssignInstruction> >( 
                        "OverWriteParams", init< optional<bool> >() )
        
        .def( "__str__", &__str__<OverWriteParams> )
        
        .def( self == bool() )
        
        .def( "toString", &OverWriteParams::toString )
        
        .def( "value", &OverWriteParams::value )
    ;
    
    implicitly_convertible<OverWriteParams,bool>();
    
    class_<using_database, bases<AssignInstruction, QStringList>, 
                           boost::noncopyable>("using_database", no_init)
    ;
    
    class_<using_parameters_base, bases<using_database> >
                      ( "using_parameters", init<>() )
    
        .def( init<const QStringList&>() )
        .def( init<const using_parameters_base&>() )
        
    ;
    
    class_<using_relationships_base, bases<using_database> >
                      ( "using_relationships", init<>() )
    
        .def( init<const QStringList&>() )
        .def( init<const using_relationships_base&>() )
    ;
}

}
