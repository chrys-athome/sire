/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef PYWRAP_SIREN_CONVERTOBJPTR_HPP
#define PYWRAP_SIREN_CONVERTOBJPTR_HPP

#include <boost/python.hpp>

#include "Siren/sirenglobal.h"
#include "Siren/objref.h"
#include "Siren/objptr.hpp"

namespace bp = boost::python;

SIREN_BEGIN_HEADER

template<class T>
struct to_py_objptr
{
    typedef typename T::value_type pointee_type;

    static PyObject* convert(const T &objptr)
    {
        if (objptr.isNull())
            return bp::detail::none();
        else
        {
            Siren::ObjRef objref = objptr->clone();
            return bp::detail::make_owning_holder::execute<Siren::Object>(
                             Siren::extractPointer(objref) );
        }
    }
};

template<class T>
void register_objptr()
{
    bp::to_python_converter< T, to_py_objptr<T> >();
    bp::implicitly_convertible< typename T::value_type, T >();
}

SIREN_END_HEADER

#endif
