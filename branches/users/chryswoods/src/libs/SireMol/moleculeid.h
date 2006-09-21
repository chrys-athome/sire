#ifndef SIREMOL_MOLECULEID_H
#define SIREMOL_MOLECULEID_H

#include "id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This ID number is used to identify a Molecule.

    @author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeID : public IDBase
{

public:
    MoleculeID() : IDBase()
    {}

    MoleculeID(quint32 id) : IDBase(id)
    {}

    MoleculeID(const MoleculeID &other) : IDBase(other)
    {}

    ~MoleculeID()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::MoleculeID, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
