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

#include "atomidentifier.h"
#include "atomidx.h"

#include "molinfo.h"

#include "atom.h"
#include "selector.hpp"
#include "molecules.h"
#include "moleculegroup.h"
#include "moleculegroups.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireID;
using namespace SireStream;

static const RegisterMetaType<AtomIdentifier> r_atomid;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const AtomIdentifier &atomid)
{
    throw SireError::incomplete_code( CODELOC );
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       AtomIdentifier &atomid)
{
    throw SireError::incomplete_code( CODELOC );
    return ds;
}

/** Null constructor */
AtomIdentifier::AtomIdentifier() : AtomID()
{}

/** Construct from the passed AtomID */
AtomIdentifier::AtomIdentifier(const AtomID &atomid)
               : AtomID()
{
    if (atomid.isA<AtomIdentifier>())
        d = atomid.asA<AtomIdentifier>().d;
    else if (not atomid.isNull())
        d.reset( atomid.clone() );
}

/** Copy constructor */
AtomIdentifier::AtomIdentifier(const AtomIdentifier &other)
               : AtomID(other), d(other.d)
{}

/** Destructor */
AtomIdentifier::~AtomIdentifier()
{}

/** Is this selection null? */
bool AtomIdentifier::isNull() const
{
    return d.get() == 0;
}

/** Return a hash of this identifier */
uint AtomIdentifier::hash() const
{
    if (d.get() == 0)
        return 0;
    else
        return d->hash();
}
            
/** Return a string representatio of this ID */
QString AtomIdentifier::toString() const
{
    if (d.get() == 0)
        return "null";
    else
        return d->toString();
}

/** Return the base type of this ID */
const AtomID& AtomIdentifier::base() const
{
    if (d.get() == 0)
        return *this;
    else
        return *d;
}

/** Copy assignment operator */
AtomIdentifier& AtomIdentifier::operator=(const AtomIdentifier &other)
{
    d = other.d;
    return *this;
}

/** Copy assignment operator */
AtomIdentifier& AtomIdentifier::operator=(const AtomID &other)
{
    if (other.isA<AtomIdentifier>())
        d = other.asA<AtomIdentifier>().d;
    else if (other.isNull())
        d.reset();
    else
        d.reset(other.clone());
    
    return *this;
}

/** Comparison operator */
bool AtomIdentifier::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare<AtomIdentifier>(*this, other);
}

/** Comparison operator */
bool AtomIdentifier::operator==(const AtomIdentifier &other) const
{
    if (d.get() == 0 or other.d.get() == 0)
        return d.get() == other.d.get();
    else
        return d == other.d or *d == *(other.d);
}

/** Comparison operator */
bool AtomIdentifier::operator!=(const AtomIdentifier &other) const
{
    if (d.get() == 0 or other.d.get() == 0)
        return d.get() != other.d.get();
    else
        return d != other.d and *d != *(other.d);
}

/** Comparison operator */
bool AtomIdentifier::operator==(const AtomID &other) const
{
    if (d.get() == 0)
        return other.isNull();
    else if (other.isA<AtomIdentifier>())
        return this->operator==(other.asA<AtomIdentifier>());
    else
        return d->operator==(other);
}

/** Comparison operator */
bool AtomIdentifier::operator!=(const AtomID &other) const
{
    if (d.get() == 0)
        return not other.isNull();
    else if (other.isA<AtomIdentifier>())
        return this->operator!=(other.asA<AtomIdentifier>());
    else
        return d->operator!=(other);
}

/** Map this ID to the list of indicies of atoms that match this ID

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
QList<AtomIdx> AtomIdentifier::map(const MolInfo &molinfo) const
{
    if (d.get() == 0)
        return molinfo.getAtoms();
    else
        return d->map(molinfo);
}

/** Return the atom from the molecules 'molecules' that matches
    this ID
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
*/
Atom AtomIdentifier::selectFrom(const Molecules &molecules) const
{
    if (d.get() == 0)
        return AtomID::selectFrom(molecules);
    
    else
        return d->selectFrom(molecules);
}

/** Return all of the atoms from the 'molecules' that match
    this ID
    
    \throw SireMol::missing_atom
*/
QHash< MolNum,Selector<Atom> > 
AtomIdentifier::selectAllFrom(const Molecules &molecules) const
{
    if (d.get() == 0)
        return AtomID::selectAllFrom(molecules);
        
    else
        return d->selectAllFrom(molecules);
}

/** Return the atom from the molecule group 'molgroup' that matches
    this ID
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
*/
Atom AtomIdentifier::selectFrom(const MoleculeGroup &molgroup) const
{
    if (d.get() == 0)
        return AtomID::selectFrom(molgroup);
        
    else
        return d->selectFrom(molgroup);
}

/** Return the atoms from the molecule group 'molgroup' that match
    this ID
    
    \throw SireMol::missing_atom
*/
QHash< MolNum,Selector<Atom> > 
AtomIdentifier::selectAllFrom(const MoleculeGroup &molgroup) const
{
    if (d.get() == 0)
        return AtomID::selectAllFrom(molgroup);
        
    else
        return d->selectAllFrom(molgroup);
}

/** Return the atom from the molecule groups 'molgroups' that matches 
    this ID
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
*/
Atom AtomIdentifier::selectFrom(const MolGroupsBase &molgroups) const
{
    if (d.get() == 0)
        return AtomID::selectFrom(molgroups);
        
    else
        return d->selectFrom(molgroups);
}

/** Return the set of atoms that match this ID in the molecule groups
    set 'molgroups' 
    
    \throw SireMol::missing_atom
*/
QHash< MolNum,Selector<Atom> > 
AtomIdentifier::selectAllFrom(const MolGroupsBase &molgroups) const
{
    if (d.get() == 0)
        return AtomID::selectAllFrom(molgroups);
        
    else
        return d->selectAllFrom(molgroups);
}
