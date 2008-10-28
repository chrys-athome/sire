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

#include "repexreplicas.h"

#include "SireFF/forcefields.h"

#include "SireUnits/units.h"
#include "SireUnits/temperature.h"
#include "SireUnits/dimensions.h"

#include "SireError/errors.h"
#include "SireFF/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireMove;
using namespace SireFF;
using namespace SireCAS;
using namespace SireBase;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace SireStream;

///////
/////// Implementation of RepExReplica
///////

static const RegisterMetaType<RepExReplica> r_replica;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const RepExReplica &replica)
{
    writeHeader(ds, r_replica, 1);
    
    SharedDataStream sds(ds);
    
    sds << replica.lambda_component << replica.lambda_value
        << static_cast<const Replica&>(replica);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, RepExReplica &replica)
{
    VersionID v = readHeader(ds, r_replica);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        RepExReplica new_replica;
        
        sds >> new_replica.lambda_component >> new_replica.lambda_value
            >> static_cast<Replica&>(new_replica);

        new_replica.updatedMoves();
    }
    else
        throw version_error( v, "1", r_replica, CODELOC );
        
    return ds;
}

void RepExReplica::updatedMoves()
{
    nrg_component = this->moves().energyComponent();
    space_property = this->moves().spaceProperty();
    replica_ensemble = this->moves().ensemble();

    if ( not (replica_ensemble.isConstantNParticles() or
              replica_ensemble.isConstantChemicalPotential()) )
        throw SireError::incompatible_error( QObject::tr(
            "Only replica exchange moves involving a constant number of "
            "particles or constant chemical potential are supported. "
            "The %1 is not supported.").arg(replica_ensemble.toString()), 
                CODELOC );

    if ( not (replica_ensemble.isConstantEnergy() or
              replica_ensemble.isConstantTemperature()) )
        throw SireError::incompatible_error( QObject::tr(
            "Only replica exchange moves involving constant energy or "
            "constant temperature ensembles are supported. "
            "The %1 is not supported.").arg(replica_ensemble.toString()), 
                CODELOC );

    if ( not (replica_ensemble.isConstantVolume() or
              replica_ensemble.isConstantPressure()) )
        throw SireError::incompatible_error( QObject::tr(
            "Only replica exchange moves involving constant volume or "
            "constant pressure ensembles are supported. "
            "The %1 is not supported.").arg(replica_ensemble.toString()), 
                CODELOC );
}

/** Null constructor */
RepExReplica::RepExReplica()
             : ConcreteProperty<RepExReplica,Replica>(),
               lambda_value(0)
{
    this->updatedMoves();
}

/** Construct from the passed replica - the remaining parameters will
    be set to default values */
RepExReplica::RepExReplica(const Replica &replica)
             : ConcreteProperty<RepExReplica,Replica>(replica),
               lambda_value(0)

{   
    this->updatedMoves();
}

/** Copy constructor */
RepExReplica::RepExReplica(const RepExReplica &other)
             : ConcreteProperty<RepExReplica,Replica>(other),
               replica_ensemble(other.replica_ensemble),
               space_property(other.space_property),
               nrg_component(other.nrg_component),
               lambda_component(other.lambda_component),
               lambda_value(other.lambda_value)
{}
               
/** Destructor */
RepExReplica::~RepExReplica()
{}

/** Copy assignment operator */
RepExReplica& RepExReplica::operator=(const RepExReplica &other)
{
    if (this != &other)
    {
        Replica::operator=(other);
        replica_ensemble = other.replica_ensemble;
        space_property = other.space_property;
        nrg_component = other.nrg_component;
        lambda_component = other.lambda_component;
        lambda_value = other.lambda_value;
    }
    
    return *this;
}

/** Copy from a basic replica */
RepExReplica& RepExReplica::operator=(const Replica &other)
{
    return this->operator=( RepExReplica(other) );
}

/** Comparison operator */
bool RepExReplica::operator==(const RepExReplica &other) const
{
    return this == &other or
           (replica_ensemble == other.replica_ensemble and
            space_property == other.space_property and
            nrg_component == other.nrg_component and
            lambda_component == other.lambda_component and
            lambda_value == other.lambda_value and
            Replica::operator==(other));
}

/** Comparison operator */
bool RepExReplica::operator!=(const RepExReplica &other) const
{
    return not this->operator==(other);
}

/** Return the ensemble that this replica samples */
const Ensemble& RepExReplica::ensemble() const
{
    return replica_ensemble;
}

/** Return the component of the energy of the system that this replica
    uses as the total energy of the replica. This allows different
    replicas to use completely different Hamiltonians */
const Symbol& RepExReplica::energyComponent() const
{
    return nrg_component;
}

/** Return the property that contains the space (simulation box)  
    for this replica */
const PropertyName& RepExReplica::spaceProperty() const
{
    return space_property;
}

/** Return the symbol representing the lambda component if this replica
    is part of a lambda-coordinate-based Hamiltonian replica exchange
    simulation. This component is null if this is not the case */
const Symbol& RepExReplica::lambdaComponent() const
{
    return lambda_component;
}

/** Return the value of lambda for this replica, assuming that it is
    part of a lambda-coordinate-based replica exchange simulation. If
    it isn't then 0 is returned */
double RepExReplica::lambdaValue() const
{
    return lambda_value;
}

/** Return the temperature of this replica.

    \throw SireError::incompatible_error
*/
Temperature RepExReplica::temperature() const
{
    return ensemble().temperature();
}

/** Return the pressure of this replica

    \throw SireError::incompatible_error
*/
Pressure RepExReplica::pressure() const
{
    return ensemble().pressure();
}

/** Return the fugacity of this replica

    \throw SireError::incompatible_error
*/
Pressure RepExReplica::fugacity() const
{
    return ensemble().fugacity();
}

/** Return the chemical potential of this replica

    \throw SireError::incompatible_error
*/
MolarEnergy RepExReplica::chemicalPotential() const
{
    return ensemble().chemicalPotential();
}

/** Return the current volume of this replica - this could be infinite! */
Volume RepExReplica::volume() const
{
    #warning Need to work out how to get the simulation space from a System
    throw SireError::incomplete_code( QObject::tr(
        "Need to work out how to get the simulation space!"), CODELOC );
        
    return Volume(0);
}

/** Return the current energy of this replica */
MolarEnergy RepExReplica::energy()
{
    System new_system = this->system();
    MolarEnergy nrg = new_system.energy( nrg_component );
    Replica::setSystem(new_system);

    return nrg;
}

/** Return whether or not this is a constant energy replica
    (all moves sample the same total energy) */
bool RepExReplica::isConstantEnergy() const
{
    return ensemble().isConstantEnergy();
}

/** Return whether or not this is a constant temperature replica
    (all moves sample the same temperature) */
bool RepExReplica::isConstantTemperature() const
{
    return ensemble().isConstantTemperature();
}

/** Return whether or not this is a constant volume replica
    (all moves sample the same volume) */
bool RepExReplica::isConstantVolume() const
{
    return ensemble().isConstantVolume();
}

/** Return whether or not this is a constant pressure replica
    (all moves sample the same pressure) */
bool RepExReplica::isConstantPressure() const
{
    return ensemble().isConstantPressure();
}

/** Return whether or not the number of particles is constant
    (all moves keep the same number of particles) */
bool RepExReplica::isConstantNParticles() const
{
    return ensemble().isConstantNParticles();
}

/** Return whether the moves keep the same fugacity */
bool RepExReplica::isConstantFugacity() const
{
    return ensemble().isConstantFugacity();
}

/** Return whether the moves keep the same chemical potential */
bool RepExReplica::isConstantChemicalPotential() const
{
    return ensemble().isConstantChemicalPotential();
}

/** Return whether or not this is a constant lambda replica
    (there is a lambda component, 'lam', and all moves sample the 
     same value of this lambda coordinate) */
bool RepExReplica::isConstantLambda(const Symbol &lam) const
{
    return this->moves().isConstantLambda(lam);
}

/** Set the system to be simulated at this replica 

    \throw SireFF::missing_component
*/
void RepExReplica::setSystem(const System &system)
{
    if (not system.hasComponent(nrg_component))
    {
        throw SireFF::missing_component( QObject::tr(
            "Cannot set the system for this replica to %1, "
            "as this system is missing the required energy "
            "component for this replica (%2). Available energy "
            "components are %3.")
                .arg(system.toString(), nrg_component.toString(),
                     Sire::toString(system.components())), CODELOC );
    }

    System new_system(system);
    
    if (not lambda_component.isNull())
    {
        new_system.setComponent( lambda_component, lambda_value );
    }
    
    Replica::setSystem(new_system);
}

/** Set the moves to be used to simulate the system at this replica 

    \throw SireBase::missing_property
*/
void RepExReplica::setMoves(const Moves &moves)
{
    RepExReplica old_state(*this);
    
    try
    {
        Replica::setMoves(moves);
        
        //changing the moves will change the ensemble for this replica
        this->updatedMoves();
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the energy component to be used to calculate the total energy
    of this replica
    
    \throw SireFF::missing_component
*/
void RepExReplica::setEnergyComponent(const Symbol &symbol)
{
    if (nrg_component == symbol)
        return;

    if (not this->system().hasComponent(symbol))
        throw SireFF::missing_component( QObject::tr(
            "Cannot set the energy component for this replica to %1, "
            "as this system (%2) doesn't have such a component. Available energy "
            "components are %3.")
                .arg(symbol.toString(), system().toString(),
                     Sire::toString(system().components())), CODELOC );

    MovesPtr mvs = Replica::moves();
    
    mvs.edit().setEnergyComponent(symbol);
    
    Replica::setMoves(mvs);

    nrg_component = symbol;
}

/** Set the property used to find the space (simulation box) for the replica */
void RepExReplica::setSpaceProperty(const PropertyName &spaceproperty)
{
    if (spaceproperty == space_property)
        return;
        
    MovesPtr mvs = Replica::moves();
    
    mvs.edit().setSpaceProperty(spaceproperty);
    
    Replica::setMoves(mvs);
    
    space_property = spaceproperty;
}

/** Set the lambda component used for lambda-based Hamiltonian
    replica exchange */
void RepExReplica::setLambdaComponent(const Symbol &symbol)
{
    if (lambda_component == symbol)
        return;
        
    if (symbol.isNull())
    {
        lambda_component = symbol;
        lambda_value = 0;
        return;
    }
    
    System new_system( this->system() );
    
    //default always to lambda=0
    new_system.setComponent(symbol, 0);
    Replica::setSystem(new_system);
    
    lambda_component = symbol;
    lambda_value = 0;
}

/** Set the value of the lambda component to 'value' */
void RepExReplica::setLambdaValue(double value)
{
    if (lambda_component.isNull() or value == lambda_value)
        return;
        
    System new_system( this->system() );
    
    new_system.setComponent(lambda_component, value);
    Replica::setSystem(new_system);
    
    lambda_value = value;
}

/** Set the temperature of this replica to 'temperature'. This is only possible
    if the moves sample a constant temperature ensemble
    
    \throw SireError::incompatible_error
*/
void RepExReplica::setTemperature(const Temperature &t)
{
    if (this->temperature() != t)
    {
        MovesPtr mvs = Replica::moves();
        mvs.edit().setTemperature(t);
        this->setMoves(mvs);
    }
}

/** Set the pressure of this replica to 'pressure'. This is only possible if
    the moves sample a constant pressure ensemble
    
    \throw SireError::incompatible_error
*/
void RepExReplica::setPressure(const Pressure &p)
{
    if (this->pressure() != p)
    {
        MovesPtr mvs = Replica::moves();
        mvs.edit().setPressure(p);
        this->setMoves(mvs);
    }
}

/** Set the fugacity of this replica to 'fugacity'. This is only possible
    if the moves sample a constant fugacity
    
    \throw SireError::incompatible_error
*/
void RepExReplica::setFugacity(const Pressure &f)
{
    if (this->fugacity() != f)
    {
        MovesPtr mvs = Replica::moves();
        mvs.edit().setFugacity(f);
        this->setMoves(mvs);
    }
}

/** Set the chemical potential of this replica to 'chemical_potential'. 
    This is only possible if the moves sample a constant chemical potential
    
    \throw SireError::incompatible_error
*/
void RepExReplica::setChemicalPotential(const MolarEnergy &c)
{
    if (this->chemicalPotential() != c)
    {
        MovesPtr mvs = Replica::moves();
        mvs.edit().setChemicalPotential(c);
        this->setMoves(mvs);
    }
}

/** Swap the systems between the two replicas, 'rep0' and 'rep1'. This
    swaps the systems, but leaves the system monitors behind
    (thus the monitors remain with the replica, which is what is
    normally required). However, if the optional 'swap_monitors'
    is true then the monitors are swapped as well. */
void RepExReplica::swapSystems(RepExReplica &rep0, RepExReplica &rep1,
                               bool swap_monitors)
{
    System new_rep0 = rep1.system();
    System new_rep1 = rep0.system();
    
    if (not swap_monitors)
    {
        new_rep0.setMonitors( rep1.system().monitors() );
        new_rep1.setMonitors( rep0.system().monitors() );
    }
    
    rep0.setSystem( new_rep0 );
    rep1.setSystem( new_rep1 );
}

/** Swap the molecules between the two replicas, 'rep0' and 'rep1'. This
    updates all of the molecules in 'rep0' with the status of all of the
    molecules in 'rep1', and vice-versa. Note that this won't have any
    affect unless 'rep0' and 'rep1' contain the same molecules */
void RepExReplica::swapMolecules(RepExReplica &rep0, RepExReplica &rep1)
{
    System new_rep0 = rep0.system();
    System new_rep1 = rep1.system();
    
    new_rep0.update( rep1.system().molecules() );
    new_rep1.update( rep0.system().molecules() );
    
    rep0.setSystem( new_rep0 );
    rep1.setSystem( new_rep1 );
}

///////
/////// Implementation of RepExReplicas
///////

static const RegisterMetaType<RepExReplicas> r_replicas;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const RepExReplicas &replicas)
{
    writeHeader(ds, r_replicas, 1);
    
    ds << static_cast<const Replicas&>(replicas);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, RepExReplicas &replicas)
{
    VersionID v = readHeader(ds, r_replicas);
    
    if (v == 1)
    {
        ds >> static_cast<Replicas&>(replicas);
    }
    else
        throw version_error( v, "1", r_replicas, CODELOC );
        
    return ds;
}

/** Null constructor */
RepExReplicas::RepExReplicas() : ConcreteProperty<RepExReplicas,Replicas>()
{}

/** Construct a set of 'n' replicas */
RepExReplicas::RepExReplicas(int n) 
              : ConcreteProperty<RepExReplicas,Replicas>(n)
{
    if (n > 0)
    {
        Replicas::setReplica( RepExReplica() );
    }
}

/** Construct a set of 'n' replicas that are all a copy 
    of the system 'system' */
RepExReplicas::RepExReplicas(const System &system, int n)
              : ConcreteProperty<RepExReplicas,Replicas>(n)
{
    if (n > 0)
    {
        RepExReplica replica;
        replica.setSystem(system);
    
        Replicas::setReplica(replica);
    }
}

/** Construct the replicas from the array of systems in 'systems' */
RepExReplicas::RepExReplicas(const QVector<System> &systems)
              : ConcreteProperty<RepExReplicas,Replicas>(systems.count())
{
    if (not systems.isEmpty())
    {
        for (int i=0; i<systems.count(); ++i)
        {
            RepExReplica replica;
            replica.setSystem(systems[i]);
            
            Replicas::setReplica(i, replica);
        }
    }
}

/** Construct from a passed set of replicas */
RepExReplicas::RepExReplicas(const Replicas &replicas)
              : ConcreteProperty<RepExReplicas,Replicas>(replicas.count())
{
    if (replicas.isEmpty())
        return;
        
    if (replicas.isA<RepExReplicas>())
    {
        this->operator=(replicas.asA<RepExReplicas>());
        return;
    }
    
    for (int i=0; i<replicas.count(); ++i)
    {
        RepExReplica replica( replicas[i] );
        Replicas::setReplica(i, replica);
    }
}

/** Copy constructor */
RepExReplicas::RepExReplicas(const RepExReplicas &other)
              : ConcreteProperty<RepExReplicas,Replicas>(other)
{}

/** Destructor */
RepExReplicas::~RepExReplicas()
{}

/** Copy assignment operator */
RepExReplicas& RepExReplicas::operator=(const RepExReplicas &other)
{
    Replicas::operator=(other);
    return *this;
}

/** Copy from another 'replicas' object */
RepExReplicas& RepExReplicas::operator=(const Replicas &other)
{
    return this->operator=( RepExReplicas(other) );
}

/** Comparison operator */
bool RepExReplicas::operator==(const RepExReplicas &other) const
{
    return Replicas::operator==(other);
}

/** Comparison operator */
bool RepExReplicas::operator!=(const RepExReplicas &other) const
{
    return Replicas::operator!=(other);
}

/** Return the replica at index 'i'

    \throw SireError::invalid_index
*/
const RepExReplica& RepExReplicas::operator[](int i) const
{
    const Replica &replica = Replicas::operator[](i);
    
    BOOST_ASSERT( replica.isA<RepExReplica>() );
    
    return replica.asA<RepExReplica>();
}

/** Return the replica at index 'i'

    \throw SireError::invalid_index
*/
const RepExReplica& RepExReplicas::at(int i) const
{
    return this->operator[](i);
}

/** Internal function used to get a writable reference to the ith replica

    \throw SireError::invalid_index
*/
RepExReplica& RepExReplicas::_pvt_replica(int i)
{
    Replica &replica = Replicas::_pvt_replica(i);
    
    BOOST_ASSERT( replica.isA<RepExReplica>() );
    
    return replica.asA<RepExReplica>();
}

/** Set the replicas in this set equal to 'replicas'

    \throw SireError::incompatible_error
*/
void RepExReplicas::setReplicas(const Replicas &replicas)
{
    if (replicas.isA<RepExReplicas>())
    {
        //no need for type-checking
        Replicas::setReplicas(replicas);
    }
    else
    {
        RepExReplicas new_replicas(replicas);
    
        //check that all of the replicas are RepExReplica objects
        for (int i=0; i<replicas.count(); ++i)
        {
            if (not replicas[i].isA<RepExReplica>())
            {
                //if not - then convert them
                new_replicas.setReplica(i, RepExReplica(replicas[i]));
            }
        }
        
        Replicas::setReplicas(new_replicas);
    }
}

/** Set all replicas equal to 'replica' */
void RepExReplicas::setReplica(const Replica &replica)
{
    if (replica.isA<RepExReplica>())
    {
        Replicas::setReplica(replica);
    }
    else
    {
        Replicas::setReplica( RepExReplica(replica) );
    }
}

/** Set the ith replica equal to the replica 'replica'

    \throw SireError::invalid_index
*/
void RepExReplicas::setReplica(int i, const Replica &replica)
{
    if (replica.isA<RepExReplica>())
    {
        Replicas::setReplica(i, replica);
    }
    else
    {
        Replicas::setReplica(i, RepExReplica(replica));
    }
}

/** Set the energy component to be used to calculate the energy 
    for all replicas to 'symbol' */
void RepExReplicas::setEnergyComponent(const Symbol &symbol)
{
    RepExReplicas old_state(*this);
    
    try
    {
        for (int i=0; i < nReplicas(); ++i)
        {
            this->_pvt_replica(i).setEnergyComponent(symbol);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the energy component used to calculate the energy of 
    the ith replica to 'symbol'
    
    \throw SireError::invalid_index
*/
void RepExReplicas::setEnergyComponent(int i, const Symbol &symbol)
{
    this->_pvt_replica(i).setEnergyComponent(symbol);
}

/** Set the property to be used to find the simulation space (box)
    for all replicas to 'spaceproperty' */
void RepExReplicas::setSpaceProperty(const PropertyName &spaceproperty)
{
    RepExReplicas old_state(*this);
    
    try
    {
        for (int i=0; i < nReplicas(); ++i)
        {
            this->_pvt_replica(i).setSpaceProperty(spaceproperty);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the property used to find the simulation space (box) of 
    the ith replica to 'spaceproperty'
    
    \throw SireError::invalid_index
*/
void RepExReplicas::setSpaceProperty(int i, const PropertyName &spaceproperty)
{
    this->_pvt_replica(i).setSpaceProperty(spaceproperty);
}

/** Set the component that represents lambda in lambda-based
    Hamiltonian replica exchange */
void RepExReplicas::setLambdaComponent(const Symbol &symbol)
{
    RepExReplicas old_state(*this);
    
    try
    {
        for (int i=0; i < nReplicas(); ++i)
        {
            this->_pvt_replica(i).setLambdaComponent(symbol);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the lambda component used for the ith replica

    \throw SireError::invalid_index
*/
void RepExReplicas::setLambdaComponent(int i, const Symbol &symbol)
{
    this->_pvt_replica(i).setLambdaComponent(symbol);
}

/** Set the value of lambda used for all replicas in lambda-based 
    Hamiltonian replica exchange */
void RepExReplicas::setLambdaValue(double value)
{
    RepExReplicas old_state(*this);
    
    try
    {
        for (int i=0; i < nReplicas(); ++i)
        {
            this->_pvt_replica(i).setLambdaValue(value);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the value of lambda for the ith replica 

    \throw SireError::invalid_index
*/
void RepExReplicas::setLambdaValue(int i, double value)
{
    this->_pvt_replica(i).setLambdaValue(value);
}

/** Set the temperature of all of the replicas in this set */
void RepExReplicas::setTemperature(const Temperature &temperature)
{
    RepExReplicas old_state(*this);
    
    try
    {
        for (int i=0; i < nReplicas(); ++i)
        {
            this->_pvt_replica(i).setTemperature(temperature);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the temperature of the ith replica to 'temperature' 

    \throw SireError::invalid_index
*/
void RepExReplicas::setTemperature(int i, const Temperature &temperature)
{
    this->_pvt_replica(i).setTemperature(temperature);
}

/** Set the pressure of all of the replicas to 'pressure' */
void RepExReplicas::setPressure(const Pressure &pressure)
{
    RepExReplicas old_state(*this);
    
    try
    {
        for (int i=0; i < nReplicas(); ++i)
        {
            this->_pvt_replica(i).setPressure(pressure);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the pressure acting on the ith replica to 'pressure'

    \throw SireError::invalid_index
*/
void RepExReplicas::setPressure(int i, const Pressure &pressure)
{
    this->_pvt_replica(i).setPressure(pressure);
}

/** Set the fugacity of all of the replicas to 'fugacity' */
void RepExReplicas::setFugacity(const Pressure &fugacity)
{
    RepExReplicas old_state(*this);
    
    try
    {
        for (int i=0; i < nReplicas(); ++i)
        {
            this->_pvt_replica(i).setFugacity(fugacity);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the fugacity of the ith replica to 'fugacity'

    \throw SireError::invalid_index
*/
void RepExReplicas::setFugacity(int i, const Pressure &fugacity)
{
    this->_pvt_replica(i).setFugacity(fugacity);
}

/** Set the chemical potential of all of the replicas to 'chemical_potential' */
void RepExReplicas::setChemicalPotential(const MolarEnergy &chemical_potential)
{
    RepExReplicas old_state(*this);
    
    try
    {
        for (int i=0; i < nReplicas(); ++i)
        {
            this->_pvt_replica(i).setChemicalPotential(chemical_potential);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the chemical potential of the ith replica to 'chemical_potential'

    \throw SireError::invalid_index
*/
void RepExReplicas::setChemicalPotential(int i, const MolarEnergy &chemical_potential)
{
    this->_pvt_replica(i).setChemicalPotential(chemical_potential);
}
