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

#ifndef SIREMOVE_REPLICAS_H
#define SIREMOVE_REPLICAS_H

#include "SireBase/property.h"
#include "SireBase/sharedpolypointer.hpp"

#include "SireSystem/system.h"
#include "SireMove/moves.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class Replica;
class Replicas;
}

QDataStream& operator<<(QDataStream&, const SireMove::Replica&);
QDataStream& operator>>(QDataStream&, SireMove::Replica&);

QDataStream& operator<<(QDataStream&, const SireMove::Replicas&);
QDataStream& operator>>(QDataStream&, SireMove::Replicas&);

namespace SireMove
{

/** This class holds information about a single replica. A replica is
    a system, plus information about the moves to be applied to that
    system
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT Replica 
            : public SireBase::ConcreteProperty<Replica,SireBase::PropertyBase>
{

friend class Replicas;  //so can call editing functions

friend QDataStream& ::operator<<(QDataStream&, const Replica&);
friend QDataStream& ::operator>>(QDataStream&, Replica&);

public:
    Replica();
 
    Replica(const Replica &other);

    virtual ~Replica();
    
    Replica& operator=(const Replica &other);
    
    bool operator==(const Replica &other) const;
    bool operator!=(const Replica &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Replica>() );
    }

    virtual Replica* clone() const
    {
        return new Replica(*this);
    }
    
    const System& system() const;
    const MovesBase& moves() const;
    
    int nMoves() const;
    
    bool recordStatistics() const;

    static void swapSystems(Replica &rep0, Replica &rep1);

protected:
    virtual void setSystem(const System &system);
    virtual void setMoves(const MovesBase &moves);
    virtual void setNMoves(int nmoves);
    virtual void setRecordStatistics(bool recordstats);
    
private:
    /** The system being simulated */
    System sim_system;
    
    /** The moves to be performed on the system */
    Moves sim_moves;
    
    /** The number of moves to perform on the system */
    quint32 sim_nmoves;
    
    /** Whether or not to record statistics during the moves */
    bool sim_recordstats;
};

/** This class holds a set of systems to be simulated (a set of replicas)
    and information about the moves to be applied to each replica

    @author Christopher Woods
*/
class SIREMOVE_EXPORT Replicas
         : public SireBase::ConcreteProperty<Replicas,SireBase::PropertyBase>
{

friend QDataStream& ::operator<<(QDataStream&, const Replicas&);
friend QDataStream& ::operator>>(QDataStream&, Replicas&);

public:
    Replicas();
    Replicas(int n);
    Replicas(const System &system, int n);
    Replicas(const QVector<System> &systems);
    
    Replicas(const Replicas &other);
    
    virtual ~Replicas();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Replicas>() );
    }
    
    virtual Replicas* clone() const
    {
        return new Replicas(*this);
    }
    
    virtual Replicas& operator=(const Replicas &other);
    
    bool operator==(const Replicas &other) const;
    bool operator!=(const Replicas &other) const;
    
    const Replica& operator[](int i) const;
    
    const Replica& at(int i) const;
    
    bool isEmpty();
    
    int nReplicas() const;
    int count() const
    int size();
    
    virtual void setReplicas(const Replicas &replicas);
    
    virtual void setReplica(const Replica &replica);
    virtual void setReplica(int i, const Replica &replica);
    
    virtual void setSystem(const System &system);
    virtual void setSystem(int i, const System &system);
    
    virtual void setMoves(const MovesBase &moves);
    virtual void setMoves(int i, const MovesBase &moves);
    
    virtual void setNMoves(int nmoves);
    virtual void setNMoves(int i, int nmoves);
    
    virtual void setRecordStatistics(bool record_stats);
    virtual void setRecordStatistics(int i, bool record_stats);

protected:
    Replica& _pvt_replica(int i);

private:
    /** The array of all of the replicas */
    QVector< SireBase::SharedPolyPointer<Replica> > replicas_array;
};

}

Q_DECLARE_METATYPE( SireMove::Replica )
Q_DECLARE_METATYPE( SireMove::Replicas )

SIRE_EXPOSE_CLASS( SireMove::Replica )
SIRE_EXPOSE_CLASS( SireMove::Replicas )

SIRE_END_HEADER

#endif
