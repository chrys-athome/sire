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

#ifndef SIREPY_PYTHON2QT_H
#define SIREPY_PYTHON2QT_H

/**
  * This file contains several functions that may be useful to convert Python
  * objects (either direct Python API or via boost::python API) into Qt C++
  * objects.
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <QString>
#include <boost/python.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SirePy
{

class Python2Qt
{
public:
    /** perform the 'str' function on a python object and return the result
    as a QString. Returns a null string if the object is null */
    static QString str(PyObject *obj);
    static QString str(boost::python::object &obj);
    
    /** Eventually provide the 'unicode' to QString functions */
    
    /** Return whether an object is None */
    static bool isNone(boost::python::object &obj);
    static bool isNone(PyObject *obj);

    /** Return the member variable (or function) called 'name' from object 'obj',
        or Python None if no such variable exists */
    static boost::python::object getMember(boost::python::object &obj, QString name);
        
    /** Return a boost::python::object from a PyObject pointer. 
        Note that this increments the reference count for 'obj'
        (and will decrement it when this object is deleted) */
    static boost::python::object toBoostObject(PyObject *obj);
    
private:
    Python2Qt();
    ~Python2Qt();
};

}

SIRE_END_HEADER

#endif
