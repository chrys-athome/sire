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

#ifndef SIREMOVE_REPEXMOVE_H
#define SIREMOVE_REPEXMOVE_H

#include "SireMaths/rangenerator.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class RepExMove;
}

QDataStream& operator<<(QDataStream&, const SireMove::RepExMove&);
QDataStream& operator>>(QDataStream&, SireMove::RepExMove&);

namespace SireMPI
{
class MPINodes;
}

namespace SireMove
{

class RepExReplicas;
class RepExReplica;

class ReplicaRunner;

using SireMaths::RanGenerator;
using SireMPI::MPINodes;

/** This class is used to perform replica exchange moves on a collection
    of RepExReplicas. Each move involves running a block of sampling
    on each of the replicas, and then performing replice exchange swaps
    and tests between pairs.
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT RepExMove
{

friend QDataStream& ::operator<<(QDataStream&, const RepExMove&);
friend QDataStream& ::operator>>(QDataStream&, RepExMove&);

public:
    RepExMove();
    
    RepExMove(const RepExMove &other);
    
    ~RepExMove();
    
    RepExMove& operator=(const RepExMove &other);

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<RepExMove>() );
    }
    
    const char* what() const
    {
        return RepExMove::typeName();
    }

    bool operator==(const RepExMove &other) const;
    bool operator!=(const RepExMove &other) const;
    
    quint32 nAttempted() const;
    quint32 nAccepted() const;
    quint32 nRejected() const;
    
    quint32 nMoves() const;
    
    double acceptanceRatio() const;
    
    void clearMoveStatistics();
    
    void setGenerator(const RanGenerator &generator);
    const RanGenerator& generator() const;

    void move(RepExReplicas &replicas, int nmoves, bool record_stats=true);
    
    void move(RepExReplicas &replicas, int nmoves,
              const MPINodes &nodes, bool record_stats=true);
              
    void move(RepExReplicas &replicas, int nmoves,
              const ReplicaRunner &reprunner, bool record_stats=true);

protected:
    void testAndSwap(RepExReplicas &replicas);
    bool testAndSwap(RepExReplica &replica0, RepExReplica &replica1);

    void acceptedMove();
    void rejectedMove();

private:
    /** The random number generator used to accept or reject the moves */
    RanGenerator rangenerator;
    
    /** The total number of replica exchange supra-moves attempted */
    quint32 nmoves;
    
    /** The number of times a replica exchange move has been accepted */
    quint32 naccept;
    
    /** The number of times a replica exchange move has been rejected */
    quint32 nreject;
};

}

Q_DECLARE_METATYPE( SireMove::RepExMove )

SIRE_EXPOSE_CLASS( SireMove::RepExMove )

SIRE_END_HEADER

#endif
