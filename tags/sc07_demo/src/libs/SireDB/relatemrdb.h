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

#ifndef SIREDB_RELATEMRDB_H
#define SIREDB_RELATEMRDB_H

#include "SireMol/atomindex.h"

#include "dbbase.h"
#include "nameidset.h"
#include "relateidmap.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireDB
{

class NameDB;
class RelateMRData;
class MatchMR;
class MatchRes;

using boost::tuple;

class RelateMRDB;

/**
@author Christopher Woods
*/
class SIREDB_EXPORT RelateMRDB : public DBBase
{

public:
    RelateMRDB();
    RelateMRDB(const RelateMRDB &other);

    ~RelateMRDB();

    static const char* typeName()
    {
        return "SireDB::RelateMRDB";
    }

    const char* what() const
    {
        return RelateMRDB::typeName();
    }
    
    tuple<RelateID,int> add(const MatchMR &match);
    tuple<RelateID,int> get(const MatchMR &match);
    RelateIDMap search(const MatchMR &match);
    
    void alias(const MatchRes &matchres, const QString &alias);
    
    MatchMR get(RelateID relateid);    
    
    void setCaseSensitiveMoleculeGroups();
    void setCaseInsensitiveMoleculeGroups();
    bool caseSensitiveMoleculeGroups() const;
    
    void setCaseSensitiveMoleculeStates();
    void setCaseInsensitiveMoleculeStates();
    bool caseSensitiveMoleculeStates() const;
    
    void setCaseSensitiveResidueAliases();
    void setCaseInsensitiveResidueAliases();
    bool caseSensitiveResidueAliases();
    
    NameID addMoleculeGroup(const QString &molgroup);
    NameID getMoleculeGroupID(const QString &molgroup);
    QString getMoleculeGroup(NameID id);
    
    NameID addMoleculeState(const QString &molstate);
    NameID getMoleculeStateID(const QString &molstate);
    QString getMoleculeState(NameID id);

    NameID addResidueAlias(const QString &resalias);
    NameID getResidueAliasID(const QString &resalias);
    QString getResidueAlias(NameID id);

protected:
    void initialise();
    void prepareToDump();
    void postLoad();

private:
    RelateMRData convert(const MatchMR &match);
    MatchMR convert(const RelateMRData &match);
    
    RelateID get(const RelateMRData &relatedata);
    RelateIDMap search(const RelateMRData &relatedata);

    /** Pointer to the NameDB - this is necessary to map
        names to ID numbers */
    NameDB *namedb;
};

}

Q_DECLARE_METATYPE(SireDB::RelateMRDB)

SIRE_END_HEADER

#endif
