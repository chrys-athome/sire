#ifndef PYWRAP_SIREMM_WRAP_INTERNALGENERATOR_HPP
#define PYWRAP_SIREMM_WRAP_INTERNALGENERATOR_HPP

#include <Python.h>
#include <boost/python.hpp>

#include "SireMM/internalgenerator.hpp"
#include "SireMol/molecule.h"

using namespace boost::python;

SIRE_BEGIN_HEADER

namespace SireMM
{

using SireMol::Molecule;

template<class Internals>
void wrap_InternalGenerator(const char *wrapname)
{
    typedef InternalGenerator<Internals> IG;
    
    class_<IG, boost::noncopyable>(wrapname, no_init)
    
        .def( "clone", &IG::clone, return_value_policy<manage_new_object>() )
    
        .def( "generate", (void (IG::*)(const Molecule&, Internals&) const)
                          &IG::generate )
        .def( "generate", (Internals (IG::*)(const Molecule&) const)
                          &IG::generate )

        .def( "what", &IG::what )
    ;
}

template<class Internals>
void wrap_UsePassedInternals(const char *wrapname)
{
    typedef InternalGenerator<Internals> IG;
    typedef UsePassedInternals<Internals> UP;
    
    class_<UP, bases<IG>, boost::noncopyable>(wrapname, no_init)
    ;
}

}

SIRE_END_HEADER

#endif
