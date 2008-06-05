
#include "relateatomtypedata.h"

using namespace SireDB;

/** Null constructor */
RelateAtomTypeData::RelateAtomTypeData()
{}

/** Construct from the passed data */
RelateAtomTypeData::RelateAtomTypeData(RelateID mrid, NameID atype, uint nprotons)
                   : molresid(mrid), molres_score(0), atomid(atype), nprtns(nprotons)
{}

/** Construct from the passed data */
RelateAtomTypeData::RelateAtomTypeData(const tuple<RelateID,int> &idscore, 
                                       NameID atype, uint nprotons)
                   : molresid(idscore.get<0>()), molres_score(idscore.get<1>()),
                     atomid(atype), nprtns(nprotons)
{}
    
/** Copy constructor */
RelateAtomTypeData::RelateAtomTypeData(const RelateAtomTypeData &other)
                   : molresid(other.molresid), molres_score(other.molres_score),
                     atomid(other.atomid), nprtns(other.nprtns)
{}

/** Destructor */
RelateAtomTypeData::~RelateAtomTypeData()
{}

/** Return the score of this match. Matching the atom type ID string is more
    significant than matching the atom type's element */
int RelateAtomTypeData::score() const
{
    return molres_score + 256 * (atomid != 0)
                        + 128 * (nprtns != 0);
}
    
/** Return the relationship ID of the molecule and residue matching criteria */
RelateID RelateAtomTypeData::molResID() const
{
    return molresid;
}

/** Return the NameID of the atom type ID string */
NameID RelateAtomTypeData::atomType() const
{
    return atomid;
}

/** Return the number of protons in the element of the atom type */
uint RelateAtomTypeData::nprotons() const
{
    return nprtns;
}
    
/** Return this data as a SQL query string for the nth atom of the group */
QString RelateAtomTypeData::toQueryString(uint n) const
{
    return QString("MolResID%1 = %2 and "
                   "AtomType%1 = %3 and "
                   "Element%1 = %4").arg(n).arg(molresid).arg(atomid).arg(nprtns);
}

/** Return a string containing this data in a format that may be used
    to insert the data into the SQL database */
QString RelateAtomTypeData::toInsertString() const
{
    return QString("%1, %2, %3").arg(molresid).arg(atomid).arg(nprtns);
}
