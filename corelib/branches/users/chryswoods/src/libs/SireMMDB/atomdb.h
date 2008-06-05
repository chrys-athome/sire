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

#ifndef SIREDB_ATOMDB_H
#define SIREDB_ATOMDB_H

#include "SireDB/relatedb.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomIndex;
}

namespace SireDB
{

class AtomTable;
class ParameterTable;

using SireMol::AtomIndex;

/**
This is the base class of all databases that store atom parameters. This provides a common interface, thus allowing all atom parameters to be assigned at once (rather than having to run through all atoms each time a new type of parameter is assigned).

@author Christopher Woods
*/
class SIREDB_EXPORT AtomDB : public RelateDB
{
public:
    AtomDB();
    AtomDB(const AtomDB &other);

    ~AtomDB();

    static const char* typeName()
    {
        return "SireDB::AtomDB";
    }

    virtual void createTable( ParameterTable &param_table ) const=0;

    virtual bool assignParameter( const AtomIndex &atom,
                                  const RelateIDMap &relateids,
                                  ParameterTable &param_table )=0;

protected:
    void prepareToDump();
    void postLoad();
};

}

SIRE_END_HEADER

#endif
