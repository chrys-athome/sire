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
