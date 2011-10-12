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

#include "Siren/date.h"
#include "Siren/siren.hpp"

REGISTER_SIREN_CLASS( Siren::Date )

using namespace Siren;

/** Constructor that creates the current date */
Date::Date() : Object(), d( QDate::currentDate() )
{}

/** Construct the specified year, month and day */
Date::Date(int y, int m, int d) : Object(), d(y,m,d)
{}

/** Copy constructor */
Date::Date(const Date &other) : Object(other), d(other.d)
{}

#ifdef SIREN_QT_SUPPORT
    Date::Date(const QDate &date) : Object(), d(date)
    {}
    
    Date::operator QDate() const
    {
        return d;
    }
#endif

/** Destructor */
Date::~Date()
{}

/** Return a string representation of the date */
String Date::toString() const
{
    return d.toString();
}

/** Return the day */
int Date::day() const
{
    return d.day();
}

/** Return the month */
int Date::month() const
{
    return d.month();
}

/** Return the year */
int Date::year() const
{
    return d.year();
}

/** Return the day of the week */
int Date::dayOfWeek() const
{
    return d.dayOfWeek();
}

/** Return the day of the year */
int Date::dayOfYear() const
{
    return d.dayOfYear();
}

/** Return the number of days in this month */
int Date::daysInMonth() const
{
    return d.daysInMonth();
}

/** Return the number of days in this year */
int Date::daysInYear() const
{
    return d.daysInYear();
}

/** Return whether or not this is a valid date */
bool Date::isValid() const
{
    return d.isValid();
}

/** Comparison operator */
bool Date::operator<(const Date &other) const
{
    return d < other.d;
}

/** Comparison operator */
bool Date::operator<=(const Date &other) const
{
    return d <= other.d;
}

/** Comparison operator */
bool Date::operator>=(const Date &other) const
{
    return d >= other.d;
}

/** Comparison operator */
bool Date::operator>(const Date &other) const
{
    return d > other.d;
}

/** Return the current date */
Date Date::current()
{
    return Date();
}

/** Copy assignment operator */
void Date::copy_object(const Date &other)
{
    d = other.d;
    super::copy_object(other);
}

/** Comparison operator */
bool Date::compare_object(const Date &other) const
{
    return d == other.d and super::compare_object(other);
}
