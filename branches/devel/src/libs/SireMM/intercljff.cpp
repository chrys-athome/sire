/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "SireCAS/qhash_sirecas.h"
#include "SireMol/qhash_siremol.h"

#include <QDebug>

#include "intercljff.h"

#include "chargetable.h"
#include "ljtable.h"

#include "atomiccharges.h"
#include "atomicljs.h"

#include "detail/molcljinfo.h"

#include "SireVol/coordgroup.h"

#include "SireMol/errors.h"

using namespace SireMM;
using namespace SireMM::detail;

using namespace SireDB;

///////////
/////////// Implementation of InterCLJFF::Components
///////////

/** Constructor */
InterCLJFF::Components::Components() : CLJFF::Components()
{}

/** Constructor that just passes its arguments up to the parent's constructor */
InterCLJFF::Components::Components(const FFBase &ffbase, const Symbols &symbols)
           : CLJFF::Components(ffbase,symbols)
{}

/** Copy constructor */
InterCLJFF::Components::Components(const Components &other)
           : CLJFF::Components(other)
{}

/** Destructor */
InterCLJFF::Components::~Components()
{}

///////////
/////////// Implementation of InterCLJFF::Parameters
///////////

/** Constructor */
InterCLJFF::Parameters::Parameters() : CLJFF::Parameters()
{}

/** Copy constructor */
InterCLJFF::Parameters::Parameters(const Parameters &other)
           : CLJFF::Parameters(other)
{}

/** Destructor */
InterCLJFF::Parameters::~Parameters()
{}

///////////
/////////// Implementation of InterCLJFF::Groups
///////////

/** Constructor */
InterCLJFF::Groups::Groups() : CLJFF::Groups()
{}

/** Copy constructor */
InterCLJFF::Groups::Groups(const Groups &other)
           : CLJFF::Groups(other)
{}

/** Destructor */
InterCLJFF::Groups::~Groups()
{}

///////////
/////////// Implementation of InterCLJFF
///////////

/** Constructor */
InterCLJFF::InterCLJFF() : CLJFF()
{}

/** Construct a CLJ forcefield using the passed Space, combining rules and
    switching function */
InterCLJFF::InterCLJFF(const Space &space, const SwitchingFunction &switchfunc)
           : CLJFF(space, switchfunc)
{}

/** Copy constructor */
InterCLJFF::InterCLJFF(const InterCLJFF &other)
           : CLJFF(other), mols(other.mols)
{}

/** Destructor */
InterCLJFF::~InterCLJFF()
{}

/** Recalculate the total energy of this forcefield from scratch */
void InterCLJFF::recalculateTotalEnergy()
{
    //calculate the total CLJ energy of all molecule pairs...
    double cnrg = 0.0;
    double ljnrg = 0.0;

    int nmols = mols.count();

    const MolCLJInfo *molarray = mols.constData();

    //loop over all molecule pairs
    for (int i=0; i<nmols-1; ++i)
    {
        const MolCLJInfo &mol0 = molarray[i];

        for (int j=i+1; j<nmols; ++j)
        {
            const MolCLJInfo &mol1 = molarray[j];

            CLJFF::calculateEnergy( mol0, mol1, space(),
                                    switchingFunction(),
                                    workspace() );

            cnrg += workspace().cnrg;
            ljnrg += workspace().ljnrg;
        }
    }

    this->setComponent( components().coulomb(), cnrg );
    this->setComponent( components().lj(), ljnrg );
    this->setComponent( components().total(), cnrg+ljnrg );

    //clear the list of changed molecules
    changedmols.clear();
    molid_to_changedindex.clear();
    removedmols.clear();
}

/** Recalculate the energy by using a delta from the old configuration */
void InterCLJFF::recalculateViaDelta()
{
    int nmols = mols.count();
    int nchanged = changedmols.count();

    if (nchanged == 0)
    {
        //no changed molecules, so just recalculate the total energy
        recalculateTotalEnergy();
        return;
    }
    else if (nmols == 0)
    {
        //all of the molecules have been deleted! Set the energy to
        //zero and clear the moved list
        this->setComponent( components().coulomb(), 0 );
        this->setComponent( components().lj(), 0 );
        this->setComponent( components().total(), 0 );

        changedmols.clear();
        removedmols.clear();
        molid_to_changedindex.clear();

        return;
    }

    //temp components to hold the change in energies
    double icnrg = 0;
    double iljnrg = 0;

    //now loop over all of the molecules in the system
    const MolCLJInfo *molarray = mols.constData();
    const ChangedMolCLJInfo *changedarray = changedmols.constData();

    QHash<MoleculeID,int>::const_iterator it;

    for (int i=0; i<nmols; ++i)
    {
        const MolCLJInfo &mol = molarray[i];

        //is this one of the moved molecules?
        it = molid_to_changedindex.constFind(mol.molecule().ID());

        if (it == molid_to_changedindex.constEnd())
        {
            //this isn't one of the changed molecules.
            //Calculate the difference in energy between this molecule
            //and the old and new configurations of all of the changed
            //molecules
            for (int j=0; j<nchanged; ++j)
            {
                const ChangedMolCLJInfo &changedmol = changedarray[j];

                //calculate the energy of the new configuration
                calculateEnergy(mol, changedmol.newParts(), space(),
                                switchingFunction(), workspace());

                icnrg += workspace().cnrg;
                iljnrg += workspace().ljnrg;

                calculateEnergy(mol, changedmol.oldParts(), space(),
                                switchingFunction(), workspace());

                icnrg -= workspace().cnrg;
                iljnrg -= workspace().ljnrg;
            }
        }
        else
        {
            //this is one of the changed molecules!

            //calculate the energy of the changed molecule with all changed
            //molecules that are after it in the changed mols array
            int idx = it.value();

            const ChangedMolCLJInfo &changedmol0 = changedarray[idx];

            if (changedmol0.changedAll())
            {
                //the whole of this molecule has been moved - calculate the
                //change in energy of the whole of this molecule with the whole
                //of the other moved molecules
                for (int j=idx+1; j<nchanged; ++j)
                {
                    const ChangedMolCLJInfo &changedmol1 = changedarray[j];

                    calculateEnergy(changedmol0.newMol(), changedmol1.newMol(),
                                    space(), switchingFunction(), workspace());

                    icnrg += workspace().cnrg;
                    iljnrg += workspace().ljnrg;

                    calculateEnergy(changedmol0.oldMol(), changedmol1.oldMol(),
                                    space(), switchingFunction(), workspace());

                    icnrg -= workspace().cnrg;
                    iljnrg -= workspace().ljnrg;
                }
            }
            else
            {
                //only part of this molecule has been moved...
                for (int j=idx+1; j<nchanged; ++j)
                {
                    const ChangedMolCLJInfo &changedmol1 = changedarray[j];

                    if (changedmol1.changedAll())
                    {
                        //the whole of the other molecule has moved, so we need
                        //to calculate the change in energy of the whole molecule
                        //with this molecule
                        calculateEnergy(changedmol0.newMol(), changedmol1.newMol(),
                                        space(), switchingFunction(), workspace());

                        icnrg += workspace().cnrg;
                        iljnrg += workspace().ljnrg;

                        calculateEnergy(changedmol0.oldMol(), changedmol1.oldMol(),
                                        space(), switchingFunction(), workspace());

                        icnrg -= workspace().cnrg;
                        iljnrg -= workspace().ljnrg;
                    }
                    else
                    {
                        //this is the complicated, but hopefully much rarer edge case
                        // - part of this molecule *and* part of the other molecule have
                        // changed. For now, I will do the less efficient method of calculating
                        // the change in energy, namely calculating the energy of the part
                        // of mol0 that changed with all of mol1, then the energy of the part
                        // of mol1 that changed with all of mol0, then finally, subtracting the
                        // energy of the part of mol0 with the part of mol1 (which would
                        // otherwise be double-counted).

                        //ok, so first the changed part of mol0 with all of mol1
                        calculateEnergy(changedmol0.newParts(), changedmol1.newMol(),
                                        space(), switchingFunction(), workspace());

                        icnrg += workspace().cnrg;
                        iljnrg += workspace().ljnrg;

                        calculateEnergy(changedmol0.oldParts(), changedmol1.oldMol(),
                                        space(), switchingFunction(), workspace());

                        icnrg -= workspace().cnrg;
                        iljnrg -= workspace().ljnrg;

                        //now the changed part of mol1 with all of mol0
                        calculateEnergy(changedmol0.newMol(), changedmol1.newParts(),
                                        space(), switchingFunction(), workspace());

                        icnrg += workspace().cnrg;
                        iljnrg += workspace().ljnrg;

                        calculateEnergy(changedmol0.oldMol(), changedmol1.oldParts(),
                                        space(), switchingFunction(), workspace());

                        icnrg -= workspace().cnrg;
                        iljnrg -= workspace().ljnrg;

                        //finally, remove the doubly-counted contribution of the
                        //changed part of mol0 with the changed part of mol1
                        calculateEnergy(changedmol0.newParts(), changedmol1.newParts(),
                                        space(), switchingFunction(), workspace());

                        icnrg -= workspace().cnrg;   // subtract doubly-counted contribution
                        iljnrg -= workspace().ljnrg;

                        calculateEnergy(changedmol0.oldParts(), changedmol1.oldParts(),
                                        space(), switchingFunction(), workspace());

                        icnrg += workspace().cnrg;
                        iljnrg += workspace().ljnrg;
                    }
                }
            }
        }
    }

    //finally, loop over all of the molecules that have been removed - the energy
    //of non-changed molecules with removed molecules has already been calculated,
    //as has the energy of moved molecules that are before the removed molecules
    //in the moved list. We only now have to calculate the energy of the removed
    //molecules with all of the molecules that lie above us in the moved list
    for (QSet<MoleculeID>::const_iterator it = removedmols.constBegin();
         it != removedmols.constEnd();
         ++it)
    {
        //get the index of the removed molecule in the moved list
        int idx = molid_to_changedindex.value(*it);

        const ChangedMolCLJInfo &removedmol = changedarray[idx];

        //calculate the change in energy associated with removing this molecule
        //(only have to do the 'old' energy, as the new energy is zero)

        for (int j=idx+1; j<nchanged; ++j)
        {
            const ChangedMolCLJInfo &changedmol = changedarray[j];

            calculateEnergy(removedmol.oldMol(), changedmol.oldMol(),
                            space(), switchingFunction(), workspace());

            icnrg -= workspace().cnrg;
            iljnrg -= workspace().ljnrg;
        }
    }

    //clear the list of changed molecules
    changedmols.clear();
    molid_to_changedindex.clear();
    removedmols.clear();

    //save the new total energy of this forcefield
    this->changeComponent( components().coulomb(), icnrg );
    this->changeComponent( components().lj(), iljnrg );
    this->changeComponent( components().total(), icnrg+iljnrg );
}

/** Calculate the total energy of this forcefield. This will either
    calculate the energy from scratch, or it will calculate it as a
    delta based on the parts of the forcefield that have moved since
    the last evaluation. */
void InterCLJFF::recalculateEnergy()
{
    if (changedmols.isEmpty())
    {
        this->recalculateTotalEnergy();
    }
    else
    {
        this->recalculateViaDelta();
    }
}

/** Save the current state of 'mol' */
void InterCLJFF::setCurrentState(const MolCLJInfo &mol)
{
    MoleculeID id = mol.molecule().ID();

    if (molid_to_molindex.contains(id))
    {
        int idx = molid_to_molindex.value(id);
        mols[idx] = mol;
    }
    else
    {
        int idx = mols.count();
        molid_to_molindex.insert(id, idx);
        mols.append(mol);

        if (removedmols.contains(id))
            removedmols.remove(id);
    }
}

/** Remove 'mol' from the current state */
void InterCLJFF::removeCurrentState(const Molecule &mol)
{
    MoleculeID id = mol.ID();

    if (molid_to_molindex.contains(id))
    {
        // need to remove from list, reindex hash, remove from hash

        removedmols.insert(id);
    }
}

/** Add the molecule 'mol' to this forcefield using the optionally
    supplied map to find the forcefield parameters amongst the
    molecule's properties.

    \throw SireMol::missing_property
    \throw SireError::invalid_cast
*/
bool InterCLJFF::add(const Molecule &mol, const ParameterMap &map)
{
    //get the Molecule's ID
    MoleculeID id = mol.ID();

    //does the molecule already exist and has been changed since
    //the last update?
    if ( molid_to_changedindex.contains(id) )
    {
        int idx = molid_to_changedindex.value(id);

        //get the existing change record
        const ChangedMolCLJInfo &changerecord = changedmols.at(idx);

        //create a new record for the new state
        MolCLJInfo newmol(mol, parameters(), map);

        //save this new molecule
        this->setCurrentState(newmol);

        //replace this with a new change record describing the
        //new molecule
        changedmols[idx] = ChangedMolCLJInfo( changerecord.oldMol(), newmol );
    }
    //does the molecule already exist, but has not been changed yet?
    else if ( molid_to_molindex.contains(id) )
    {
        //get the old molecule
        int idx = molid_to_molindex.value(id);

        MolCLJInfo oldmol = mols.at(idx);

        //is this the same version molecule, and same parameter map?
        if ( mol.version() == oldmol.molecule().version() and
             map == oldmol.map() )
        {
            //they are the same - there is nothing to do
            return isDirty();
        }

        //create a record for the new molecule
        MolCLJInfo newmol( mol, parameters(), map );

        //save the new state of this molecule
        this->setCurrentState(newmol);

        //create a change record that moved from the old state
        //to the new state
        idx = changedmols.count();
        molid_to_changedindex.insert(id, idx);

        changedmols.append( ChangedMolCLJInfo(oldmol, newmol) );
    }
    else
    {
        MolCLJInfo newmol( mol, parameters(), map );
        this->setCurrentState(newmol);

        int idx = changedmols.count();
        molid_to_changedindex.insert(id, idx);

        changedmols.append( ChangedMolCLJInfo(MolCLJInfo(), newmol) );
    }

    //we've added a molecule - change the forcefield's major version number
    this->incrementMajorVersion();

    return isDirty();
}

/** Remove the molecule 'mol' */
bool InterCLJFF::remove(const Molecule &molecule)
{
    //get the Molecule's ID
    MoleculeID id = molecule.ID();

    if ( removedmols.contains(id) )
    {
        //no point double-removing a molecule
        return isDirty();
    }
    else if ( molid_to_changedindex.contains(id) )
    {
        //remove the molecule from the current state
        this->removeCurrentState(molecule);

        int idx = molid_to_changedindex.value(id);

        //this molecule exists and has been changed - remove it
        const ChangedMolCLJInfo &changerecord = changedmols.at(id);

        //create a new record for this molecule based on the two states
        changedmols[idx] = ChangedMolCLJInfo( changerecord.oldMol(),
                                              MolCLJInfo() );

        removedmols.insert(id);
        this->incrementMajorVersion();
    }
    else if ( molid_to_molindex.contains(id) )
    {
        //create a record to removed from the current state
        int idx = molid_to_molindex.value(id);

        MolCLJInfo oldmol = mols.at(idx);

        //remove the molecule from the current state
        this->removeCurrentState(molecule);

        //add a change record to record this removal
        idx = changedmols.count();
        molid_to_changedindex.insert(id, idx);

        changedmols.append( ChangedMolCLJInfo(oldmol, MolCLJInfo()) );

        removedmols.insert(id);
        this->incrementMajorVersion();
    }

    return isDirty();
}

/** Change the molecule 'molecule' */
bool InterCLJFF::change(const Molecule &molecule)
{
    //try to find this molecule in this forcefield,
    //based on its ID number
    MoleculeID id = molecule.ID();

    if ( removedmols.contains(id) )
    {
        //no point changing a molecule that has been removed
        return isDirty();
    }
    else if ( molid_to_changedindex.contains(id) )
    {
        //the molecule has been changed since the last evaluation
        int idx = molid_to_changedindex.value(id);

        const ChangedMolCLJInfo &changerecord = changedmols.at(idx);

        ChangedMolCLJInfo newrecord = changerecord.change(molecule,
                                                          parameters());

        changedmols[idx] = newrecord;

        this->setCurrentState(newrecord.newMol());

        this->incrementMinorVersion();
    }
    else if ( molid_to_molindex.contains(id) )
    {
        //the molecule has not yet been changed since the last evaluation
        int idx = molid_to_molindex.value(id);

        MolCLJInfo oldmol = mols.at(idx);

        //only change if the version number is different...
        if (molecule.version() != oldmol.molecule().version())
        {
            ChangedMolCLJInfo newrecord = oldmol.change(molecule, parameters());

            this->setCurrentState(newrecord.newMol());

            idx = changedmols.count();
            molid_to_changedindex.insert(id, idx);
            changedmols.append(newrecord);

            this->incrementMinorVersion();
        }
    }

    return isDirty();
}

/** Change the residue 'residue' */
bool InterCLJFF::change(const Residue &residue)
{
    Molecule molecule = residue.molecule();

    MoleculeID id = molecule.ID();

    if ( removedmols.contains(id) )
    {
        //no point moving a residue that has been removed
        return isDirty();
    }
    else if ( molid_to_changedindex.contains(id) )
    {
        //changing a residue in an already-changed molecule
        int idx = molid_to_changedindex.value(id);

        const ChangedMolCLJInfo &changerecord = changedmols.at(idx);

        ChangedMolCLJInfo newrecord = changerecord.change(residue, parameters());

        this->setCurrentState(newrecord.newMol());

        changedmols[idx] = newrecord;

        this->incrementMinorVersion();
    }
    else if ( molid_to_molindex.contains(id) )
    {
        //changing a residue that has not yet been changed
        int idx = molid_to_molindex.value(id);

        MolCLJInfo oldmol = mols.at(idx);

        ChangedMolCLJInfo newrecord = oldmol.change(residue, parameters());

        this->setCurrentState( newrecord.newMol() );

        idx = changedmols.count();
        molid_to_changedindex.insert(id, idx);
        changedmols.append(newrecord);

        this->incrementMinorVersion();
    }

    return isDirty();
}
