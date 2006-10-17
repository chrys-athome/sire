#ifndef SIREMM_RESBONDINFO_H
#define SIREMM_RESBONDINFO_H

#include "internaldatatypes.h"
#include "resinternalinfo.hpp"
#include "SireMol/bond.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class ResBondInfo;
}

QDataStream& operator<<(QDataStream&, const SireMM::ResBondInfo&);
QDataStream& operator>>(QDataStream&, SireMM::ResBondInfo&);

namespace SireMM
{

class MolBondInfo;

/**
This class is to MolBondInfo what ResidueInfo is to MoleculeInfo, namely this contains the bonding metadata for a residue, while MolBondInfo contains the bonding metadata for the molecule.

@author Christopher Woods
*/
class SIREMM_EXPORT ResBondInfo : public ResInternalInfo<Bond>
{
public:
    ResBondInfo();
    ResBondInfo(const MolBondInfo &molinfo, ResNum resnum);

    ResBondInfo(const ResInternalInfo<Bond> &other);

    ~ResBondInfo();

    int nBonds() const;
    
    int nIntraBonds() const;
    int nInterBonds() const;
    
    const_iterator bonds() const;
    const_iterator intraBonds() const;
    const_iterator interBonds() const;
};

}

Q_DECLARE_METATYPE(SireMM::ResBondInfo)

SIRE_END_HEADER

#endif
