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

#include "SireSystem/system.h"
#include "SireSystem/move.h"
#include "SireSystem/moves.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/moleculegroup.h"

#include "SireFF/ffexpression.h"
#include "SireFF/forcefields.h"

#include "SireCAS/function.h"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;

using namespace SireQt;
using namespace SireMol;
using namespace SireFF;
using namespace SireCAS;

namespace SireSystem
{

Moves system_run_1_1(System &system, const Move &move)
{
    return system.run(move);
}

Moves system_run_1_2(System &system, const Move &move, quint32 nmoves)
{
    return system.run(move, nmoves);
}

void SIRESYSTEM_EXPORT export_System()
{
    class_<System>( "System", init<>() )
        .def( init<const QString&>() )
        .def( init<const System&>() )

        .def( self == self )
        .def( self != self )

        .def( "__rrshift__", &__rrshift__QDataStream<System>,
                    return_internal_reference<1, with_custodian_and_ward<1,2> >() )
        .def( "__rlshift__", &__rlshift__QDataStream<System>,
                    return_internal_reference<1, with_custodian_and_ward<1,2> >() )

        .def( "forceFields", (const ForceFields& (System::*)() const)
                    &System::forceFields, return_value_policy<copy_const_reference>() )

        .def( "info", (const SystemData& (System::*)() const)
                    &System::info, return_value_policy<copy_const_reference>() )

        .def( "run", &system_run_1_1 )
        .def( "run", &system_run_1_2 )

        .def( "run", (Moves (System::*)(const Moves&))
                    &System::run )
        .def( "run", (Moves (System::*)(const Moves&, quint32))
                    &System::run )

        .def( "prepareForSimulation", (void (System::*)())
                    &System::prepareForSimulation )

        .def( "ID", (SystemID (System::*)() const)
                    &System::ID )

        .def( "version", (const Version& (System::*)() const)
                    &System::version, return_value_policy<copy_const_reference>() )

    ;
}

}
