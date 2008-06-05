/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "rigidbodymc.h"

#include "SireSystem/simsystem.h"

#include "SireMol/moleculemover.h"
#include "SireMol/propertyextractor.h"

#include "SireMaths/quaternion.h"

#include "SireUnits/units.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireMove;
using namespace SireSystem;
using namespace SireMol;
using namespace SireUnits;
using namespace SireStream;
using namespace SireMaths;

/////////////
///////////// Implementation of RigidBodyMC::CheckPoint
/////////////

/** Null constructor */
RigidBodyMC::CheckPoint::CheckPoint()
{}

/** Construct using the passed system checkpoint and sampler */
RigidBodyMC::CheckPoint::CheckPoint(const SireSystem::CheckPoint &checkpoint,
                                    const Sampler &sampler)
            : sys_ckpt(checkpoint),
              smplr_ckpt(sampler)
{}

/** Copy constructor */
RigidBodyMC::CheckPoint::CheckPoint(const CheckPoint &other)
            : sys_ckpt(other.sys_ckpt),
              smplr_ckpt(other.smplr_ckpt)
{}

/** Destructor */
RigidBodyMC::CheckPoint::~CheckPoint()
{}

/////////////
///////////// Implementation of RigidBodyMC
/////////////

static const RegisterMetaType<RigidBodyMC> r_rbmc;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds,
                                        const RigidBodyMC &rbmc)
{
    writeHeader(ds, r_rbmc, 1);

    SharedDataStream sds(ds);

    sds << rbmc._sampler
        << rbmc.adel << double(rbmc.rdel)
        << static_cast<const MonteCarlo&>(rbmc);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, RigidBodyMC &rbmc)
{
    VersionID v = readHeader(ds, r_rbmc);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        double rdel;

        sds >> rbmc._sampler
            >> rbmc.adel >> rdel
            >> static_cast<MonteCarlo&>(rbmc);
            
        rbmc.rdel = SireUnits::Dimension::Angle(rdel);
    }
    else
        throw version_error(v, "1", r_rbmc, CODELOC);

    return ds;
}

/** Null constructor */
RigidBodyMC::RigidBodyMC(const RanGenerator &generator)
            : MonteCarlo(generator),
              adel( 0.15 * angstrom ),
              rdel( 15 * degrees )
{}

/** Construct a move that moves molecules returned by the sampler 'sampler',
    using the supplied optional random number generator */
RigidBodyMC::RigidBodyMC(const Sampler &sampler, const RanGenerator &generator)
            : MonteCarlo(generator),
              _sampler(sampler),
              adel( 0.15 * angstrom ),
              rdel( 15 * degrees )
{}

/** Construct a move that moves molecules returned by the sampler 'sampler',
    performing moves with a maximum translation of 'max_translation' and a
    maximum rotation of 'max_rotation' */
RigidBodyMC::RigidBodyMC(const Sampler &sampler,
                         Dimension::Length max_translation,
                         Dimension::Angle max_rotation,
                         const RanGenerator &generator)
            : MonteCarlo(generator),
              _sampler(sampler),
              adel(max_translation),
              rdel(max_rotation)
{}

/** Copy constructor */
RigidBodyMC::RigidBodyMC(const RigidBodyMC &other)
            : MonteCarlo(other),
              _sampler(other._sampler),
              adel(other.adel), rdel(other.rdel)
{}

/** Destructor */
RigidBodyMC::~RigidBodyMC()
{}

/** Copy assignment operator */
RigidBodyMC& RigidBodyMC::operator=(const RigidBodyMC &other)
{
    if (this != &other)
    {
        _sampler = other._sampler;
        adel = other.adel;
        rdel = other.rdel;
        MonteCarlo::operator=(other);
    }
    
    return *this;
}

/** Set the maximum delta for any translation */
void RigidBodyMC::setMaximumTranslation(Dimension::Length max_translation)
{
    adel = max_translation;
}

/** Set the maximum delta for any rotation */
void RigidBodyMC::setMaximumRotation(Dimension::Angle max_rotation)
{
    rdel = max_rotation;
}

/** Return the maximum translation for each move */
Dimension::Length RigidBodyMC::maximumTranslation() const
{
    return Dimension::Length(adel);
}

/** Return the maximum rotation for each move */
Dimension::Angle RigidBodyMC::maximumRotation() const
{
    return rdel;
}

/** Checkpoint this move */
RigidBodyMC::CheckPoint RigidBodyMC::checkPoint(QuerySystem &system) const
{
    return RigidBodyMC::CheckPoint( system.checkPoint(), _sampler );
}

/** Rollback to a previous checkpoint */
void RigidBodyMC::rollBack(SimSystem &system,
                           const RigidBodyMC::CheckPoint &checkpoint)
{
    system.rollBack(checkpoint.system());
    _sampler = checkpoint.sampler();
}

/** Assert that this move is compatible with the passed system - this is 
    to try and catch any silly errors before the simulation starts... */
void RigidBodyMC::assertCompatibleWith(QuerySystem &system) const
{
    _sampler.assertCompatibleWith(system);
    MonteCarlo::assertCompatibleWith(system);
}

/** Attempt a single rigid-body move */
void RigidBodyMC::move(SimSystem &system)
{
    //get the current total energy of the system
    double old_nrg = energy(system);
    
    //checkpoint the simulation - we only need to save
    //the system and our sampler...
    RigidBodyMC::CheckPoint checkpoint = checkPoint(system);

    try
    {
        //randomly select a molecule to move
        tuple<PartialMolecule,double> mol_and_bias = _sampler.sample(system);

        double old_bias = mol_and_bias.get<1>();

        //now translate and rotate the molecule
        Vector delta = rangenerator.vectorOnSphere(adel);

        Quaternion rotdelta( rdel * rangenerator.rand(),
                             rangenerator.vectorOnSphere() );

        const PartialMolecule &oldmol = mol_and_bias.get<0>();

        PartialMolecule newmol = oldmol.move().translateAndRotate(
                                                delta,
                                                rotdelta,
                                                oldmol.extract().geometricCenter()
                                               );

        //update the simulation with the new coordinates
        newmol = system.change(newmol);

        //calculate the energy of the system
        double new_nrg = energy(system);

        //get the new bias on this molecule
        double new_bias = _sampler.probabilityOf(newmol, system);

        //accept or reject the move based on the change of energy
        //and the biasing factors
        if (not this->test(new_nrg, old_nrg, new_bias, old_bias))
            //the move has been rejected - reset the state
            rollBack(system, checkpoint);
    }
    catch(...)
    {
        //rollback to before the move
        rollBack(system, checkpoint);

        //rethrow the exception
        throw;
    }
}
