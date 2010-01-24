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

#include "mutex.h"
#include "forages.h"

using namespace Siren;

Mutex::Mutex( QMutex::RecursionMode mode )
      : m(mode)
{}

Mutex::~Mutex()
{}

void Mutex::lock()
{
    if (not m.tryLock())
    {
        while (for_ages())
        {
            if (m.tryLock(5000))
                return;
        }
    }
}

bool Mutex::tryLock()
{
    return m.tryLock();
}

bool Mutex::tryLock(int ms)
{
    const int block = 5000;
    
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
    
    return false;
}

void Mutex::unlock()
{
    m.unlock();
}
