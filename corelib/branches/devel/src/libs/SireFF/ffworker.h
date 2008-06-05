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

#ifndef SIREFF_FFWORKER_H
#define SIREFF_FFWORKER_H

#include "ffworkerbase.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

/** This is a basic class that provides a local FFWorker

    @author Christopher Woods
*/
class SIREFF_EXPORT FFLocalWorker : public FFWorkerBase
{
public:
    FFLocalWorker(FFCalculatorBase *ffcalculator);

    ~FFLocalWorker();

protected:
    ForceField _pvt_forceField();
    bool _pvt_setForceField(const ForceField &forcefield);

    double _pvt_getEnergies(Values &components);
    
    bool _pvt_setProperty(const QString &name, const Property &property);
    Property _pvt_getProperty(const QString &name);
    bool _pvt_containsProperty(const QString &name);
    
    QHash<QString,Property> _pvt_properties();
    
    bool _pvt_add(const PartialMolecule &molecule, const ParameterMap &map);
    bool _pvt_add(const QList<PartialMolecule> &molecules, const ParameterMap &map);

    bool _pvt_addTo(const FFBase::Group &group, const PartialMolecule &molecule,
                    const ParameterMap &map);
    bool _pvt_addTo(const FFBase::Group &group,
                    const QList<PartialMolecule> &molecules,
                    const ParameterMap &map);

    void _pvt_mustNowRecalculateFromScratch();

    bool _pvt_change(const PartialMolecule &molecule);
    bool _pvt_change(const QHash<MoleculeID,PartialMolecule> &molecules);
    bool _pvt_change(const QList<PartialMolecule> &molecules);

    bool _pvt_remove(const PartialMolecule &molecule);
    bool _pvt_remove(const QList<PartialMolecule> &molecules);

    bool _pvt_removeFrom(const FFBase::Group &group,
                         const PartialMolecule &molecule);
    bool _pvt_removeFrom(const FFBase::Group &group,
                         const QList<PartialMolecule> &molecules);

    bool _pvt_contains(const PartialMolecule &molecule);
    bool _pvt_contains(const PartialMolecule &molecule,
                       const FFBase::Group &group);

    bool _pvt_refersTo(MoleculeID molid);
    bool _pvt_refersTo(MoleculeID molid, const FFBase::Group &group);

    QSet<FFBase::Group> _pvt_groupsReferringTo(MoleculeID molid);

    QSet<MoleculeID> _pvt_moleculeIDs();
    QSet<MoleculeID> _pvt_moleculeIDs(const FFBase::Group &group);

    PartialMolecule _pvt_molecule(MoleculeID molid);
    PartialMolecule _pvt_molecule(MoleculeID molid, const FFBase::Group &group);

    QHash<MoleculeID,PartialMolecule> _pvt_molecules();
    QHash<MoleculeID,PartialMolecule> _pvt_molecules(const FFBase::Group &group);
    QHash<MoleculeID,PartialMolecule> _pvt_molecules(const QSet<MoleculeID> &molids);

    QHash<MoleculeID,PartialMolecule> _pvt_contents(const FFBase::Group &group);
    QHash<MoleculeID,PartialMolecule> _pvt_contents();

    bool _pvt_isDirty();
    bool _pvt_isClean();

    ForceFieldID _pvt_ID();
    Version _pvt_version();

    void _pvt_assertContains(const FFComponent &component);

    /** The calculator used to evaluate the forcefield */
    std::auto_ptr<FFCalculatorBase> ffcalculator;
};

/** This is a test class which calculates the forcefield energy in the main thread.

    @author Christopher Woods
*/
class SIREFF_EXPORT FFWorker : public FFLocalWorker, public SireCluster::WorkerBase
{
public:
    FFWorker(FFCalculatorBase *ffcalculator);

    ~FFWorker();

protected:
    void _pvt_recalculateEnergy();
    void _pvt_recalculateEnergyFG();

    void _pvt_waitUntilReady();
};

}

SIRE_END_HEADER

#endif
