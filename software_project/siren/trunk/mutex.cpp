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
#include "Siren/string.h"

using namespace Siren;

/** Construct a mutex with the specified recursion mode */
Mutex::Mutex(Mutex::RecursionMode mode) : m( QMutex::RecursionMode(mode) )
{}

/** Destructor - please make sure you have unlocked the mutex
    before destroying it! */
Mutex::~Mutex()
{}

/** Return a string representation of this Mutex */
String Mutex::toString() const
{
    return String::tr("Mutex(%1)").arg(this);
}

/** Lock the mutex. This will block until the mutex is locked,
    or the end of for_ages is signalled on this thread
    
    \throw Siren::interupted_thread
*/
void Mutex::lock()
{
    m.lock();
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
    return m.tryLock();
}

/** Unlock the mutex. You can only unlock the mutex in the 
    same thread in which it was locked */
void Mutex::unlock()
{
    m.unlock();
}

/** Called by for_ages to tell the mutex to check for the
    end of for_ages in each blocked thread */
void Mutex::checkEndForAges() const
{
}
