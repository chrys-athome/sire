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

#include "replicas.h"

#include "SireID/index.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireMove;
using namespace SireBase;
using namespace SireID;
using namespace SireStream;

///////
/////// Implementation of Replica
///////

static const RegisterMetaType<Replica> r_replica;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const Replica &replica)
{
    writeHeader(ds, r_replica, 1);
    
    SharedDataStream sds(ds);
    
    sds << replica.sim_system << replica.sim_moves
        << replica.sim_nmoves << replica.sim_recordstats;
        
    return ds;
}

/** Read from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, Replica &replica)
{
    VersionID v = readHeader(ds, r_replica);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> replica.sim_system >> replica.sim_moves
            >> replica.sim_nmoves >> replica.sim_recordstats;
    }
    else
        throw version_error( v, "1", r_replica, CODELOC );
        
    return ds;
}

/** Null constructor */
Replica::Replica()
        : ConcreteProperty<Replica,Property>(),
          sim_nmoves(0), sim_recordstats(true)
{}

/** Copy constructor */
Replica::Replica(const Replica &other)
        : ConcreteProperty<Replica,Property>(other),
          sim_system(other.sim_system), sim_moves(other.sim_moves),
          sim_nmoves(other.sim_nmoves), sim_recordstats(other.sim_recordstats)
{}

/** Destructor */
Replica::~Replica()
{}

/** Copy assignment operator */
Replica& Replica::operator=(const Replica &other)
{
    if (this != &other)
    {
        sim_system = other.sim_system;
        sim_moves = other.sim_moves;
        sim_nmoves = other.sim_nmoves;
        sim_recordstats = other.sim_recordstats;
    }
    
    return *this;
}

/** Comparison operator */
bool Replica::operator==(const Replica &other) const
{
    return this == &other or
           (sim_nmoves == other.sim_nmoves and 
            sim_recordstats == other.sim_recordstats and
            sim_system == other.sim_system and sim_moves == other.sim_moves);
}

/** Comparison operator */
bool Replica::operator!=(const Replica &other) const
{
    return not this->operator==(other);
}

/** Return the system being simulated in this replica */
const System& Replica::system() const
{
    return sim_system;
}

/** Return the moves to be performed during the simulation on this replica */
const Moves& Replica::moves() const
{
    return sim_moves;
}

/** Return the number of moves to be applied to this replica */
int Replica::nMoves() const
{
    return sim_nmoves;
}

/** Return whether or not statistics should be recorded when simulating
    the system */
bool Replica::recordStatistics() const
{
    return sim_recordstats;
}

/** Set the system to be simulated at this replica */
void Replica::setSystem(const System &system)
{
    sim_system = system;
}

/** Set the moves to be applied to the system */
void Replica::setMoves(const Moves &moves)
{
    sim_moves = moves;
}

/** Set the number of moves to run for this replica */
void Replica::setNMoves(int nmoves)
{
    sim_nmoves = nmoves;
}

/** Set whether or not to record statistics during the simulation */
void Replica::setRecordStatistics(bool recordstats)
{
    sim_recordstats = recordstats;
}

/** Swap the systems between the two replicas, rep0 and rep1 */
void Replica::swapSystems(Replica &rep0, Replica &rep1)
{
    const SharedPolyPointer<Replica> old_rep0( rep0.clone() );
    const SharedPolyPointer<Replica> old_rep1( rep1.clone() );

    try
    {
        rep0.setSystem( old_rep1->system() );
        rep1.setSystem( old_rep0->system() );
    }
    catch(...)
    {
        rep0 = *old_rep0;
        rep1 = *old_rep1;
        throw;
    }
}

///////
/////// Implementation of Replicas
///////

static const RegisterMetaType<Replicas> r_replicas;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const Replicas &replicas)
{
    writeHeader(ds, r_replicas, 1);
    
    SharedDataStream sds(ds);
    
    sds << replicas.replicas_array;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, Replicas &replicas)
{
    VersionID v = readHeader(ds, r_replicas);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> replicas.replicas_array;
        
        return ds;
    }
    else
        throw version_error( v, "1", r_replicas, CODELOC );
        
    return ds;
}

/** Null constructor */
Replicas::Replicas() : ConcreteProperty<Replicas,Property>()
{}

/** Construct a set of 'n' null replicas */
Replicas::Replicas(int n)
         : ConcreteProperty<Replicas,Property>(),
           replicas_array(n)
{
    SharedPolyPointer<Replica> null_replica( new Replica() );

    for (int i=0; i<n; ++i)
    {
        replicas_array[i] = null_replica;
    }
}

/** Construct a set of replicas that are 'n' copies of the 
    system 'system' */
Replicas::Replicas(const System &system, int n)
         : ConcreteProperty<Replicas,Property>(),
           replicas_array(n)
{
    for (int i=0; i<n; ++i)
    {
        replicas_array[i] = new Replica();
        replicas_array[i]->setSystem(system);
    }
}

/** Construct a set of replicas from the passed array of systems */
Replicas::Replicas(const QVector<System> &systems)
         : ConcreteProperty<Replicas,Property>()
{
    if (not systems.isEmpty())
    {
        replicas_array = QVector< SharedPolyPointer<Replica> >(systems.count());
        
        for (int i=0; i<systems.count(); ++i)
        {
            replicas_array[i] = new Replica();
            replicas_array[i]->setSystem(systems[i]);
        }
    }
}

/** Copy constructor */
Replicas::Replicas(const Replicas &other)
         : ConcreteProperty<Replicas,Property>(other),
           replicas_array(other.replicas_array)
{}

/** Destructor */
Replicas::~Replicas()
{}

/** Copy assignment operator */
Replicas& Replicas::operator=(const Replicas &other)
{
    replicas_array = other.replicas_array;
    return *this;
}

/** Comparison operator */
bool Replicas::operator==(const Replicas &other) const
{
    return replicas_array == other.replicas_array;
}

/** Comparison operator */
bool Replicas::operator!=(const Replicas &other) const
{
    return replicas_array != other.replicas_array;
}

/** Return the 'ith' replica 

    \throw SireError::invalid_index
*/
const Replica& Replicas::operator[](int i) const
{
    return *(replicas_array.at( Index(i).map(replicas_array.count()) ));
}

/** Return the 'ith' replica 

    \throw SireError::invalid_index
*/
const Replica& Replicas::at(int i) const
{
    return this->operator[](i);
}

/** Internal function used to get a writable reference to the ith replica

    \throw SireError::invalid_index
*/
Replica& Replicas::_pvt_replica(int i)
{
    return *(replicas_array[ Index(i).map(nReplicas()) ]);
}

/** Return whether or not this set is empty */
bool Replicas::isEmpty() const
{
    return replicas_array.isEmpty();
}

/** Return the number of replicas in this set */
int Replicas::nReplicas() const
{
    return replicas_array.count();
}

/** Return the number of replicas in this set */
int Replicas::count() const
{
    return this->nReplicas();
}

/** Return the number of replicas in this set */
int Replicas::size()
{
    return this->nReplicas();
}

/** Set the replicas in this set equal to the replicas in 'replicas'.
    Note that the number of replicas *must* agree with the number of
    replicas in this set 
    
    \throw SireError::incompatible_error
*/
void Replicas::setReplicas(const Replicas &replicas)
{
    if (replicas.nReplicas() != this->nReplicas())
        throw SireError::incompatible_error( QObject::tr(
            "Cannot set the replicas from the other set, as they have "
            "different numbers of replicas! (%1 vs. %2)")
                .arg(this->nReplicas()).arg(replicas.nReplicas()),
                    CODELOC );
                    
    replicas_array = replicas.replicas_array;
}

/** Set all replicas equal to 'replica' */
void Replicas::setReplica(const Replica &replica)
{
    SharedPolyPointer<Replica> replica_copy(replica);

    for (int i=0; i < this->nReplicas(); ++i)
    {
        replicas_array[i] = replica_copy;
    }
}

/** Set the ith replica equal to 'replica'

    \throw SireError::invalid_index
*/
void Replicas::setReplica(int i, const Replica &replica)
{
    replicas_array[ Index(i).map(nReplicas()) ] = replica;
}

/** Set all the replicas to use the system 'system' */
void Replicas::setSystem(const System &system)
{
    Replicas old_state(*this);

    try
    {
        for (int i=0; i < this->nReplicas(); ++i)
        {
            replicas_array[i]->setSystem(system);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the system of the ith replica to 'system'

    \throw SireError::invalid_index
*/
void Replicas::setSystem(int i, const System &system)
{
    replicas_array[ Index(i).map(nReplicas()) ]->setSystem(system);
}

/** Set the moves to be performed on all replicas to 'moves' */
void Replicas::setMoves(const Moves &moves)
{
    Replicas old_state(*this);
    
    try
    {
        for (int i=0; i < this->nReplicas(); ++i)
        {
            replicas_array[i]->setMoves(moves);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the moves performed on the ith replica to 'moves'

    \throw SireError::invalid_index
*/
void Replicas::setMoves(int i, const Moves &moves)
{
    replicas_array[ Index(i).map(nReplicas()) ]->setMoves(moves);
}

/** Set the number of moves to be performed on all replicas to 'nmoves' */
void Replicas::setNMoves(int nmoves)
{
    Replicas old_state(*this);
    
    try
    {
        for (int i=0; i < this->nReplicas(); ++i)
        {
            replicas_array[i]->setNMoves(nmoves);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set the number of moves to perform on the ith replica to 'nmoves'

    \throw SireError::invalid_index
*/
void Replicas::setNMoves(int i, int nmoves)
{
    replicas_array[ Index(i).map(nReplicas()) ]->setNMoves(nmoves);
}

/** Set whether or not statistics will be recorded during simulations
    of all of the replicas */
void Replicas::setRecordStatistics(bool record_stats)
{
    Replicas old_state(*this);
    
    try
    {
        for (int i=0; i < this->nReplicas(); ++i)
        {
            replicas_array[i]->setRecordStatistics(record_stats);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Set whether or not to record statistics during the simulation of 
    the ith replica
    
    \throw SireError::invalid_index
*/
void Replicas::setRecordStatistics(int i, bool record_stats)
{
    replicas_array[ Index(i).map(nReplicas()) ]->setRecordStatistics(record_stats);
}
