#ifndef SIREMOL_CGATOMID_H
#define SIREMOL_CGATOMID_H

#include "atomid.h"
#include "cutgroupid.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireMol
{
class CGAtomID;
}

QDataStream& operator<<(QDataStream&, const SireMol::CGAtomID&);
QDataStream& operator>>(QDataStream&, SireMol::CGAtomID&);

uint qHash(const SireMol::CGAtomID&);

namespace SireMol
{

using boost::tuple;

/** A CGAtomID is the ID number that uniquely identifies an Atom within 
    an identified CutGroup. It is the combination of the CutGroupID with an 
    index number into the CutGroup of the Atom */
class SIREMOL_EXPORT CGAtomID
{

friend QDataStream& ::operator>>(QDataStream&, CGAtomID&);

public:
    explicit CGAtomID(CutGroupID cgid = CutGroupID(0), AtomID atmid = AtomID(0)) 
          : _cgid(cgid), _atmid(atmid)
    {}
    
    CGAtomID(const tuple<CutGroupID,AtomID> &t)
              : _cgid( t.get<0>() ), _atmid( t.get<1>() )
    {}
    
    CGAtomID(const CGAtomID &other) : _cgid(other._cgid), _atmid(other._atmid)
    {}
    
    ~CGAtomID()
    {}
    
    CutGroupID cutGroupID() const
    {
        return _cgid;
    }
    
    AtomID atomID() const
    {
        return _atmid;
    }
    
    bool operator==(const CGAtomID &other) const
    {
        return _cgid == other._cgid and _atmid == other._atmid;
    }
    
    bool operator!=(const CGAtomID &other) const
    {
        return _cgid != other._cgid or _atmid != other._atmid;
    }
    
    bool operator<(const CGAtomID &other) const
    {
        return _cgid < other._cgid or 
               (_cgid == other._cgid and _atmid < other._atmid);
    }
    
    bool operator<=(const CGAtomID &other) const
    {
        return _cgid < other._cgid or
               (_cgid == other._cgid and _atmid <= other._atmid);
    }
    
    bool operator>(const CGAtomID &other) const
    {
        return _cgid > other._cgid or
               (_cgid == other._cgid and _atmid > other._atmid);
    }
    
    bool operator>=(const CGAtomID &other) const
    {
        return _cgid > other._cgid or
               (_cgid == other._cgid and _atmid >= other._atmid);
    }
    
    QString toString() const;
    
private:
    CutGroupID _cgid;
    AtomID _atmid;
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::CGAtomID, Q_MOVABLE_TYPE);

/** Hash this ID type */
inline uint qHash(const SireMol::CGAtomID &id)
{
    return ( (qHash(id.cutGroupID())) << 16 ) | 
           ( (qHash(id.atomID())) | 0x0000FFFF );
}

SIRE_END_HEADER

#endif
