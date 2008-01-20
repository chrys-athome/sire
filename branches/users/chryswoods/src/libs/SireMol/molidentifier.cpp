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

#include "molidentifier.h"
#include "molgroup.h"
#include "moleculeinfodata.h"

#include "mover.hpp"

using namespace SireMol;
using namespace SireID;

/** Null constructor */
MolIdentifier::MolIdentifier() : MolID()
{}

/** Construct from the passed MolID */
MolIdentifier::MolIdentifier(const MolID &molid)
              : MolID()
{
    if (molid.isA<MolIdentifier>())
        d = molid.asA<MolIdentifier>().d;
    else if (not molid.isNull())
        d.reset( molid.clone() );
}

/** Copy constructor */
MolIdentifier::MolIdentifier(const MolIdentifier &other)
              : MolID(other), d(other.d)
{}

/** Destructor */
MolIdentifier::~MolIdentifier()
{}

/** Is this selection null? */
bool MolIdentifier::isNull() const
{
    return d.get() == 0;
}

/** Return a hash of this identifier */
uint MolIdentifier::hash() const
{
    if (d.get() == 0)
        return 0;
    else
        return d->hash();
}
            
/** Return a string representatio of this ID */
QString MolIdentifier::toString() const
{
    if (d.get() == 0)
        return "null";
    else
        return d->toString();
}

/** Return the base type of this ID */
const MolID& MolIdentifier::base() const
{
    if (d.get() == 0)
        return *this;
    else
        return *d;
}

/** Copy assignment operator */
MolIdentifier& MolIdentifier::operator=(const MolIdentifier &other)
{
    d = other.d;
    return *this;
}

/** Copy assignment operator */
MolIdentifier& MolIdentifier::operator=(const MolID &other)
{
    if (other.isA<MolIdentifier>())
        d = other.asA<MolIdentifier>().d;
    else if (other.isNull())
        d.reset();
    else
        d.reset(other.clone());
    
    return *this;
}

/** Comparison operator */
bool MolIdentifier::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare<MolIdentifier>(*this, other);
}

/** Comparison operator */
bool MolIdentifier::operator==(const MolIdentifier &other) const
{
    if (d.get() == 0 or other.d.get() == 0)
        return d.get() == other.d.get();
    else
        return d == other.d or *d == *(other.d);
}

/** Comparison operator */
bool MolIdentifier::operator!=(const MolIdentifier &other) const
{
    if (d.get() == 0 or other.d.get() == 0)
        return d.get() != other.d.get();
    else
        return d != other.d and *d != *(other.d);
}

/** Comparison operator */
bool MolIdentifier::operator==(const MolID &other) const
{
    if (d.get() == 0)
        return other.isNull();
    else if (other.isA<MolIdentifier>())
        return this->operator==(other.asA<MolIdentifier>());
    else
        return d->operator==(other);
}

/** Comparison operator */
bool MolIdentifier::operator!=(const MolID &other) const
{
    if (d.get() == 0)
        return not other.isNull();
    else if (other.isA<MolIdentifier>())
        return this->operator!=(other.asA<MolIdentifier>());
    else
        return d->operator!=(other);
}

/** Map this ID back to the numbers of the matching molecules in the 
    molecule group 'molgroup'
    
    \throw SireMol::missing_molecule
    \throw SireError::invalid_index
*/
QList<MolNum> MolIdentifier::map(const MolGroup &molgroup) const
{
    if (d.get() == 0)
        return molgroup.molNums().toList();
    else
        return d->map(molgroup);
}
