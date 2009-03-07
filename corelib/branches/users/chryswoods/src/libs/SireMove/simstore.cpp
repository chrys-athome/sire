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

#include "SireError/errors.h"

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
    writeHeader(ds, r_simstore, 3);
    
    SharedDataStream sds(ds);
    
    sds << simstore.sim_system << simstore.sim_moves << simstore.compressed_data;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, SimStore &simstore)
{
    VersionID v = readHeader(ds, r_simstore);

    if (v == 3)
    {
        SharedDataStream sds(ds);
        
        sds >> simstore.sim_system >> simstore.sim_moves >> simstore.compressed_data;
    }
    else if (v == 2)
    {
        SharedDataStream sds(ds);
        
        SimStore new_store;
        sds >> new_store.compressed_data;
        
        simstore = new_store;
    }
    else if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> simstore.sim_system >> simstore.sim_moves;
    }
    else
        throw version_error( v, "1,2,3", r_simstore, CODELOC );

    return ds;
}

/** Null constructor */
SimStore::SimStore()
{}

/** Construct from the passed system and moves - optionally specify whether
    or not to compress the data now */
SimStore::SimStore(const System &system, const Moves &moves, bool compress)
         : sim_system(system), sim_moves(moves)
{
    if (compress)
        this->pack();
}

/** Copy constructor */
SimStore::SimStore(const SimStore &other)
{
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
    
    sim_system = other.sim_system;
    sim_moves = other.sim_moves;
    compressed_data = other.compressed_data;
    
    return *this;
}

/** Comparison operator */
bool SimStore::operator==(const SimStore &other) const
{
    if (this == &other)
        return true;
        
    return sim_system == other.sim_system and 
           sim_moves == other.sim_moves and
           compressed_data == other.compressed_data;
}

/** Comparison operator */
bool SimStore::operator!=(const SimStore &other) const
{
    return not this->operator==(other);
}

/** Pack the system and moves into a compressed binary array */
void SimStore::pack()
{
    if (not compressed_data.isEmpty())
        //the data is already compressed
        return;

    QByteArray data;
    
    QDataStream ds( &data, QIODevice::WriteOnly );
    
    SharedDataStream sds(ds);
    
    sds << sim_system << sim_moves;
    
    sim_system = System();
    sim_moves = MovesPtr();
    compressed_data = qCompress(data);
}

/** Unpack the system and move from the compressed binary array */
void SimStore::unpack()
{
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

    compressed_data = QByteArray();
    sim_system = new_system;
    sim_moves = new_moves;
}

/** Return whether or not the data is packed into a compressed
    binary array */
bool SimStore::isPacked() const
{
    return not compressed_data.isEmpty();
}

/** Set the system to be stored */
void SimStore::setSystem(const System &system)
{
    if (this->isPacked())
        throw SireError::invalid_state( QObject::tr(
            "Cannot set the system in a SimStore that is packed. You must "
            "unpack it first."), CODELOC );
    
    sim_system = system;
}

/** Set the moves to be stored */
void SimStore::setMoves(const Moves &moves)
{
    if (this->isPacked())
        throw SireError::invalid_state( QObject::tr(
            "Cannot set the moves in a SimStore that is packed. You must "
            "unpack it first."), CODELOC );
        
    sim_moves = moves;
}

/** Set both the system and moves to be stored */
void SimStore::setSystemAndMoves(const System &system, const Moves &moves)
{
    if (this->isPacked())
        throw SireError::invalid_state( QObject::tr(
            "Cannot set the system and moves in a SimStore that is packed. You must "
            "unpack it first."), CODELOC );

    sim_system = system;
    sim_moves = moves;
}

/** Return a copy of the system being stored */
System SimStore::system() const
{
    if (this->isPacked())
        throw SireError::invalid_state( QObject::tr(
            "You cannot get the system from a SimStore that is packed. You must "
            "unpack it first."), CODELOC );

    return sim_system;
}

/** Return a copy of the moves being stored */
MovesPtr SimStore::moves() const
{
    if (this->isPacked())
        throw SireError::invalid_state( QObject::tr(
            "You cannot get the moves from a SimStore that is packed. You must "
            "unpack it first."), CODELOC );

    return sim_moves;
}
