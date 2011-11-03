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

#include "Siren/mutex.h"
#include "Siren/forages.h"

using namespace Siren;

/** Construct a mutex with the specified recursion mode */
Mutex::Mutex(Mutex::RecursionMode mode) : m( QMutex::RecursionMode(mode) )
{}

/** Destructor - please make sure you have unlocked the mutex
    before destroying it! */
Mutex::~Mutex()
{}

#if QT_VERSION < 0x403000
    static bool tryLock(QMutex &mutex, int ms)
    {
        while (not mutex.tryLock())
        {
            int wait = qMin(ms, 10);
            Siren::msleep(wait);

            ms -= wait;
            if (ms <= 0)
                return mutex.tryLock();
        }

        return true;
    }
#endif

/** Lock the mutex. This will block until the mutex is locked,
    or the end of for_ages is signalled on this thread */
void Mutex::lock()
{
    if (not m.tryLock())
    {
        while (for_ages())
        {
            #if QT_VERSION >= 0x403000
                if (m.tryLock(5000))
                    return;
            #else
                if (::tryLock(m, 5000))
                    return;
            #endif
        }
    }
}

/** Try to lock this mutex - this will return whether 
    or not locking was successful */
bool Mutex::tryLock()
{
    return m.tryLock();
}

/** Try to lock this mutex, for up to 'ms' milliseconds.
    This returns whether or not locking was successful */
bool Mutex::tryLock(int ms)
{
    const int block = 5000;

    #if QT_VERSION >= 0x403000
        if (ms < block)
            return m.tryLock(ms);
    
        while (for_ages())
        {
            int wait = qMin(ms, block);
        
            if (m.tryLock(wait))
                return true;
            
            ms -= wait;
        
            if (ms <= 0)
                return false;
        }
    #else
       if (ms < block)
           return ::tryLock(m, ms);

       while (for_ages())
       {
           int wait = qMin(ms, block);
            
           if (::tryLock(m, wait))
               return true;

           ms -= wait;

           if (ms <= 0)
               return false;
       }  
    #endif

    return false;
}

/** Unlock the mutex. You can only unlock the mutex in the 
    same thread in which it was locked */
void Mutex::unlock()
{
    m.unlock();
}
