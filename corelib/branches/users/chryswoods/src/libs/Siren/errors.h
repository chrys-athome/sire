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

#ifndef SIREN_ERRORS_H
#define SIREN_ERRORS_H

#include "Siren/exception.h"
#include "Siren/version_error.h"

#include <stdexcept>
#include <QObject>

class QFile;
class QProcess;

SIREN_BEGIN_HEADER

namespace Siren
{

namespace detail
{
    QString fileError(const QFile &f);
    QString processError(const QString &executable, const QProcess &p);
}

/** This exception is thrown when the code detects a programming bug. I hope that
    eventually Sire will not throw too many of these!

    @author Christopher Woods
*/
class SIREN_EXPORT program_bug : public ImplementsException<program_bug,exception>
{
public:
    program_bug();

    program_bug(QString err, QString place = QString::null);

    program_bug(const program_bug &other);

    ~program_bug() throw();
};

/** This exception is thrown when unsupported hardware is detected (e.g. unsupported
    openGL system)

    @author Christopher Woods
*/
class SIREN_EXPORT unsupported : public ImplementsException<unsupported,exception>
{
public:
    unsupported();

    unsupported(QString err, QString place = QString::null);

    unsupported(const unsupported &other);

    ~unsupported() throw();
};

/** This exception is thrown when an invalid key is used, e.g. a hash or set is
    used with a non-existant key.
    @author Christopher Woods
*/
class SIREN_EXPORT invalid_key : public ImplementsException<invalid_key,exception>
{
public:
    invalid_key();

    invalid_key(QString err, QString place = QString::null);

    invalid_key(const invalid_key &other);

    ~invalid_key() throw();
};

/** This exception is thrown when an invalid index is used, e.g. an invalid index
    is used to access an array.
    @author Christopher Woods
*/
class SIREN_EXPORT invalid_index : public ImplementsException<invalid_index,exception>
{
public:
    invalid_index();

    invalid_index(QString err, QString place=QString::null);

    invalid_index(const invalid_index &other);

    ~invalid_index() throw();
};

/** This exception is thrown when an invalid cast is attempted, e.g. a dynamic_cast
    that fails.
    @author Christopher Woods
*/
class SIREN_EXPORT invalid_cast : public ImplementsException<invalid_cast,exception>
{
public:
    invalid_cast();

    invalid_cast(QString err, QString place = QString::null);
    
    invalid_cast(const invalid_cast &other);

    ~invalid_cast() throw();
};

/** This exception is thrown whenever something incompatible is requested, e.g.
    trying to combine two incompatible objects.

    @author Christopher Woods
*/
class SIREN_EXPORT incompatible_error 
        : public ImplementsException<incompatible_error,exception>
{
public:
    incompatible_error();

    incompatible_error(QString err, QString place = QString::null);

    incompatible_error(const incompatible_error &other);

    ~incompatible_error() throw();
};

/** This exception is thrown when request is made of an unknown type, e.g.
    when using QMetaType

    @author Christopher Woods
*/
class SIREN_EXPORT unknown_type : public ImplementsException<unknown_type,exception>
{
public:
    unknown_type();
    
    unknown_type(QString err, QString place = QString::null);

    unknown_type(const unknown_type &other);

    ~unknown_type() throw();
};

/** This exception is thrown whenever an attempt is made to copy an uncopyable object.
    This will normally be a result of a programming bug!
    @author Christopher Woods
*/
class SIREN_EXPORT noncopyable_error 
          : public ImplementsException<noncopyable_error,exception>
{
public:
    noncopyable_error();
    
    noncopyable_error(QString err, QString place=QString::null);

    noncopyable_error(const noncopyable_error &other);

    ~noncopyable_error() throw();
};

/** This exception is thrown when an attempt is made to access a null pointer.
    @author Christopher Woods
*/
class SIREN_EXPORT nullptr_error : public ImplementsException<nullptr_error,exception>
{
public:
    nullptr_error();

    nullptr_error(QString err, QString place=QString::null);

    nullptr_error(const nullptr_error &other);

    ~nullptr_error() throw();
};

/** This exception is thrown whenever there is an error with locking a resource,
    e.g. a file lock, or mutex lock.
    @author Christopher Woods
*/
class SIREN_EXPORT lock_error : public ImplementsException<lock_error,exception>
{
public:
    lock_error();

    lock_error(QString err, QString place=QString::null);

    lock_error(const lock_error &other);

    ~lock_error() throw();
};

/** This exception is thrown whenever an assertation fails. This is normally
    a result of a coding bug where a function is being used in an unintended
    and unsupported way.
    @author Christopher Woods
*/
class SIREN_EXPORT assertation_error : 
            public ImplementsException<assertation_error,exception>
{
public:
    assertation_error();

    assertation_error(QString err, QString place=QString::null);

    assertation_error(const assertation_error &other);

    ~assertation_error() throw();
};

/** This exception is thrown whenever there is an file handling error */
class SIREN_EXPORT file_error : public ImplementsException<file_error,exception>
{
public:
    file_error();

    file_error(const QString &err, const QString &place=QString::null);
    
    file_error(const QFile &file, QString place=QString::null);
    
    file_error(const file_error &other);

    ~file_error() throw();
};

/** This exception is thrown whenever there is an error with a child process */
class SIREN_EXPORT process_error : public ImplementsException<process_error,exception>
{
public:
    process_error();

    process_error(const QString &err, const QString place=QString::null);

    process_error(const QString &executable, const QProcess &process,
                  QString place=QString::null);

    process_error(const process_error &other);
    
    ~process_error() throw();
};

/** This exception is thrown whenever a request is made of a resource
    that is just not available */
class SIREN_EXPORT unavailable_resource 
        : ImplementsException<unavailable_resource,exception>
{
public:
    unavailable_resource();

    unavailable_resource(QString err, QString place=QString::null);

    unavailable_resource(const unavailable_resource &other);

    ~unavailable_resource() throw();
};

/** This exception is thrown whenever there is an input/output error */
class SIREN_EXPORT io_error : public ImplementsException<io_error,exception>
{
public:
    io_error();

    io_error(QString err, QString place=QString::null);

    io_error(const io_error &other);

    ~io_error() throw();
};

/** This exception is thrown whenever a function is called with invalid arguments.
    This is normally as a result of a programming bug, or an attempt to use a function
    in an unsupported way.
    @author Christopher Woods
*/
class SIREN_EXPORT invalid_arg : public ImplementsException<invalid_arg,exception>
{
public:
    invalid_arg();

    invalid_arg(QString err, QString place=QString::null);

    invalid_arg(const invalid_arg &other);

    ~invalid_arg() throw();
};

/** This exception is thrown whenever the code detects that it has entered an
    invalid state. This will be as a result of a programming bug, or undocumented
    use of the code.
    @author Christopher Woods
*/
class SIREN_EXPORT invalid_state : public ImplementsException<invalid_state,exception>
{
public:
    invalid_state();

    invalid_state(QString err, QString place=QString::null);

    invalid_state(const invalid_state &other);

    ~invalid_state() throw();
};

/** This exception is thrown whenever an invalid operation is requested

    @author Christopher Woods
*/
class SIREN_EXPORT invalid_operation 
        : public ImplementsException<invalid_operation,exception>
{
public:
    invalid_operation();

    invalid_operation(QString err, QString place=QString::null);

    invalid_operation(const invalid_operation &other);

    ~invalid_operation() throw();
};

/** This exception is thrown whenever the program tries to use code that has yet
    to be written. This is definitely a programming bug, and results from the use
    of stubs that I have forgotton to expand!
    @author Christopher Woods
*/
class SIREN_EXPORT incomplete_code 
        : public ImplementsException<incomplete_code,exception>
{
public:
    incomplete_code();

    incomplete_code(QString err, QString place=QString::null);

    incomplete_code(const incomplete_code &other);

    ~incomplete_code() throw();
};

/** This exception is used to translate a std::exception into a Siren::exception */
class SIREN_EXPORT std_exception : public ImplementsException<std_exception,exception>
{
public:
    std_exception();
    std_exception(const std::logic_error &error);
    std_exception(const std::domain_error &error);
    std_exception(const std::invalid_argument &error);
    std_exception(const std::length_error &error);
    std_exception(const std::out_of_range &error);
    std_exception(const std::runtime_error &error);
    std_exception(const std::range_error &error);
    std_exception(const std::overflow_error &error);
    std_exception(const std::underflow_error &error);
    std_exception(const std::bad_alloc &error);
    std_exception(const std::exception &error);

    ~std_exception() throw();

protected:
    static QString getWhatString(QString typstring, const std::exception &error);
};

/** This exception is thrown whenever an unidentified exception needs translating into
    a Siren::exception, or when an unidentified Siren::exception is detected.
    
    @author Christopher Woods
*/
class SIREN_EXPORT unknown_error : public ImplementsException<unknown_error,exception>
{
public:
    unknown_error();

    unknown_error(QString err, QString place=QString::null);

    unknown_error(const unknown_error &other);

    ~unknown_error() throw();
};

/** This exception is thrown whenever a problem is detected
    that involves dependencies, e.g. a dependency is missing,
    or a dependency may be about to be broken.
    
    @author Christopher Woods
*/
class SIREN_EXPORT dependency_error 
        : public ImplementsException<dependency_error,exception>
{
public:
    dependency_error();

    dependency_error(QString err, QString place=QString::null);

    dependency_error(const dependency_error &other);

    ~dependency_error() throw();
};

/** This exception is thrown whenever corrupted data is detected
    
    @author Christopher Woods
*/
class SIREN_EXPORT corrupted_data 
        : public ImplementsException<corrupted_data,exception>
{
public:
    corrupted_data();

    corrupted_data(QString err, QString place=QString::null);

    corrupted_data(const corrupted_data &other);

    ~corrupted_data() throw();
};

/** This exception is thrown whenever there is an error with an
    ID number

    @author Christopher Woods
*/
class SIREN_EXPORT id_error : public ImplementsException<id_error,exception>
{
public:
    id_error();

    id_error(QString err, QString place = QString::null);

    id_error(const id_error &other);

    ~id_error() throw();
};

}

Q_DECLARE_METATYPE(Siren::program_bug)
Q_DECLARE_METATYPE(Siren::unsupported)
Q_DECLARE_METATYPE(Siren::id_error)
Q_DECLARE_METATYPE(Siren::invalid_key)
Q_DECLARE_METATYPE(Siren::invalid_index)
Q_DECLARE_METATYPE(Siren::invalid_cast)
Q_DECLARE_METATYPE(Siren::noncopyable_error)
Q_DECLARE_METATYPE(Siren::nullptr_error)
Q_DECLARE_METATYPE(Siren::lock_error)
Q_DECLARE_METATYPE(Siren::assertation_error)
Q_DECLARE_METATYPE(Siren::incompatible_error)
Q_DECLARE_METATYPE(Siren::file_error)
Q_DECLARE_METATYPE(Siren::io_error)
Q_DECLARE_METATYPE(Siren::process_error)
Q_DECLARE_METATYPE(Siren::invalid_arg)
Q_DECLARE_METATYPE(Siren::invalid_state)
Q_DECLARE_METATYPE(Siren::invalid_operation)
Q_DECLARE_METATYPE(Siren::unavailable_resource)
Q_DECLARE_METATYPE(Siren::incomplete_code)
Q_DECLARE_METATYPE(Siren::std_exception)
Q_DECLARE_METATYPE(Siren::unknown_error)
Q_DECLARE_METATYPE(Siren::unknown_type)
Q_DECLARE_METATYPE(Siren::corrupted_data)
Q_DECLARE_METATYPE(Siren::dependency_error)

SIREN_END_HEADER

#endif
