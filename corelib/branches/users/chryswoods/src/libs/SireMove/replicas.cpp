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
    writeHeader(ds, r_replica, 2);
    
    SharedDataStream sds(ds);

    sds << replica.sim_store << replica.sim_nmoves << replica.sim_recordstats;
        
    return ds;
}

/** Read from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, Replica &replica)
{
    VersionID v = readHeader(ds, r_replica);
    
    if (v == 2)
    {
        SharedDataStream sds(ds);
        sds >> replica.sim_store >> replica.sim_nmoves >> replica.sim_recordstats;
    }
    else if (v == 1)
    {
        SharedDataStream sds(ds);

        System sim_system;
        MovesPtr sim_moves;
        
        sds >> sim_system >> sim_moves
            >> replica.sim_nmoves >> replica.sim_recordstats;
            
        replica.sim_store = SimStore(sim_system, sim_moves);
    }
    else
        throw version_error( v, "1,2", r_replica, CODELOC );
        
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
          sim_store(other.sim_store),
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
        sim_store = other.sim_store;
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
            sim_store == other.sim_store);
}

/** Comparison operator */
bool Replica::operator!=(const Replica &other) const
{
    return not this->operator==(other);
}

/** Revert back to the state 'old_state' */
void Replica::revertTo(const Replica &old_state)
{
    Replica::operator=(old_state);
}

/** Internal function used to throw an invalid_cast exception 

    \throw SireError::invalid_cast
*/
void Replica::throwCastingError(const char *typenam) const
{
    throw SireError::invalid_cast( QObject::tr(
        "You cannot cast a replica of type %1 to a replica of type %2.")    
            .arg(this->what()).arg(typenam), CODELOC );
}

/** Return whether or not the system and moves are packed together
    into a compressed binary representation - this is used to save
    memory if we have lots of replicas */
bool Replica::isPacked() const
{
    return sim_store.isPacked();
}

/** Pack the system and moves into a compressed binary representation. 
    This is useful to save memory if there are lots of replicas */
void Replica::pack()
{
    sim_store.pack();
}

/** Unpack the system and moves */
void Replica::unpack()
{
    sim_store.unpack();
}

/** Pack the system and moves to disk - this saves the most memory */
void Replica::packToDisk()
{
    sim_store.packToDisk();
}

/** Pack the system and moves to disk - this places them in the
    directory 'tempdir' */
void Replica::packToDisk(const QString &tempdir)
{
    sim_store.packToDisk(tempdir);
}

/** Pack the system and moves to memory */
void Replica::packToMemory()
{
    sim_store.packToMemory();
}

/** Return whether or not the system and moves are packed to disk */
bool Replica::isPackedToDisk() const
{
    return sim_store.isPackedToDisk();
}

/** Return whether or not the system and moves are packed to memory */
bool Replica::isPackedToMemory() const
{
    return sim_store.isPackedToMemory();
}

/** Return the system being simulated in this replica */
System Replica::system() const
{
    return sim_store.system();
}

/** Tell this replica to clear all of its statistics */
void Replica::clearStatistics()
{
    if (this->isPacked())
        throw SireError::invalid_state( QObject::tr(
            "You cannot clear the statistics of a Replica that is packed. "
            "You must unpack it first."), CODELOC );
    
    System sim_system = sim_store.system();
    MovesPtr sim_moves = sim_store.moves();
    
    sim_system.clearStatistics();
    sim_moves.edit().clearStatistics();
    
    sim_store.setSystemAndMoves(sim_system, sim_moves);
}

/** Tell this replica that the system energy must be recalculated
    from scratch */
void Replica::mustNowRecalculateFromScratch()
{
    if (this->isPacked())
        throw SireError::invalid_state( QObject::tr(
            "You cannot ask a replica to recalculate from scratch when it is packed. "
            "You must unpack it first."), CODELOC );

    System sim_system = sim_store.system();

    sim_system.mustNowRecalculateFromScratch();

    sim_store.setSystem(sim_system);
}

/** Return the moves to be performed during the simulation on this replica */
MovesPtr Replica::moves() const
{
    return sim_store.moves();
}

/** Return both the system and moves together */
SimStore Replica::systemAndMoves() const
{
    return sim_store;
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
    sim_store.setSystem(system);
}

/** Set the moves to be applied to the system */
void Replica::setMoves(const Moves &moves)
{
    sim_store.setMoves(moves);
}

/** Simultaneously set the System to 'system' and the moves to 'moves' */
void Replica::setSystemAndMoves(const System &system, const Moves &moves)
{
    this->setSystemAndMoves( SimStore(system,moves) );
}

/** Simultaneously set the System and Moves to those contained in 'simstore' */
void Replica::setSystemAndMoves(const SimStore &simstore)
{
    sim_store = simstore;
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
    if ( &rep0 == &rep1 )
        return;

    if (rep0.isPacked() or rep1.isPacked())
        throw SireError::invalid_state( QObject::tr(
            "You cannot swap the systems as at least one of the replicas is "
            "packed (%1 : %2) - you must unpack it first.")
                .arg(rep0.isPacked()).arg(rep1.isPacked()), CODELOC );

    const SharedPolyPointer<Replica> old_rep0( rep0.clone() );
    const SharedPolyPointer<Replica> old_rep1( rep1.clone() );

    try
    {
        rep0.setSystem( old_rep1->system() );
        rep1.setSystem( old_rep0->system() );
    }
    catch(...)
    {
        rep0.revertTo(*old_rep0);
        rep1.revertTo(*old_rep1);
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

/** Return whether or not all of the replicas are packed */
bool Replicas::isPacked() const
{
    int nreplicas = replicas_array.count();
    
    for (int i=0; i<nreplicas; ++i)
    {
        if (not replicas_array.at(i)->isPacked())
            return false;
    }
    
    return true;
}

/** Return whether or not all of the replicas are packed to memory */
bool Replicas::isPackedToMemory() const
{
    int nreplicas = replicas_array.count();
    
    for (int i=0; i<nreplicas; ++i)
    {
        if (not replicas_array.at(i)->isPackedToMemory())
            return false;
    }
    
    return true;
}

/** Return whether or not all of the replicas are packed to memory */
bool Replicas::isPackedToDisk() const
{
    int nreplicas = replicas_array.count();
    
    for (int i=0; i<nreplicas; ++i)
    {
        if (not replicas_array.at(i)->isPackedToDisk())
            return false;
    }
    
    return true;
}

/** Pack all of the replicas - this compresses all of the replicas
    into compressed binary arrays, thereby saving lots of memory. This
    is useful if you have lots of replicas (or very large replicas) */
void Replicas::pack()
{
    if (this->isPacked())
        return;

    int nreplicas = replicas_array.count();
    
    for (int i=0; i<nreplicas; ++i)
    {
        if (not replicas_array[i].constData()->isPacked())
        {
            replicas_array[i].detach();
            replicas_array[i]->pack();
        }
    }
}

/** Unpack all of the replicas */
void Replicas::unpack()
{
    if (not this->isPacked())
        return;
        
    int nreplicas = replicas_array.count();
    
    for (int i=0; i<nreplicas; ++i)
    {
        if (replicas_array[i].constData()->isPacked())
        {
            replicas_array[i].detach();
            replicas_array[i]->unpack();
        }
    }
}

/** Pack all of the replicas to disk */
void Replicas::packToDisk()
{
    if (this->isPackedToDisk())
        return;

    int nreplicas = replicas_array.count();
    
    for (int i=0; i<nreplicas; ++i)
    {
        if (not replicas_array[i].constData()->isPackedToDisk())
        {
            replicas_array[i].detach();
            replicas_array[i]->packToDisk();
        }
    }
}

/** Pack all of the replicas to disk - they are placed in the 
    directory 'tempdir' */
void Replicas::packToDisk(const QString &tempdir)
{
    if (this->isPackedToDisk())
        return;

    int nreplicas = replicas_array.count();
    
    for (int i=0; i<nreplicas; ++i)
    {
        if (not replicas_array[i].constData()->isPackedToDisk())
        {
            replicas_array[i].detach();
            replicas_array[i]->packToDisk(tempdir);
        }
    }
}

/** Pack all of the replicas to memory */
void Replicas::packToMemory()
{
    if (this->isPackedToMemory())
        return;
        
    int nreplicas = replicas_array.count();
    
    for (int i=0; i<nreplicas; ++i)
    {
        if (not replicas_array[i].constData()->isPackedToMemory())
        {
            replicas_array[i].detach();
            replicas_array[i]->packToMemory();
        }
    }
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

/** Clear the statistics of all of the replicas */
void Replicas::clearStatistics()
{
    for (int i=0; i<this->nReplicas(); ++i)
    {
        this->_pvt_replica(i).clearStatistics();
    }
}

/** Tell all of the replicas that they must reevaluate
    their energies from scratch */
void Replicas::mustNowRecalculateFromScratch()
{
    for (int i=0; i<this->nReplicas(); ++i)
    {
        this->_pvt_replica(i).mustNowRecalculateFromScratch();
    }
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

/** Set the system and moves performed for the ith replica to 
    'system' and 'moves'
    
    \throw SireError::invalid_index
*/
void Replicas::setSystemAndMoves(int i, const System &system, const Moves &moves)
{
    this->setSystemAndMoves(i, SimStore(system,moves) );
}

/** Set the system and moves performed for the ith replica to 
    those contained in 'simstore'
    
    \throw SireError::invalid_index
*/
void Replicas::setSystemAndMoves(int i, const SimStore &simstore)
{
    replicas_array[ Index(i).map(nReplicas()) ]->setSystemAndMoves(simstore);
}

/** Set the system and moves to be performed for all of the replicas */
void Replicas::setSystemAndMoves(const System &system, const Moves &moves)
{
    this->setSystemAndMoves( SimStore(system,moves) );
}

/** Set the system and moves to be performed for all of the replicas */
void Replicas::setSystemAndMoves(const SimStore &simstore)
{
    Replicas old_state(*this);
    
    try
    {
        for (int i=0; i < this->nReplicas(); ++i)
        {
            replicas_array[i]->setSystemAndMoves(simstore);
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
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
