#ifndef SIREMOL_INDEX_H
#define SIREMOL_INDEX_H

#include "id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This ID number is used to represent a specific index in 
    an array, or the index of an item in a group.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT Index : public ID<Index>
{

public:
    Index() : ID<Index>()
    {}
    
    Index(quint32 id) : ID<Index>(id)
    {}
    
    Index(const Index &other) : ID<Index>(other)
    {}
    
    ~Index()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::Index, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
