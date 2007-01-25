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

/**
  * This file contains the boost::python wrapping of an AABox
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireVol/aabox.h"
#include "SireVol/coordgroup.h"

using namespace boost::python;

namespace SireVol
{

void  
SIREVOL_EXPORT
export_AABox()
{

    class_<AABox>("AABox",init<>())
        .def(init<const Vector&, const Vector&>())
        .def(init<const CoordGroup&>())

        .def( self == self )
        .def( self != self )

        .def( "recalculate", (void (AABox::*)(const CoordGroup&))
                                &AABox::recalculate )

        .def( "translate", (void (AABox::*)(const Vector&))
                                &AABox::translate )

        .def( "center", (const Vector& (AABox::*)() const)
                                &AABox::center,
                                return_value_policy<copy_const_reference>() )

        .def( "halfExtents", (const Vector& (AABox::*)() const)
                                &AABox::halfExtents,
                                return_value_policy<copy_const_reference>() )

        .def( "maxCoords", (Vector (AABox::*)() const)
                                &AABox::maxCoords )

        .def( "minCoords", (Vector (AABox::*)() const)
                                &AABox::minCoords )

        .def( "radius", (double (AABox::*)() const)
                                &AABox::radius )

        .def( "withinDistance", (bool (AABox::*)(double,
                                                 const AABox&) const)
                                &AABox::withinDistance )

        .def( "intersects", (bool (AABox::*)(const AABox&) const)
                                &AABox::intersects )
    ;
}

}
