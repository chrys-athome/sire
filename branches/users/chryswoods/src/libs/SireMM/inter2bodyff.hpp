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

#ifndef SIREMM_INTER2BODYFF_HPP
#define SIREMM_INTER2BODYFF_HPP

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
template<class T>
class Inter2BodyFF;
}

template<class T>
QDataStream& operator<<(QDataStream&, const SireMM::Inter2BodyFF<T>&);
QDataStream& operator>>(QDataStream&, SireMM::Inter2BodyFF<T>&);

namespace SireMM
{

/** This is the mid-base class of all 2-body intermolecular forcefields.
    This class calculates the intermolecular
    energy between all pairs of molecules held in this forcefield. This
    forcefield must be built on top of a base class, T, that provides
    the type T::Molecule, which is used to hold all of the information
    about a molecule in the forcefield, T::ChangedMolecule that contains
    information about the molecule as it is changed, T::Energy,
    which holds the energy of the molecule-molecule interaction,
    and the function 
    
    <code>
    static T::Energy T::calculateEnergy(const T::Molecule&, const T::Molecule&,
                                        const Space&, const SwitchingFunction&,
                                        DistMatrix&, T::ParamPairMatrix&) 
    </code>

    which calculates the energy between two molecules, using the 
    provided space, switching function and distance and parameter
    pair matrix workspaces.
    
    @author Christopher Woods
*/
template<class T>
class Inter2BodyFF : public T
{

friend QDataStream& ::operator<<<>(QDataStream&, const Inter2BodyFF<T>&);
friend QDataStream& ::operator>><>(QDataStream&, Inter2BodyFF<T>&);

public:
    Inter2BodyFF();

    Inter2BodyFF(const Space &space, const SwitchingFunction &switchingfunction);

    Inter2BodyFF(const Inter2BodyFF &other);

    ~Inter2BodyFF();

    void mustNowRecalculateFromScratch();

    bool change(const PartialMolecule &molecule);
    bool change(const Molecules &molecules);

    bool remove(const PartialMolecule &molecule);
    bool remove(const Molecules &molecules);

    bool contains(const PartialMolecule &molecule) const;

    bool refersTo(MoleculeID molid) const;

    QSet<FFBase::Group> groupsReferringTo(MoleculeID molid) const;

    QSet<MoleculeID> moleculeIDs() const;

    PartialMolecule molecule(MoleculeID molid) const;

    Molecules contents() const;

protected:
    T::Energy calculateEnergy();
    
    bool add(const T::Molecule &molecule);

    T::ChangedMolecule changeRecord(MoleculeID molid) const;

    bool applyChange(MoleculeID molid,
                     const T::ChangedMolecule &new_molecule);

    void _pvt_copy(const FFBase &other);

private:
    
    T::Energy recalculateViaDelta();
    T::Energy recalculateTotalEnergy();
    
    void updateCurrentState(const T::Molecule &new_molecule);
    void removeFromCurrentState(MoleculeID molid);

    /** All of the molecules that have at least one atom
        in this forcefield */
    QVector<T::Molecule> mols;

    /** Hash mapping the MoleculeID to the index of the molecule in 'mols' */
    QHash<MoleculeID, uint> molid_to_index;

    /** Information about all of the changed molecules since the
        last energy calculation */
    QVector<T::ChangedMolecule> changed_mols;

    /** Hash mapping the MoleculeID of a changed molecule to its
        index in changed_mols */
    QHash<MoleculeID, uint> molid_to_changedindex;

    /** MoleculeIDs of all molecules that have been removed since
        the last energy evaluation */
    QSet<MoleculeID> removed_mols;

    /** Whether or not a total energy recalculation is required */
    bool need_total_recalc;
};

/** Constructor */
template<class T>
Inter2BodyFF<T>::Inter2BodyFF() : T(), need_total_recalc(true)
{}

/** Construct a forcefield using the passed Space, and
    switching function */
template<class T>
Inter2BodyFF<T>::Inter2BodyFF(const Space &space, const SwitchingFunction &switchfunc)
                : T(space, switchfunc), need_total_recalc(true)
{}

/** Copy constructor */
template<class T>
Inter2BodyFF<T>::Inter2BodyFF(const Inter2BodyFF &other)
                : T(other),
                  mols(other.mols),
                  molid_to_index(other.molid_to_index),
                  changed_mols(other.changed_mols),
                  molid_to_changedindex(other.molid_to_changedindex),
                  removed_mols(other.removed_mols),
                  need_total_recalc(other.need_total_recalc)
{}

/** Destructor */
template<class T>
Inter2BodyFF<T>::~Inter2BodyFF()
{}

/** Copy assignment function used by derived classes */
template<class T>
void Inter2BodyFF<T>::_pvt_copy(const FFBase &ffbase)
{
    T::_pvt_copy(ffbase);

    const Inter2BodyFF<T> &other = dynamic_cast<const Inter2BodyFF<T>&>(ffbase);

    mols = other.mols;
    molid_to_index = other.molid_to_index;

    changed_mols = other.changed_mols;
    molid_to_changedindex = other.molid_to_changedindex;

    removed_mols = other.removed_mols;

    need_total_recalc = other.need_total_recalc;
}

/** Tell the forcefield that it has to recalculate everything from
    scratch */
template<class T>
void Inter2BodyFF<T>::mustNowRecalculateFromScratch()
{
    if (not need_total_recalc)
    {
        need_total_recalc = true;
        changed_mols.clear();
        molid_to_changedindex.clear();
        removed_mols.clear();
    }
}

/** Recalculate and return the total energy of this forcefield from scratch */
template<class T>
T::Energy Inter2BodyFF<T>::recalculateTotalEnergy()
{
    //calculate the total energy of all molecule pairs...
    T::Energy nrg(0);

    int nmols = mols.count();

    const T::Molecule *mols_array = mols.constData();

    DistMatrix distmat(30,30);
    T::ParamPairMatrix paramat(30,30);

    //loop over all molecule pairs
    for (int i=0; i<nmols-1; ++i)
    {
        const T::Molecule &mol0 = mols_array[i];

        for (int j=i+1; j<nmols; ++j)
        {
            const T::Molecule &mol1 = mols_array[j];

            nrg += T::calculateEnergy( mol0, mol1,
                                       space(),
                                       switchingFunction(),
                                       distmat,
                                       paramat );
        }
    }

    //clear the list of changed molecules
    changed_mols.clear();
    molid_to_changedindex.clear();
    removed_mols.clear();
    need_total_recalc = false;

    //return the total energy
    return nrg;
}

/** Recalculate the energy by using a delta from the old configuration */
template<class T>
T::Energy Inter2BodyFF<T>::recalculateViaDelta(const T &current_nrg)
{
    int nmols = mols.count();
    int nchanged = changed_mols.count();

    if (need_total_recalc or nchanged == 0)
    {
        //no changed molecules, so just recalculate the total energy
        return recalculateTotalEnergy();
    }
    else if (nmols == 0)
    {
        //all of the molecules have been deleted! Set the energy to
        //zero and clear the moved list
        changed_mols.clear();
        molid_to_changedindex.clear();
        removed_mols.clear();
        need_total_recalc = false;

        return T::Energy(0);
    }

    //temp components to hold the change in energies
    T::Energy nrg(0);

    //now loop over all of the molecules in the system
    const T::Molecule *mols_array = mols.constData();
    const T::ChangedMolecule *changed_array = changed_mols.constData();

    DistMatrix distmat(30,30);
    T::ParamPairMatrix paramat(30,30);

    QHash<MoleculeID,uint>::const_iterator it;

    for (int i=0; i<nmols; ++i)
    {
        const T::Molecule &mol = mols_array[i];

        //is this one of the changed molecules?
        it = molid_to_changedindex.constFind(mol.molecule().ID());

        if (it == molid_to_changedindex.constEnd())
        {
            //this isn't one of the changed molecules.
            //Calculate the difference in energy between this molecule
            //and the old and new configurations of all of the changed
            //molecules
            for (int j=0; j<nchanged; ++j)
            {
                const T::ChangedMolecule &changed_mol = changed_array[j];

                //calculate the energy of the new configuration
                nrg += T::calculateEnergy(mol, changed_mol.newParts(),
                                          space(), switchingFunction(),
                                          distmat, paramat);

                //subtract the energy of the old configuration
                nrg -= T::calculateEnergy(mol, changed_mol.oldParts(),
                                          space(), switchingFunction(),
                                          distmat, paramat);
            }
        }
        else
        {
            //this is one of the changed molecules!

            //calculate the energy of the changed molecule with all changed
            //molecules that are after it in the changed mols array
            int idx = it.value();

            const T::ChangedMolecule &changed_mol0 = changed_array[idx];

            if (changed_mol0.changedAll())
            {
                //the whole of this molecule has been moved - calculate the
                //change in energy of the whole of this molecule with the whole
                //of the other moved molecules
                for (int j=idx+1; j<nchanged; ++j)
                {
                    const T::ChangedMolecule &changed_mol1 = changed_array[j];

                    //add on the new interaction energy
                    nrg += T::calculateEnergy(changed_mol0.newMolecule(),
                                              changed_mol1.newMolecule(),
                                              space(), switchingFunction(),
                                              distmat, paramat);

                    //subtract the old interaction energy
                    nrg -= T::calculateEnergy(changed_mol0.oldMolecule(),
                                              changed_mol1.oldMolecule(),
                                              space(), switchingFunction(),
                                              distmat, paramat);
                }
            }
            else
            {
                //only part of this molecule has been moved...
                for (int j=idx+1; j<nchanged; ++j)
                {
                    const T::ChangedMolecule &changed_mol1 = changed_array[j];

                    if (changed_mol1.changedAll())
                    {
                        //the whole of the other molecule has moved, so we need
                        //to calculate the change in energy of the whole molecule
                        //with this molecule
                        nrg += T::calculateEnergy(changed_mol0.newMolecule(),
                                                  changed_mol1.newMolecule(),
                                                  space(), switchingFunction(),
                                                  distmat, paramat);

                        //subtract the old energy
                        nrg -= T::calculateEnergy(changed_mol0.oldMolecule(),
                                                  changed_mol1.oldMolecule(),
                                                  space(), switchingFunction(),
                                                  distmat, paramat);
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
                        nrg += T::calculateEnergy(changed_mol0.newParts(),
                                                  changed_mol1.newMolecule(),
                                                  space(), switchingFunction(),
                                                  distmat, paramat);


                        nrg -= T::calculateEnergy(changed_mol0.oldParts(),
                                                  changed_mol1.oldMolecule(),
                                                  space(), switchingFunction(),
                                                  distmat, paramat);

                        //now the changed part of mol1 with all of mol0
                        nrg += T::calculateEnergy(changed_mol0.newMolecule(),
                                                  changed_mol1.newParts(),
                                                  space(), switchingFunction(),
                                                  distmat, paramat);

                        nrg -= T::calculateEnergy(changed_mol0.oldMolecule(),
                                                  changed_mol1.oldParts(),
                                                  space(), switchingFunction(),
                                                  distmat, paramat);

                        //finally, remove the doubly-counted contribution of the
                        //changed part of mol0 with the changed part of mol1

                        //subtract doubly counted contributions
                        nrg -= T::calculateEnergy(changed_mol0.newParts(),
                                                  changed_mol1.newParts(),
                                                  space(), switchingFunction(),
                                                  distmat, paramat);

                        nrg += T::calculateEnergy(changed_mol0.oldParts(),
                                                  changed_mol1.oldParts(),
                                                  space(), switchingFunction(),
                                                  distmat, paramat);
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

        const T::ChangedMolecule &removed_mol = changed_array[idx];

        //calculate the change in energy associated with removing this molecule
        //(only have to do the 'old' energy, as the new energy is zero)

        for (int j=idx+1; j<nchanged; ++j)
        {
            const T::ChangedMolecule &changed_mol = changed_array[j];

            nrg -= T::calculateEnergy(removed_mol.oldMolecule(),
                                      changed_mol.oldMolecule(),
                                      space(), switchingFunction(),
                                      distmat, paramat);
        }
    }

    //clear the list of changed molecules
    changed_mols.clear();
    molid_to_changedindex.clear();
    removed_mols.clear();
    need_total_recalc = false;

    return current_nrg + nrg;
}

/** Calculate and return the total energy of this forcefield. This will either
    calculate the energy from scratch, or it will calculate it as a
    delta based on the parts of the forcefield that have moved since
    the last evaluation. */
template<class T>
T::Energy Inter2BodyFF<T>::calculateEnergy()
{
    if (need_total_recalc or changed_mols.isEmpty())
    {
        return this->recalculateTotalEnergy();
    }
    else
    {
        return this->recalculateViaDelta();
    }
}

/** Save the current state of 'mol' */
template<class T>
void Inter2BodyFF<T>::updateCurrentState(const T::Molecule &mol)
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

        if ( (not need_total_recalc) and removed_mols.contains(id) )
            removed_mols.remove(id);
    }
}

/** Remove 'mol' from the current state */
template<class T>
void Inter2BodyFF<T>::removeFromCurrentState(MoleculeID molid)
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

            const T::Molecule *mols_array = mols.constData();

            for (uint i=0; i<nmols; ++i)
            {
                molid_to_index.insert( mols_array[i].molecule().ID(), i );
            }
        }

        if (not need_total_recalc)
            removed_mols.insert(molid);
    }
}

/** Apply the change described by 'changed_mol' - return whether or not
    this changed anything */
template<class T>
bool Inter2BodyFF<T>::applyChange(MoleculeID molid, 
                                  const T::ChangedMolecule &changed_mol)
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

    if (not need_total_recalc)
    {
        //save the change
        QHash<MoleculeID,uint>::const_iterator it = molid_to_changedindex.constFind(molid);

        if (it != molid_to_changedindex.constEnd())
        {
            MoleculeID molid = it.key();
            uint idx = *it;

            //the molecule has been changed before
            if (changed_mols[idx].oldMolecule() == changed_mol.newMolecule())
            {
                //we have changed back to the old state!
                changed_mols[idx] = T::ChangedMolecule();
                molid_to_changedindex.remove(molid);
                removed_mols.remove(molid);

                if (molid_to_changedindex.isEmpty())
                {
                    //no molecules have now changed - the forcefield is clean again
                    molid_to_changedindex.clear();
                    changed_mols.clear();
                    removed_mols.clear();
                    this->setClean();
                }
            }
            else
                changed_mols[it.value()] = changed_mol;
        }
        else if (not changed_mol.nothingChanged())
        {
            //this is a new change
            uint idx = changed_mols.count();

            changed_mols.append(changed_mol);
            molid_to_changedindex.insert(molid, idx);
        }
        else
            return false;
    }

    return true;
}

/** Get the current change record for a molecule */
T::ChangedMolecule Inter2BodyFF<T>::changeRecord(MoleculeID molid) const
{
    if ( not need_total_recalc )
    {
        QHash<MoleculeID,uint>::const_iterator it = molid_to_changedindex.find(molid);

        if (it != molid_to_changedindex.end())
            //this molecule has been changed before - return the record
            //of that change
            return changed_mols.constData()[ *it ];
    }

    QHash<MoleculeID,uint>::const_iterator it = molid_to_index.find(molid);

    if (it != molid_to_index.end())
    {
        //this molecule has not been changed before - return a change
        //record that has the state, but doesn't change it
        return T::ChangedMolecule( mols.constData()[*it] );
    }
    else
    {
        //this molecule does not yet exist in this forcefield -
        //return an empty change record
        return T::ChangedMolecule();
    }
}

/** Add the molecule 'molecule' to this forcefield using the optionally
    supplied map to find the forcefield parameters amongst the
    molecule's properties.

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
template<class T>
bool Inter2BodyFF<T>::add(const T::Molecule &molecule)
{
    //get the molecule's ID
    MoleculeID molid = molecule.molecule().ID();

    T::ChangedMolecule new_molecule = changeRecord(molid).add(molecule);

    if (this->applyChange(molid, new_molecule))
    {
        this->incrementMajorVersion();
        return true;
    }
    else
        return isDirty();
}

/** Remove the molecule 'molecule' */
template<class T>
bool Inter2BodyFF<T>::remove(const PartialMolecule &molecule)
{
    MoleculeID molid = molecule.ID();

    T::ChangedMolecule new_molecule = changeRecord(molid).remove(molecule);

    if (this->applyChange(molid, new_molecule))
    {
        this->incrementMajorVersion();
        return true;
    }
    else
        return isDirty();
}

/** Remove lots of molecules */
template<class T>
bool Inter2BodyFF<T>::remove(const Molecules &molecules)
{
    return T::remove(molecules);
}

/** Change the molecule 'molecule' */
template<class T>
bool Inter2BodyFF<T>::change(const PartialMolecule &molecule)
{
    MoleculeID molid = molecule.ID();

    T::ChangedMolecule new_molecule = this->changeRecord(molid).change(molecule);

    if (this->applyChange(molid, new_molecule))
    {
        this->incrementMinorVersion();
        return true;
    }
    else
        return this->isDirty();
}

/** Change lots of molecules */
template<class T>
bool Inter2BodyFF<T>::change(const Molecules &molecules)
{
    return T::change(molecules);
}

/** Return whether this forcefield contains a complete copy of
    any version of the partial molecule 'molecule' */
template<class T>
bool Inter2BodyFF<T>::contains(const PartialMolecule &molecule) const
{
    QHash<MoleculeID,uint>::const_iterator it = molid_to_index.find(molecule.ID());

    if (it == molid_to_index.end())
        return false;

    return mols.constData()[it.value()].molecule()
                             .selectedAtoms().contains(molecule.selectedAtoms());
}

/** Return whether or not this forcefield contains *any part* of
    any version of the molecule with ID 'molid' */
template<class T>
bool Inter2BodyFF<T>::refersTo(MoleculeID molid) const
{
    return molid_to_index.contains(molid);
}

/** Return the groups that refer to the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
template<class T>
QSet<FFBase::Group> Inter2BodyFF<T>::groupsReferringTo(MoleculeID molid) const
{
    QSet<FFBase::Group> groups;

    if (this->refersTo(molid))
    {
        groups.insert(this->groups().main());
    }

    return groups;
}

/** Return the set of all of the ID numbers of all of the
    molecules that are referred to by this forcefield
    (i.e. all molecules that have at least some part
    in this forcefield) */
template<class T>
QSet<MoleculeID> Inter2BodyFF<T>::moleculeIDs() const
{
    return molid_to_index.keys().toSet();
}

/** Return the copy of the molecule in this forcefield that
    has the ID == molid

    \throw SireMol::missing_molecule
*/
template<class T>
PartialMolecule Inter2BodyFF<T>::molecule(MoleculeID molid) const
{
    QHash<MoleculeID,uint>::const_iterator it = molid_to_index.find(molid);

    if (it == molid_to_index.end())
        throw SireMol::missing_molecule( QObject::tr(
            "The Inter2BodyFF forcefield (%1, %2 : %3) does not contain the "
            "molecule with ID == %4.")
                .arg(this->name()).arg(this->ID())
                .arg(this->version().toString()).arg(molid),
                    CODELOC );

    return mols.constData()[it.value()].molecule();
}

/** Return all of the molecules (and parts of molecules) that
    are in this forcefield */
template<class T>
Molecules Inter2BodyFF<T>::contents() const
{
    Molecules all_mols;

    int nmols = mols.count();

    if (nmols > 0)
    {
        all_mols.reserve(nmols);

        const CLJMolecule *mols_array = mols.constData();

        for (int i=0; i<nmols; ++i)
        {
            const PartialMolecule &mol = mols_array[i].molecule();

            all_mols.add( mol );
        }
    }

    return all_mols;
}

}


SIRE_END_HEADER

#endif
