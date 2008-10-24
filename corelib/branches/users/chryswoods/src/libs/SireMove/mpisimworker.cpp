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

#include "mpisimworker.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireMove;
using namespace SireMPI;
using namespace SireSystem;
using namespace SireStream;

static const RegisterMetaType<MPISimWorker> r_simworker;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const MPISimWorker &worker)
{
    writeHeader(ds, r_simworker, 1);
    
    SharedDataStream sds(ds);
    
    QMutexLocker lkr( const_cast<QMutex*>(&(worker.data_mutex)) );
    
    sds << worker.sim_system;
    sds << worker.sim_moves;
    sds << worker.nmoves;
    sds << worker.ncompleted_moves;
    sds << worker.chunk_size;
    sds << worker.record_stats;
    sds << static_cast<const MPIWorker&>(worker);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, MPISimWorker &worker)
{
    VersionID v = readHeader(ds, r_simworker);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        MPISimWorker new_worker;
        
        sds >> new_worker.sim_system >> new_worker.sim_moves
            >> new_worker.nmoves >> new_worker.ncompleted_moves
            >> new_worker.chunk_size
            >> new_worker.record_stats
            >> static_cast<MPIWorker&>(new_worker);
            
        worker = new_worker;
    }
    else
        throw version_error(v, "1", r_simworker, CODELOC);
        
    return ds;
}

/** Null constructor */
MPISimWorker::MPISimWorker() 
             : MPIWorker(), nmoves(0), ncompleted_moves(0), 
               chunk_size(0), record_stats(false)
{}

/** Construct a worker to perform 'nmoves' moves of the type contained
    in 'moves' on the system 'system', recording statistics if
    'record_stats' is true */
MPISimWorker::MPISimWorker(const System &system, const Moves &moves,
                           int num_moves, bool record_statistics,
                           int chunksize)
             : MPIWorker(),
               sim_system(system), sim_moves(moves), 
               nmoves(num_moves), ncompleted_moves(0), chunk_size(chunksize),
               record_stats(record_statistics)
{
    if (nmoves < 0)
        nmoves = 0;
}

/** Copy assignment operator */
MPISimWorker& MPISimWorker::operator=(const MPISimWorker &other)
{
    if (this != &other)
    {
        QMutexLocker my_lkr( &data_mutex );
        QMutexLocker other_lkr( const_cast<QMutex*>(&(other.data_mutex)) );
        
        sim_system = other.sim_system;
        sim_moves = other.sim_moves;
        nmoves = other.nmoves;
        ncompleted_moves = other.ncompleted_moves;
        chunk_size = other.chunk_size;
        record_stats = other.record_stats;
        
        MPIWorker::operator=(other);
    }
    
    return *this;
}
             
/** Copy constructor */
MPISimWorker::MPISimWorker(const MPISimWorker &other) : MPIWorker()
{
    this->operator=(other);
}

/** Destructor */
MPISimWorker::~MPISimWorker()
{
    //wait until it has finished
    data_mutex.lock();
    data_mutex.unlock();
}

/** Comparison operator */
bool MPISimWorker::operator==(const MPISimWorker &other) const
{
    if (this == &other)
    {
        return true;
    }
    else
    {
        QMutexLocker my_lkr( const_cast<QMutex*>(&data_mutex) );
        QMutexLocker other_lkr( const_cast<QMutex*>(&(other.data_mutex)) );
        
        return nmoves == other.nmoves and 
               ncompleted_moves == other.ncompleted_moves and
               chunk_size == other.chunk_size and
               record_stats == other.record_stats and
               sim_system == other.sim_system and sim_moves == other.sim_moves;
    }
}

/** Comparison operator */
bool MPISimWorker::operator!=(const MPISimWorker &other) const
{
    return not this->operator==(other);
}

/** Return the system being simulated */
System MPISimWorker::system() const
{
    QMutexLocker lkr( const_cast<QMutex*>(&data_mutex) );
    return sim_system;
}

/** Return the moves being performed on the system */
MovesPtr MPISimWorker::moves() const
{
    QMutexLocker lkr( const_cast<QMutex*>(&data_mutex) );
    return sim_moves;
}

/** Return the number of moves being performed */
int MPISimWorker::nMoves() const
{
    QMutexLocker lkr( const_cast<QMutex*>(&data_mutex) );
    return nmoves;
}

/** Return the number of completed moves */
int MPISimWorker::nCompleted() const
{
    QMutexLocker lkr( const_cast<QMutex*>(&data_mutex) );
    return ncompleted_moves;
}

/** Return the number of moves to perform per chunk */
int MPISimWorker::chunkSize() const
{
    QMutexLocker lkr( const_cast<QMutex*>(&data_mutex) );
    return chunk_size;
}

/** Record whether or not we are recording statistics */
bool MPISimWorker::recordStatistics() const
{
    QMutexLocker lkr( const_cast<QMutex*>(&data_mutex) );
    return record_stats;
}

/** Perform a chunk of the simulation */
void MPISimWorker::runChunk()
{
    QMutexLocker lkr(&data_mutex);
    
    if (ncompleted_moves >= nmoves)
    {
        //the simulation has finished
        return;
    }

    //run a maximum of chunk_size moves per chunk
    int nmoves_to_run = qMin( chunk_size, nmoves - ncompleted_moves );
    
    //run those moves
    try
    {
        sim_system = sim_moves.edit().move(sim_system, nmoves_to_run, record_stats);
    }
    catch(const SireError::exception &e)
    {
        qDebug() << e.toString();
        throw;
    }
    
    //increment the number of completed moves
    ncompleted_moves += nmoves_to_run;
    
    //set the progress
    MPIWorker::setProgress( (100.0 * ncompleted_moves) / nmoves );
}

/** Return whether or not the simulation has finished */
bool MPISimWorker::hasFinished() const
{
    QMutexLocker lkr( const_cast<QMutex*>(&data_mutex) );
    return ncompleted_moves >= nmoves;
}
