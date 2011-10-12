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

#include "Siren/timer.h"
#include "Siren/siren.hpp"

REGISTER_SIREN_CLASS( Siren::Timer )

using namespace Siren;

/** Construct a timer that starts timing immediately */
Timer::Timer() : Object()
{
    d.start();
}

/** Copy constructor */
Timer::Timer(const Timer &other) : Object(other), d(other.d)
{}

/** Destructor */
Timer::~Timer()
{}

/** Return a timer that starts timing immediately */
Timer Timer::start()
{
    return Timer();
}

/** Return the number of milliseconds that have elasped
    since this timer was started */
int Timer::elapsed() const
{
    return d.elapsed();
}

/** Copy assignment operator */
void Timer::copy_object(const Timer &other)
{
    d = other.d;
    super::copy_object(other);
}

/** Comparison operator */
bool Timer::compare_object(const Timer &other) const
{
    return d == other.d and super::compare_object(other);
}
