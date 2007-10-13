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

#include "cgidentifier.h"
#include "moleculeinfodata.h"

using namespace SireMol;
using namespace SireID;

/** Null constructor */
CGIdentifier::CGIdentifier() : CGID()
{}

/** Construct from the passed CGID */
CGIdentifier::CGIdentifier(const CGID &cgid)
             : CGID()
{
    if (cgid.isA<CGIdentifier>())
        d = cgid.asA<CGIdentifier>().d;
    else if (not cgid.isNull())
        d.reset( cgid.clone() );
}

/** Copy constructor */
CGIdentifier::CGIdentifier(const CGIdentifier &other)
             : CGID(other), d(other.d)
{}

/** Destructor */
CGIdentifier::~CGIdentifier()
{}

/** Is this selection null? */
bool CGIdentifier::isNull() const
{
    return d.get() == 0;
}

/** Return a hash of this identifier */
uint CGIdentifier::hash() const
{
    if (d.get() == 0)
        return 0;
    else
        return d->hash();
}
            
/** Return a string representatio of this ID */
QString CGIdentifier::toString() const
{
    if (d.get() == 0)
        return "null";
    else
        return d->toString();
}

/** Return the base type of this ID */
const CGID& CGIdentifier::base() const
{
    if (d.get() == 0)
        return *this;
    else
        return *d;
}

/** Copy assignment operator */
CGIdentifier& CGIdentifier::operator=(const CGIdentifier &other)
{
    d = other.d;
    return *this;
}

/** Copy assignment operator */
CGIdentifier& CGIdentifier::operator=(const CGID &other)
{
    if (other.isA<CGIdentifier>())
        d = other.asA<CGIdentifier>().d;
    else if (other.isNull())
        d.reset();
    else
        d.reset(other.clone());
    
    return *this;
}

/** Comparison operator */
bool CGIdentifier::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare<CGIdentifier>(*this, other);
}

/** Comparison operator */
bool CGIdentifier::operator==(const CGIdentifier &other) const
{
    if (d.get() == 0 or other.d.get() == 0)
        return d.get() == other.d.get();
    else
        return d == other.d or *d == *(other.d);
}

/** Comparison operator */
bool CGIdentifier::operator!=(const CGIdentifier &other) const
{
    if (d.get() == 0 or other.d.get() == 0)
        return d.get() != other.d.get();
    else
        return d != other.d and *d != *(other.d);
}

/** Comparison operator */
bool CGIdentifier::operator==(const CGID &other) const
{
    if (d.get() == 0)
        return other.isNull();
    else if (other.isA<CGIdentifier>())
        return this->operator==(other.asA<CGIdentifier>());
    else
        return d->operator==(other);
}

/** Comparison operator */
bool CGIdentifier::operator!=(const CGID &other) const
{
    if (d.get() == 0)
        return not other.isNull();
    else if (other.isA<CGIdentifier>())
        return this->operator!=(other.asA<CGIdentifier>());
    else
        return d->operator!=(other);
}

/** Map this ID to the list of indicies of atoms that match this ID

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
QList<CGIdx> CGIdentifier::map(const MoleculeInfoData &molinfo) const
{
    if (d.get() == 0)
        return molinfo.getCutGroups();
    else
        return d->map(molinfo);
}
