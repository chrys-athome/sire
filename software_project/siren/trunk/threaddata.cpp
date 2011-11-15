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

#include "Siren/detail/threaddata.h"

using namespace Siren;
using namespace Siren::detail;

/** Internal constructor */
ThreadPoolData::ThreadPoolData() 
               : noncopyable(),
                 n_desired(0), n_allocated(0), n_foreground(0), n_background(0)
{}

static exp_shared_ptr<ThreadPoolData>::Type global_pool;
static AtomicPointer<ThreadPoolData>::Type ptr;

/** Return the global system pool of threads */
exp_shared_ptr<ThreadPoolData>::Type ThreadPoolData::systemCPUs()
{
    while (global_pool.get() == 0)
    {
        exp_shared_ptr<ThreadPoolData>::Type p( new ThreadPoolData() );
        
        if (ptr.testAndSetAcquire(p.get(), 0))
        {
            global_pool = p;
        }
    }
    
    return global_pool;
}

/** Create a ThreadPool that manages 'n' threads */
exp_shared_ptr<ThreadPoolData>::Type ThreadPoolData::createCPU(int n)
{
    if (n <= 0)
        n = 1;

    exp_shared_ptr<ThreadPoolData>::Type pool( new ThreadPoolData() );
    
    pool->n_desired = n;
    exp_shared_ptr<ThreadPoolData>::Type sys_pool = systemCPUs();
    pool->self = pool;
    pool->prnt = sys_pool;
    sys_pool->addChild(pool);
    
    return pool;
}

/** Create a ThreadPool that manages 'n' threads that are taken from the passed parent */
exp_shared_ptr<ThreadPoolData>::Type ThreadPoolData::createCPU(
                const exp_shared_ptr<ThreadPoolData>::Type &parent, int n)
{
    if (n <= 0)
        n = 1;
        
    exp_shared_ptr<ThreadPoolData>::Type pool( new ThreadPoolData() );
    
    pool->n_desired = n;
    pool->self = pool;
    pool->prnt = parent;
    parent->addChild(pool);
    
    return pool;
}

/** Destructor */
ThreadPoolData::~ThreadPoolData()
{
    //if we hold any foreground threads, return them to the parent,
    //or if we don't have a parent, return them to the system
    if (n_foreground > 0)
    {
        exp_shared_ptr<ThreadPoolData>::Type parent = prnt.lock();
        
        if (parent.get() == 0)
            parent = systemCPUs();
        
        //give any available foreground threads back to the parent
        parent->returnThreads(n_foreground);
        
        //also give all of the children to their grandparent ;-)
        //(we don't want any orphans!)
        for (List<exp_weak_ptr<ThreadPoolData>::Type>::const_iterator
                                            it = chldrn.constBegin();
             it != chldrn.constEnd();
             ++it)
        {
            exp_shared_ptr<ThreadPoolData>::Type child = it->lock();
            
            if (child.get() != 0)
                child->reparent(parent);
        }
    }
}

/** Return the number of threads that this ThreadPool desires */
int ThreadPoolData::nDesired()
{
    MutexLocker lkr(&m);
    return n_desired;
}

/** Return the number of foreground threads owned by this pool */
int ThreadPoolData::nForeGround()
{
    MutexLocker lkr(&m);
    return n_foreground;
}

/** Return the number of background threads owned by this pool */
int ThreadPoolData::nBackGround()
{
    MutexLocker lkr(&m);
    return n_background;
}

/** Return a child pool that manages 'n' threads taken from this parent */
exp_shared_ptr<ThreadPoolData>::Type ThreadPoolData::getChild(int n)
{
    return ThreadPoolData::createCPU(self.lock(), n);
}

/** Run the passed WorkPacket using 'n' threads from this pool, returning
    the result in the passed promise */
Promise ThreadPoolData::run(const WorkPacket &packet, int n)
{
    exp_shared_ptr<ThreadPoolData>::Type worker = this->getChild(n);
    
    worker->setPacket(packet);
    worker->run();
    
    return Promise(worker);
}

Promise ThreadPoolData::run(const WorkPacket &packet, 
                            WorkSpace &workspace, int n=1);
