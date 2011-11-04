#ifndef SIREN_FORAGES_H
#define SIREN_FORAGES_H
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
    class Block;

    class SIREN_EXPORT for_ages
    {
    public:
        static int registerThisThread();
        static void unregisterThisThread();
        
        static bool loop();
        static void test();
        
        static bool loop(int n);
        static void test(int n);
        
        static bool end();
        static bool end(int thread_id);
        static bool endAll();
        
        static bool play();
        static bool play(int thread_id);
        static bool playAll();
        
        static bool pause();
        static bool pause(int thread_id);
        static bool pauseAll();
        
        static void setThisThreadName(const String &thread_name);
        static String getThisThreadName();
        
        void msleep(int ms);
        void sleep(int secs);

    protected:
        friend class Block;
        static void aboutToSleep(const Block *block);
        static bool shouldWake(const Block *block);
        static void hasWoken(const Block *block);
        
        static void setShouldWakeAll(const Block *w);
        static void setShouldWakeOne(const Block *w);

    private:
        for_ages(){}
        ~for_ages(){}
        
    }; // end of class for_ages

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::for_ages )

SIREN_END_HEADER

#endif // ifndef SIREN_FORAGES_H
