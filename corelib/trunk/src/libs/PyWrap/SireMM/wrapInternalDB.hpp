#ifndef PYWRAP_SIREMM_WRAP_INTERNALDB_HPP
#define PYWRAP_SIREMM_WRAP_INTERNALDB_HPP

#include <Python.h>
#include <boost/python.hpp>

#include "SireMM/internaldb.hpp"

using namespace boost::python;

SIRE_BEGIN_HEADER

namespace SireMM
{

template<class Internals>
void wrap_InternalDB(const char *wrapname)
{
    typedef InternalDB<Internals> IDB;

    class_<IDB, boost::noncopyable>(wrapname, no_init)
    
        .def( "hasTable", &IDB::hasTable )
        
        //.def( "getTable", &IDB::getTable, //how do we return this reference?
        
        //.def( "createTable", &IDB::createTable //how do we return this reference?
        
        .def( "assignParameter", &IDB::assignParameter )
    ;
}

}

SIRE_END_HEADER

#endif
