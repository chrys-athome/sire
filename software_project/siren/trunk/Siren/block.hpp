#ifndef SIREN_BLOCK_HPP
#define SIREN_BLOCK_HPP
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

#include "Siren/block.h"
#include "Siren/forages.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    /** Internal function called to tell for_ages that
        the current thread is about to sleep on this block */
    inline void Block::aboutToSleep() const
    {
        for_ages::aboutToSleep(this);
    }
    
    /** Internal function used to ask for_ages whether or not
        it is time for the current thread to awake from this block */
    inline bool Block::shouldWake() const
    {
        return for_ages::shouldWake(this);
    }
    
    /** Internal function used to tell for_ages that the current thread
        has woken up from this block (e.g. because of an exception, or a
        timeout) */
    inline void Block::hasWoken() const
    {
        for_ages::hasWoken(this);
    }
        
    /** Internal function used to tell for_ages that one of the threads
        sleeping on this block should be allowed to wake up */
    inline void Block::setShouldWakeOne() const
    {
        for_ages::setShouldWakeOne(this);
    }
    
    /** Internal function used to tell for_ages that all of the threads
        sleeping on this block should be allowed to wake up */
    inline void Block::setShouldWakeAll() const
    {
        for_ages::setShouldWakeAll(this);
    }

} // end of namespace Siren

SIREN_END_HEADER

#endif // ifndef SIREN_BLOCK_HPP
