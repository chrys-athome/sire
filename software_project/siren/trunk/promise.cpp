/********************************************\
  *
  *  Siren - C++ metaobject library
  *
  *  Copyright (C) 2011  Christopher Woods
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

#include "Siren/promise.h"
#include "Siren/siren.hpp"
#include "Siren/thread.h"
#include "Siren/waitcondition.h"
#include "Siren/mutex.h"
#include "Siren/exceptions.h"

#include "Siren/detail/promisedata.h"

using namespace Siren;
using namespace Siren::detail;

REGISTER_SIREN_CLASS( Siren::Promise )

/** Construct an empty Promise - this has a "None" result */
Promise::Promise() : Object()
{}

/** Construct an empty Promise - this is the same as a default-constructed promise */
Promise::Promise(const None &none) : Object()
{}

/** Construct a Promise with the supplied result - this is useful for occasions
    where the result is immediately available, but you still want to use the 
    Promise API */
Promise::Promise(const Object &object) : Object(), reslt(object)
{}

/** Copy constructor */
Promise::Promise(const Promise &other)
        : Object(other), reslt(other.reslt), d(other.d)
{}

/** Destructor */
Promise::~Promise()
{}

/** Internal constructor used by Thread and ThreadPool to create a promise
    to hold a future result */
Promise::Promise(const exp_shared_ptr<PromiseData>::Type &ptr)
        : Object(), d(ptr)
{}

/** Copy assignment operator */
void Promise::copy_object(const Promise &other)
{
    reslt = other.reslt;
    d = other.d;
    super::copy_object(other);
}

/** Comparison operator */
bool Promise::compare_object(const Promise &other) const
{
    return d.get() == other.d.get() and reslt == other.reslt and
           super::compare_object(other);
}

/** Return whether or not the result is available */
bool Promise::available() const
{
    if (d.get() == 0)
        return true;
    
    else
        return d->available();
}

/** Wait until the result is available */
void Promise::wait() const
{
    if (d.get() != 0)
        d->wait();
}

/** Wait until the result is available, or 'ms' milliseconds has 
    passed. This returns whether or not the result is available */
bool Promise::wait(int ms) const
{
    if (d.get() != 0)
        return d->wait(ms);
    else
        return true;
}

/** Return the result of the calculation - this will block until the result
    is available */
Obj Promise::result() const
{
    if (d.get() != 0)
    {
        const_cast<Promise*>(this)->reslt = d->result();
        const_cast<Promise*>(this)->d.reset();
    }
    
    return reslt;
}

/** Return the result of the calculation, if it is immediately available,
    or return 'def' instead */
Obj Promise::result(const Object &def) const
{
    if (d.get() == 0)
        return reslt;
    
    else
    {
        if (d->available())
        {
            const_cast<Promise*>(this)->reslt = d->result();
            const_cast<Promise*>(this)->d.reset();
            return reslt;
        }
        else
            return def;
    }
}

/** Return the result of the calculation, if it is available within 'ms' milliseconds,
    or return 'def' instead */
Obj Promise::result(const Object &def, int ms) const
{
    if (d.get() == 0)
        return reslt;
        
    else
    {
        if (d->available() or d->wait(ms))
        {
            const_cast<Promise*>(this)->reslt = d->result();
            const_cast<Promise*>(this)->d.reset();
            return reslt;
        }
        else
            return def;
    }
}

/** Abort the calculation. This returns whether or not the result
    of the calculation is available */
void Promise::abort() const
{
    if (d.get() != 0)
    {
        if (d->available())
            const_cast<Promise*>(this)->reslt = d->result();
            
        const_cast<Promise*>(this)->d.reset();
    }
}

/** Abort the calculation if it has not finished in 'ms' milliseconds */
void Promise::abort(int ms) const
{
    if (d.get() != 0)
    {
        if (d->available() or d->wait(ms))
        {
            const_cast<Promise*>(this)->reslt = d->result();
        }
        
        const_cast<Promise*>(this)->d.reset();
    }
}
