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

#include "molgroups.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

/////////////
///////////// Implementation of MolGroupsBase
/////////////


/////////////
///////////// Implementation of MolGroups
/////////////

RegisterMetaType<MolGroups> r_molgroups;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, 
                                       const MolGroups &molgroups)
{
    writeHeader(ds, r_molgroups, 1);
    
    SharedDataStream sds(ds);
    sds << molgroups.mgroups 
        << static_cast<const MolGroupsBase&>(molgroups);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MolGroups &molgroups)
{
    VersionID v = readHeader(ds, r_molgroups);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> molgroups.mgroups
            >> static_cast<MolGroupsBase&>(molgroups);
    }
    else
        throw version_error(r_molgroups, "1", CODELOC);
        
    return ds;
}

/** Construct an empty set of groups */
MolGroups::MolGroups()
          : ConcreteProperty<MolGroups,MolGroupsBase>()
{}

/** Construct a set of groups that contains only the single group
    'molgroup' */
MolGroups::MolGroups(const MolGroup &molgroup)
          : ConcreteProperty<MolGroups,MolGroupsBase>()
{
    this->add(molgroup);
}

/** Construct a set of groups that contains the groups in 'molgroups'.
    The groups will be added in the order that they appear in this list. */
MolGroups::MolGroups(const QList<MoleculeGroup> &molgroups)
          : ConcreteProperty<MolGroups,MolGroupsBase>()
{
    foreach (const MoleculeGroup &molgroup, molgroups)
    {
        this->add(molgroup);
    }
}

/** Copy constructor */
MolGroups::MolGroups(const MolGroups &other)
          : ConcreteProperty<MolGroups,MolGroupsBase>(other),
            mgroups(other.mgroups)
{}

/** Destructor */
MolGroups::~MolGroups()
{}

/** Copy assignment operator */
MolGroups& MolGroups::operator=(const MolGroups &other)
{
    if (this != &other)
    {
        mgroups = other.mgroups;
        MolGroupsBase::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool MolGroups::operator==(const MolGroups &other) const
{
    return this == &other or mgroups == other.mgroups;
}

/** Comparison operator */
bool MolGroups::operator!=(const MolGroups &other) const
{
    return this != &other and mgroups != other.mgroups;
}

/** Return the molecule group with number 'mgnum'

    \throw SireMol::missing_group
*/
const MolGroup& MolGroups::at(MGNum mgnum) const
{
    QHash<MGNum,MoleculeGroup>::const_iterator it = mgroups.find(mgnum);
    
    if (it == mgroups.end())
        throw SireMol::missing_group( QObject::tr(
            "There is no molecule group with number %1 in this set. "
            "The available groups are [ %2 ].")
                .arg(mgnum)
                .arg( Sire::toString(this->mgNums()) ), CODELOC );
                
    return it->base();
}

/** Add the molecule group 'molgroup' to this set. This does
    nothing if this group is already in this set. This updates
    the molecules in 'molgroup' so that they are at the 
    same version as any existing copies of the molecules
    in this set. */
void MolGroups::add(const MolGroup &molgroup)
{
    if (this->contains(molgroup.number()))
        return;

    //copy this group
    MoleculeGroup new_group(molgroup);
    
    //update the group so that it has the same version of the 
    //molecules as the other groups in this set
    for (QHash<MGNum,MoleculeGroup>::const_iterator it = mgroups.constBegin();
         it != mgroups.constEnd();
         ++it)
    {
        new_group.update(it->molecules());
    }
    
    //add the group to the index
    this->addToIndex(new_group.base());
    
    mgroups.insert(new_group.number(), new_group);
}

/** Update the group 'molgroup'. If this group is in this set,
    then it updates the group to the same version. Then, regardless
    of whether the group is in this set, it then updates all
    molecules in all of the groups so that they have the same
    version number as 'molgroup'. This does nothing if
    molgroup and none of its molecules are in this set */
void MolGroups::update(const MolGroup &molgroup)
{
    QHash<MGNum,MoleculeGroup>::iterator it = mgroups.find(molgroup.number());
    
    if (it != mgroups.end() and 
        molgroup.majorVersion() != it->majorVersion())
    {
        //this group exists in this set and it is at a different
        //major version - this means that its complement of
        //molecules has changed, so there needs to be a change 
        //in the index...
        
        //get the list of current molecules in this group
        QSet<MolNum> old_molnums = it->molNums();
        
        //now the new set of numbers...
        QSet<MolNum> new_molnums = molgroup.molNums();
        
        this->addToIndex(molgroup.number(), new_molnums - old_molnums);
        this->removeFromIndex(molgroup.number(), old_molnums - new_molnums);
    }
    
    //update all of the groups;
    for (it = mgroups.begin() ; it != mgroups.end(); ++it)
    {
        it->update(molgroup);
    }
}

/** Completely remove the group with number 'mgnum'. This 
    does nothing if there is no such group in this set */
void MolGroups::remove(MGNum mgnum)
{
    if (mgroups.contains(mgnum))
    {
        mgroups.remove(mgnum);
        this->removeFromIndex(mgnum);
    }
}

/** Remove the group 'molgroup' from this set. This does nothing
    if this group doesn't exist in this set */
void MolGroups::remove(const MolGroup &molgroup)
{
    this->remove(molgroup.number());
}

/** Remove the groups that match the ID 'mgid' from this set. This
    does nothing if there are no such groups. */
void MolGroups::remove(const MGID &mgid)
{
    try
    {
        QSet<MGNum> mgnums = this->map(mgid);
    
        foreach (MGNum mgnum, mgnums)
        {
            this->remove(mgnum);
        }
    }
    catch(...)
    {}
}

/** Add the view of the molecule in 'molview' to the groups
    identified by 'mgid'. This adds the view as a duplicate
    if it already exists in the group. The version
    of the molecule added is the version already present
    in this set, if it exists.
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::add(const MoleculeView &molview, const MGID &mgid)
{
    //get the number of the groups identified by 'mgid'
    QList<MGNum> mgnums = this->map(mgid);

    PartialMolecule view(molview);
    view.update( this->matchToExistingData(view.data()) );
    
    foreach (MGNum mgnum, mgnums)
    {
        mgroups.find(mgnum)->add(view);
        this->addToIndex(mgnum, view.number());
    }
}

/** Add the views of the molecule in 'molviews' to the groups
    identified by 'mgid'. This adds the view as a duplicate if
    it already exists in a group.
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::add(const ViewsOfMol &molviews, const MGID &mgid)
{
    QList<MGNum> mgnums = this->map(mgid);

    ViewsOfMol views(molviews);
    views.update( this->matchToExistingVersion(views.data()) );
    
    foreach (MGNum mgnum, mgnums)
    {
        mgroups.find(mgnum)->add(views);
        this->addToIndex(mgnum, views.number());
    }
}

/** Add each of the molecules in 'molecules' to the groups
    identified by 'mgid'. This adds the views as duplicates
    if they exist already in a group. Any molecules that
    already exist in any of the groups in this set are
    updated to the versions that are already present
    in this set.
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::add(const Molecules &molecules, const MGID &mgid)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    Molecules mols = this->matchToExistingVersion(molecules);
    QSet<MolNum> molnums = mols.molNums();
    
    foreach (MGNum mgnum, mgnums)
    {
        mgroups.find(mgnum)->add(mols);
        this->addToIndex(mgnum, molnums);
    }
}

/** Add the molecules in the group 'molgroup' to the groups
    identified by 'mgid'. This adds the views as duplicates 
    if they already exist, and adds the views in the same
    order as they appear in 'molgroup'. This is slightly less
    efficient than MolGroups::add(const Molecules&), so use
    that function if you don't care about the order.
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::add(const MolGroup &molgroup, const MGID &mgid)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    //update the group to match the molecule versions
    //already present in this group...
    MoleculeGroup group(molgroup);
    group.update( this->matchToExistingVersions(group.molecules()) );
    
    QSet<MolNum> molnums = group.molNums();
    
    foreach (MGNum mgnum, mgnums)
    {
        mgroups.find(mgnum)->add(group);
        this->addToIndex(mgnum,molnums);
    }
}

/** Add the view of the molecule in 'molview' to the groups
    identified by 'mgid'. This only adds the view to a group
    if it doesn't already exist in the group. The version
    of the molecule already present in this set is used if 
    such a molecule already exists.
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::addIfUnique(const MoleculeView &molview, const MGID &mgid)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    PartialMolecule view(molview);
    view.update( this->matchToExistingVersion(view.data()) );
    
    foreach (MGNum mgnum, mgnums)
    {
        if ( mgroups.find(mgnum)->addIfUnique(view) )
            this->addToIndex(mgnum,view.number());
    }
}

/** Add the views of the molecule in 'molviews' ot the groups
    identified by 'mgid'. This only adds views to groups that
    don't already exist in that group, and uses the existing
    version of the molecule is it already exists in one 
    of the groups of this set.
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::addIfUnique(const ViewsOfMol &molviews, const MGID &mgid)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    ViewsOfMol views(molviews);
    views.update( this->matchToExistingVersion(views.data()) );
    
    foreach (MGNum mgnum, mgnums)
    {
        ViewsOfMol added_views = groups.find(mgnum)->addIfUnique(views);
        
        if (not added_views.isEmpty())
            this->addToIndex(mgnum,views.number());
    }
}

/** Add all of the views of the molecules in 'molecules' to the groups
    identified by 'mgid'. This only adds views that don't already 
    exist in the group, and uses the version of the molecules that already
    exists in one of the groups of this set (if one exists)
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::addIfUnique(const Molecules &molecules, const MGID &mgid)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    Molecules mols = this->matchExistingVersion(molecules);
    QSet<MolNum> molnums = mols.molNums();
    
    foreach (MGNum mgnum, mgnums)
    {
        QList<ViewsOfMol> added_mols = mgroups.find(mgnum)->addIfUnique(mols);
        
        if (not added_mols.isEmpty())
        {
            QSet<MolNum> added_molnums;
            
            foreach (const ViewsOfMol &mol, added_mols)
            {
                added_molnums.insert(mol.number());
            }
        
            this->addToIndex(mgnum,added_molnums);
        }
    }
}

/** This adds all of the views of the molecules in the group
    'molgroup', in the same order as they exist in this group,
    to all of the groups identified by 'mgid'. This only
    adds views to a group that don't already exist in that
    group and uses the existing version of the molecule if 
    it exists anywhere in this set. 
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::addIfUnique(const MolGroup &molgroup, const MGID &mgid)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    //update the group...
    MolGroup group(molgroup);
    group.update( this->matchExistingVersion(group.molecules()) );
    
    //now add the molecules...
    foreach (MGNum mgnum, mgnums)
    {
        QList<ViewsOfMol> added_mols = mgroups.find(mgnum)->addIfUnique(group);
        
        if (not added_mols.isEmpty())
        {
            QSet<MolNum> added_molnums;
            
            foreach (const ViewsOfMol &mol, added_mols)
            {
                added_molnums.insert(mol.number());
            }
            
            this->addToIndex(mgnum,added_molnums);
        }
    }
}

/** Remove the view of the molecule in 'molview' from the groups
    identified by 'mgid'. This only removes the first copy
    of the view from each group, if multiple copies exist.
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::remove(const MoleculeView &molview, const MGID &mgid)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    MolNum molnum = molview.data().number();
    
    foreach (MGNum mgnum, mgnums)
    {
        MoleculeGroup &mgroup = *(mgroups.find(mgnum));
        
        if (mgroup.remove(molview))
        {
            if (not mgroup.contains(molnum))
                this->removeFromIndex(mgnum,molnum);
        }
    }
}

/** Remove the views of the molecule in 'molviews' from the groups
    identified by 'mgid'. This only removes the first copy of the
    views from each group if they exist multiple times.
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::remove(const ViewsOfMol &molviews, const MGID &mgid)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    MolNum molnum = molviews.number();
    
    foreach (MGNum mgnum, mgnums)
    {
        MoleculeGroup &mgroup = *(mgroups.find(mgnum));
        
        ViewsOfMol removed_views = mgroup.remove(molviews);
        
        if (not removed_views.isEmpty())
        {
            if (not mgroup.contains(molnum))
                this->removeFromIndex(mgnum,molnum);
        }
    }
}

/** Remove all of the views of the molecules in 'molecules' from 
    the groups identified by 'mgid'. This removes only the first
    copies of the views in each group.
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::remove(const Molecules &molecules, const MGID &mgid)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    foreach (MGNum mgnum, mgnums)
    {
        MoleculeGroup &mgroup = *(mgroups.find(mgnum));
    
        QList<ViewsOfMol> removed_mols = mgroup.remove(molecules);

        QSet<MolNum> removed_molnums;
        
        foreach (const ViewsOfMol &removed_mol, removed_mols)
        {
            if (not mgroup.contains(removed_mol.number()))
                removed_molnums.insert(removed_mol.number());
        }
        
        if (not removed_molnums.isEmpty())
            this->removeFromIndex(mgnum, removed_molnums);
    }
}

/** Remove all of the views of the molecules in the group 'molgroup' from 
    the groups identified by 'mgid'. This removes only the first
    copies of the views in each group.
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::remove(const MolGroup &molgroup, const MGID &mgid)
{
    this->remove(molgroup.molecules(), mgid);
}

/** Remove all copies of the view of the molecule in 'molview' from
    the groups identified by 'mgid'
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::removeAll(const MoleculeView &molview, const MGID &mgid);
void MolGroups::removeAll(const ViewsOfMol &molviews, const MGID &mgid);
void MolGroups::removeAll(const Molecules &molecules, const MGID &mgid);
void MolGroups::removeAll(const MolGroup &molgroup, const MGID &mgid);

void MolGroups::remove(MolNum molnum, const MGID &mgid);
void MolGroups::remove(const QSet<MolNum> &molnums, const MGID &mgid);

void MolGroups::removeAll(const MGID &mgid);

void MolGroups::update(const MoleculeData &moldata);

void MolGroups::update(const Molecules &molecules);
void MolGroups::update(const MolGroup &molgroup);

void MolGroups::setContents(const MGID &mgid, const MoleculeView &molview);
void MolGroups::setContents(const MGID &mgid, const ViewsOfMol &molviews);
void MolGroups::setContents(const MGID &mgid, const Molecules &molecules);
void MolGroups::setContents(const MGID &mgid, const MolGroup &molgroup);

MolGroup& MolGroups::getGroup(MGNum mgnum);

void MolGroups::getGroups(const QList<MGNum> &mgnums,
                          QVarLengthArray<MolGroup*,10> &groups);

QHash<MGNum,MolGroup*> MolGroups::getGroups();
QHash<MGNum,const MolGroup*> MolGroups::getGroups() const;
