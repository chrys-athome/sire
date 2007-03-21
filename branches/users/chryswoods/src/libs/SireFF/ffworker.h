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

#include "SireCluster/processor.h"
#include "SireCAS/values.h"

#include "parametermap.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
class Residue;
class NewAtom;
class PartialMolecule;
class MoleculeID;
}

namespace SireFF
{

class ForceField;
class FFCalculator;
class FFComponent;

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::NewAtom;
using SireMol::PartialMolecule;
using SireMol::MoleculeID;

using SireCAS::Values;

/** This is the base class of all FFWorkers - an FFWorker is a class
    that evaluates a ForceField on a (possibly) remote thread or processor.

    @author Christopher Woods
*/
class SIREFF_EXPORT FFWorkerBase
{
public:
    virtual ~FFWorkerBase();

    virtual ForceField forcefield() const=0;
    void setForceField(const ForceField &forcefield);

    void recalculateEnergy();
    void waitUntilReady();

    double energy();
    double energy(const FFComponent &component);

    Values energies();

    bool add(const Molecule &molecule,
             const ParameterMap &map = ParameterMap());
    bool add(const Residue &residue,
             const ParameterMap &map = ParameterMap());
    bool add(const NewAtom &atom,
             const ParameterMap &map = ParameterMap());
    bool add(const PartialMolecule &molecule,
             const ParameterMap &map = ParameterMap());

    bool add(const QList<Molecule> &molecules,
             const ParameterMap &map = ParameterMap());
    bool add(const QList<Residue> &residues,
             const ParameterMap &map = ParameterMap());
    bool add(const QList<NewAtom> &atoms,
             const ParameterMap &map = ParameterMap());
    bool add(const QList<PartialMolecule> &molecules,
             const ParameterMap &map = ParameterMap());

    bool addTo(const FFBase::FFGroup &group,
               const Molecule &molecule,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::FFGroup &group,
               const Residue &residue,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::FFGroup &group,
               const NewAtom &atom,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::FFGroup &group,
               const PartialMolecule &molecule,
               const ParameterMap &map = ParameterMap());

    bool addTo(const FFBase::FFGroup &group,
               const QList<Molecule> &molecules,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::FFGroup &group,
               const QList<Residue> &residues,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::FFGroup &group,
               const QList<NewAtom> &atoms,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::FFGroup &group,
               const QList<PartialMolecule> &molecules,
               const ParameterMap &map = ParameterMap());

    bool change(const Molecule &molecule);
    bool change(const Residue &residue);
    bool change(const NewAtom &atom);
    bool change(const PartialMolecule &molecule);

    bool change(const QHash<MoleculeID,Molecule> &molecules);
    bool change(const QHash<MoleculeID,PartialMolecule> &molecules);

    bool remove(const Molecule &molecule);
    bool remove(const Residue &residue);
    bool remove(const NewAtom &atom);
    bool remove(const PartialMolecule &molecule);

    bool contains(const Molecule &molecule);
    bool contains(const Residue &residue);
    bool contains(const NewAtom &atom);
    bool contains(const PartialMolecule &molecule);

    bool contains(const Molecule &molecule,
                  const FFBase::Group &group);
    bool contains(const Residue &residue,
                  const FFBase::Group &group);
    bool contains(const NewAtom &atom,
                  const FFBase::Group &group);
    bool contains(const PartialMolecule &molecule,
                  const FFBase::Group &group);

    bool refersTo(MoleculeID molid);
    bool refersTo(MoleculeID molid, const FFBase::Group &group);

    QSet<MoleculeID> moleculeIDs();
    QSet<MoleculeID> moleculeIDs(const FFBase::Group &group);

    PartialMolecule contents(MoleculeID molid);
    
    QHash<MoleculeID,PartialMolecule> contents(const FFGroup::Group group);
    QHash<MoleculeID,PartialMolecule> contents();

    bool isDirty();
    bool isClean();

    ForceFieldID ID();
    Version version();

    void assertContains(const FFComponent &component) const;

protected:
    FFWorkerBase();

    virtual bool _pvt_setForceField(const ForceField &forcefield)=0;

    virtual bool _pvt_add(const Molecule &molecule, const ParameterMap &map)=0;
    virtual bool _pvt_add(const Residue &residue, const ParameterMap &map)=0;
    virtual bool _pvt_add(const NewAtom &atom, const ParameterMap &map)=0;
    virtual bool _pvt_add(const PartialMolecule &molecule, const ParameterMap &map)=0;

    virtual bool _pvt_add(const QList<Molecule> &molecules,
                          const ParameterMap &map)=0;
    virtual bool _pvt_add(const QList<Residue> &residues,
                          const ParameterMap &map)=0;
    virtual bool _pvt_add(const QList<NewAtom> &atoms,
                          const ParameterMap &map)=0;
    virtual bool _pvt_add(const QList<PartialMolecule> &molecules,
                          const ParameterMap &map)=0;

    virtual bool _pvt_addTo(const FFBase::FFGroup &group,
                            const Molecule &molecule,
                            const ParameterMap &map)=0;
    virtual bool _pvt_addTo(const FFBase::FFGroup &group,
                            const Residue &residue,
                            const ParameterMap &map)=0;
    virtual bool _pvt_addTo(const FFBase::FFGroup &group,
                            const NewAtom &atom,
                            const ParameterMap &map)=0;
    virtual bool _pvt_addTo(const FFBase::FFGroup &group,
                            const PartialMolecule &molecule,
                            const ParameterMap &map)=0;

    virtual bool _pvt_addTo(const FFBase::FFGroup &group,
                            const QList<Molecule> &molecules,
                            const ParameterMap &map)=0;
    virtual bool _pvt_addTo(const FFBase::FFGroup &group,
                            const QList<Residue> &residues,
                            const ParameterMap &map)=0;
    virtual bool _pvt_addTo(const FFBase::FFGroup &group,
                            const QList<NewAtom> &atoms,
                            const ParameterMap &map)=0;
    virtual bool _pvt_addTo(const FFBase::FFGroup &group,
                            const QList<PartialMolecule> &molecules,
                            const ParameterMap &map)=0;

    virtual bool _pvt_change(const Molecule &molecule)=0;
    virtual bool _pvt_change(const Residue &residue)=0;
    virtual bool _pvt_change(const NewAtom &atom)=0;
    virtual bool _pvt_change(const PartialMolecule &molecule)=0;

    virtual bool _pvt_change(const QHash<MoleculeID,Molecule> &molecules)=0;
    virtual bool _pvt_change(const QHash<MoleculeID,PartialMolecule> &molecules)=0;

    virtual bool _pvt_remove(const Molecule &molecule)=0;
    virtual bool _pvt_remove(const Residue &residue)=0;
    virtual bool _pvt_remove(const NewAtom &atom)=0;
    virtual bool _pvt_remove(const PartialMolecule &molecule)=0;

    virtual bool _pvt_contains(const Molecule &molecule)=0;
    virtual bool _pvt_contains(const Residue &residue)=0;
    virtual bool _pvt_contains(const NewAtom &atom)=0;
    virtual bool _pvt_contains(const PartialMolecule &molecule)=0;

    virtual bool _pvt_contains(const Molecule &molecule,
                               const FFBase::Group &group)=0;
    virtual bool _pvt_contains(const Residue &residue,
                               const FFBase::Group &group)=0;
    virtual bool _pvt_contains(const NewAtom &atom,
                               const FFBase::Group &group)=0;
    virtual bool _pvt_contains(const PartialMolecule &molecule,
                               const FFBase::Group &group)=0;

    virtual bool _pvt_refersTo(MoleculeID molid)=0;
    virtual bool _pvt_refersTo(MoleculeID molid, const FFBase::Group &group)=0;

    virtual QSet<MoleculeID> _pvt_moleculeIDs()=0;
    virtual QSet<MoleculeID> _pvt_moleculeIDs(const FFBase::Group &group)=0;

    virtual PartialMolecule _pvt_contents(MoleculeID molid)=0;
    
    virtual QHash<MoleculeID,PartialMolecule> _pvt_contents(
                                                    const FFGroup::Group group)=0;
    virtual QHash<MoleculeID,PartialMolecule> _pvt_contents()=0;

    virtual bool _pvt_isDirty()=0;
    virtual bool _pvt_isClean()=0;

    virtual ForceFieldID _pvt_ID()=0;
    virtual Version _pvt_version()=0;

    virtual void _pvt_assertContains(const FFComponent &component) const=0;

    virtual void _pvt_recalculateEnergy()=0;
    virtual void _pvt_recalculateEnergyFG()=0;

    virtual void _pvt_waitUntilReady()=0;

    virtual double _pvt_getEnergies(Values &components)=0;

private:
    void checkEnergiesUpToDate();

    /** The total energy of the forcefield */
    double total_nrg;

    /** The values of the energy components */
    Values nrg_components;

    /** The different states of this processor */
    enum FF_STATE{ IDLE  = 0x00000000,  ///< The processor is idle
                   CALCULATING_ENERGY = 0x00000001,  ///< Energy recalculation is underway
                   CALCULATING_FORCE = 0x00000002   ///< Force recalculation is underway
                 };

    /** The current status of this processor */
    FF_STATE current_state;

    /** Whether or not the energy needs recalculating */
    bool needs_energy_recalculation;
};

/** This is a basic class that provides a local FFWorker

    @author Christopher Woods
*/
class SIREFF_EXPORT FFLocalWorker : public FFWorkerBase
{
public:
    FFLocalWorker(FFCalculator *ffcalculator);
    
    ~FFLocalWorker();
    
    ForceField forcefield() const;

protected:
    bool _pvt_setForceField(const ForceField &forcefield);

    bool _pvt_add(const Molecule &molecule, const ParameterMap &map);
    bool _pvt_add(const Residue &residue, const ParameterMap &map);
    bool _pvt_add(const NewAtom &atom, const ParameterMap &map);
    bool _pvt_add(const PartialMolecule &molecule, const ParameterMap &map);

    bool _pvt_add(const QList<Molecule> &molecules, const ParameterMap &map);
    bool _pvt_add(const QList<Residue> &residues, const ParameterMap &map);
    bool _pvt_add(const QList<NewAtom> &atoms, const ParameterMap &map);
    bool _pvt_add(const QList<PartialMolecule> &molecules, const ParameterMap &map);

    bool _pvt_addTo(const FFBase::FFGroup &group, const Molecule &molecule,
                            const ParameterMap &map);
    bool _pvt_addTo(const FFBase::FFGroup &group, const Residue &residue,
                            const ParameterMap &map);
    bool _pvt_addTo(const FFBase::FFGroup &group, const NewAtom &atom,
                            const ParameterMap &map);
    bool _pvt_addTo(const FFBase::FFGroup &group, const PartialMolecule &molecule,
                            const ParameterMap &map);

    bool _pvt_addTo(const FFBase::FFGroup &group,
                            const QList<Molecule> &molecules,
                            const ParameterMap &map);
    bool _pvt_addTo(const FFBase::FFGroup &group,
                            const QList<Residue> &residues,
                            const ParameterMap &map);
    bool _pvt_addTo(const FFBase::FFGroup &group,
                            const QList<NewAtom> &atoms,
                            const ParameterMap &map);
    bool _pvt_addTo(const FFBase::FFGroup &group,
                            const QList<PartialMolecule> &molecules,
                            const ParameterMap &map);

    bool _pvt_change(const Molecule &molecule);
    bool _pvt_change(const Residue &residue);
    bool _pvt_change(const NewAtom &atom);
    bool _pvt_change(const PartialMolecule &molecule);

    bool _pvt_change(const QHash<MoleculeID,Molecule> &molecules);
    bool _pvt_change(const QHash<MoleculeID,PartialMolecule> &molecules);

    bool _pvt_remove(const Molecule &molecule);
    bool _pvt_remove(const Residue &residue);
    bool _pvt_remove(const NewAtom &atom);
    bool _pvt_remove(const PartialMolecule &molecule);

    bool _pvt_contains(const Molecule &molecule);
    bool _pvt_contains(const Residue &residue);
    bool _pvt_contains(const NewAtom &atom);
    bool _pvt_contains(const PartialMolecule &molecule);

    bool _pvt_contains(const Molecule &molecule,
                       const FFBase::Group &group);
    bool _pvt_contains(const Residue &residue,
                       const FFBase::Group &group);
    bool _pvt_contains(const NewAtom &atom,
                       const FFBase::Group &group);
    bool _pvt_contains(const PartialMolecule &molecule,
                       const FFBase::Group &group);

    bool _pvt_refersTo(MoleculeID molid);
    bool _pvt_refersTo(MoleculeID molid, const FFBase::Group &group);

    QSet<MoleculeID> _pvt_moleculeIDs();
    QSet<MoleculeID> _pvt_moleculeIDs(const FFBase::Group &group);

    PartialMolecule _pvt_contents(MoleculeID molid);
    
    QHash<MoleculeID,PartialMolecule> _pvt_contents(const FFGroup::Group group);
    QHash<MoleculeID,PartialMolecule> _pvt_contents();

    bool _pvt_isDirty();
    bool _pvt_isClean();

    ForceFieldID _pvt_ID();
    Version _pvt_version();

    void _pvt_assertContains(const FFComponent &component) const;
};

/** This is a test class which calculates the forcefield energy in the main thread.

    @author Christopher Woods
*/
class SIREFF_EXPORT FFWorker : public FFLocalWorker, public SireCluster::WorkerBase
{
public:
    FFWorker(FFCalculator *ffcalculator);

    ~FFWorker();

    ForceField forcefield() const;

protected:
    void _pvt_recalculateEnergy();
    void _pvt_recalculateEnergyFG();

    void _pvt_waitUntilReady();

    double _pvt_getEnergies(Values &components);

private:
    /** The calculator used to evaluate the forcefield */
    std::auto_ptr<FFCalculator> ffcalculator;
};

}

SIRE_END_HEADER

#endif
