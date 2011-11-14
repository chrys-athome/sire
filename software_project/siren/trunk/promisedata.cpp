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

#include "Siren/detail/promisedata.h"
#include "Siren/exceptions.h"

using namespace Siren;
using namespace Siren::detail;

/** Null constructor */
PromiseData::PromiseData(const exp_shared_ptr<ThreadPoolData>::Type &t) 
            : noncopyable(), thread(t), ready(false)
{}

/** Destructor */
PromiseData::~PromiseData()
{
    //tell any thread processing the work that the calculation
    //should be aborted as no-one is listening the for result
    exp_shared_ptr<ThreadPoolData>::Type t = thread.lock();
    t->abort(this);
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
