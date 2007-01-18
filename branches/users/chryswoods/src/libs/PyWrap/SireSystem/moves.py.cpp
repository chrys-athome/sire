
#include <boost/python.hpp>

#include "SireSystem/moves.h"
#include "SireSystem/simsystem.h"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;

using namespace SireQt;

namespace SireSystem
{

void SIRESYSTEM_EXPORT export_Moves()
{
    class_<MovesBase, boost::noncopyable>( "MovesBase", no_init )
    
        .def( "what", &MovesBase::what )
        .def( "count", &MovesBase::count )
        
        .def( "nextMove", &MovesBase::nextMove,
                   return_internal_reference<1, with_custodian_and_ward<1,2> >() )
        
        .def( "initialise", &MovesBase::initialise )
    ;
    
    class_<SameMoves, bases<MovesBase> >( "SameMoves", init<>() )
        .def( init<const Move&>() )
    
        .def( "__rrshift__", &__rrshift__QDataStream<SameMoves>,
                    return_internal_reference<1, with_custodian_and_ward<1,2> >() )
        .def( "__rlshift__", &__rlshift__QDataStream<SameMoves>,
                    return_internal_reference<1, with_custodian_and_ward<1,2> >() )
    ;
    
    class_<Moves>( "Moves", init<>() )
        .def( init<const Move&>() )
        .def( init<const MovesBase&>() )
        .def( init<const Moves&>() )
        
        .def( "__rrshift__", &__rrshift__QDataStream<Moves>,
                    return_internal_reference<1, with_custodian_and_ward<1,2> >() )
        .def( "__rlshift__", &__rlshift__QDataStream<Moves>,
                    return_internal_reference<1, with_custodian_and_ward<1,2> >() )
        
        .def( "initialise", &Moves::initialise )
        .def( "count", &Moves::count )
        .def( "run", &Moves::run )
        .def( "resume", &Moves::resume )
        .def( "rerun", &Moves::rerun )
        .def( "play", &Moves::play )
        .def( "pause", &Moves::pause )
        .def( "stop", &Moves::stop )
        
        .def( "percentProgress", &Moves::percentProgress )
    ;
    
    implicitly_convertible<Move,Moves>();
    implicitly_convertible<MovesBase,Moves>();
}

}
