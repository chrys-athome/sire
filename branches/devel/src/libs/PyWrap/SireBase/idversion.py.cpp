
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
