#ifndef SIREMOL_CUTGROUPID_H
#define SIREMOL_CUTGROUPID_H

#include "id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This ID number is used to identify a CutGroup.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT CutGroupID : public ID<CutGroupID>
{

public:
    CutGroupID() : ID<CutGroupID>()
    {}
    
    CutGroupID(quint32 id) : ID<CutGroupID>(id)
    {}
    
    CutGroupID(const CutGroupID &other) : ID<CutGroupID>(other)
    {}
    
    ~CutGroupID()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::CutGroupID, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
