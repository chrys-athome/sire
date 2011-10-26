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
    struct for_ages
    {
        static void registerThisThread();
        static void unregisterThisThread();
    };

    int register_this_thread();
    void unregister_this_thread();

    void msleep(int ms);
    void sleep(int secs);

    bool for_ages();

    void check_for_ages();

    void pause_for_ages();
    void pause_for_ages(const Thread *thread);

    void play_for_ages();
    void play_for_ages(const Thread *thread);

    void end_for_ages();
    void end_for_ages(const Thread *thread);

} // end of namespace Siren

SIREN_EXPOSE_FUNCTION( Siren::msleep )
SIREN_EXPOSE_FUNCTION( Siren::sleep )
SIREN_EXPOSE_FUNCTION( Siren::for_ages )
SIREN_EXPOSE_FUNCTION( Siren::check_for_ages )
SIREN_EXPOSE_FUNCTION( Siren::pause_for_ages )
SIREN_EXPOSE_FUNCTION( Siren::play_for_ages )
SIREN_EXPOSE_FUNCTION( Siren::end_for_ages )

SIREN_END_HEADER

#endif // ifndef SIREN_FORAGES_H
