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

#include "Siren/readwritelock.h"
#include "Siren/string.h"
#include "Siren/forages.h"

using namespace Siren;

/** Constructor */
ReadWriteLock::ReadWriteLock() : Block()
{}

/** Destructor */
ReadWriteLock::~ReadWriteLock()
{}

/** Lock for reading */
void ReadWriteLock::lockForRead()
{
    l.lockForRead();
}

/** Try to lock for reading - return whether we succeed */
bool ReadWriteLock::tryLockForRead()
{
    return l.tryLockForRead();
}

/** Try to lock for reading for up to 'ms' milliseconds.
    This returns whether or not we succeeded */
bool ReadWriteLock::tryLockForRead(int ms)
{
    return l.tryLockForRead(ms);
}

/** Lock fro writing */
void ReadWriteLock::lockForWrite()
{
    l.lockForWrite();
}

/** Try to lock for writing - return whether or not we succeed */
bool ReadWriteLock::tryLockForWrite()
{
    return l.tryLockForWrite();
}

/** Try to lock for writing for up to "ms" milliseconds */
bool ReadWriteLock::tryLockForWrite(int ms)
{
    return l.tryLockForWrite(ms);
}

/** Unlock the lock */
void ReadWriteLock::unlock()
{
    l.unlock();
}

/** Return a string representation of this lock */
String ReadWriteLock::toString() const
{
    return String::tr("ReadWriteLock(%1)").arg(this);
}

/** Called by for_ages to asked this lock to check for the end of for_ages */
void ReadWriteLock::checkEndForAges()
{
}
