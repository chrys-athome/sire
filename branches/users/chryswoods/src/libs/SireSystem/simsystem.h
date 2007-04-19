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

#ifndef SIRESYSTEM_SIMSYSTEM_H
#define SIRESYSTEM_SIMSYSTEM_H

#include "querysystem.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{

/** This class holds a system that is being actively
    simulated.

    While System is an implicitly-shared class,
    SimSystem is designed to be a non-copyable
    class (as a Simulation is occuring only once).

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SimSystem : public QuerySystem
{

public:
    ~SimSystem();

    virtual void rollback(const CheckPoint &checkpoint)=0;

    void commit();

    void setProperty(const QString &name, const Property &property);
    void setProperty(ForceFieldID ffid, const QString &name, const Property &property);

    void setProperty(const QSet<ForceFieldID> &ffids,
                     const QString &name, const Property &property);

    void change(const PartialMolecule &molecule);
    void change(const QHash<MoleculeID,PartialMolecule> &molecules);
    void change(const QList<PartialMolecule> &molecules);

    void add(const PartialMolecule &molecule,
             const QSet<MoleculeGroupID> &molgroupids);

    void add(const PartialMolecule &molecule,
             const QSet<FFGroupID> &ffgroupids,
             const QSet<MoleculeGroupID> &molgroupids = QSet<MoleculeGroupID>());

    void add(const QHash<MoleculeID,PartialMolecule> &molecules,
             const QSet<MoleculeGroupID> &molgroupids);

    void add(const QHash<MoleculeID,PartialMolecule> &molecules,
             const QSet<FFGroupID> &ffgroupids,
             const QSet<MoleculeGroupID> &molgroupids = QSet<MoleculeGroupID>());

    void add(const QHash<MoleculeID,PartialMolecule> &molecules,
             const QSet<MoleculeGroupID> &molgroupids);

    void add(const QHash<MoleculeID,PartialMolecule> &molecules,
             const QSet<FFGroupID> &ffgroupids,
             const QSet<MoleculeGroupID> &molgroupids = QSet<MoleculeGroupID>());

    void remove(const PartialMolecule &molecule);
    void remove(const QList<PartialMolecule> &molecules);

    void remove(const PartialMolecule &molecule,
                const QSet<MoleculeGroupID> &molgroupids);

    void remove(const PartialMolecule &molecule,
                const QSet<FFGroupID> &ffgroupids,
                const QSet<MoleculeGroupID> &molgroupids = QSet<MoleculeGroupID>());

    void remove(const QList<PartialMolecule> &molecules,
                const QSet<MoleculeGroupID> &molgroupids);

    void remove(const QList<PartialMolecule> &molecules,
                const QSet<FFGroupID> &ffgroupids,
                const QSet<MoleculeGroupID> &molgroupids = QSet<MoleculeGroupID>());

protected:
    SimSystem(SystemData &sysdata,
              ForceFieldsBase &ffields,
              SystemMonitors &monitors);

    void setSystem(SystemData &sysdata,
                   ForceFieldsBase &ffields,
                   SystemMonitors &monitors);
};

}

SIRE_END_HEADER

#endif
