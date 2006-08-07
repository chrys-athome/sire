#ifndef PYWRAP_SIREMM_WRAP_ASSIGN_INTERNALS_HPP
#define PYWRAP_SIREMM_WRAP_ASSIGN_INTERNALS_HPP

#include <Python.h>
#include <boost/python.hpp>

#include "SireMM/assign_internals.hpp"

using namespace boost::python;

SIRE_BEGIN_HEADER

namespace SireMM
{

template<class Internals>
void wrap_assign_internals(const char *wrapname)
{
    typedef assign_internals<Internals> AI;
    
    class_<AI, bases<AssignBase>, boost::noncopyable>(wrapname, no_init)
    ;
    
}

}

SIRE_END_HEADER

#endif
