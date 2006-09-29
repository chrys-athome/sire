#ifndef SIREMOL_CGNUMATOMID_H
#define SIREMOL_CGNUMATOMID_H

#include "atomid.h"
#include "cutgroupnum.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireMol
{
class CGNumAtomID;
}

QDataStream& operator<<(QDataStream&, const SireMol::CGNumAtomID&);
QDataStream& operator>>(QDataStream&, SireMol::CGNumAtomID&);

uint qHash(const SireMol::CGNumAtomID&);

namespace SireMol
{

using boost::tuple;

/** A CGNumAtomID is the ID number that uniquely identifies an Atom within
    an identified CutGroup. It is the combination of the CutGroupNum with an
    index number into the CutGroup of the Atom */
class SIREMOL_EXPORT CGNumAtomID
{

friend QDataStream& ::operator>>(QDataStream&, CGNumAtomID&);

public:
    CGNumAtomID(CutGroupNum cgid = CutGroupNum(0), AtomID atmid = AtomID(0))
          : _cgnum(cgid), _atmid(atmid)
    {}

    CGNumAtomID(const tuple<CutGroupNum,AtomID> &t)
              : _cgnum( t.get<0>() ), _atmid( t.get<1>() )
    {}

    CGNumAtomID(const CGNumAtomID &other) : _cgnum(other._cgnum), _atmid(other._atmid)
    {}

    ~CGNumAtomID()
    {}

    CutGroupNum cutGroupNum() const
    {
        return _cgnum;
    }

    AtomID atomID() const
    {
        return _atmid;
    }

    bool operator==(const CGNumAtomID &other) const
    {
        return _cgnum == other._cgnum and _atmid == other._atmid;
    }

    bool operator!=(const CGNumAtomID &other) const
    {
        return _cgnum != other._cgnum or _atmid != other._atmid;
    }

    bool operator<(const CGNumAtomID &other) const
    {
        return _cgnum < other._cgnum or
               (_cgnum == other._cgnum and _atmid < other._atmid);
    }

    bool operator<=(const CGNumAtomID &other) const
    {
        return _cgnum < other._cgnum or
               (_cgnum == other._cgnum and _atmid <= other._atmid);
    }

    bool operator>(const CGNumAtomID &other) const
    {
        return _cgnum > other._cgnum or
               (_cgnum == other._cgnum and _atmid > other._atmid);
    }

    bool operator>=(const CGNumAtomID &other) const
    {
        return _cgnum > other._cgnum or
               (_cgnum == other._cgnum and _atmid >= other._atmid);
    }

    QString toString() const;

private:
    CutGroupNum _cgnum;
    AtomID _atmid;
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::CGNumAtomID, Q_MOVABLE_TYPE);

/** Hash this ID type */
inline uint qHash(const SireMol::CGNumAtomID &id)
{
    return ( (qHash(id.cutGroupNum())) << 16 ) |
           ( (qHash(id.atomID())) | 0x0000FFFF );
}

SIRE_END_HEADER

#endif
