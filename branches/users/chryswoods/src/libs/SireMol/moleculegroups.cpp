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
    return this->operator=(MoleculeGroups(other);
}

/** Comparison operator */
bool MoleculeGroups::operator==(const MoleculeGroups &other) const
{
    return groups_by_idx == other.groups_by_index;
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
            it2.insert(group.number());
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
        groups_by_idx[0] = groups_by_idx[0].update(molecules);
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
                
                group = group.update(molecules);
                
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
    already_updated.insert(group.number());
    
    //now update all of the molecules in this set so that
    //they are the same version as in 'group'
    this->_pvt_update(group.molecules(), already_updated);
}

void MoleculeGroups::_pvt_update(const MoleculeGroups &groups)
{
    if (groups.isEmpty() or this->isEmpty())
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
    if (this->contains(group.number()))
    {
        this->_pvt_update(group);
    }
    else
    {
        groups_by_idx.append(group);
        int idx = groups_by_index.count() - 1;
        
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

/** Return the groups to which the group 'group' has been added.
    Note that this will update 'group' if it already exists in this set */
MoleculeGroups MoleculeGroups::add(const MoleculeGroup &group) const
{
    MoleculeGroups ret(*this);
    ret._pvt_add(group);
    
    return ret;
}

/** Return the groups to which all of the groups in 'groups'
    have been added. If any of the groups already exist in this
    set, then they will be updated rather than added. */
MoleculeGroups MoleculeGroups::add(const MoleculeGroups &groups) const
{
    MoleculeGroups ret(*this);
    
    for (MoleculeGroups::const_iterator it = groups.constBegin();
         it != groups.constEnd();
         ++it)
    {
        ret._pvt_add(*it);
    }
    
    return ret;
}

void MoleculeGroups::_pvt_add(const MoleculeView &molview,
                              int idx)
{
    MoleculeGroup &group = groups_by_index[idx];
    group = group.add(molview);
    
    groups_by_mol[molview.data().number()].insert(group.number());
}

/** Return the groups where the molecule view 'molview' has been
    added to the group(s) identified by 'mgid'. If this view
    already exists in the group(s) then it will be duplicated.
    
    \throw SireMol::missing_molgroup
*/
MoleculeGroups MoleculeGroups::add(const MoleculeView &molview,
                                   const MGID &mgid) const
{
    QList<MGIdx> mgidxs = mgid.map(*this);

    MoleculeGroups ret(*this);
    
    foreach (MGIdx mgidx, mgidxs)
    {
        ret._pvt_add(molview, mgidx);
    }
    
    return ret;
}

void MoleculeGroups::_pvt_add(const ViewsOfMol &molviews,
                              int idx)
{
    MoleculeGroup &group = groups_by_index[idx];
    group = group.add(molviews);
    
    groups_by_mol[molviews.number()].insert(group.number());
}
                 
/** Return the groups where the views of the molecule in 'molviews'
    are added to the group(s) identified by 'mgid'. This adds
    duplicates of any views that are already in these group(s)
    
    \throw SireMol::missing_molgroup
*/
MoleculeGroups MoleculeGroups::add(const ViewsOfMol &molviews,
                                   const MGID &mgid) const
{
    QList<MGIdx> mgidxs = mgid.map(*this);
    
    MoleculeGroups ret(*this);
    
    foreach (MGIdx mgidx, mgidxs)
    {
        ret._pvt_add(molviews, mgidx);
    }
    
    return ret;
}
               
void MoleculeGroups::add(const Molecules &molecules,
                         int idx)
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
                           
/** Return the groups where all of the molecules in 'molecules'
    have been added to the group(s) identified by 'mgid'. This
    adds duplicates of any views that are already in these 
    group(s)
    
    \throw SireMol::missing_molgroup
*/
MoleculeGroups MoleculeGroups::add(const Molecules &molecules,
                                   const MGID &mgid) const
{
    QList<MGIdx> mgidxs = mgid.map(*this);
    
    MoleculeGroups ret(*this);
    
    foreach (MGIdx mgidx, mgidxs)
    {
        ret._pvt_add(molecules, mgidx);
    }
    
    return ret;
}

/** Update the molecule group 'group' so that it has the same
    contents as 'group' - this will update all of the molecules
    in all of the groups in this set so that they are the same 
    version as in 'group' - this updates the molecules even if
    this group is not in this set */
MoleculeGroups MoleculeGroups::update(const MoleculeGroup &group) const
{
    MoleculeGroups ret(*this);
    
    ret._pvt_update(group);
    
    return ret;
}

/** Update all of the groups in 'groups' so that the groups
    that exist in this set have the same version and contents
    as the groups in 'group' - and also so that all of the molecules
    in this set are at the same version as in 'groups' - this
    updated the molecules even if none of the groups in 'groups'
    are in this set. */
MoleculeGroups MoleculeGroups::update(const MoleculeGroups &groups) const
{
    MoleculeGroups ret(*this);
    
    ret._pvt_update(groups);
    
    return ret;
}

void MoleculeGroups::_pvt_update(const MoleculeData &moldata)
{
    foreach (MGNum mgnum, groups_by_mol.value(moldata.number()))
    {
        MoleculeGroup &group = groups_by_idx( groups_by_num.value(mgnum) );
        group = group.update(moldata);
    }
}

/** Return the set where the molecule whose data is in 'moldata' 
    has been updated so that it is at the same version as 'moldata'. 
    This does nothing if this molecule is not in this set */
MoleculeGroups MoleculeGroups::update(const MoleculeData &moldata) const
{
    MoleculeGroups ret(*this);
    
    ret._pvt_update(moldata);
    
    return ret;
}

/** Return the set where the molecule that is viewed in 'molview'
    has been updated so that it is at the same version as 'mollview'
    in this set. This does nothing  is this molecule is not in 
    this set. */
MoleculeGroups MoleculeGroups::update(const MoleculeView &molview) const
{
    return this->update(molview.data());
}

/** Return the set where the molecules in 'molecules' have been
    updated in this set so that they are the same version as 
    in 'molecules'. This does nothing if none of the molecules
    are in this set. */
MoleculeGroups MoleculeGroups::update(const Molecules &molecules) const
{
    if (molecules.isEmpty() or this->isEmpty())
        return *this;

    MoleculeGroups ret(*this);
    
    QSet<MGNum> already_updated;
    already_updated.reserve( nGroups() - 1 );
    
    ret._pvt_update(molecules, already_updated);
    
    return ret;
}


MoleculeGroups MoleculeGroups::remove(const MoleculeGroup &group) const;
MoleculeGroups MoleculeGroups::remove(const MoleculeGroups &groups) const;

MoleculeGroups MoleculeGroups::remove(const MGID &mgid) const;

MoleculeGroups MoleculeGroups::remove(const MoleculeView &molview) const;
MoleculeGroups MoleculeGroups::remove(const MoleculeView &molview, 
                      const MGID &mgid) const;

MoleculeGroups MoleculeGroups::remove(const ViewsOfMol &molviews) const;
MoleculeGroups MoleculeGroups::remove(const ViewsOfMol &molviews,
                      const MGID &mgid) const;

MoleculeGroups MoleculeGroups::remove(const Molecules &molecules) const;
MoleculeGroups MoleculeGroups::remove(const Molecules &molecules,
                      const MGID &mgid) const;

MoleculeGroups MoleculeGroups::remove(const MolID &molid) const;
MoleculeGroups MoleculeGroups::remove(const MolID &molid, const MGID &mgid) const;

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
    QList<MGIdx> mgidxs = mgid.map(*this);
    
    if (mgidxs.count() == 1)
    {
        return groups_by_idx[mgidx.first()].nMolecules();
    }

    QSet<MolNum> molnums;

    foreach (MGIdx mgidx, mgid.map(*this))
    {
        molnums += groups_by_idx[mgidx].molecules().molNums();
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
    QList<MGIdx> mgidxs = mgid.map(*this);
    
    if (mgidxs.count() == 1)
    {
        return groups_by_idx[mgidxs.first()].nViews();
    }
    else
    {
        //sum up the total number of views in these groups
        int nviews = 0;
        
        foreach (MGIdx mgidx, mgidxs)
        {
            nviews += groups_by_idx[mgidx].nViews();
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
    QList<MGIdx> mgidxs = mgid.map(*this);
    
    if (mgidxs.count() > 1)
        throw SireMol::duplicate_molgroup( QObject::tr(
            "More than one MoleculeGroup matches the ID \"%1\" "
            "(groups at indicies %2)")
                .arg(mgid.toString(), Sire::toString(mgidxs)), CODELOC );
                
    return groups_by_idx[ mgidxs.first() ];
}

/** Return the group(s) identified by the ID 'mgid'

    \throw SireMol::missing_molgroup
*/
MoleculeGroups MoleculeGroups::groups(const MGID &mgid) const
{
    QList<MGIdx> mgidxs = mgid.map(*this);

    if (mgidxs.count() == 1)
    {
        return MoleculeGroups( groups_by_idx[ mgidxs.first() ] );
    }
    else
    {
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
    QList<MGIdx> mgidxs = mgid.map(*this);
    
    if (mgidxs.count() == 1)
    {
        return groups_by_idx[ mgidxs.begin() ].molecules();
    }
    else
    {
        Molecules mols;
        
        foreach (MGIdx mgidx, mgidxs)
        {
            mols = mols.addIfUnique( groups_by_idx[mgidx].molecules() );
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
