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

#include "Siren/siren.hpp"
#include "Siren/object.hpp"
#include "Siren/workspace.hpp"
#include "Siren/systemsemaphore.h"
#include "Siren/forages.h"
#include "Siren/string.h"
#include "Siren/exceptions.h"
#include "Siren/workpacket.h"
#include "Siren/workspace.h"
#include "Siren/mutex.h"
#include "Siren/semaphore.h"
#include "Siren/waitcondition.h"
#include "Siren/obj.h"
#include "Siren/none.h"
#include "Siren/testreport.h"
#include "Siren/exceptions.h"
#include "Siren/assert.h"
#include "Siren/timer.h"

#include "Siren/detail/blockdata.h"

using namespace Siren;
using namespace Siren::detail;

namespace Siren
{
    class SysSemWorkPacket;

    /** This is the WorkSpace used in the background to communicate between the 
        program and the background thread that is used to acquire the 
        system semaphore */
    class SysSemWorkSpace : public WSpace
    {
    public:
        static WorkSpace create();
    
        ~SysSemWorkSpace();

        static const char* typeName() { return "Siren::SysSemWorkSpace"; }
        const char* what() const { return typeName(); }

        //////
        ////// Functions called by SystemSemaphore
        //////
        int nAcquired();
        int nAvailable();
        
        void acquire(int n);
        void release(int n);
        
        bool tryAcquire(int n);
        bool tryAcquire(int n, int ms);
        
        bool hasError() const;
        void checkError() const;
        
        //////
        ////// Functions called by SysSemWorkSpace
        //////
        
        void gotLock();
        void lostLock();
        
        Semaphore& semaphore();
        WaitCondition& waiter();

        void initialise(const SysSemWorkPacket &parent);

        bool mustAcquire() const;
        bool mustRelease() const;

        void setError(const Exception &e);

    private:
        SysSemWorkSpace();

        Semaphore *s;
        WaitCondition *w;

        String key;
        Obj error;
        
        int n_available;
        
        int n_acquired;
        int n_requested;
   
    }; // end of class SysSemWorkSpace

    /** Create a background thread to interface with the system semaphore.
        This is needed as the system semaphore can only be acquired one at a time,
        cannot by tryAcquired, and acquires are not interuptable! */
    class SysSemWorkPacket : public WorkPacket
    {
        SIREN_CLASS( SysSemWorkPacket, WorkPacket, 1 )

    public:
        SysSemWorkPacket();
        SysSemWorkPacket(const String &key, int initial_value);
        SysSemWorkPacket(const SysSemWorkPacket &other);
        
        ~SysSemWorkPacket();
        
        int nAvailable() const;
        String key() const;
        
        bool isFinished() const;
        int progress() const;
        
    protected:
        Obj runChunk() const;
        Obj runChunk(WorkSpace &workspace) const;
        
        void copy_object(const SysSemWorkPacket &other);
        bool compare_object(const SysSemWorkPacket &other) const;
        
        void test(TestReportEditor &report) const;

    private:
        /** Key used to find the system semaphore */
        String k;
        
        /** The initial value of the system semaphore */
        int initial_value;

    }; // end of class SysSemWorkPacket

    namespace detail
    {
        /** This class holds the actual data needed by SystemSemamphore */
        class SystemSemaphoreData : public BlockData
        {
        public:
            SystemSemaphoreData(const String &key, int initialValue, 
                                SystemSemaphore::AccessMode mode);
                                
            ~SystemSemaphoreData();
                  
            String toString() const;
            
            const char* blockType() const;
            
            void checkEndForAges() const;
            
            QSystemSemaphore s;
        };
    
    } // end of namespace detail
        
} // end of namespace Siren
    
/////////
///////// Implementation of SysSemWorkSpace
/////////

/** Constructor */
SysSemWorkSpace::SysSemWorkSpace() 
                : WSpace(), s(0), w(0), n_acquired(0), n_requested(0)
{}

/** Public construction must use the "create" function */
WorkSpace SysSemWorkSpace::create()
{
    return WorkSpace( new SysSemWorkSpace() );
}

/** Destructor */
SysSemWorkSpace::~SysSemWorkSpace()
{
    WriteLocker lkr(&(lock()));
    delete s;
    delete w;
}

/** Return the number of resources that have been acquired */
int SysSemWorkSpace::nAcquired()
{
    ReadLocker lkr(&(lock()));
    return n_acquired;
}

/** Return the number of resources that are available */
int SysSemWorkSpace::nAvailable()
{
    ReadLocker lkr(&(lock()));
    return n_available;
}

/** Return whether or not we are in an error state - note that the calling
    thread must hold a read or write lock */
bool SysSemWorkSpace::hasError() const
{
    return not error.isNone();
}

/** Function called by SysSemWorkSpace when it holds the write lock to set 
    the error state */
void SysSemWorkSpace::setError(const Exception &e)
{
    error = e;
}

/** Throw the error if we are in an error state 
        - note that the calling thread must hold a read or write lock */
void SysSemWorkSpace::checkError() const
{
    if (not error.isNone())
        error.asA<Exception>().throwSelf();
}

/** Tell the SysSemWorkPacket to acquire an addition 'n' reservations.
    This blocks until the extra 'n' reservations have been made, or
    until the end of for_ages has been signalled on this thread */
void SysSemWorkSpace::acquire(int n)
{
    if (n <= 0)
        n = 1;

    WriteLocker lkr(&(lock()));

    //raise an exception if this semaphore is in an error state
    this->checkError();

    if (n_requested + n > n_available)
        throw Siren::unavailable_resource( String::tr(
                "Cannot acquire an additional %1 resources for the SystemSemaphore "
                "with key \"%2\" because the number of locks we already hold "
                "on this semaphore is %3, and the number available is just %4.")
                    .arg(n).arg(key)
                    .arg(n_requested).arg(n_available), CODELOC );

    n_requested += n;

    try
    {
        w->wakeAll();
        lkr.unlock();
        s->acquire(n);
        lkr.relock();
        
        if (this->hasError())
        {
            s->release(n);
            this->checkError();
            n_requested -= n;
        }
    }
    catch(...)
    {
        n_requested -= n;
        throw;
    }
}

/** Try to acquire 'n' resources. This acquires the resources if they
    are immediately available (returns 'true'), or, if, not, then it returns 'false' */
bool SysSemWorkSpace::tryAcquire(int n)
{
    if (n <= 0)
        n = 1;

    WriteLocker lkr(&(lock()));

    //raise an exception if this semaphore is in an error state
    this->checkError();

    if (n_requested + n > n_available)
        //not possible as not enough resources!
        return false;

    n_requested += n;

    try
    {
        w->wakeAll();
        
        //we now sleep, so that we give a chance for the background
        //thread to actually acquire a lock!
        w->wait(&(lock()));
        
        if (s->tryAcquire(n))
        {
            if (this->hasError())
            {
                s->release(n);
                this->checkError();
                n_requested -= n;
                return false;
            }
            
            return true;
        }

        //could not get the lock in time
        n_requested -= n;
        w->wakeAll();
            
        return false;
    }
    catch(...)
    {
        n_requested -= n;
        throw;
    }
    
    return false;
}

/** Try to acquire 'n' resources, waiting for a maximum of 'ms' milliseconds.
    This will return 'true' if the resources have been acquired, and 'false' if not */
bool SysSemWorkSpace::tryAcquire(int n, int ms)
{
    if (ms <= 0)
        return this->tryAcquire(n);

    if (n <= 0)
        n = 1;

    //raise an exception if this semaphore is in an error state
    this->checkError();

    Timer t = Timer::start();

    WriteLocker lkr(&(lock()));

    if (n_requested + n > n_available)
        //not possible as not enough resources!
        return false;

    n_requested += n;

    try
    {
        w->wakeAll();
        
        //we now sleep, so that we give a chance for the background
        //thread to actually acquire a lock!
        w->wait(&(lock()));
        
        ms -= t.elapsed();
        
        if (ms <= 0)
        {
            if (s->tryAcquire(n))
            {
                if (this->hasError())
                {
                    s->release(n);
                    this->checkError();
                    n_requested -= n;
                    return false;
                }
            
                return true;
            }
        }
        else
        {
            lkr.unlock();
            bool success = s->tryAcquire(n,ms);
            lkr.relock();
            
            if (success)
            {
                if (this->hasError())
                {
                    s->release(n);
                    this->checkError();
                    n_requested -= n;
                    return false;
                }
                
                return true;
            }
        }

        //could not get the lock in time
        n_requested -= n;
        w->wakeAll();
            
        return false;
    }
    catch(...)
    {
        n_requested -= n;
        throw;
    }
    
    return false;
}

/** Tell the SysSemWorkPacket to release 'n' reservations */
void SysSemWorkSpace::release(int n)
{
    if (n <= 0)
        n = 1;
        
    WriteLocker lkr(&(lock()));
    
    //raise an exception if this semaphore is in an error state
    this->checkError();

    if (n_requested - n < 0)
        throw Siren::unavailable_resource( String::tr(
                "It is not possible to release more resources than have "
                "been requested! (%1 versus %2, %3 acquired)")
                    .arg(n).arg(n_requested).arg(n_acquired), CODELOC );
                    
    n_requested -= n;
    
    s->release(n);
    w->wakeAll();
}

/** Intialise this workspace to work with the passed SysSemWorkPacket */
void SysSemWorkSpace::initialise(const SysSemWorkPacket &parent)
{
    WriteLocker lkr(&(lock()));
    
    if (s != 0)
        throw Siren::program_bug( String::tr(
                "It is a mistake for a SysSemWorkSpace to be initialised twice!"),
                    CODELOC );

    n_acquired = 0;
    n_requested = 0;
    n_available = parent.nAvailable();
    
    if (n_available == 0)
        //this is an empty semaphore!
        return;

    //create a new semaphore that holds a local copy of the 
    //number of available resources - acquire all of these
    //resources as they are not yet acquired locally
    s = new Semaphore(n_available);
    s->acquire(n_available);
    
    w = new WaitCondition();
}

/** Signal that a global resource has been acquired - this releases
    a spot on the local semaphore thus allowing the resource to be
    made available to the local process. Note that this must be 
    called only by the thread running the workpacket, while the workpacket
    holds a lock on the mutex */
void SysSemWorkSpace::gotLock()
{
    SIREN_ASSERT( s != 0 );

    n_acquired += 1;
    s->release();
}

/** Signal that a global resource has been returned to the pool.
    This must only be called by the thread running the workpacket,
    and only while the workpacket holds a lock on the mutex */
void SysSemWorkSpace::lostLock()
{
    SIREN_ASSERT( s != 0 );
    
    n_acquired -= 1;
    s->acquire();
}

/** Return the semaphore that holds a local copy of the resources */
Semaphore& SysSemWorkSpace::semaphore()
{
    if (s == 0)
        throw Siren::program_bug( String::tr(
                "You cannot get the semaphore of a workspace that has not "
                "yet been initialised."), CODELOC );
                
    return *s;
}

/** Return the waiter that is used to tell the background thread
    to wake up and process a lock request */
WaitCondition& SysSemWorkSpace::waiter()
{
    if (w == 0)
        throw Siren::program_bug( String::tr(
                "You cannot get the WaitCondition of a workspace that has not "
                "yet been initialised."), CODELOC );
                
    return *w;
}

/** Return whether or not a global resource needs to be acquired. This
    can only be called by the thread running the workpacket, and only
    while the workpacket holds a lock of the mutex */
bool SysSemWorkSpace::mustAcquire() const
{
    return n_acquired < n_requested;
}

/** Return whether or not a global resource needs to be released. This
    can only be called by the thread running the workpacket, and only
    while the workpacket holds a lock of the mutex */
bool SysSemWorkSpace::mustRelease() const
{
    return n_acquired > n_requested;
}

/////////
///////// Implementation of SysSemWorkPacket
/////////
    
REGISTER_SIREN_CLASS( Siren::SysSemWorkPacket )

/** Null constructor */
SysSemWorkPacket::SysSemWorkPacket() 
                 : WorkPacket(), initial_value(0)
{}

/** Construct to create and manage a SystemSemaphore backend with
    key 'key', with 'initial_value' resources available. Note that this
    can only set the number of resources available if the system semaphore
    has not already been created */
SysSemWorkPacket::SysSemWorkPacket(const String &key, int val)
                 : WorkPacket(), k(key), initial_value(val)
{}

/** Copy constructor */
SysSemWorkPacket::SysSemWorkPacket(const SysSemWorkPacket &other)
                 : WorkPacket(other),
                   k(other.k), initial_value(other.initial_value)
{}

/** Destructor */
SysSemWorkPacket::~SysSemWorkPacket()
{}

/** Copy assignment operator */
void SysSemWorkPacket::copy_object(const SysSemWorkPacket &other)
{
    k = other.k;
    initial_value = other.initial_value;
    super::copy_object(other);
}

/** Comparison operator */
bool SysSemWorkPacket::compare_object(const SysSemWorkPacket &other) const
{
    return k == other.k and initial_value == other.initial_value and
           super::compare_object(other);
}

/** Return the key used to identify the system semaphore */
String SysSemWorkPacket::key() const
{
    return k;
}

/** Return the number of resources that should be available on this semaphore */
int SysSemWorkPacket::nAvailable() const
{
    return initial_value;
}

/** Return whether or not the workpacket has finished */
bool SysSemWorkPacket::isFinished() const
{
    return false;
}

/** Return the progress made in running the packet */
int SysSemWorkPacket::progress() const
{
    return 0;
}
 
/** Cannot run this WorkPacket without an accompanying WorkSpace */
Obj SysSemWorkPacket::runChunk() const
{
    throw Siren::invalid_arg( String::tr(
                "Cannot process a SysSemWorkPacket without an accompanying "
                "WorkSpace (SysSemWorkSpace)."), CODELOC );
                
    return None();
}
 
/** Actually run the work in the WorkPacket */
Obj SysSemWorkPacket::runChunk(WorkSpace &workspace) const
{
    //no need to run anything that manages a null semaphore!
    if (initial_value == 0)
        return None();

    SysSemWorkSpace &ws = workspace.asA<SysSemWorkSpace>();
    WriteLocker lkr( &(ws.lock()) );

    ws.initialise(*this);

    int nheld = 0;
    QSystemSemaphore *sys_sem = new QSystemSemaphore(k, initial_value);
    
    if (sys_sem->error() != QSystemSemaphore::NoError)
    {
        ws.setError( system_error( String::tr(
                "Could not create a system semaphore with key \"%1\". %2.")
                    .arg(k, sys_sem->errorString()), CODELOC ) );

        delete sys_sem;
        
        return None();
    }
    
    try
    {
        while (for_ages::loop())
        {
            //do I need to acquire another SystemSemaphore resource?
            do
            {
                if (ws.mustAcquire())
                {
                    do
                    {
                        ws.waiter().wakeAll();
                        lkr.unlock();
                        bool success = sys_sem->acquire();
                        lkr.relock();
                        
                        if (not success)
                        {
                            ws.setError( system_error( String::tr(
                                    "There was an error acquiring the system semaphore "
                                    "with key \"%1\": %2.")
                                        .arg(k, sys_sem->errorString()), CODELOC ) );
                                        
                            delete sys_sem;
                            return None();
                        }
                        
                        nheld += 1;
                        ws.gotLock();
                    }
                    while (ws.mustAcquire());
                }
                else if (ws.mustRelease())
                {
                    do
                    {
                        ws.waiter().wakeAll();
                        ws.lostLock();
                        lkr.unlock();
                        bool success = sys_sem->release();
                        lkr.relock();
                        
                        if (not success)
                        {
                            ws.setError( system_error( String::tr(
                                    "There was an error releasing the system semaphore "
                                    "with key \"%1\": %2.")
                                        .arg(k, sys_sem->errorString()), CODELOC ) );

                            delete sys_sem;
                            return None();
                        }
                        
                        nheld -= 1;
                    }
                    while (ws.mustRelease());
                }
            }
            while (ws.mustAcquire() or ws.mustRelease());

            //now sleep until something happens (waking up anyone who has 
            //been waiting for us to finish communicating with the system semaphore)
            ws.waiter().wakeAll();
            ws.waiter().wait( &(ws.lock()) );
        }
    }
    catch(...)
    {
        ws.waiter().wakeAll();
    
        //release all holds of the System semaphore
        while (nheld > 0)
        {
            sys_sem->release();
            nheld -= 1;
        }
        
        //must delete the system semaphore so that it is properly cleaned
        delete sys_sem;

        throw;
    }
    
    ws.waiter().wakeAll();
    
    if (nheld > 0)
    {
        sirenDebug() << "WEIRD!!! Exiting with non-zero nheld!" << nheld
                     << "IN" << __FILE__ << __LINE__;
    
        while (nheld > 0)
        {
            sys_sem->release();
            nheld -= 1;
        }
    }
    
    delete sys_sem;
    
    return None();
}

/** Test this WorkPacket */        
void SysSemWorkPacket::test(TestReportEditor &report) const
{
    //report.addFailure( String::tr("NEED TO WRITE TESTS!!!") );
}

/////////
///////// Implementation of SystemSemaphoreData
/////////
        
SystemSemaphoreData::SystemSemaphoreData(const String &key, int initialValue, 
                                         SystemSemaphore::AccessMode mode)
                    : BlockData(), 
                      s(key, initialValue, QSystemSemaphore::AccessMode(mode))
{}

SystemSemaphoreData::~SystemSemaphoreData()
{}

const char* SystemSemaphoreData::blockType() const
{
    return SystemSemaphore::typeName();
}

/** Return a string representation of this error */
String SystemSemaphoreData::toString() const
{
    return String::tr("SystemSemaphore(%1)").arg(this);
}

/** Called by for_ages to check for the end of for_ages */
void SystemSemaphoreData::checkEndForAges() const
{
}

/////////
///////// Implementation of SystemSemaphore
/////////

/** Construct a SystemSemaphore with the specified key, initial value
    and access mode */
SystemSemaphore::SystemSemaphore(const String &key, int initialValue, AccessMode mode)
                : Block()
{
    d = new SystemSemaphoreData(key, initialValue, mode);
    Block::setData(d);
}

/** Internal constructor used to construct a SystemSemaphore from a BlockRef */
SystemSemaphore::SystemSemaphore(const exp_shared_ptr<detail::BlockData>::Type &ptr)
                : Block(ptr), d(0)
{
    if (ptr)
    {
        if ( std::strcmp(ptr->blockType(), SystemSemaphore::typeName()) == 0 )
        {
            d = static_cast<SystemSemaphoreData*>(ptr.get());
        }
        else
            throw Siren::invalid_cast( String::tr(
                    "Cannot cast a Block of type %1 to a Siren::SystemSemaphore.")
                        .arg(ptr->blockType()), CODELOC );
    }
}

/** Copy constructor */
SystemSemaphore::SystemSemaphore(const SystemSemaphore &other)
                : Block(other), d(other.d)
{}

/** Destructor */
SystemSemaphore::~SystemSemaphore()
{}

/** Internal function used by BlockRef to check if the passed data is a Mutex */
bool SystemSemaphore::isOfType(const exp_shared_ptr<BlockData>::Type &ptr)
{
    if (ptr.get() == 0)
        return false;
    else
        return std::strcmp(ptr->blockType(), SystemSemaphore::typeName()) == 0;
}

/** Copy assignment operator */
SystemSemaphore& SystemSemaphore::operator=(const SystemSemaphore &other)
{
    if (this != &other)
    {
        Block::operator=(other);
        d = other.d;
    }
    
    return *this;
}

/** Set the key for this semaphore */
void SystemSemaphore::setKey(const String &key, int initialValue, AccessMode mode)
{
    d->s.setKey(key, initialValue, QSystemSemaphore::AccessMode(mode));
}

/** Return the key for this semaphore */
String SystemSemaphore::key() const
{
    return d->s.key();
}

/** Acquire the semaphore.

    \throw Siren::system_error
*/
void SystemSemaphore::acquire(int n)
{
    if (not d->s.acquire())
    {
        throw system_error( String::tr("Unable to acquire %1: %2")
                .arg(this->toString(), d->s.errorString()), CODELOC );
    }
}

/** Release the semaphore */
void SystemSemaphore::release(int n)
{
    if (not d->s.release())
    {
        throw system_error( String::tr("Unable to release %1: %2")
                .arg(this->toString(), d->s.errorString()), CODELOC );
    }
}
