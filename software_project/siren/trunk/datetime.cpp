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

#include "Siren/datetime.h"
#include "Siren/siren.hpp"

REGISTER_SIREN_CLASS( Siren::DateTime )

using namespace Siren;

/** Construct a DateTime to represent the current date and time */
DateTime::DateTime() : Object()
{}

/** Construct a DateTime with the passed date and time */
DateTime::DateTime(const Date &date, const Time &time)
         : Object(), d(date), t(time)
{}

/** Copy constructor */
DateTime::DateTime(const DateTime &other) : Object(other), d(other.d), t(other.t)
{}

/** Destructor */
DateTime::~DateTime()
{}

/** Return the date */
Date DateTime::date() const
{
    return d;
}

/** Return the time */
Time DateTime::time() const
{
    return t;
}

/** Return a string representation of this date and time */
String DateTime::toString() const
{
    return String::tr("Date: %1 - Time %2")
                    .arg(d.toString(), t.toString());
}

/** Return the current date and time */
DateTime DateTime::current()
{
    return DateTime();
}

/** Copy assignment operator */
void DateTime::copy_object(const DateTime &other)
{
    d = other.d;
    t = other.t;
    super::copy_object(other);
}

/** Comparison operator */
bool DateTime::compare_object(const DateTime &other) const
{
    return d == other.d and t == other.t and super::compare_object(other);
}
