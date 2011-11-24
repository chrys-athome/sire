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
#include "Siren/string.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    class for_ages;
    class BlockRef;

    namespace detail
    { 
        class ProgramState; 
        class BlockData;
    }

    /** This is the virtual base class of all Blocking types. 
        A Block object is used to stop or block a thread, e.g.
        a Mutex, WaitCondition, Semaphore or ReadWriteLock
        
        @author Christopher Woods
    */
    class SIREN_EXPORT Block
    {
    public:
        Block();
        Block(const Block &other);
        ~Block();
        
        const char* what() const;
        
        String toString() const;
        
        bool operator==(const Block &other) const;
        bool operator!=(const Block &other) const;
        
        bool operator==(const BlockRef &other) const;
        bool operator!=(const BlockRef &other) const;
        
    protected:
        Block(const exp_shared_ptr<detail::BlockData>::Type &ptr);

        void setData(detail::BlockData *ptr);

        Block& operator=(const Block &other);
    
        void aboutToSleep() const;
        bool shouldWake() const;
        void hasWoken() const;
        
        void setShouldWakeOne() const;
        void setShouldWakeAll() const;
    
    private:
        friend class BlockRef;
        exp_shared_ptr<detail::BlockData>::Type d;

    }; // end of class Block

    /** This class holds a weak reference to a Block. This holds
        the reference for as long as the block is alive, and is then
        automatically set to null when the block is deleted. */
    class SIREN_EXPORT BlockRef
    {
    public:
        BlockRef();
        BlockRef(const Block &block);
        BlockRef(const BlockRef &other);
        
        ~BlockRef();
        
        BlockRef& operator=(const BlockRef &other);
        
        bool operator==(const Block &block) const;
        bool operator!=(const Block &block) const;
        
        bool operator==(const BlockRef &other) const;
        bool operator!=(const BlockRef &other) const;
        
        bool isNull() const;
        
        String toString() const;
        
        const char* what() const;
        
        template<class T>
        bool isA() const;
        
        template<class T>
        T asA() const;
        
    protected:
        friend class for_ages;
        friend class Siren::detail::ProgramState;

        void checkEndForAges() const;
        void throwNullRefError(const char *type_name) const;

    private:
        exp_weak_ptr<detail::BlockData>::Type d;

    }; // end of class BlockRef

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::Block )
SIREN_EXPOSE_CLASS( Siren::BlockRef )

SIREN_END_HEADER

#endif // ifndef SIREN_BLOCK_H
