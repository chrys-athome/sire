#ifndef SIREDB_RELATIONSHIPDB_H
#define SIREDB_RELATIONSHIPDB_H

#include <QList>

#include "dbbase.h"
#include "atomindex.h"
#include "matchmrdata.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

using SireMol::AtomIndex;

/**
This is the base class of all relationship databases. This contains no data - just an interface that is used by the assign classes (e.g. assign_atoms)

@author Christopher Woods
*/
class SIREDB_EXPORT RelationshipDB : public DBBase
{
public:
    RelationshipDB();
    RelationshipDB(const RelationshipDB &other);

    ~RelationshipDB();

    static const char* typeName()
    {
        return "SireDB::RelationshipDB";
    }

    template<class T>
    RelateIDMap search( const T &internal, const ParameterTable &param_table,
                        const MatchMRData &matchmr = MatchMRData() );
    
protected:
    virtual RelateIDMap findMatches(const QList<AtomIndex> &atoms,
                                    const ParameterTable &param_table,
                                    const MatchMRData &matchmr)=0;

    void prepareToDump();
    void postLoad();
};

/** Return the relationship IDs that match the internal 'internal', using
    the information that is currently stored in the parameter table 'param_table' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
RelateIDMap RelationshipDB::search(const T &internal,
                                   const ParameterTable &param_table,
                                   const MatchMRData &matchmr)
{
    //convert the internal into a list of atoms
    QList<AtomIndex> atoms;
    
    for (int i=0; i<internal.size(); ++i)
        atoms.append( internal[i] );
        
    return findMatches(atoms, param_table, matchmr);
}

/** Specialisation for search(atom) */
template<>
inline RelateIDMap RelationshipDB::search(const AtomIndex &atom,
                                          const ParameterTable &param_table,
                                          const MatchMRData &matchmr)
{
    QList<AtomIndex> atoms;
    atoms.append( atom );
    
    return findMatches(atoms, param_table, matchmr);
}

}

SIRE_END_HEADER

#endif
