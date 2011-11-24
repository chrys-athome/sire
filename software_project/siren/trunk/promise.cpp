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
#include "Siren/timer.h"
#include "Siren/obj.hpp"
#include "Siren/object.hpp"

#include "Siren/detail/promisedata.h"
#include "Siren/detail/workqueuedata.h"
#include "Siren/detail/workqueueitemdata.h"

using namespace Siren;
using namespace Siren::detail;

/////////
///////// Implementation of PromiseData
/////////

/** Construct the promise that will be processed by the 
    passed WorkQueueItem */
PromiseData::PromiseData(const WorkQueueItem &item) 
            : noncopyable(), workitem(item.d), 
              has_started(false), is_cancelled(false), has_finished(false)
{}

/** Construct the promise that had already been processed,
    and has the passed result */
PromiseData::PromiseData(const Obj &result)
            : noncopyable(), reslt(result), 
              has_started(true), is_cancelled(false), has_finished(true)
{}

/** Destructor */
PromiseData::~PromiseData()
{
    //tell the job to abort as there is no longer a promise
    //available to hold the result
    workitem.abort();
}

/** Return a string representation of the promise */
String PromiseData::toString()
{
    MutexLocker lkr(&m);
    
    if (has_finished)
        return String::tr("Promise( hasFinished()==true, result()==\"%1\" )")
                            .arg(reslt.toString());
    
    else if (is_cancelled)
        return String::tr("Promise( isCancelled()==true, workPacket()==\"%1\" )")
                            .arg( WorkQueueItem(workitem).workPacket().toString() );
                            
    else if (has_started)
        return String::tr("Promise( hasStarted()==true, workPacket()==\"%1\" )")
                            .arg( WorkQueueItem(workitem).workPacket().toString() );

    else
        return String::tr("Promise( workPacket()==\"%1\" )")
                            .arg( WorkQueueItem(workitem).workPacket().toString() );
}

/** Return whether or not the job has finished */
bool PromiseData::hasFinished()
{
    MutexLocker lkr(&m);
    return has_finished;
}

/** Return whether or not the job has started */
bool PromiseData::hasStarted() 
{
    MutexLocker lkr(&m);
    return has_started;
}

/** Return whether or not the job has been cancelled */
bool PromiseData::isCancelled()
{
    MutexLocker lkr(&m);
    return is_cancelled;
}

/** Wait until the result is available, and then return the result */
Obj PromiseData::result()
{
    MutexLocker lkr(&m);
    
    while (not has_finished)
    {
        w.wait(&m);
        
        if (is_cancelled)
            return None();
    }
    
    return reslt;
}

/** Wait until the result is available */
void PromiseData::wait()
{
    MutexLocker lkr(&m);
    
    while (not (has_finished or is_cancelled))
    {
        w.wait(&m);
    }
}

/** Wait until the result is available, or until 'ms' milliseconds
    has passed - this returns whether the result is available or not */
bool PromiseData::wait(int ms)
{
    MutexLocker lkr(&m);
    
    Timer t = Timer::start();
    
    while (not (has_finished or is_cancelled))
    {
        int new_ms = ms - t.elapsed();
    
        if (new_ms <= 0 or is_cancelled)
            return false;
            
        w.wait(&m, new_ms);
    }

    return true;
}

/** Wait until the job has started */
void PromiseData::waitForStarted()
{
    MutexLocker lkr(&m);
    
    while (not has_started)
    {
        if (is_cancelled)
            return;
    
        w.wait(&m);
    }
}

/** Wait until the job has started, or until 'ms' milliseconds
    has passed - this returns whether or not the job has started */
bool PromiseData::waitForStarted(int ms)
{
    MutexLocker lkr(&m);
    
    Timer t = Timer::start();
    
    while (not has_started)
    {
        int new_ms = ms - t.elapsed();
    
        if (new_ms <= 0 or is_cancelled)
            return false;
            
        w.wait(&m, new_ms);
    }

    return true;
}

/** Set the promised result */
void PromiseData::setResult(const Obj &result)
{
    if (has_finished)
        throw Siren::program_bug( String::tr(
                "It is a mistake to try to give a Promise two results! "
                "Trying to set result...\n"
                "%1\n"
                "...when already contain result...\n"
                "%2")
                    .arg(reslt.toString(), result.toString()), CODELOC );
                    
    reslt = result;
    has_finished = true;
    w.wakeAll();
}

/** Use to say that the job has started */
void PromiseData::jobStarted()
{
    MutexLocker lkr(&m);
    
    if (not has_started)
    {
        has_started = true;
        w.wakeAll();
    }
}

/** Use to say that the job has been cancelled */
void PromiseData::jobCancelled()
{
    MutexLocker lkr(&m);
    
    if (not is_cancelled)
    {
        is_cancelled = true;
        w.wakeAll();
    }
}

/** Use to say that the job has finished, with the passed result */
void PromiseData::jobFinished(const Obj &result)
{
    MutexLocker lkr(&m);
    this->setResult(result);
}

/** Use to say that that the part of the job identified with 'i' out of 'n'
    has finished, and set the value of this part. If all parts have been
    set, then they will be merged together to make the final result */
void PromiseData::jobFinished(const Obj &result, int i, int n)
{
    if (n <= 0 or i < 0 or i >= n)
        throw Siren::program_bug( String::tr(
                "Cannot set the result as the index or count "
                "are really, badly wrong!\n(%1 of %2) == %3")
                    .arg(i).arg(n).arg(result.toString()), CODELOC );

    MutexLocker lkr(&m);
    
    if (has_finished)
        throw Siren::program_bug( String::tr(
                "It is a mistake to try to give a Promise two results! "
                "Trying to set result (%1 of %2)...\n"
                "%3\n"
                "...when already contain result...\n"
                "%4")
                    .arg(i).arg(n)
                    .arg(reslt.toString(), result.toString()), CODELOC );

    if (got_result.isEmpty())
    {
        //this is the first of the n parts
        got_result = Set<int>::Type();
        got_result.reserve(n);
        result_part = Vector<Obj>::Type(n);
    }
    
    if (n != result_part.count())    
        throw Siren::program_bug( String::tr(
                "It is a mistake to try to set part %1 of %2, when the number "
                "of parts is equal to %3...")
                    .arg(i).arg(n).arg(result_part.count()), CODELOC );
    
    if (got_result.contains(i))
        throw Siren::program_bug( String::tr(
                "It is a mistake to try to set part %1 of %2 as the Promise "
                "already has a result for this part...\n"
                "%3\n"
                "...versus...\n"
                "%4")
                    .arg(i).arg(n)
                    .arg(result.toString(), result_part[i].toString()), CODELOC );

    result_part[i] = result;
    got_result.insert(i);
    
    //check to see if all of the results have been collected :-)
    if (got_result.count() >= n)
    {
        Obj workpacket = WorkQueueItem(workitem).workPacket();
        
        if (workpacket.isNone())
        {
            //there is no work packet to combine the result, so there is no result...
            this->setResult(None());
            lkr.unlock();
            return;
        }

        Obj final_result;

        try
        {
            final_result = workpacket.asA<WorkPacket>().reduce(result_part);
        }
        catch(const Exception &e)
        {
            final_result = e;
        }
        catch(const std::exception &e)
        {
            final_result = standard_exception(e, CODELOC);
        }
        catch(...)
        {
            final_result = unknown_exception(CODELOC);
        }
        
        this->setResult(final_result);
    }
    
    lkr.unlock();
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
{
    d->self = d;
}

/** Construct an empty Promise - this is the same as a default-constructed promise */
Promise::Promise(const None &none)
{}

/** Construct a Promise with the supplied result - this is useful for occasions
    where the result is immediately available, but you still want to use the 
    Promise API */
Promise::Promise(const Object &object) : reslt(object)
{
    if (not reslt.isNone())
    {
        d.reset(new PromiseData(reslt));
        d->self = d;
    }
}

/** Construct from the passed reference */
Promise::Promise(const PromiseRef &ref) : d(ref.d.lock())
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

/** Comparison operator */
bool Promise::operator==(const PromiseRef &other) const
{
    return other.operator==(*this);
}

/** Comparison operator */
bool Promise::operator!=(const PromiseRef &other) const
{
    return other.operator!=(*this);
}

/** Return whether or not the calculation has finished (result
    available or job cancelled) */
bool Promise::hasFinished() const
{
    if (d)
        return d->hasFinished();
    else
        return true;
}

/** Return whether or not the job has started */
bool Promise::hasStarted() const
{
    if (d)
        return d->hasStarted();
    else
        return true;
}

/** Return whether or not the job is cancelled */
bool Promise::isCancelled() const
{
    if (d)
        return d->isCancelled();
    else
        return false;
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

/** Wait until the job has started */
void Promise::waitForStarted() const
{
    if (d)
        d->waitForStarted();
}

/** Wait until the job has started, or for 'ms' milliseconds has passed.
    This returns whether or not the job has started */
bool Promise::waitForStarted(int ms) const
{
    if (d)
        return d->waitForStarted(ms);
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
        if (d->hasFinished())
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
        if (d->wait(ms))
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
        if (d->hasFinished())
            const_cast<Promise*>(this)->reslt = d->result();
            
        const_cast<Promise*>(this)->d.reset();
    }
}

/** Abort the calculation if it has not finished in 'ms' milliseconds */
void Promise::abort(int ms) const
{
    if (d)
    {
        if (d->wait(ms))
        {
            const_cast<Promise*>(this)->reslt = d->result();
        }
        
        const_cast<Promise*>(this)->d.reset();
    }
}

/** Return a string representation of this promise */
String Promise::toString() const
{
    if (d)
        return d->toString();

    else
        return String::tr("Promise( result() == %1 )").arg( reslt.toString() );
}

/** This internal function is called when the job attached to this promise
    is cancelled by the queue */
void Promise::jobCancelled()
{
    if (d)
        d->jobCancelled();
}

/** This internal function is called when the job attached to this promise
    is finished, and is used to supply the result */
void Promise::jobFinished(const Obj &result)
{
    if (d)
        d->jobFinished(result);
}

/** This internal function is called when part 'i' of 'n' parts of this
    work has finished */
void Promise::jobFinished(const Obj &result, int i, int n)
{
    if (d)
        d->jobFinished(result, i, n);
}

/** This internal function is called when the job attached to this promise
    has started */
void Promise::jobStarted()
{
    if (d)
        d->jobStarted();
}

/////////
///////// Implementation of PromiseRef
/////////

/** Null constructor */
PromiseRef::PromiseRef()
{}

/** Construct from the passed promise */
PromiseRef::PromiseRef(const Promise &promise) : d(promise.d)
{}

/** Copy constructor */
PromiseRef::PromiseRef(const PromiseRef &other) : d(other.d)
{}

/** Destructor */
PromiseRef::~PromiseRef()
{}

/** Copy assignment operator */
PromiseRef& PromiseRef::operator=(const PromiseRef &other)
{
    d = other.d;
}

/** Comparison operator */
bool PromiseRef::operator==(const PromiseRef &other) const
{
    return d.lock().get() == other.d.lock().get();
}

/** Comparison operator */
bool PromiseRef::operator!=(const PromiseRef &other) const
{
    return not operator==(other);
}

/** Comparison operator */
bool PromiseRef::operator==(const Promise &other) const
{
    return Promise(*this) == other;
}

/** Comparison operator */
bool PromiseRef::operator!=(const Promise &other) const
{
    return not operator==(other);
}

/** Return whether or not this reference has expired */
bool PromiseRef::isNull() const
{
    return d.expired();
}

/** Call this to cancel the job */
void PromiseRef::jobCancelled()
{
    exp_shared_ptr<PromiseData>::Type ptr = d.lock();

    if (ptr)
        ptr->jobCancelled();
}

/** Return a string representation of this promise */
String PromiseRef::toString() const
{
    exp_shared_ptr<PromiseData>::Type ptr = d.lock();
    
    if (ptr)
        return Promise(ptr).toString();
    else
        return String::tr("PromiseRef::null");
}
