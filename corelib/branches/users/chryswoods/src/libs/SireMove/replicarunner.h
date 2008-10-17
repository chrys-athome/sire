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

#ifndef SIREMOVE_REPLICARUNNER_H
#define SIREMOVE_REPLICARUNNER_H

#include "SireBase/property.h"

#include "SireMPI/mpinodes.h"

#include "replicas.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class ReplicaRunner;

class BasicRepRunner;
class MPIRepRunner;

}

QDataStream& operator<<(QDataStream&, const SireMove::ReplicaRunner&);
QDataStream& operator>>(QDataStream&, SireMove::ReplicaRunner&);

QDataStream& operator<<(QDataStream&, const SireMove::BasicRepRunner&);
QDataStream& operator>>(QDataStream&, SireMove::BasicRepRunner&);

QDataStream& operator<<(QDataStream&, const SireMove::MPIRepRunner&);
QDataStream& operator>>(QDataStream&, SireMove::MPIRepRunner&);

namespace SireMove
{

using SireMPI::MPINodes;

/** This is the base class of all objects that are used to run the 
    simulations of a set of replicas
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT ReplicaRunner : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const SireMove::ReplicaRunner&);
friend QDataStream& ::operator>>(QDataStream&, SireMove::ReplicaRunner&);

public:
    ReplicaRunner();
    ReplicaRunner(const ReplicaRunner &other);
    
    virtual ~ReplicaRunner();
    
    static const char* typeName()
    {
        return "SireMove::ReplicaRunner";
    }
    
    virtual ReplicaRunner* clone() const=0;
    
    virtual Replicas run(const Replicas &replicas, bool record_stats=true) const=0;

    static const BasicRepRunner& null();
};

/** This is a basic Replica runner that just simulates the replicas
    sequentially in the current thread
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT BasicRepRunner
         : public SireBase::ConcreteProperty<BasicRepRunner,ReplicaRunner>
{

friend QDataStream& ::operator<<(QDataStream&, const BasicRepRunner&);
friend QDataStream& ::operator>>(QDataStream&, BasicRepRunner&);

public:
    BasicRepRunner();
    BasicRepRunner(const BasicRepRunner &other);
    
    ~BasicRepRunner();
    
    BasicRepRunner& operator=(const BasicRepRunner &other);
    
    bool operator==(const BasicRepRunner &other) const;
    bool operator!=(const BasicRepRunner &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<BasicRepRunner>() );
    }
    
    BasicRepRunner* clone() const
    {
        return new BasicRepRunner(*this);
    }

    Replicas run(const Replicas &replicas, bool record_stats=true) const;
};

/** This is a Replica runner that just simulates the replicas
    on as many MPI nodes as possible.
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT MPIRepRunner
         : public SireBase::ConcreteProperty<MPIRepRunner,ReplicaRunner>
{

friend QDataStream& ::operator<<(QDataStream&, const MPIRepRunner&);
friend QDataStream& ::operator>>(QDataStream&, MPIRepRunner&);

public:
    MPIRepRunner();
    MPIRepRunner(const MPINodes &nodes);
    
    MPIRepRunner(const MPIRepRunner &other);
    
    ~MPIRepRunner();
    
    MPIRepRunner& operator=(const MPIRepRunner &other);
    
    bool operator==(const MPIRepRunner &other) const;
    bool operator!=(const MPIRepRunner &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MPIRepRunner>() );
    }
    
    MPIRepRunner* clone() const
    {
        return new MPIRepRunner(*this);
    }

    Replicas run(const Replicas &replicas, bool record_stats=true) const;

private:
    /** The nodes on which to run the simulations */
    MPINodes nodes;
};

typedef SireBase::PropPtr<ReplicaRunner> RepRunnerPtr;

}

Q_DECLARE_METATYPE( SireMove::BasicRepRunner )
Q_DECLARE_METATYPE( SireMove::MPIRepRunner )

SIRE_EXPOSE_CLASS( SireMove::ReplicaRunner )
SIRE_EXPOSE_CLASS( SireMove::BasicRepRunner )
SIRE_EXPOSE_CLASS( SireMove::MPIRepRunner )

SIRE_EXPOSE_PROPERTY( SireMove::RepRunnerPtr, SireMove::ReplicaRunner )

SIRE_END_HEADER

#endif
