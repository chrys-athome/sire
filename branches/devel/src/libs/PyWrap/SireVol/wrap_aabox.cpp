
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
