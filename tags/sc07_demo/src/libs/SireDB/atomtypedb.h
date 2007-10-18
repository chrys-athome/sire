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

#ifndef SIREDB_ATOMTYPEDB_H
#define SIREDB_ATOMTYPEDB_H

#include "atomdb.h"
#include "atomtype.h"
#include "atomtypetable.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/**
This database is used to store and retrieve atom types.

@author Christopher Woods
*/
class SIREDB_EXPORT AtomTypeDB : public AtomDB
{
public:
    /** An AtomTypeDB stores AtomType objects */
    typedef AtomType parameter_type;

    /** An AtomTypeDB will place its parameters into an AtomTypeTable */
    typedef AtomTypeTable table_type;

    AtomTypeDB();
    AtomTypeDB(const AtomTypeDB &other);

    ~AtomTypeDB();

    /** Return the full name of this database component */
    static const char* typeName()
    {
        return "SireDB::AtomTypeDB";
    }

    const char* what() const
    {
        return AtomTypeDB::typeName();
    }

    void addAtomType(const QString &userid, const AtomType &atomtype);
    AtomType getAtomType(const QString &userid, bool *foundtype = 0);

    void relateAtomType(const AssertMatch<1> &matchatom, const QString &userid);
    void relateAtomType(const AssertMatch<1> &matchatom, const AtomType &atomtype);

    void relateAtomType(RelateID relateid, const QString &userid);
    void relateAtomType(RelateID relateid, const AtomType &atomtype);

    AtomType getAtomType(RelateID relateid, bool *foundtype = 0);
    AtomType getAtomType(const RelateIDMap &relateids, bool *foundtype = 0);

    void createTable( ParameterTable &param_table ) const;

    bool assignParameter( const AtomIndex &atom, const RelateIDMap &relateids,
                          ParameterTable &param_table );

protected:
    void initialise();
    void prepareToDump();
    void postLoad();

    AtomType retrieveAtomType(ParamID id);
    ParamID addAtomType(const AtomType &atomtype);

};

}

Q_DECLARE_METATYPE(SireDB::AtomTypeDB)

SIRE_END_HEADER

#endif
