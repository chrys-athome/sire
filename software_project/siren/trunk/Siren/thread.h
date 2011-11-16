#ifndef SIREN_THREAD_H
#define SIREN_THREAD_H

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
    namespace detail{ class ThreadData; }

    /** This internal class holds a pool of threads that can
        run specified functions */
    class Thread
    {
    public:
        Thread();
        Thread(const Thread &other);
        ~Thread();
        
        Thread& operator=(const Thread &other);
        
        bool operator==(const Thread &other) const;
        bool operator!=(const Thread &other) const;

        static Thread run( void (*function)() );

        bool isNull();

        void abort();
        void pause();
        void play();

        bool isError();
        void throwError();
        
        void checkError();

    private:
        
        exp_shared_ptr<detail::ThreadData>::Type d;
        int session_id;

    }; // end of class Thread


} // end of namespace Siren

SIREN_END_HEADER

#endif // ifndef SIREN_THREAD
