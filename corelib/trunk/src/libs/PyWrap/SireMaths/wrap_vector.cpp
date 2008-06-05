
/**
  * This file contains the boost::python wrapping of a Vector
  *
  * @author Christopher Woods
  *
  */
  
#include <Python.h>
#include <boost/python.hpp>
#include <QString>
#include "SirePy/indexer.hpp"

#include "SireMaths/vector.h"
#include "SireMaths/matrix.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/angle.h"

using namespace boost::python;

namespace SireMaths
{

void SIREMATHS_EXPORT export_Vector()
{
    /** Add static function pointers */
    Vector (*staticFromString)(const QString&) = &Vector::fromString;
    double (*staticDistance)(const Vector&, const Vector&) = &Vector::distance;
    double (*staticDistance2)(const Vector&, const Vector&) = &Vector::distance2;
    double (*staticDot)(const Vector&, const Vector&) = &Vector::dot;
    Vector (*staticCross)(const Vector&, const Vector&) = &Vector::cross;
    Angle (*staticAngle1)(const Vector&, const Vector&) = &Vector::angle;
    Angle (*staticAngle2)(const Vector&, const Vector&, const Vector&) = &Vector::angle;
    Angle (*staticDihedral)(const Vector&, const Vector&, const Vector&, const Vector&) 
                                                        = &Vector::dihedral;
    Vector (*staticGenerate)(double, const Vector&, const Angle&, const Vector&, const Angle&,
                             const Vector&) = &Vector::generate;
    
    
    class_<Vector>( "Vector", init<double,double,double>() )
              .def(           init<double>() )
              .def(           init<>() )
              .def(           init<const Vector&>() )
              .def( "__len__", &Vector::count)
              .def( "__getitem__", &readonly_indexer<Vector,double>::__getitem__ )
              .def( "__str__", &Vector::toString )
              .def( "count", &Vector::count )
              .def( "size", &Vector::count )
              .def( "at", &Vector::at, return_value_policy<copy_const_reference>() )
              .def( "toString",&Vector::toString )
              .def( "fromString",staticFromString ).staticmethod("fromString")
              .def( "distance",staticDistance ).staticmethod("distance")
              .def( "distance2",staticDistance2 ).staticmethod("distance2")
              .def( "x", &Vector::x )
              .def( "y", &Vector::y )
              .def( "z", &Vector::z )
              .def( "r", &Vector::r )
              .def( "g", &Vector::g )
              .def( "b", &Vector::b )
              .def( "length", &Vector::length )
              .def( "manhattanLength", &Vector::manhattanLength )
              .def( "length2", &Vector::length2 )
              .def( "invLength", &Vector::invLength )
              .def( "invLength2", &Vector::invLength2 )
              .def( "normalise", &Vector::normalise )
              .def( "dot", staticDot ).staticmethod("dot")
              .def( "cross", staticCross ).staticmethod("cross")
              .def( self + self )
              .def( self - self )
              .def( self == self )
              .def( self != self )
              .def( self * double() )
              .def( double() * self )
              .def( self / double() )
              .def( self * Matrix() )
              .def( Matrix() * self )
              .def( self * Quaternion() )
              .def( Quaternion() * self )
              .def( "setMax", &Vector::setMax )
              .def( "setMin", &Vector::setMin )
              .def( "max", &Vector::max )
              .def( "min", &Vector::min )
              .def( "bearing", &Vector::bearing )
              .def( "bearingXY", &Vector::bearingXY )
              .def( "bearingXZ", &Vector::bearingXZ )
              .def( "bearingYZ", &Vector::bearingYZ )
              .def( "angle", staticAngle1 )
              .def( "angle", staticAngle2 ).staticmethod("angle")
              .def( "dihedral", staticDihedral ).staticmethod("dihedral")
              .def( "generate", staticGenerate ).staticmethod("generate")
   ;
}

}
