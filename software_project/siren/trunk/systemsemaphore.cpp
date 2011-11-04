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

#include "Siren/systemsemaphore.h"
#include "Siren/forages.h"
#include "Siren/string.h"
#include "Siren/exceptions.h"
#include "Siren/workpacket.h"
#include "Siren/workspace.h"
#include "Siren/mutex.h"
#include "Siren/semaphore.h"
#include "Siren/obj.h"

using namespace Siren;

namespace Siren
{
    /** This is the WorkSpace used in the background to communicate between the 
        program and the background thread that is used to acquire the 
        system semaphore */
    class SysSemWorkSpace : public WorkSpace
    {
    public:
        SysSemWorkSpace();
        ~SysSemWorkSpace();
        
        void acquire(int n);
        void release(int n);
        
        Mutex& mutex();
        Semaphore& semaphore();
        WaitCondition& waiter();

        QSystemSemaphore& systemSemaphore();

        bool mustAcquire();
        bool mustRelease();

    private:
        Mutex m;
        Semaphore s;
        WaitCondition w;
        
        int n_to_acquire;
   
    }; // end of class SysSemWorkSpace

    /** Create a background thread to interface with the system semaphore.
        This is needed as the system semaphore can only be acquired one at a time,
        cannot by tryAcquired, and acquires are not interuptable! */
    class SysSemWorkPacket : public WorkPacket
    {
        SIREN_CLASS( SysSemWorkPacket, WorkPacket, 1 )

    public:
        SysSemWorkPacket();
        SysSemWorkPacket(const SysSemWorkPacket &other);
        
        ~SysSemWorkPacket();
        
        bool isFinished() const;
        int progress() const;
        
    protected:
        Obj runChunk() const;
        Obj runChunk(WorkSpace &workspace) const;
        
        void copy_object(const SysSemWorkPacket &other);
        bool compare_object(const SysSemWorkPacket &other) const;
        
    }; // end of class SysSemWorkPacket
        
} // end of namespace Siren
    
/////////
///////// Implementation of SysSemWorkSpace
/////////

void addResource()
{
    nresources += 1;
    semaphore.release();
}
    
/////////
///////// Implementation of SysSemWorkPacket
/////////
    
Obj SysSemWorkPacket::runChunk(WorkSpace &workspace) const
{
    SysSemWorkSpace &ws = workspace.asA<SysSemWorkSpace>();
    
    while (for_ages::loop())
    {
        MutexLocker lkr( &(ws.mutex()) );

        //do I need to acquire another SystemSemaphore resource?
        do
        {
            if (ws.mustAcquire())
            {
                do
                {
                    lkr.unlock();
                    ws.systemSemaphore().acquire();
                    lkr.relock();
                    ws.addResource();
                }
                while (ws.mustAcquire());
            }
            else if (ws.mustRelease())
            {
                do
                {
                    ws.semaphore().acquire();
                    ws.lostResource();
                    lkr.unlock();
                    ws.systemSemaphore().release();
                    lkr.relock();

                    //now need to release ws.sem, as this resource has now
                    //permanently gone
                }
                while (ws.mustRelease());
            }
        }
        while (ws.mustAcquire() or ws.mustRelease());

        //now sleep until something happens...
        ws.waiter().wait( &(ws.mutex()) );
    }
}
    
/////////
///////// Implementation of SystemSemaphore
/////////

/** Construct a SystemSemaphore with the specified key, initial value
    and access mode */
SystemSemaphore::SystemSemaphore(const String &key, int initialValue, AccessMode mode)
                : Block(), s(key, initialValue, QSystemSemaphore::AccessMode(mode))
{}

/** Destructor */
SystemSemaphore::~SystemSemaphore()
{}

/** Set the key for this semaphore */
void SystemSemaphore::setKey(const String &key, int initialValue, AccessMode mode)
{
    s.setKey(key, initialValue, QSystemSemaphore::AccessMode(mode));
}

/** Return the key for this semaphore */
String SystemSemaphore::key() const
{
    return s.key();
}

/** Acquire the semaphore.

    \throw Siren::system_error
*/
void SystemSemaphore::acquire(int n)
{
    if (not s.acquire())
    {
        throw system_error( String::tr("Unable to acquire %1: %2")
                .arg(this->toString(), s.errorString()), CODELOC );
    }
}

/** Release the semaphore */
void SystemSemaphore::release(int n)
{
    if (not s.release())
    {
        throw system_error( String::tr("Unable to release %1: %2")
                .arg(this->toString(), s.errorString()), CODELOC );
    }
}

/** Return a string representation of this error */
String SystemSemaphore::toString() const
{
    return String::tr("SystemSemaphore(%1)").arg(this);
}

/** Called by for_ages to check for the end of for_ages */
void SystemSemaphore::checkEndForAges() const
{
}
