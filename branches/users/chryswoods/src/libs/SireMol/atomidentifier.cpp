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
#include "specify.hpp"
#include "atomname.h"
#include "atomnum.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireID;

///////
/////// Implementation of AtomName
///////

QList<AtomIdx> AtomName::map(const MoleculeInfoData &molinfo) const
{
    return molinfo.map(*this);
}

///////
/////// Implementation of AtomNum
///////

QList<AtomIdx> AtomNum::map(const MoleculeInfoData &molinfo) const
{
    return molinfo.map(*this);
}

///////
/////// Implementation of AtomIdentifier
///////

static const RegisterMetaType<AtomIdentifier> r_atomid;

/** Serialise to a binary datastream */
QDataStream SIREID_EXPORT &operator<<(QDataStream &ds, const AtomIdentifier &atomid)
{
    return atomid.save(ds, r_atomid);
}

/** Deserialise from a binary datastream */
QDataStream SIREID_EXPORT &operator>>(QDataStream &ds, AtomIdentifier &atomid)
{
    return atomid.load(ds, r_atomid);
}

/** Return the hash of this ID */
uint SIREID_EXPORT qHash(const AtomIdentifier &atomid)
{
    return atomid.hash();
}

/** Construct a null identifier */
AtomIdentifier::AtomIdentifier() : Identifier_T_<AtomIdentifier,AtomID>()
{}

/** Construct from the passed AtomID */
AtomIdentifier::AtomIdentifier(const AtomID &atomid)
               : Identifier_T_<AtomIdentifier,AtomID>(atomid)
{}

/** Copy constructor */
AtomIdentifier::AtomIdentifier(const AtomIdentifier &other)
               : Identifier_T_<AtomIdentifier,AtomID>(other)
{}
  
/** Destructor */ 
AtomIdentifier::~AtomIdentifier()
{}

/** Return a specific atom that matches this identifier */
Specify<AtomIdentifier> AtomIdentifier::operator[](int i) const
{
    return Specify<AtomIdentifier>(*this, i);
}

/** Return a specific atom that matches this identifier */
Specify<AtomIdentifier> AtomIdentifier::operator()(int i) const
{
    return this->operator[](i);
}

/** Return a range of atoms that match this identifier */
Specify<AtomIdentifier> AtomIdentifier::operator()(int i, int j) const
{
    return Specify<AtomIdentifier>(*this, i, j);
}
    
/** Return the list of indicies of atoms that match this identifier */
QList<AtomIdx> AtomIdentifier::map(const MoleculeInfoData &molinfo) const
{
    if (this->isNull())
        return molinfo.getAtoms();
    else
        return this->asA<AtomID>().map(molinfo);
}
