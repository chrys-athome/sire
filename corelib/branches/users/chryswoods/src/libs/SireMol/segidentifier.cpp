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

#include "segidentifier.h"
#include "segidx.h"
#include "molinfo.h"

using namespace SireMol;
using namespace SireID;

/** Null constructor */
SegIdentifier::SegIdentifier() : SegID()
{}

/** Construct from the passed SegID */
SegIdentifier::SegIdentifier(const SegID &segid)
              : SegID()
{
    if (segid.isA<SegIdentifier>())
        d = segid.asA<SegIdentifier>().d;
    else if (not segid.isNull())
        d.reset( segid.clone() );
}

/** Copy constructor */
SegIdentifier::SegIdentifier(const SegIdentifier &other)
              : SegID(other), d(other.d)
{}

/** Destructor */
SegIdentifier::~SegIdentifier()
{}

/** Is this selection null? */
bool SegIdentifier::isNull() const
{
    return d.get() == 0;
}

/** Return a hash of this identifier */
uint SegIdentifier::hash() const
{
    if (d.get() == 0)
        return 0;
    else
        return d->hash();
}
            
/** Return a string representatio of this ID */
QString SegIdentifier::toString() const
{
    if (d.get() == 0)
        return "null";
    else
        return d->toString();
}

/** Return the base type of this ID */
const SegID& SegIdentifier::base() const
{
    if (d.get() == 0)
        return *this;
    else
        return *d;
}

/** Copy assignment operator */
SegIdentifier& SegIdentifier::operator=(const SegIdentifier &other)
{
    d = other.d;
    return *this;
}

/** Copy assignment operator */
SegIdentifier& SegIdentifier::operator=(const SegID &other)
{
    if (other.isA<SegIdentifier>())
        d = other.asA<SegIdentifier>().d;
    else if (other.isNull())
        d.reset();
    else
        d.reset(other.clone());
    
    return *this;
}

/** Comparison operator */
bool SegIdentifier::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare<SegIdentifier>(*this, other);
}

/** Comparison operator */
bool SegIdentifier::operator==(const SegIdentifier &other) const
{
    if (d.get() == 0 or other.d.get() == 0)
        return d.get() == other.d.get();
    else
        return d == other.d or *d == *(other.d);
}

/** Comparison operator */
bool SegIdentifier::operator!=(const SegIdentifier &other) const
{
    if (d.get() == 0 or other.d.get() == 0)
        return d.get() != other.d.get();
    else
        return d != other.d and *d != *(other.d);
}

/** Comparison operator */
bool SegIdentifier::operator==(const SegID &other) const
{
    if (d.get() == 0)
        return other.isNull();
    else if (other.isA<SegIdentifier>())
        return this->operator==(other.asA<SegIdentifier>());
    else
        return d->operator==(other);
}

/** Comparison operator */
bool SegIdentifier::operator!=(const SegID &other) const
{
    if (d.get() == 0)
        return not other.isNull();
    else if (other.isA<SegIdentifier>())
        return this->operator!=(other.asA<SegIdentifier>());
    else
        return d->operator!=(other);
}

/** Map this ID to the list of indicies of segments that match this ID

    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
QList<SegIdx> SegIdentifier::map(const MolInfo &molinfo) const
{
    if (d.get() == 0)
        return molinfo.getSegments();
    else
        return d->map(molinfo);
}
