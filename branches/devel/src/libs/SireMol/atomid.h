#ifndef SIREMOL_ATOMID_H
#define SIREMOL_ATOMID_H

#include "id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This ID number is used to identify an Atom via its index.
    (e.g. the fifth atom in a residue)

    Be careful not to confuse this with AtomNum, which is a
    user-supplied ID number for an atom (AtomID is to
    AtomNum as ResID is to ResNum)

    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomID : public IDBase
{

public:
    AtomID() : IDBase()
    {}

    explicit AtomID(quint32 id) : IDBase(id)
    {}

    AtomID(const AtomID &other) : IDBase(other)
    {}

    ~AtomID()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::AtomID, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
