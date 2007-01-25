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

#include "SireMM/atomiccharges.h"

#include "SireQt/qvector.hpp"

using namespace boost::python;

using namespace SireQt;
using namespace SireMol;

namespace SireMM
{

void SIREMM_EXPORT export_AtomicCharges()
{
    //Atomic charges is based on QVector< QVector<ChargeParameter> >
    // Wrap up these first....

    //wrap up QVector<ChargeParameter>
    wrap_QVector<ChargeParameter>( "QVector_ChargeParameter_" );

    //now QVector< QVector<ChargeParameter> >
    wrap_QVector< QVector<ChargeParameter> >( "QVector_QVector_ChargeParameter_" );

    //now AtomicCharges
    class_<AtomicCharges,
           bases< PropertyBase, QVector< QVector<ChargeParameter> > >
          >( "AtomicCharges", init<>() )


        .def( init<const QVector< QVector<ChargeParameter> >&>() )
        .def( init<const QVector<ChargeParameter>&>() )
        .def( init<const Property&>() )
        .def( init<const AtomicCharges&>() )
    ;
}

}
