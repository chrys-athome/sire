#ifndef SIREMOL_MOLCGATOMID_H
#define SIREMOL_MOLCGATOMID_H

#include "atomid.h"
#include "cutgroupid.h"
#include "moleculeid.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireMol
{
class MolCGAtomID;
}

QDataStream& operator<<(QDataStream&, const SireMol::MolCGAtomID&);
QDataStream& operator>>(QDataStream&, SireMol::MolCGAtomID&);

uint qHash(const SireMol::MolCGAtomID&);

namespace SireMol
{

using boost::tuple;

/** A MolCGAtomID is the ID number that uniquely identifies an Atom within 
    an identified CutGroup. It is the combination of the CutGroupID with an 
    index number into the CutGroup of the Atom */
class SIREMOL_EXPORT MolCGAtomID
{

friend QDataStream& ::operator>>(QDataStream&, MolCGAtomID&);

public:
    MolCGAtomID(MoleculeID molid = MoleculeID(0), 
                CutGroupID cgid = CutGroupID(0), AtomID atmid = AtomID(0)) 
              : _molid(molid), _cgid(cgid), _atmid(atmid)
    {}
    
    MolCGAtomID( const tuple<MoleculeID,CutGroupID,AtomID> &t )
              : _molid( t.get<0>() ), _cgid( t.get<1>() ), _atmid( t.get<2>() )
    {}
    
    MolCGAtomID(const MolCGAtomID &other) 
              : _molid(other._molid), _cgid(other._cgid), _atmid(other._atmid)
    {}
    
    ~MolCGAtomID()
    {}
    
    MoleculeID moleculeID() const
    {
        return _molid;
    }
    
    CutGroupID cutGroupID() const
    {
        return _cgid;
    }
    
    AtomID atomID() const
    {
        return _atmid;
    }
    
    bool operator==(const MolCGAtomID &other) const
    {
        return _molid == other._molid and _cgid == other._cgid and _atmid == other._atmid;
    }
    
    bool operator!=(const MolCGAtomID &other) const
    {
        return _molid != other._molid or _cgid != other._cgid or _atmid != other._atmid;
    }
    
    bool operator<(const MolCGAtomID &other) const
    {
        return _molid < other._molid or
               (_molid == other._molid and ( _cgid < other._cgid or
                                           ( _cgid == other._cgid and _atmid < other._atmid ) ) );
    }
    
    bool operator<=(const MolCGAtomID &other) const
    {
        return _molid < other._molid or
               (_molid == other._molid and ( _cgid < other._cgid or
                                           ( _cgid == other._cgid and _atmid <= other._atmid ) ) );
    }
    
    bool operator>(const MolCGAtomID &other) const
    {
        return _molid > other._molid or
               (_molid == other._molid and ( _cgid > other._cgid or
                                           ( _cgid == other._cgid and _atmid > other._atmid ) ) );
    }
    
    bool operator>=(const MolCGAtomID &other) const
    {
        return _molid > other._molid or
               (_molid == other._molid and ( _cgid > other._cgid or
                                           ( _cgid == other._cgid and _atmid >= other._atmid ) ) );
    }
    
    QString toString() const;
    
private:
    MoleculeID _molid;
    CutGroupID _cgid;
    AtomID _atmid;
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::MolCGAtomID, Q_MOVABLE_TYPE);

/** Hash this ID type */
inline uint qHash(const SireMol::MolCGAtomID &id)
{
    //return the lowest 11 bits for the molecule and CutGroup, then 10 bits for the 
    //atom, as
    //    33222222222211111111110000000000
    //    10987654321098765432109876543210
    //    mmmmmmmmmmmcccccccccccaaaaaaaaaa  (m = MoleculeID bit, c = CutGroupID bit, a = AtomID bit)
    return ( qHash(id.moleculeID()) << 21) | 
           ( (qHash(id.cutGroupID()) << 10) & 0x001FFC00 ) | 
           ( qHash(id.atomID()) & 0x000003FF);
}

SIRE_END_HEADER

#endif
