
/**
  * This file contains the boost::python wrapping of Space and PairMatrix
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireVol/space.h"
#include "SireVol/cartesian.h"
#include "SireVol/periodicbox.h"

#include "SireVol/coordgroup.h"

using namespace boost::python;

namespace SireVol
{

void export_Space()
{
    class_<SpaceBase, boost::noncopyable>("SpaceBase", no_init)

        .def( "what", (const char* (SpaceBase::*)() const)
                          &SpaceBase::what )

        .def( "calcDist", (double (SpaceBase::*)(const CoordGroup&,
                                             DistMatrix&) const)
                          &SpaceBase::calcDist )

        .def( "calcDist2", (double (SpaceBase::*)(const CoordGroup&,
                                              DistMatrix&) const)
                          &SpaceBase::calcDist2 )
        .def( "calcInvDist", (double (SpaceBase::*)(const CoordGroup&,
                                                DistMatrix&) const)
                          &SpaceBase::calcInvDist )

        .def( "calcInvDist2", (double (SpaceBase::*)(const CoordGroup&,
                                                 DistMatrix&) const)
                          &SpaceBase::calcInvDist2 )

        .def( "calcDist", (double (SpaceBase::*)(const CoordGroup&,
                                             const CoordGroup&,
                                             DistMatrix&) const)
                          &SpaceBase::calcDist )

        .def( "calcDist2", (double (SpaceBase::*)(const CoordGroup&,
                                              const CoordGroup&,
                                              DistMatrix&) const)
                          &SpaceBase::calcDist2 )

        .def( "calcInvDist", (double (SpaceBase::*)(const CoordGroup&,
                                                const CoordGroup&,
                                                DistMatrix&) const)
                          &SpaceBase::calcInvDist )

        .def( "calcInvDist2", (double (SpaceBase::*)(const CoordGroup&,
                                                 const CoordGroup&,
                                                 DistMatrix&) const)
                          &SpaceBase::calcInvDist2 )

        .def( "beyond", (bool (SpaceBase::*)(double,
                                         const CoordGroup&,
                                         const CoordGroup&) const)
                          &SpaceBase::beyond )
    ;


    class_<Space>( "Space", init<>() )

        .def( init<const Space&>() )
        .def( init<const SpaceBase&>() )

        .def( "what", (const char* (Space::*)() const)
                          &Space::what )

        .def( "calcDist", (double (Space::*)(const CoordGroup&,
                                             DistMatrix&) const)
                          &Space::calcDist )

        .def( "calcDist2", (double (Space::*)(const CoordGroup&,
                                              DistMatrix&) const)
                          &Space::calcDist2 )
        .def( "calcInvDist", (double (Space::*)(const CoordGroup&,
                                                DistMatrix&) const)
                          &Space::calcInvDist )

        .def( "calcInvDist2", (double (Space::*)(const CoordGroup&,
                                                 DistMatrix&) const)
                          &Space::calcInvDist2 )

        .def( "calcDist", (double (Space::*)(const CoordGroup&,
                                             const CoordGroup&,
                                             DistMatrix&) const)
                          &Space::calcDist )

        .def( "calcDist2", (double (Space::*)(const CoordGroup&,
                                              const CoordGroup&,
                                              DistMatrix&) const)
                          &Space::calcDist2 )

        .def( "calcInvDist", (double (Space::*)(const CoordGroup&,
                                                const CoordGroup&,
                                                DistMatrix&) const)
                          &Space::calcInvDist )

        .def( "calcInvDist2", (double (Space::*)(const CoordGroup&,
                                                 const CoordGroup&,
                                                 DistMatrix&) const)
                          &Space::calcInvDist2 )

        .def( "beyond", (bool (Space::*)(double,
                                         const CoordGroup&,
                                         const CoordGroup&) const)
                          &Space::beyond )
    ;


    class_<Cartesian, bases<SpaceBase> >("Cartesian", init<>())
        .def( init<const Cartesian&>() )
    ;

    class_<PeriodicBox, bases<SpaceBase> >("PeriodicBox", init<>())

        .def(init<const Vector&, const Vector&>())
        .def( init<const PeriodicBox&>() )

        .def( "setDimension", (void (PeriodicBox::*)(const Vector&,
                                                     const Vector&))
                          &PeriodicBox::setDimension )

        .def( "minCoords", (const Vector& (PeriodicBox::*)() const)
                          &PeriodicBox::minCoords,
                          return_value_policy<copy_const_reference>() )

        .def( "maxCoords", (const Vector& (PeriodicBox::*)() const)
                          &PeriodicBox::maxCoords,
                          return_value_policy<copy_const_reference>() )

        .def( "center", (Vector (PeriodicBox::*)() const)
                          &PeriodicBox::center )
    ;
}

}
