#ifndef SIREMOL_RESNUM_H
#define SIREMOL_RESNUM_H

#include "id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This ID number is used to identify a residue by the user-supplied
    residue number (this is typically the number assigned to the
    residue from the PDB or other coordinate file)

    Be careful not to confuse this with ResID, which is the
    index of the residue in the molecule or group (e.g. the
    fifth residue in the molecule would have ResID '4' but has
    whatever ResNum the user supplied.

    @author Christopher Woods
*/
class SIREMOL_EXPORT ResNum : public IDBase
{

public:
    ResNum() : IDBase()
    {}

    explicit ResNum(quint32 id) : IDBase(id)
    {}

    ResNum(const ResNum &other) : IDBase(other)
    {}

    ~ResNum()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::ResNum, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
