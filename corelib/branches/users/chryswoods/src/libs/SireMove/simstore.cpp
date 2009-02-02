/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "simstore.h"

#include "SireError/getbacktrace.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireMove;
using namespace SireSystem;
using namespace SireStream;

static const RegisterMetaType<SimStore> r_simstore;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const SimStore &simstore)
{
    QMutexLocker lkr( const_cast<QMutex*>(&(simstore.datamutex)) );

    if (not simstore.isPacked())
    {
        //only stream the packed version of the SimStore
        SimStore new_store(simstore);
        lkr.unlock();
        
        new_store.pack();
        ds << new_store;
    }
    else
    {
        writeHeader(ds, r_simstore, 1);
    
        SharedDataStream sds(ds);
    
        sds << simstore.compressed_data;
    }
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, SimStore &simstore)
{
    VersionID v = readHeader(ds, r_simstore);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        QByteArray data;
        
        sds >> data;
        
        QMutexLocker lkr( &(simstore.datamutex) );
        
        simstore.compressed_data = data;
        simstore.sim_system = System();
        simstore.sim_moves = MovesPtr();
    }
    else
        throw version_error( v, "1", r_simstore, CODELOC );

    return ds;
}

/** Null constructor */
SimStore::SimStore() : datamutex( QMutex::Recursive )
{}

/** Construct from the passed system and moves - optionally specify whether
    or not to compress the data now */
SimStore::SimStore(const System &system, const Moves &moves, bool compress)
         : sim_system(system), sim_moves(moves), datamutex( QMutex::Recursive )
{
    if (compress)
        this->pack();
}

/** Copy constructor */
SimStore::SimStore(const SimStore &other) : datamutex( QMutex::Recursive )
{
    QMutexLocker lkr( const_cast<QMutex*>(&(other.datamutex)) );
    
    sim_system = other.sim_system;
    sim_moves = other.sim_moves;
    compressed_data = other.compressed_data;
}

/** Destructor */
SimStore::~SimStore()
{}

/** Copy assignment operator */
SimStore& SimStore::operator=(const SimStore &other)
{
    if (this == &other)
        return *this;
        
    QMutexLocker lkr1( const_cast<QMutex*>(&(other.datamutex)) );
    
    System other_system = other.sim_system;
    MovesPtr other_moves = other.sim_moves;
    QByteArray other_data = other.compressed_data;
    
    lkr1.unlock();
    
    QMutexLocker lkr2( &datamutex );
    
    sim_system = other_system;
    sim_moves = other_moves;
    compressed_data = other_data;
    
    return *this;
}

/** Comparison operator */
bool SimStore::operator==(const SimStore &other) const
{
    if (this == &other)
        return true;
        
    QMutexLocker lkr1( const_cast<QMutex*>(&(other.datamutex)) );

    System other_system = other.sim_system;
    MovesPtr other_moves = other.sim_moves;
    QByteArray other_data = other.compressed_data;
    
    lkr1.unlock();

    QMutexLocker lkr2( const_cast<QMutex*>(&datamutex) );
    
    return sim_system == other_system and 
           sim_moves == other_moves and
           compressed_data == other_data;
}

/** Comparison operator */
bool SimStore::operator!=(const SimStore &other) const
{
    return not this->operator==(other);
}

/** Pack the system and moves into a compressed binary array */
void SimStore::pack() const
{
    SimStore *nonconst_this = const_cast<SimStore*>(this);
    
    QMutexLocker lkr( &(nonconst_this->datamutex) );
    
    if (not compressed_data.isEmpty())
        //the data is already compressed
        return;

    QByteArray data;
    
    QDataStream ds( &data, QIODevice::WriteOnly );
    
    SharedDataStream sds(ds);
    
    sds << sim_system << sim_moves;
    
    nonconst_this->sim_system = System();
    nonconst_this->sim_moves = MovesPtr();
    nonconst_this->compressed_data = qCompress(data);
}

/** Unpack the system and move from the compressed binary array */
void SimStore::unpack() const
{
    SimStore *nonconst_this = const_cast<SimStore*>(this);
    
    QMutexLocker lkr( &(nonconst_this->datamutex) );
    
    if (compressed_data.isEmpty())
        //the data is already uncompressed
        return;

    System new_system;
    MovesPtr new_moves;

    //use a local scope so that the uncompressed data is deleted
    //as soon as possible
    {
        QByteArray data = qUncompress( compressed_data );
        QDataStream ds(data);
        SharedDataStream sds(ds);
        
        sds >> new_system >> new_moves;
    }

    nonconst_this->compressed_data = QByteArray();
    nonconst_this->sim_system = new_system;
    nonconst_this->sim_moves = new_moves;
}

/** Return whether or not the data is packed into a compressed
    binary array */
bool SimStore::isPacked() const
{
    QMutexLocker lkr( const_cast<QMutex*>(&datamutex) );
    return not compressed_data.isEmpty();
}

/** Set the system to be stored */
void SimStore::setSystem(const System &system)
{
    QMutexLocker lkr(&datamutex);
    
    bool is_packed = (not compressed_data.isEmpty());
    
    if (is_packed)
        this->unpack();
        
    sim_system = system;
    
    if (is_packed)
        this->pack();
}

/** Set the moves to be stored */
void SimStore::setMoves(const Moves &moves)
{
    QMutexLocker lkr(&datamutex);
    
    bool is_packed = (not compressed_data.isEmpty());
    
    if (is_packed)
        this->unpack();
        
    sim_moves = moves;
    
    if (is_packed)
        this->pack();
}

/** Set both the system and moves to be stored */
void SimStore::setSystemAndMoves(const System &system, const Moves &moves)
{
    QMutexLocker lkr(&datamutex);
    
    bool is_packed = (not compressed_data.isEmpty());

    sim_system = system;
    sim_moves = moves;
    compressed_data = QByteArray();
    
    if (is_packed)
        this->pack();
}

/** Return a copy of the system being stored */
System SimStore::system() const
{
    QMutexLocker lkr( const_cast<QMutex*>(&datamutex) );
    
    if (not compressed_data.isEmpty())
    {
        //the data is compressed
        SimStore other(*this);
        
        other.unpack();
        return other.system();
    }
    else
        return sim_system;
}

/** Return a copy of the moves being stored */
MovesPtr SimStore::moves() const
{
    QMutexLocker lkr( const_cast<QMutex*>(&datamutex) );
    
    if (not compressed_data.isEmpty())
    {
        //the data is compressed
        SimStore other(*this);
        
        other.unpack();
        return other.moves();
    }
    else
        return sim_moves;
}
