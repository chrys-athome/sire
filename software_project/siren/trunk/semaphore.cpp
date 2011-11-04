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

#include "Siren/semaphore.h"
#include "Siren/forages.h"
#include "Siren/string.h"

using namespace Siren;

/** Construct a semaphore with 'n' reservations available */
Semaphore::Semaphore(int n) : Block(), s(n)
{}

/** Destructor */
Semaphore::~Semaphore()
{}

/** Acquire 'n' reservations - block until this is available */
void Semaphore::acquire(int n)
{
    s.acquire(n);
}

/** Try to acquire 'n' reservations - block until this is available */
bool Semaphore::tryAcquire(int n)
{
    return s.tryAcquire(n);
}

/** Try to acquire 'n' reservations for up to 'ms' milliseconds.
    Retruns whether or not it was successful */
bool Semaphore::tryAcquire(int n, int ms)
{
    return s.tryAcquire(n,ms);
}

/** Release 'n' reservations - this will create extra reservations
    on the semaphore if this increases the limit beyond the original
    value */
void Semaphore::release(int n)
{
    s.release(n);
}

/** Return the number of available reservations */
int Semaphore::available() const
{
    return s.available();
}

/** Return a string representation of the semaphore */
String Semaphore::toString() const
{
    return String::tr("Semaphore(%1)").arg(this);
}

/** Check for the end of for_ages */
void Semaphore::checkEndForAges() const
{
}
