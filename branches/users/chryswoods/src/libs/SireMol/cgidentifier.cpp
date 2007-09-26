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
#include "cgidx.h"
#include "moleculeinfodata.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireID;

////////
//////// Implementation of CGName
////////

QList<CGIdx> CGName::map(const MoleculeInfoData &molinfo) const
{
    return molinfo.map(*this);
}

////////
//////// Implementation of CGAtomIdx
////////

/** Map this CGAtomIdx back to the index of the atom in the molecule 
    
    \throw SireError::invalid_index
*/
QList<AtomIdx> CGAtomIdx::map(const MoleculeInfoData &molinfo) const
{
    QList<AtomIdx> atomidxs;
    atomidxs.append( molinfo.atomIdx(*this) );
    
    return atomidxs;
}

////////
//////// Implementation of CGIdentifier
////////

static const RegisterMetaType<CGIdentifier> r_cgid;

/** Serialise to a binary datastream */
QDataStream SIREID_EXPORT &operator<<(QDataStream &ds, const CGIdentifier &cgid)
{
    return cgid.save(ds, r_cgid);
}

/** Deserialise from a binary datastream */
QDataStream SIREID_EXPORT &operator>>(QDataStream &ds, CGIdentifier &cgid)
{
    return cgid.load(ds, r_cgid);
}

/** Return the hash of this ID */
uint SIREID_EXPORT qHash(const CGIdentifier &cgid)
{
    return cgid.hash();
}

