/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "python2qt.h"

using namespace boost::python;
using namespace SirePy;

Python2Qt::Python2Qt()
{}

Python2Qt::~Python2Qt()
{}

QString Python2Qt::str(PyObject *obj)
{
    if (!obj)
      return QString::null;

    //perform the 'str' function on the object (note that this is 
    //this is a new reference, so we will need to decrement the 
    //count once we are finished with it)
    PyObject *strng = PyObject_Str(obj);
    
    //extract a c style string from the resulting python
    //string object
    const char *cstr = extract<const char*>(strng);

    //create a QString copy of the string
    QString s(cstr);

    //decrement the reference count of the python string
    Py_DECREF(strng);
    
    //return the QString
    return s;
}

QString Python2Qt::str(object &obj)
{
    return str(obj.ptr());
}

object Python2Qt::toBoostObject(PyObject *obj)
{
    if (!obj)
      return object();
    
    handle<PyObject> hdle = handle<PyObject>(obj);
    return object(hdle);
}

bool Python2Qt::isNone(object &obj)
{
    //compare the pointer to the PyObject to the pointer for the None (empty) object()
    return obj.ptr() == object().ptr();
}

bool Python2Qt::isNone(PyObject *obj)
{
    return obj == object().ptr();
}

object Python2Qt::getMember(object &obj, QString name)
{
    return obj.attr(name.toAscii().constData());
}
