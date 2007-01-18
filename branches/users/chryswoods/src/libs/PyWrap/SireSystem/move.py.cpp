
#include <boost/python.hpp>

#include "SireSystem/move.h"
#include "SireSystem/simsystem.h"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;

using namespace SireQt;

namespace SireSystem
{

void SIRESYSTEM_EXPORT export_Move()
{
    class_<MoveBase, boost::noncopyable>( "MoveBase", no_init )
    
        .def( "what", &MoveBase::what )
        .def( "initialise", &MoveBase::initialise )
        .def( "move", &MoveBase::move )
    ;
    
    class_<Move>( "Move", init<>() )
        .def( init<const MoveBase&>() )
        .def( init<const Move&>() )
        
        .def( "__rrshift__", &__rrshift__QDataStream<Move>,
                    return_internal_reference<1, with_custodian_and_ward<1,2> >() )
        .def( "__rlshift__", &__rlshift__QDataStream<Move>,
                    return_internal_reference<1, with_custodian_and_ward<1,2> >() )
        
        .def( "initialise", &Move::initialise )
        .def( "move", &Move::move )
    ;
    
    implicitly_convertible<MoveBase, Move>();
}

}
