/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#include "semaphore.h"
#include "forages.h"

using namespace Siren;

Semaphore::Semaphore(int n) : s(n)
{}

Semaphore::~Semaphore()
{}

const int chunk = 5000;

void Semaphore::acquire(int n)
{
    if (s.tryAcquire(n))
        return;

    while (for_ages())
    {
        if (s.tryAcquire(n, chunk))
            break;
    }
}

int Semaphore::available() const
{
    return s.available();
}

void Semaphore::release(int n)
{
    s.release(n);
}

bool Semaphore::tryAcquire(int n)
{
    return s.tryAcquire(n);
}

bool Semaphore::tryAcquire(int n, int ms)
{
    if (s.tryAcquire(n))
        return true;
        
    while (for_ages())
    {
        int wait = qMin(ms, chunk);
        
        if (s.tryAcquire(n, wait))
            return true;
            
        ms -= chunk;
        
        if (ms <= 0)
            return false;
    }
    
    return false;
}
