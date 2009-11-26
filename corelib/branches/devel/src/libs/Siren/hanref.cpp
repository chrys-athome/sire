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

#include "handle.h"
#include "class.h"
#include "hanref.h"
#include "stream.h"
#include "tester.h"
#include "logger.h"

#include "Siren/errors.h"

using namespace Siren;

/** Null constructor */
HanRef::HanRef()
{}

/** Construct for the pointer to the handle 'handle' - this 
    takes over ownership of this handle */
HanRef::HanRef(Handle *handle) : d(handle)
{}

/** Construct for the handle 'handle' - this creates a reference
    to a clone of this handle */
HanRef::HanRef(const Handle &handle) : d(handle.clone())
{}

/** Copy constructor */
HanRef::HanRef(const HanRef &other) : d(other.d)
{}

/** Destructor */
HanRef::~HanRef()
{}

/** Copy assignment operator */
HanRef& HanRef::operator=(const HanRef &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool HanRef::operator==(const HanRef &other) const
{
    if (d.get() == other.d.get())
        return true;
    else
    {
        const void *o0 = 0;
        const void *o1 = 0;
        
        if (d.get())
            o0 = d->resource_lock.get();
            
        if (other.d.get())
            o1 = other.d->resource_lock.get();
            
        return o0 == o1;
    }
}

void HanRef::detach()
{
    if (d.get())
    {
        if (not d.unique())
            d.reset( d->clone() );
    }
}

/** Comparison operator */
bool HanRef::operator!=(const HanRef &other) const
{
    return not HanRef::operator==(other);
}

/** Return the class type of the referred Handle */
const Class& HanRef::getClass() const
{
    if (d.get() == 0)
    {
        None n;
        return n.getClass();
    }
    else
        return d->getClass();
}

/** Return the type name of the referred Handle */
QString HanRef::what() const
{
    if (d.get() == 0)
    {
        None n;
        return n.what();
    }
    else
    {
        return d->what();
    }
}

/** Return a copy of this reference */
HanRef HanRef::copy() const
{
    HanRef r;
    
    if (d.get() != 0)
    {
        r.d.reset( d->clone() );
    }
    
    return r;
}

/** Test this handle */
bool HanRef::test() const
{
    if (d.get() != 0)
        return d->test();
    else
        return true;
}

/** Test this handle */
bool HanRef::test(Logger &logger) const
{
    if (d.get() != 0)
        return d->test(logger);
    else
        return true;
}

/** Copy into this handle the contents of 'other' */
void HanRef::copy(const Handle &other)
{
    detach();
    
    if (d.get() != 0)
        d->copy(other);
}

/** Return whether or not the contained handle equals 'other' */
bool HanRef::equals(const Handle &other) const
{
    if (d.get() != 0)
        return d->equals(other);
    else
        return false;
}

/** Return the hash code for the referred handle */
uint HanRef::hashCode() const
{
    if (d.get() != 0)
        return d->hashCode();
    else
        return 0;
}

/** Lock the referred handle */
void HanRef::lock()
{
    if (d.get() != 0)
        d->lock();
}

/** Unlock the referred handle */
void HanRef::unlock()
{
    if (d.get() != 0)
        d->unlock();
}

/** Try to lock the referred handle, returning whether a  
    lock was obtained - if it was, then you are responsible
    for unlocking the Handle */
bool HanRef::tryLock()
{
    if (d.get() != 0)
        return d->tryLock();
    else
        return true;
}

/** Try to lock the referred handle, waiting at most
    'ms' milliseconds, returning whether a  
    lock was obtained - if it was, then you are responsible
    for unlocking the Handle */
bool HanRef::tryLock(int ms)
{
    if (d.get() != 0)
        return d->tryLock(ms);
    else
        return true;
}

/** Return whether or not this reference is null */
bool HanRef::isNull() const
{
    return d.get() == 0;
}

/** Allow automatic casting to a 'Handle'

    \throw Siren::nullptr_error
*/
HanRef::operator const Handle&() const
{
    if (d.get() == 0)
        throw Siren::nullptr_error( QObject::tr(
                "You cannot dereference a null HanRef handle reference!"),
                    CODELOC );

    return *d;
}

void HanRef::throwNullPtrError(const QString &type_name)
{
    throw Siren::nullptr_error( QObject::tr(
            "You cannot cast a null handle reference to type %1.")
                .arg(type_name), CODELOC );
}
