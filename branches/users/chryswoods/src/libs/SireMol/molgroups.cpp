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

RegisterMetaType<MolGroupsBase> r_molgroupsbase(MAGIC_ONLY,
                                                "SireMol::MolGroupsBase");
                                                
/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const MolGroupsBase &molgroupsbase)
{
    writeHeader(ds, r_molgroupsbase, 1);
    
    SharedDataStream sds(ds);
    
    sds << molgroupsbase.mgidx_to_num
        << molgroupsbase.mgname_to_mgnum
        << molgroupsbase.molnum_to_mgnum
        << static_cast<const PropertyBase&>(molgroupsbase);
        
    return ds;
}

/** Read from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       MolGroupsBase &molgroupsbase)
{
    VersionID v = readHeader(ds, r_molgroupsbase);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> molgroupsbase.mgidx_to_num
            >> molgroupsbase.mgname_to_mgnum
            >> molgroupsbase.molnum_to_mgnum
            >> static_cast<PropertyBase&>(molgroupsbase);
    }
    else
        throw version_error(r_molgroupsbase, "1", CODELOC);
        
    return ds;
}

/** Null constructor */
MolGroupsBase::MolGroupsBase() : PropertyBase()
{}

/** Copy constructor */
MolGroupsBase::MolGroupsBase(const MolGroupsBase &other)
              : PropertyBase(other),
                mgidx_to_num(other.mgidx_to_num),
                mgname_to_mgnum(other.mgname_to_mgnum),
                molnum_to_mgnum(other.molnum_to_mgnum)
{}

MolGroupsBase::~MolGroupsBase();

/** Copy assignment operator */
MolGroupsBase& MolGroupsBase::operator=(const MolGroupsBase &other)
{
    if (this != &other)
    {
        mgidx_to_num = other.mgidx_to_num;
        mgname_to_mgnum = other.mgname_to_mgnum;
        molnum_to_mgnum = other.molnum_to_mgnum;
    }
    
    return *this;
}

/** Return a const reference to the molecule group with number 'mgnum'

    \throw SireMol::missing_group
*/
const MolGroup& MolGroupsBase::operator[](MGNum mgnum) const
{
    return this->at(mgnum);
}

/** Return a const reference to the molecule group with name 'mgname'

    \throw SireMol::missing_group
*/
const MolGroup& MolGroupsBase::operator[](const MGName &mgname) const
{
    return this->at(mgname);
}

/** Return a const reference to the molecule group that is 
    identified by 'mgid'
    
    \throw SireMol::missing_group
    \throw SireMol::duplicate_group
*/
const MolGroup& MolGroupsBase::operator[](const MGID &mgid) const
{
    return this->at(mgid);
}

/** Return all of the views of the molecule with number 'molnum'
    that are contained in this set of groups. Note that if the 
    same view appears in multiple groups, then it will be returned
    multiple times in the returned set of views
    
    \throw SireMol::missing_molecule
*/
ViewsOfMol MolGroupsBase::operator[](MolNum molnum) const
{
    return this->at(molnum);
}

/** Return all of the views of the molecule identified by 'molid'
    that are contained in this set of groups. Note that if the 
    same view appears in multiple groups, then it will be returned
    multiple times in the returned set of views
    
    \throw SireMol::missing_molecule
    \throw SireMol::duplicate_molecule
*/
ViewsOfMol MolGroupsBase::operator[](const MolID &molid) const
{
    return this->at(molid);
}

/** Get the number of the molecule group whose number is 'mgnum'.
    This is an obvious function, only provided as a shortcut
    to prevent the MGID function being called if an MGNum is passed.
    
    \throw SireMol::missing_group
*/
MGNum MolGroupsBase::getGroupNumber(MGNum mgnum) const
{
    this->assertContains(mgnum);
    return mgnum;
}

/** Return the number of the molecule group that is called 'mgname'.

    \throw SireMol::missing_group
    \throw SireMol::duplicate_group
*/
MGNum MolGroupsBase::getGroupNumber(const MGName &mgname) const
{
    QList<MGNum> mgnums = this->map(mgname);
    
    if (mgnums.count() > 1)
        throw SireMol::duplicate_group( QObject::tr(
            "There are multiple molecule groups called \"%1\" "
            "in this set - with group numbers %2.")
                .arg(mgname).arg(Sire::toString(mgnums)), CODELOC );
                
    return mgnums.first();
}

/** Return the number of the group at index 'mgidx'

    \throw SireError::invalid_index
*/
MGNum MolGroupsBase::getGroupNumber(MGIdx mgidx) const
{
    return mgidx_to_num.constData()[ mgidx.map(mgidx_to_num.count()) ];
}

/** Return the number of the groups that matches 'mgid'

    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
MGNum MolGroupsBase::getGroupNumber(const MGID &mgid) const
{
    QList<MGNum> mgnums = this->map(mgid);
    
    if (mgnums.count() > 1)
        throw SireMol::duplicate_group( QObject::tr(
            "There is more than one molecule group that matches "
            "the ID \"%1\". Matching groups have numbers %2.")
                .arg(mgid.toString()).arg(Sire::toString(mgnums)), 
                    CODELOC );
                    
    return mgnums.first();
}

/** Return the numbers of all groups in this set that are called
    'mgname'
    
    \throw SireMol::missing_group
*/
QList<MGNum> MolGroups::map(const MGName &mgname) const
{
    QHash< MGName,QList<MGNum> >::const_iterator it = mgname_to_mgnum.find(mgname);
    
    if (it == mgname_to_mgnum.end())
        throw SireMol::missing_group( QObject::tr(
            "There are no molecule groups called \"%1\" in this set. "
            "Available groups are %2.")
                .arg(mgname).arg(Sire::toString(mgname_to_mgnum.keys())), 
                    CODELOC );
                    
    return *it;
}

/** Return the list of numbers of groups that have the number 'mgnum'.
    This is a simple and obvious function that acts as a shortcut
    preventing map(const MGID&) being called for an MGNum
    
    \throw SireMol::missing_group
*/
QList<MGNum> MolGroupsBase::map(MGNum mgnum) const
{
    this->assertContains(mgnum);
    
    QList<MGNum> mgnums;
    mgnums.append(mgnum);
    
    return mgnums;
}

/** Return the list (of only one) molecule group that is at  
    index 'mgidx'
    
    \throw SireError::invalid_index
*/
QList<MGNum> MolGroupsBase::map(MGIdx mgidx) const
{
    QList<MGNum> mgnums;
    
    mgnums.append( this->getGroupNumber(mgidx) );
    
    return mgnums;
}

/** Map the molecule group ID 'mgid' to the list of molecule
    group numbers of the groups that match this ID in this set.
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
QList<MGNum> MolGroupsBase::map(const MGID &mgid) const
{
    return mgid.map(*this);
}

/** Simple function that just checks if a molecule with number 
    'molnum' is in the set, and returns it. This shortcuts
    the getMoleculeNumber(const MolID&) function in the case
    of MolNums
    
    \throw SireMol::missing_molecule
*/
MolNum MolGroupsBase::getMoleculeNumber(MolNum molnum) const
{
    this->assertContains(molnum);
    return molnum;
}

/** Return the number of the molecule at index 'molidx' in 
    this set
    
    \throw SireError::invalid_index
*/
MolNum MolGroupsBase::getMoleculeNumber(MolIdx molidx) const
{
    int n = molidx.map( molnum_to_mgnum.count() );
    
    //find the nth molnum in the hash - this is potentially very slow!
    QHash< MolNum,QVector<MGNum> >::const_iterator it = molnum_to_mgnum.begin();
    
    for (int i=0; i<n; ++i)
    {
        ++it;
    }
    
    return it.key();
}

/** Return the number of the molecule called 'molname' from this set.
    
    \throw SireMol::missing_molecule
    \throw SireMol::duplicate_molecule
*/
MolNum MolGroupsBase::getMoleculeNumber(const MolName &molname) const
{
    QList<MolNum> molnums = this->map(molname);
    
    if (molnums.count() > 1)
        throw SireMol::duplicate_molecule( QObject::tr(
            "There is more than one molecule with the name \"%1\" "
            "in this set of molecule groups. Molecules with this "
            "name have numbers %2.")
                .arg(molname).arg(Sire::toString(molnums)), CODELOC );
                
    return molnums.first();
}

/** Return the number of the molecule that matches the ID 'molid'

    \throw SireMol::missing_molecule
    \throw SireMol::duplicate_molecule
    \throw SireError::invalid_index
*/
MolNum MolGroupsBase::getMoleculeNumber(const MolID &molid) const
{
    QList<MolNum> molnums = this->map(molid);
    
    if (molnums.count() > 1)
        throw SireMol::duplicate_molecule( QObject::tr(
            "There is more than one molecule that matches the "
            "ID \"%1\". Molecules that match have number %2.")
                .arg(molid.toString()).arg(Sire::toString(molnums)),
                    CODELOC );
                    
    return molnums.first();
}

/** Simple function that provides a shortcut for map(const MolID&)

    \throw SireMol::missing_molecule
*/
QList<MolNum> MolGroupsBase::map(MolNum molnum) const
{
    this->assertContains(molnum);
    
    QList<MolNum> molnums;
    molnums.append(molnum);
    
    return molnums;
}

/** Return the number of the molecule at index 'molidx'

    \throw SireError::invalid_index
*/
QList<MolNum> MolGroupsBase::map(MolIdx molidx) const
{
    QList<MolNum> molnums;
    molnums.append( this->getMoleculeNumber(molidx) );
    
    return molnums;
}

/** Return the numbers of all of the molecules that have the
    name 'molname'
    
    \throw SireMol::missing_molecule
*/
QList<MolNum> MolGroupsBase::map(const MolName &molname) const
{
    QList<MolNum> molnums;
    
    //loop over all of the groups in this set
    const QHash<MGNum,const MolGroup*> molgroups = this->getGroups();
    
    for (QHash<MGNum,const MolGroup*>::const_iterator it = molgroups.constBegin();
         it != molgroups.constEnd();
         ++it)
    {
        try
        {
            molnums.append( (*it)->map(molname) );
        }
        catch(...)
        {}
    }
    
    if (molnums.isEmpty())
        throw SireMol::missing_molecule( QObject::tr(
            "There are no molecules called \"%1\" in any of the "
            "molecule groups in this set.")
                .arg(molname), CODELOC );
    
    else if (molnums.count() > 1)
        //remove any duplicates
        molnums = molnums.toSet().toList();

    return molnums;
}

/** Return the numbers of all molecules that match the ID 'molid'

    \throw SireMol::missing_molecule
    \throw SireError::invalid_index
*/
QList<MolNum> MolGroupsBase::map(const MolID &molid) const
{
    return molid.map(*this);
}

/** Return a const reference to the group at index 'mgidx'

    \throw SireError::invalid_index
*/
const MolGroup& MolGroupsBase::at(MGIdx mgidx) const
{
    return this->at( this->getGroupNumber(mgidx) );
}

/** Return a const reference to the group in this set 
    called 'mgname'
    
    \throw SireMol::missing_group
    \throw SireMol::duplicate_group
*/
const MolGroup& MolGroupsBase::at(const MGName &mgname) const
{
    return this->at( this->getGroupNumber(mgname) );
}

/** Return a const reference to the group in this set that
    is identified by 'mgid' 
    
    \throw SireMol::missing_group
    \throw SireMol::duplicate_group
    \throw SireError::invalid_index
*/
const MolGroup& MolGroupsBase::at(const MGID &mgid) const
{
    return this->at( this->getGroupNumber(mgid) );
}

/** Return all of the views of the molecule with number 'molnum'
    that are contained in this set of groups. Note that if the 
    same view appears in multiple groups, then it will be returned
    multiple times in the returned set of views
    
    \throw SireMol::missing_molecule
*/
ViewsOfMol MolGroupsBase::at(MolNum molnum) const
{
    //get the list of groups that contain this molecule
    const QVector<MGNum> &mgnums = this->groupsContaining(molnum);
    
    if (mgnums.count() == 1)
        return this->at( mgnums.at(0) ).molecule(molnum);
    else
    {
        ViewsOfMol mol = this->at( mgnums.at(0) ).molecule(molnum);
        
        for (int i=1; i<mgnums.count(); ++i)
        {
            mol.add( this->at( mgnums.at(i) ).molecule(molnum) );
        }
        
        return mol;
    }
}

/** Return all of the views of the molecule identified by 'molid'
    that are contained in this set of groups. Note that if the 
    same view appears in multiple groups, then it will be returned
    multiple times in the returned set of views
    
    \throw SireMol::missing_molecule
    \throw SireMol::duplicate_molecule
*/
ViewsOfMol MolGroupsBase::at(const MolID &molid) const
{
    QList<MolNum> molnums = molid.map(*this);
    
    if (molnums.count() > 1)
        throw SireMol::duplicate_molecule( QObject::tr(
            "There is more than one molecule that matches the ID "
            "\"%1\". Matching molecules have numbers %2.")
                .arg(molid.toString())
                .arg(Sire::toString(molnums)), CODELOC );
                
    return this->at(molnums.first());
}

/** Return the MoleculeGroup that matches the ID 'mgid'

    \throw SireMol::missing_group
    \throw SireMol::duplicate_group
    \throw SireError::invalid_index
*/
MoleculeGroup MolGroupsBase::select(const MGID &mgid) const
{
    return this->at(mgid);
}

/** Return all of the views of the molecule with number 'molnum'
    that are contained in this set of groups. Note that if the 
    same view appears in multiple groups, then it will be returned
    multiple times in the returned set of views
    
    \throw SireMol::missing_molecule
*/
ViewsOfMol MolGroupsBase::select(const MolID &molid) const
{
    return this->at(molid);
}

Segment MolGroupsBase::select(const SegID &segid) const;
Chain MolGroupsBase::select(const ChainID &chainid) const;
Residue MolGroupsBase::select(const ResID &resid) const;
CutGroup MolGroupsBase::select(const CGID &cgid) const;

/** Return the atom from this set that matches the ID 'atomid'.
    This atom must be contained in one of the groups in this set.
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom MolGroupsBase::select(const AtomID &atomid) const
{
    return atomid.selectFrom(*this);
}

QList<MoleculeGroup> MolGroupsBase::selectAll(const MGID &mgid) const;
QList<ViewsOfMol> MolGroupsBase::selectAll(const MolID &molid) const;

QHash< MolNum,Selector<Segment> > MolGroupsBase::selectAll(const SegID &segid) const;
QHash< MolNum,Selector<Chain> > MolGroupsBase::selectAll(const ChainID &chainid) const;
QHash< MolNum,Selector<Residue> > MolGroupsBase::selectAll(const ResID &resid) const;
QHash< MolNum,Selector<CutGroup> > MolGroupsBase::selectAll(const CGID &cgid) const;

/** Return all of the atoms from this set that match the ID 'atomid'.
    The returned atoms are arranged by molecule, and only one copy
    of each atom is returned, regardless of how many times it appears
    in this set.
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
QHash< MolNum,Selector<Atom> > 
MolGroupsBase::selectAll(const AtomID &atomid) const
{
    return atomid.selectAllFrom(*this);
}

const MolGroup& MolGroupsBase::group(MGNum mgnum) const;
const MolGroup& MolGroupsBase::group(const MGName &mgname) const;
const MolGroup& MolGroupsBase::group(MGIdx mgidx) const;
const MolGroup& MolGroupsBase::group(const MGID &mgid) const;

QList<MoleculeGroup> MolGroupsBase::groups(MGNum mgnum) const;
QList<MoleculeGroup> MolGroupsBase::groups(const MGName &mgname) const;
QList<MoleculeGroup> MolGroupsBase::groups(const MGID &mgid) const;

ViewsOfMol MolGroupsBase::molecule(MolNum molnum) const;
ViewsOfMol MolGroupsBase::molecule(const MolID &molid) const;

QList<ViewsOfMol> MolGroupsBase::molecules(MolNum molnum) const;
QList<ViewsOfMol> MolGroupsBase::molecules(const MolID &molid) const;

Segment MolGroupsBase::segment(const SegID &segid) const;
Chain MolGroupsBase::chain(const ChainID &chainid) const;
Residue MolGroupsBase::residue(const ResID &resid) const;
CutGroup MolGroupsBase::cutGroup(const CGID &cgid) const;
Atom MolGroupsBase::atom(const AtomID &atomid) const;

QHash< MolNum,Selector<Segment> > MolGroupsBase::segments(const SegID &segid) const;
QHash< MolNum,Selector<Chain> > MolGroupsBase::chains(const ChainID &chainid) const;
QHash< MolNum,Selector<Residue> > MolGroupsBase::residues(const ResID &resid) const;
QHash< MolNum,Selector<CutGroup> > MolGroupsBase::cutGroups(const CGID &cgid) const;
QHash< MolNum,Selector<Atom> > MolGroupsBase::atoms(const AtomID &atomid) const;

bool MolGroupsBase::contains(MGNum mgnum) const;
bool MolGroupsBase::contains(MolNum molnum) const;
bool MolGroupsBase::contains(const MoleculeView &molview) const;
bool MolGroupsBase::contains(const ViewsOfMol &molviews) const;
bool MolGroupsBase::contains(const Molecules &molecules) const;

bool MolGroupsBase::intersects(const MoleculeView &molview) const;
bool MolGroupsBase::intersects(const Molecules &other) const;

const QVector<MGNum>& MolGroupsBase::groupsContaining(MolNum molnum) const;

int MolGroupsBase::nMolecules() const;

int MolGroupsBase::nViews() const;
int MolGroupsBase::nViews(MolNum molnum) const;

bool MolGroupsBase::isEmpty() const;

Molecules MolGroupsBase::molecules() const;

QSet<MolNum> MolGroupsBase::molNums() const;
QSet<MGNum> MolGroupsBase::mgNums() const;

void MolGroupsBase::assertContains(MolNum molnum) const;
void MolGroupsBase::assertContains(const MolID &molid) const;

void MolGroupsBase::assertContains(MGNum mgnum) const;
void MolGroupsBase::assertContains(const MGID &mgid) const;

void MolGroupsBase::unite(const MoleculeView &molview, const MGID &mgid);
void MolGroupsBase::unite(const ViewsOfMol &molviews, const MGID &mgid);
void MolGroupsBase::unite(const Molecules &molecules, const MGID &mgid);
void MolGroupsBase::unite(const MolGroup &molgroup, const MGID &mgid);

void MolGroupsBase::remove(const MoleculeView &molview);
void MolGroupsBase::remove(const ViewsOfMol &molviews);
void MolGroupsBase::remove(const Molecules &molecules);
void MolGroupsBase::remove(const MolGroup &molgroup);

void MolGroupsBase::removeAll(const MoleculeView &molview);
void MolGroupsBase::removeAll(const ViewsOfMol &molviews);
void MolGroupsBase::removeAll(const Molecules &molecules);
void MolGroupsBase::removeAll(const MolGroup &molgroup);

void MolGroupsBase::remove(MolNum molnum);
void MolGroupsBase::remove(const QSet<MolNum> &molnums);

void MolGroupsBase::removeAll();

void MolGroupsBase::update(const MoleculeView &molview);

bool MolGroupsBase::needToUpdate(const MoleculeData &moldata) const;

const MoleculeData& MolGroupsBase::matchToExistingVersion(const MoleculeData &moldata) const;

Molecules MolGroupsBase::matchToExistingVersion(const Molecules &molecules) const;

void MolGroupsBase::addToIndex(const MolGroup &molgroup);
void MolGroupsBase::addToIndex(MGNum mgnum, MolNum molnum);
void MolGroupsBase::addToIndex(MGNum mgnum, const QSet<MolNum> &molnums);

void MolGroupsBase::removeFromIndex(MGNum mgnum);
void MolGroupsBase::removeFromIndex(MolNum molnum);

void MolGroupsBase::removeFromIndex(MGNum mgnum, MolNum molnum);
void MolGroupsBase::removeFromIndex(MGNum mgnum, const QSet<MolNum> &molnums);

void MolGroupsBase::clearIndex(MGNum mgnum);

void MolGroupsBase::clearIndex();

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
void MolGroups::removeAll(const MoleculeView &molview, const MGID &mgid)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    MolNum molnum = molview.data().number();
    
    foreach (MGNum mgnum, mgnums)
    {
        MoleculeGroup &mgroup = *(mgroups.find(mgnum));
        
        if (mgroup.removeAll(molview))
        {
            if (not mgroup.contains(molnum))
                this->removeFromIndex(mgnum, molnum);
        }
    }
}

/** Remove all copies of the views of the molecule in 'molviews' from
    the groups identified by 'mgid'
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::removeAll(const ViewsOfMol &molviews, const MGID &mgid)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    MolNum molnum = molviews.number();
    
    foreach (MGNum mgnum, mgnums)
    {
        MoleculeGroup &mgroup = *(mgroups.find(mgnum));
    
        ViewsOfMol removed_views = mgroup.removeAll(molviews);
        
        if (not removed_views.isEmpty())
        {
            if (not mgroup.contains(molnum))
                this->removeFromIndex(mgnum, molnum);
        }
    }
}

/** Remove all copies of the views of the molecules in 'molecules'
    from the groups identified by 'mgid'
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::removeAll(const Molecules &molecules, const MGID &mgid)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    foreach (MGNum mgnum, mgnums)
    {
        MoleculeGroup &mgroup = *(mgroups.find(mgnum));
    
        QList<ViewsOfMol> removed_mols = mgroup.removeAll(molecules);
        
        QSet<MolNum> removed_molnums;
        
        foreach (const ViewsOfMol &removed_mol, removed_mols)
        {
            if (not mgroup.contains(removed_mol.number()))
                removed_molnums.insert(removed_mol.number());
        }
        
        this->removeFromIndex(mgnum,removed_molnums);
    }
}

/** Remove all of the views of the molecules in the group 'molgroup'
    from the groups identified by 'mgid'
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::removeAll(const MolGroup &molgroup, const MGID &mgid)
{
    this->removeAll(molgroup.molecules(), mgid);
}

/** Remove all views of the molecule with number 'molnum' from the 
    groups identified by 'mgid'. This does nothing to any groups
    that don't contain this molecule
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::remove(MolNum molnum, const MGID &mgid)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    foreach (MGNum mgnum, mgnums)
    {
        MoleculeGroup &mgroup = *(mgroups.find(mgnum));
    
        ViewsOfMol removed_views = mgroup.remove(molnum);
        
        if (not removed_views.isEmpty())
            this->removeFromIndex(molnum);
    }
}

/** Remove the molecules whose numbers are in 'molnums' from the 
    groups identified by 'mgid'
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::remove(const QSet<MolNum> &molnums, const MGID &mgid)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    foreach (MGNum mgnum, mgnums)
    {
        MoleculeGroup &mgroup = *(mgroups.find(mgnum));
    
        QList<ViewsOfMol> removed_mols = mgroup.remove(molnums);
        
        QSet<MolNum> removed_nums;
        
        foreach (const ViewsOfMol &removed_mol, removed_mols)
        {
            removed_nums.insert(removed_mol.number());
        }
        
        this->removeFromIndex(mgnum, removed_nums);
    }
}

/** Update all of the groups to use the version of the molecule
    present in 'moldata' */
void MolGroups::update(const MoleculeData &moldata)
{
    //get the current copy of the molecule...
    if (this->needToUpdate(moldata))
    {
        const QVector<MGNum> &mgnums = this->groupsContaining(moldata.number());
        
        foreach (MGNum mgnum, mgnums)
        {
            mgroups.find(mgnum)->update(moldata);
        }
    }
}

/** Update all of the groups to use the versions of the molecules
    held in 'molecules' */
void MolGroups::update(const Molecules &molecules)
{
    for (QHash<MGNum,MoleculeGroup>::iterator it = mgroups.begin();
         it != mgroups.end();
         ++it)
    {
        it->update(molecules);
    }
}

/** Update all of the groups to use the versions of the molecules
    help in the group 'molgroup' */
void MolGroups::update(const MolGroup &molgroup)
{
    QHash<MGNum,MoleculeGroup>::const_iterator 
                                  it = mgroups.constFind(molgroup.number());
                                  
    if (it != mgroups.constEnd() and it->majorVersion() == molgroup.majorVersion()
                                 and it->minorVersion() == molgroup.minorVersion())
    {
        //the group is the same as the existing one, and at the same
        //version, so nothing to do :-)
        return;
    }
    
    this->update(molgroup.molecules());
}

/** Set the contents of the groups identified by 'mgid' so that
    they only contain the view in 'molview'
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::setContents(const MGID &mgid, const MoleculeView &molview)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    PartialMolecule view(molview);
    view.update( this->matchToExistingVersion(view.data()) );

    foreach (MGNum mgnum, mgnums)
    {
        if (molgroup.find(mgnum)->setContents(view))
        {
            this->clearIndex(mgnum);
            this->addToIndex(mgnum, view.number());
        }
    }
}

/** Set the contents of the groups identified by 'mgid' so that
    they only contain the views of the molecule in 'molviews'
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::setContents(const MGID &mgid, const ViewsOfMol &molviews)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    ViewsOfMol views(molviews);
    views.update( this->matchToExistingVersion(views.data()) );
    
    foreach (MGNum mgnum, mgnums)
    {
        if (molgroup.find(mgnum)->setContents(views))
        {
            this->clearIndex(mgnum);
            this->addToIndex(mgnum, views.number());
        }
    }
}

/** Set the contents of the groups identified by 'mgid' so that
    they contain only the views of the molecules contained in 'molecules'
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::setContents(const MGID &mgid, const Molecules &molecules)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    Molecules mols = this->matchToExistingVersion(molecules);
    
    QSet<MolNum> molnums = mols.molNums();
    
    foreach (MGNum mgnum, mgnums)
    {
        if (molgroup.find(mgnum)->setContents(mols))
        {
            this->clearIndex(mgnum);
            this->addToIndex(mgnum,molnums);
        }
    }
}

/** Set the contents of the groups identified by 'mgid' so that
    they contain the same views of the same molecules in the 
    same order as in the group 'molgroup'
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void MolGroups::setContents(const MGID &mgid, const MolGroup &molgroup)
{
    QList<MGNum> mgnums = this->map(mgid);
    
    QHash<MGNum,MoleculeGroup>::const_iterator 
                                  it = mgroups.constFind(molgroup.number());
                                  
    if (it != mgroups.constEnd() and it->majorVersion() == molgroup.majorVersion()
                                 and it->minorVersion() == molgroup.minorVersion())
    {
        //the group is the same as the existing one, and at the same
        //version, so nothing to do :-)
        return;
    }

    //ok we need to set the contents, but not necessarily at the same
    //versions... (we need to update this group to use the versions
    //of any molecules already present in this group)
    Molecules mols = this->matchToExistingVersion(molgroup.molecules());
    
    QSet<MolNum> molnums = mols.molNums();
    
    foreach (MGNum mgnum, mgnums)
    {
        //try to set the contents using the wrong molecule version...
        MolGroup &group = *(molgroups.find(mgnum));
        
        if (group.setContents(molgroup))
        {
            //ok, we need to update the group to ensure that it uses
            //the same version of molecules as are present in the 
            //rest of the set
            group.update(mols);
            
            //now update the index
            this->clearIndex(mgnum);
            this->addToIndex(mgnum, molnums);
        }
    }
}

/** Protected function used to return a modifiable reference to the 
    group with number 'mgnum'
    
    \throw SireMol::missing_group
*/
MolGroup& MolGroups::getGroup(MGNum mgnum)
{
    QHash<MGNum,MoleculeGroup>::iterator it = mgroups.find(mgnum);
    
    if (it == mgroups.end())
        throw SireMol::missing_group( QObject::tr(
            "Cannot find the MoleculeGroup with number %1. Available "
            "groups are %2")
                .arg(mgnum).arg(Sire::toString(mgroups.keys())), CODELOC );

    return it->base();
}

/** Protected function used to return modifiable pointers to the
    groups whose numbers are in 'mgnums'
    
    \throw SireMol::missing_group
*/
void MolGroups::getGroups(const QList<MGNum> &mgnums,
                          QVarLengthArray<MolGroup*,10> &groups)
{
    groups.clear();
    
    QHash<MGNum,MoleculeGroup>::iterator it;
    
    foreach (MGNum mgnum, mgnums)
    {
        it = mgroups.find(mgnum);
        
        if (it == mgroups.end())
            throw SireMol::missing_group( QObject::tr(
                "Cannot find the MoleculeGroup with number %1. Available "
                "groups are %2")
                    .arg(mgnum).arg(Sire::toString(mgroups.keys())), CODELOC );
        
    
        groups.append( &(it->base()) );
    }
}

/** Protected function used to return a hash of modifiable 
    pointers to all of the groups in this set. */
QHash<MGNum,MolGroup*> MolGroups::getGroups()
{
    QHash<MGNum,MolGroup*> groups;
    
    groups.reserve(mgroups.count());
    
    for (QHash<MGNum,MoleculeGroup>::iterator it = mgroups.begin();
         it != mgroups.end();
         ++it)
    {
        groups.insert( it.key(), &(it->base()) );
    }
    
    return groups;
}

/** Protected function used to return a hash of modifiable 
    pointers to all of the groups in this set. */
QHash<MGNum,const MolGroup*> MolGroups::getGroups() const
{
    QHash<MGNum,const MolGroup*> groups;
    
    groups.reserve(mgroups.count());
    
    for (QHash<MGNum,MoleculeGroup>::const_iterator it = mgroups.begin();
         it != mgroups.end();
         ++it)
    {
        groups.insert( it.key(), &(it.base()) );
    }
    
    return groups;
}
