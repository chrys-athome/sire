
/**
  * This file contains the boost::python wrapping of Bond, Angle and Dihedral, and
  * their lists and sets.
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

#include "SireMol/bond.h"
#include "SireMol/angle.h"
#include "SireMol/dihedral.h"
#include "SireMol/improper.h"

#include "SirePy/converttuple.hpp"
#include "SirePy/pycontainer.hpp"
#include "SirePy/str.hpp"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;
using namespace SirePy;
using namespace SireQt;

namespace SireMol
{

void export_Internals()
{
    /** Wrap up the Bond class */
    class_<Bond>("Bond", init<>())
        .def( init<const AtomIndex&, const AtomIndex&>() )
        .def( init<const tuple<AtomIndex,AtomIndex>&>() )
        .def( init<const QString&, const QString&, ResNum>() )
        .def( init<const QString&, ResNum, const QString&, ResNum>() )
        .def( init<const Bond&>() )

        .def( self == self )
        .def( self != self )
        .def( self > self )
        .def( self >= self )
        .def( self < self )
        .def( self <= self )

        .def( "__contains__", &__contains__<Bond,AtomIndex> )
        .def( "__contains__", &__contains__<Bond,QString> )
        .def( "__contains__", &__contains__<Bond,ResNum> )

        .def( "__getitem__", &__getitem__list_const<Bond,AtomIndex> )
        .def( "__getitem__", &__getitem__dict_const<Bond,ResNum,AtomIndex> )

        .def( "__len__", &__len__<Bond> )

        .def( "__str__", &__str__<Bond> )

        .def( "__rlshift__", &__rlshift__QDataStream<Bond>,
                      return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<Bond>,
                      return_internal_reference< 1, with_custodian_and_ward<1,2> >() )

        .def( "at", (const AtomIndex& (Bond::*)(int) const)&Bond::at,
                                        return_value_policy<copy_const_reference>() )
        .def( "at", (const AtomIndex& (Bond::*)(ResNum) const)&Bond::at,
                                        return_value_policy<copy_const_reference>() )

        .def( "atom", &Bond::atom, return_value_policy<copy_const_reference>() )

        .def( "atom0", &Bond::atom0, return_value_policy<copy_const_reference>() )
        .def( "atom1", &Bond::atom1, return_value_policy<copy_const_reference>() )

        .def( "contains", (bool (Bond::*)(const AtomIndex&) const)
                      &Bond::contains )
        .def( "contains", (bool (Bond::*)(const QString&) const)
                      &Bond::contains )
        .def( "contains", (bool (Bond::*)(ResNum) const)
                      &Bond::contains )

        .def( "count", &Bond::count )

        .def( "interResidue", &Bond::interResidue )
        .def( "intraResidue", &Bond::intraResidue )

        .def( "other", (const AtomIndex& (Bond::*)(const AtomIndex&) const)
                      &Bond::other, return_value_policy<copy_const_reference>() )
        .def( "other", (const AtomIndex& (Bond::*)(ResNum resnum) const)
                      &Bond::other, return_value_policy<copy_const_reference>() )

        .def( "otherRes", &Bond::otherRes )

        .def( "resNum0", &Bond::resNum0 )
        .def( "resNum1", &Bond::resNum1 )

        .def( "size", &Bond::size )

        .def( "toString", &Bond::toString )
    ;

    /** Allow python tuple of two AtomIndex objects to be implicitly converted
        to a boost::tuple<AtomIndex,AtomIndex> */
    SirePy::convert_python_tuple_to_boost_tuple2<AtomIndex,AtomIndex>();

    /** Allow a boost::tuple<AtomIndex,AtomIndex> to be implicitly converted
        to a Bond */
    implicitly_convertible< boost::tuple<AtomIndex,AtomIndex>, Bond >();

    /** Wrap up the angle class */
    class_<Angle>("Angle", init<>())
        .def( init<const AtomIndex&, const AtomIndex&, const AtomIndex&>() )
        .def( init<const tuple<AtomIndex,AtomIndex,AtomIndex>&>() )
        .def( init<const QString&, const QString&, const QString&, ResNum>() )
        .def( init<const QString&, ResNum, const QString&, ResNum,
                   const QString&, ResNum>() )
        .def( init<const Angle&>() )

        .def( self == self )
        .def( self != self )
        .def( self < self )
        .def( self <= self )
        .def( self > self )
        .def( self >= self )

        .def( "__contains__", &__contains__<Angle,AtomIndex> )
        .def( "__contains__", &__contains__<Angle,QString> )
        .def( "__contains__", &__contains__<Angle,ResNum> )

        .def( "__getitem__", &__getitem__list_const<Angle,AtomIndex> )

        .def( "__len__", &__len__<Angle> )

        .def( "__str__", &__str__<Angle> )

        .def( "__rlshift__", &__rlshift__QDataStream<Angle>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def ("__rrshift__", &__rrshift__QDataStream<Angle>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )

        .def( "at", &Angle::at, return_value_policy<copy_const_reference>() )

        .def( "atom0", &Angle::atom0, return_value_policy<copy_const_reference>() )
        .def( "atom1", &Angle::atom1, return_value_policy<copy_const_reference>() )
        .def( "atom2", &Angle::atom2, return_value_policy<copy_const_reference>() )

        .def( "contains", (bool (Angle::*)(const AtomIndex&) const)
                    &Angle::contains )
        .def( "contains", (bool (Angle::*)(const QString&) const)
                    &Angle::contains )
        .def( "contains", (bool (Angle::*)(ResNum) const)
                    &Angle::contains )

        .def( "count", &Angle::count )

        .def( "intraResidue", &Angle::intraResidue )
        .def( "interResidue", &Angle::interResidue )

        .def( "resNum0", &Angle::resNum0 )
        .def( "resNum1", &Angle::resNum1 )
        .def( "resNum2", &Angle::resNum2 )

        .def( "size", &Angle::size )

        .def( "toString", &Angle::toString )
    ;

    /** Allow a python tuple of three AtomIndex objects to be implicitly converted
        to a boost::tuple<AtomIndex,AtomIndex,AtomIndex> */
    SirePy::convert_python_tuple_to_boost_tuple3<AtomIndex,AtomIndex,AtomIndex>();

    /** Allow a boost::tuple<AtomIndex,AtomIndex,AtomIndex> to be implicitly converted
        to an Angle */
    implicitly_convertible<boost::tuple<AtomIndex,AtomIndex,AtomIndex>,Angle>();


    /** Wrap up the dihedral class */
    class_<Dihedral>("Dihedral", init<>())
        .def( init<const AtomIndex&, const AtomIndex&,
                   const AtomIndex&, const AtomIndex&>() )
        .def( init<const tuple<AtomIndex,AtomIndex,AtomIndex,AtomIndex>&>() )
        .def( init<const QString&, const QString&,
                   const QString&, const QString&, ResNum>() )
        .def( init<const QString&, ResNum, const QString&, ResNum,
                   const QString&, ResNum, const QString&, ResNum>() )
        .def( init<const Dihedral&>() )

        .def( self == self )
        .def( self != self )
        .def( self < self )
        .def( self <= self )
        .def( self > self )
        .def( self >= self )

        .def( "__contains__", &__contains__<Dihedral,AtomIndex> )
        .def( "__contains__", &__contains__<Dihedral,QString> )
        .def( "__contains__", &__contains__<Dihedral,ResNum> )

        .def( "__getitem__", &__getitem__list_const<Dihedral,AtomIndex> )

        .def( "__len__", &__len__<Dihedral> )

        .def( "__str__", &__str__<Dihedral> )

        .def( "__rlshift__", &__rlshift__QDataStream<Dihedral>,
                return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<Dihedral>,
                return_internal_reference< 1, with_custodian_and_ward<1,2> >() )

        .def( "atom0", &Dihedral::atom0, return_value_policy<copy_const_reference>() )
        .def( "atom1", &Dihedral::atom1, return_value_policy<copy_const_reference>() )
        .def( "atom2", &Dihedral::atom2, return_value_policy<copy_const_reference>() )
        .def( "atom3", &Dihedral::atom3, return_value_policy<copy_const_reference>() )

        .def( "at", &Dihedral::at, return_value_policy<copy_const_reference>() )

        .def( "size", &Dihedral::size )
        .def( "count", &Dihedral::count )

        .def( "contains", (bool (Dihedral::*)(const AtomIndex&) const)
                    &Dihedral::contains )
        .def( "contains", (bool (Dihedral::*)(const QString&) const)
                    &Dihedral::contains )
        .def( "contains", (bool (Dihedral::*)(ResNum) const)
                    &Dihedral::contains )

        .def( "intraResidue", &Dihedral::intraResidue )
        .def( "interResidue", &Dihedral::interResidue )

        .def( "resNum0", &Dihedral::resNum0 )
        .def( "resNum1", &Dihedral::resNum1 )
        .def( "resNum2", &Dihedral::resNum2 )
        .def( "resNum3", &Dihedral::resNum3 )

        .def( "toString", &Dihedral::toString )
    ;


    /** Allow a python tuple of four AtomIndex objects to be implicitly converted
        to a boost::tuple<AtomIndex,AtomIndex,AtomIndex,AtomIndex> */
    SirePy::convert_python_tuple_to_boost_tuple4<AtomIndex,AtomIndex,AtomIndex,AtomIndex>();

    /** Allow a boost::tuple<AtomIndex,AtomIndex,AtomIndex,AtomIndex> to be implicitly converted
        to a Dihedral */
    implicitly_convertible<boost::tuple<AtomIndex,AtomIndex,AtomIndex,AtomIndex>, Dihedral>();


    /** Wrap up the improper angle class */
    class_<Improper>("Improper", init<>())
        .def( init<const AtomIndex&, const AtomIndex&,
                   const AtomIndex&, const AtomIndex&>() )
        .def( init<const tuple<AtomIndex,AtomIndex,AtomIndex,AtomIndex>&>() )
        .def( init<const QString&, const QString&,
                   const QString&, const QString&, ResNum>() )
        .def( init<const QString&, ResNum, const QString&, ResNum,
                   const QString&, ResNum, const QString&, ResNum>() )
        .def( init<const Improper&>() )

        .def( self == self )
        .def( self != self )
        .def( self < self )
        .def( self <= self )
        .def( self > self )
        .def( self >= self )

        .def( "__contains__", &__contains__<Improper,AtomIndex> )
        .def( "__contains__", &__contains__<Improper,QString> )
        .def( "__contains__", &__contains__<Improper,ResNum> )

        .def( "__getitem__", &__getitem__list_const<Improper,AtomIndex> )

        .def( "__len__", &__len__<Improper> )

        .def( "__str__", &__str__<Improper> )

        .def( "__rlshift__", &__rlshift__QDataStream<Improper>,
                return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<Improper>,
                return_internal_reference< 1, with_custodian_and_ward<1,2> >() )

        .def( "atom0", &Improper::atom0, return_value_policy<copy_const_reference>() )
        .def( "atom1", &Improper::atom1, return_value_policy<copy_const_reference>() )
        .def( "atom2", &Improper::atom2, return_value_policy<copy_const_reference>() )
        .def( "atom3", &Improper::atom3, return_value_policy<copy_const_reference>() )

        .def( "at", &Improper::at, return_value_policy<copy_const_reference>() )

        .def( "size", &Improper::size )
        .def( "count", &Improper::count )

        .def( "contains", (bool (Improper::*)(const AtomIndex&) const)
                    &Improper::contains )
        .def( "contains", (bool (Improper::*)(const QString&) const)
                    &Improper::contains )
        .def( "contains", (bool (Improper::*)(ResNum) const)
                    &Improper::contains )

        .def( "intraResidue", &Improper::intraResidue )
        .def( "interResidue", &Improper::interResidue )

        .def( "resNum0", &Improper::resNum0 )
        .def( "resNum1", &Improper::resNum1 )
        .def( "resNum2", &Improper::resNum2 )
        .def( "resNum3", &Improper::resNum3 )

        .def( "toString", &Improper::toString )
    ;

    /** Allow a boost::tuple<AtomIndex,AtomIndex,AtomIndex,AtomIndex> to be implicitly converted
        to an Improper */
    implicitly_convertible<boost::tuple<AtomIndex,AtomIndex,AtomIndex,AtomIndex>,Improper>();

}

}
