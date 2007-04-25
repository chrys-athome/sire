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

#include "moleculegroups.h"
#include "moleculeversion.h"
#include "partialmolecule.h"
#include "atomselector.h"

#include "SireError/errors.h"
#include "SireMol/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<MoleculeGroups> r_molgroups;

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const MoleculeGroups &molgroups)
{
    writeHeader(ds, r_molgroups, 1);

    SharedDataStream sds(ds);

    //just save each of the MoleculeGroups in the set
    //(prevents streaming lots of redundant information)
    sds << quint32(molgroups.count());

    for (QHash<MoleculeGroupID,MoleculeGroup>::const_iterator
                                      it = molgroups.molgroups.begin();
         it != molgroups.molgroups.end();
         ++it)
    {
        sds << *it;
    }

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       MoleculeGroups &molgroups)
{
    VersionID v = readHeader(ds, r_molgroups);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        quint32 ngroups;
        sds >> ngroups;

        if (ngroups == 0)
        {
            molgroups.clear();
        }
        else
        {
            //read in all of the groups
            QHash<MoleculeGroupID,MoleculeGroup> groups;
            groups.reserve(ngroups);

            for (uint i=0; i<ngroups; ++i)
            {
                MoleculeGroup group;

                sds >> group;

                groups.insert(group.ID(), group);
            }

            //now index them all
            molgroups.molgroups = groups;
            molgroups.reindex();
        }
    }
    else
        throw version_error(v, "1", r_molgroups, CODELOC);

    return ds;
}

/** Index all of the molecules in the passed group */
void MoleculeGroups::_pvt_index(const MoleculeGroup &group)
{
    MoleculeGroupID groupid = group.ID();

    QHash<MoleculeID,PartialMolecule> mols = group.molecules();

    for (QHash<MoleculeID,PartialMolecule>::const_iterator it = mols.constBegin();
         it != mols.constEnd();
         ++it)
    {
        index[it.key()].insert(groupid);
    }
}

/** Completely reindex the groups */
void MoleculeGroups::reindex()
{
    index.clear();

    for (QHash<MoleculeGroupID,MoleculeGroup>::const_iterator
                                                 it = molgroups.constBegin();
         it != molgroups.constEnd();
         ++it)
    {
        //index all of the molecules in this group
        this->_pvt_index(*it);
    }
}

/** Create an empty set of MoleculeGroups */
MoleculeGroups::MoleculeGroups()
{}

/** Create a set that only contains 'group' */
MoleculeGroups::MoleculeGroups(const MoleculeGroup &group)
{
    molgroups.insert(group.ID(), group);
    this->_pvt_index(group);
}

/** Copy constructor */
MoleculeGroups::MoleculeGroups(const MoleculeGroups &other)
               : molgroups(other.molgroups), index(other.index)
{}

/** Destructor */
MoleculeGroups::~MoleculeGroups()
{}

/** Assignment operator */
MoleculeGroups& MoleculeGroups::operator=(const MoleculeGroups &other)
{
    molgroups = other.molgroups;
    index = other.index;

    return *this;
}

/** Comparison operator */
bool MoleculeGroups::operator==(const MoleculeGroups &other) const
{
    return molgroups == other.molgroups;
}

/** Comparison operator */
bool MoleculeGroups::operator!=(const MoleculeGroups &other) const
{
    return molgroups != other.molgroups;
}

/** Return the set of IDs of groups that contain the molecule with ID == molid 

    \throw SireMol::missing_molecule
*/
QSet<MoleculeGroupID> MoleculeGroups::groupsReferringTo(MoleculeID molid) const
{
    QSet<MoleculeGroupID> groupids = index.value(molid);
    
    if (groupids.isEmpty())
        throw SireMol::missing_molecule( QObject::tr(
              "There is no molecule with ID == %1 in any of the MoleculeGroups!")
                    .arg(molid), CODELOC );

    return groupids;
}

/** Return the set of IDs of groups that contain at least tall of the atoms 
    of any version of the molecule 'molecule' */
QSet<MoleculeGroupID> 
MoleculeGroups::groupsContaining(const PartialMolecule &mol) const
{
    QSet<MoleculeGroupID> groupids = this->groupsReferringTo(mol.ID());
    
    QSet<MoleculeGroupID> groups_with_mol = groupids;
    
    foreach (MoleculeGroupID groupid, groupids)
    {
        if (not this->group(groupid).contains(mol))
            groups_with_mol.remove(groupid);
    }
    
    return groups_with_mol;
}

/** Return the set of ID numbers of all molecules in this set */
QSet<MoleculeID> MoleculeGroups::moleculeIDs() const
{
    return index.keys().toSet();
}

/** Completely clear this set - this removes all groups */
void MoleculeGroups::clear()
{
    molgroups.clear();
    index.clear();
}

/** Add the MoleculeGroup 'group'

    All of the molecules in this set will be synchronised
    to the versions in 'group'
*/
bool MoleculeGroups::add(const MoleculeGroup &newgroup)
{
    //maintain the invariant
    MoleculeGroups orig_groups(*this);

    try
    {
        if (molgroups.contains(newgroup.ID()))
        {
            //get rid of the existing copy
            this->remove(newgroup.ID());
        }

        //synchronise the other groups so that they have the 
        //same version of the molecules as 'newgroup'
        this->change(newgroup.molecules());

        //add the new group
        molgroups.insert( newgroup.ID(), newgroup );

        //index the group
        this->_pvt_index(newgroup);
    }
    catch(...)
    {
        *this = orig_groups;
        throw;
    }

    return true;
}

/** Add lots of groups */
bool MoleculeGroups::add(const MoleculeGroups &groups)
{
    if (molgroups.isEmpty())
    {
        //this is empty, so just copy 'groups'
        *this = groups;
        return true;
    }
    else
    {
        bool changed = false;
        MoleculeGroups orig_groups = *this;
      
        try
        {
            //add each group in turn
            foreach (MoleculeGroup group, groups.groups())
            {
                bool this_changed = this->add(group);
                changed = changed or this_changed;
            }
        }
        catch(...)
        {
            *this = orig_groups;
            throw;
        }
        
        return changed;
    }
}

/** Remove the group with ID == groupid */
bool MoleculeGroups::remove(MoleculeGroupID groupid)
{
    if (molgroups.contains(groupid))
    {
        //maintain the invariant
        MoleculeGroups orig_groups(*this);

        try
        {
            MoleculeGroup group = molgroups.take(groupid);

            QHash<MoleculeID,PartialMolecule> mols = group.molecules();

            for (QHash<MoleculeID,PartialMolecule>::const_iterator 
                                                        it = mols.constBegin();
                 it != mols.constEnd();
                 ++it)
            {
                MoleculeID molid = it.key();

                QSet<MoleculeGroupID> groups_containing_mol = index.value(molid);

                groups_containing_mol.remove(groupid);

                if (groups_containing_mol.isEmpty())
                    index.remove(molid);
                else
                    index[molid] = groups_containing_mol;
            }
        }
        catch(...)
        {
            //maintain the invariant
            *this = orig_groups;
            throw;
        }

        return true;
    }
    else
        return false;
}

/** Remove the group 'group' from this set. */
bool MoleculeGroups::remove(const MoleculeGroup &group)
{
    return this->remove(group.ID());
}

/** Remove lots of groups */
bool MoleculeGroups::remove(const MoleculeGroups &groups)
{
    bool changed = false;

    MoleculeGroups orig_groups = *this;
    
    try
    {
        foreach (MoleculeGroupID groupid, groups.groups().keys())
        {
            bool this_changed = this->remove(groupid);
            changed = changed or this_changed;
        }
    }
    catch(...)
    {
        *this = orig_groups;
        throw;
    }
    
    return changed;
}

/** Remove all groups from this set that are called 'groupname' */
bool MoleculeGroups::remove(const QString &groupname)
{
    QSet<MoleculeGroupID> groupids;

    for (QHash<MoleculeGroupID,MoleculeGroup>::const_iterator
                                                    it = molgroups.constBegin();
         it != molgroups.constEnd();
         ++it)
    {
        if (it->name() == groupname)
            groupids.insert(it.key());
    }

    if (groupids.isEmpty())
        return false;

    bool changed = false;

    MoleculeGroups orig_groups(*this);

    try
    {
        foreach (MoleculeGroupID groupid, groupids)
        {
            bool this_changed = this->remove(groupid);
            changed = changed or this_changed;
        }
    }
    catch(...)
    {
        *this = orig_groups;
        throw;
    }

    return changed;
}

/** Add the molecule 'molecule' to the group with ID == groupid.

    \throw SireMol::missing_group
*/
bool MoleculeGroups::add(const PartialMolecule &molecule,
                         MoleculeGroupID groupid)
{
    this->assertContains(groupid);

    MoleculeGroups orig_groups(*this);

    try
    {
        //change this molecule in all groups...
        this->change(molecule);
        
        if (molgroups[groupid].add(molecule))
        {
            index[molecule.ID()].insert(groupid);

            return true;
        }
   }
   catch(...)
   {
      *this = orig_groups;
      throw;
   }

   return false;
}

/** Add the molecules in 'molecules' to the group with ID == groupid.

    \throw SireMol::missing_group
*/
bool MoleculeGroups::add(const QList<PartialMolecule> &molecules,
                         MoleculeGroupID groupid)
{
    if (molecules.isEmpty())
        return false;
    else if (molecules.count() == 1)
        return this->add( molecules.first(), groupid );
    else
    {
        this->assertContains(groupid);

        MoleculeGroups orig_groups(*this);

        bool changed = false;

        try
        {
            //change these molecules in all groups...
            changed = this->change(molecules);
        
            for (QList<PartialMolecule>::const_iterator it = molecules.begin();
                 it != molecules.end();
                 ++it)
            {
                const PartialMolecule &molecule = *it;

                if (molgroups[groupid].add(molecule))
                {
                    index[molecule.ID()].insert(groupid);

                    changed = true;
                }
            }
        }
        catch(...)
        {
            *this = orig_groups;
            throw;
        }

        return changed;
    }
}

/** Remove the molecule 'molecule' from the group with ID == groupid */
bool MoleculeGroups::remove(const PartialMolecule &molecule,
                            MoleculeGroupID groupid)
{
    if (molgroups.contains(groupid))
    {
        //we contain this group...

        MoleculeGroups orig_groups(*this);

        try
        {
            if (molgroups[groupid].remove(molecule))
            {
                //the group contains this molecule, and it was removed
                // - has it been removed completely?
                if ( molgroups[groupid].refersTo(molecule.ID()) )
                {
                    //nope - it still contains some of the molecule
                    return true;
                }

                //now remove the molecule from the index of the group
                QSet<MoleculeGroupID> groups_containing_mol = index.value(molecule.ID());

                groups_containing_mol.remove(groupid);

                if (groups_containing_mol.isEmpty())
                    index.remove(molecule.ID());
                else
                    index[molecule.ID()] = groups_containing_mol;

                return true;
            }
        }
        catch(...)
        {
            *this = orig_groups;
            throw;
        }
    }

    return false;
}

/** Change the molecule 'molecule' */
bool MoleculeGroups::change(const PartialMolecule &molecule)
{
    QHash< MoleculeID, QSet<MoleculeGroupID> >::const_iterator
                                                  it = index.constFind(molecule.ID());

    if (it != index.constEnd())
    {
        bool changed = false;

        const QSet<MoleculeGroupID> &groupids = it.value();

        MoleculeGroups orig_groups(*this);

        try
        {
            foreach (MoleculeGroupID groupid, groupids)
            {
                bool this_changed = molgroups[groupid].change(molecule);
                changed = changed or this_changed;
            }
        }
        catch(...)
        {
            *this = orig_groups;
            throw;
        }

        return changed;
    }
    else
        return false;
}

/** Change a whole molecule group! */
bool MoleculeGroups::change(const MoleculeGroup &group)
{
    //do we contain this group?
    if ( not molgroups.contains(group.ID()) )
        return false;

    //do we already have the same version?
    Version new_version = group.version();

    Version old_version = molgroups.constFind(group.ID())->version();

    if (new_version == old_version)
        return false;
    else if (new_version.major() == old_version.major())
    {
        //they contain the same molecules, but with different
        //versions - therefore no need to reindex - just synchronise

        MoleculeGroups orig_groups(*this);

        try
        {
            molgroups[group.ID()] = group;
            this->change(group.molecules());
        }
        catch(...)
        {
            *this = orig_groups;
            throw;
        }

        return true;
    }
    else
    {
        //could have changed completely! - remove the group
        //then add it again from scratch (inefficient as this is!)
        MoleculeGroups orig_groups(*this);

        try
        {
            this->remove(group.ID());
            this->add(group);
        }
        catch(...)
        {
            *this = orig_groups;
            throw;
        }

        return true;
    }
}

/** Change lots of MoleculeGroups */
bool MoleculeGroups::change(const MoleculeGroups &newgroups)
{
    if (molgroups.keys() == newgroups.groups().keys())
    {
        //we have the same groups - just do a copy
        *this = newgroups;
        return true;
    }
    else
    {
        MoleculeGroups orig_groups = *this;
        
        bool changed = false;
        
        try
        {
            foreach (MoleculeGroup group, newgroups.groups())
            {
                bool this_changed = this->change(group);
                changed = changed or this_changed;
            }
        }
        catch(...)
        {
            *this = orig_groups;
            throw;
        }
        
        return changed;
    }
}

/** Change a whole load of molecules... */
bool MoleculeGroups::change(const QList<PartialMolecule> &molecules)
{
    if (molecules.isEmpty())
        return false;
    else if (molecules.count() == 1)
        return this->change( molecules.first() );
    else
    {
        //find all of the groups that don't contain any of these
        //molecules
        QSet<MoleculeGroupID> all_groupids = molgroups.keys().toSet();

        QSet<MoleculeGroupID> groupids = all_groupids;

        for (QList<PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            //get the groups that contain this molecule
            QHash< MoleculeID,QSet<MoleculeGroupID> >::const_iterator
                                      groups_with_mol = index.find(it->ID());

            //remove all of these group IDs
            foreach (MoleculeGroupID groupid, *(groups_with_mol))
            {
                groupids.remove(groupid);
            }

            if (groupids.isEmpty())
                //all of the groups contain at least one of the molecules
                //in molecules
                break;
        }

        //groupids now contains all of the MoleculeGroups that *don't* contain
        //any of 'molecules'. Now loop over all of the MoleculeGroups that
        //do at least contain one of the molecules and change them
        groupids = all_groupids.subtract(groupids);

        bool changed = false;

        //maintain the invariant
        MoleculeGroups orig_groups(*this);

        try
        {
            foreach (MoleculeGroupID groupid, groupids)
            {
                bool this_changed = molgroups[groupid].change(molecules);

                changed = changed or this_changed;
            }
        }
        catch(...)
        {
            //maintain the invariant
            *this = orig_groups;
            throw;
        }

        return changed;
    }
}

/** Change a whole load of molecules... */
bool MoleculeGroups::change(const QHash<MoleculeID,PartialMolecule> &molecules)
{
    if (molecules.isEmpty())
        return false;
    else if (molecules.count() == 1)
        return this->change( *(molecules.begin()) );
    else
    {
        //find all of the groups that don't contain any of these
        //molecules
        QSet<MoleculeGroupID> all_groupids = molgroups.keys().toSet();

        QSet<MoleculeGroupID> groupids = all_groupids;

        for (QHash<MoleculeID,PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            //get the groups that contain this molecule
            QHash< MoleculeID,QSet<MoleculeGroupID> >::const_iterator
                                       groups_with_mol = index.find(it.key());

            //remove all of these forcefields from ffids
            foreach (MoleculeGroupID groupid, *(groups_with_mol))
            {
                groupids.remove(groupid);
            }

            if (groupids.isEmpty())
                //all of the groups contain at least one
                //of the molecules in 'molecules'
                break;
        }

        //groupids now contains all of the MoleculeGroups that *don't* contain
        //any of 'molecules'. Now loop over all of the MoleculeGroups that
        //do at least contain one of the molecules and change them
        groupids = all_groupids.subtract(groupids);

        bool changed = false;

        //maintain the invariant
        MoleculeGroups orig_groups(*this);

        try
        {
            foreach (MoleculeGroupID groupid, groupids)
            {
                bool this_changed = molgroups[groupid].change(molecules);

                changed = changed or this_changed;
            }
        }
        catch(...)
        {
            //maintain the invariant
            *this = orig_groups;
            throw;
        }

        return changed;
    }
}

/** Remove the molecule 'molecule' */
bool MoleculeGroups::remove(const PartialMolecule &molecule)
{
    QHash< MoleculeID, QSet<MoleculeGroupID> >::const_iterator
                                                  it = index.constFind(molecule.ID());

    if (it != index.constEnd())
    {
        const QSet<MoleculeGroupID> &groupids = it.value();

        bool changed = false;

        MoleculeGroups orig_groups(*this);

        try
        {
            foreach (MoleculeGroupID groupid, groupids)
            {
                if (molgroups[groupid].remove(molecule))
                {
                    if ( not molgroups[groupid].refersTo(molecule.ID()) )
                        index[molecule.ID()].remove(groupid);

                    changed = true;
                }
            }

            if (changed and index[molecule.ID()].isEmpty())
                index.remove(molecule.ID());
        }
        catch(...)
        {
            *this = orig_groups;
            throw;
        }

        return changed;
    }
    else
        return false;
}

/** Remove a whole load of molecules */
bool MoleculeGroups::remove(const QList<PartialMolecule> &molecules)
{
    if (molecules.count() == 0)
        return false;
    else if (molecules.count() == 1)
        return this->remove(molecules.first());
    else
    {
        MoleculeGroups orig_groups(*this);

        bool changed = false;

        try
        {
            for (QList<PartialMolecule>::const_iterator it = molecules.begin();
                 it != molecules.end();
                 ++it)
            {
                const PartialMolecule &molecule = *it;

                QHash< MoleculeID, QSet<MoleculeGroupID> >::const_iterator
                                               it2 = index.constFind(molecule.ID());

                if (it2 != index.constEnd())
                {
                    const QSet<MoleculeGroupID> &groupids = it2.value();

                    MoleculeGroups orig_groups(*this);

                    foreach (MoleculeGroupID groupid, groupids)
                    {
                        if (molgroups[groupid].remove(molecule))
                        {
                            if ( not molgroups[groupid].refersTo(molecule.ID()) )
                                index[molecule.ID()].remove(groupid);

                            changed = true;
                        }
                    }

                    if (changed and index[molecule.ID()].isEmpty())
                        index.remove(molecule.ID());
                }
            }
        }
        catch(...)
        {
            *this = orig_groups;
            throw;
        }

        return changed;
    }
}

/** Remove lots of molecules from the group with ID == groupid 

    \throw SireMol::missing_group
*/
bool MoleculeGroups::remove(const QList<PartialMolecule> &molecules,
                            MoleculeGroupID groupid)
{
    this->assertContains(groupid);
                
    MoleculeGroup &group = molgroups[groupid];
    
    if (group.remove(molecules))
    {
        for (QList<PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            const PartialMolecule &molecule = *it;
        
            //the group contains this molecule, and it was removed
            // - has it been removed completely?
            if ( not molgroups[groupid].refersTo(molecule.ID()) )
            {
                //now remove the molecule from the index of the group
                QSet<MoleculeGroupID> groups_containing_mol = index.value(molecule.ID());

                groups_containing_mol.remove(groupid);

                if (groups_containing_mol.isEmpty())
                    index.remove(molecule.ID());
                else
                    index[molecule.ID()] = groups_containing_mol;
            }
        }
        
        return true;
    }
    else
        return false;
}

/** Return a copy of the molecule with ID == molid in the group 
    with ID == groupid
    
    \throw SireMol::missing_molecule
    \throw SireMol::missing_group
*/
PartialMolecule MoleculeGroups::molecule(MoleculeID molid,
                                         MoleculeGroupID groupid) const
{
    return this->group(groupid).molecule(molid);
}

/** Return the molecule with ID == molid from the groups whose
    IDs are in 'groupids'
    
    \throw SireMol::missing_molecule
    \throw SireMol::missing_group
    \throw SireError::invalid_arg
*/
PartialMolecule MoleculeGroups::molecule(MoleculeID molid,
                                         const QSet<MoleculeGroupID> &groupids) const
{
    if (groupids.isEmpty())
    {
        throw SireError::invalid_arg( QObject::tr(
            "You must request the molecule from at least one MoleculeGroup!"),
                  CODELOC );
    }
    
    if (groupids.count() == 1)
    {
        return this->molecule(molid, *(groupids.begin()));
    }
    else
    {
        QSet<MoleculeGroupID>::const_iterator it = groupids.begin();
        
        //get the first group's version of the molecule
        PartialMolecule mol = this->molecule(molid,*it);
        
        for ( ++it ; it != groupids.end(); ++it )
        {
            if (not mol.selectedAtoms().selectedAll())
                mol = mol.selection().add(this->molecule(molid,*it).selectedAtoms());
            else
                this->assertContains(*it);
        }
        
        return mol;
    }
}

/** Return the molecule with ID == molid from this group - this
    returns all atoms that are in any group
    
    \throw SireMol::missing_molecule
*/
PartialMolecule MoleculeGroups::molecule(MoleculeID molid) const
{
    QSet<MoleculeGroupID> groupids = this->groupsReferringTo(molid);
    
    return this->molecule(molid, groupids);
}

/** Return all of the molecules in the group */
QHash<MoleculeID,PartialMolecule> MoleculeGroups::molecules() const
{
    QHash<MoleculeID,PartialMolecule> allmols;
    allmols.reserve(index.count());

    for (QHash<MoleculeGroupID,MoleculeGroup>::const_iterator it = molgroups.begin();
         it != molgroups.end();
         ++it)
    {
        QHash<MoleculeID,PartialMolecule> groupmols = it->molecules();
        
        for (QHash<MoleculeID,PartialMolecule>::const_iterator 
                                                it2 = groupmols.constBegin();
             it2 != groupmols.constEnd();
             ++it2)
        {
            const PartialMolecule &groupmol = *it2;
            MoleculeID molid = it2.key();
            
            if (allmols.contains(molid))
            {
                PartialMolecule &mol = allmols[molid];
                
                if (not mol.selectedAtoms().selectedAll())
                    mol = mol.selection().add(groupmol.selectedAtoms());
            }
            else
            {
                allmols.insert(molid, groupmol);
            }
        }
    }
    
    return allmols;
}

/** Return an array of all of the groups in this set */
QHash<MoleculeGroupID,MoleculeGroup> MoleculeGroups::groups() const
{
    return molgroups;
}

/** Return all of the MoleculeGroups whose IDs are in 'groupids'

    \throw SireMol::missing_group
*/
QHash<MoleculeGroupID,MoleculeGroup>
MoleculeGroups::groups(const QSet<MoleculeGroupID> &groupids) const
{
    foreach (MoleculeGroupID groupid, groupids)
    {
        this->assertContains(groupid);
    }
    
    if (groupids.count() == molgroups.count())
        //selected all of the groups
        return molgroups;
    else
    {
        QHash<MoleculeGroupID,MoleculeGroup> groups = molgroups;
        
        foreach (MoleculeGroupID groupid, groups.keys())
        {
            if (not groupids.contains(groupid))
                groups.remove(groupid);
        }
        
        return groups;
    }
}

/** Return all of the MoleculeGroups that contain the molecule 'molecule' 

    \throw SireMol::missing_molecule
*/
QHash<MoleculeGroupID,MoleculeGroup> 
MoleculeGroups::groups(const PartialMolecule &molecule) const
{
    return this->groups( this->groupsContaining(molecule) );
}

/** Return all of the MoleculeGroups that refer to the molecule with
    ID == molid 
    
    \throw SireMol::missing_molecule
*/
QHash<MoleculeGroupID,MoleculeGroup> MoleculeGroups::groups(MoleculeID molid) const
{
    return this->groups( this->groupsReferringTo(molid) );
}

/** Return the MoleculeGroup with ID == groupid

    \throw SireMol::missing_group
*/
const MoleculeGroup& MoleculeGroups::group(MoleculeGroupID groupid) const
{
    QHash<MoleculeGroupID,MoleculeGroup>::const_iterator it = molgroups.find(groupid);

    if (it == molgroups.end())
        this->assertContains(groupid);

    return *it;
}

/** Assert that the group with ID == groupid exists in this set

    \throw SireMol::missing_group
*/
void MoleculeGroups::assertContains(MoleculeGroupID groupid) const
{
    if (not molgroups.contains(groupid))
        throw SireMol::missing_group( QObject::tr(
                  "There is no MoleculeGroup with ID == %1 in this set of "
                  "MoleculeGroups")
                      .arg(groupid), CODELOC );
}

/** Return the first MoleculeGroup that is called 'name'

    \throw SireMol::missing_group
*/
const MoleculeGroup& MoleculeGroups::group(const QString &name) const
{
    for (QHash<MoleculeGroupID,MoleculeGroup>::const_iterator it = molgroups.begin();
         it != molgroups.end();
         ++it)
    {
        if (it->name() == name)
            return *it;
    }

    throw SireMol::missing_group( QObject::tr(
              "There is no MoleculeGroup called \"%1\" in the set of MoleculeGroups")
                  .arg(name), CODELOC );

    return *(molgroups.begin());
}

/** Return whether or not the molecule with ID == molid is in one of the
    groups in this set. */
bool MoleculeGroups::refersTo(MoleculeID molid) const
{
    return index.contains(molid);
}

/** Return whether or not at least all of the atoms of any version of the 
    molecule 'mol' are contained in these groups */
bool MoleculeGroups::contains(const PartialMolecule &mol) const
{
    return this->molecule(mol.ID()).selectedAtoms().contains(mol.selectedAtoms());
}

/** Return the number of MoleculeGroups in this set */
int MoleculeGroups::count() const
{
    return molgroups.count();
}

/** Return the number of molecules in this set */
int MoleculeGroups::nMolecules() const
{
    return index.count();
}
