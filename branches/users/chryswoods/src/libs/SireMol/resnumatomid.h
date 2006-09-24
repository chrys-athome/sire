#ifndef SIREMOL_RESNUMATOMID_H
#define SIREMOL_RESNUMATOMID_H

#include "atomid.h"
#include "resnum.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireMol
{
class ResNumAtomID;
}

QDataStream& operator<<(QDataStream&, const SireMol::ResNumAtomID&);
QDataStream& operator>>(QDataStream&, SireMol::ResNumAtomID&);

uint qHash(const SireMol::ResNumAtomID&);

namespace SireMol
{

using boost::tuple;

/** A ResNumAtomID is the ID number that uniquely identifies an Atom within
    an identified Residue (residue identified via residue number). It is the
    combination of the ResNum with an index number into the residue of the Atom

    @author Christopher Woods
*/
class SIREMOL_EXPORT ResNumAtomID
{

friend QDataStream& ::operator>>(QDataStream&, ResNumAtomID&);

public:
    ResNumAtomID(ResNum resnum = ResNum(0), AtomID atmid = AtomID(0)) 
              : _resnum(resnum), _atmid(atmid)
    {}

    ResNumAtomID(const tuple<ResNum,AtomID> &t)
              : _resnum( t.get<0>() ), _atmid( t.get<1>() )
    {}

    ResNumAtomID(const ResNumAtomID &other) : _resnum(other._resnum), _atmid(other._atmid)
    {}

    ~ResNumAtomID()
    {}

    ResNum resNum() const
    {
        return _resnum;
    }

    AtomID atomID() const
    {
        return _atmid;
    }

    bool operator==(const ResNumAtomID &other) const
    {
        return _resnum == other._resnum and _atmid == other._atmid;
    }

    bool operator!=(const ResNumAtomID &other) const
    {
        return _resnum != other._resnum or _atmid != other._atmid;
    }

    bool operator<(const ResNumAtomID &other) const
    {
        return _resnum < other._resnum or
               (_resnum == other._resnum and _atmid < other._atmid);
    }

    bool operator<=(const ResNumAtomID &other) const
    {
        return _resnum < other._resnum or
               (_resnum == other._resnum and _atmid <= other._atmid);
    }

    bool operator>(const ResNumAtomID &other) const
    {
        return _resnum > other._resnum or
               (_resnum == other._resnum and _atmid > other._atmid);
    }

    bool operator>=(const ResNumAtomID &other) const
    {
        return _resnum > other._resnum or
               (_resnum == other._resnum and _atmid >= other._atmid);
    }

    QString toString() const;

private:
    ResNum _resnum;
    AtomID _atmid;
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::ResNumAtomID, Q_MOVABLE_TYPE);

/** Hash this ID type */
inline uint qHash(const SireMol::ResNumAtomID &id)
{
    return ( (qHash(id.resNum())) << 16 ) |
           ( (qHash(id.atomID())) | 0x0000FFFF );
}

SIRE_END_HEADER

#endif
