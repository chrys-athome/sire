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

#include "reset.h"

#include "SireID/index.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireMol;
using namespace SireCAS;
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
        << replica.replica_parameters << replica.replica_symbol
        << replica.parent_uid << replica.replica_id
        << replica.nmoves << replica.record_stats;
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, Replica &replica)
{
    VersionID v = readHeader(ds, r_replica);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> replica.sim_system >> replica.sim_moves
            >> replica.replica_parameters >> replica.replica_symbol
            >> replica.parent_uid >> replica.replica_id
            >> replica.nmoves >> replica.record_stats;
    }
    else
        throw version_error( v, "1", r_replica, CODELOC );
    
    return ds;
}

/** Create a null replica */
Replica::Replica() : replica_id(0), nmoves(0), record_stats(false)
{}

/** Private constructor used by RESet to create a replica with 
    the specified information */
Replica::Replica(const QUuid &parentid, quint32 replicaid,
                 const QVector<double> &replicaparams,
                 const Symbol &replicasymbol,
                 const System &system, const Moves &moves,
                 quint32 n_moves, bool recording_stats)
        : sim_system(system), sim_moves(moves), 
          replica_parameters(replicaparams), replica_symbol(replicasymbol),
          parent_uid(parentid), replica_id(replicaid),
          nmoves(n_moves), record_stats(recording_stats)
{
    BOOST_ASSERT( replica_id < quint32(replica_parameters.count()) );
}

/** Copy constructor */
Replica::Replica(const Replica &other)
        : sim_system(other.sim_system), sim_moves(other.sim_moves),
          replica_parameters(other.replica_parameters),
          replica_symbol(other.replica_symbol),
          parent_uid(other.parent_uid), replica_id(other.replica_id),
          nmoves(other.nmoves), record_stats(other.record_stats)
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
        replica_parameters = other.replica_parameters;
        replica_symbol = other.replica_symbol;
        parent_uid = other.parent_uid;
        replica_id = other.replica_id;
        nmoves = other.nmoves;
        record_stats = other.record_stats;
    }
    
    return *this;
}

/** Comparison operator */
bool Replica::operator==(const Replica &other) const
{
    return parent_uid == other.parent_uid and
           replica_id == other.replica_id and
           record_stats == other.record_stats and
           replica_parameters == other.replica_parameters and
           replica_symbol == other.replica_symbol and
           nmoves == other.nmoves and
           sim_moves == other.sim_moves and
           sim_system == other.sim_system;
}

/** Comparison operator */
bool Replica::operator!=(const Replica &other) const
{
    return not this->operator==(other);
}

/** Return the system currently at this replica level */
const System& Replica::system() const
{
    return sim_system;
}

/** Return the moves applied at this replica level to data */
const MovesBase& Replica::moves() const
{
    return sim_moves;
}

/** Return the number of moves to apply at this replica level */
quint32 Replica::nMoves() const
{
    return nmoves;
}

/** Return whether to record statistics at this level. */
bool Replica::recordingStatistics() const
{
    return record_stats;
}

/** Return the symbol used for the replica exchange parameter
    for the set of replicas - this may be null if there is no
    symbol associated with the parameter, e.g. if these are
    replicas over temperature */
const Symbol& Replica::symbol() const
{
    return replica_symbol;
}

/** Return the value of the replica exchange parameter for 
    this replica */
double Replica::parameter() const
{
    if (replica_parameters.isEmpty())
        return 0;
    else
        return replica_parameters.constData()[replica_id];
}

/** Return the value of the replica exchange parameter for 
    the replica 'delta' above this replica. If this is replica
    3 of 5, then parameter(1) will return the parameter for
    replica 4 of 5, while parameter(-1) will return the parameter
    for replica 2 of 5.
    
    If 'delta' moves beyond a valid replica, then the parameter
    for the last replica is returned if we move after the last
    replica, or for the first replica if we move before the
    first replica, e.g. parameter(10) will return the parameter
    for replica 5 of 5, while parameter(-10) will return the
    parameter for replica 1 of 5 */
double Replica::parameter(int delta) const
{
    if (replica_parameters.isEmpty())
        return 0;
        
    int i = replica_id + delta;
    
    if (i >= replica_parameters.count())
        return replica_parameters.constData()[replica_parameters.count()-1];
        
    else if (i < 0)
        return replica_parameters.constData()[0];
        
    else
        return replica_parameters.constData()[i];
}

/** Return the array of the value of all of the replica parameters
    for each of the levels */
const QVector<double>& Replica::parameters() const
{
    return replica_parameters;
}

/** Return the ID of this replica in the set - this is the index of the
    replica in the array of replicas. This returns -1 if this is a
    null replica */
int Replica::replicaID() const
{
    if (replica_parameters.isEmpty())
        return -1;
        
    else
        return replica_id;
}

/** Return the unique ID of the parent RESet of this replica */
const QUuid& Replica::parentID() const
{
    return parent_uid;
}

///////
/////// Implementation of RESet
///////

static const RegisterMetaType<RESet> r_reset;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const RESet &reset)
{
    writeHeader(ds, r_reset, 1);
    
    SharedDataStream sds(ds);
    
    quint32 nreplicas = reset.nReplicas();
    
    sds << nreplicas;
    
    for (quint32 i=0; i<nreplicas; ++i)
    {
        sds << reset[i];
    }
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, RESet &reset)
{
    VersionID v = readHeader(ds, r_reset);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        quint32 nreplicas;
        sds >> nreplicas;
        
        if (nreplicas == 0)
        {
            reset = RESet();
            return ds;
        }
        
        QList<Replica> replicas;
        
        for (quint32 i=0; i<nreplicas; ++i)
        {
            Replica replica;
            sds >> replica;
            replicas.append(replica);
        }
        
        RESet new_reset;
        
        new_reset.uid = replicas.at(0).parentID();
        new_reset.replica_parameters = replicas.at(0).parameters();
        new_reset.replica_symbol = replicas.at(0).symbol();
        
        BOOST_ASSERT( new_reset.replica_parameters.count() == int(nreplicas) );
        
        new_reset.replica_systems = QVector<System>(nreplicas);
        new_reset.replica_moves = QVector<Moves>(nreplicas);
        new_reset.replica_nmoves = QVector<quint32>(nreplicas);
        new_reset.replica_record_stats = QVector<bool>(nreplicas);
        
        for (quint32 i=0; i<nreplicas; ++i)
        {
            const Replica &replica = replicas.at(i);
            
            new_reset.replica_systems[i] = replica.system();
            new_reset.replica_moves[i] = replica.moves();
            new_reset.replica_nmoves[i] = replica.nMoves();
            new_reset.replica_record_stats[i] = replica.recordingStatistics();
        }
    }
    else
        throw version_error(v, "1", r_reset, CODELOC);
        
    return ds;
}

/** Construct an empty set */
RESet::RESet() : ConcreteProperty<RESet,PropertyBase>()
{}

/** Construct a set of replicas that have the specified replica
    values. Note that the replica values will be sorted from lowest
    to highest */
RESet::RESet(const QVector<double> &replica_values)
      : ConcreteProperty<RESet,PropertyBase>(),
        replica_systems( QVector<System>(replica_values.count()) ),
        replica_moves( QVector<Moves>(replica_values.count()) ),
        replica_nmoves( QVector<quint32>(replica_values.count(), 0) ),
        replica_parameters(replica_values),
        replica_record_stats( QVector<bool>(replica_values.count(), true) )
{
    if (not replica_values.isEmpty())
    {
        qSort(replica_parameters);
        uid = QUuid::createUuid();
    }
}

/** Construct a set of replicas of the system 'system', with 
    the replicas having the supplied replica values (which
    will be sorted from lowest to highest) */
RESet::RESet(const System &system, const QVector<double> &replica_values)
      : ConcreteProperty<RESet,PropertyBase>(),
        replica_systems( QVector<System>(replica_values.count(), system) ),
        replica_moves( QVector<Moves>(replica_values.count()) ),
        replica_nmoves( QVector<quint32>(replica_values.count(), 0) ),
        replica_parameters(replica_values),
        replica_record_stats( QVector<bool>(replica_values.count(), true) )
{
    if (not replica_values.isEmpty())
    {
        qSort(replica_parameters);
        uid = QUuid::createUuid();
    }
}

/** Construct a set of replicas with 
    the replicas having the supplied value of the component identified
    by the symbol 'symbol' */
RESet::RESet(const Symbol &symbol, const QVector<double> &replica_values)
      : ConcreteProperty<RESet,PropertyBase>(),
        replica_systems( QVector<System>(replica_values.count()) ),
        replica_moves( QVector<Moves>(replica_values.count()) ),
        replica_nmoves( QVector<quint32>(replica_values.count(), 0) ),
        replica_parameters(replica_values),
        replica_record_stats( QVector<bool>(replica_values.count(), true) ),
        replica_symbol(symbol)
{
    if (not replica_values.isEmpty())
    {
        qSort(replica_parameters);
        uid = QUuid::createUuid();
    }
}

void RESet::_pvt_updateSystems()
{
    if (replica_symbol.isNull() or replica_parameters.isEmpty())
        return;
        
    int nreplicas = replica_parameters.count();
    
    for (int i=0; i<nreplicas; ++i)
    {
        replica_systems[i].setComponent(replica_symbol,
                                        replica_parameters.at(i));
    }
}

/** Construct a set of replicas of the system 'system', with the
    replicas having the supplied value of the component identified
    by the symbol 'symbol' */
RESet::RESet(const Symbol &symbol, const System &system,
             const QVector<double> &replica_values)
      : ConcreteProperty<RESet,PropertyBase>(),
        replica_systems( QVector<System>(replica_values.count(), system) ),
        replica_moves( QVector<Moves>(replica_values.count()) ),
        replica_nmoves( QVector<quint32>(replica_values.count(), 0) ),
        replica_parameters(replica_values),
        replica_record_stats( QVector<bool>(replica_values.count(), true) ),
        replica_symbol(symbol)
{
    if (not replica_values.isEmpty())
    {
        qSort(replica_parameters);
        uid = QUuid::createUuid();
        this->_pvt_updateSystems();
    }
}

void RESet::_pvt_create(const QMap<double,System> &replicas)
{
    if (replicas.isEmpty())
        return;
    
    uid = QUuid::createUuid();
    
    replica_parameters = replicas.keys().toVector();
    qSort(replica_parameters);
    
    int nreplicas = replica_parameters.count();
    
    replica_systems = QVector<System>(nreplicas);
    replica_moves = QVector<Moves>(nreplicas);
    replica_nmoves = QVector<quint32>(nreplicas, 0);
    replica_record_stats = QVector<bool>(nreplicas, true);
    
    for (int i=0; i<nreplicas; ++i)
    {
        replica_systems[i] = replicas.value( replica_parameters.at(i) );
    }
}

/** Construct a set of replicas where each system is associated with
    a different value of the replica exchange parameter. The parameters
    and systems will be sorted from lowest to highest */
RESet::RESet(const QMap<double,System> &replicas)
      : ConcreteProperty<RESet,PropertyBase>()
{
    this->_pvt_create(replicas);
}

/** Construct a set of replicas where each system is associated with
    a different value of the replica exchange parameter represented
    by the symbol 'symbol'. The parameters
    and systems will be sorted from lowest to highest */
RESet::RESet(const Symbol &symbol, const QMap<double,System> &replicas)
      : ConcreteProperty<RESet,PropertyBase>()
{
    this->_pvt_create(replicas);
    
    if (not replica_parameters.isEmpty())
    {
        replica_symbol = symbol;
        this->_pvt_updateSystems();
    }
}

/** Copy constructor */
RESet::RESet(const RESet &other)
      : ConcreteProperty<RESet,PropertyBase>(other),
        uid(other.uid),
        replica_systems(other.replica_systems),
        replica_moves(other.replica_moves),
        replica_nmoves(other.replica_nmoves),
        replica_parameters(other.replica_parameters),
        replica_record_stats(other.replica_record_stats),
        replica_symbol(other.replica_symbol)
{}

/** Destructor */
RESet::~RESet()
{}

/** Copy assignment operator */
RESet& RESet::operator=(const RESet &other)
{
    if (this != &other)
    {
        uid = other.uid;
        replica_systems = other.replica_systems;
        replica_moves = other.replica_moves;
        replica_nmoves = other.replica_nmoves;
        replica_parameters = other.replica_parameters;
        replica_record_stats = other.replica_record_stats;
        replica_symbol = other.replica_symbol;
    }
    
    return *this;
}

/** Comparison operator */
bool RESet::operator==(const RESet &other) const
{
    return uid == other.uid and
           replica_systems == other.replica_systems and
           replica_moves == other.replica_moves and
           replica_nmoves == other.replica_nmoves and
           replica_record_stats == other.replica_record_stats;
}

/** Comparison operator */
bool RESet::operator!=(const RESet &other) const
{
    return not this->operator==(other);
}

/** Return the unique ID of this replica set. */
const QUuid& RESet::ID() const
{
    return uid;
}

/** Return whether or not this set is empty */
bool RESet::isEmpty() const
{
    return replica_parameters.isEmpty();
}

/** Return the number of replicas in this set */
int RESet::nReplicas() const
{
    return replica_parameters.count();
}

/** Return the number of replicas in this set */
int RESet::count() const
{
    return this->nReplicas();
}

/** Return the number of replicas in this set */
int RESet::size() const
{
    return this->nReplicas();
}

/** Return the ith replica in this set (this wraps, so RESet[-1] will 
    return the last replica)
    
    \throw SireError::invalid_index
*/
Replica RESet::operator[](int i) const
{
    i = Index(i).map( this->nReplicas() );
    
    return Replica( uid, quint32(i), replica_parameters, replica_symbol,
                    replica_systems.at(i), replica_moves.at(i),
                    replica_nmoves.at(i), replica_record_stats.at(i) );
}

/** Return the ith replica in this set (this wraps, so RESet[-1] will 
    return the last replica)
    
    \throw SireError::invalid_index
*/
Replica RESet::at(int i) const
{
    return this->operator[](i);
}

/** Set the system at replica level 'i' to be equal to the system 'system'.
    This uses a wrappable index, so RESet::setSystem(-1, system) will
    set the last system of the set equal to 'system'
    
    \throw SireError::invalid_index
*/
void RESet::setSystem(int i, const System &system)
{
    i = Index(i).map( this->nReplicas() );

    System new_system = system;
    if (not replica_symbol.isNull())
    {
        new_system.setComponent(replica_symbol, replica_parameters.at(i));
    }

    replica_systems[i] = new_system;
}

/** Set the system at all replica levels equal to 'system'. This is
    useful if you want to initialise the replica exchange simulation
    to start with all replicas from the same place */
void RESet::setSystem(const System &system)
{
    QVector<System> old_systems = replica_systems;

    try
    {
        for (int i=0; i<this->nReplicas(); ++i)
        {
            replica_systems[i] = system;
        }
    
        this->_pvt_updateSystems();
    }
    catch(...)
    {
        replica_systems = old_systems;
        throw;
    }
}

/** Set the moves to be used at replica level 'i' to be equal to the 'moves'.
    This uses a wrappable index, so RESet::setMoves(-1, moves) will
    set the moves of the last replica of the set equal to 'moves'
    
    \throw SireError::invalid_index
*/
void RESet::setMoves(int i, const MovesBase &moves)
{
    i = Index(i).map( this->nReplicas() );
    replica_moves[i] = moves;
}

/** Set the moves at all replica levels equal to 'moves'. This is
    useful if you want to initialise the replica exchange simulation
    to start with the same set of moves for each replica */
void RESet::setMoves(const MovesBase &moves)
{
    for (int i=0; i<this->nReplicas(); ++i)
    {
        replica_moves[i] = moves;
    }
}

/** Set the number of moves to be used at replica level 'i' to be equal to 'nmoves'.
    This uses a wrappable index, so RESet::setNMoves(-1, nmoves) will
    set the number of moves of the last replica of the set equal to 'nmoves'
    
    \throw SireError::invalid_index
*/
void RESet::setNMoves(int i, int nmoves)
{
    i = Index(i).map( this->nReplicas() );
    replica_nmoves[i] = quint32( qMax(0, nmoves) );
}

/** Set the number of moves at all replica levels equal to 'nmoves'. This is
    useful if you want to initialise the replica exchange simulation
    to use the same number of moves for each replica */
void RESet::setNMoves(int nmoves)
{
    nmoves = qMax(0, nmoves);
    
    for (int i=0; i<this->nReplicas(); ++i)
    {
        replica_nmoves[i] = nmoves;
    }
}

/** Set whether or not statistics will be recorded at replica level 'i'.
    This uses a wrappable index, so RESet::setRecordStatistics(-1, true) will
    set turn on statistics collection for the last replica in the set.
    Note that this does not guarantee that statistics will definitely
    be recorded (they may be switched off by replica exchange move itself),
    but if recordStatistics() if false here, then you can guarantee that
    statistics will not be recorded for this replica level.
    
    \throw SireError::invalid_index
*/
void RESet::setRecordStatistics(int i, bool value)
{
    i = Index(i).map( this->nReplicas() );
    replica_record_stats[i] = value;
}

/** Set whether or not statistics will be recorded for all of the 
    replicas in this set. This is useful to initialise all of the 
    replicas in one go. 

    Note that this does not guarantee that statistics will definitely
    be recorded (they may be switched off by replica exchange move itself),
    but if recordStatistics() if false here, then you can guarantee that
    statistics will not be recorded for this replica level.
*/
void RESet::setRecordStatistics(bool value)
{
    for (int i=0; i<this->nReplicas(); ++i)
    {
        replica_record_stats[i] = value;
    }
}

/** Update the replica in this set with the system and moves
    copied from the replica 'replica'
    
    \throw SireError::incompatible_error
*/
void RESet::update(const Replica &replica)
{
    if (uid != replica.parentID())
        throw SireError::incompatible_error( QObject::tr(
            "Cannot update this RESet (with ID %1) as the passed replica "
            "comes from a different RESet (with ID %2)")
                .arg(uid.toString(), replica.parentID().toString()), CODELOC );
                
    BOOST_ASSERT( replica_symbol == replica.symbol() );
    BOOST_ASSERT( replica_parameters == replica.parameters() );
    
    BOOST_ASSERT( replica_parameters.count() > replica.replicaID() );
    
    replica_systems[replica.replicaID()] = replica.system();
    replica_moves[replica.replicaID()] = replica.moves();
}

/** Swap the systems between replica levels i and j. These are
    wrappable indicies, so RESet::swap(0,-1) would swap the 
    first and last replicas in this set
    
    \throw SireError::invalid_index
*/
void RESet::swap(int i, int j)
{
    i = Index(i).map( this->nReplicas() );
    j = Index(j).map( this->nReplicas() );
    
    if (i == j)
        return;
        
    QVector<System> new_systems;
    
    new_systems[i] = replica_systems.at(j);
    new_systems[j] = replica_systems.at(i);
    
    if (not replica_symbol.isNull())
    {
        new_systems[i].setComponent(replica_symbol, replica_parameters.at(i));
        new_systems[j].setComponent(replica_symbol, replica_parameters.at(j));
    }
    
    replica_systems = new_systems;
}
