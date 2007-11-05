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

void MoleculeGroups::_pvt_update(const MoleculeGroup &group)
{
    int idx = groups_by_num.value(group.number(), -1);
    
    if (idx == -1)
        return;
        
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

MoleculeGroups MoleculeGroups::update(const MoleculeGroup &group) const;
MoleculeGroups MoleculeGroups::update(const MoleculeGroups &groups) const;

MoleculeGroups MoleculeGroups::update(const MoleculeView &molview) const;
MoleculeGroups MoleculeGroups::update(const MoleculeData &moldata) const;
MoleculeGroups MoleculeGroups::update(const Molecules &molecules) const;

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

int MoleculeGroups::nGroups() const;

int MoleculeGroups::nMolecules() const;
int MoleculeGroups::nMolecules(const MGID &mgid) const;

int MoleculeGroups::nViews() const;
int MoleculeGroups::nViews(const MGID &mgid) const;

const MoleculeGroup& MoleculeGroups::group(const MGID &mgid) const;
MoleculeGroups MoleculeGroups::groups(const MGID &mgid) const;

Molecules MoleculeGroups::molecules() const;
Molecules MoleculeGroups::molecules(const MGID &mgid) const;

const ViewsOfMol& MoleculeGroups::molecule(const MolID &molid) const;
