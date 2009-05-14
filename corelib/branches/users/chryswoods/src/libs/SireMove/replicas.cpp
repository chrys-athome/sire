/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "replicas.h"
#include "replica.h"

#include "SireID/index.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

using namespace SireMove;
using namespace SireID;
using namespace SireSystem;
using namespace SireBase;
using namespace SireUnits::Dimension;
using namespace SireStream;

static const RegisterMetaType<Replicas> r_replicas;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const Replicas &replicas)
{
    writeHeader(ds, r_replicas, 2);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const SupraSystem&>(replicas);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, Replicas &replicas)
{
    VersionID v = readHeader(ds, r_replicas);
    
    if (v == 2)
    {
        SharedDataStream sds(ds);
        sds >> static_cast<SupraSystem&>(replicas);
    }
    else if (v < 2)
    {
        throw SireError::program_bug( QObject::tr(
            "Reading of version %1 of SireMove::Replicas should have "
            "been handled by the deprecated classes reader. Something has "
            "gone wrong if they are are being read here...")
                .arg(v), CODELOC );
    }
    else
        throw version_error(v, "1", r_replicas, CODELOC);
        
    return ds;
}

/** Constructor */
Replicas::Replicas() : ConcreteProperty<Replicas,SupraSystem>()
{}

/** Construct a set of 'n' replicas */
Replicas::Replicas(int n) : ConcreteProperty<Replicas,SupraSystem>(n)
{
    if (n > 0)
        SupraSystem::setSubSystem( Replica() );
}

/** Construct a set of 'n' replicas that contain the sub-system 'system' */
Replicas::Replicas(const System &system, int n)
         : ConcreteProperty<Replicas,SupraSystem>(n)
{
    if (n > 0)
    {
        Replica replica;
        replica.setSubSystem(system);
    
        SupraSystem::setSubSystem(replica);
    }
}

/** Construct a set of replicas from the passed array of systems */
Replicas::Replicas(const QVector<System> &systems)
         : ConcreteProperty<Replicas,SupraSystem>(systems.count())
{
    for (int i=0; i<systems.count(); ++i)
    {
        Replica replica;
        replica.setSubSystem(systems.at(i));
        
        SupraSystem::setSubSystem( i, replica );
    }
}

/** Construct a Replicas object that contains 'n' copies of the 
    passed subsystem */
Replicas::Replicas(const SupraSubSystem &subsystem, int n)
         : ConcreteProperty<Replicas,SupraSystem>(n)
{
    if (subsystem.isA<Replica>())
    {
        for (int i=0; i<n; ++i)
            SupraSystem::setSubSystem(i, subsystem);
    }
    else
    {
        for (int i=0; i<n; ++i)
            SupraSystem::setSubSystem(i, Replica(subsystem));
    }
}

/** Construct from the passed SupraSystem */
Replicas::Replicas(const SupraSystem &suprasystem)
         : ConcreteProperty<Replicas,SupraSystem>(suprasystem.nSubSystems())
{
    if (suprasystem.isA<Replicas>())
        this->operator=(suprasystem.asA<SupraSystem>());

    else
    {
        for (int i=0; i<suprasystem.nSubSystems(); ++i)
        {
            if (suprasystem[i].isA<Replica>())
                SupraSystem::setSubSystem( i, suprasystem[i] );
            else
                SupraSystem::setSubSystem( i, Replica(suprasystem[i]) );
        }
    }
}

/** Copy constructor */
Replicas::Replicas(const Replicas &other) : ConcreteProperty<Replicas,SupraSystem>(other)
{}

/** Destructor */
Replicas::~Replicas()
{}

/** Copy assignment operator */
Replicas& Replicas::operator=(const Replicas &other)
{
    SupraSystem::operator=(other);
    return *this;
}

/** Comparison operator */
bool Replicas::operator==(const Replicas &other) const
{
    return SupraSystem::operator==(other);
}

/** Comparison operator */
bool Replicas::operator!=(const Replicas &other) const
{
    return SupraSystem::operator!=(other);
}

/** Internal function used to get the ith replica - there is no
    bounds checking with this function */
Replica& Replicas::_pvt_replica(int i)
{
    SupraSubSystem &subsys = SupraSystem::_pvt_subSystem(i);
    
    if (not subsys.isA<Replica>())
        throw SireError::program_bug( QObject::tr(
            "How did Replicas get a replica that is a %1?")
                .arg(subsys.what()), CODELOC );
                
    return subsys.asA<Replica>();
}

/** Internal function used to get the ith replica - there is no
    bounds checking with this function */
const Replica& Replicas::_pvt_replica(int i) const
{
    const SupraSubSystem &subsys = SupraSystem::_pvt_subSystem(i);
    
    if (not subsys.isA<Replica>())
        throw SireError::program_bug( QObject::tr(
            "How did Replicas get a replica that is a %1?")
                .arg(subsys.what()), CODELOC );
                
    return subsys.asA<Replica>();
}

/** Internal function used to get the ith replica - there is no
    bounds checking with this function */
const Replica& Replicas::_pvt_constReplica(int i) const
{
    return this->_pvt_replica(i);
}

/** Return the number of replicas in this set */
int Replicas::nReplicas() const
{
    return SupraSystem::nSubSystems();
}

/** Return the ith replica 

    \throw SireError::invalid_index
*/
const Replica& Replicas::operator[](int i) const
{
    return Replicas::_pvt_replica( Index(i).map(this->nReplicas()) );
}

/** Return the ith replica

    \throw SireError::invalid_index
*/
const Replica& Replicas::at(int i) const
{
    return Replicas::operator[](i);
}

/** Set the replicas from a copy of passed replicas */
void Replicas::setReplicas(const Replicas &replicas)
{
    this->setSubSystems(replicas);
}

/** Set the replicas to all be a copy of 'replica' */
void Replicas::setReplica(const Replica &replica)
{
    this->setSubSystems(replica);
}

/** Set the ith replica equal to 'replica'

    \throw SireError::invalid_index
*/
void Replicas::setReplica(int i, const Replica &replica)
{
    this->setSubSystem(i, replica);
}

/** Overloaded function used to ensure that this system only contains
    Replica subsystems */
void Replicas::setSubSystem(int i, const SupraSubSystem &subsystem)
{
    if (subsystem.isA<Replica>())
        SupraSystem::setSubSystem(i, subsystem);
        
    else
        SupraSystem::setSubSystem(i, Replica(subsystem));
}

/** This internal function checks for shared copies of replica 'i'
    (which used to be 'old_replica') and copies whatever is now
    at replica i to any previous replica that still points to 'old_replica'.
    
    Note that this only checks replicas i+1 <= j < this->nReplicas()
*/
void Replicas::copySharedCopies(int i, const Replica *old_replica)
{
    const Replica *new_replica = &(this->_pvt_constReplica(i));
    
    if (new_replica != old_replica)
    {
        //there may be some shared copies
        int n = this->nReplicas();
        
        for (int j=i+1; j<n; ++j)
        {
            if (old_replica == &(this->_pvt_constReplica(j)))
                SupraSystem::setSubSystem(j, *new_replica);
        }
    }
}

/** Set the energy component sampled by all replicas to 'symbol' */
void Replicas::setEnergyComponent(const Symbol &symbol)
{
    Replicas old_state = *this;
    
    try
    {
        int n = this->nReplicas();
    
        for (int i=0; i<n; ++i)
        {
            const Replica *old_replica = &(this->_pvt_constReplica(i));
            
            if (old_replica->energyComponent() != symbol)
            {
                this->_pvt_replica(i).setEnergyComponent(symbol);
                this->copySharedCopies(i, old_replica);
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the energy component sampled by the ith replica to 'symbol'

    \throw SireError::invalid_index
*/
void Replicas::setEnergyComponent(int i, const Symbol &symbol)
{
    i = Index(i).map( this->nReplicas() );

    if (this->_pvt_constReplica(i).energyComponent() != symbol)
    {
        this->_pvt_replica(i).setEnergyComponent(symbol);
    }
}

/** Set the property used to find the simulation space for all replicas 
    to 'spaceproperty' */
void Replicas::setSpaceProperty(const PropertyName &spaceproperty)
{
    Replicas old_state = *this;
    
    try
    {
        int n = this->nReplicas();
        
        for (int i=0; i<n; ++i)
        {
            const Replica *old_replica = &(this->_pvt_constReplica(i));
            
            if (old_replica->spaceProperty() != spaceproperty)
            {
                this->_pvt_replica(i).setSpaceProperty(spaceproperty);
                this->copySharedCopies(i, old_replica);
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the space property used by the ith replica to 'spaceproperty'

    \throw SireError::invalid_index
*/
void Replicas::setSpaceProperty(int i, const PropertyName &spaceproperty)
{
    i = Index(i).map( this->nReplicas() );

    if (this->_pvt_constReplica(i).spaceProperty() != spaceproperty)
    {
        this->_pvt_replica(i).setSpaceProperty(spaceproperty);
    }
}

/** Set the lambda component used for Hamiltonian replica exchange 
    for all replicas to 'symbol' */
void Replicas::setLambdaComponent(const Symbol &symbol)
{
    Replicas old_state = *this;
    
    try
    {
        int n = this->nReplicas();
        
        for (int i=0; i<n; ++i)
        {
            const Replica *old_replica = &(this->_pvt_constReplica(i));
            
            if (old_replica->lambdaComponent() != symbol)
            {
                this->_pvt_replica(i).setLambdaComponent(symbol);
                this->copySharedCopies(i, old_replica);
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the lambda component used by the ith replica for Hamiltonian
    replica exchange to 'symbol'
    
    \throw SireError::invalid_index
*/
void Replicas::setLambdaComponent(int i, const Symbol &symbol)
{
    i = Index(i).map( this->nReplicas() );

    if (this->_pvt_constReplica(i).lambdaComponent() != symbol)
        this->_pvt_replica(i).setLambdaComponent(symbol);
}

/** Set the lambda value for Hamiltonian replica exchange for all
    replicas to 'value' */
void Replicas::setLambdaValue(double value)
{
    Replicas old_state = *this;
    
    try
    {
        int n = this->nReplicas();
        
        for (int i=0; i<n; ++i)
        {
            const Replica *old_replica = &(this->_pvt_constReplica(i));
            
            if (old_replica->lambdaValue() != value)
            {
                this->_pvt_replica(i).setLambdaValue(value);
                this->copySharedCopies(i, old_replica);
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the value of lambda used for Hamiltonian replica exchange for
    the ith replica to 'value'
    
    \throw SireError::invalid_index
*/
void Replicas::setLambdaValue(int i, double value)
{
    i = Index(i).map( this->nReplicas() );

    if (this->_pvt_constReplica(i).lambdaValue() != value)
    {
        this->_pvt_replica(i).setLambdaValue(value);
    }
}

/** Set the temperature of the ensemble sampled by all replicas
    to 'temperature'
    
    \throw SireError::incompatible_error
*/
void Replicas::setTemperature(const Temperature &temperature)
{
    Replicas old_state = *this;
    
    try
    {
        int n = this->nReplicas();
        
        for (int i=0; i<n; ++i)
        {
            const Replica *old_replica = &(this->_pvt_constReplica(i));
            
            if (old_replica->temperature() != temperature)
            {
                this->_pvt_replica(i).setTemperature(temperature);
                this->copySharedCopies(i, old_replica);
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the temperature of the ensemble sampled by
    the ith replica to 'temperature'

    \throw SireError::invalid_index
    \throw SireError::incompatible_error
*/
void Replicas::setTemperature(int i, const Temperature &temperature)
{
    i = Index(i).map( this->nReplicas() );

    if (this->_pvt_constReplica(i).temperature() != temperature)
    {
        this->_pvt_replica(i).setTemperature(temperature);
    }
}

/** Set the pressure of the ensemble sampled by all replicas
    to 'pressure' */
void Replicas::setPressure(const Pressure &pressure)
{
    Replicas old_state = *this;
    
    try
    {
        int n = this->nReplicas();
        
        for (int i=0; i<n; ++i)
        {
            const Replica *old_replica = &(this->_pvt_constReplica(i));
            
            if (old_replica->pressure() != pressure)
            {
                this->_pvt_replica(i).setPressure(pressure);
                this->copySharedCopies(i, old_replica);
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the pressure of the ensemble sampled by the ith replica
    to 'pressure'
    
    \throw SireError::invalid_index
    \throw SireError::incompatible_error
*/
void Replicas::setPressure(int i, const Pressure &pressure)
{
    i = Index(i).map( this->nReplicas() );
    
    if (this->_pvt_constReplica(i).pressure() != pressure)
        this->_pvt_replica(i).setPressure(pressure);
}

/** Set the fugacity of the ensemble sampled by all replicas
    to 'fugacity'
    
    \throw SireError::incompatible_error
*/
void Replicas::setFugacity(const Pressure &fugacity)
{
    Replicas old_state = *this;
    
    try
    {
        int n = this->nReplicas();
        
        for (int i=0; i<n; ++i)
        {
            const Replica *old_replica = &(this->_pvt_constReplica(i));
            
            if (old_replica->fugacity() != fugacity)
            {
                this->_pvt_replica(i).setFugacity(fugacity);
                this->copySharedCopies(i, old_replica);
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the fugacity of the ensemble sampled by the ith replica
    to 'fugacity'
    
    \throw SireError::invalid_index
    \throw SireError::incompatible_error
*/
void Replicas::setFugacity(int i, const Pressure &fugacity)
{
    i = Index(i).map( this->nReplicas() );
    
    if (this->_pvt_constReplica(i).fugacity() != fugacity)
        this->_pvt_replica(i).setFugacity(fugacity);
}

/** Set the chemical potential of the ensemble sampled by all
    replicas to 'chemical_potential'
    
    \throw SireError::incompatible_error
*/
void Replicas::setChemicalPotential(const MolarEnergy &chemical_potential)
{
    Replicas old_state = *this;
    
    try
    {
        int n = this->nReplicas();
        
        for (int i=0; i<n; ++i)
        {
            const Replica *old_replica = &(this->_pvt_constReplica(i));
            
            if (old_replica->chemicalPotential() != chemical_potential)
            {
                this->_pvt_replica(i).setChemicalPotential(chemical_potential);
                this->copySharedCopies(i, old_replica);
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the chemical potential of the ensemble sampled by the ith
    replica to 'chemical_potential'
    
    \throw SireError::invalid_index
    \throw SireError::incompatible_error
*/
void Replicas::setChemicalPotential(int i, const MolarEnergy &chemical_potential)
{
    i = Index(i).map( this->nReplicas() );
    
    if (this->_pvt_constReplica(i).chemicalPotential() != chemical_potential)
        this->_pvt_replica(i).setChemicalPotential(chemical_potential);
}
