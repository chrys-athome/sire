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

#ifndef SIREMOL_TRAJECTORY_H
#define SIREMOL_TRAJECTORY_H

#include <QMap>

#include "SireBase/property.h"

#include "SireMol/molviewproperty.h"
#include "SireMol/partialmolecule.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Trajectory;
}

QDataStream& operator<<(QDataStream&, const SireMol::Trajectory&);
QDataStream& operator>>(QDataStream&, SireMol::Trajectory&);

namespace SireMol
{

class SIREMOL_EXPORT TrajectoryBase
{

public:
    TrajectoryBase();
    
    TrajectoryBase(const TrajectoryBase &other);
    
    ~TrajectoryBase();
    
    int lowestFrame() const;
    int highestFrame() const;

    const QList<qint32>& availableFrames() const;
    
    PartialMolecule getFrame(int framenum) const;

private:
    /** The (sorted) numbers of all of the frames available in this
        object */
    QList<qint32> available_frames;
    
    /** Frames that have been paged from the disk */
    QCache<qint32, PartialMolecule> frames_in_ram;
    
    /** The link to the persistant file that contains all of the 
        frames */
    DiskCache frames_on_disk;
};

/** This class holds a (read-only) trajectory of a single molecule (or part of a
    molecule). The molecule itself and the selection of the molecule
    may change significantly from frame to frame (e.g. completely change atoms etc.)
    though it is expected that normally the animation will involve
    just small changes, e.g. of the coordinates
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT Trajectory 
         : public SireBase::ConcreteProperty<Trajectory,MoleculeProperty>,
           public TrajectoryBase
{

friend QDataStream& ::operator<<(QDataStream&, const Trajectory&);
friend QDataStream& ::operator>>(QDataStream&, Trajectory&);

public:
    Trajectory();
    
    Trajectory(const Trajectory &other);

    ~Trajectory();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Trajectory>() );
    }
    
    Trajectory& operator=(const Trajectory &other);
    
    bool operator==(const Trajectory &other) const;
    bool operator!=(const Trajectory &other) const;
};

/** This class provides the functionality necessary to edit a trajectory
    of a single molecule */
class SIREMOL_EXPORT TrajectoryEditor : public TrajectoryBase
{
public:
    TrajectoryEditor();
    
    ~TrajectoryEditor();
    
    void set(int framenum, const PartialMolecule &frame);
    
    void remove(int framenum);
    
    void removeBefore(int framenum);
    void removeFrom(int framenum);
    
    TrajectoryEditor& operator+=(const TrajectoryBase &other);
    
    void append(const TrajectoryBase &other);
};

}

Q_DECLARE_METATYPE( SireMol::Trajectory );

SIRE_END_HEADER

#endif

