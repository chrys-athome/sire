/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIRESIM_CLUSTERSIMSYSTEM_H
#define SIRESIM_CLUSTERSIMSYSTEM_H

#include "SireSystem/simsystem.h"

SIRE_BEGIN_HEADER

namespace SireSim
{

using SireSystem::SimSystem;

class SIRESIM_EXPORT ClusterSimSystem : public SimSystem
{
public:
    ClusterSimSystem(const System &system,
                     const Cluster &cluster,
                     const ClusterMap &map = ClusterMap());

    ~ClusterSimSystem();

    double energy(const Function &component);

    System checkpoint();

    void rollback(const System &system);

    MoleculeGroup group(const MoleculeGroup &group);

    QHash<MolGroupID,MoleculeGroup> groups();

private:
    /** The forcefields calculated in the main thread of the
        cluster, indexed by forcefield ID */
    QHash<ForceFieldID, ForceField> localff;

    /** The processors in the cluster running forcefields,
        indexed by ForceFieldID */
    QHash<ForceFieldID, Processor> remoteff;

    /** The metadata about the system (groups, monitors,
        properties, version etc.) */
    SystemData sysdata;

    /** The cluster that is being used to run this simulation */
    Cluster clstr;

};

}

SIRE_END_HEADER

#endif
