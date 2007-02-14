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

#ifndef SIREDB_RELATEATOMTYPEDATA_H
#define SIREDB_RELATEATOMTYPEDATA_H

#include <QList>

#include <boost/tuple/tuple.hpp>

#include "relateidmap.h"
#include "nameidset.h"
#include "relateatomtypedatalist.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

using boost::tuple;

/**
This small class is used by RelateAtomTypeDB to store the atom type matching criteria in a format that is usable by the database. This class should not be used by any other code.

@author Christopher Woods
*/
class SIREDB_EXPORT RelateAtomTypeData
{
public:
    RelateAtomTypeData();
    
    RelateAtomTypeData(RelateID molresid, NameID atype, uint nprotons);
    RelateAtomTypeData(const tuple<RelateID,int> &idscore, NameID atype, uint nprotons);
    
    RelateAtomTypeData(const RelateAtomTypeData &other);
    
    ~RelateAtomTypeData();

    int score() const;
    
    RelateID molResID() const;
    NameID atomType() const;
    uint nprotons() const;
    
    QString toQueryString(uint n) const;
    QString toInsertString() const;

private:
    /** The relationship ID of the molecule and residue matching criteria */
    RelateID molresid;
    /** The score of the molecule and residue matching criteria */
    int molres_score;

    /** The NameID of the atom type ID string */
    NameID atomid;
    /** The number of protons in the element of the atom 
        type (0 if this is a wild element match) */
    uint nprtns;

};

}

SIRE_END_HEADER

#endif
