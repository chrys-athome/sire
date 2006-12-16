
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
