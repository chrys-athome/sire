
/**
  * This file contains the boost::python wrapping of AtomType
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireDB/atomtype.h"

#include "SirePy/str.hpp"
#include "SireQt/qdatastream.hpp"

using namespace boost::python;
using namespace SireQt;
using namespace SirePy;

namespace SireDB
{

void export_AtomType()
{
    
    class_<AtomType>("AtomType", init<>())
        .def( init<const QString&>() )
        .def( init<const QString&, const Element&>() )
        .def( init<const Element&>() )
        .def( init<const AtomType&>() )
        
        .def( self == self )
        .def( self != self )
        
        .def( "__str__", &__str__<AtomType> )
        
        .def( "__rlshift__", &__rlshift__QDataStream<AtomType>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<AtomType>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "ID", &AtomType::ID )
        .def( "element", &AtomType::element, return_value_policy<copy_const_reference>() )
        
        .def( "isDummy", &AtomType::isDummy )
        .def( "isWild", (bool (AtomType::*)() const) &AtomType::isWild )
        .def( "isWild", (bool (AtomType::*)(const Element&) const) &AtomType::isWild )
    
        .def( "isWildType", &AtomType::isWildType )
        .def( "isWildElement", &AtomType::isWildElement )
    
        .def( "toString", &AtomType::toString )
    
        .def( "dummy", &AtomType::dummy ).staticmethod("dummy")
        .def( "wild", (AtomType (*)()) &AtomType::wild )
        .def( "wild", (AtomType (*)(const Element&)) &AtomType::wild )
                                      .staticmethod("wild")
    ;
}

}
