/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

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
