#ifndef SIREVOL_CARTESIAN_H
#define SIREVOL_CARTESIAN_H

#include "space.h"

SIRE_BEGIN_HEADER

namespace SireVol
{
class Cartesian;
}

QDataStream& operator<<(QDataStream&, const SireVol::Cartesian&);
QDataStream& operator>>(QDataStream&, SireVol::Cartesian&);

namespace SireVol
{

/**
This class overloads SimVolume to provide an infinite Cartesian (3-dimensional, orthoganol dimensions) volume. This corresponds to a traditional gas-phase or no-boundary system.

@author Christopher Woods
*/
class SIREVOL_EXPORT Cartesian : public SpaceBase
{

friend QDataStream& ::operator<<(QDataStream&, const Cartesian&);
friend QDataStream& ::operator>>(QDataStream&, Cartesian&);

public:
    Cartesian();
    Cartesian(const Cartesian &other);

    ~Cartesian();

    static const char* typeName()
    {
        return "SireVol::Cartesian";
    }

    const char* what() const
    {
        return Cartesian::typeName();
    }

    SpaceBase* clone() const
    {
        return new Cartesian(*this);
    }

    double calcDist(const CoordGroup &group, DistMatrix &mat) const;
    double calcDist2(const CoordGroup &group, DistMatrix &mat) const;
    double calcInvDist(const CoordGroup &group, DistMatrix &mat) const;
    double calcInvDist2(const CoordGroup &group, DistMatrix &mat) const;

    double calcDist(const CoordGroup &group1, const CoordGroup &group2,
                    DistMatrix &mat) const;

    double calcDist2(const CoordGroup &group1, const CoordGroup &group2,
                     DistMatrix &mat) const;

    double calcInvDist(const CoordGroup &group1, const CoordGroup &group2,
                       DistMatrix &mat) const;

    double calcInvDist2(const CoordGroup &group1, const CoordGroup &group2,
                        DistMatrix &mat) const;

    bool beyond(double dist, const CoordGroup &group0,
                const CoordGroup &group1) const;
};

}

Q_DECLARE_METATYPE(SireVol::Cartesian)

SIRE_END_HEADER

#endif
