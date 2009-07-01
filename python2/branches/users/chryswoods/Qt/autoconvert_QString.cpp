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
#include <QString>
#include <QRegExp>
#include <wchar.h>

#include "Helpers/str.hpp"

using namespace boost::python;

/** This function converts a QString to a python str object

    This function is copied from python-qt, placed under the GPL
    Copyright (C) 2005 by Eric Jardim.

*/
PyObject* convert(QString const& s)
{
    //FIXME: Done with Python/C API, needs Boost::Python-ifying
    //FIXME: UCS-4 case can probably be done a lot more effciently.
#if defined(Py_UNICODE_WIDE)
    //qDebug("Py_UNICODE_WIDE");
    int unichars = s.size();
    Py_UNICODE* buf = new Py_UNICODE[unichars];
    for (int i = 0; i < unichars; i++)
        buf[i] = s.at(i).unicode();

    PyObject* tempObj = PyUnicode_FromUnicode(buf, unichars);
    delete[] buf;
    return tempObj;
#else
    return PyUnicode_FromUnicode(s.utf16(), s.size());
#endif
}

/** This function is copied from python-qt, placed under the GPL,
    Copyright (C) 2005 by Eric Jardim. It converts a python
    string or unicode to a QString */
void QString_from_python_str_or_unicode( PyObject* obj_ptr,
                                         converter::rvalue_from_python_stage1_data* data )
{
    // First, convert the input to Python `unicode'.
    PyObject* temp_obj_ptr;
    if (PyString_Check(obj_ptr))
    {
        // Coerce the `str' to `unicode' using sysdefaultencoding. UnicodeDecodeError
        // is thrown if the string doesn't make sense in that encoding.
        temp_obj_ptr = PyUnicode_FromObject(obj_ptr); // new reference
        if (temp_obj_ptr == 0)
        {
            boost::python::throw_error_already_set();
        }
    }
    else
    {
        temp_obj_ptr = obj_ptr;
        Py_INCREF(temp_obj_ptr); // to balance DECREF at end
    }

    // FIXME: This implementation is probably rather inefficient
    Py_UNICODE* value = PyUnicode_AsUnicode(temp_obj_ptr);
    if (value == 0)
    {
        boost::python::throw_error_already_set();
    }
    int unichars = PyUnicode_GET_SIZE(temp_obj_ptr);
#if defined(Py_UNICODE_WIDE)
    //qDebug("Py_UNICODE_WIDE");

    // Python is using a 4-byte unsigned buffer of UCS-4
    // FIXME: Qt doesn't give us any direct way to load UCS-4, so we're doing
    //        it a rather clunky way that can probably be improved.
    // FIXME: Qt can't represent UCS-4 characters; we need to check for this
    //        and throw an exception.
    QString tempString("");
    int i;
    for (i = 0; i < unichars; i++)
        tempString.append(QChar(int(value[i])));
#else
    // Python is using a 2-byte unsigned buffer of UCS-2 with
    // limited support for UTF-16
    QString tempString(QString::fromUtf16(value, unichars));
#endif
    Py_DECREF(temp_obj_ptr);
    void* storage = ((converter::rvalue_from_python_storage<QString>*) data)->storage.bytes;
    new (storage) QString(tempString);
    data->convertible = storage;
}

/** The actual struct used to control the conversion */
struct QString_from_python
{
    QString_from_python()
    {
        converter::registry::push_back(  &convertible,
                                         &construct,
                                         type_id<QString>() );
    }

    /** Can the python object pointed to by 'obj_ptr' be converted
        to a QString?
    */
    static void* convertible(PyObject* obj_ptr)
    {
        if ( PyString_Check(obj_ptr) or
             PyUnicode_Check(obj_ptr) )
                return obj_ptr;
        else
            return 0;
    }

    /** Perform the actual conversion */
    static void construct(  PyObject* obj_ptr,
                            converter::rvalue_from_python_stage1_data* data)
    {
        //use python-qt conversion function
        QString_from_python_str_or_unicode(obj_ptr, data);
    }
};

/** This exports the QString wrapper. I use the python-qt conversion to convert
    from python to QString, then a full wrapping of QString to convert from
    QString to python. This allows me to have a fully-featured QString type
    that is visible from python.

    @author Christopher Woods
*/
void autoconvert_QString()
{
    //code to get a QString from a python object
    QString_from_python();
}
