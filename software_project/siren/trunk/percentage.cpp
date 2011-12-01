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

#include "Siren/percentage.h"
#include "Siren/string.h"
#include "Siren/siren.hpp"

using namespace Siren;

REGISTER_SIREN_CLASS( Siren::Percentage )

/** Null constructor - this is 0% */
Percentage::Percentage() : Object(), percent(0)
{}

/** Construct from the passed integer - this is clamped to the range
    of 0 to 100 */
Percentage::Percentage(int p) : Object()
{
    if (p >= 100)
        percent = 100;
    
    else if (p <= 0)
        percent = 0;
    
    else
        percent = p;
}

Percentage::Percentage(const Percentage &other) 
           : Object(other), percent(other.percent)
{}

/** Destructor */
Percentage::~Percentage()
{}

/** Copy assignment operator */
void Percentage::copy_object(const Percentage &other)
{
    percent = other.percent;
    super::copy_object(other);
}

/** Comparison operator */
bool Percentage::compare_object(const Percentage &other) const
{
    return percent == other.percent and super::compare_object(other);
}

/** Return the actual value */
int Percentage::value() const
{
    return percent;
}

/** Return the actual value */
Percentage::operator int() const
{
    return percent;
}

/** Return a string representation of this percentage */
String Percentage::toString() const
{
    return String::tr("%1%%").arg(percent);
}
