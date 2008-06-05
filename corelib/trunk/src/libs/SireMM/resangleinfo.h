#ifndef SIREMM_RESANGLEINFO_H
#define SIREMM_RESANGLEINFO_H

#include "internaldatatypes.h"
#include "resinternalinfo.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
class ResAngleInfo;
}

QDataStream& operator<<(QDataStream&, const SireMM::ResAngleInfo&);
QDataStream& operator>>(QDataStream&, SireMM::ResAngleInfo&);

namespace SireMM
{

class MolAngleInfo;

/**
This class holds the metadata about the angles in a residue - the MolAngleInfo class holds the information about the angles in the molecule.

@author Christopher Woods
*/
class SIREMM_EXPORT ResAngleInfo : public ResInternalInfo<Angle>
{
public:
    ResAngleInfo();
    ResAngleInfo(const MolAngleInfo &molinfo, ResNum resnum);

    ResAngleInfo(const ResInternalInfo<Angle> &other);

    ~ResAngleInfo();

    int nAngles() const;
    
    int nIntraAngles() const;
    int nInterAngles() const;
    
    const_iterator angles() const;
    const_iterator intraAngles() const;
    const_iterator interAngles() const;
};

}

Q_DECLARE_METATYPE(SireMM::ResAngleInfo)

SIRE_END_HEADER

#endif
