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

#include <QHash>

#include "montecarlo.h"
#include "sampler.h"

#include "SireCAS/symbol.h"
#include "SireMaths/angle.h"

#include "SireSystem/checkpoint.h"

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

using SireCAS::Symbol;

/** This class implements a rigid body Monte Carlo move that
    may be applied to a random molecule within a MoleculeGroup

    @author Christopher Woods
*/
class SIREMOVE_EXPORT RigidBodyMC : public MonteCarlo
{

friend QDataStream& ::operator<<(QDataStream&, const RigidBodyMC&);
friend QDataStream& ::operator>>(QDataStream&, RigidBodyMC&);

public:
    RigidBodyMC(const RanGenerator &generator = RanGenerator());

    RigidBodyMC(const Sampler &sampler,
                const RanGenerator &generator = RanGenerator());

    RigidBodyMC(const Sampler &sampler,
                double max_translation,
                const SireMaths::Angle &max_rotation,
                const RanGenerator &generator = RanGenerator());

    RigidBodyMC(const RigidBodyMC &other);

    ~RigidBodyMC();

    static const char* typeName()
    {
        return "SireMove::RigidBodyMC";
    }

    const char* what() const
    {
        return RigidBodyMC::typeName();
    }

    RigidBodyMC* clone() const
    {
        return new RigidBodyMC(*this);
    }

    void setMaximumTranslation(double max_translation);
    void setMaximumRotation(const SireMaths::Angle &max_rotation);

    void setComponent(const Symbol &energy);

    void move(SimSystem &system);

protected:
    class CheckPoint
    {
    public:
        CheckPoint();

        CheckPoint(const SireSystem::CheckPoint &checkpoint,
                   const Sampler &sampler);

        CheckPoint(const CheckPoint &other);

        ~CheckPoint();

        const SireMove::CheckPoint& system() const
        {
            return sys_ckpt;
        }

        const Sampler& sampler() const
        {
            return smplr_ckpt;
        }

    private:
        /** Checkpoint of the system */
        SireSystem::CheckPoint sys_ckpt;

        /** Checkpoint of the sampler */
        Sampler smplr_ckpt;
    };

    RigidBodyMC::CheckPoint checkPoint(QuerySystem &system) const;
    void rollBack(const RigidBodyMC::CheckPoint &checkpoint);

private:
    /** The sampler used to select random molecules from the
        MoleculeGroup */
    Sampler _sampler;

    /** The energy component evaluated during the move */
    Symbol nrg_component;

    /** The maximum translation */
    double adel;

    /** The maximum rotation */
    SireMaths::Angle rdel;
};

}

Q_DECLARE_METATYPE(SireMove::RigidBodyMC);

SIRE_END_HEADER

#endif
