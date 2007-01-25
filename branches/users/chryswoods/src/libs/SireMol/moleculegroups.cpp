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

#include "SireMol/moleculeversion.h"

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
    
    int nmols = group.molecules().count();
    
    const Molecule *mol_array = group.molecules().constData();
    
    for (int i=0; i<nmols; ++i)
    {
        index[mol_array[i].ID()].insert(groupid);
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

/** Return the set of IDs of groups that contain the molecule with ID == molid */
QSet<MoleculeGroupID> MoleculeGroups::groupsContaining(MoleculeID molid) const
{
    return index.value(molid);
}

/** Return the set of ID numbers of all molecules in this set */
QSet<MoleculeID> MoleculeGroups::moleculeIDs() const
{
    return index.keys().toSet();
}

/** Synchronise the molecule with ID == molid so that each 
    group has a copy of the latest version */
void MoleculeGroups::synchronise(MoleculeID molid)
{
    QSet<MoleculeGroupID> groupids = this->groupsContaining(molid);
    
    if (groupids.isEmpty() or groupids.count() == 1)
        //there is either no copy, or just one copy of 
        //the molecule, so no need to synchronise
        return;
        
    //record the version number of each molecule in each of the containing
    //groups, and the groups that contain the molecule at that version

    QMap< MoleculeVersion, QSet<MoleculeGroupID> > molversions;

    foreach (MoleculeGroupID groupid, groupids)
    {
        const MoleculeGroup &group = *(molgroups.constFind(groupid));

        molversions[ group.molecule(molid).version() ].insert(groupid);
    }

    if (molversions.isEmpty() or molversions.count() == 1)
    {
        //there is either no copy of the molecule, or there
        //is already only a single version of the molecule
        return;
    }

    //there is some disagreement over the version number
    //The latest version is the last item in the map
    QMap< MoleculeVersion, QSet<MoleculeGroupID> >::const_iterator latest;

    latest = molversions.constEnd();
    --latest;

    //latest now points at the last item in the map
    MoleculeVersion latest_version = latest.key();
    const QSet<MoleculeGroupID> &latest_groupids = latest.value();

    //get a copy of the latest version of the molecule
    Molecule latest_mol = 
              molgroups.constFind(*(latest_groupids.begin()))->molecule(molid);

    //now loop over the forcefields that don't have this version and
    //update them
    molversions.remove(latest_version);

    QSet<MoleculeGroupID> groupids_needing_updating;

    for (QMap< MoleculeVersion, QSet<MoleculeGroupID> >::const_iterator
                                                  it = molversions.constBegin();
         it != molversions.constEnd();
         ++it)
    {
        groupids_needing_updating += it.value();
    }

    //change the molecule in the requested MoleculeGroups
    foreach (MoleculeGroupID groupid, groupids_needing_updating)
    {
        molgroups[groupid].change(latest_mol);
    }
}

/** Synchronise all molecules in the group with ID == groupid */
void MoleculeGroups::synchronise(MoleculeGroupID groupid)
{
    QSet<MoleculeID> molids = this->group(groupid).moleculeIDs();
    
    foreach (MoleculeID molid, molids)
    {
        this->synchronise(molid);
    }
}

/** Synchronise all of the molecules in the groups */
void MoleculeGroups::synchronise()
{
    QSet<MoleculeID> molids = this->moleculeIDs();
    
    foreach (MoleculeID molid, molids)
    {
        this->synchronise(molid);
    }
}

/** Completely clear this set - this removes all groups */
void MoleculeGroups::clear()
{
    molgroups.clear();
    index.clear();
}

/** Add the MoleculeGroup 'group' - this will only
    add the group if it doesn't already exist,
    or if the existing version has a lower
    version number.
    
    All of the molecules in this set will be synchronised 
    to their latest versions.
*/
bool MoleculeGroups::add(const MoleculeGroup &newgroup)
{
    if (molgroups.contains(newgroup.ID()))
    {
        if (this->group(newgroup.ID()).version() >= newgroup.version())
            //there is already a copy in this set, and its newer!
            return false;
            
        //get rid of the existing copy
        this->remove(newgroup.ID());
    }
    
    //add the new group
    molgroups.insert( newgroup.ID(), newgroup );
    
    //index the group
    this->_pvt_index(newgroup);
    
    //now synchronise this group...
    this->synchronise(newgroup.ID());
    
    return true;
}

/** Remove the group with ID == groupid */
bool MoleculeGroups::remove(MoleculeGroupID groupid)
{
    if (molgroups.contains(groupid))
    {
  
        MoleculeGroup group = molgroups.take(groupid);
    
        int nmols = group.molecules().count();
        const Molecule *group_array = group.molecules().constData();
    
        for (int i=0; i<nmols; ++i)
        {
            MoleculeID molid = group_array[i].ID();
    
            QSet<MoleculeGroupID> groups_containing_mol = index.value(molid);
          
            groups_containing_mol.remove(groupid);
        
            if (groups_containing_mol.isEmpty())
                index.remove(molid);
            else
                index[molid] = groups_containing_mol;
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
    
    bool changed = false;
    
    foreach (MoleculeGroupID groupid, groupids)
    {
        bool this_changed = this->remove(groupid);
        
        changed = changed or this_changed;
    }
    
    return changed;
}

/** Add the molecule 'molecule' to the group with ID == groupid. This
    throws an exception if this molecule already exists in this group.
    
    \throw SireMol::missing_group
    \throw SireMol::duplicate_molecule
*/
bool MoleculeGroups::add(const Molecule &molecule, MoleculeGroupID groupid)
{
    this->assertContains(groupid);
    
    if (molgroups[groupid].add(molecule))
    {
        index[molecule.ID()].insert(groupid);
        
        //synchronise this molecule across all of the groups
        this->synchronise(molecule.ID());
        
        return true;
    }
    else
        return false;
}

/** Remove the molecule 'molecule' from the group with ID == groupid */
bool MoleculeGroups::remove(const Molecule &molecule, MoleculeGroupID groupid)
{
    if (molgroups.contains(groupid))
    {
        //we contain this group...
    
        if (molgroups[groupid].remove(molecule))
        {
            //the group contains this molecule, and it was removed
            
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
    
    return false;
}

/** Change the molecule 'molecule' */
bool MoleculeGroups::change(const Molecule &molecule)
{
    QHash< MoleculeID, QSet<MoleculeGroupID> >::const_iterator 
                                                  it = index.constFind(molecule.ID());
                                                  
    if (it != index.constEnd())
    {
        bool changed = false;
    
        const QSet<MoleculeGroupID> &groupids = it.value();
        
        foreach (MoleculeGroupID groupid, groupids)
        {
            bool this_changed = molgroups[groupid].change(molecule);
            
            changed = changed or this_changed;
        }
        
        return changed;
    }
    else
        return false;
}

/** Change a whole load of molecules... */
bool MoleculeGroups::change(const QHash<MoleculeID,Molecule> &molecules)
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
        
        for (QHash<MoleculeID,Molecule>::const_iterator it = molecules.begin();
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
        
        foreach (MoleculeGroupID groupid, groupids)
        {
            bool this_changed = molgroups[groupid].change(molecules);
            
            changed = changed or this_changed;
        }
        
        return changed;
    }
}

/** Remove the molecule 'molecule' */
bool MoleculeGroups::remove(const Molecule &molecule)
{
    QHash< MoleculeID, QSet<MoleculeGroupID> >::const_iterator 
                                                  it = index.constFind(molecule.ID());
                                                  
    if (it != index.constEnd())
    {
        const QSet<MoleculeGroupID> &groupids = it.value();
        
        foreach (MoleculeGroupID groupid, groupids)
        {
            molgroups[groupid].remove(molecule);
        }
        
        index.remove(molecule.ID());
        
        return true;
    }
    else
        return false;
}

/** Return an array of all of the molecules in the group */
QVector<Molecule> MoleculeGroups::molecules() const
{
    QVector<Molecule> allmols;
    allmols.resize(index.count());
    
    Molecule *mols_array = allmols.data();

    int i = 0;

    for (QHash< MoleculeID, QSet<MoleculeGroupID> >::const_iterator it = index.begin();
         it != index.end();
         ++it)
    {
        //just get the first available copy of the molecule
        mols_array[i] = molgroups[ *(it->begin()) ].molecule(it.key());
        
        ++i;
    }
    
    return allmols;
}

/** Return an array of all of the groups in this set */
QVector<MoleculeGroup> MoleculeGroups::groups() const
{
    if ( molgroups.count() == 0 )
        return QVector<MoleculeGroup>();
    else
    {
        QVector<MoleculeGroup> allgroups;
        allgroups.resize(molgroups.count());
        
        MoleculeGroup *group_array = allgroups.data();
    
        int i = 0;
    
        for (QHash<MoleculeGroupID,MoleculeGroup>::const_iterator 
                                                    it = molgroups.begin();
             it != molgroups.end();
             ++it)
        {
            group_array[i] = *it;
            
            ++i;
        }
        
        return allgroups;
    }
}

/** Return all of the MoleculeGroups that contain the molecule with 
    ID == molid */
QVector<MoleculeGroup> MoleculeGroups::groups(MoleculeID molid) const
{
    QHash< MoleculeID, QSet<MoleculeGroupID> >::const_iterator it = index.find(molid);
    
    if (it != index.end())
    {
        const QSet<MoleculeGroupID> &groupids = *it;
        
        QVector<MoleculeGroup> groups_containing_mol;
        groups_containing_mol.resize(groupids.count());
        
        MoleculeGroup *groups_array = groups_containing_mol.data();
        
        int i = 0;
        
        foreach (MoleculeGroupID groupid, groupids)
        {
            groups_array[i] = molgroups[groupid];
            ++i;
        }
        
        return groups_containing_mol;
    }
    else
        return QVector<MoleculeGroup>();
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
bool MoleculeGroups::contains(MoleculeID molid) const
{
    return index.contains(molid);
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
