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

#ifndef SIREN_HANREF_H
#define SIREN_HANREF_H

#include "handle.h"

SIREN_BEGIN_HEADER

namespace Siren
{

/** This is a small and simple class that provides a reference
    to a handle. This is an explicitly shared reference to 
    an explicitly shared handle
*/
class SIREN_EXPORT HanRef
{
public:
    HanRef();
    HanRef(Handle *handle);
    HanRef(const Handle &handle);
    
    HanRef(const HanRef &other);
    
    ~HanRef();
    
    HanRef& operator=(const HanRef &other);
    
    bool operator==(const HanRef &other) const;
    bool operator!=(const HanRef &other) const;
    
    const Class& getClass() const;
    
    QString what() const;
    
    HanRef clone() const;
    
    bool test() const;
    bool test(Logger &logger) const;
    
    void copy(const Handle &other);
    
    bool equals(const Handle &other) const;
    
    uint hashCode() const;
    
    void lock();
    void unlock();
    
    bool tryLock();
    bool tryLock(int ms);
    
    bool isNull() const;
    
    template<class T>
    bool isA() const;
    
    template<class T>
    T& asA();
    
    template<class T>
    const T& asA() const;
    
    operator const Handle&() const;

private:
    void detach();

    static void throwNullPtrError(const QString &type_name);

    /** Explicitly shared pointer to the handle */
    boost::shared_ptr<Handle> d;
};

#ifndef SIREN_SKIP_INLINE_FUNCTIONS

/** Return whether or not this HandleRef refers to a Handle of type 'T' */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
bool HanRef::isA() const
{
    if (d.get() != 0)
        return d->isA<T>();
    else
        return false;
}

/** Return this HandleRef cast as type 'T'
    
    \throw Siren::invalid_cast
*/
template<class T>
SIREN_OUTOFLINE_TEMPLATE
T& HanRef::asA()
{
    if (d.get() == 0)
        this->throwNullPtrError( T::typeName() );
    
    return d->asA<T>();
}

/** Return this HandleRef cast as type 'T'
    
    \throw Siren::invalid_cast
*/
template<class T>
SIREN_OUTOFLINE_TEMPLATE
const T& HanRef::asA() const
{
    if (d.get() == 0)
        this->throwNullPtrError( T::typeName() );
    
    return d->asA<T>();
}

#endif // SIREN_SKIP_INLINE_FUNCTIONS

}

SIREN_EXPOSE_CLASS( Siren::HanRef )

SIREN_END_HEADER

#endif
