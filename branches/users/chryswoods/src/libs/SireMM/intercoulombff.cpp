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

#include "intercoulombff.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireFF;
using namespace SireStream;

///////////
/////////// Implementation of InterCoulombFF::Components
///////////

/** Constructor */
InterCoulombFF::Components::Components() : CoulombFF::Components()
{}

/** Constructor that just passes its arguments up to the parent's constructor */
InterCoulombFF::Components::Components(const FFBase &ffbase, const Symbols &symbols)
               : CoulombFF::Components(ffbase,symbols)
{}

/** Copy constructor */
InterCoulombFF::Components::Components(const Components &other)
               : CoulombFF::Components(other)
{}

/** Destructor */
InterCoulombFF::Components::~Components()
{}

///////////
/////////// Implementation of InterCoulombFF::Parameters
///////////

/** Constructor */
InterCoulombFF::Parameters::Parameters() : CoulombFF::Parameters()
{}

/** Copy constructor */
InterCoulombFF::Parameters::Parameters(const Parameters &other)
               : CoulombFF::Parameters(other)
{}

/** Destructor */
InterCoulombFF::Parameters::~Parameters()
{}

///////////
/////////// Implementation of InterCoulombFF::Groups
///////////

/** Constructor */
InterCoulombFF::Groups::Groups() : CoulombFF::Groups()
{}

/** Copy constructor */
InterCoulombFF::Groups::Groups(const Groups &other)
               : CoulombFF::Groups(other)
{}

/** Destructor */
InterCoulombFF::Groups::~Groups()
{}

///////////
/////////// Implementation of InterCoulombFF
///////////

static const RegisterMetaType<InterCoulombFF> r_intercoulff;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const InterCoulombFF &intercoulff)
{
    writeHeader(ds, r_intercoulff, 1);

    SharedDataStream sds(ds);

    sds << intercoulff.mols
        << intercoulff.changed_mols
        << intercoulff.removed_mols
        << static_cast<const CoulombFF&>(intercoulff);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      InterCoulombFF &intercoulff)
{
    VersionID v = readHeader(ds, r_intercoulff);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> intercoulff.mols
            >> intercoulff.changed_mols
            >> intercoulff.removed_mols
            >> static_cast<CoulombFF&>(intercoulff);

        //rebuild molid_to_index and molid_to_changedindex

        uint nmols = intercoulff.mols.count();

        if (nmols > 0)
        {
            QHash<MoleculeID,uint> molid_to_index;
            molid_to_index.reserve(nmols);

            const CoulombFF::CoulombMolecule *mol_array = intercoulff.mols.constData();

            for (uint i=0; i<nmols; ++i)
            {
                molid_to_index.insert( mol_array[i].molecule().ID(), i );
            }

            intercoulff.molid_to_index = molid_to_index;
        }
        else
            intercoulff.molid_to_index.clear();

        nmols = intercoulff.changed_mols.count();

        if (nmols > 0)
        {
            QHash<MoleculeID,uint> molid_to_changedindex;
            molid_to_changedindex.reserve(nmols);

            const CoulombFF::ChangedCoulombMolecule *mol_array
                                          = intercoulff.changed_mols.constData();

            for (uint i=0; i<nmols; ++i)
            {
                MoleculeID molid = mol_array[i].oldMolecule().molecule().ID();

                if (molid == 0)
                    molid = mol_array[i].newMolecule().molecule().ID();

                BOOST_ASSERT(molid != 0);

                molid_to_changedindex.insert( molid, i );
            }

            intercoulff.molid_to_changedindex = molid_to_changedindex;
        }
        else
            intercoulff.molid_to_changedindex.clear();
    }
    else
        throw version_error(v, "1", r_intercoulff, CODELOC);

    return ds;
}

/** Constructor */
InterCoulombFF::InterCoulombFF() : CoulombFF()
{}

/** Construct a Coulomb forcefield using the passed Space, combining rules and
    switching function */
InterCoulombFF::InterCoulombFF(const Space &space, const SwitchingFunction &switchfunc)
               : CoulombFF(space, switchfunc)
{}

/** Copy constructor */
InterCoulombFF::InterCoulombFF(const InterCoulombFF &other)
               : CoulombFF(other),
                 mols(other.mols),
                 molid_to_index(other.molid_to_index),
                 changed_mols(other.changed_mols),
                 molid_to_changedindex(other.molid_to_changedindex),
                 removed_mols(other.removed_mols)
{}

/** Destructor */
InterCoulombFF::~InterCoulombFF()
{}

/** Assignment operator */
InterCoulombFF& InterCoulombFF::operator=(const InterCoulombFF &other)
{
    mols = other.mols;
    molid_to_index = other.molid_to_index;

    changed_mols = other.changed_mols;
    molid_to_changedindex = other.molid_to_changedindex;

    removed_mols = other.removed_mols;

    return *this;
}

/** Recalculate the total energy of this forcefield from scratch */
void InterCoulombFF::recalculateTotalEnergy()
{
    //calculate the total Coulomb energy of all molecule pairs...
    double cnrg = 0;

    int nmols = mols.count();

    const CoulombMolecule *mols_array = mols.constData();

    //loop over all molecule pairs
    for (int i=0; i<nmols-1; ++i)
    {
        const CoulombMolecule &mol0 = mols_array[i];

        for (int j=i+1; j<nmols; ++j)
        {
            const CoulombMolecule &mol1 = mols_array[j];

            cnrg += CoulombFF::calculateEnergy( mol0, mol1,
                                                space(),
                                                switchingFunction(),
                                                distanceMatrix() );
        }
    }

    this->setComponent( components().coulomb(), cnrg );
    this->setComponent( components().total(), cnrg );

    //clear the list of changed molecules
    changed_mols.clear();
    molid_to_changedindex.clear();
    removed_mols.clear();
}

/** Recalculate the energy by using a delta from the old configuration */
void InterCoulombFF::recalculateViaDelta()
{
    int nmols = mols.count();
    int nchanged = changed_mols.count();

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
        this->setComponent( components().total(), 0 );

        changed_mols.clear();
        molid_to_changedindex.clear();
        removed_mols.clear();

        return;
    }

    //temp components to hold the change in energies
    double cnrg = 0;

    //now loop over all of the molecules in the system
    const CoulombMolecule *mols_array = mols.constData();
    const ChangedCoulombMolecule *changed_array = changed_mols.constData();

    QHash<MoleculeID,uint>::const_iterator it;

    for (int i=0; i<nmols; ++i)
    {
        const CoulombMolecule &mol = mols_array[i];

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
                const ChangedCoulombMolecule &changed_mol = changed_array[j];

                //calculate the energy of the new configuration
                cnrg += calculateEnergy(mol, changed_mol.newParts(),
                                        space(), switchingFunction(),
                                        distanceMatrix());

                //subtract the energy of the old configuration
                cnrg -= calculateEnergy(mol, changed_mol.oldParts(),
                                        space(), switchingFunction(),
                                        distanceMatrix());
            }
        }
        else
        {
            //this is one of the changed molecules!

            //calculate the energy of the changed molecule with all changed
            //molecules that are after it in the changed mols array
            int idx = it.value();

            const ChangedCoulombMolecule &changed_mol0 = changed_array[idx];

            if (changed_mol0.changedAll())
            {
                //the whole of this molecule has been moved - calculate the
                //change in energy of the whole of this molecule with the whole
                //of the other moved molecules
                for (int j=idx+1; j<nchanged; ++j)
                {
                    const ChangedCoulombMolecule &changed_mol1 = changed_array[j];

                    //add on the new interaction energy
                    cnrg += calculateEnergy(changed_mol0.newMolecule(),
                                            changed_mol1.newMolecule(),
                                            space(), switchingFunction(),
                                            distanceMatrix());

                    //subtract the old interaction energy
                    cnrg -= calculateEnergy(changed_mol0.oldMolecule(),
                                            changed_mol1.oldMolecule(),
                                            space(), switchingFunction(),
                                            distanceMatrix());
                }
            }
            else
            {
                //only part of this molecule has been moved...
                for (int j=idx+1; j<nchanged; ++j)
                {
                    const ChangedCoulombMolecule &changed_mol1 = changed_array[j];

                    if (changed_mol1.changedAll())
                    {
                        //the whole of the other molecule has moved, so we need
                        //to calculate the change in energy of the whole molecule
                        //with this molecule
                        cnrg += calculateEnergy(changed_mol0.newMolecule(),
                                                changed_mol1.newMolecule(),
                                                space(), switchingFunction(),
                                                distanceMatrix());

                        //subtract the old energy
                        cnrg -= calculateEnergy(changed_mol0.oldMolecule(),
                                                changed_mol1.oldMolecule(),
                                                space(), switchingFunction(),
                                                distanceMatrix());
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
                        cnrg += calculateEnergy(changed_mol0.newParts(),
                                                changed_mol1.newMolecule(),
                                                space(), switchingFunction(),
                                                distanceMatrix());


                        cnrg -= calculateEnergy(changed_mol0.oldParts(),
                                                changed_mol1.oldMolecule(),
                                                space(), switchingFunction(),
                                                distanceMatrix());

                        //now the changed part of mol1 with all of mol0
                        cnrg += calculateEnergy(changed_mol0.newMolecule(),
                                                changed_mol1.newParts(),
                                                space(), switchingFunction(),
                                                distanceMatrix());

                        cnrg -= calculateEnergy(changed_mol0.oldMolecule(),
                                                changed_mol1.oldParts(),
                                                space(), switchingFunction(),
                                                distanceMatrix());

                        //finally, remove the doubly-counted contribution of the
                        //changed part of mol0 with the changed part of mol1

                        //subtract doubly counted contributions
                        cnrg -= calculateEnergy(changed_mol0.newParts(),
                                                changed_mol1.newParts(),
                                                space(), switchingFunction(),
                                                distanceMatrix());

                        cnrg += calculateEnergy(changed_mol0.oldParts(),
                                                changed_mol1.oldParts(),
                                                space(), switchingFunction(),
                                                distanceMatrix());
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
    for (QSet<MoleculeID>::const_iterator it = removed_mols.constBegin();
         it != removed_mols.constEnd();
         ++it)
    {
        //get the index of the removed molecule in the moved list
        int idx = molid_to_changedindex.value(*it);

        const ChangedCoulombMolecule &removed_mol = changed_array[idx];

        //calculate the change in energy associated with removing this molecule
        //(only have to do the 'old' energy, as the new energy is zero)

        for (int j=idx+1; j<nchanged; ++j)
        {
            const ChangedCoulombMolecule &changed_mol = changed_array[j];

            cnrg -= calculateEnergy(removed_mol.oldMolecule(),
                                    changed_mol.oldMolecule(),
                                    space(), switchingFunction(),
                                    distanceMatrix());
        }
    }

    //clear the list of changed molecules
    changed_mols.clear();
    molid_to_changedindex.clear();
    removed_mols.clear();

    //save the new total energy of this forcefield
    this->changeComponent( components().coulomb(), cnrg );
    this->changeComponent( components().total(), cnrg );
}

/** Calculate the total energy of this forcefield. This will either
    calculate the energy from scratch, or it will calculate it as a
    delta based on the parts of the forcefield that have moved since
    the last evaluation. */
void InterCoulombFF::recalculateEnergy()
{
    if (changed_mols.isEmpty())
    {
        this->recalculateTotalEnergy();
    }
    else
    {
        this->recalculateViaDelta();
    }
}

/** Save the current state of 'mol' */
void InterCoulombFF::updateCurrentState(const CoulombFF::CoulombMolecule &mol)
{
    MoleculeID id = mol.molecule().ID();

    if (molid_to_index.contains(id))
    {
        uint idx = molid_to_index.value(id);
        mols[idx] = mol;
    }
    else
    {
        uint idx = mols.count();
        molid_to_index.insert(id, idx);
        mols.append(mol);

        if (removed_mols.contains(id))
            removed_mols.remove(id);
    }
}

/** Remove 'mol' from the current state */
void InterCoulombFF::removeFromCurrentState(MoleculeID molid)
{
    if (molid_to_index.contains(molid))
    {
        // need to remove from list, reindex hash, remove from hash
        int idx = molid_to_index[molid];

        // remove this item from the array of molecules...
        mols.remove(idx);

        // reindex the vector
        molid_to_index.clear();

        uint nmols = mols.count();

        if (nmols > 0)
        {
            molid_to_index.reserve(nmols);

            const CoulombMolecule *mols_array = mols.constData();

            for (uint i=0; i<nmols; ++i)
            {
                molid_to_index.insert( mols_array[i].molecule().ID(), i );
            }
        }

        removed_mols.insert(molid);
    }
}

/** Apply the change described by 'changed_mol' */
bool InterCoulombFF::applyChange(MoleculeID molid, const ChangedCoulombMolecule &changed_mol)
{
    if (changed_mol.isEmpty())
        return false;

    if (changed_mol.newMolecule().isEmpty())
    {
        //the molecule has been completely removed!
        BOOST_ASSERT( molid == changed_mol.oldMolecule().molecule().ID() );
        this->removeFromCurrentState(molid);
    }
    else
    {
        BOOST_ASSERT( molid == changed_mol.newMolecule().molecule().ID() );
        this->updateCurrentState(changed_mol.newMolecule());
    }

    //save the change
    QHash<MoleculeID,uint>::const_iterator it = molid_to_changedindex.constFind(molid);

    if (it != molid_to_changedindex.constEnd())
    {
        //the molecule has been changed before
        changed_mols[it.value()] = changed_mol;
    }
    else
    {
        //this is a new change
        uint idx = changed_mols.count();

        changed_mols.append(changed_mol);
        molid_to_changedindex.insert(molid, idx);
    }

    return true;
}

/** Get the current change record for a molecule */
CoulombFF::ChangedCoulombMolecule InterCoulombFF::changeRecord(MoleculeID molid) const
{
    if (molid_to_changedindex.contains(molid))
    {
        //this molecule has been changed before - return the record
        //of that change
        return changed_mols.at( molid_to_changedindex[molid] );
    }
    else if (molid_to_index.contains(molid))
    {
        //this molecule has not been changed before - return a change
        //record that has the state, but doesn't change it
        return ChangedCoulombMolecule( mols.at(molid_to_index[molid]) );
    }
    else
    {
        //this molecule does not yet exist in this forcefield -
        //return an empty change record
        return ChangedCoulombMolecule();
    }
}

/** Private class used by the "add" functions to actually add the
    molecule or part of molecule */
template<class T>
bool InterCoulombFF::_pvt_add(const T &mol, const ParameterMap &map)
{
    //get the molecule's ID
    MoleculeID molid = mol.ID();

    ChangedCoulombMolecule new_molecule =
                changeRecord(molid).add( mol,
                                         map.source(this->parameters().coulomb()) );

    if (this->applyChange(molid, new_molecule))
    {
        this->incrementMajorVersion();
        return true;
    }
    else
        return isDirty();
}

/** Add the molecule 'molecule' to this forcefield using the optionally
    supplied map to find the forcefield parameters amongst the
    molecule's properties.

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
bool InterCoulombFF::add(const Molecule &molecule, const ParameterMap &map)
{
    return this->_pvt_add<Molecule>(molecule, map);
}

/** Add the residue 'residue' to this forcefield using the optionally
    supplied map to find the forcefield parameters amongst the
    residue's properties. Note that the property used must agree
    with the rest of the molecule if it is already in this forcefield.

    \throw SireBase::missing_property
    \throw SireMol::invalid_cast
*/
bool InterCoulombFF::add(const Residue &residue, const ParameterMap &map)
{
    return this->_pvt_add<Residue>(residue, map);
}

/** Add the atom 'atom' to this forcefield using the optionally
    supplied map to find the forcefield parameters amongst the
    atom's properties. Note that the property used must agree
    with the rest of the molecule if it is already in this forcefield.

    \throw SireBase::missing_property
    \throw SireMol::invalid_cast
*/
bool InterCoulombFF::add(const NewAtom &atom, const ParameterMap &map)
{
    return this->_pvt_add<NewAtom>(atom, map);
}

/** Add the selected atoms from 'molecule' to this forcefield
    using the supplied map to find the forcefield parameters
    amongst the molecule's properties. Note that the property
    used must agree with the rest of the molecule if it is already
    in this forcefield.

    \throw SireBase::missing_property
    \throw SireMol::invalid_cast
*/
bool InterCoulombFF::add(const Molecule &molecule, const AtomSelection &selected_atoms,
                    const ParameterMap &map)
{
    //get the molecule's ID
    MoleculeID molid = molecule.ID();

    ChangedCoulombMolecule new_molecule = changeRecord(molid);

    if (new_molecule.oldMolecule().isEmpty())
    {
        new_molecule = ChangedCoulombMolecule( CoulombMolecule(),
                                          CoulombMolecule( molecule, selected_atoms,
                                          map.source(this->parameters().coulomb()) ) );
    }
    else
    {
        new_molecule = new_molecule.add( selected_atoms,
                                         map.source(this->parameters().coulomb()) );
    }

    if (this->applyChange(molid, new_molecule))
    {
        this->incrementMajorVersion();
        return true;
    }
    else
        return isDirty();
}

/** Private class used by the "remove" functions to actually remove the
    molecule or part of molecule */
template<class T>
bool InterCoulombFF::_pvt_remove(const T &mol)
{
    MoleculeID molid = mol.ID();

    ChangedCoulombMolecule new_molecule = changeRecord(molid).remove(mol);

    if (this->applyChange(molid, new_molecule))
    {
        this->incrementMajorVersion();
        return true;
    }
    else
        return isDirty();
}

/** Remove the molecule 'molecule' */
bool InterCoulombFF::remove(const Molecule &molecule)
{
    return this->_pvt_remove<Molecule>(molecule);
}

/** Remove the residue 'residue' */
bool InterCoulombFF::remove(const Residue &residue)
{
    return this->_pvt_remove<Residue>(residue);
}

/** Remove the atom 'atom' */
bool InterCoulombFF::remove(const NewAtom &atom)
{
    return this->_pvt_remove<NewAtom>(atom);
}

/** Remove the selected atoms from 'molecule' from this forcefield */
bool InterCoulombFF::remove(const Molecule &molecule, const AtomSelection &selected_atoms)
{
    MoleculeID molid = molecule.ID();

    ChangedCoulombMolecule new_molecule = changeRecord(molid).remove(selected_atoms);

    if (this->applyChange(molid, new_molecule))
    {
        this->incrementMajorVersion();
        return true;
    }
    else
        return isDirty();
}

/** Private class used by the "change" functions to actually change the
    molecule or part of molecule */
template<class T>
bool InterCoulombFF::_pvt_change(const T &mol)
{
    MoleculeID molid = mol.ID();

    ChangedCoulombMolecule new_molecule = this->changeRecord(molid).change(mol);

    if (this->applyChange(molid, new_molecule))
    {
        this->incrementMinorVersion();
        return true;
    }
    else
        return this->isDirty();
}

/** Change the molecule 'molecule' */
bool InterCoulombFF::change(const Molecule &molecule)
{
    return this->_pvt_change<Molecule>(molecule);
}

/** Change the residue 'residue' */
bool InterCoulombFF::change(const Residue &residue)
{
    return this->_pvt_change<Residue>(residue);
}

/** Change the atom 'atom' */
bool InterCoulombFF::change(const NewAtom &atom)
{
    return this->_pvt_change<NewAtom>(atom);
}
