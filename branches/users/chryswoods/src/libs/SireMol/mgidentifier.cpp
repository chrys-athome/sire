/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "mgidentifier.h"
#include "mgnum.h"
#include "molgroups.h"

using namespace SireMol;
using namespace SireID;

/** Null constructor */
MGIdentifier::MGIdentifier() : MGID()
{}

/** Construct from the passed MGID */
MGIdentifier::MGIdentifier(const MGID &molid)
             : MGID()
{
    if (molid.isA<MGIdentifier>())
        d = molid.asA<MGIdentifier>().d;
    else if (not molid.isNull())
        d.reset( molid.clone() );
}

/** Copy constructor */
MGIdentifier::MGIdentifier(const MGIdentifier &other)
             : MGID(other), d(other.d)
{}

/** Destructor */
MGIdentifier::~MGIdentifier()
{}

/** Is this selection null? */
bool MGIdentifier::isNull() const
{
    return d.get() == 0;
}

/** Return a hash of this identifier */
uint MGIdentifier::hash() const
{
    if (d.get() == 0)
        return 0;
    else
        return d->hash();
}
            
/** Return a string representatio of this ID */
QString MGIdentifier::toString() const
{
    if (d.get() == 0)
        return "null";
    else
        return d->toString();
}

/** Return the base type of this ID */
const MGID& MGIdentifier::base() const
{
    if (d.get() == 0)
        return *this;
    else
        return *d;
}

QList<MGNum> MGIdentifier::map(const MolGroupsBase &molgroups) const
{
    if (d.get() == 0)
        return molgroups.groupNums();
    else
        return molgroups.map(*d);
}

/** Copy assignment operator */
MGIdentifier& MGIdentifier::operator=(const MGIdentifier &other)
{
    d = other.d;
    return *this;
}

/** Copy assignment operator */
MGIdentifier& MGIdentifier::operator=(const MGID &other)
{
    if (other.isA<MGIdentifier>())
        d = other.asA<MGIdentifier>().d;
    else if (other.isNull())
        d.reset();
    else
        d.reset(other.clone());
    
    return *this;
}

/** Comparison operator */
bool MGIdentifier::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare<MGIdentifier>(*this, other);
}

/** Comparison operator */
bool MGIdentifier::operator==(const MGIdentifier &other) const
{
    if (d.get() == 0 or other.d.get() == 0)
        return d.get() == other.d.get();
    else
        return d == other.d or *d == *(other.d);
}

/** Comparison operator */
bool MGIdentifier::operator!=(const MGIdentifier &other) const
{
    if (d.get() == 0 or other.d.get() == 0)
        return d.get() != other.d.get();
    else
        return d != other.d and *d != *(other.d);
}

/** Comparison operator */
bool MGIdentifier::operator==(const MGID &other) const
{
    if (d.get() == 0)
        return other.isNull();
    else if (other.isA<MGIdentifier>())
        return this->operator==(other.asA<MGIdentifier>());
    else
        return d->operator==(other);
}

/** Comparison operator */
bool MGIdentifier::operator!=(const MGID &other) const
{
    if (d.get() == 0)
        return not other.isNull();
    else if (other.isA<MGIdentifier>())
        return this->operator!=(other.asA<MGIdentifier>());
    else
        return d->operator!=(other);
}
