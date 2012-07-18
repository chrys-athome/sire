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
#include "mutex.h"
#include "stream.h"

SIREN_BEGIN_HEADER

namespace Siren
{

/** This is the base class of all Siren exceptions.

    @author Christopher Woods
*/
class SIREN_EXPORT exception : public Extends<exception,Object>
{
public:
    exception();
    exception(QString error, QString place = QString::null);

    exception(const exception &other);

    virtual ~exception() throw();

    static QString typeName();

    QString error() const throw();
    QString from() const throw();
    QStringList trace() const throw();
    QString where() const throw();
    QString why() const throw();
    QString pid() const throw();

    uint hashCode() const;

    QString toString() const throw();

    bool test(Logger &logger) const;

    void stream(Stream &s);

    virtual void throwSelf() const=0;

protected:
    virtual bool testException() const=0;

    exception& operator=(const exception &other);
    
    bool operator==(const exception &other) const;
    bool operator!=(const exception &other) const;

private:
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
    ImplementsException<Derived,Base>& operator=(const Derived &other);

    bool operator==(const Object &other) const;
    bool operator!=(const Object &other) const;
    
    bool operator==(const Derived &other) const;
    bool operator!=(const Derived &other) const;

    static QString typeName();

    QString what() const;

    const Class& getClass() const;

    void stream(Siren::Stream &s);

    void copy(const Object &other);
    bool equals(const Object &other) const;

    void throwSelf() const;

protected:
    static const Class& createTypeInfo();

    ImplementsException<Derived,Base>* ptr_clone() const;
    
    bool testException() const;

    typedef Base super;

private:
    static const Class* class_typeinfo;
};

void setProcessString(const QString &s);
void setThreadString(const QString &s);

QString getProcessString();
QString getThreadString();
QString getPIDString();

void printError(const Siren::exception &e);
void printError(const QString &s);

#ifndef SIREN_SKIP_INLINE_FUNCTIONS

//////
////// Implementation of 'ImplementsException'
//////

template<class Derived, class Base>
const Class* ImplementsException<Derived,Base>::class_typeinfo = 0;

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
ImplementsException<Derived,Base>::ImplementsException() : Base()
{}

template<class Derived, class Base>
template<class T0>
SIREN_OUTOFLINE_TEMPLATE
ImplementsException<Derived,Base>::ImplementsException(const T0 &t0) : Base(t0)
{}

template<class Derived, class Base>
template<class T0, class T1>
SIREN_OUTOFLINE_TEMPLATE
ImplementsException<Derived,Base>::ImplementsException(const T0 &t0, const T1 &t1)
                                  : Base(t0, t1)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2>
SIREN_OUTOFLINE_TEMPLATE
ImplementsException<Derived,Base>::ImplementsException(const T0 &t0, const T1 &t1, 
                                                       const T2 &t2)
                                  : Base(t0, t1, t2)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3>
SIREN_OUTOFLINE_TEMPLATE
ImplementsException<Derived,Base>::ImplementsException(const T0 &t0, const T1 &t1, 
                                                       const T2 &t2, const T3 &t3)
                                  : Base(t0, t1, t2, t3)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4>
SIREN_OUTOFLINE_TEMPLATE
ImplementsException<Derived,Base>::ImplementsException(const T0 &t0, const T1 &t1,
                                                       const T2 &t2, const T3 &t3, 
                                                       const T4 &t4)
                                  : Base(t0, t1, t2, t3, t4)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4, class T5>
SIREN_OUTOFLINE_TEMPLATE
ImplementsException<Derived,Base>::ImplementsException(const T0 &t0, const T1 &t1, 
                                                       const T2 &t2, const T3 &t3, 
                                                       const T4 &t4, const T5 &t5)
                                  : Base(t0, t1, t2, t3, t4, t5)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4, 
         class T5, class T6>
SIREN_OUTOFLINE_TEMPLATE
ImplementsException<Derived,Base>::ImplementsException(const T0 &t0, const T1 &t1, 
                                                       const T2 &t2, const T3 &t3, 
                                                       const T4 &t4, const T5 &t5, 
                                                       const T6 &t6)
                                  : Base(t0, t1, t2, t3, t4, t5, t6)
{}

template<class Derived, class Base>
template<class T0, class T1, class T2, class T3, class T4, 
         class T5, class T6, class T7>
SIREN_OUTOFLINE_TEMPLATE
ImplementsException<Derived,Base>::ImplementsException(const T0 &t0, const T1 &t1, 
                                                       const T2 &t2, const T3 &t3, 
                                                       const T4 &t4, const T5 &t5, 
                                                       const T6 &t6, const T7 &t7)
                                  : Base(t0, t1, t2, t3, t4, t5, t6, t7)
{}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
ImplementsException<Derived,Base>::~ImplementsException() throw()
{}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
QString ImplementsException<Derived,Base>::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Derived>() );
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
QString ImplementsException<Derived,Base>::what() const
{
    return ImplementsException<Derived,Base>::typeName();
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
ImplementsException<Derived,Base>* ImplementsException<Derived,Base>::ptr_clone() const
{
    const Derived* derived_this = dynamic_cast<const Derived*>(this);

    BOOST_ASSERT( derived_this );
    
    return new Derived( *derived_this );
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
void ImplementsException<Derived,Base>::copy(const Object &other)
{
    static_cast<Derived*>(this)->operator=( other.asA<Derived>() );
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
bool ImplementsException<Derived,Base>::equals(const Object &other) const
{
    if (not other.isA<Derived>())
        return false;
    else
        return static_cast<const Derived*>(this)->operator==( other.asA<Derived>() );
}

/** Return the class typeinfo object for 'Derived' */
template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
const Class& ImplementsException<Derived,Base>::createTypeInfo()
{
    if ( ImplementsException<Derived,Base>::class_typeinfo == 0 )
    {
        MutexLocker lkr( &(Object::globalLock()) );
        
        if ( ImplementsException<Derived,Base>::class_typeinfo == 0 )
        {
            const Class &base_class = Base::createTypeInfo();
            QStringList interfaces = Derived::listInterfaces();

            const RegisterMetaType *r = RegisterObject<Derived>::getRegistration();
                    
            if (not r)
                Object::throwUnregisteredMetaTypeError( Derived::typeName() );
        
            ImplementsException<Derived,Base>::class_typeinfo = 
                    Object::registerConcreteClass(r, base_class, interfaces);
        }
    }

    return *(ImplementsException<Derived,Base>::class_typeinfo);
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
const Class& ImplementsException<Derived,Base>::getClass() const
{
    return ImplementsException<Derived,Base>::createTypeInfo();
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
ImplementsException<Derived,Base>&
ImplementsException<Derived,Base>::operator=(const Derived &other)
{
    Base::operator=(other);
    return *this;
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
bool ImplementsException<Derived,Base>::operator==(const Derived &other) const
{
    return Base::operator==(other);
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
bool ImplementsException<Derived,Base>::operator!=(const Derived &other) const
{
    return Base::operator!=(other);
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
ImplementsException<Derived,Base>&
ImplementsException<Derived,Base>::operator=(const Object &other)
{
    Base::operator=(other.asA<Derived>());
    return *this;
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
bool ImplementsException<Derived,Base>::operator==(const Object &other) const
{
    if (not other.isA<Derived>())
        return false;

    return Base::operator==(other.asA<Derived>());
}

template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
bool ImplementsException<Derived,Base>::operator!=(const Object &other) const
{
    if (not other.isA<Derived>())
        return true;

    return Base::operator!=(other.asA<Derived>());
}

/** Stream this exception */
template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
void ImplementsException<Derived,Base>::stream(Siren::Stream &s)
{
    s.assertVersion<Derived>(1);
    Siren::Schema schema = s.item<Derived>();
    super::stream( schema.base() );
}

/** Throw this exception */
template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
void ImplementsException<Derived,Base>::throwSelf() const
{
    throw Derived( static_cast<const Derived&>(*this) );
}

/** Test this exception */
template<class Derived, class Base>
SIREN_OUTOFLINE_TEMPLATE
bool ImplementsException<Derived,Base>::testException() const
{
    try
    {
        this->throwSelf();
    }
    catch(const Derived &self)
    {}
    
    return true;
}

#endif // SIREN_SKIP_INLINE_FUNCTIONS

}

SIREN_EXPOSE_FUNCTION( Siren::printError )
SIREN_EXPOSE_FUNCTION( Siren::getProcessString )
SIREN_EXPOSE_FUNCTION( Siren::getThreadString )
SIREN_EXPOSE_FUNCTION( Siren::getPIDString )

SIREN_END_HEADER

#endif
