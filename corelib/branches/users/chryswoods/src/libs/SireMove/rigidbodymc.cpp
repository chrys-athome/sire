/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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
#include "uniformsampler.h"
#include "ensemble.h"

#include "SireSystem/system.h"

#include "SireMol/partialmolecule.h"

#include "SireVol/space.h"

#include "SireMaths/quaternion.h"

#include "SireUnits/units.h"
#include "SireUnits/temperature.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>
#include <QTime>

using namespace SireMove;
using namespace SireSystem;
using namespace SireMol;
using namespace SireUnits;
using namespace SireVol;
using namespace SireStream;
using namespace SireMaths;

static const RegisterMetaType<RigidBodyMC> r_rbmc;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds,
                                        const RigidBodyMC &rbmc)
{
    writeHeader(ds, r_rbmc, 2);

    SharedDataStream sds(ds);

    sds << rbmc.smplr
        << rbmc.adel << rbmc.rdel
        << rbmc.sync_trans << rbmc.sync_rot
        << static_cast<const MonteCarlo&>(rbmc);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, RigidBodyMC &rbmc)
{
    VersionID v = readHeader(ds, r_rbmc);

    if (v == 2)
    {
        SharedDataStream sds(ds);
        
        sds >> rbmc.smplr
            >> rbmc.adel >> rbmc.rdel
            >> rbmc.sync_trans >> rbmc.sync_rot
            >> static_cast<MonteCarlo&>(rbmc);
    }
    else if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> rbmc.smplr
            >> rbmc.adel >> rbmc.rdel
            >> static_cast<MonteCarlo&>(rbmc);
            
        rbmc.sync_trans = false;
        rbmc.sync_rot = false;
    }
    else
        throw version_error(v, "1,2", r_rbmc, CODELOC);

    return ds;
}

/** Null constructor */
RigidBodyMC::RigidBodyMC() 
            : ConcreteProperty<RigidBodyMC,MonteCarlo>(),
              adel( 0.15 * angstrom ), rdel( 15 * degrees ),
              sync_trans(false), sync_rot(false)
{
    MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
}

/** Construct a move that moves molecules returned by the sampler 'sampler' */
RigidBodyMC::RigidBodyMC(const Sampler &sampler)
            : ConcreteProperty<RigidBodyMC,MonteCarlo>(), 
              smplr(sampler),
              adel( 0.15 * angstrom ),
              rdel( 15 * degrees ),
              sync_trans(false), sync_rot(false)
{
    MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
    smplr.edit().setGenerator( this->generator() );
}

/** Construct a move that moves molecule views from the molecule group 'molgroup',
    selecting views randomly, with each view having an equal chance of
    being chosen */
RigidBodyMC::RigidBodyMC(const MoleculeGroup &molgroup)
            : ConcreteProperty<RigidBodyMC,MonteCarlo>(), 
              smplr( UniformSampler(molgroup) ),
              adel( 0.15 * angstrom ), rdel( 15 * degrees ),
              sync_trans(false), sync_rot(false)
{
    MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
    smplr.edit().setGenerator( this->generator() );
}

/** Copy constructor */
RigidBodyMC::RigidBodyMC(const RigidBodyMC &other)
            : ConcreteProperty<RigidBodyMC,MonteCarlo>(other), 
              smplr(other.smplr),
              adel(other.adel), rdel(other.rdel),
              sync_trans(other.sync_trans), sync_rot(other.sync_rot)
{}

/** Destructor */
RigidBodyMC::~RigidBodyMC()
{}

void RigidBodyMC::_pvt_setTemperature(const Temperature &temperature)
{
    MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
}

/** Copy assignment operator */
RigidBodyMC& RigidBodyMC::operator=(const RigidBodyMC &other)
{
    if (this != &other)
    {
        smplr = other.smplr;
        adel = other.adel;
        rdel = other.rdel;
        sync_trans = other.sync_trans;
        sync_rot = other.sync_rot;
        MonteCarlo::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool RigidBodyMC::operator==(const RigidBodyMC &other) const
{
    return smplr == other.smplr and adel == other.adel and
           rdel == other.rdel and 
           sync_trans == other.sync_trans and sync_rot == other.sync_rot and
           MonteCarlo::operator==(other);
}

/** Comparison operator */
bool RigidBodyMC::operator!=(const RigidBodyMC &other) const
{
    return not this->operator==(other);
}

/** Return a string representation of this move */
QString RigidBodyMC::toString() const
{
    return QObject::tr("RigidBodyMC( maximumTranslation() = %1 A, "
                       "maximumRotation() = %2 degrees "
                       "nAccepted() = %3 nRejected() = %4 )")
                  .arg(this->maximumTranslation().to(angstrom))
                  .arg(this->maximumRotation().to(degrees))
                  .arg(this->nAccepted())
                  .arg(this->nRejected());
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

/** Set the sampler (and contained molecule group) that provides
    the random molecules to be moved. This gives the sampler the
    same random number generator that is used by this move */
void RigidBodyMC::setSampler(const Sampler &sampler)
{
    smplr = sampler;
    smplr.edit().setGenerator( this->generator() );
}

/** Set the sampler to be one that selects views at random 
    from the molecule group 'molgroup' (each view has an
    equal chance of being chosen) */
void RigidBodyMC::setSampler(const MoleculeGroup &molgroup)
{
    smplr = UniformSampler(molgroup);
}

/** Return the sampler that is used to draw random molecules */
const Sampler& RigidBodyMC::sampler() const
{
    return smplr;
}

/** Return the molecule group from which random molecule views 
    are drawn */
const MoleculeGroup& RigidBodyMC::moleculeGroup() const
{
    return smplr->group();
}

/** Set the random number generator used by this move */
void RigidBodyMC::setGenerator(const RanGenerator &rangenerator)
{
    MonteCarlo::setGenerator(rangenerator);
    smplr.edit().setGenerator(this->generator());
}

/** Set whether or not to synchronise translation of all of the views */
void RigidBodyMC::setSynchronisedTranslation(bool on)
{
    sync_trans = on;
}

/** Set whether or not to synchronise rotation of all of the views */
void RigidBodyMC::setSynchronisedRotation(bool on)
{
    sync_rot = on;
}

/** Return whether or not translation of all molecules is synchronised */
bool RigidBodyMC::synchronisedTranslation() const
{
    return sync_trans;
}

/** Return whether or not rotation of all molecules is synchronised */
bool RigidBodyMC::synchronisedRotation() const
{
    return sync_rot;
}

/** This internal function is used to actually move the molecule(s) */
void RigidBodyMC::performMove(System &system, const Space &space,
                              double &old_bias, double &new_bias,
                              const PropertyMap &map)
{
    //update the sampler with the latest version of the molecules
    smplr.edit().updateFrom(system);

    //get the random amounts by which to translate and
    //rotate the molecule(s)
    const Vector delta = generator().vectorOnSphere(adel);

    const Quaternion rotdelta( rdel * generator().rand(),
                               generator().vectorOnSphere() );

    if ( (not sync_trans) and (not sync_rot) )
    {
        //randomly select a molecule to move
        tuple<PartialMolecule,double> mol_and_bias = smplr.read().sample();

        const PartialMolecule &oldmol = mol_and_bias.get<0>();
        old_bias = mol_and_bias.get<1>();

        //perform the move - need to map to the infinite cartesian
        //space, make the move, and then convert back again
        PartialMolecule newmol = oldmol.move()
                                       .toCartesian(space, map)
                                       .rotate(rotdelta,
                                               oldmol.evaluate().centerOfGeometry(),
                                               map)
                                       .translate(delta, map)
                                       .fromCartesian(space,map)
                                       .commit();

        //update the system with the new coordinates
        system.update(newmol);

        //get the new bias on this molecule
        smplr.edit().updateFrom(system);

        new_bias = smplr.read().probabilityOf(newmol);
    }
    else if (sync_trans)
    {
        if (sync_rot)
        {
            //translate and rotate all molecules
            old_bias = 1;
            new_bias = 1;

            const Molecules &molecules = smplr.read().group().molecules();

            Molecules new_molecules = molecules;

            for (Molecules::const_iterator it = molecules.constBegin();
                 it != molecules.constEnd();
                 ++it)
            {
                //perform the move - need to map to the infinite cartesian
                //space, make the move, and then convert back again
                PartialMolecule newmol = it->move()
                                            .toCartesian(space, map)
                                            .rotate(rotdelta,
                                                    it->evaluate().centerOfGeometry(),
                                                    map)
                                            .translate(delta, map)
                                            .fromCartesian(space,map)
                                            .commit();
                
                new_molecules.update(newmol);
            }            
            
            system.update(new_molecules);            
        }
        else
        {
            //translate all molecules
            const Molecules &molecules = smplr.read().group().molecules();

            Molecules new_molecules = molecules;

            for (Molecules::const_iterator it = molecules.constBegin();
                 it != molecules.constEnd();
                 ++it)
            {
                //perform the move - need to map to the infinite cartesian
                //space, make the move, and then convert back again
                PartialMolecule newmol = it->move()
                                            .toCartesian(space, map)
                                            .translate(delta, map)
                                            .fromCartesian(space,map)
                                            .commit();

                new_molecules.update(newmol);
            }

            system.update(new_molecules);

            //then rotate a single random molecule
            smplr.edit().updateFrom(system);

            tuple<PartialMolecule,double> mol_and_bias = smplr.read().sample();

            const PartialMolecule &oldmol = mol_and_bias.get<0>();
            old_bias = mol_and_bias.get<1>();

            //perform the move - need to map to the infinite cartesian
            //space, make the move, and then convert back again
            PartialMolecule newmol = oldmol.move()
                                           .toCartesian(space, map)
                                           .rotate(rotdelta,
                                                   oldmol.evaluate().centerOfGeometry(),
                                                   map)
                                           .fromCartesian(space,map)
                                           .commit();

            //update the system with the new coordinates
            system.update(newmol);

            //get the new bias on this molecule
            smplr.edit().updateFrom(system);

            new_bias = smplr.read().probabilityOf(newmol);
        }
    }
    else if (sync_rot)
    {
        //rotate all of the molecules
        const Molecules &molecules = smplr.read().group().molecules();

        Molecules new_molecules = molecules;

        for (Molecules::const_iterator it = molecules.constBegin();
             it != molecules.constEnd();
             ++it)
        {
            //perform the move - need to map to the infinite cartesian
            //space, make the move, and then convert back again
            PartialMolecule newmol = it->move()
                                        .toCartesian(space, map)
                                        .rotate(rotdelta,
                                                it->evaluate().centerOfGeometry(),
                                                map)
                                        .fromCartesian(space,map)
                                        .commit();

            new_molecules.update(newmol);
        }

        system.update(new_molecules);

        //then translate a single random molecule
        smplr.edit().updateFrom(system);

        tuple<PartialMolecule,double> mol_and_bias = smplr.read().sample();

        const PartialMolecule &oldmol = mol_and_bias.get<0>();
        old_bias = mol_and_bias.get<1>();

        //perform the move - need to map to the infinite cartesian
        //space, make the move, and then convert back again
        PartialMolecule newmol = oldmol.move()
                                       .toCartesian(space, map)
                                       .translate(delta, map)
                                       .fromCartesian(space,map)
                                       .commit();

        //update the system with the new coordinates
        system.update(newmol);

        //get the new bias on this molecule
        smplr.edit().updateFrom(system);

        new_bias = smplr.read().probabilityOf(newmol);
    }
}

/** Attempt 'n' rigid body moves of the views of the system 'system' */
void RigidBodyMC::move(System &system, int nmoves, bool record_stats)
{
    if (nmoves <= 0)
        return;

    QTime all_t;
    all_t.start();

    //save our, and the system's, current state
    RigidBodyMC old_state(*this);

    System old_system_state(system);
    
    try
    {
        PropertyMap map;
        map.set("coordinates", this->coordinatesProperty());
        
        const Space *space = &(Space::null());
        
        if ( this->spaceProperty().hasSource() )
        {
            if ( system.containsProperty(this->spaceProperty().source()) )
                space = &( system.property( this->spaceProperty() ).asA<Space>() );
        }
        else
        {
            space = &(this->spaceProperty().value().asA<Space>());
        }
    
        for (int i=0; i<nmoves; ++i)
        {
            //get the old total energy of the system
            double old_nrg = system.energy( this->energyComponent() );

            //save the old system and sampler
            System old_system(system);
            SamplerPtr old_sampler(smplr);

            double old_bias = 1;
            double new_bias = 1;

            this->performMove(system, *space, old_bias, new_bias, map);
    
            //calculate the energy of the system
            double new_nrg = system.energy( this->energyComponent() );

            //accept or reject the move based on the change of energy
            //and the biasing factors
            if (not this->test(new_nrg, old_nrg, new_bias, old_bias))
            {
                //the move has been rejected - reset the state
                smplr = old_sampler;
                system = old_system;
            }

            if (record_stats)
            {
                system.collectStats();
            }
        }
    }
    catch(...)
    {
        system = old_system_state;
        this->operator=(old_state);

        throw;
    }

    qDebug() << "RigidBodyMC move took" << all_t.elapsed();
}
