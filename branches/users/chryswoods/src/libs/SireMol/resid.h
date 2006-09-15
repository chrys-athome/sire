#ifndef SIREMOL_RESID_H
#define SIREMOL_RESID_H

#include "id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This is the ID type for a residue (index into an array of residues)
    Be careful not to confuse this with ResNum, which is the identifying
    number given to the residue by the user, and used internally to
    identify the residue
*/
class SIREMOL_EXPORT ResID : public ID<ResID>
{

public:
    ResID() : ID<ResID>()
    {}

    explicit ResID(quint32 id) : ID<ResID>(id)
    {}

    ResID(const ResID &other) : ID<ResID>(other)
    {}

    ~ResID()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::ResID, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
