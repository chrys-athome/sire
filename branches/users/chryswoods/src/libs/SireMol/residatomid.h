#ifndef SIREMOL_RESIDATOMID_H
#define SIREMOL_RESIDATOMID_H

#include "atomid.h"
#include "resid.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireMol
{
class ResIDAtomID;
}

QDataStream& operator<<(QDataStream&, const SireMol::ResIDAtomID&);
QDataStream& operator>>(QDataStream&, SireMol::ResIDAtomID&);

uint qHash(const SireMol::ResIDAtomID&);

namespace SireMol
{

using boost::tuple;

/** A ResIDAtomID is the ID number that uniquely identifies an Atom within
    an identified Residue (residue identified via residue number). It is the
    combination of the ResID with an index number into the residue of the Atom

    @author Christopher Woods
*/
class SIREMOL_EXPORT ResIDAtomID
{

friend QDataStream& ::operator>>(QDataStream&, ResIDAtomID&);

public:
    ResIDAtomID(ResID resid=0, AtomID atmid=0) : _resid(resid), _atmid(atmid)
    {}

    ResIDAtomID(const tuple<ResID,AtomID> &t)
              : _resid( t.get<0>() ), _atmid( t.get<1>() )
    {}

    ResIDAtomID(const ResIDAtomID &other) : _resid(other._resid), _atmid(other._atmid)
    {}

    ~ResIDAtomID()
    {}

    ResID resID() const
    {
        return _resid;
    }

    AtomID atomID() const
    {
        return _atmid;
    }

    bool operator==(const ResIDAtomID &other) const
    {
        return _resid == other._resid and _atmid == other._atmid;
    }

    bool operator!=(const ResIDAtomID &other) const
    {
        return _resid != other._resid or _atmid != other._atmid;
    }

    bool operator<(const ResIDAtomID &other) const
    {
        return _resid < other._resid or
               (_resid == other._resid and _atmid < other._atmid);
    }

    bool operator<=(const ResIDAtomID &other) const
    {
        return _resid < other._resid or
               (_resid == other._resid and _atmid <= other._atmid);
    }

    bool operator>(const ResIDAtomID &other) const
    {
        return _resid > other._resid or
               (_resid == other._resid and _atmid > other._atmid);
    }

    bool operator>=(const ResIDAtomID &other) const
    {
        return _resid > other._resid or
               (_resid == other._resid and _atmid >= other._atmid);
    }

    QString toString() const;

private:
    ResID _resid;
    AtomID _atmid;
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::ResIDAtomID, Q_MOVABLE_TYPE);

/** Hash this ID type */
inline uint qHash(const SireMol::ResIDAtomID &id)
{
    return ( (qHash(id.resID())) << 16 ) |
           ( (qHash(id.atomID())) | 0x0000FFFF );
}

SIRE_END_HEADER

#endif
