#ifndef PYWRAP_SIREMOL_WRAPID_HPP
#define PYWRAP_SIREMOL_WRAPID_HPP

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/id.h"
#include "SireQt/qdatastream.hpp"
#include "SirePy/str.hpp"

using namespace boost::python;
using namespace SireQt;
using namespace SirePy;

SIRE_BEGIN_HEADER

namespace SireMol
{

template<class T>
void wrap_ID(const char *wrapname)
{
    class_<T>( wrapname, init<>() )
        .def( init<quint32>() )
        .def( init<const T&>() )
        .def( self == self )
        .def( self != self )
        .def( self < self )
        .def( self <= self )
        .def( self > self )
        .def( self >= self )
        .def( self + int() )
        .def( self += int() )
        .def( self - int() )
        .def( self -= int() )

        .def( "toString", &T::toString )

        .def( "__str__", __str__<T> )
        .def( "__rrshift__", &__rrshift__QDataStream<T>,
                   return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rlshift__", &__rlshift__QDataStream<T>,
                   return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    ;
}

}

SIRE_END_HEADER

#endif
