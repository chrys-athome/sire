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

#include "moleculeinfodata.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireID;

static const RegisterMetaType<SegIdentifier> r_segid;

///////
/////// Implementation of SegName
///////

QList<SegIdx> SegName::map(const MoleculeInfoData &molinfo) const
{
    return molinfo.map(*this);
}

///////
/////// Implementation of SegIdentifier
///////

/** Serialise to a binary datastream */
QDataStream SIREID_EXPORT &operator<<(QDataStream &ds, const SegIdentifier &segid)
{
    return segid.save(ds, r_segid);
}

/** Deserialise from a binary datastream */
QDataStream SIREID_EXPORT &operator>>(QDataStream &ds, SegIdentifier &segid)
{
    return segid.load(ds, r_segid);
}

/** Return the hash of this ID */
uint SIREID_EXPORT qHash(const SegIdentifier &segid)
{
    return segid.hash();
}
