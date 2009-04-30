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

#ifndef SIREMOVE_SIMSTORE_H
#define SIREMOVE_SIMSTORE_H

#include <QMutex>
#include <QByteArray>
#include <QTemporaryFile>

#include <boost/shared_ptr.hpp>

#include "SireSystem/system.h"
#include "moves.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class SimStore;
}

QDataStream& operator<<(QDataStream&, const SireMove::SimStore&);
QDataStream& operator>>(QDataStream&, SireMove::SimStore&);

namespace SireMove
{

using SireSystem::System;

/** This is a simple class that provides a place to store the primary
    information for a simulation, namely the system being simulated and
    the moves to be applied to the system. This information can be stored
    directly, for rapid access, or it can be all compressed down into 
    a binary array, so to save memory or diskspace.
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT SimStore
{

friend QDataStream& ::operator<<(QDataStream&, const SimStore&);
friend QDataStream& ::operator>>(QDataStream&, SimStore&);

public:
    SimStore();
    SimStore(const System &system, const Moves &moves, bool compress=false);
    
    SimStore(const SimStore &other);
    
    ~SimStore();
    
    SimStore& operator=(const SimStore &other);
    
    bool operator==(const SimStore &other) const;
    bool operator!=(const SimStore &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<SimStore>() );
    }

    const char* what() const
    {
        return SimStore::typeName();
    }

    SimStore* clone() const
    {
        return new SimStore(*this);
    }

    void setSystem(const System &system);
    void setMoves(const Moves &moves);
    
    void setSystemAndMoves(const System &system, const Moves &moves);

    void pack();
    void unpack();
    
    bool isPacked() const;
    
    void packToDisk();
    void packToDisk(const QString &tempdir);
    
    bool isPackedToDisk() const;

    void packToMemory();
    bool isPackedToMemory() const;
    
    System system() const;
    MovesPtr moves() const;

private:
    /** The simulation system */
    System sim_system;
  
    /** The moves to be applied to the system */
    MovesPtr sim_moves;
    
    /** A binary representation of the system and moves */
    QByteArray compressed_data;
    
    /** The temporary file used to hold the SimStore when it
        is packed to disk */
    boost::shared_ptr<QTemporaryFile> packed_file;
};

}

Q_DECLARE_METATYPE( SireMove::SimStore )

SIRE_EXPOSE_CLASS( SireMove::SimStore )

SIRE_END_HEADER

#endif
