/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREDB_RELATEMRADATA_H
#define SIREDB_RELATEMRADATA_H

#include <QList>

#include <boost/tuple/tuple.hpp>

#include "SireMol/atom.h"

#include "relateidmap.h"
#include "nameidset.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

using boost::tuple;

using SireMol::AtomNum;

/**
This is a simple class that is used by RelateMRADB to contain the atom matching criteria in a format that can be used with the database.

@author Christopher Woods
*/
class SIREDB_EXPORT RelateMRAData
{
public:
    RelateMRAData();
    RelateMRAData(RelateID molresid, NameID aname, AtomNum anum);
    RelateMRAData(const tuple<RelateID,int> &idscore, NameID aname, AtomNum anum);
    
    RelateMRAData(const RelateMRAData &other);
    
    ~RelateMRAData();
    
    int score() const;
    
    RelateID molResID() const;
    NameID atomName() const;
    AtomNum atomNumber() const;

    QString toQueryString(uint n) const;
    QString toInsertString() const;

private:

    /** The RelateID of the molecule and residue matching criteria */
    RelateID molresid;
    /** The score of the molecule and residue matching criteria */
    int molres_score;

    /** The ID number of the atom */
    NameID atomname;
    /** The atom number */
    AtomNum atomnum;
    
};

}

SIRE_END_HEADER

#endif
