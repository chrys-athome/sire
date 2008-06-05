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
