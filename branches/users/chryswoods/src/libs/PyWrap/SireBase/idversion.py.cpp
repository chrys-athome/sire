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

#include <boost/python.hpp>

#include "SireBase/idmajminversion.h"
#include "SireBase/idversion.h"

#include "SirePy/str.hpp"

using namespace boost::python;
using namespace SirePy;

namespace SireBase
{

void SIREBASE_EXPORT export_IDVersion()
{
    class_<MajMinVersion>( "MajMinVersion", init<>() )
        .def( init<const MajMinVersion&>() )
        
        .def( self == self )
        .def( self != self )
        
        .def( "__str__", &__str__<MajMinVersion> )
        
        .def( "toString", &MajMinVersion::toString )
        
        .def( "major", &MajMinVersion::major )
        .def( "minor", &MajMinVersion::minor )
        
        .def( "incrementMajor", &MajMinVersion::incrementMajor )
        .def( "incrementMinor", &MajMinVersion::incrementMinor )
        
        .def( "sameMajorVersion", &MajMinVersion::sameMajorVersion )
        .def( "sameMinorVersion", &MajMinVersion::sameMinorVersion )
        .def( "sameVersion", &MajMinVersion::sameVersion )
        
        .def( "assertSameMajorVersion", &MajMinVersion::assertSameMajorVersion )
        .def( "assertSameMinorVersion", &MajMinVersion::assertSameMinorVersion )
        .def( "assertSameVersion", &MajMinVersion::assertSameVersion )
    ;

    class_<IDVersion>( "IDVersion", init<>() )
        .def( init<const IDVersion&>() )
        
        .def( self == self )
        .def( self != self )
        
        .def( "__str__", &__str__<IDVersion> )
        
        .def( "toString", &IDVersion::toString )
        
        .def( "ID", &IDVersion::ID )
        .def( "version", &IDVersion::version )
        
        .def( "incrementID", &IDVersion::incrementID )
        .def( "incrementVersion", &IDVersion::incrementVersion )
        
        .def( "sameID", &IDVersion::sameID )
        .def( "sameVersion", &IDVersion::sameVersion )
        .def( "sameIDAndVersion", &IDVersion::sameIDAndVersion )
        
        .def( "assertSameID", &IDVersion::assertSameID )
        .def( "assertSameVersion", &IDVersion::assertSameVersion )
        .def( "assertSameIDAndVersion", &IDVersion::assertSameIDAndVersion )
    ;

    class_<IDMajMinVersion>( "IDMajMinVersion", init<>() )
        .def( init<const IDMajMinVersion&>() )
        
        .def( self == self )
        .def( self != self )
        
        .def( "__str__", &__str__<IDMajMinVersion> )
        
        .def( "toString", &IDMajMinVersion::toString )
        
        .def( "ID", &IDMajMinVersion::ID )
        .def( "major", &IDMajMinVersion::major )
        .def( "minor", &IDMajMinVersion::minor )
        
        .def( "ID", &IDMajMinVersion::ID )
        .def( "incrementMajor", &IDMajMinVersion::incrementMajor )
        .def( "incrementMinor", &IDMajMinVersion::incrementMinor )
        
        .def( "sameID", &IDMajMinVersion::sameID )
        .def( "sameIDAndVersion", &IDMajMinVersion::sameIDAndVersion )
        .def( "sameIDAndMajorVersion", 
                              &IDMajMinVersion::sameIDAndMajorVersion )
        
        .def( "sameMajorVersion", &IDMajMinVersion::sameMajorVersion )
        .def( "sameMinorVersion", &IDMajMinVersion::sameMinorVersion )
        .def( "sameVersion", &IDMajMinVersion::sameVersion )
        
        .def( "assertSameID", &IDMajMinVersion::assertSameID )
        .def( "assertSameIDAndVersion", &IDMajMinVersion::assertSameIDAndVersion )
        .def( "assertSameIDAndMajorVersion",
                           &IDMajMinVersion::assertSameIDAndMajorVersion )
        
        .def( "assertSameMajorVersion", &IDMajMinVersion::assertSameMajorVersion )
        .def( "assertSameMinorVersion", &IDMajMinVersion::assertSameMinorVersion )
        .def( "assertSameVersion", &IDMajMinVersion::assertSameVersion )
    ;

}

}
