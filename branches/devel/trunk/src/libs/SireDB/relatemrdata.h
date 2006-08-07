#ifndef SIREDB_RELATEMRDATA_H
#define SIREDB_RELATEMRDATA_H

#include "SireMol/atomindex.h"

#include "relatemradb.h"
#include "relatemradatalist.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

using SireMol::ResNum;

/**
@author Christopher Woods
*/
class SIREDB_EXPORT RelateMRData
{
public:
    RelateMRData();
    RelateMRData(NameID molname, NameID molgroup, NameID molstate, 
                 NameID resalias, NameID resname, ResNum resnum, 
                 const NameIDSet &resbonds, const NameIDSet &resnobonds);
    
    RelateMRData(const RelateMRData &other);
    
    ~RelateMRData();

    bool isEmpty() const;

    bool hasCompatibleBonds(const NameIDSet &bondatoms, 
                            const NameIDSet &nobondatoms) const;
    
    const NameIDSet& bondedAtoms() const;
    const NameIDSet& nonBondedAtoms() const;
    
    NameID moleculeName() const;
    NameID moleculeGroup() const;
    NameID moleculeState() const;
    
    NameID residueAlias() const;
    
    NameID residueName() const;
    ResNum residueNumber() const;
    
    QString toQueryString() const;
    QString toSearchString(NameIDSet resaliases) const;
    QString toInsertString() const;

    int score() const;

private:

    /** The ID number of the molecule */
    NameID molname;
    /** The ID number of the molecule's group */
    NameID molgroup;
    /** The ID number of the molecule's state */
    NameID molstate;
    
    /** The ID number of the residue alias */
    NameID resalias;
    
    /** The ID number of the residue name */
    NameID resname;
    /** The residue number */
    ResNum resnum;
    /** Set of ID numbers of the atoms that must be involved in 
        inter-residue bonds */
    NameIDSet resbonds;
    /** Set of ID numbers of the atoms that must not be involved in
        inter-residue bonds */
    NameIDSet resnobonds;
    
};

}

SIRE_END_HEADER

#endif
