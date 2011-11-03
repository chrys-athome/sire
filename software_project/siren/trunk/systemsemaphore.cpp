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

using namespace Siren;

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
void SystemSemaphore::acquire()
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
void SystemSemaphore::checkEndForAges()
{
}
