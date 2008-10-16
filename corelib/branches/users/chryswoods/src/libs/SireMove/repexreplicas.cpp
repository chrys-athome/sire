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

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireFF;
using namespace SireCAS;
using namespace SireBase;
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
    
    sds << replica.nrg_component << replica.lambda_component
        << replica.lambda_value
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
        
        sds >> new_replica_nrg_component >> new_replica.lambda_component
            >> new_replica.lambda_value
            >> static_cast<Replica&>(new_replica);

        new_replica.updateEnsembleParameters();
    }
    else
        throw version_error( v, "1", r_replica, CODELOC );
        
    return ds;
}

void RepExReplica::updateEnsembleParameters()
{
    //initialise
    is_constant_energy = false;
    is_constant_temperature = false;
    is_constant_volume = false;
    is_constant_pressure = false;
    
    replica_temperature = Temperature(0);
    replica_pressure = Pressure(0);
    
    nrg_component = this->moves().energyComponent();

    if (this->moves().isConstantEnergy())
    {
        is_constant_energy = true;
    }
    else if (this->moves().isConstantTemperature())
    {
        is_constant_temperature = true;
        replica_temperature = this->moves().temperature();
    }
    else
        throw SireError::incompatible_error( QObject::tr(
            "Only replica exchange moves involving constant energy or "
            "constant temperature ensembles are supported."), CODELOC );

    if (this->moves().isConstantVolume())
    {
        is_constant_volume = true;
    }
    else if (this->moves().isConstantPressure())
    {
        is_constant_pressure = true;
        replica_pressure = this->moves().pressure();
    }
    else
        throw SireError::incompatible_error( QObject::tr(
            "Only replica exchange moves involving constant volume or "
            "constant pressure ensembles are supported."), CODELOC );
}

/** Null constructor */
RepExReplica::RepExReplica()
             : ConcreteProperty<RepExReplica,Replica>(),
               nrg_component(ForceFields::totalComponent()),
               lambda_value(0),
{
    this->updateEnsembleParameters();
}

/** Construct from the passed replica - the remaining parameters will
    be set to default values */
RepExReplica::RepExReplica(const Replica &replica)
             : ConcreteProperty<RepExReplica,Replica>(replica),
               nrg_component(ForceFields::totalComponent()),
               lambda_value(0)

{   
    this->updateEnsembleParameters();
}

/** Copy constructor */
RepExReplica::RepExReplica(const RepExReplica &other)
             : ConcreteProperty<RepExReplica,Replica>(other),
               nrg_component(other.nrg_component),
               lambda_component(other.lambda_component),
               lambda_value(other.lambda_value),
               replica_temperature(other.replica_temperature),
               replica_pressure(other.replica_pressure),
               is_constant_energy(other.is_constant_energy),
               is_constant_temperature(other.is_constant_temperature),
               is_constant_volume(other.is_constant_volume),
               is_constant_pressure(other.is_constant_pressure)
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
        nrg_component = other.nrg_component;
        lambda_component = other.lambda_component;
        lambda_value = other.lambda_value;
        is_constant_energy = other.is_constant_energy;
        replica_temperature = other.replica_temperature;
        is_constant_volume = other.is_constant_volume;
        replica_pressure = other.replica_pressure;
    }
    
    return *this;
}

/** Comparison operator */
bool RepExReplica::operator==(const RepExReplica &other) const
{
    return this == &other or
           (nrg_component == other.nrg_component and
            lambda_component == other.lambda_component and
            lambda_value == other.lambda_value and
            Replica::operator==(other));
}

/** Comparison operator */
bool RepExReplica::operator!=(const RepExReplica &other) const
{
    return not this->operator==(other);
}

/** Return the component of the energy of the system that this replica
    uses as the total energy of the replica. This allows different
    replicas to use completely different Hamiltonians */
const Symbol& RepExReplica::energyComponent() const
{
    return nrg_component;
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

/** Return the temperature of this replica, if it is a constant
    temperature replica. If not, then this returns 0 */
Temperature RepExReplica::temperature() const
{
    return replica_temperature;
}

/** Return the current volume of this replica - this could be infinite! */
Volume RepExReplica::volume() const
{
    const SpaceBase &space = this->system().property( this->moves().spaceProperty() );
    return space.volume();
}

/** Return the pressure of this replica, if it is a constant
    pressure replica. If not, then this returns 0 */
Pressure RepExReplica::pressure() const
{
    return replica_pressure;
}

/** Return whether or not this is a constant energy replica
    (all moves sample the same potential energy) */
bool RepExReplica::isConstantEnergy() const
{
    return is_constant_energy;
}

/** Return whether or not this is a constant temperature replica
    (all moves sample the same temperature) */
bool RepExReplica::isConstantTemperature() const
{
    return is_constant_temperature;
}

/** Return whether or not this is a constant volume replica
    (all moves sample the same volume) */
bool RepExReplica::isConstantVolume() const
{
    return is_constant_volume;
}

/** Return whether or not this is a constant pressure replica
    (all moves sample the same pressure) */
bool RepExReplica::isConstantPressure() const
{
    return is_constant_pressure;
}

/** Return whether or not this is a constant lambda replica
    (there is a lambda component, 'lam', and all moves sample the 
     same value of this lambda coordinate) */
bool RepExReplica::isConstantLambda(const Symbol &lam) const
{
    return this->moves().isConstantLambda();
}

/** Return whether 'rep0' and 'rep1' sample different temperatures */
bool RepExReplica::differentTemperatures(const RepExReplica &rep0,
                                         const RepExReplica &rep1)
{
    if (rep0.is_constant_temperature)
    {
        if (not rep1.is_constant_temperature)
            return false;
        else
            return rep0.replica_temperature == rep1.replica_temperature;
    }
    else
        return not rep1.is_constant_temperature;
}
                                  
/** Return whether 'rep0' and 'rep1' sample different pressures */
bool RepExReplica::differentPressures(const RepExReplica &rep0,
                                      const RepExReplica &rep1)
{
    if (rep0.is_constant_pressure)
    {
        if (not rep1.is_constant_pressure)
            return false;
        else
            return rep0.replica_pressure == rep1.replica_pressure;
    }
    else
        return not rep1.is_constant_pressure;
}
                               
/** Return whether or not this replicas 'rep0' and 'rep1' sample
    different Hamiltonians */
bool RepExpReplica::differentHamiltonians(const RepExReplica &rep0,
                                          const RepExReplica &rep1)
{
    return rep0.nrg_component == rep1.nrg_component and
           rep0.lambda_component == rep1.lambda_component and
           rep0.lambda_value == rep1.lambda_value;
}

/** Return the total energy of this replica */
double RepExReplica::energy()
{
    System sys = Replica::system();
    
    double nrg = sys.energy( this->energyComponent() );
    
    Replica::setSystem(sys);
    
    return nrg;
}

/** Set the system to be simulated at this replica 

    \throw SireBase::missing_property
    \throw SireFF::missing_component
*/
void RepExReplica::setSystem(const System &system)
{
    if (is_constant_pressure)
    {
        if (not system.hasProperty( this->moves().spaceProperty() ))
        {
            throw SireBase::missing_property( QObject::tr(
                "Cannot set the system for this replica to %1, "
                "as this is a constant pressure replica, and the system "
                "is missing the required space property %2.")
                    .arg(system.toString(), 
                         this->moves().spaceProperty().toString()),
                                CODELOC );
        }
    }

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
    
    Replicas::setSystem(new_system);
}

/** Set the moves to be used to simulate the system at this replica 

    \throw SireBase::missing_property
*/
void RepExReplica::setMoves(const MovesBase &moves)
{
    RepExReplica old_state(*this);
    
    try
    {
        Replica::setMoves(moves);
        
        //changing the moves will change the ensemble for this replica
        this->updateEnsembleParameters();
        
        //ensure that the system has the necessary space
        if (is_constant_pressure)
        {
            if (not system.hasProperty( this->moves().spaceProperty() ))
            {
                throw SireBase::missing_property( QObject::tr(
                    "Cannot set the system for this replica to %1, "
                    "as this is a constant pressure replica, and the system "
                    "is missing the required space property %2.")
                        .arg(system.toString(), 
                             this->moves().spaceProperty().toString()),
                                    CODELOC );
            }
        }
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
            "as this system (%2) doens't have such a component. Available energy "
            "components are %3.")
                .arg(symbol.toString(), system().toString(),
                     Sire::toString(system().components())), CODELOC );

    nrg_component = symbol;
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
    Replicas::setSystem(new_system);
    
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
    Replicas::setSystem(new_system);
    
    lambda_value = value;
}

/** Set the temperature of this replica to 'temperature'. This is only possible
    if this is a constant temperature replica
    
    \throw SireError::incompatible_error
*/
void RepExReplica::setTemperature(const Temperature &temperature)
{
    if (not is_constant_temperature)
        throw SireError::incompatible_error( QObject::tr(
            "It is not possible to set the temperature of a replica that is "
            "not running constant temperature moves! This replica is running "
            "in this %1 ensemble.")
                .arg( this->moves().ensemble().shortHand() ), CODELOC );
}

void RepExReplica::setPressure(const Pressure &pressure);

/** Set the energy
void RepExReplica::setEnergy(const Energy &energy);
void RepExReplica::setVolume(const Volume &volume);

/** Swap the systems between the two replicas, 'rep0' and 'rep1' */
void RepExReplica::swapSystems(RepExReplica &rep0, RepExReplica &rep1)
{
    RepExReplica new_rep0(rep0);
    RepExReplica new_rep1(rep1);
    
    new_rep0.setSystem(rep1.system());
    new_rep1.setSystem(rep0.system());
    
    rep0 = new_rep0;
    rep1 = new_rep1;
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
              : ConcreteProperty<RepExReplica,Replicas>(n)
              
{
    if (n > 0)
    {
        Replicas::setReplica( RepExReplica() );
    }
}

/** Construct a set of 'n' replicas that are all a copy 
    of the system 'system' */
RepExReplicas::RepExReplicas(const System &system, int n)
              : ConcreteProperty<RepExReplica,Replicas>(n)
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
              : ConcreteProperty<RepExReplica,Replicas>(systems.count())
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
              : ConcreteProperty<RepExReplica,Replicas>(replicas.count())
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
              : ConcreteProperty<RepExReplica,Replicas>(other)
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
    this->_pvt_replicas(i).setPressure(pressure);
}
