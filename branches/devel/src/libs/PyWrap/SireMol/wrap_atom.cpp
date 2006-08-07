
/**
  * This file contains the boost::python wrapping of an Atom. Atoms
  * in Python are immutable, and are copied by value.
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SirePy/converttuple.hpp"
#include "SirePy/str.hpp"
#include "SireQt/qdatastream.hpp"

#include "SireMol/atom.h"
#include "SireMol/elementdb.h"

using namespace boost::python;
using namespace SirePy;
using namespace SireQt;

using SireMaths::Vector;

namespace SireMol
{

void export_Atom()
{
    /** Wrap up the Element class */
    class_<Element>("Element",init<>())
        .def( init<QString>() )
        .def( init<const char*>() )
        .def( init<unsigned int>() )
        .def( init<int>() )
        .def( init<const Element&>() )
        
        .def( self == self )
        .def( self != self )
        .def( self < self )
        .def( self <= self )
        .def( self > self )
        .def( self >= self )
        
        .def( "__str__", &__str__<Element> )
        .def( "__rlshift__", &__rlshift__QDataStream<Element>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<Element>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "actinide", &Element::actinide )
        .def( "alkaliEarthMetal", &Element::alkaliEarthMetal )
        .def( "alkaliMetal", &Element::alkaliMetal )
        .def( "biological", &Element::biological )
        
        .def( "biologicalElement", &Element::biologicalElement )
                                        .staticmethod("biologicalElement")
        
        .def( "blue", &Element::blue )
        .def( "bondOrderRadius", &Element::bondOrderRadius )
        .def( "covalentRadius", &Element::covalentRadius )
        .def( "electroNegativity", &Element::electroNegativity )
        
        .def( "elementWithMass", &Element::elementWithMass )
                                        .staticmethod("elementWithMass")
        
        .def( "green", &Element::green )
        .def( "group", &Element::group )
        .def( "halogen", &Element::halogen )
        .def( "lanthanide", &Element::lanthanide )
        .def( "mass", &Element::mass )
        .def( "maxBonds", &Element::maxBonds )
        .def( "name", &Element::name )
        .def( "nobleGas", &Element::nobleGas )
        .def( "nProtons", &Element::nProtons )
        .def( "period", &Element::period )
        .def( "rareEarth", &Element::rareEarth )
        .def( "red", &Element::red )
        .def( "symbol", &Element::symbol )
        .def( "toString", &Element::toString )
        .def( "transitionMetal", &Element::transitionMetal )
        .def( "vdwRadius", &Element::vdwRadius )
    ;    

    implicitly_convertible<QString,Element>();
    implicitly_convertible<unsigned int,Element>();
        
    /** Wrap up the AtomIndex class... */
    class_<AtomIndex>( "AtomIndex", init< optional<QString,ResNum> >() )
        .def( init<const boost::tuple<QString,ResNum>&>() )
        .def( init<const AtomIndex&>() )
        .def( self == self )
        .def( self != self )
        .def( self <= self )
        .def( self < self )
        .def( self >= self )
        .def( self > self )
                 
        .def( "__str__", &__str__<AtomIndex> )
        .def( "__rlshift__", &__rlshift__QDataStream<AtomIndex>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<AtomIndex>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
                 
        .def( "name", &AtomIndex::name )
        .def( "resNum", &AtomIndex::resNum )
        .def( "toString", &AtomIndex::toString )
    ;
    
    /** Allow implicit conversion from python tuple of string and int to 
        a boost::tuple<QString,ResNum>, so that it can be used to construct
        AtomIndex objects */
    convert_python_tuple_to_boost_tuple2<QString,ResNum>();
    
    /** Allow implicit conversion from boost::tuple<QString,ResNum> to AtomIndex */
    implicitly_convertible< boost::tuple<QString,ResNum>, AtomIndex >();
    
    /** Wrap up the Atom class... */
    class_<Atom, bases<Vector,Element,AtomIndex> >( "Atom",init<>() )
        .def( init<const Atom&>() )
        .def( init<const AtomIndex&, const Element&, const Vector&>() )
        .def( init<const Atom&, ResNum>() )
        .def( init<AtomNum, const AtomIndex&, const Element&, const Vector&>() )
        .def( init<AtomNum, const Atom&>() )
        .def( init<AtomNum, const Atom&, ResNum>() )
        
        .def( init<const QString&>() )
        .def( init<const QString&, const QString&>() )
        
        .def( init<const QString&, ResNum>() )
        .def( init<const QString&, ResNum, const QString&>() )
        
        .def( init<const AtomIndex&>() )
        
        .def( init<const QString&, const Vector&>() )
        .def( init<const QString&, const QString&, const Vector&>() )
        .def( init<const QString&, const Atom&>() )
        
        .def( init<AtomNum, const QString&>() )
        .def( init<AtomNum, const QString&, const QString&>() )
        
        .def( init<AtomNum, const QString&, const Vector&>() )
        .def( init<AtomNum, const QString&, const QString&, const Vector&>() )
    
        .def( self == self )
        .def( self != self )
        .def( self == other<AtomIndex>() )
        .def( self != other<AtomIndex>() )
        
        .def( "__str__", &__str__<Atom> )
        .def( "__rlshift__", &__rlshift__QDataStream<Atom>,
                return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<Atom>,
                return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "atomNum", &Atom::atomNum )
        
        .def( "index", (const AtomIndex& (Atom::*)() const) &Atom::index,
                return_value_policy<copy_const_reference>() )
        .def( "index", (AtomIndex& (Atom::*)()) &Atom::index,
                return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
                
        .def( "element", (const Element& (Atom::*)() const) &Atom::element,
                return_value_policy<copy_const_reference>() )
        .def( "element", (Element& (Atom::*)()) &Atom::element,
                return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
                
        .def( "name", &Atom::name )
        .def( "number", &Atom::number )
        
        .def( "toString", &Atom::toString )
                
        .def( "vector", (const Vector& (Atom::*)() const) &Atom::vector,
                return_value_policy<copy_const_reference>() )
        .def( "vector", (Vector& (Atom::*)()) &Atom::vector,
                return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    
    
        .def( "withinBondRadii", &Atom::withinBondRadii )
                                          .staticmethod("withinBondRadii")
    ;
}

}
