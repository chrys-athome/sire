/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#ifndef SIREFF_PATCHES_H
#define SIREFF_PATCHES_H

#include "patch.h"

#include "SireVol/patching.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class Patches;
}

QDataStream& operator<<(QDataStream&, const SireFF::Patches&);
QDataStream& operator>>(QDataStream&, SireFF::Patches&);

namespace SireFF
{

using SireVol::Patching;
using SireVol::Space;
using SireVol::CoordGroup;
using SireVol::AABox;

/** This class holds a collection of Patch objects. All of the beads
    in a forcefield are arranged into Patches, with each patch containing
    a neighbouring group of beads.
    
    @author Christopher Woods
*/
class SIREFF_EXPORT Patches
        : public SireBase::ConcreteProperty<Patches,SireBase::Property>
{

friend QDataStream& operator<<(QDataStream&, const Patches&);
friend QDataStream& operator>>(QDataStream&, Patches&);

public:
    Patches();
    Patches(const Patching &patching);

    Patches(const Patches &other);

    ~Patches();
    
    Patches& operator=(const Patches &other);
    
    bool operator==(const Patches &other) const;
    bool operator!=(const Patches &other) const;
    
    const Patching& patching() const;
    const Space& space() const;
    
    void repatch(const Patching &patching);
    void repatch(const Space &space);
   
    int nPatches() const;
    int size() const;
    int count() const;
    
    const Patch& operator[](int i) const;
    const Patch& at(int i) const;
    
    const Patch* data() const;
    const Patch* constData() const;

    const Patching& patching() const;

    quint32 add(const CoordGroup &coords, const FFParameters &params);
    
    QVector<quint32> add(const CoordGroupArray &coords, 
                         const FFParametersArray &params);
    
    QVector<quint32> add(const QVector<CoordGroup> &coords,
                         const QVector<FFParametersPtr> &params);
                         
    QVector< QVector<quint32> > add(const QVector<CoordGroupArray> &coords,
                                    const QVector<FFParametersArrayPtr> &params);
    
    void update(quint32 beadid, const CoordGroup &coords);
    void update(quint32 beadid, const FFParameters &params);
    void update(quint32 beadid, const CoordGroup &coords,
                const FFParameters &params);
    
    void update(const QVector<quint32> &beadids, const CoordGroupArray &coords);
    void update(const QVector<quint32> &beadids, const FFParametersArray &params);
    void update(const QVector<quint32> &beadids, const CoordGroupArray &coords,
                const FFParametersArray &params);
    
    void remove(quint32 beadid);
    void remove(const QVector<quint32> &beadids);

    void removeAll();
    
private:
    /** The current patching scheme (contains the space as well) */
    PatchingPtr patching;

    /** All of the patches */
    QVector<Patch> patches;
    
    /** Index of the patch that contains each bead */
    QHash<quint32,quint32> beadid_to_patch;
    
    /** The list of beads to skip (normally because they have changed) */
    QSet<quint32> beads_to_skip;
    
    /** The last assigned bead ID */
    quint32 last_beadid;
};

}

Q_DECLARE_METATYPE( SireFF::Patches )

SIRE_END_HEADER

#endif
