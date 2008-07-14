/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#ifndef SIREMOVE_RIGIDBODYMC_H
#define SIREMOVE_RIGIDBODYMC_H

#include "montecarlo.h"
#include "sampler.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class RigidBodyMC;
}

QDataStream& operator<<(QDataStream&, const SireMove::RigidBodyMC&);
QDataStream& operator>>(QDataStream&, SireMove::RigidBodyMC&);

namespace SireSystem
{
class SimSystem;
class CheckPoint;
}

namespace SireMol
{
class MoleculeID;
class MoleculeGroup;
class PartialMolecule;
}

namespace SireMove
{

class Sampler;

using SireSystem::SimSystem;

using SireMol::MoleculeGroup;
using SireMol::MoleculeID;

using SireMol::PartialMolecule;

/** This class implements a rigid body Monte Carlo move that
    may be applied to a random molecule within a MoleculeGroup

    @author Christopher Woods
*/
class SIREMOVE_EXPORT RigidBodyMC 
        : public SireBase::ConcreteProperty<RigidBodyMC,MonteCarlo>
{

friend QDataStream& ::operator<<(QDataStream&, const RigidBodyMC&);
friend QDataStream& ::operator>>(QDataStream&, RigidBodyMC&);

public:
    RigidBodyMC();

    RigidBodyMC(const MolGroup &molgroup);
    RigidBodyMC(const Sampler &sampler);

    RigidBodyMC(const RigidBodyMC &other);

    ~RigidBodyMC();

    RigidBodyMC& operator=(const RigidBodyMC &other);

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<RigidBodyMC>() );
    }

    RigidBodyMC* clone() const
    {
        return new RigidBodyMC(*this);
    }

    void setSampler(const Sampler &sampler);
    void setSampler(const MolGroup &molgroup);

    const SamplerBase& sampler() const;
    const MolGroup& moleculeGroup() const;

    void setMaximumTranslation(SireUnits::Dimension::Length max_translation);
    void setMaximumRotation(SireUnits::Dimension::Angle max_rotation);

    SireUnits::Dimension::Length maximumTranslation() const;
    SireUnits::Dimension::Angle maximumRotation() const;

    void move(System &system, int nmoves, bool record_stats);

private:
    /** The sampler used to select random molecules for the move */
    Sampler smplr;

    /** The maximum translation */
    double adel;

    #ifndef SKIP_BROKEN_GCCXML_PARTS
    /** The maximum rotation */
    SireUnits::Dimension::Angle rdel;
    #endif
};

}

Q_DECLARE_METATYPE(SireMove::RigidBodyMC);

SIRE_EXPOSE_CLASS( SireMove::RigidBodyMC )

SIRE_END_HEADER

#endif
