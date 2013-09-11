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

#include "Siren/time.h"
#include "Siren/siren.hpp"

REGISTER_SIREN_CLASS( Siren::Time )

using namespace Siren;

/** Construct a null time */
Time::Time() : Object()
{}

/** Construct the time from the specified number of hours, minutes
    and seconds (and milliseconds too, if you want to be that accurate) */
Time::Time(int h, int m, int s, int ms) : Object(), d(h,m,s,ms)
{
    if (not d.isValid())
        d = QTime();
}

#ifdef SIREN_QT_SUPPORT
    Time::Time(const QTime &time) : Object(), d(time)
    {
        if (not d.isValid())
            d = QTime();
    }
    
    Time::operator QTime() const
    {
        return d;
    }
#endif

/** Copy constructor */
Time::Time(const Time &other) : Object(other), d(other.d)
{}

/** Destructor */
Time::~Time()
{}

/** Return whether or not this time is null */
bool Time::isNull() const
{
    return d.isNull();
}

/** Return the hour */
int Time::hour() const
{
    return d.hour();
}

/** Return the minute */
int Time::minute() const
{
    return d.minute();
}

/** Return the second */
int Time::second() const
{
    return d.second();
}

/** Return the number of milliseconds */
int Time::millisecond() const
{
    return d.msec();
}

/** Return the number of seconds to the passed time */
int Time::secsTo(const Time &other) const
{
    return d.secsTo(other.d);
}

/** Return the number of milliseconds to the passed time */
int Time::msecsTo(const Time &other) const
{
    return d.msecsTo(other.d);
}

/** Return a string representation of this time */
String Time::toString() const
{
    return d.toString();
}

/** Comparison operator */
bool Time::operator<(const Time &other) const
{
    return d < other.d;
}

/** Comparison operator */
bool Time::operator<=(const Time &other) const
{
    return d <= other.d;
}

/** Comparison operator */
bool Time::operator>=(const Time &other) const
{
    return d >= other.d;
}

/** Comparison operator */
bool Time::operator>(const Time &other) const
{
    return d > other.d;
}

/** Return the current time */
Time Time::current()
{
    return Time( QTime::currentTime() );
}

/** Copy assignment operator */
void Time::copy_object(const Time &other)
{
    d = other.d;
    super::copy_object(other);
}

/** Comparison operator */
bool Time::compare_object(const Time &other) const
{
    return d == other.d and super::compare_object(other);
}
