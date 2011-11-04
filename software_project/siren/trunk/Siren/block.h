#ifndef SIREN_BLOCK_H
#define SIREN_BLOCK_H
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

#include "Siren/siren.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    class for_ages;

    /** This is the virtual base class of all Blocking types. 
        A Block object is used to stop or block a thread, e.g.
        a Mutex, WaitCondition, Semaphore or ReadWriteLock
        
        @author Christopher Woods
    */
    class SIREN_EXPORT Block : public noncopyable
    {
    public:
        Block();
        virtual ~Block();
        
        virtual String toString() const=0;
        
    protected:
        friend class for_ages;
        virtual void checkEndForAges() const=0;
    
        void aboutToSleep() const;
        bool shouldWake() const;
        void hasWoken() const;
        
        void setShouldWakeOne() const;
        void setShouldWakeAll() const;
    
    }; // end of class Block

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::Block )

SIREN_END_HEADER

#endif // ifndef SIREN_BLOCK_H
