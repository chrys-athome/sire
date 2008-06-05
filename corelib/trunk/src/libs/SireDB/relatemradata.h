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
