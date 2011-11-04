#ifndef SIREN_READWRITELOCK_H
#define SIREN_READWRITELOCK_H
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
    namespace detail
    {
        class LockBreaker;
    
    } // end of namespace detail

    /** This is a ReadWriteLock. This is like a mutex,
        but allows multiple threads to hold a simultaneous
        reading lock, but only one thread to hold the write lock.
        It is heavily based on QReadWriteLock from Qt4 */
    class SIREN_EXPORT ReadWriteLock : public Block
    {
    public:
        ReadWriteLock();
        ~ReadWriteLock();
        
        void lockForRead();
        bool tryLockForRead();
        bool tryLockForRead(int timeout);

        void lockForWrite();
        bool tryLockForWrite();
        bool tryLockForWrite(int timeout);

        void unlock();
    
        String toString() const;
    
    protected:
        void checkEndForAges() const;
        
    private:
        void createBreaker();
    
        QReadWriteLock l;
    
        AtomicPointer<detail::LockBreaker>::Type breaker;
    
    }; // end of class ReadWriteLock
    
    /** This is a RAII locker that holds a read-lock on a ReadWriteLock.
        It is extremely heavily based on QReadLocker from Qt4 */
    class SIREN_EXPORT ReadLocker : public noncopyable
    {
    public:
        inline ReadLocker(ReadWriteLock *readWriteLock);

        inline ~ReadLocker()
        { unlock(); }

        inline void unlock()
        {
            if (q_lock) {
                if ((q_val & quintptr(1u)) == quintptr(1u)) {
                    q_val &= ~quintptr(1u);
                    q_lock->unlock();
                }
            }
        }

        inline void relock()
        {
            if (q_lock) {
                if ((q_val & quintptr(1u)) == quintptr(0u)) {
                    q_lock->lockForRead();
                    q_val |= quintptr(1u);
                }
            }
        }

        inline ReadWriteLock *readWriteLock() const
        { return reinterpret_cast<ReadWriteLock *>(q_val & ~quintptr(1u)); }

    private:
        union {
            ReadWriteLock *q_lock;
            quintptr q_val;
        };
    };
    
    inline ReadLocker::ReadLocker(ReadWriteLock *areadWriteLock)
        : noncopyable(), q_lock(areadWriteLock)
    {
        Q_ASSERT_X((q_val & quintptr(1u)) == quintptr(0),
                   "ReadLocker", "ReadWriteLock pointer is misaligned");
        relock();
    }

    /** This is a RAII locker that holds a write lock on a ReadWriteLock.
        This is extremely heavily based on QWriteLocker from Qt4 */
    class SIREN_EXPORT WriteLocker : public noncopyable
    {
    public:
        inline WriteLocker(ReadWriteLock *readWriteLock);

        inline ~WriteLocker()
        { unlock(); }

        inline void unlock()
        {
            if (q_lock) {
                if ((q_val & quintptr(1u)) == quintptr(1u)) {
                    q_val &= ~quintptr(1u);
                    q_lock->unlock();
                }
            }
        }

        inline void relock()
        {
            if (q_lock) {
                if ((q_val & quintptr(1u)) == quintptr(0u)) {
                    q_lock->lockForWrite();
                    q_val |= quintptr(1u);
                }
            }
        }

        inline ReadWriteLock *readWriteLock() const
        { return reinterpret_cast<ReadWriteLock *>(q_val & ~quintptr(1u)); }


    private:
        union{
            ReadWriteLock *q_lock;
            quintptr q_val;
        };
    };

    inline WriteLocker::WriteLocker(ReadWriteLock *areadWriteLock)
        : noncopyable(), q_lock(areadWriteLock)
    {
        Q_ASSERT_X((q_val & quintptr(1u)) == quintptr(0),
                   "WriteLocker", "ReadWriteLock pointer is misaligned");
        relock();
    }
    
} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::ReadWriteLock )
SIREN_EXPOSE_CLASS( Siren::ReadLocker )
SIREN_EXPOSE_CLASS( Siren::WriteLocker )

SIREN_END_HEADER

#endif // SIREN_READWRITELOCK_H
