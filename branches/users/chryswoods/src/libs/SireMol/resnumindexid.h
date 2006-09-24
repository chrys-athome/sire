#ifndef SIREMOL_RESNUMINDEXID_H
#define SIREMOL_RESNUMINDEXID_H

#include "index.h"
#include "resnum.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireMol
{
class ResNumIndexID;
}

QDataStream& operator<<(QDataStream&, const SireMol::ResNumIndexID&);
QDataStream& operator>>(QDataStream&, SireMol::ResNumIndexID&);

uint qHash(const SireMol::ResNumIndexID&);

namespace SireMol
{

using boost::tuple;

/** A ResNumIndexID is the ID number that uniquely identifies an Atom within 
    an identified Residue (residue identified via residue number). It is the 
    combination of the ResNum with an index number into the residue of the Atom 
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT ResNumIndexID
{

friend QDataStream& ::operator>>(QDataStream&, ResNumIndexID&);

public:
    ResNumIndexID(ResNum resnum = ResNum(0), Index idx = Index(0)) 
            : _resnum(resnum), _idx(idx)
    {}
    
    ResNumIndexID(const tuple<ResNum,Index> &t)
              : _resnum( t.get<0>() ), _idx( t.get<1>() )
    {}
    
    ResNumIndexID(const ResNumIndexID &other) : _resnum(other._resnum), _idx(other._idx)
    {}
    
    ~ResNumIndexID()
    {}
    
    ResNum resNum() const
    {
        return _resnum;
    }
    
    Index index() const
    {
        return _idx;
    }
    
    bool operator==(const ResNumIndexID &other) const
    {
        return _resnum == other._resnum and _idx == other._idx;
    }
    
    bool operator!=(const ResNumIndexID &other) const
    {
        return _resnum != other._resnum or _idx != other._idx;
    }
    
    bool operator<(const ResNumIndexID &other) const
    {
        return _resnum < other._resnum or 
               (_resnum == other._resnum and _idx < other._idx);
    }
    
    bool operator<=(const ResNumIndexID &other) const
    {
        return _resnum < other._resnum or
               (_resnum == other._resnum and _idx <= other._idx);
    }
    
    bool operator>(const ResNumIndexID &other) const
    {
        return _resnum > other._resnum or
               (_resnum == other._resnum and _idx > other._idx);
    }
    
    bool operator>=(const ResNumIndexID &other) const
    {
        return _resnum > other._resnum or
               (_resnum == other._resnum and _idx >= other._idx);
    }
    
    QString toString() const;
    
private:
    ResNum _resnum;
    Index _idx;
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::ResNumIndexID, Q_MOVABLE_TYPE);

/** Hash this ID type */
inline uint qHash(const SireMol::ResNumIndexID &id)
{
    return ( (qHash(id.resNum())) << 16 ) | 
           ( (qHash(id.index())) | 0x0000FFFF );
}

SIRE_END_HEADER

#endif
