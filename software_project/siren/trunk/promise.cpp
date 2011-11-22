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
#include "Siren/none.h"
#include "Siren/workpacket.h"

#include "Siren/detail/promisedata.h"
#include "Siren/detail/workqueuedata.h"
#include "Siren/detail/workqueueitemdata.h"

using namespace Siren;
using namespace Siren::detail;

/////////
///////// Implementation of PromiseData
/////////

/** Null constructor */
PromiseData::PromiseData(const WorkQueueItem &item) 
            : noncopyable(), workitem(item.d), ready(false)
{}

/** Destructor */
PromiseData::~PromiseData()
{
    //tell the job to abort as there is no longer a promise
    //available to hold the result
    WorkQueueItem(workitem.lock()).abort();
}

/** Return whether or not the result is available */
bool PromiseData::available()
{
    MutexLocker lkr(&m);
    return ready;
}

/** Wait until the result is available, and then return the result */
Obj PromiseData::result()
{
    MutexLocker lkr(&m);
    
    while (not ready)
    {
        w.wait(&m);
    }
    
    return reslt;
}

/** Wait until the result is available */
void PromiseData::wait()
{
    MutexLocker lkr(&m);
    
    if (ready)
        return;
        
    w.wait(&m);
}

/** Wait until the result is available, or until 'ms' milliseconds
    has passed - this returns whether the result is available or not */
bool PromiseData::wait(int ms)
{
    MutexLocker lkr(&m);
    
    if (ready)
        return true;
        
    w.wait(&m, ms);
    
    return ready;
}

/** Set the promised result */
void PromiseData::setResult(const Obj &result)
{
    MutexLocker lkr(&m);
    
    if (ready)
        throw Siren::program_bug( String::tr(
                "It is a mistake to try to give a Promise two results! "
                "Trying to set result...\n"
                "%1\n"
                "...when already contain result...\n"
                "%2")
                    .arg(reslt.toString(), result.toString()), CODELOC );
                    
    reslt = result;
    ready = true;
    w.wakeAll();
}

/////////
///////// Implementation of Promise
/////////

/** Construct an empty Promise - this has a "None" result */
Promise::Promise()
{}

/** Internal function used by WorkQueue to construct a Promise from 
    the passed WorkQueueItem */
Promise::Promise(const WorkQueueItem &item)
        : d(new PromiseData(item))
{}

/** Construct an empty Promise - this is the same as a default-constructed promise */
Promise::Promise(const None &none)
{}

/** Construct a Promise with the supplied result - this is useful for occasions
    where the result is immediately available, but you still want to use the 
    Promise API */
Promise::Promise(const Object &object) : reslt(object)
{}

/** Copy constructor */
Promise::Promise(const Promise &other)
        : reslt(other.reslt), d(other.d)
{}

/** Destructor */
Promise::~Promise()
{}

/** Internal constructor used to create a promise to hold a future result */
Promise::Promise(const exp_shared_ptr<PromiseData>::Type &ptr) : d(ptr)
{}

/** Copy operator */
Promise& Promise::operator=(const Promise &other)
{
    d = other.d;
    reslt = other.reslt;
    return *this;
}

/** Comparison operator */
bool Promise::operator==(const Promise &other) const
{
    return d == other.d and reslt == other.reslt;
}

/** Comparison operator */
bool Promise::operator!=(const Promise &other) const
{
    return not operator==(other);
}

/** Return whether or not the result is available */
bool Promise::available() const
{
    if (d)
        return d->available();
    
    else
        return true;
}

/** Wait until the result is available */
void Promise::wait() const
{
    if (d)
        d->wait();
}

/** Wait until the result is available, or 'ms' milliseconds has 
    passed. This returns whether or not the result is available */
bool Promise::wait(int ms) const
{
    if (d)
        return d->wait(ms);
    else
        return true;
}

/** Return the result of the calculation - this will block until the result
    is available */
Obj Promise::result() const
{
    if (d)
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
    if (not d)
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
    if (not d)
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
    if (d)
    {
        if (d->available())
            const_cast<Promise*>(this)->reslt = d->result();
            
        const_cast<Promise*>(this)->d.reset();
    }
}

/** Abort the calculation if it has not finished in 'ms' milliseconds */
void Promise::abort(int ms) const
{
    if (d)
    {
        if (d->available() or d->wait(ms))
        {
            const_cast<Promise*>(this)->reslt = d->result();
        }
        
        const_cast<Promise*>(this)->d.reset();
    }
}

/** This internal function is called when the job attached to this promise
    is cancelled */
void Promise::jobCancelled()
{
    throw Siren::incomplete_code("TODO", CODELOC);
}

/** This internal function is called when the job attached to this promise
    is finished, and is used to supply the result */
void Promise::jobFinished(const Obj &result)
{
    throw Siren::incomplete_code("TODO", CODELOC);
}

/** This internal function is called when the job attached to this promise
    has started */
void Promise::jobStarted()
{
    throw Siren::incomplete_code("TODO", CODELOC);
}