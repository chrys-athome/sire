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

#include "hanptr.hpp"

#include "Siren/errors.h"

using namespace Siren;
using namespace Siren::detail;

HanPtrBase::HanPtrBase(const Handle &handle) : ptr( handle.clone() )
{}

HanPtrBase::HanPtrBase(Handle *handle) : ptr(handle)
{}

HanPtrBase::HanPtrBase(const HanPtrBase &other) : ptr(other.ptr)
{}

HanPtrBase& HanPtrBase::operator=(const HanPtrBase &other)
{
    ptr = other.ptr;
    return *this;
}

HanPtrBase::~HanPtrBase()
{}

bool HanPtrBase::operator==(const Handle &handle) const
{
    if (ptr.get())
        return ptr->equals(handle);
    else
        return false;
}

bool HanPtrBase::operator!=(const Handle &handle) const
{
    return not this->operator==(handle);
}

bool HanPtrBase::operator==(const HanPtrBase &other) const
{
    if (ptr.get() == other.ptr.get())
        return true;
    else
    {
        if (ptr.get())
        {
            if (other.ptr.get())
                return ptr->equals(other);
        }
        
        return false;
    }
}

bool HanPtrBase::operator!=(const HanPtrBase &other) const
{
    return not this->operator==(other);
}

/** Detach this pointer from shared storage */
void HanPtrBase::detach()
{
    if (ptr.get() and not ptr.unique())
    {
        ptr.reset( ptr->clone() );
    }
}

/** Return whether or not this is the only pointer
    to the handle (note handle, not resource pointed
    to by that handle!) */
bool HanPtrBase::unique() const
{
    return ptr.unique();
}

bool HanPtrBase::isNull() const
{
    return ptr.get() == 0;
}

/** Assert that this handle pointer is not null 

    \throw Siren::nullptr_error
*/
void HanPtrBase::assertNotNull() const
{
    if (ptr.get() == 0)
        throw Siren::nullptr_error( QObject::tr(
                "Cannot dereference a null Handle pointer (HanPtr)."), CODELOC );
}

/** Allow automatic casting to a Handle 

    \throw Siren::nullptr_error
*/
HanPtrBase::operator const Handle&() const
{
    assertNotNull();
    return *ptr;
}

Handle* HanPtrBase::get()
{
    return ptr.get();
}

const Handle* HanPtrBase::get() const
{
    return ptr.get();
}

void HanPtrBase::throwCastingError(QString got_type, QString want_type)
{
    throw Siren::invalid_cast( QObject::tr(
            "It is not possible to cast a Handle of type %1 to type %2.")
                .arg(got_type, want_type), CODELOC );
}
