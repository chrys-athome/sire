#ifndef SIREN_MUTEX_H
#define SIREN_MUTEX_H
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
#include "Siren/block.h"

SIREN_BEGIN_HEADER

namespace Siren
{

    class WaitCondition;

    /** This class provides a Mutex which respects the end
        of for_ages (i.e. .lock() and .tryLock() can be interupted
        by signalling the end of for_ages, in which case they will
        throw a Siren::interupted exception. In all other
        respects, this is identical to a QMutex
        
        @author Christopher Woods
    */
    class SIREN_EXPORT Mutex : public Block
    {
    public:
        enum RecursionMode 
        { 
            Recursive = QMutex::Recursive, 
            NonRecursive = QMutex::NonRecursive
        };
    
        Mutex( Mutex::RecursionMode mode = Mutex::NonRecursive );
        ~Mutex();
        
        void lock();
        bool tryLock();
        bool tryLock(int ms);
        
        void unlock();
        
        String toString() const;
        
    protected:
        void checkEndForAges() const;
        
    private:
        friend class WaitCondition;

        /** The actual QMutex */
        QMutex m;

    }; // end of class Mutex

    /** This is the equivalent for QMutexLocker for a Mutex.
        This is blatently copied from Qt's qmutex.h,
        which is (C) Nokia, 2010, placed under the LGPL.
        
        See
        http://www.qt.gitorious.org/qt/qt/blobs/master/src/corelib/thread/qmutex.h
    */
    class SIREN_EXPORT MutexLocker
    {
    public:
        inline explicit MutexLocker(Mutex *mutex) : mtx(mutex)
        {
            Q_ASSERT_X((val & quintptr(1u)) == quintptr(0),
                       "MutexLocker", "Mutex pointer is misaligned");
            relock();
        }
        
        inline ~MutexLocker()
        {
            unlock();
        }
        
        inline void unlock()
        {
            if (mtx) 
            {
                if ((val & quintptr(1u)) == quintptr(1u)) 
                {
                    val &= ~quintptr(1u);
                    mtx->unlock();
                }
            }
        }

        inline void relock()
        {
            if (mtx) 
            {
                if ((val & quintptr(1u)) == quintptr(0u)) 
                {
                    mtx->lock();
                    val |= quintptr(1u);
                }
            }
        }

    #if defined(Q_CC_MSVC)
    #pragma warning( push )
    #pragma warning( disable : 4312 ) // ignoring the warning from /Wp64
    #endif

        inline QMutex *mutex() const
        {
            return reinterpret_cast<QMutex *>(val & ~quintptr(1u));
        }

    #if defined(Q_CC_MSVC)
    #pragma warning( pop )
    #endif

    private:
        Q_DISABLE_COPY(MutexLocker)

        union 
        {
            Mutex *mtx;
            quintptr val;
        };
    
    }; // end of class MutexLocker

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::Mutex )
SIREN_EXPOSE_CLASS( Siren::MutexLocker )

SIREN_END_HEADER

#endif // ifndef SIREN_MUTEX_H
