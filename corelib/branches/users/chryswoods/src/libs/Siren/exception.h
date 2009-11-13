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

#ifndef SIREN_EXCEPTION_H
#define SIREN_EXCEPTION_H

#include <exception>

#include <QString>
#include <QStringList>

#include <boost/shared_ptr.hpp>

#include <boost/assert.hpp>

#ifndef BOOST_ENABLE_ASSERT_HANDLER
#define BOOST_ENABLE_ASSERT_HANDLER 1
#endif

#include "object.h"

SIREN_BEGIN_HEADER

namespace Siren
{

/** This is the base class of all Siren exceptions.

    @author Christopher Woods
*/
class SIREN_EXPORT exception : public Object
{
public:
    exception();
    exception(QString error, QString place = QString::null);

    exception(const exception &other);

    virtual ~exception() throw();
    
    static void unpackAndThrow(const QByteArray &errordata);

    QString error() const throw();
    QString from() const throw();
    QStringList trace() const throw();
    QString where() const throw();
    QString why() const throw();
    QString pid() const throw();

    HASH_CODE hashCode() const throw();

    QString toString() const throw();

    virtual void throwSelf() const=0;

protected:
    QString err;  ///< The error associated with the exception.
    QString plce; ///< Description of from where the exception was thrown.
    QStringList bt; ///< Backtrace at the point that the exception was constructed
    QString pidstr; /**< String identifying the process from which
                             process/thread this exception was thrown */
};

/** Derive all exception classes using this template type */
template<class Derived, class Base>
class ImplementsException : public Base
{
public:
    ImplementsException();

    template<class T0>
    ImplementsException(const T0 &t0);

    template<class T0, class T1>
    ImplementsException(const T0 &t0, const T1 &t1);

    template<class T0, class T1, class T2>
    ImplementsException(const T0 &t0, const T1 &t1, const T2 &t2);

    template<class T0, class T1, class T2, class T3>
    ImplementsException(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3);

    template<class T0, class T1, class T2, class T3, class T4>
    ImplementsException(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                      const T4 &t4);

    template<class T0, class T1, class T2, class T3, class T4, class T5>
    ImplementsException(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                      const T4 &t4, const T5 &t5);

    template<class T0, class T1, class T2, class T3, class T4, 
             class T5, class T6>
    ImplementsException(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                      const T4 &t4, const T5 &t5, const T6 &t6);

    template<class T0, class T1, class T2, class T3, class T4, 
             class T5, class T6, class T7>
    ImplementsException(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3,
                      const T4 &t4, const T5 &t5, const T6 &t6, const T7 &t7);

    virtual ~ImplementsException() throw();

    ImplementsException<Derived,Base>& operator=(const Object &other);

    bool operator==(const Object &other) const;

    bool operator!=(const Object &other) const;

    static QString typeName();

    QString what() const;

    ImplementsException<Derived,Base>* clone() const;

    const Class& getClass() const;

    void copy(const Object &other);
    bool equals(const Object &other) const;

    void throwSelf() const;

protected:
    ImplementsException<Derived,Base>&
    operator=(const ImplementsException<Derived,Base> &other);

    bool operator==(const ImplementsException<Derived,Base> &other) const;
    bool operator!=(const ImplementsException<Derived,Base> &other) const;
};

void setProcessString(const QString &s);
void setThreadString(const QString &s);

QString getProcessString();
QString getThreadString();
QString getPIDString();

void printError(const Siren::exception &e);
void printError(const QString &s);

}

SIREN_EXPOSE_FUNCTION( Siren::printError )
SIREN_EXPOSE_FUNCTION( Siren::getProcessString )
SIREN_EXPOSE_FUNCTION( Siren::getThreadString )
SIREN_EXPOSE_FUNCTION( Siren::getPIDString )

SIREN_END_HEADER

#endif
