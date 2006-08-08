
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
