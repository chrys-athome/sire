
/**
  * This file contains the boost::python wrapping of an AABox
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/aabox.h"
#include "SireMol/atom.h"
#include "SireMol/atomset.h"

using namespace boost::python;

namespace SireMol
{

void export_AABox()
{
    /** Function overloads */
    void (AABox::*wrap_recalculate1)(const AtomVector&) = &AABox::recalculate;
    void (AABox::*wrap_recalculate2)(const AtomSet&) = &AABox::recalculate;
    
    class_<AABox>("AABox",init<>())
             .def(init<const Vector&, const Vector&>())
             .def(init<const AtomVector&>())
             .def(init<const AtomSet&>())
             .def("translate", &AABox::translate)
             .def("recalculate",wrap_recalculate1)
             .def("recalculate",wrap_recalculate2)
             .def("maxCoords",&AABox::maxCoords)
             .def("minCoords",&AABox::minCoords)
             .def("radius",&AABox::radius)
             .def("center",&AABox::center, 
                    return_internal_reference<1,with_custodian_and_ward<1, 2> >())
             .def("halfExtents",&AABox::halfExtents,
                    return_internal_reference<1,with_custodian_and_ward<1, 2> >())
             .def("withinDistance",&AABox::withinDistance)
             .def("intersects",&AABox::intersects)
    ;
}

}
