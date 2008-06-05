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
