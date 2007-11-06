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
#include "molecules.h"
#include "mgidx.h"
#include "mgnum.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

RegisterMetaType<MoleculeGroups> r_molgroups;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, 
                                       const MoleculeGroups &molgroups)
{
    writeHeader(ds, r_molgroups, 1);

    SharedDataStream sds(ds);
    
    sds << molgroups.groups_by_idx 
        << molgroups.groups_by_num
        << molgroups.groups_by_mol;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, 
                                       MoleculeGroups &molgroups)
{
    VersionID v = readHeader(ds, r_molgroups);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> molgroups.groups_by_idx
            >> molgroups.groups_by_num
            >> molgroups.groups_by_mol;
    }
    else
        throw version_error(v, "1", r_molgroups, CODELOC);

    return ds;
}

/** Construct an empty set of groups */
MoleculeGroups::MoleculeGroups()
{}

/** Construct a set that holds a single group */
MoleculeGroups::MoleculeGroups(const MoleculeGroup &group)
{
    this->_pvt_add(group);
}

/** Copy constructor */
MoleculeGroups::MoleculeGroups(const MoleculeGroups &other)
               : groups_by_idx(other.groups_by_idx),
                 groups_by_num(other.groups_by_num),
                 groups_by_mol(other.groups_by_mol)
{}

/** Destructor */
MoleculeGroups::~MoleculeGroups()
{}

/** Copy assignment operator */
MoleculeGroups& MoleculeGroups::operator=(const MoleculeGroups &other)
{
    if (this != &other)
    {
        groups_by_idx = other.groups_by_idx;
        groups_by_num = other.groups_by_num;
        groups_by_mol = other.groups_by_mol;
    }
    
    return *this;
}

/** Copy from a single group */
MoleculeGroups& MoleculeGroups::operator=(const MoleculeGroup &other)
{
    return this->operator=(MoleculeGroups(other));
}

/** Comparison operator */
bool MoleculeGroups::operator==(const MoleculeGroups &other) const
{
    return groups_by_idx == other.groups_by_idx;
}

/** Comparison operator */
bool MoleculeGroups::operator!=(const MoleculeGroups &other) const
{
    return groups_by_idx != other.groups_by_idx;
}

void MoleculeGroups::_pvt_addToIndex(int idx)
{
    const MoleculeGroup &group = groups_by_idx.at(idx);
    
    for (Molecules::const_iterator it = group.molecules().begin();
         it != group.molecules().end();
         ++it)
    {
        QHash< MolNum, QSet<MGNum> >::iterator 
                                it2 = groups_by_mol.find(it->number());
    
        if (it2 != groups_by_mol.end())
        {
            it2->insert(group.number());
        }
        else
        {
            QSet<MGNum> mgnums;
            mgnums.insert(group.number());
            groups_by_mol.insert( it->number(), mgnums );
        }
    }
}

void MoleculeGroups::_pvt_removeFromIndex(MGNum mgnum)
{
    QMutableHashIterator< MolNum, QSet<MGNum> > it(groups_by_mol);
    
    while (it.hasNext())
    {
        it.next();
        
        if (it.value().contains(mgnum))
        {
            it.value().remove(mgnum);

            if (it.value().isEmpty())
                it.remove();
        }
    }
}

void MoleculeGroups::_pvt_update(const Molecules &molecules,
                                 QSet<MGNum> &already_updated)
{
    if (groups_by_idx.count() == 0 or molecules.isEmpty())
    {
        return;
    }
    else if (groups_by_idx.count() == 1 and already_updated.isEmpty())
    {
        groups_by_idx[0].update(molecules);
        return;
    }
    
    for (Molecules::const_iterator it = molecules.begin();
         it != molecules.end();
         ++it)
    {
        QSet<MGNum> mgnums = groups_by_mol.value(it->number());
        
        foreach (MGNum mgnum, mgnums)
        {
            if (not already_updated.contains(mgnum))
            {
                MoleculeGroup &group = groups_by_idx[groups_by_num.value(mgnum)];
                
                group.update(molecules);
                
                if (already_updated.count() == groups_by_idx.count() - 1)
                    //we have updated all of the groups
                    return;
                else
                    already_updated.insert(mgnum);
            }
        }
    }
}

void MoleculeGroups::_pvt_update(const MoleculeGroup &group)
{
    int idx = groups_by_num.value(group.number(), -1);
    
    if (idx != -1)
    {
        QSet<MGNum> already_updated;
        already_updated.reserve( nGroups() - 1 );
    
        this->_pvt_update(group.molecules(), already_updated);
        return;
    }
        
    //if the major version number of the group has changed
    //then molecules/views have been added or removed
    //from the group - it will need to be reindexed
    MoleculeGroup &oldgroup = groups_by_idx[idx];
    
    if (oldgroup.majorVersion() != group.majorVersion())
    {
        this->_pvt_removeFromIndex(group.number());

        oldgroup = group;
        this->_pvt_addToIndex(idx);
    }
    else
    {
        //the major number isn't different, so the same molecules
        //are in the group, so there is no need to reindex
        oldgroup = group;
    }
    
    if (groups_by_idx.count() == 1 or group.isEmpty())
        //nothing left to do
        return;
    
    //we have already updated this group
    QSet<MGNum> already_updated;
    already_updated.reserve( nGroups() - 1 );
    already_updated.insert(group.number());
    
    //now update all of the molecules in this set so that
    //they are the same version as in 'group'
    this->_pvt_update(group.molecules(), already_updated);
}

void MoleculeGroups::_pvt_update(const MoleculeGroups &groups)
{
    if (groups.nGroups() == 0 or this->nGroups() == 0)
        return;
        
    QSet<MGNum> already_updated;
    already_updated.reserve(nGroups());
    
    for (MoleculeGroups::const_iterator it = groups.begin();
         it != groups.end();
         ++it)
    {
        int idx = groups_by_num.value(it->number(), -1);
        
        if (idx != -1)
        {
            MoleculeGroup &group = groups_by_idx[idx];
            group = *it;
            
            already_updated.insert(group.number());
        }
    }
    
    if (already_updated.count() < nGroups())
    {
        //some of the groups haven't been updated - we will now
        //have to update the molecules in those groups so that
        //they match the version
        for (MoleculeGroups::const_iterator it = groups.begin();
             it != groups.end();
             ++it)
        {
            this->_pvt_update(it->molecules(), already_updated);
            
            if (already_updated.count() == nGroups())
                return;
        }
    }
}

void MoleculeGroups::_pvt_add(const MoleculeGroup &group)
{
    if (groups_by_num.contains(group.number()))
    {
        this->_pvt_update(group);
    }
    else
    {
        groups_by_idx.append(group);
        int idx = groups_by_idx.count() - 1;
        
        groups_by_num.insert( group.number(), idx );
        
        this->_pvt_addToIndex(idx);

        if (nGroups() > 1)
        {
            QSet<MGNum> already_updated;
            already_updated.reserve(nGroups() - 1);
            already_updated.insert(group.number());
            this->_pvt_update(group.molecules(), already_updated);
        }
    }
}

/** Add the group 'group' to this set. Note that this will update 'group' 
    if it already exists in this set, and it will also update all of 
    the molecules in this set so that they have the same version
    in all of the groups as 'group' */
void MoleculeGroups::add(const MoleculeGroup &group)
{
    this->_pvt_add(group);
}

/** Add the groups 'groups' to this set.
    If any of the groups already exist in this
    set, then they will be updated rather than added. */
void MoleculeGroups::add(const MoleculeGroups &groups)
{
    if (groups.nGroups() == 0)
    {
        return;
    }
    else if (groups.nGroups() == 1)
    {
        this->add( groups.at(MGIdx(0)) );
    }
    else
    {
        for (MoleculeGroups::const_iterator it = groups.constBegin();
             it != groups.constEnd();
             ++it)
        {
            this->_pvt_add(*it);
        }
    }
}

void MoleculeGroups::_pvt_add(const MoleculeView &molview,
                              MGNum mgnum)
{
    MoleculeGroup &group = groups_by_index[ groups_by_num.value(mgnum) ];
    group = group.add(molview);
    
    groups_by_mol[molview.data().number()].insert(group.number());
}

/** Add the the molecule view 'molview' to the group(s) identified by 
    'mgid'. If this view already exists in the group(s) then it 
    will be duplicated. Also, all groups holding a view of this molecule
    will be updated to the version of the molecule in 'molview'
    
    \throw SireMol::missing_molgroup
*/
void MoleculeGroups::add(const MoleculeView &molview, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    if (mgnums.count() == 1)
    {
        this->_pvt_add(molview, mgnums.first());
    }
    else
    {
        MoleculeGroups saved_state(*this);
        
        try
        {
            foreach (MGNum mgnum, mgnums)
            {
                ret._pvt_add(molview, mgnum);
            }
        }
        catch(...)
        {
            //restore the previous state
            this->operator=(saved_state);
            throw;
        }
    }
}

void MoleculeGroups::_pvt_add(const ViewsOfMol &molviews,
                              MGNum mgnum)
{
    MoleculeGroup &group = groups_by_index[ groups_by_num.value(mgnum) ];
    group = group.add(molviews);
    
    groups_by_mol[molviews.number()].insert(group.number());
}
                 
/** Add the views of the molecule in 'molviews'
    to the group(s) identified by 'mgid'. This adds
    duplicates of any views that are already in these group(s)
    
    \throw SireMol::missing_molgroup
*/
void MoleculeGroups::add(const ViewsOfMol &molviews, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    if (mgnums.count() == 1)
    {
        this->_pvt_add(molviews, mgnums.first());
    }
    else
    {
        MoleculeGroups saved_state(*this);
    
        try
        {
            foreach (MGNum mgnum, mgnums)
            {
                this->_pvt_add(molviews, mgnum);
            }
        }
        catch(...)
        {
            this->operator=(saved_state);
            throw;
        }
    }
}
               
void MoleculeGroups::_pvt_add(const Molecules &molecules,
                              MGNum mgnum)
{
    MoleculeGroup &group = groups_by_index[idx];
    groups = group.add(molecules);
    
    for (Molecules::const_iterator it = molecules.begin();
         it != molecules.end();
         ++it)
    {
        groups_by_mol[it->number()].insert(group.number());
    }
}
                           
/** Add all of the molecules in 'molecules'
    to the group(s) identified by 'mgid'. This
    adds duplicates of any views that are already in these 
    group(s)
    
    \throw SireMol::missing_molgroup
*/
void MoleculeGroups::add(const Molecules &molecules, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    if (mgnums.count() == 1)
    {
        this->_pvt_add(molecules, mgnums.first());
    }
    else
    {
        MoleculeGroups saved_state(*this);
        
        try
        {
            foreach (MGNum mgnum, mgnums)
            {
                this->_pvt_add(molecules, groups_by_num[mgnum]);
            }
        }
        catch(...)
        {
            this->operator=(saved_state);
            throw;
        }
    }
}

/** Update the molecule group 'group' so that it has the same
    contents as 'group' - this will update all of the molecules
    in all of the groups in this set so that they are the same 
    version as in 'group' - this updates the molecules even if
    this group is not in this set */
void MoleculeGroups::update(const MoleculeGroup &group)
{
    this->_pvt_update(group);
}

/** Update all of the groups in 'groups' so that the groups
    that exist in this set have the same version and contents
    as the groups in 'group' - and also so that all of the molecules
    in this set are at the same version as in 'groups' - this
    updated the molecules even if none of the groups in 'groups'
    are in this set. */
void MoleculeGroups::update(const MoleculeGroups &groups)
{
    this->_pvt_update(groups);
}

void MoleculeGroups::_pvt_update(const MoleculeData &moldata)
{
    foreach (MGNum mgnum, groups_by_mol.value(moldata.number()))
    {
        MoleculeGroup &group = groups_by_idx( groups_by_num.value(mgnum) );
        group = group.update(moldata);
    }
}

/** Updated the molecule whose data is in 'moldata' 
    so that it is at the same version as 'moldata'. 
    This does nothing if this molecule is not in this set */
void MoleculeGroups::update(const MoleculeData &moldata)
{
    this->_pvt_update(moldata);
}

/** Update the molecule that is viewed in 'molview'
    so that it is at the same version as 'molview'
    in this set. This does nothing  is this molecule is not in 
    this set. */
void MoleculeGroups::update(const MoleculeView &molview)
{
    this->update(molview.data());
}

/** Update the molecules in 'molecules' so that they are the same version as 
    in 'molecules'. This does nothing if none of the molecules
    are in this set. */
void MoleculeGroups::update(const Molecules &molecules)
{
    if (molecules.isEmpty() or this->isEmpty())
        return;

    QSet<MGNum> already_updated;
    already_updated.reserve( nGroups() - 1 );
    
    this->_pvt_update(molecules, already_updated);
}

void MoleculeGroups::_pvt_remove(MGNum mgnum)
{
    int idx = groups_by_num.value(mgnum, -1);
    
    if (idx == -1)
        return;

    this->_pvt_removeFromIndex(mgnum);
    
    groups_by_idx.removeAt(idx);
    groups_by_num.remove(mgnum);
}

void MoleculeGroups::_pvt_reindex()
{
    groups_by_num.clear();

    int ngroups = groups_by_idx.count();
    const MoleculeGroup *groups_by_idx_array = groups_by_idx.constData();

    groups_by_num.reserve(ngroups);
    
    for (int i=0; i<ngroups; ++i)
    {
        groups_by_num.insert( groups_by_idx_array[i].number(), i );
    }
}

/** Remove the group 'group'. This does nothing if 
    this group is not part of this set. This removes the group
    even if it has a different version in this set */
void MoleculeGroups::remove(const MoleculeGroup &group)
{
    this->_pvt_remove(group.number());
    this->_pvt_reindex();
}

/** Remove all of the groups in 'groups' from this set. */
void MoleculeGroups::remove(const MoleculeGroups &groups)
{
    if (groups.nGroups() == 0)
        return;
    else if (groups.nGroups() == 1)
    {
        this->remove(groups.at(MGIdx(0)));
        return;
    }
    else
    {
        MoleculeGroups saved_state(*this);
    
        try
        {
            for (MoleculeGroups::const_iterator it = groups.begin();
                 it != groups.end();
                 ++it)
            {
                this->_pvt_remove(it->number());
            }
            
            this->_pvt_reindex();
        }
        catch(...)
        {
            this->operator=(saved_state);
            throw;
        }
    }
}

/** Remove the group(s) identified by 'mgid' */
void MoleculeGroups::remove(const MGID &mgid)
{
    QList<MGNum> mgnums;
    
    try
    {
        mgnums = mgid.map(*this);
    }
    catch(...)
    {
        return;
    }

    if (mgnums.count() == 1)
    {
        this->_pvt_remove(mgnums.first());
        this->_pvt_reindex();
        return;
    }
    else
    {
        MoleculeGroups saved_state(*this);
        
        try
        {
            foreach (MGNum mgnum, mgnums)
            {
                ret._pvt_remove(mgnum);
            }
        
            ret._pvt_reindex();
        }
        catch(...)
        {
            this->operator=(saved_state);
            throw;
        }
    }
}

void MoleculeGroups::_pvt_removeFromIndex(MGNum mgnum, MolNum molnum)
{
    QSet<MGNum> &mgnums = groups_by_mol[molnum];
    
    mgnums.remove(mgnum);
    
    if (mgnums.isEmpty())
        groups_by_mol.remove(molnum);
}

void MoleculeGroups::_pvt_remove(const MoleculeView &molview,
                                 const QSet<MGNum> &mgnums)
{
    MolNum molnum = molview.data().number();
    
    foreach (MGNum mgnum, mgnums)
    {
        MoleculeGroup &group = groups_by_idx[ groups_by_num.value(mgnum) ];
        
        group = group.remove(molview);
        
        if (not group.contains(molnum))
        {
            //the molecule has now been completely removed from this group
            this->_pvt_removeFromIndex(mgnum, molnum);
        }
    }
}

void MoleculeGroups::_pvt_remove(const ViewsOfMol &molviews,
                                 const QSet<MGNum> &mgnums)
{
    MolNum molnum = molviews.number();
    
    QSet<MGNum> mgnums_with_mol = groups_by_mol.value(molnum);
    mgnums_with_mol.intersect(mgnums);
    
    foreach (MGNum mgnum, mgnums_with_mol)
    {
        MoleculeGroup &group = groups_by_idx[ groups_by_num.value(mgnum) ];
        
        group = group.remove(molviews);
        
        if (not group.contains(molnum))
        {
            //the molecule has now been completely removed from this group
            this->_pvt_removeFromIndex(mgnum, molnum);
        }
    }
}

void MoleculeGroups::_pvt_remove(MolNum molnum, 
                                 const QSet<MGNum> &mgnums)
{
    QSet<MGNum> mgnums_with_mol = groups_by_mol.value(molnum);
    mgnums_with_mol.intersect(mgnums);
    
    foreach (MGNum mgnum, mgnums_with_mol)
    {
        MoleculeGroup &group = groups_by_idx[ groups_by_num.value(mgnum) ];
        
        group = group.remove(molnum);
        
        this->_pvt_removeFromIndex(mgnum, molnum);
    }
    
}

/** Completely remove the view of the molecule 'molview' from all
    of the groups in this set - this only removes this exact view
    from the set */
void MoleculeGroups::remove(const MoleculeView &molview)
{
    QSet<MGNum> mgnums = groups_by_mol.value(molview.data().number());
    
    if (mgnums.isEmpty())
        return;
    
    this->_pvt_remove(molview, mgnums);
}

/** Remove the view of the molecule 'molview' from the 
    group(s) identified by 'mgid' 
    
    \throw SireMol::missing_molgroup
*/
void MoleculeGroups::remove(const MoleculeView &molview, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);

    this->_pvt_remove(molview, mgnums.toSet());
}

/** Remove all of the views in 'molviews' from the returned set */
void MoleculeGroups::remove(const ViewsOfMol &molviews)
{
    QSet<MGNum> mgnums = groups_by_mol.value(molviews.number());
    
    if (mgnums.isEmpty())
        return *this;
    
    this->_pvt_remove(molviews, mgnums);
}

/** Return the set with all of the views in 'molviews' removed
    from the group(s) identified by 'mgid' 
    
    \throw SireMol::missing_molgroup
*/
void MoleculeGroups::remove(const ViewsOfMol &molviews, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    this->_pvt_remove(molviews, mgnums.toSet());
}                                      

/** Return the set where all of the molecules in 'molecules' have
    been removed */
void MoleculeGroups::remove(const Molecules &molecules)
{
    if (molecules.isEmpty())
        return;
    else if (molecules.count() == 1)
    {
        this->remove(molecules.first());
        return;
    }
    else
    {
        MoleculeGroups saved_state(*this);
    
        try
        {
            for (Molecule::const_iterator it = molecules.begin();
                 it != molecules.end();
                 ++it)
            {
                QSet<MGNum> mgnums = groups_by_mol.value(it->number());
        
                if (not mgnums.isEmpty())
                    ret._pvt_remove(*it, mgnums);
            }
        }
        catch(...)
        {
            this->operator=(saved_state);
            throw;
        }
    }
}

/** Remove all of the molecules in 'molecules'
    from the group(s) identified by 'mgid'
    
    \throw SireMol::missing_molgroup
*/
void MoleculeGroups::remove(const Molecules &molecules, const MGID &mgid)
{
    QSet<MGNum> mgnums = mgid.map(*this).toSet();

    if (molecules.isEmpty())
        return;
    else if (molecules.count() == 1)
    {
        this->_pvt_remove(molecules.first(), mgnums.toSet());
        return;
    }
    else
    {
        MoleculeGroups saved_state(*this);
    
        try
        {
            for (Molecules::const_iterator it = molecules.begin();
                 it != molecules.constEnd();
                 ++it)
            {
                this->_pvt_remove(*it, mgnums);
            }
        }
        catch(...)
        {
            this->operator=(saved_state);
            throw;
        }
    }
}

/** Return the set where all the molecule(s) identified by 'molid'
    have been completely removed. This does nothing if no molecules
    matching this ID are in this set */
void MoleculeGroups::remove(const MolID &molid)
{
    QList<MolNum> molnums;
    
    try
    {
        molnums = molid.map(*this);
    }
    catch(...)
    {
        return;
    }

    if (molnums.count() == 1)
    {
        MolNum molnum = molnums.first();
        
        QSet<MGNum> mgnums = groups_by_mol.value(molnum);
        
        if (not mgnums.isEmpty())
            this->_pvt_remove(molnum, mgnums);
            
        return;
    }
    else
    {
        MoleculeGroups saved_state(*this);
        
        try
        {
            foreach (MolNum molnum, molnums)
            {
                QSet<MGNum> mgnums = groups_by_mol.value(molnum);
            
                if (not mgnums.isEmpty())
                    this->_pvt_remove(molnum, mgnums);
            }
        }
        catch(...)
        {
            this->operator=(saved_state);
            throw;
        }
    }
}

/** Remove the molecule(s) identified by 'molid'
    from the group(s) identified by 'mgid'
    
    \throw SireMol::missing_molgroup
*/
MoleculeGroups MoleculeGroups::remove(const MolID &molid, const MGID &mgid) const
{
    QSet<MGNum> mgnums = mgid.map(*this).toSet();
    
    QList<MolNum> molnums;
    
    try
    {
        molnums = molid.map(*this);
    }
    catch(...)
    {
        return;
    }
    
    if (molnums.isEmpty())
        return;
    else if (molnums.count() == 1)
    {
        MolNum molnum = molnums.first();
        this->_pvt_remove(molnum, mgnums);
        return;
    }
    else
    {
        MoleculeGroups saved_state(*this);
        
        try
        {
            foreach (MolNum molnum, molnums)
            {
                ret._pvt_remove(molnum, mgnums);
            }
        }
        catch(...)
        {
            this->operator=(saved_state);
            throw;
        }
    }
}

/** Return the number of groups in this set */
int MoleculeGroups::nGroups() const
{
    return groups_by_idx.count();
}

/** Return the number of molecules in this set */
int MoleculeGroups::nMolecules() const
{
    return groups_by_mol.count();
}

/** Return the number of molecules in the group(s) identified
    by 'mgid'
    
    \throw SireMol::missing_molgroup
*/
int MoleculeGroups::nMolecules(const MGID &mgid) const
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    if (mgnums.count() == 1)
    {
        return this->group(mgnums.first()).nMolecules();
    }

    QSet<MolNum> molnums;

    foreach (MGNum mgnum, mgid.map(*this))
    {
        molnums += this->group(mgnum).molecules().molNums();
    }
    
    return molnums.count();
}

/** Return the number of unique molecule views in this set */
int MoleculeGroups::nViews() const
{
    if (this->isEmpty())
    {
        return 0;
    }
    else if (this->nGroups() == 1)
    {
        return groups_by_idx[0].molecules().nViews();
    }
    else
    {
        //first, sum up all of the views...
        int nviews = 0;
        
        for (MoleculeGroups::const_iterator it = this->begin();
             it != this->end();
             ++it)
        {
            nviews += it->molecules().nViews();
        }
        
        //now subtract any duplicates...
        for (QHash< MolNum, QSet<MGNum> >::const_iterator it = groups_by_mol.begin();
             it != groups_by_mol.end();
             ++it)
        {
            if (it.value().count() > 1)
            {
                //this molecule is in more than one group - we need
                //to check for any duplicated views...
                throw SireError::incomplete_code( "REMOVE DUPLICATES!", CODELOC );
            }
        }
        
        return nviews;
    }
}

/** Return the number of unique molecule views in the 
    group(s) identified by 'mgid'
    
    \throw SireMol::missing_molgroup
*/
int MoleculeGroups::nViews(const MGID &mgid) const
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    if (mgnums.count() == 1)
    {
        return this->group(mgnums.first()).nViews();
    }
    else
    {
        //sum up the total number of views in these groups
        int nviews = 0;
        
        foreach (MGIdx mgnum, mgnums)
        {
            nviews += this->group(mgnum).nViews();
        }
        
        //now remove duplicates
        throw SireError::incomplete_code( "REMOVE DUPLICATES!", CODELOC );
        
        return nviews;
    }
}

/** Return the group with number 'mgnum'

    \throw SireMol::missing_molgroup
*/
const MoleculeGroup& MoleculeGroups::group(MGNum mgnum) const
{
    int idx = groups_by_num.value(mgnum, -1);
    
    if (idx == -1)
        throw SireMol::missing_molgroup( QObject::tr(
            "There are no MoleculeGroups in this set with the "
            "ID number %1. (available groups are %2).")
                .arg(mgnum)
                .arg( Sire::toString(groups_by_num.keys()) ), CODELOC );

    return groups_by_idx.constData()[idx];
}

/** Return the group identified by the ID 'mgid'

    \throw SireMol::missing_molgroup
    \throw SireMol::duplicate_molgroup
*/
const MoleculeGroup& MoleculeGroups::group(const MGID &mgid) const
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    if (mgnums.count() > 1)
        throw SireMol::duplicate_molgroup( QObject::tr(
            "More than one MoleculeGroup matches the ID \"%1\" "
            "(groups with numbes %2)")
                .arg(mgid.toString(), Sire::toString(mgnums)), CODELOC );
                
    return this->group(mgnums.first());
}

/** Return the group(s) identified by the ID 'mgid'

    \throw SireMol::missing_molgroup
*/
MoleculeGroups MoleculeGroups::groups(const MGID &mgid) const
{
    QList<MGNum> mgnums = mgid.map(*this);

    if (mgnums.count() == 1)
    {
        return MoleculeGroups( this->group(mgnums.first()) );
    }
    else
    {
        //convert group numbers to indicies
        QList<MGIdx> mgidxs;
        
        foreach (MGNum mgnum, mgnums)
        {
            mgidxs.append( groups_by_num[mgnum] );
        }

        qSort(mgidxs);
    
        MoleculeGroups ret;
    
        ret.groups_by_idx.reserve(mgidxs.count());
        ret.groups_by_num.reserve(mgidxs.count());
    
        int i = 0;
    
        foreach (MGIdx mgidx, mgidxs)
        {
            const MoleculeGroup &group = groups_by_idx[mgidx];
        
            ret.groups_by_idx.append(group);
            ret.groups_by_num.insert(group.number(), i);
            ++i;
            
            ret._pvt_addToIndex(i);
        }
        
        return ret;
    }
}

/** Return all of the unique views of all of the molecules in this set */
Molecules MoleculeGroups::molecules() const
{
    if (this->isEmpty())
        return Molecules();
    else if (nGroups() == 1)
        return groups_by_idx[0].molecules();
    else
    {
        MoleculeGroups::const_iterator it = this->begin();
    
        Molecules mols = it->molecules();
        
        for (++it; it != this->end(); ++it)
        {
            mols = mols.addIfUnique(it->molecules());
        }
    }
}

/** Return all of the unique views of all of the molecules in the group(s)
    identified by 'mgid'
    
    \throw SireMol::missing_molgroup
*/
Molecules MoleculeGroups::molecules(const MGID &mgid) const
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    if (mgnums.count() == 1)
    {
        return this->group(mgnums.first()).molecules();
    }
    else
    {
        Molecules mols;
        
        foreach (MGNum mgnum, mgnums)
        {
            mols = mols.addIfUnique( this->group(mgnum).molecules() );
        }
        
        return mols;
    }
}

/** Return all of the unique views of the molecule identified
    by the ID 'molid' in this set 
    
    \throw SireMol::missing_molecule
    \throw SireMol::duplicate_molecule
*/
ViewsOfMol MoleculeGroups::molecule(const MolID &molid) const
{
    QList<MolNum> molnums = molid.map(*this);
    
    if (molnums.count() > 1)
        throw SireMol::duplicate_molecule( QObject::tr(
            "Multiple molecules in these groups match the ID \"%1\" "
            "(molecule numbers %2).")
                .arg(molid.toString(), Sire::toString(molnums)), CODELOC );
             
    MolNum molnum = molnums.first();  
              
    QSet<MGNum> mgnums = groups_by_mol.value(molnum);
    
    if (mgnums.count() == 1)
    {
        return this->group(*(mgnums.begin())).molecule(molnum);
    }
    else
    {
        QSet<MGNum>::const_iterator it = mgnums.constBegin();
        
        ViewsOfMol molviews = this->group(*it).molecule(molnum);
        
        for (++it; it != mgnums.constEnd(); ++it)
        {
            molviews = molviews.addIfUnique(this->group(*it).molecule(molnum));
        }
        
        return molviews;
    }
}
