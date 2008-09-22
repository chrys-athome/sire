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

#ifndef SIREMOVE_MPISIMWORKER_H
#define SIREMOVE_MPISIMWORKER_H

#include <QMutex>

#include "moves.h"

#include "SireMPI/mpiworker.h"

#include "SireSystem/system.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class MPISimWorker;
}

QDataStream& operator<<(QDataStream&, const SireMove::MPISimWorker&);
QDataStream& operator>>(QDataStream&, SireMove::MPISimWorker&);

namespace SireMove
{

using SireSystem::System;

/** This class performs a simulation as a collection of work chunks.
    This class is thread-safe.

    @author Christopher Woods
*/
class SIREMPI_EXPORT MPISimWorker : public SireMPI::MPIWorker
{

friend QDataStream& ::operator<<(QDataStream&, const MPISimWorker&);
friend QDataStream& ::operator>>(QDataStream&, MPISimWorker&);

public:
    MPISimWorker();
    MPISimWorker(const System &system, const MovesBase &moves,
                 int nmoves, bool record_stats,
                 int chunk_size=100);
                 
    MPISimWorker(const MPISimWorker &other);
    
    ~MPISimWorker();
    
    MPISimWorker& operator=(const MPISimWorker &other);
    
    bool operator==(const MPISimWorker &other) const;
    bool operator!=(const MPISimWorker &other) const;

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MPISimWorker>() );
    }
    
    const char* what() const
    {
        return MPISimWorker::typeName();
    }

    MPISimWorker* clone() const
    {
        return new MPISimWorker(*this);
    }

    System system() const;
    Moves moves() const;
    
    int nMoves() const;
    int nCompleted() const;
    
    int chunkSize() const;
    
    bool recordStatistics() const;
    
    void runChunk();
    
    bool hasFinished() const;
    
private:
    /** Mutex to protect access to this data */
    QMutex data_mutex;

    /** The system being simulated */
    System sim_system;
    
    /** The moves being performed on the system */
    Moves sim_moves;
    
    /** The number of moves to perform */
    int nmoves;
    
    /** The number of moves already performed */
    int ncompleted_moves;
    
    /** The number of moves to run per chunk */
    int chunk_size;
    
    /** Whether or not to record stats */
    bool record_stats;
};

}

Q_DECLARE_METATYPE( SireMove::MPISimWorker )

SIRE_EXPOSE_CLASS( SireMove::MPISimWorker )

SIRE_END_HEADER

#endif
