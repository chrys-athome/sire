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

#ifndef SIREMOVE_SIMPACKET_H
#define SIREMOVE_SIMPACKET_H

#include "SireCluster/workpacket.h"

#include "SireSystem/system.h"

#include "moves.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class SimPacket;
}

QDataStream& operator<<(QDataStream&, const SireMove::SimPacket&);
QDataStream& operator>>(QDataStream&, SireMove::SimPacket&);

namespace SireMove
{

using SireSystem::System;

/** This is a WorkPacket that is used to run part of a 
    simulation
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT SimPacket : public SireCluster::WorkPacketBase
{

friend QDataStream& ::operator<<(QDataStream&, const SimPacket&);
friend QDataStream& ::operator>>(QDataStream&, SimPacket&);

public:
    SimPacket();
    
    SimPacket(const System &system, const Moves &moves,
              int nmoves, bool record_stats=true);
              
    SimPacket(const System &system, const Moves &moves,
              int nmoves, int nmoves_per_chunk, bool record_stats=true);
              
    SimPacket(const SimPacket &other);
    
    ~SimPacket();
    
    SimPacket& operator=(const SimPacket &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<SimPacket>() );
    }
    
    const char* what() const
    {
        return SimPacket::typeName();
    }
    
    SimPacket* clone() const
    {
        return new SimPacket(*this);
    }
    
    bool operator==(const SimPacket &other) const;
    bool operator!=(const SimPacket &other) const;

    bool shouldPack() const;
    int approximatePacketSize() const;
    
    const System& system() const;
    const Moves& moves() const;
    
    int nMoves() const;
    int nCompleted() const;
    
    int nMovesPerChunk() const;
    
    bool recordingStatistics() const;
    
    bool hasFinished() const;

protected:
    float chunk();

private:
    /** The system being simulated */
    System sim_system;
    
    /** The moves to be applied to the system */
    MovesPtr sim_moves;
    
    /** The number of moves to run on the system */
    quint32 nmoves;
    
    /** The number of moves already run on the system */
    quint32 ncompleted;
    
    /** The number of moves to run per chunk */
    quint32 nmoves_per_chunk;
    
    /** Whether or not to record move statistics */
    bool record_stats;
};

}

Q_DECLARE_METATYPE( SireMove::SimPacket )

SIRE_EXPOSE_CLASS( SireMove::SimPacket )

SIRE_END_HEADER

#endif
