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

#ifndef SIREMOVE_RESET_H
#define SIREMOVE_RESET_H

#include <QVector>
#include <QMap>
#include <QUuid>

#include "SireBase/property.h"

#include "SireCAS/symbol.h"

#include "SireSystem/system.h"

#include "moves.h"
#include "simulation.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class ReplicaData;
class Replica;
class ReplicaSim;
class RESet;
}

QDataStream& operator<<(QDataStream&, const SireMove::ReplicaData&);
QDataStream& operator>>(QDataStream&, SireMove::ReplicaData&);

QDataStream& operator<<(QDataStream&, const SireMove::Replica&);
QDataStream& operator>>(QDataStream&, SireMove::Replica&);

QDataStream& operator<<(QDataStream&, const SireMove::ReplicaSim&);
QDataStream& operator>>(QDataStream&, SireMove::ReplicaSim&);

QDataStream& operator<<(QDataStream&, const SireMove::RESet&);
QDataStream& operator>>(QDataStream&, SireMove::RESet&);

namespace SireMPI
{
class MPINode;
}

namespace SireMove
{

class ReplicaSim;

/** This class holds all of the data about a replica (i.e. everything
    except for the system and moves within a replica)
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT ReplicaData
{

friend QDataStream& ::operator<<(QDataStream&, const ReplicaData&);
friend QDataStream& ::operator>>(QDataStream&, ReplicaData&);

public:
    ~ReplicaData();
    
    static const char* typeName()
    {
        return "SireMove::ReplicaData";
    }
    
    const char* what() const
    {
        return ReplicaData::typeName();
    }
    
    quint32 nMoves() const;
    bool recordStatistics() const;
    
    const Symbol& symbol() const;
    
    double parameter() const;
    double parameter(int delta) const;
    
    const QVector<double>& parameters() const;
    
    const QUuid& parentID() const;
    int replicaID() const;

    bool isCompatibleWith(const ReplicaData &other) const;
    void assertCompatibleWith(const ReplicaData &other) const;

protected:
    ReplicaData();
    ReplicaData(const QUuid &parentid, quint32 replicaid,
                const QVector<double> &replica_parameters,
                const Symbol &replica_symbol,
                quint32 nmoves, bool record_stats);

    ReplicaData(const ReplicaData &other);

    ReplicaData& operator=(const ReplicaData &other);
    
    bool operator==(const ReplicaData &other) const;
    bool operator!=(const ReplicaData &other) const;

private:
    /** The values of the replica exchange parameters for each level */
    QVector<double> replica_parameters;
    
    /** The symbol that represents this parameter in the system. This may
        be null of the RE parameter doesn't change the system (e.g. as in
        a parallel tempering simulation) */
    Symbol replica_symbol;
    
    /** The ID of the parent set that contains this replica */
    QUuid parent_uid;
    
    /** The index of this replica exchange level within that set */
    quint32 replica_id;
    
    /** The number of moves to run */
    quint32 nmoves;
    
    /** Whether or not to record statistics for this replica */
    bool record_stats;
};

/** This class holds all of the information about a replica
    in a replica exchange simulation. A replica is a particular
    level in a replica exchange simulation, not a particular
    system. Individual systems will be swapped between replicas
    during the simulation.
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT Replica : public ReplicaData
{

friend QDataStream& ::operator<<(QDataStream&, const SireMove::Replica&);
friend QDataStream& ::operator>>(QDataStream&, Replica&);

friend class RESet;  // so can call private constructor

public:
    Replica();
    
    Replica(ReplicaSim replica);
    
    Replica(const Replica &other);
    
    ~Replica();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Replica>() );
    }
    
    const char* what() const
    {
        return Replica::typeName();
    }
    
    Replica* clone() const
    {
        return new Replica(*this);
    }
    
    Replica& operator=(const Replica &other);
    
    bool operator==(const Replica &other) const;
    bool operator!=(const Replica &other) const;
    
    const System& system() const;
    const MovesBase& moves() const;

    ReplicaSim run(bool record_stats=true);
    ReplicaSim runBG(bool record_stats=true);
    ReplicaSim run(const SireMPI::MPINode &node, bool record_stats=true);

    void update(const Replica &replica);
    void update(ReplicaSim replica);
    
protected:
    Replica(const QUuid &parentid, quint32 replicaid,
            const QVector<double> &replica_parameters,
            const Symbol &replica_symbol,
            const System &system, const Moves &moves,
            quint32 nmoves, bool record_stats);
    
private:
    /** Copy of the system of molecules */
    System sim_system;
    
    /** Copy of the moves that have been applied to this replica level to data */
    Moves sim_moves;
};

/** This class holds a replica that is currently being simulated

    @author Christopher Woods
*/
class SIREMOVE_EXPORT ReplicaSim : public Simulation, public ReplicaData
{

friend QDataStream& ::operator<<(QDataStream&, const ReplicaSim&);
friend QDataStream& ::operator>>(QDataStream&, ReplicaSim&);

friend class Replica; //so can call protected constructor

public:
    ReplicaSim();
    ReplicaSim(const ReplicaSim &other);

    ~ReplicaSim();
    
    ReplicaSim& operator=(const ReplicaSim &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<ReplicaSim>() );
    }
    
    const char* what() const
    {
        return ReplicaSim::typeName();
    }
    
    ReplicaSim* clone() const
    {
        return new ReplicaSim(*this);
    }
    
    bool operator==(const ReplicaSim &other) const;
    bool operator!=(const ReplicaSim &other) const;

protected:
    ReplicaSim(const Replica &replica, const Simulation &simulation);
};

/** This class holds the set of replicas (systems + moves) that
    are used during a replica exchange simulation. A replica
    exchange move operates on this set, advancing each member
    of this set by a specified number of moves, and then 
    performing replica exchange tests to see if pairs of replicas
    in the set should be swapped.
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT RESet 
            : public SireBase::ConcreteProperty<RESet,SireBase::PropertyBase>
{

friend QDataStream& ::operator<<(QDataStream&, const RESet&);
friend QDataStream& ::operator>>(QDataStream&, RESet&);

public:
    RESet();
    
    RESet(const QVector<double> &replica_values);
    RESet(const System &system, const QVector<double> &replica_values);
    
    RESet(const Symbol &symbol, const QVector<double> &replica_values);
    RESet(const Symbol &symbol, const System &system,
          const QVector<double> &replica_values);
    
    RESet(const QMap<double,System> &replicas);
    RESet(const Symbol &symbol, const QMap<double,System> &replicas);
    
    RESet(const RESet &other);
    
    ~RESet();
    
    RESet& operator=(const RESet &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<RESet>() );
    }
    
    RESet* clone() const
    {
        return new RESet(*this);
    }
    
    bool operator==(const RESet &other) const;
    bool operator!=(const RESet &other) const;

    Replica operator[](int i) const;

    Replica at(int i) const;

    const QUuid& ID() const;

    bool isEmpty() const;

    int count() const;
    int size() const;
    int nReplicas() const;

    void setSystem(int i, const System &system);
    void setSystem(const System &system);
    
    void setMoves(int i, const MovesBase &moves);
    void setMoves(const MovesBase &moves);

    void setNMoves(int i, int nmoves);
    void setNMoves(int nmoves);
    
    void setRecordStatistics(int i, bool value);
    void setRecordStatistics(bool value);

    void update(const Replica &replica);
    void update(const ReplicaSim &replica);
    
    void swap(int i, int j);

private:
    void _pvt_create(const QMap<double,System> &replicas);
    void _pvt_updateSystems();

    /** The ID of this replica set - this is used to identify the 
        set from which a replica came */
    QUuid uid;

    /** Each of the replicas in this set */
    QVector<System> replica_systems;
    
    /** Each of the moves applied to each of the replicas */
    QVector<Moves> replica_moves;
    
    /** The number of moves to apply to each replica */
    QVector<quint32> replica_nmoves;
    
    /** The value of the replica exchange parameter for each replica */
    QVector<double> replica_parameters;

    /** A mask that can be used to prevent statistics being
        recorded for particular replicas, even if this is requested
        when running the RE move */
    QVector<bool> replica_record_stats;
    
    /** The symbol associated with the replica exchange parameter.
        This may be null, if there is no symbol (e.g. if we are replicating
        over temperature, so there is no change in replica with parameter) */
    Symbol replica_symbol;
};

}

Q_DECLARE_METATYPE( SireMove::Replica )
Q_DECLARE_METATYPE( SireMove::ReplicaSim )
Q_DECLARE_METATYPE( SireMove::RESet )

SIRE_EXPOSE_CLASS( SireMove::ReplicaData )
SIRE_EXPOSE_CLASS( SireMove::Replica )
SIRE_EXPOSE_CLASS( SireMove::ReplicaSim )
SIRE_EXPOSE_CLASS( SireMove::RESet )

SIRE_END_HEADER

#endif
