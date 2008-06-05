#ifndef SIREMOL_WRAPINTERNALINFO_HPP
#define SIREMOL_WRAPINTERNALINFO_HPP

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"
#include "SireMol/groupedvector.hpp"

#include "SirePy/pycontainer.hpp"
#include "SirePy/str.hpp"

#include "SireQt/qdatastream.hpp"

SIRE_BEGIN_HEADER

using namespace SirePy;
using namespace SireQt;

using namespace boost::python;

namespace SireMol
{

/** Wrap up the GroupedVector class */
template<class IDX, class V>
void wrap_GroupedVector(const char *classname)
{
    typedef GroupedVector<IDX,V> Vec;
    typedef QVector<V> group_type;
    typedef typename IndexInfo<IDX>::groupid_type groupid_type;
    typedef IDX index_type;
    
    class_<Vec>(classname, init<>())
        .def( init<const Vec&>() )
        
        .def( self == self )
        .def( self != self )
        
        .def( "__getitem__", &__getitem__dict<Vec,groupid_type,QVector<V> >,
                            typename get_return_reference_policy< QVector<V> >::type() )
        .def( "__getitem__", &__getitem__dict<Vec,index_type,V>,
                            typename get_return_reference_policy<V>::type() )
        
        .def( "__setitem__", &__setitem__dict<Vec,groupid_type,QVector<V> > )
        .def( "__setitem__", &__setitem__dict<Vec,index_type,V> )
        
        .def( "__delitem__", &__delitem__dict<Vec,groupid_type> )
        .def( "__delitem__", &__delitem__dict<Vec,index_type> )
        
        .def( "__iter__", boost::python::iterator<Vec>() )
        .def( "__iter__", boost::python::iterator<const Vec>() )
        
        .def( "__len__", (int (Vec::*)() const)
                         &Vec::count )
        
        .def( "__contains__", &__contains__<Vec,groupid_type> )
        .def( "__contains__", &__contains__<Vec,index_type> )
        
        .def( "__rlshift__", &__rlshift__QDataStream<Vec>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<Vec>,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "__str__", &__str__dict<Vec> )
        
        .def( "append", &Vec::append )
        
        .def( "at", (const V& (Vec::*)(const index_type&) const)
                        &Vec::at,
                    return_value_policy<copy_const_reference>() )
        .def( "at", (const QVector<V>& (Vec::*)(const groupid_type&) const)
                        &Vec::at,
                    return_value_policy<copy_const_reference>() )
                  
        .def( "clear", &Vec::clear )
        
        .def( "constVector", &Vec::constVector,
                    return_value_policy<copy_const_reference>() )

        .def( "contains", (bool (Vec::*)(const groupid_type&) const)
                        &Vec::contains )
        .def( "contains", (bool (Vec::*)(const index_type&) const)
                        &Vec::contains )

        .def( "count", (int (Vec::*)() const)
                         &Vec::count )
        .def( "count", (int (Vec::*)(const groupid_type&) const)
                         &Vec::count )

        .def( "insert", (void (Vec::*)(const groupid_type&, const QVector<V>&))
                         &Vec::insert )
        .def( "insert", (void (Vec::*)(const index_type&, const V&))
                         &Vec::insert )
        
        .def( "isEmpty", (bool (Vec::*)() const)
                         &Vec::isEmpty )
        .def( "isEmpty", (bool (Vec::*)(const groupid_type&) const)
                         &Vec::isEmpty )
        
        .def( "remove", (void (Vec::*)(const index_type&))
                         &Vec::remove )
        .def( "remove", (void (Vec::*)(const groupid_type&))
                         &Vec::remove )
        
        .def( "reserve", (void (Vec::*)(int))
                         &Vec::reserve )
        .def( "reserve", (void (Vec::*)(const groupid_type&, int))
                         &Vec::reserve )
        
        .def( "resize", &Vec::resize )

        .def( "size", (int (Vec::*)() const)
                         &Vec::size )
        .def( "size", (int (Vec::*)(const groupid_type&) const )
                         &Vec::size )
        
        .def( "squeeze", (void (Vec::*)())
                         &Vec::squeeze )
        .def( "squeeze", (void (Vec::*)(const groupid_type&))
                         &Vec::squeeze )
                
        .def( "take", &Vec::take )
                
        .def( "value", (V (Vec::*)(const index_type&) const)
                        &Vec::value )
        .def( "value", (V (Vec::*)(const index_type&, const V&) const)
                        &Vec::value )
                        
        .def( "values", (QVector<V> (Vec::*)(const groupid_type&) const)
                        &Vec::values )
        .def( "values", (QVector<V> (Vec::*)(const groupid_type&, const QVector<V>&) const)
                        &Vec::values )
    
        .def( "vector", (const QVector<V>& (Vec::*)(const groupid_type&) const)
                        &Vec::vector,
                    return_value_policy<copy_const_reference>() )
        .def( "vector", (QVector<V>& (Vec::*)(const groupid_type&))
                        &Vec::vector,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    ;

}

}

SIRE_END_HEADER

#endif
