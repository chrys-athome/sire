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

#include "intergroupcoulombff.h"

#include "SireMol/partialmolecule.h"
#include "SireMol/molecules.h"

#include "SireMol/errors.h"
#include "SireFF/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireFF;
using namespace SireStream;

///////////
/////////// Implementation of InterGroupCoulombFF::Components
///////////

/** Constructor */
InterGroupCoulombFF::Components::Components() : CoulombFF::Components()
{}

/** Constructor that just passes its arguments up to the parent's constructor */
InterGroupCoulombFF::Components::Components(const FFBase &ffbase,
                                            const Symbols &symbols)
                    : CoulombFF::Components(ffbase,symbols)
{}

/** Copy constructor */
InterGroupCoulombFF::Components::Components(const Components &other)
                    : CoulombFF::Components(other)
{}

/** Destructor */
InterGroupCoulombFF::Components::~Components()
{}

///////////
/////////// Implementation of InterGroupCoulombFF::Parameters
///////////

/** Constructor */
InterGroupCoulombFF::Parameters::Parameters() : CoulombFF::Parameters()
{}

/** Copy constructor */
InterGroupCoulombFF::Parameters::Parameters(const Parameters &other)
                    : CoulombFF::Parameters(other)
{}

/** Destructor */
InterGroupCoulombFF::Parameters::~Parameters()
{}

///////////
/////////// Implementation of InterGroupCoulombFF::Groups
///////////

/** Constructor */
InterGroupCoulombFF::Groups::Groups() : CoulombFF::Groups()
{
    a = this->getUniqueID();
    b = this->getUniqueID();
}

/** Copy constructor */
InterGroupCoulombFF::Groups::Groups(const Groups &other)
                    : CoulombFF::Groups(other), a(other.a), b(other.b)
{}

/** Destructor */
InterGroupCoulombFF::Groups::~Groups()
{}

/** Static instance of this class returned by all InterGroupCoulombFF objects */
InterGroupCoulombFF::Groups InterGroupCoulombFF::Groups::default_group;

///////////
/////////// Implementation of InterGroupCoulombFF
///////////

static const RegisterMetaType<InterGroupCoulombFF> r_intercoulff;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const InterGroupCoulombFF &intercoulff)
{
    writeHeader(ds, r_intercoulff, 1);

    SharedDataStream sds(ds);

    sds << intercoulff.mols[0]
        << intercoulff.mols[1]
        << intercoulff.changed_mols[0]
        << intercoulff.changed_mols[1]
        << intercoulff.need_total_recalc
        << static_cast<const CoulombFF&>(intercoulff);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      InterGroupCoulombFF &intercoulff)
{
    VersionID v = readHeader(ds, r_intercoulff);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> intercoulff.mols[0]
            >> intercoulff.mols[1]
            >> intercoulff.changed_mols[0]
            >> intercoulff.changed_mols[1]
            >> intercoulff.need_total_recalc
            >> static_cast<CoulombFF&>(intercoulff);

        //rebuild mols and changed_mols

        for (int group=0; group<2; ++group)
        {
            uint nmols = intercoulff.mols[group].count();

            if (nmols > 0)
            {
                QHash<MoleculeID,uint> molid_to_index;
                molid_to_index.reserve(nmols);

                const CoulombFF::CoulombMolecule *mol_array =
                                            intercoulff.mols[group].constData();

                for (uint i=0; i<nmols; ++i)
                {
                    molid_to_index.insert( mol_array[i].molecule().ID(), i );
                }

                intercoulff.molid_to_index[group] = molid_to_index;
            }
            else
                intercoulff.molid_to_index[group].clear();

            nmols = intercoulff.changed_mols[group].count();

            if (nmols > 0)
            {
                QHash<MoleculeID,uint> molid_to_changedindex;
                molid_to_changedindex.reserve(nmols);

                const CoulombFF::ChangedCoulombMolecule *mol_array
                                = intercoulff.changed_mols[group].constData();

                for (uint i=0; i<nmols; ++i)
                {
                    MoleculeID molid = mol_array[i].oldMolecule().molecule().ID();

                    if (molid == 0)
                        molid = mol_array[i].newMolecule().molecule().ID();

                    BOOST_ASSERT(molid != 0);

                    molid_to_changedindex.insert( molid, i );
                }

                intercoulff.molid_to_changedindex[group] = molid_to_changedindex;
            }
            else
                intercoulff.molid_to_changedindex[group].clear();
        }
    }
    else
        throw version_error(v, "1", r_intercoulff, CODELOC);

    return ds;
}

/** Constructor */
InterGroupCoulombFF::InterGroupCoulombFF() : CoulombFF(), need_total_recalc(true)
{}

/** Construct a Coulomb forcefield using the passed Space, combining rules and
    switching function */
InterGroupCoulombFF::InterGroupCoulombFF(const Space &space,
                                         const SwitchingFunction &switchfunc)
                    : CoulombFF(space, switchfunc), need_total_recalc(true)
{}

/** Copy constructor */
InterGroupCoulombFF::InterGroupCoulombFF(const InterGroupCoulombFF &other)
                    : CoulombFF(other), need_total_recalc(other.need_total_recalc)
{
    for (int i=0; i<2; ++i)
    {
        mols[i] = other.mols[i];
        molid_to_index[i] = other.molid_to_index[i];

        changed_mols[i] = other.changed_mols[i];
        molid_to_changedindex[i] = other.molid_to_changedindex[i];
    }
}

/** Destructor */
InterGroupCoulombFF::~InterGroupCoulombFF()
{}

/** Copy assignment function used by derived classes */
void InterGroupCoulombFF::_pvt_copy(const FFBase &ffbase)
{
    CoulombFF::_pvt_copy(ffbase);

    const InterGroupCoulombFF &other = dynamic_cast<const InterGroupCoulombFF&>(ffbase);

    for (int i=0; i<2; ++i)
    {
        mols[i] = other.mols[i];
        molid_to_index[i] = other.molid_to_index[i];

        changed_mols[i] = other.changed_mols[i];
        molid_to_changedindex[i] = other.molid_to_changedindex[i];
    }

    need_total_recalc = other.need_total_recalc;
}

/** Tell the forcefield that it has to recalculate everything from
    scratch */
void InterGroupCoulombFF::mustNowRecalculateFromScratch()
{
    if (not need_total_recalc)
    {
        need_total_recalc = true;

        for (int i=0; i<2; ++i)
        {
            changed_mols[i].clear();
            molid_to_changedindex[i].clear();
        }
    }
}

/** Return the ID of the other group

    \throw SireFF::invalid_group
*/
int InterGroupCoulombFF::otherGroup(int group_id) const
{
    BOOST_ASSERT( group_id == 0 or group_id == 1 );
    return (group_id == 0);
}

/** Assert that there is no molecule with the ID == molid in
    any other group than the group with ID == group_id,
    and that there is indeed a group available with this ID number.

    \throw SireFF::invalid_group
*/
void InterGroupCoulombFF::assertValidGroup(int group_id, MoleculeID molid) const
{
    if ( molid_to_index[ otherGroup(group_id) ].contains(molid) )
    {
        QString newgroup = "A";
        QString oldgroup = "B";

        if ( FFBase::Group(group_id) == groups().B() )
        {
            newgroup = "B";
            oldgroup = "A";
        }

        throw SireFF::invalid_group( QObject::tr(
              "The molecule with ID == %1 cannot be put in the group %2 "
              "as group %3 already contains a molecule with this ID number.")
                  .arg(molid).arg(newgroup,oldgroup), CODELOC );
    }
}

/** Recalculate the total energy of this forcefield from scratch */
void InterGroupCoulombFF::recalculateTotalEnergy()
{
    //calculate the total Coulomb energy between pairs of
    //molecules between the two groups

    double cnrg = 0;

    int nmols0 = mols[0].count();
    int nmols1 = mols[1].count();

    DistMatrix distmat(30,30);

    if (nmols0 > 0 and nmols1 > 0)
    {
        const CoulombMolecule *mols0_array = mols[0].constData();
        const CoulombMolecule *mols1_array = mols[1].constData();

        for (int i=0; i<nmols0; ++i)
        {
            const CoulombMolecule &mol0 = mols0_array[i];

            for (int j=0; j<nmols1; ++j)
            {
                const CoulombMolecule &mol1 = mols1_array[j];

                cnrg += CoulombFF::calculateEnergy( mol0, mol1,
                                                    space(),
                                                    switchingFunction(),
                                                    distmat );
            }
        }
    }

    this->setComponent( components().coulomb(), cnrg );
    this->setComponent( components().total(), cnrg );

    //clear the list of changed molecules
    for (int i=0; i<2; ++i)
    {
        changed_mols[i].clear();
        molid_to_changedindex[i].clear();
    }

    need_total_recalc = false;
}

/** Recalculate the energy knowing that only the group at index 'changed_idx'
    has changed */
double InterGroupCoulombFF::recalculateWithOneChangedGroup(int changed_idx)
{
    BOOST_ASSERT(changed_idx == 0 or changed_idx == 1);

    //just calculate the change in energy of the changed parts
    //of the changed group with all of the unchanged group

    //get the index of the unchanged group
    //(0 if 1 has changed, 1 if 0 has changed)
    int unchanged_idx = (changed_idx == 0);

    BOOST_ASSERT(unchanged_idx == 0 or unchanged_idx == 1);

    int nchanged = changed_mols[changed_idx].count();
    const ChangedCoulombMolecule *changed_array = changed_mols[changed_idx].constData();

    int nmols = mols[unchanged_idx].count();
    const CoulombMolecule *mols_array = mols[unchanged_idx].constData();

    double cnrg = 0;

    DistMatrix distmat(30,30);

    for (int i=0; i<nchanged; ++i)
    {
        const ChangedCoulombMolecule &changedmol = changed_array[i];

        for (int j=0; j<nmols; ++j)
        {
            const CoulombMolecule &mol = mols_array[j];

            //calculate the change in energy between this molecule and the changed
            //parts of the changed molecule
            cnrg += calculateEnergy(mol, changedmol.newParts(),
                                    space(), switchingFunction(),
                                    distmat);

            cnrg -= calculateEnergy(mol, changedmol.oldParts(),
                                    space(), switchingFunction(),
                                    distmat);
        }
    }

    return cnrg;
}

/** Recalculate the energy of the unchanged parts of group 'unchanged_idx'
    with the changed parts of group 'changed_idx' */
double InterGroupCoulombFF::recalculateChangedWithUnchanged(int unchanged_idx,
                                                            int changed_idx)
{
    BOOST_ASSERT( ( unchanged_idx == 0 or unchanged_idx == 1 ) and
                  ( changed_idx == 0 or changed_idx == 1) and
                  ( unchanged_idx != changed_idx ) );

    int nchanged = changed_mols[changed_idx].count();
    const ChangedCoulombMolecule *changed_array = changed_mols[changed_idx].constData();

    int nmols = mols[unchanged_idx].count();
    const CoulombMolecule *mols_array = mols[unchanged_idx].constData();

    double cnrg = 0;

    DistMatrix distmat(30,30);

    for (int i=0; i<nmols; ++i)
    {
        const CoulombMolecule &mol = mols_array[i];

        //has this molecule changed? if so, then skip it
        if ( molid_to_changedindex[unchanged_idx].contains(mol.molecule().ID()) )
        {
            continue;
        }

        //this molecule hasn't changed, so calculate its energy with all of
        //the changed molecules in the other group
        for (int j=0; j<nchanged; ++j)
        {
            const ChangedCoulombMolecule &changedmol = changed_array[j];

            cnrg += calculateEnergy( mol, changedmol.newParts(),
                                     space(), switchingFunction(),
                                     distmat );

            cnrg -= calculateEnergy( mol, changedmol.oldParts(),
                                     space(), switchingFunction(),
                                     distmat );
        }
    }

    return cnrg;
}

/** Recalculate the energy of the changed parts of one group with the
    changed parts of the other group */
double InterGroupCoulombFF::recalculateChangedWithChanged()
{
    int nchanged0 = changed_mols[0].count();
    int nchanged1 = changed_mols[1].count();

    const ChangedCoulombMolecule *changed_array0 = changed_mols[0].constData();
    const ChangedCoulombMolecule *changed_array1 = changed_mols[1].constData();

    double cnrg = 0;

    DistMatrix distmat(30,30);

    for (int i=0; i<nchanged0; ++i)
    {
        const ChangedCoulombMolecule &changedmol0 = changed_array0[i];

        if (changedmol0.changedAll())
        {
            //the whole of mol0 has changed - calculate the change in energy
            //of the whole of this molecule with the whole of the changed molecules
            //in group 1
            for (int j=0; j<nchanged1; ++j)
            {
                const ChangedCoulombMolecule &changedmol1 = changed_array1[j];

                cnrg += calculateEnergy( changedmol0.newMolecule(),
                                         changedmol1.newMolecule(),
                                         space(), switchingFunction(),
                                         distmat );

                cnrg -= calculateEnergy( changedmol0.oldMolecule(),
                                         changedmol1.oldMolecule(),
                                         space(), switchingFunction(),
                                         distmat );
            }
        }
        else
        {
            //only part of mol0 has changed - what we do now depends on how much
            //the molecules in the other group have changed...
            for (int j=0; j<nchanged1; ++j)
            {
                const ChangedCoulombMolecule &changedmol1 = changed_array1[j];

                if (changedmol1.changedAll())
                {
                    //all of mol1 has changed - calculate the change in energy
                    //of the whole of this molecule with the whole of mol0
                    cnrg += calculateEnergy( changedmol0.newMolecule(),
                                             changedmol1.newMolecule(),
                                             space(), switchingFunction(),
                                             distmat );

                    cnrg -= calculateEnergy( changedmol0.oldMolecule(),
                                             changedmol1.oldMolecule(),
                                             space(), switchingFunction(),
                                             distmat );
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

                    //first, the changed parts of mol0 with all of mol1
                    cnrg += calculateEnergy( changedmol0.newParts(),
                                             changedmol1.newMolecule(),
                                             space(), switchingFunction(),
                                             distmat );

                    cnrg -= calculateEnergy( changedmol0.oldParts(),
                                             changedmol1.oldMolecule(),
                                             space(), switchingFunction(),
                                             distmat );

                    //now the changed parts of mol1 with the whole of mol0
                    cnrg += calculateEnergy( changedmol0.newMolecule(),
                                             changedmol1.newParts(),
                                             space(), switchingFunction(),
                                             distmat );

                    cnrg -= calculateEnergy( changedmol0.oldMolecule(),
                                             changedmol1.oldParts(),
                                             space(), switchingFunction(),
                                             distmat );

                    //finally, remove the double-counted interaction of the parts
                    //of mol0 that changed with the parts of mol1 that changed
                    cnrg -= calculateEnergy( changedmol0.newParts(),
                                             changedmol1.newParts(),
                                             space(), switchingFunction(),
                                             distmat );

                    cnrg += calculateEnergy( changedmol0.oldParts(),
                                             changedmol1.oldParts(),
                                             space(), switchingFunction(),
                                             distmat );
                }
            }
        }
    }

    return cnrg;
}

/** Recalculate the energy knowing that both of the groups have changes */
double InterGroupCoulombFF::recalculateWithTwoChangedGroups()
{
    //first calculate the energy of the changed parts
    //of each group with the unchanged parts of the other group
    double cnrg = recalculateChangedWithUnchanged(0,1);
    cnrg += recalculateChangedWithUnchanged(1,0);

    //now calculate the change in energy between the two changed
    //parts of each group
    cnrg += recalculateChangedWithChanged();

    return cnrg;
}

/** Recalculate the energy by using a delta from the old configuration */
void InterGroupCoulombFF::recalculateViaDelta()
{
    int nmols0 = mols[0].count();
    int nmols1 = mols[1].count();

    int nchanged0 = changed_mols[0].count();
    int nchanged1 = changed_mols[1].count();

    if ( (nchanged0 == 0 and nchanged1 == 0) or nmols0 == 0 or nmols1 == 0 )
    {
        //no changed molecules, or no molecules,
        //so just recalculate the total energy
        this->recalculateTotalEnergy();
        return;
    }

    double cnrg = 0;

    //calculate the change in energy of group 0 with group 1
    if ( nchanged0 == 0 )
    {
        cnrg = this->recalculateWithOneChangedGroup(1);
    }
    else if ( nchanged1 == 0 )
    {
        cnrg = this->recalculateWithOneChangedGroup(0);
    }
    else
    {
        cnrg = this->recalculateWithTwoChangedGroups();
    }

    this->changeComponent( components().coulomb(), cnrg );
    this->changeComponent( components().total(), cnrg );

    //clear the list of changed molecules
    for (int i=0; i<2; ++i)
    {
        changed_mols[i].clear();
        molid_to_changedindex[i].clear();
    }

    need_total_recalc = false;
}

/** Calculate the total energy of this forcefield. This will either
    calculate the energy from scratch, or it will calculate it as a
    delta based on the parts of the forcefield that have moved since
    the last evaluation. */
void InterGroupCoulombFF::recalculateEnergy()
{
    if ( need_total_recalc or (changed_mols[0].isEmpty() and
                               changed_mols[1].isEmpty()) )
    {
        this->recalculateTotalEnergy();
    }
    else
    {
        this->recalculateViaDelta();
    }
}

/** Update the current state of 'molecule' in the forcefield */
void InterGroupCoulombFF::updateCurrentState(int group_idx,
                                             const CoulombFF::CoulombMolecule &molecule)
{
    MoleculeID molid = molecule.molecule().ID();

    //get the group that contains this molecule (one of the groups *must*
    //contain this molecule!)
    BOOST_ASSERT( group_idx == 0 or group_idx == 1 );
    BOOST_ASSERT( molid_to_index[group_idx].contains(molid) );
    BOOST_ASSERT( not molid_to_index[ group_idx == 0 ].contains(molid) );

    uint idx = molid_to_index[group_idx].value(molid);

    mols[group_idx][idx] = molecule;
}

/** Add the 'molecule' to the current state of group 'group_idx' */
void InterGroupCoulombFF::addToCurrentState(int group_idx,
                                            const CoulombFF::CoulombMolecule &molecule)
{
    BOOST_ASSERT( group_idx == 0 or group_idx == 1 );

    MoleculeID molid = molecule.molecule().ID();

    BOOST_ASSERT( not (molid_to_index[0].contains(molid) or
                       molid_to_index[1].contains(molid)) );

    uint idx = mols[group_idx].count();
    molid_to_index[group_idx].insert( molid, idx );
    mols[group_idx].append(molecule);
}

/** Remove the molecule with ID == molid from the current state
    of the forcefield */
void InterGroupCoulombFF::removeFromCurrentState(int group_idx, MoleculeID molid)
{
    BOOST_ASSERT( group_idx == 0 or group_idx == 1 );

    if (molid_to_index[group_idx].contains(molid))
    {
        // need to remove from list, reindex hash, remove from hash
        int idx = molid_to_index[group_idx][molid];

        // remove this item from the array of molecules...
        mols[group_idx].remove(idx);

        // reindex the vector
        molid_to_index[group_idx].clear();

        uint nmols = mols[group_idx].count();

        if (nmols > 0)
        {
            molid_to_index[group_idx].reserve(nmols);

            const CoulombMolecule *mols_array = mols[group_idx].constData();

            for (uint i=0; i<nmols; ++i)
            {
                molid_to_index[group_idx].insert(
                                    mols_array[i].molecule().ID(), i );
            }
        }

        return;
    }
}

/** Return the change record for the molecule with ID == molid. This
    returns an empty record if this molecule is not in this forcefield */
CoulombFF::ChangedCoulombMolecule InterGroupCoulombFF::changeRecord(MoleculeID molid) const
{
    for (int i=0; i<2; ++i)
    {
        //only return the changerecord if the molecule is currently in the
        //group - beware of old changerecords of molecules that were deleted
        //from a group - these will hang around until the next energy
        //evaluation
        if (molid_to_index[i].contains(molid))
        {
            if (molid_to_changedindex[i].contains(molid))
                return changed_mols[i].at( molid_to_changedindex[i][molid] );
            else
                return ChangedCoulombMolecule( mols[i].at(molid_to_index[i][molid]) );
        }
    }

    //the molecule is not in this forcefield
    return ChangedCoulombMolecule();
}

/** Record the change described by 'changed_mol' */
void InterGroupCoulombFF::recordChange(int group_idx, MoleculeID molid,
                                       const ChangedCoulombMolecule &changed_mol)
{
    //save the change
    QHash<MoleculeID,uint>::const_iterator it
                        = molid_to_changedindex[group_idx].constFind(molid);

    if (it != molid_to_changedindex[group_idx].constEnd())
    {
        //the molecule has been changed before
        changed_mols[group_idx][it.value()] = changed_mol;
    }
    else
    {
        //this is a new change
        uint idx = changed_mols[group_idx].count();

        changed_mols[group_idx].append(changed_mol);
        molid_to_changedindex[group_idx].insert(molid, idx);
    }
}

/** Apply the change described by 'changed_mol' */
bool InterGroupCoulombFF::applyChange(MoleculeID molid,
                                      const ChangedCoulombMolecule &changed_mol)
{
    if (changed_mol.nothingChanged())
        return false;

    //get the current group in which the molecule resides
    int group_idx = molid_to_index[1].contains(molid);
    BOOST_ASSERT( group_idx == 0 or group_idx == 1 );

    if (not molid_to_index[group_idx].contains(molid))
    {
        //this function cannot be used to add molecules!
        throw SireMol::missing_molecule( QObject::tr(
                "You cannot add parts to a molecule that doesn't yet exist "
                "in this forcefield without first saying to which group it "
                "should belong!"), CODELOC );
    }

    BOOST_ASSERT( not changed_mol.oldMolecule().isEmpty() );

    if (changed_mol.newMolecule().isEmpty())
    {
        //the molecule has been completely removed
        BOOST_ASSERT( molid == changed_mol.oldMolecule().molecule().ID() );
        this->removeFromCurrentState(group_idx, molid);
    }
    else
    {
        BOOST_ASSERT( molid == changed_mol.newMolecule().molecule().ID() );
        this->updateCurrentState(group_idx, changed_mol.newMolecule());
    }

    if (not need_total_recalc)
        this->recordChange(group_idx, molid, changed_mol);

    return true;
}

/** Change the molecule 'molecule' */
bool InterGroupCoulombFF::change(const PartialMolecule &molecule)
{
    MoleculeID molid = molecule.ID();

    ChangedCoulombMolecule new_molecule = this->changeRecord(molid).change(molecule);

    if (this->applyChange(molid, new_molecule))
    {
        this->incrementMinorVersion();
        return true;
    }
    else
        return isDirty();
}

/** Change lots of molecules */
bool InterGroupCoulombFF::change(const Molecules &molecules)
{
    return CoulombFF::change(molecules);
}

/** Remove the molecule 'molecule' */
bool InterGroupCoulombFF::remove(const PartialMolecule &molecule)
{
    MoleculeID molid = molecule.ID();

    ChangedCoulombMolecule new_molecule = changeRecord(molid).remove(molecule);

    if (this->applyChange(molid, new_molecule))
    {
        this->incrementMajorVersion();
        return true;
    }
    else
        return isDirty();
}

/** Remove lots of molecules */
bool InterGroupCoulombFF::remove(const Molecules &molecules)
{
    return CoulombFF::remove(molecules);
}

/** Remove the molecule 'molecule' from the group 'group'

    \throw SireFF::missing_group
*/
bool InterGroupCoulombFF::removeFrom(const FFBase::Group &group,
                                     const PartialMolecule &molecule)
{
    if (this->refersTo(molecule.ID(),group))
        return this->remove(molecule);
    else
        return false;
}

/** Remove lots of molecules from the group 'group'

    \throw SireFF::invalid_group
*/
bool InterGroupCoulombFF::removeFrom(const FFBase::Group &group,
                                     const Molecules &molecules)
{
    return CoulombFF::removeFrom(group, molecules);
}

/** Remove the molecule 'molecule' from group 'A' */
bool InterGroupCoulombFF::removeFromA(const PartialMolecule &molecule)
{
    return this->removeFrom(groups().A(), molecule);
}

/** Remove lots of molecules from group 'A' */
bool InterGroupCoulombFF::removeFromA(const Molecules &molecules)
{
    return CoulombFF::removeFrom(groups().A(), molecules);
}

/** Remove the molecule 'molecule' from group 'B' */
bool InterGroupCoulombFF::removeFromB(const PartialMolecule &molecule)
{
    return this->removeFrom(groups().B(), molecule);
}

/** Remove lots of molecules from group 'B' */
bool InterGroupCoulombFF::removeFromB(const Molecules &molecules)
{
    return CoulombFF::removeFrom(groups().B(), molecules);
}

/** Add the molecule 'molecule' to this forcefield using the optionally
    supplied map to find the forcefield parameters amongst the
    molecule's properties.

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
bool InterGroupCoulombFF::add(const PartialMolecule &molecule,
                              const ParameterMap &map)
{
    //get the molecule's ID
    MoleculeID molid = molecule.ID();

    ChangedCoulombMolecule new_molecule =
                changeRecord(molid).add( molecule,
                                         map.source(this->parameters().coulomb()) );

    if (this->applyChange(molid, new_molecule))
    {
        this->incrementMajorVersion();
        return true;
    }
    else
        return isDirty();
}

/** Add lots of molecules to the forcefield

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
bool InterGroupCoulombFF::add(const Molecules &molecules,
                              const ParameterMap &map)
{
    return CoulombFF::add(molecules, map);
}

/** Return the index of the group in this forcefield

    \throw SireFF::invalid_group
*/
int InterGroupCoulombFF::groupIndex(FFBase::Group group) const
{
    if (group == groups().A())
        return 0;
    else if (group == groups().B())
        return 1;
    else
    {
        throw SireFF::invalid_group( QObject::tr(
                    "InterGroupCoulombFF can only place molecules into group A or B! %1")
                          .arg(group), CODELOC );

        return -1;
    }
}

/** Add the molecule 'molecule' to the group with index 'group' using the
    supplied map to find the forcefield parameters amongst the molecule's
    properties. Note that it is an error to try to add this to more than
    one group in the forcefield

    \throw SireBase::missing_property
    \throw SireMol::invalid_cast
    \throw SireFF::invalid_group
*/
bool InterGroupCoulombFF::addTo(const FFBase::Group &group,
                                const PartialMolecule &molecule,
                                const ParameterMap &map)
{
    //get the index for this group
    int group_idx = this->groupIndex(group);

    //get the molecule's ID
    MoleculeID molid = molecule.ID();

    this->assertValidGroup(group_idx, molid);

    ChangedCoulombMolecule new_molecule = changeRecord(molid);

    if (new_molecule.oldMolecule().isEmpty())
    {
        new_molecule = ChangedCoulombMolecule( CoulombMolecule(),
                                               CoulombMolecule(molecule,
                                                  map.source(parameters().coulomb())
                                                              )
                                             );

        //this is a freshly added molecule - add it to the current state
        this->addToCurrentState(group_idx, new_molecule.newMolecule());
        this->recordChange(group_idx, molid, new_molecule);

        this->incrementMajorVersion();

        return true;
    }
    else
    {
        new_molecule = new_molecule.add( molecule,
                                         map.source(parameters().coulomb()) );

        if (this->applyChange(molid, new_molecule))
        {
            this->incrementMajorVersion();
            return true;
        }
        else
            return this->isDirty();
    }
}

/** Add lots of molecules to the group 'group'

    \throw SireBase::missing_property
    \throw SireMol::invalid_cast
    \throw SireFF::invalid_group
*/
bool InterGroupCoulombFF::addTo(const FFBase::Group &group,
                                const Molecules &molecules,
                                const ParameterMap &map)
{
    return CoulombFF::addTo(group, molecules, map);
}

/** Add the molecule 'molecule' to group 'A' using the
    supplied map to find the forcefield parameters amongst the molecule's
    properties. Note that it is an error to try to add this to more than
    one group in the forcefield

    \throw SireBase::missing_property
    \throw SireMol::invalid_cast
    \throw SireFF::invalid_group
*/
bool InterGroupCoulombFF::addToA(const PartialMolecule &molecule,
                                 const ParameterMap &map)
{
    return this->addTo(groups().A(), molecule, map);
}

/** Add lots of molecules to group 'A'

    \throw SireBase::missing_property
    \throw SireMol::invalid_cast
    \throw SireFF::invalid_group
*/
bool InterGroupCoulombFF::addToA(const Molecules &molecules,
                                 const ParameterMap &map)
{
    return CoulombFF::addTo(groups().A(), molecules, map);
}

/** Add the molecule 'molecule' to group 'B' using the
    supplied map to find the forcefield parameters amongst the molecule's
    properties. Note that it is an error to try to add this to more than
    one group in the forcefield

    \throw SireBase::missing_property
    \throw SireMol::invalid_cast
    \throw SireFF::invalid_group
*/
bool InterGroupCoulombFF::addToB(const PartialMolecule &molecule,
                                 const ParameterMap &map)
{
    return this->addTo(groups().B(), molecule, map);
}

/** Add lots of molecules to group 'B'

    \throw SireBase::missing_property
    \throw SireMol::invalid_cast
    \throw SireFF::invalid_group
*/
bool InterGroupCoulombFF::addToB(const Molecules &molecules,
                                 const ParameterMap &map)
{
    return CoulombFF::addTo(groups().B(), molecules, map);
}

/** Return whether this forcefield contains a complete copy of
    any version of the partial molecule 'molecule' */
bool InterGroupCoulombFF::contains(const PartialMolecule &mol) const
{
    if ( this->refersTo(mol.ID()) )
        return this->molecule(mol.ID())
                        .selectedAtoms().contains(mol.selectedAtoms());
    else
        return false;
}

/** Return whether or not this forcefield contains a complete copy
    of any version of the partial molecule 'molecule' in the
    group 'group'

    \throw SireFF::missing_group
*/
bool InterGroupCoulombFF::contains(const PartialMolecule &mol,
                                   const FFBase::Group &group) const
{
    if ( this->refersTo(mol.ID(),group) )
        return this->molecule(mol.ID(),group)
                        .selectedAtoms().contains(mol.selectedAtoms());
    else
        return false;
}

/** Return whether or not this forcefield refers to the molecule
    with ID == molid */
bool InterGroupCoulombFF::refersTo(MoleculeID molid) const
{
    return molid_to_index[0].contains(molid) or
           molid_to_index[1].contains(molid);
}

/** Return whether or not the group 'group' refers to the molecule
    with ID == molid

    \throw SireFF::missing_group
*/
bool InterGroupCoulombFF::refersTo(MoleculeID molid,
                                   const FFBase::Group &group) const
{
    return molid_to_index[groupIndex(group)].contains(molid);
}

/** Return the groups referring to the molecule with ID == molid */
QSet<FFBase::Group> InterGroupCoulombFF::groupsReferringTo(MoleculeID molid) const
{
    QSet<FFBase::Group> molgroups;

    if (molid_to_index[0].contains(molid))
        molgroups.insert(this->groups().A());
    else if (molid_to_index[1].contains(molid))
        molgroups.insert(this->groups().B());

    return molgroups;
}

/** Return the IDs of all of the molecules contained in this forcefield */
QSet<MoleculeID> InterGroupCoulombFF::moleculeIDs() const
{
    return (molid_to_index[0].keys() + molid_to_index[1].keys()).toSet();
}

/** Return the IDs of all of the molecules in the group 'group'

    \throw SireFF::missing_group
*/
QSet<MoleculeID> InterGroupCoulombFF::moleculeIDs(const FFBase::Group &group) const
{
    return molid_to_index[groupIndex(group)].keys().toSet();
}

/** Return the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
PartialMolecule InterGroupCoulombFF::molecule(MoleculeID molid) const
{
    QHash<MoleculeID,uint>::const_iterator it = molid_to_index[0].find(molid);

    if (it != molid_to_index[0].end())
    {
        return mols[0].constData()[*it].molecule();
    }
    else
    {
        it = molid_to_index[1].find(molid);

        if (it != molid_to_index[1].end())
            return mols[1].constData()[*it].molecule();
    }

    throw SireMol::missing_molecule( QObject::tr(
            "There is no molecule with ID == %1 in the forcefield "
            "\"%2\" (%3 : %4).")
                .arg(molid).arg(this->name())
                .arg(this->ID()).arg(this->version().toString()),
                    CODELOC );

    return PartialMolecule();
}

/** Return the molecule with ID == molid from the group 'group'

    \throw SireMol::missing_molecule
    \throw SireFF::missing_group
*/
PartialMolecule InterGroupCoulombFF::molecule(MoleculeID molid,
                                              const FFBase::Group &group) const
{
    int idx = groupIndex(group);

    QHash<MoleculeID,uint>::const_iterator it = molid_to_index[idx].find(molid);

    if (it == molid_to_index[idx].end())
        throw SireMol::missing_molecule( QObject::tr(
            "There is no molecule with ID == %1 in the forcefield "
            "\"%2\" (%3 : %4).")
                .arg(molid).arg(this->name())
                .arg(this->ID()).arg(this->version().toString()),
                    CODELOC );

    return mols[idx].constData()[*it].molecule();
}

/** Return all of the molecules in this forcefield */
Molecules InterGroupCoulombFF::contents() const
{
    Molecules all_mols;

    QSet<MoleculeID> molids = this->moleculeIDs();

    if (not molids.isEmpty())
    {
        all_mols.reserve(molids.count());

        foreach (MoleculeID molid, molids)
        {
            all_mols.add( this->molecule(molid) );
        }
    }

    return all_mols;
}

/** Return all of the molecules in group 'group'

    \throw SireFF::missing_group
*/
Molecules InterGroupCoulombFF::contents(const FFBase::Group &group) const
{
    int idx = groupIndex(group);

    Molecules all_mols;

    int nmols = mols[idx].count();

    if (nmols > 0)
    {
        all_mols.reserve(nmols);

        const CoulombMolecule *mols_array = mols[idx].constData();

        for (int i=0; i<nmols; ++i)
        {
            const PartialMolecule &mol = mols_array[i].molecule();

            all_mols.add(mol);
        }
    }

    return all_mols;
}
