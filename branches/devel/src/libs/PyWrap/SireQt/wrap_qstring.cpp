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

#include "SirePy/str.hpp"

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
        tempString.append(QChar(value[i]));
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

template<class T>
QString wrap_arg_str_3(const QString &string, const T &a, int fieldWidth, 
                     const QChar &fillChar)
{
    return string.arg( a, fieldWidth, fillChar );
}

template<class T>
QString wrap_arg_str_2(const QString &string, const T &a, int fieldWidth)
{
    return string.arg( a, fieldWidth );
}

template<class T>
QString wrap_arg_str_1(const QString &string, const T &a)
{
    return string.arg( a );
}

template<typename T>
QString wrap_arg_int_4(const QString &string, T val, int fieldWidth, int base,
                       const QChar &fillChar)
{
    return string.arg(val, fieldWidth, base, fillChar);
}

template<typename T>
QString wrap_arg_int_3(const QString &string, T val, int fieldWidth, int base)
{
    return string.arg(val, fieldWidth, base);
}

template<typename T>
QString wrap_arg_int_2(const QString &string, T val, int fieldWidth)
{
    return string.arg(val, fieldWidth);
}

template<typename T>
QString wrap_arg_int_1(const QString &string, T val)
{
    return string.arg(val);
}

template<typename T>
QString wrap_arg_double_5(const QString &string, T val, int fieldWidth, char fmt,
                          int prec, const QChar &fillChar)
{
    return string.arg(val,fieldWidth,fmt,prec,fillChar);
}

template<typename T>
QString wrap_arg_double_4(const QString &string, T val, int fieldWidth, char fmt,
                          int prec)
{
    return string.arg(val,fieldWidth,fmt,prec);
}

template<typename T>
QString wrap_arg_double_3(const QString &string, T val, int fieldWidth, char fmt)
{
    return string.arg(val,fieldWidth,fmt);
}

template<typename T>
QString wrap_arg_double_2(const QString &string, T val, int fieldWidth)
{
    return string.arg(val,fieldWidth);
}

template<typename T>
QString wrap_arg_double_1(const QString &string, T val)
{
    return string.arg(val);
}

template<class T>
bool wrap_contains2(const QString &string, T str, Qt::CaseSensitivity cs)
{
    return string.contains(str, cs);
}

template<class T>
bool wrap_contains1(const QString &string, T str)
{
    return string.contains(str);
}

template<class T>
int wrap_count2(const QString &string, T str, Qt::CaseSensitivity cs)
{
    return string.count(str,cs);
}

template<class T>
int wrap_count1(const QString &string, T str)
{
    return string.count(str);
}

template<class T>
bool wrap_endsWith2(const QString &string, const T &s, Qt::CaseSensitivity cs)
{
    return string.endsWith(s,cs);
}

template<class T>
bool wrap_endsWith1(const QString &string, const T &s)
{
    return string.endsWith(s);
}

QString& wrap_fill2(QString &string, QChar ch, int size)
{
    return string.fill(ch,size);
}

QString& wrap_fill1(QString &string, QChar ch)
{
    return string.fill(ch);
}

template<class T>
int wrap_indexOf3(const QString &string, T str, int from, Qt::CaseSensitivity cs)
{
    return string.indexOf(str, from, cs);
}

template<class T>
int wrap_indexOf2(const QString &string, T str, int from)
{
    return string.indexOf(str, from);
}

template<class T>
int wrap_indexOf1(const QString &string, T str)
{
    return string.indexOf(str);
}

template<class T>
int wrap_lastIndexOf3(const QString &string, T str, int from, Qt::CaseSensitivity cs)
{
    return string.lastIndexOf(str,from,cs);
}

template<class T>
int wrap_lastIndexOf2(const QString &string, T str, int from)
{
    return string.lastIndexOf(str,from);
}

template<class T>
int wrap_lastIndexOf1(const QString &string, T str)
{
    return string.lastIndexOf(str);
}

void export_QString2(class_<QString> &wrapper);
void export_QString3(class_<QString> &wrapper);

/** This exports the QString wrapper. I use the python-qt conversion to convert
    from python to QString, then a full wrapping of QString to convert from 
    QString to python. This allows me to have a fully-featured QString type
    that is visible from python.
    
    @author Christopher Woods
*/
void  
SIREQT_EXPORT
export_QString()
{
    //code to get a QString from a python object
    QString_from_python();

    //wrapper for QString::Null
    class_<QString::Null>( "QString_Null", init<>() )
    ;

    //wrapper for the C++ QString class
    class_<QString> qstring_wrapper = class_<QString>( "QString", init<>() )
        .def( init<const QChar*, int>() )
        .def( init<QChar>() )
        .def( init<int, QChar>() )
        .def( init<const char*>() )
        .def( init<const QByteArray&>() )
        .def( init<const QString&>() )
        
        .def( "__str__", &convert )
        
        .def( "append", (QString& (QString::*)(const QString&))
                                        &QString::append,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "append", (QString& (QString::*)(const QLatin1String&))
                                        &QString::append,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "append", (QString& (QString::*)(const QByteArray&))
                                        &QString::append,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "append", (QString& (QString::*)(const char*))
                                        &QString::append,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "append", (QString& (QString::*)(QChar))
                                        &QString::append,
                            return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
                                        
        .def( "arg", (QString (QString::*)(const QString&, const QString&) const)
                                        &QString::arg )
        .def( "arg", (QString (QString::*)(const QString&, const QString&, 
                                           const QString&) const)
                                        &QString::arg )
        .def( "arg", (QString (QString::*)(const QString&, const QString&, 
                                           const QString&, const QString&) const)
                                        &QString::arg )
        
        .def( "arg", &wrap_arg_int_1<int> )
        .def( "arg", &wrap_arg_int_2<int> )
        .def( "arg", &wrap_arg_int_3<int> )
        .def( "arg", &wrap_arg_int_4<int> )
        
        .def( "arg", &wrap_arg_int_1<uint> )
        .def( "arg", &wrap_arg_int_2<uint> )
        .def( "arg", &wrap_arg_int_3<uint> )
        .def( "arg", &wrap_arg_int_4<uint> )
        
        .def( "arg", &wrap_arg_int_1<long> )
        .def( "arg", &wrap_arg_int_2<long> )
        .def( "arg", &wrap_arg_int_3<long> )
        .def( "arg", &wrap_arg_int_4<long> )
        
        .def( "arg", &wrap_arg_int_1<ulong> )
        .def( "arg", &wrap_arg_int_2<ulong> )
        .def( "arg", &wrap_arg_int_3<ulong> )
        .def( "arg", &wrap_arg_int_4<ulong> )
        
        .def( "arg", &wrap_arg_int_1<qlonglong> )
        .def( "arg", &wrap_arg_int_2<qlonglong> )
        .def( "arg", &wrap_arg_int_3<qlonglong> )
        .def( "arg", &wrap_arg_int_4<qlonglong> )
        
        .def( "arg", &wrap_arg_int_1<qulonglong> )
        .def( "arg", &wrap_arg_int_2<qulonglong> )
        .def( "arg", &wrap_arg_int_3<qulonglong> )
        .def( "arg", &wrap_arg_int_4<qulonglong> )
        
        .def( "arg", &wrap_arg_int_1<qulonglong> )
        .def( "arg", &wrap_arg_int_2<qulonglong> )
        .def( "arg", &wrap_arg_int_3<qulonglong> )
        .def( "arg", &wrap_arg_int_4<qulonglong> )
        
        .def( "arg", &wrap_arg_int_1<qulonglong> )
        .def( "arg", &wrap_arg_int_2<qulonglong> )
        .def( "arg", &wrap_arg_int_3<qulonglong> )
        .def( "arg", &wrap_arg_int_4<qulonglong> )
        
        .def( "arg", &wrap_arg_int_1<short> )
        .def( "arg", &wrap_arg_int_2<short> )
        .def( "arg", &wrap_arg_int_3<short> )
        .def( "arg", &wrap_arg_int_4<short> )
        
        .def( "arg", &wrap_arg_int_1<ushort> )
        .def( "arg", &wrap_arg_int_2<ushort> )
        .def( "arg", &wrap_arg_int_3<ushort> )
        .def( "arg", &wrap_arg_int_4<ushort> )
        
        .def( "arg", &wrap_arg_str_1<QString> )
        .def( "arg", &wrap_arg_str_2<QString> )
        .def( "arg", &wrap_arg_str_3<QString> )
        
        .def( "arg", &wrap_arg_str_1<QChar> )
        .def( "arg", &wrap_arg_str_2<QChar> )
        .def( "arg", &wrap_arg_str_3<QChar> )
        
        .def( "arg", &wrap_arg_str_1<char> )
        .def( "arg", &wrap_arg_str_2<char> )
        .def( "arg", &wrap_arg_str_3<char> )
        
        .def( "arg", &wrap_arg_double_1<double> )
        .def( "arg", &wrap_arg_double_2<double> )
        .def( "arg", &wrap_arg_double_3<double> )
        .def( "arg", &wrap_arg_double_4<double> )
        .def( "arg", &wrap_arg_double_5<double> )
        
        .def( "at", &QString::at )
        .def( "capacity", &QString::capacity )
        .def( "chop", &QString::chop )
        .def( "clear", &QString::clear )
        
        .def( "compare", (int (QString::*)(const QString&) const)
                                &QString::compare )
        .def( "compare", (int (*)(const QString&, const QString&))
                                &QString::compare ).staticmethod("compare")
        
        .def( "contains", &wrap_contains1<const QString&> )
        .def( "contains", &wrap_contains2<const QString&> )
        .def( "contains", &wrap_contains1<QChar> )
        .def( "contains", &wrap_contains2<QChar> )
        .def( "contains", (QBool (QString::*)(const QRegExp&) const)
                                &QString::contains )
        
        .def( "count", &wrap_count2<const QString&> )
        .def( "count", &wrap_count1<const QString&> )
        .def( "count", &wrap_count2<QChar> )
        .def( "count", &wrap_count1<QChar> )
        .def( "count", (int (QString::*)(const QRegExp&) const)
                                &QString::count )
        .def( "count", (int (QString::*)() const)
                                &QString::count )
        
        .def( "endsWith", &wrap_endsWith2<QString> )
        .def( "endsWith", &wrap_endsWith1<QString> )
        .def( "endsWith", &wrap_endsWith2<QLatin1String> )
        .def( "endsWith", &wrap_endsWith1<QLatin1String> )
        .def( "endsWith", &wrap_endsWith2<QChar> )
        .def( "endsWith", &wrap_endsWith1<QChar> )
        
        .def( "fill", &wrap_fill1, 
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "fill", &wrap_fill2, 
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
                    
        .def( "indexOf", &wrap_indexOf3<const QString&> )
        .def( "indexOf", &wrap_indexOf2<const QString&> )
        .def( "indexOf", &wrap_indexOf1<const QString&> )
        .def( "indexOf", &wrap_indexOf3<QChar> )
        .def( "indexOf", &wrap_indexOf2<QChar> )
        .def( "indexOf", &wrap_indexOf1<QChar> )
        .def( "indexOf", &wrap_indexOf2<const QRegExp&> )
        .def( "indexOf", &wrap_indexOf1<const QRegExp&> )
        
        .def( "insert", (QString& (QString::*)(int, const QString&))
                                &QString::insert,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "insert", (QString& (QString::*)(int, const QLatin1String&))
                                &QString::insert,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "insert", (QString& (QString::*)(int, const QChar*, int))
                                &QString::insert,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "insert", (QString& (QString::*)(int, QChar))
                                &QString::insert,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
                                
        .def( "isEmpty", &QString::isEmpty )
        .def( "isNull", &QString::isNull )
        
        .def( "lastIndexOf", &wrap_lastIndexOf3<const QString&> )
        .def( "lastIndexOf", &wrap_lastIndexOf2<const QString&> )
        .def( "lastIndexOf", &wrap_lastIndexOf1<const QString&> )
        .def( "lastIndexOf", &wrap_lastIndexOf3<QChar> )
        .def( "lastIndexOf", &wrap_lastIndexOf2<QChar> )
        .def( "lastIndexOf", &wrap_lastIndexOf1<QChar> )
        .def( "lastIndexOf", &wrap_lastIndexOf2<const QRegExp&> )
        .def( "lastIndexOf", &wrap_lastIndexOf1<const QRegExp&> )
        
        .def( "left", &QString::left )
    ;

    //split this interface over multiple files to reduce 
    //memory consumption of compiler (speed compilation 
    //on machines with 512MB of RAM)
    export_QString2(qstring_wrapper);
    export_QString3(qstring_wrapper);
}
