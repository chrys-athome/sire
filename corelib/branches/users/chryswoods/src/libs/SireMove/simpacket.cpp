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

#include "simpacket.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireCluster;
using namespace SireStream;

static const RegisterMetaType<SimPacket> r_simpacket;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const SimPacket &simpacket)
{
    writeHeader(ds, r_simpacket, 2);
    
    SharedDataStream sds(ds);
    
    QMutexLocker lkr( const_cast<QMutex*>(&(simpacket.packing_mutex)) );
    
    simpacket.pack();
    
    sds << simpacket.compressed_system_and_moves
        << simpacket.nmoves << simpacket.ncompleted
        << simpacket.nmoves_per_chunk << simpacket.record_stats;
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, SimPacket &simpacket)
{
    VersionID v = readHeader(ds, r_simpacket);
    
    if (v == 2)
    {
        SharedDataStream sds(ds);
        
        sds >> simpacket.compressed_system_and_moves
            >> simpacket.nmoves >> simpacket.ncompleted
            >> simpacket.nmoves_per_chunk >> simpacket.record_stats;
            
        simpacket.sim_system = System();
        simpacket.sim_moves = MovesPtr();
    }
    else if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> simpacket.sim_system >> simpacket.sim_moves
            >> simpacket.nmoves >> simpacket.ncompleted
            >> simpacket.nmoves_per_chunk >> simpacket.record_stats;
            
        simpacket.compressed_system_and_moves = QByteArray();
    }
    else
        throw version_error( v, "1", r_simpacket, CODELOC );
        
    return ds;
}

/** Null constructor */
SimPacket::SimPacket() 
          : WorkPacketBase(), nmoves(0), ncompleted(0),
            nmoves_per_chunk(0), record_stats(true)
{}

/** Construct a workpacket that runs 'nmoves' of the Moves 'moves' on the 
    passed System 'system', optionally recording simulation statistics
    if 'record_stats' is true */
SimPacket::SimPacket(const System &system, const Moves &moves,
                     int n_moves, bool recording_stats)
          : WorkPacketBase(), sim_system(system), sim_moves(moves),
            ncompleted(0), nmoves_per_chunk(100), record_stats(recording_stats)
{
    if (n_moves > 0)
        nmoves = n_moves;
    else
        nmoves = 0;
}
          
/** Construct a workpacket that runs 'nmoves' of the Moves 'moves' on the 
    passed System 'system', optionally recording simulation statistics
    if 'record_stats' is true, and running 'nmoves_per_chunk' moves
    for each chunk */
SimPacket::SimPacket(const System &system, const Moves &moves,
                     int n_moves, int n_moves_per_chunk, bool recording_stats)
          : WorkPacketBase(), sim_system(system), sim_moves(moves),
            ncompleted(0), record_stats(recording_stats)
{
    if (n_moves > 0)
        nmoves = n_moves;
    else
        nmoves = 0;

    if (n_moves_per_chunk > 0)
        nmoves_per_chunk = n_moves_per_chunk;
    else
        nmoves_per_chunk = 1;
}

/** Copy constructor */
SimPacket::SimPacket(const SimPacket &other)
          : WorkPacketBase(other), sim_system(other.sim_system),
            sim_moves(other.sim_moves), 
            compressed_system_and_moves(other.compressed_system_and_moves),
            nmoves(other.nmoves),
            ncompleted(other.ncompleted), nmoves_per_chunk(other.nmoves_per_chunk),
            record_stats(other.record_stats)
{}

/** Destructor */
SimPacket::~SimPacket()
{}

/** Copy assignment operator */
SimPacket& SimPacket::operator=(const SimPacket &other)
{
    if (this != &other)
    {
        sim_system = other.sim_system;
        sim_moves = other.sim_moves;
        compressed_system_and_moves = other.compressed_system_and_moves;
        nmoves = other.nmoves;
        ncompleted = other.ncompleted;
        nmoves_per_chunk = other.nmoves_per_chunk;
        record_stats = other.record_stats;
        
        WorkPacketBase::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool SimPacket::operator==(const SimPacket &other) const
{
    return this == &other or
           (sim_system == other.sim_system and sim_moves == other.sim_moves and
            compressed_system_and_moves == other.compressed_system_and_moves and
            nmoves == other.nmoves and ncompleted == other.ncompleted and
            nmoves_per_chunk == other.nmoves_per_chunk and
            record_stats == other.record_stats);
}

/** Comparison operator */
bool SimPacket::operator!=(const SimPacket &other) const
{
    return not this->operator==(other);
}

/** Yes, we should compress this workpacket as it takes
    up quite a bit of space */
bool SimPacket::shouldPack() const
{
    return true;
}

/** Because it takes too long to calculate the size of this
    packet, we say that it will be 8MB - this is enough for 
    most cases */
int SimPacket::approximatePacketSize() const
{
    return 8 * 1024 * 1024;
}

/** Return whether or not the system and moves are packed away */
bool SimPacket::isPacked() const
{
    QMutexLocker lkr( const_cast<QMutex*>(&packing_mutex) );
    return not compressed_system_and_moves.isEmpty();
}

/** Pack the system and moves into a compressed binary array */
void SimPacket::pack() const
{
    QMutexLocker lkr( const_cast<QMutex*>(&packing_mutex) );
    
    if (not compressed_system_and_moves.isEmpty())
        //it is already packed
        return;
        
    QByteArray data;
    
    QDataStream ds( &data, QIODevice::WriteOnly );
    
    SharedDataStream sds(ds);
    
    sds << sim_system << sim_moves;
    
    SimPacket *nonconst_this = const_cast<SimPacket*>(this);
    
    nonconst_this->sim_system = System();
    nonconst_this->sim_moves = MovesPtr();
    
    nonconst_this->compressed_system_and_moves = qCompress(data);
}

/** Unpack the system and moves from the compressed binary array */
void SimPacket::unpack() const
{
    QMutexLocker lkr( const_cast<QMutex*>(&packing_mutex) );
    
    if (compressed_system_and_moves.isEmpty())
        //it is already unpacked
        return;

    System new_system;
    MovesPtr new_moves;
    
    //read the data in a local scope so that memory is freed as soon as possible
    {
        QByteArray data = qUncompress(compressed_system_and_moves);
    
        QDataStream ds(data);
        SharedDataStream sds(ds);
    
        sds >> new_system >> new_moves;
    }
    
    SimPacket *nonconst_this = const_cast<SimPacket*>(this);
    
    nonconst_this->compressed_system_and_moves = QByteArray();
    
    nonconst_this->sim_system = new_system;
    nonconst_this->sim_moves = new_moves;
}

/** Return the system being simulated */
System SimPacket::system() const
{
    this->unpack();
    return sim_system;
}

/** Return the moves being applied to the system */
MovesPtr SimPacket::moves() const
{
    this->unpack();
    return sim_moves;
}

/** Return the number of moves being applied to the system */
int SimPacket::nMoves() const
{
    return nmoves;
}

/** Return the number of moves already run on the system */
int SimPacket::nCompleted() const
{
    return ncompleted;
}

/** Return the number of moves to apply for each chunk */
int SimPacket::nMovesPerChunk() const
{
    return nmoves_per_chunk;
}

/** Return whether or not simulation statistics will be recorded
    during the moves */
bool SimPacket::recordingStatistics() const
{
    return record_stats;
}

/** Return whether or not this simulation has finished */
bool SimPacket::hasFinished() const
{
    return ncompleted >= nmoves;
}

/** Run a chunk of the simulation, and return the progress */
float SimPacket::chunk()
{
    if (nmoves == 0)
        return 100;

    int n_to_run = qMin( nmoves-ncompleted, nmoves_per_chunk );

    if (n_to_run > 0)
    {
        this->unpack();
    
        //run a chunk of moves
        sim_system = sim_moves.edit().move(sim_system, n_to_run, record_stats);
        
        //it all completed successfully :-)
        ncompleted += n_to_run;
    }

    return 100.0 * ( float(ncompleted) / float(nmoves) );
}
