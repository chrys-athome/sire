#ifndef SIREN_SEMAPHORE_H
#define SIREN_SEMAPHORE_H
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
        class SemBreaker;
    
    } // end of namespace detail

    /** This is a semaphore - this provides a single counter
        that can be used to reserve resources etc. It is heavily
        based on QSemaphore */
    class SIREN_EXPORT Semaphore : public Block
    {
    public:
        explicit Semaphore(int n = 0);
        ~Semaphore();

        void acquire(int n = 1);
        bool tryAcquire(int n = 1);
        bool tryAcquire(int n, int timeout);

        void release(int n = 1);

        int available() const;

        String toString() const;
        
    protected:
        void checkEndForAges() const;
        
    private:
        void createBreaker();
    
        QSemaphore s;
    
        AtomicPointer<detail::SemBreaker>::Type breaker;
    
    }; // end of class Semaphore

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::Semaphore )

SIREN_END_HEADER

#endif // ifndef SIREN_SEMAPHORE_H
