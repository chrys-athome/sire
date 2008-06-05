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

#ifndef SIREDB_RELATEATOMTYPEDB_H
#define SIREDB_RELATEATOMTYPEDB_H

#include "relationshipdb.h"

#include "relateidmap.h"
#include "relateatomtypedatalist.h"
#include "nameidset.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireDB
{

using boost::tuple;

class MatchAtomType;
class MatchAtomTypeData;
class MatchMR;

class RelateMRDB;
class NameDB;
class RelateAtomTypeDB;

/**
This database is to MatchAtomTypeData what RelateMRADB is to MatchMRAData. This stores the relationships that involve atom types.

@author Christopher Woods
*/
class SIREDB_EXPORT RelateAtomTypeDB : public RelationshipDB
{

public:
    RelateAtomTypeDB();
    RelateAtomTypeDB(const RelateAtomTypeDB &other);

    ~RelateAtomTypeDB();

    /** Return the full name of this database component */
    static const char* typeName()
    {
        return "SireDB::RelateAtomTypeDB";
    }

    const char* what() const
    {
        return RelateAtomTypeDB::typeName();
    }

    RelateID add(const MatchAtomTypeData &matchdata, uint n);
    RelateID get(const MatchAtomTypeData &match, uint n);

    RelateID add(const MatchAtomType &match);
    RelateID get(const MatchAtomType &match);

    MatchAtomTypeData get(RelateID relateid, uint n);

    void setCaseSensitiveAtomTypes();
    void setCaseInsensitiveAtomTypes();
    bool caseSensitiveAtomTypes();

    NameID addAtomType(const QString &atomtype);
    NameID getAtomTypeID(const QString &atomtype);
    QString getAtomType(NameID id);

protected:
    void initialise();
    void prepareToDump();
    void postLoad();

    RelateIDMap findMatches(const MatchAtomTypeData &match, uint n);

    RelateIDMap findMatches(const QList<AtomIndex> &atoms,
                            const Molecule &molecule,
                            const ParameterTable &param_table,
                            const MatchMRData &matchmr);

private:

    static QString relateTable(uint n);
    static QString queryString(const RelateIDMap &molresmap, uint i);

    void createRelateTable(uint n);

    RelateAtomTypeData convert(const MatchAtomType &match);
    RelateAtomTypeData convert(const tuple<MatchMR,MatchAtomType> &match);
    RelateAtomTypeDataList convert(const MatchAtomTypeData &match, uint n);

    tuple<MatchMR,MatchAtomType> convert(const RelateAtomTypeData &relatedata);
    MatchAtomTypeData convert(const RelateAtomTypeDataList &relatedata);

    RelateID add(const RelateAtomTypeDataList &relatedata);
    RelateID get(const RelateAtomTypeDataList &relatedata);

    /** The set of all relate tables that have been created */
    QSet<quint32> relatetables;

    /** Pointer to the RelateMRDB that is used to store and index matching
        relationships involving molecule and residue criteria */
    RelateMRDB *relatemrdb;

    /** Pointer to the NameDB that is used to store the atom type IDs
        and to decide on the case-sensitivity of the matching */
    NameDB *namedb;
};

}

Q_DECLARE_METATYPE(SireDB::RelateAtomTypeDB)

SIRE_END_HEADER

#endif
