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

#include <Python.h>
#include <boost/python.hpp>
#include <QObject>

#include "SireError/exception.h"
#include "SireError/errors.h"

using namespace boost::python;

namespace SireError
{

void out_of_range( const SireError::invalid_index &ex )
{
    PyErr_SetString(PyExc_StopIteration,ex.toString().toUtf8());
}

void exception_translator( const SireError::exception &ex )
{
    PyErr_SetString(PyExc_UserWarning,ex.toString().toUtf8());
}

void SIREERROR_EXPORT export_exceptions()
{
    register_exception_translator<SireError::exception>(&exception_translator);
    register_exception_translator<SireError::invalid_index>(&out_of_range);
}

}