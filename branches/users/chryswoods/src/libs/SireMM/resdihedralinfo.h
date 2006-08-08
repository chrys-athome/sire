#ifndef SIREMM_RESDIHEDRALINFO_H
#define SIREMM_RESDIHEDRALINFO_H

#include "internaldatatypes.h"
#include "resinternalinfo.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
class ResDihedralInfo;
}

QDataStream& operator<<(QDataStream&, const SireMM::ResDihedralInfo&);
QDataStream& operator>>(QDataStream&, SireMM::ResDihedralInfo&);

namespace SireMM
{

class MolDihedralInfo;

/**
This class holds the metadata about the dihedrals in a residue - the MolDihedralInfo class holds the information about the dihedrals in the molecule.

@author Christopher Woods
*/
class SIREMM_EXPORT ResDihedralInfo : public ResInternalInfo<Dihedral>
{
public:
    ResDihedralInfo();
    ResDihedralInfo(const MolDihedralInfo &molinfo, ResNum resnum);

    ResDihedralInfo(const ResInternalInfo<Dihedral> &other);

    ~ResDihedralInfo();

    int nDihedrals() const;
    
    int nIntraDihedrals() const;
    int nInterDihedrals() const;
    
    const_iterator dihedrals() const;
    const_iterator intraDihedrals() const;
    const_iterator interDihedrals() const;
};

}

Q_DECLARE_METATYPE(SireMM::ResDihedralInfo)

SIRE_END_HEADER

#endif
