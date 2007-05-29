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

#include "SireMol/molecules.h"

#include "SireFF/parametermap.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class FFGroupID;
}

namespace SireVol
{
class Space;
}

namespace SireSystem
{

using SireFF::FFGroupID;
using SireFF::ParameterMap;

using SireMol::Molecules;

using SireVol::Space;

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

    virtual void rollBack(const CheckPoint &checkpoint)=0;

    void commit();

    void setProperty(const QString &name, const Property &property);
    void setProperty(ForceFieldID ffid, const QString &name, const Property &property);

    void setProperty(const QSet<ForceFieldID> &ffids,
                     const QString &name, const Property &property);

    void setSpace(const Space &new_space,
                  const Molecules &changed_mols = Molecules());

    PartialMolecule change(const PartialMolecule &molecule);

    Molecules change(const Molecules &molecules);

    PartialMolecule add(const PartialMolecule &molecule,
                        const QSet<MoleculeGroupID> &molgroupids);

    PartialMolecule add(const PartialMolecule &molecule,
                        const QSet<FFGroupID> &ffgroupids,
                        const ParameterMap &map = ParameterMap());

    PartialMolecule add(const PartialMolecule &molecule,
                        const QSet<FFGroupID> &ffgroupids,
                        const QSet<MoleculeGroupID> &molgroupids,
                        const ParameterMap &map = ParameterMap());

    Molecules add(const Molecules &molecules,
                  const QSet<MoleculeGroupID> &molgroupids);

    Molecules add(const Molecules &molecules,
                  const QSet<FFGroupID> &ffgroupids,
                  const ParameterMap &map = ParameterMap());

    Molecules add(const Molecules &molecules,
                  const QSet<FFGroupID> &ffgroupids,
                  const QSet<MoleculeGroupID> &molgroupids,
                  const ParameterMap &map = ParameterMap());

    void remove(const PartialMolecule &molecule);
    void remove(const Molecules &molecules);

    void remove(const PartialMolecule &molecule,
                const QSet<MoleculeGroupID> &molgroupids);

    void remove(const PartialMolecule &molecule,
                const QSet<FFGroupID> &ffgroupids,
                const QSet<MoleculeGroupID> &molgroupids = QSet<MoleculeGroupID>());

    void remove(const Molecules &molecules,
                const QSet<MoleculeGroupID> &molgroupids);

    void remove(const Molecules &molecules,
                const QSet<FFGroupID> &ffgroupids,
                const QSet<MoleculeGroupID> &molgroupids = QSet<MoleculeGroupID>());

protected:
    SimSystem();

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
