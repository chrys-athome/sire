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

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
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
        << replica.lambda_value << double(replica.replica_temperature)
        << double(replica.replica_pressure)
        << replica.replica_properties
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
        
        double temp, press;
        
        sds >> replica_nrg_component >> replica.lambda_component
            >> replica.lambda_value >> temp >> press
            >> replica.replica.properties
            >> static_cast<Replica&>(replica);
            
        replica.replica_temperature = Temperature(temp);
        replica.replica_pressure = Pressure(press);
    }
    else
        throw version_error( v, "1", r_replica, CODELOC );
        
    return ds;
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

//// All moves have a temperature, pressure, chemical potential etc.
////         Exceptions thrown if move doesn't support that, e.g. NVT, NVE moves
////         Moves object works out ensemble from comprised moves - looks
////         at what changes, get ensemble. So it can then consistentise 
////         all constituent moves.

void RepExReplicas::setPropertyNames(const PropertyMap &map);
void RepExReplicas::setPropertyNameS(int i, const PropertyMap &map);
