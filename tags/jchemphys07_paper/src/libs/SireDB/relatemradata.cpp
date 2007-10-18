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

#include "relatemradata.h"

using namespace SireDB;

/** Null constructor */
RelateMRAData::RelateMRAData()
              : molresid(0), molres_score(0), atomname(0), atomnum(0)
{}

/** Construct a RelateMRAData that contains the matching criteria
    in a database-friendly format. */
RelateMRAData::RelateMRAData(RelateID mrid, NameID aname, AtomNum anum)
              : molresid(mrid), molres_score(0),
                atomname(aname), atomnum(anum)
{}

/** Construct a RelateMRAData that contains the matching criteria
    in a database-friendly format. */
RelateMRAData::RelateMRAData(const tuple<RelateID,int> &idscore, NameID aname, AtomNum anum)
              : molresid(idscore.get<0>()), molres_score(idscore.get<1>()),
                atomname(aname), atomnum(anum)
{}

/** Copy constructor */
RelateMRAData::RelateMRAData(const RelateMRAData &other)
              : molresid(other.molresid), molres_score(other.molres_score),
                atomname(other.atomname), atomnum(other.atomnum)
{}

/** Destructor */
RelateMRAData::~RelateMRAData()
{}

/** Return the ID number of the relationship containing the molecule and 
    residue matching criteria */
RelateID RelateMRAData::molResID() const
{
    return molresid;
}

/** Return the ID number of the atom's name */
NameID RelateMRAData::atomName() const
{
    return atomname;
}

/** Return the atom's number */
AtomNum RelateMRAData::atomNumber() const
{
    return atomnum;
}

/** Return this data formatted to form part of an SQL query string for atom 'n'
    that will find the exact match of this data */
QString RelateMRAData::toQueryString(uint n) const
{
    return QString("MolResID%1 = %2 and "
                   "AtomName%1 = %3 and "
                   "AtomNum%1 = %4")
              .arg(n).arg(molresid).arg(atomname).arg(atomnum.toString());
}

/** Return this data formatted to form part of an SQL insert string for atom 'n' */
QString RelateMRAData::toInsertString() const
{
    return QString("%1, %2, %3").arg(molresid).arg(atomname).arg(atomnum.toString());
}

/** Return the score value of this match - this is used to rank matches according to 
    how specific they are.
*/
int RelateMRAData::score() const
{
    return molres_score + 8192*(atomnum != 0) + 512*(atomname != 0);
}
