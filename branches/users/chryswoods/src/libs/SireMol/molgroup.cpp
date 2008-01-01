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

#include <QVector>

#include "molgroup.h"
#include "molnum.h"
#include "partialmolecule.h"
#include "molecule.h"

#include "mover.hpp"
#include "editor.hpp"

#include "mgname.h"
#include "mgnum.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;
using namespace SireBase;

typedef boost::tuple<MolNum,quint32> MolNumUInt;

bool operator==(const MolNumUInt &i0, const MolNumUInt &i1)
{
    return i0.get<0>() == i1.get<0>() and 
           i0.get<1>() == i1.get<1>();
}

static QDataStream& operator<<(QDataStream &ds, 
                               const MolNumUInt &molviewidx)
{
    ds << molviewidx.get<0>() << molviewidx.get<1>();
    return ds;
}

static QDataStream& operator>>(QDataStream &ds, 
                               MolNumUInt &molviewidx)
{
    MolNum molnum;
    quint32 viewidx;
    
    ds >> molnum >> viewidx;

    molviewidx = MolNumUInt(molnum,viewidx);
    
    return ds;
}

////////////
//////////// Implementation of MolGroupPvt
////////////

namespace SireMol
{
namespace detail
{

class MolGroupPvt : public QSharedData
{
public:
    MolGroupPvt();
    
    MolGroupPvt(const QString &name);
    MolGroupPvt(const QString &name, const MolGroupPvt &other);
    
    MolGroupPvt(const MolGroupPvt &other);
    
    ~MolGroupPvt();

    MolGroupPvt& operator=(const MolGroupPvt &other);
    
    bool operator==(const MolGroupPvt &other) const;
    bool operator!=(const MolGroupPvt &other) const;
    
    void incrementMajor();
    void incrementMinor();
    
    Molecules molecules;
    
    QVector<MolNum> molidx_to_num;
    QVector<MolNumUInt> molviewidx_to_num;
    
    MGName name;
    MGNum number;
    
    quint64 major_version;
    quint64 minor_version;
};

} // end of namespace detail
} // end of namespace SireMol;

using namespace SireMol::detail;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, 
                                       const MolGroupPvt &molgrouppvt)
{
    SharedDataStream sds(ds);
    
    sds << molgrouppvt.molecules
        << molgrouppvt.molidx_to_num
        << molgrouppvt.molviewidx_to_num
        << molgrouppvt.name
        << molgrouppvt.number
        << molgrouppvt.major_version
        << molgrouppvt.minor_version;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,     
                                       MolGroupPvt &molgrouppvt)
{
    SharedDataStream sds(ds);
    
    sds >> molgrouppvt.molecules
        >> molgrouppvt.molidx_to_num
        >> molgrouppvt.molviewidx_to_num
        >> molgrouppvt.name
        >> molgrouppvt.number
        >> molgrouppvt.major_version
        >> molgrouppvt.minor_version;
    
    return ds;
}

static QSharedDataPointer<MolGroupPvt> shared_null;

static const QSharedDataPointer<MolGroupPvt>& getSharedNull()
{
    if (shared_null.constData() == 0)
        shared_null = new MolGroupPvt();
        
    return shared_null;
}

/** Construct an empty, unnamed MolGroupPvt */
MolGroupPvt::MolGroupPvt() 
            : QSharedData(),
              name("unnamed"),
              number( MGNum::getUniqueNumber() ),
              major_version(0),
              minor_version(0)
{}

/** Construct an empty, named MolGroupPvt */
MolGroupPvt::MolGroupPvt(const QString &nme)
            : QSharedData(),
              name(nme),
              number( MGNum::getUniqueNumber() ),
              major_version(0),
              minor_version(0)
{}

/** Construct a named group that contains the same molecules as 'other' */
MolGroupPvt::MolGroupPvt(const QString &nme, 
                         const MolGroupPvt &other)
            : QSharedData(),
              molecules(other.molecules),
              molidx_to_num(other.molidx_to_num),
              molviewidx_to_num(other.molviewidx_to_num),
              name(nme),
              number( MGNum::getUniqueNumber() ),
              major_version(0),
              minor_version(0)
{}

/** Copy constructor */
MolGroupPvt::MolGroupPvt(const MolGroupPvt &other)
            : QSharedData(),
              molecules(other.molecules),
              molidx_to_num(other.molidx_to_num),
              molviewidx_to_num(other.molviewidx_to_num),
              name(other.name),
              number(other.number),
              major_version(other.major_version),
              minor_version(other.minor_version)
{}
                   
/** Destructor */
MolGroupPvt::~MolGroupPvt()
{}

/** Copy assignment operator */
MolGroupPvt& MolGroupPvt::operator=(const MolGroupPvt &other)
{
    if (this != &other)
    {
        molecules = other.molecules;
        molidx_to_num = other.molidx_to_num;
        molviewidx_to_num = other.molviewidx_to_num;
        name = other.name;
        number = other.number;
        major_version = other.major_version;
        minor_version = other.minor_version;
    }
    
    return *this;
}

bool MolGroupPvt::operator==(const MolGroupPvt &other) const
{
    return number == other.number and
           major_version == other.major_version and
           minor_version == other.minor_version;
}

bool MolGroupPvt::operator!=(const MolGroupPvt &other) const
{
    return number != other.number or
           major_version != other.major_version or
           minor_version != other.minor_version;
}

void MolGroupPvt::incrementMajor()
{
    throw SireError::incomplete_code("Unimplemented", CODELOC);
}

void MolGroupPvt::incrementMinor()
{
    throw SireError::incomplete_code("Unimplemented", CODELOC);
}

////////////
//////////// Implementation of MolGroup
////////////

/** Default constructor */
MolGroup::MolGroup() 
         : ConcreteProperty<MolGroup,PropertyBase>(),
           d( getSharedNull() )
{}

/** Construct a group that holds the passed molecules */
MolGroup::MolGroup(const Molecules &molecules)
         : ConcreteProperty<MolGroup,PropertyBase>(),
           d( getSharedNull() )
{
    this->add(molecules);
}

/** Construct an empty, but named, group */
MolGroup::MolGroup(const QString &name)
         : ConcreteProperty<MolGroup,PropertyBase>(),
           d( new MolGroupPvt(name) )
{}

/** Construct a named group that contains the passed molecules */
MolGroup::MolGroup(const QString &name, const Molecules &molecules)
         : ConcreteProperty<MolGroup,PropertyBase>(),
           d( new MolGroupPvt(name) )
{
    this->add(molecules);
}
  
/** Construct a named group that contains the same molecules as 'other' */       
MolGroup::MolGroup(const QString &name, const MolGroup &other)
         : ConcreteProperty<MolGroup,PropertyBase>()
{
    if (name == other.name())
    {
        d = other.d;
    }
    else
        d = new MolGroupPvt(name, *(other.d));
}

/** Copy constructor */
MolGroup::MolGroup(const MolGroup &other)
         : ConcreteProperty<MolGroup,PropertyBase>(),
           d(other.d)
{}

/** Destructor */
MolGroup::~MolGroup()
{}

/** Comparison operator */
bool MolGroup::operator==(const MolGroup &other) const
{
    return d == other.d or *d == *(other.d);
}

/** Comparison operator */
bool MolGroup::operator!=(const MolGroup &other) const
{
    return d != other.d and *d != *(other.d);
}

/** Return the views of the molecule with number 'molnum' from this group

    \throw SireMol::missing_molecule
*/
const ViewsOfMol& MolGroup::operator[](MolNum molnum) const
{
    return d->molecules.at(molnum);
}

/** Return the specified view of the specified molecule...

    \throw SireMol::missing_molecule
    \throw SireError::invalid_index
*/
PartialMolecule MolGroup::operator[](const boost::tuple<MolNum,Index> &viewidx) const
{
    return d->molecules.at(viewidx);
}

/** Add the views of the molecules in 'molecules' to this group. This
    adds duplicates of any views that already exist in this group. */
MolGroup& MolGroup::operator+=(const Molecules &molecules)
{
    this->add(molecules);
    return *this;
}

/** Remove the views of the molecules in 'molecules' from this
    group. This only removes the first copy of any duplicated
    views that exist in this group. */
MolGroup& MolGroup::operator-=(const Molecules &molecules)
{
    this->remove(molecules);
    return *this;
}

/** Return the views of the molecule with number 'molnum' from this group.

    \throw SireMol::missing_molecule
*/
const ViewsOfMol& MolGroup::at(MolNum molnum) const
{
    return d->molecules.at(molnum);
}

/** Return the specified view of the specified molecule in this group.

    \throw SireMol::missing_molecule
    \throw SireError::invalid_index
*/
PartialMolecule MolGroup::at(const boost::tuple<MolNum,Index> &viewidx) const
{
    return d->molecules.at(viewidx);
}

/** Return the specified view of the specified molecule in this group.

    \throw SireMol::missing_molecule
    \throw SireError::invalid_index
*/
PartialMolecule MolGroup::at(MolNum molnum, int viewidx) const
{
    return d->molecules.at(molnum, viewidx);
}

/** Return the views of the molecule at index 'idx' in this group.

    \throw SireError::invalid_index
*/
const ViewsOfMol& MolGroup::moleculeAt(int idx) const
{
    return d->molecules.at( d->molidx_to_num.constData()
                            [Index(idx).map(d->molidx_to_num.count())] );
}

/** Return the view of the molecule at index 'idx' in this group.

    \throw SireError::invalid_index
*/
PartialMolecule MolGroup::viewAt(int idx) const
{
    idx = Index(idx).map(d->molviewidx_to_num.count());
    
    const MolNumUInt &molviewidx = d->molviewidx_to_num.constData()[idx];
    
    return d->molecules.at( molviewidx.get<0>(), molviewidx.get<1>() );
}

/** Return the views of the molecule with number 'molnum' from
    this group
    
    \throw SireMol::missing_molecule
*/
const ViewsOfMol& MolGroup::molecule(MolNum molnum) const
{
    return d->molecules.at(molnum);
}

/** Return whether or not this group contains any views of the 
    molecule with number 'molnum' */
bool MolGroup::contains(MolNum molnum) const
{
    return d->molecules.contains(molnum);
}

/** Return whether or not this group contains any version of 
    the view of the molecule in 'molview' */
bool MolGroup::contains(const MoleculeView &molview) const
{
    return d->molecules.contains(molview);
}

/** Return whether or not this group contains all of the views
    of any version of the molecule in 'molviews' */
bool MolGroup::contains(const ViewsOfMol &molviews) const
{
    return d->molecules.contains(molviews);
}

/** Return whether or not this group contains all of the 
    views of any version of all of the molecules contained
    in 'molecules' */
bool MolGroup::contains(const Molecules &molecules) const
{
    return d->molecules.contains(molecules);
}

/** Return whether or not this group contains all of the 
    views of any version of all of the molecules contained
    in the group 'other' */
bool MolGroup::contains(const MolGroup &other) const
{
    return this->contains(other.molecules());
}

/** Return whether or not this group contains any version 
    of any of the atoms of the molecule in 'molview' */
bool MolGroup::intersects(const MoleculeView &molview) const
{
    return d->molecules.intersects(molview);
}

/** Return whether or not this group contains any version
    of any of the atoms in any of the molecules in 'molecules' */
bool MolGroup::intersects(const Molecules &other) const
{
    return d->molecules.intersects(other);
}

/** Return whether or not this group contains any version
    of any of the atoms in any of the molecules contained in 
    the group 'other' */
bool MolGroup::intersects(const MolGroup &other) const
{
    return this->intersects(other.molecules());
}

/** Return the number of molecules in this group */
int MolGroup::nMolecules() const
{
    return d->molidx_to_num.count();
}

/** Return the number of views of molecules in this group - 
    this must always be greater or equal to the number of 
    molecules! */
int MolGroup::nViews() const
{
    return d->molviewidx_to_num.count();
}

/** Return the number of views of the molecule with number 'molnum'
    that are present in this group.
    
    \throw SireMol::missing_molecule
*/
int MolGroup::nViews(MolNum molnum) const
{
    return d->molecules.nViews(molnum);
}

/** Return the number of views of the molecule at index 'idx'
    in this group */
int MolGroup::nViews(Index idx) const
{
    return this->nViews( d->molidx_to_num.constData()
                             [idx.map(d->molidx_to_num.count())] );
}

/** Return whether or not this group is empty */
bool MolGroup::isEmpty() const
{
    return d->molecules.isEmpty();
}

/** Return all views of all of the molecules in this group */
const Molecules& MolGroup::molecules() const
{
    return d->molecules;
}

/** Return a reference to the first molecule in the group 

    \throw SireError::invalid_index
*/
const ViewsOfMol& MolGroup::first() const
{
    return d->molecules.first();
}

/** Return a reference to the last molecule in the group

    \throw SireError::invalid_index
*/
const ViewsOfMol& MolGroup::last() const
{
    return d->molecules.last();
}

/** Return a reference to the first molecule in the group

    \throw SireError::invalid_index
*/
const ViewsOfMol& MolGroup::front() const
{
    return d->molecules.front();
}

/** Return a reference to the last molecule in the group 

    \throw SireError::invalid_index
*/
const ViewsOfMol& MolGroup::back() const
{
    return d->molecules.back();
}

/** Return an iterator pointing to the first molecule
    in the group */
MolGroup::const_iterator MolGroup::begin() const
{
    return d->molecules.begin();
}

/** Return an iterator pointing one space after the last
    molecule in the group */
MolGroup::const_iterator MolGroup::end() const
{
    return d->molecules.end();
}

/** Return an iterator pointing to the first molecule
    in the group */
MolGroup::const_iterator MolGroup::constBegin() const
{
    return d->molecules.constBegin();
}

/** Return an iterator pointing to one space after
    the last molecule in the group */
MolGroup::const_iterator MolGroup::constEnd() const
{
    return d->molecules.constEnd();
}

/** Return an iterator pointing to the molecule with 
    number 'molnum'. If there is no such molecule then
    this returns MolGroup::end() */
MolGroup::const_iterator MolGroup::find(MolNum molnum) const
{
    return d->molecules.find(molnum);
}

/** Return an iterator pointing to the molecule with 
    number 'molnum'. If there is no such molecule then
    this returns MolGroup::end() */
MolGroup::const_iterator MolGroup::constFind(MolNum molnum) const
{
    return d->molecules.constFind(molnum);
}

/** Return the numbers of all molecules present in this group */
QSet<MolNum> MolGroup::molNums() const
{
    return d->molecules.molNums();
}

/** Assert that this group contains a view of any part of the 
    molecule with number 'molnum'
    
    \throw SireMol::missing_molecule
*/
void MolGroup::assertContains(MolNum molnum) const
{
    d->molecules.assertContains(molnum);
}

/** Return the name of this group */
const MGName& MolGroup::name() const
{
    return d->name;
}

/** Return the ID number of this group */
MGNum MolGroup::number() const
{
    return d->number;
}

/** Return the major version number of this group. This number
    changes whenever views are added or removed from this group */
quint64 MolGroup::majorVersion() const
{
    return d->major_version;
}

/** Return the minor version number of this group. This number
    changes whenever any of the versions of molecules in this group
    are changed. This number is reset to zero whenever the major
    version number of this group is changed. */
quint64 MolGroup::minorVersion() const
{
    return d->minor_version;
}

/** Add the view of the molecule in 'molview' to this group. 
    This adds the view as a duplicate if it already exists 
    in this group */
void MolGroup::add(const MoleculeView &molview)
{
    if (molview.selection().isEmpty())
        return;

    MolNum molnum = molview.data().number();

    MolGroupPvt &dref = *d;

    if (not dref.molecules.contains(molnum))
        dref.molidx_to_num.append(molnum);
    
    dref.molecules.add(molview);
   
    dref.molviewidx_to_num.append( MolNumUInt(molnum,
                                         dref.molecules.nViews(molnum) - 1) );

    dref.incrementMajor();
}

/** Add the views of the molecule in 'molviews' to this group.
    This adds the views as duplicates if they already exist
    in this group */
void MolGroup::add(const ViewsOfMol &molviews)
{
    if (molviews.isEmpty())
        return;
    else if (molviews.nViews() == 1)
    {
        this->add(molviews.at(0));
        return;
    }

    MolNum molnum = molviews.number();
    
    MolGroupPvt &dref = *d;
    
    if (not dref.molecules.contains(molnum))
        dref.molidx_to_num.append(molnum);
        
    dref.molecules.add(molviews);
    
    quint32 nviews = dref.molecules.nViews(molnum);
    
    for (quint32 i = nviews - molviews.count() - 1; i < nviews; ++i)
    {
        dref.molviewidx_to_num.append( MolNumUInt(molnum,i) );
    }
    
    dref.incrementMajor();
}

/** Add all of the molecules in 'molecules' to this group.
    This duplicates any molecules that already exist in this 
    group. */
void MolGroup::add(const Molecules &molecules)
{
    if (molecules.isEmpty())
        return;
    else if (molecules.count() == 1)
    {
        this->add(molecules.first());
        return;
    }

    MolGroupPvt &dref = *d;
    
    //add the molecules to the index
    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        MolNum molnum = it.key();
        
        if (not dref.molecules.contains(molnum))
            dref.molidx_to_num.append(molnum);
            
        quint32 nviews = dref.molecules.nViews(molnum);
        quint32 n_newviews = it->nViews();
        
        for (quint32 i=nviews; i<nviews + n_newviews; ++i)
        {
            dref.molviewidx_to_num.append( MolNumUInt(molnum,i) );
        }
    }

    //now add the molecules themselves
    dref.molecules.add(molecules);
    
    dref.incrementMajor();
}

/** Add the molecules in 'molgroup' to this set. This adds the 
    molecules and views in the same order as they appear in 
    'molgroup', adding them as duplicates if they already
    exist in this set. Note that the version of the molecule
    will be taken from this set. */
void MolGroup::add(const MolGroup &molgroup)
{
    if (molgroup.isEmpty())
        return;
    
    MolGroupPvt &dref = *d;
    
    if (this->isEmpty())
    {
        dref.molecules = molgroup.d->molecules;
        dref.molidx_to_num = molgroup.d->molidx_to_num;
        dref.molviewidx_to_num = molgroup.d->molviewidx_to_num;
        
        dref.incrementMajor();
        return;
    }
    
    //append the other group's index onto this group
    foreach (MolNum molnum, molgroup.d->molidx_to_num)
    {
        if (not dref.molecules.contains(molnum))
            dref.molidx_to_num.append(molnum);
    }
    
    foreach (const MolNumUInt &molviewidx,
             molgroup.d->molviewidx_to_num)
    {
        MolNum molnum = molviewidx.get<0>();
    
        dref.molviewidx_to_num.append( MolNumUInt( molnum,
                          molviewidx.get<1>() + dref.molecules.nViews(molnum) ) );
    }
    
    //now add the molecules themselves to this set
    dref.molecules.add(molgroup.d->molecules);
    
    dref.incrementMajor();
}

/** Add the view of the molecule in 'molview' to this group. 
    This only adds the view if it does not already exist in 
    this group, and returns whether or not the view was added */
bool MolGroup::addIfUnique(const MoleculeView &molview)
{
    if (molview.selection().isEmpty())
        return false;

    MolNum molnum = molview.data().number();

    MolGroupPvt &dref = *d;
    
    bool hasmol = dref.molecules.contains(molnum);
    
    if (dref.molecules.addIfUnique(molview))
    {
        //the view has been added!
        if (not hasmol)
            dref.molidx_to_num.append(molnum);
            
        dref.molviewidx_to_num.append( MolNumUInt(molnum,
                                        dref.molecules.nViews(molnum) - 1) );
    
        dref.incrementMajor();
        return true;
    }
    else
        return false;
}

/** Add the views of the molecule 'molviews' to this group,
    adding the views only if they don't already exist in this
    group. This returns the views that were added successfully
    to this group. */
ViewsOfMol MolGroup::addIfUnique(const ViewsOfMol &molviews)
{
    if (molviews.isEmpty())
        return ViewsOfMol();
        
    MolNum molnum = molviews.number();
    
    MolGroupPvt &dref = *d;
    
    bool hasmol = dref.molecules.contains(molnum);
    
    ViewsOfMol added_views = dref.molecules.addIfUnique(molviews);
    
    if (not added_views.isEmpty())
    {
        if (not hasmol)
            dref.molidx_to_num.append(molnum);
            
        quint32 nviews = dref.molecules.nViews(molnum);
        quint32 nadded = added_views.nViews();
        
        for (quint32 i=nviews-nadded-1; i<nviews; ++i)
        {
            dref.molviewidx_to_num.append( MolNumUInt(molnum,i) );
        }
        
        dref.incrementMajor();
    }
    
    return added_views;
}

/** Add the views of the molecules in 'molecules' to this group. This
    only adds views that don't already exist in this group. This
    returns all of the views that were successfully added. */
QList<ViewsOfMol> MolGroup::addIfUnique(const Molecules &molecules)
{
    if (molecules.isEmpty())
        return QList<ViewsOfMol>();
        
    MolGroupPvt &dref = *d;

    //which molecules already exist in this group?
    QSet<MolNum> groupmols = dref.molecules.molNums();
    
    //add the molecules to this group
    QList<ViewsOfMol> added_mols = dref.molecules.addIfUnique(molecules);
    
    //now update the index
    if (not added_mols.isEmpty())
    {
        foreach (const ViewsOfMol &added_mol, added_mols)
        {
            MolNum molnum = added_mol.number();
            
            if (not groupmols.contains(molnum))
                dref.molidx_to_num.append(molnum);
                
            quint32 nviews = dref.molecules.nViews(molnum);
            quint32 nadded = added_mol.nViews();
            
            for (quint32 i=nviews-nadded-1; i<nviews; ++i)
            {
                dref.molviewidx_to_num.append( MolNumUInt(molnum,i) );
            }
        }
        
        dref.incrementMajor();
    }

    return added_mols;
}

/** Add the views/molecules in 'molgroup' to this group, but
    only if they don't already exist in this group. This has
    the same action as MolGroup::addIfUnique(molecules), but
    it ensures that the added views are in the same order as
    in 'molgroup'. This is costly, so if you don't care
    about the added order, then use 
    MolGroup::addIfUnique(molgroup.molecules()) instead. 
    
    This returns the added views.
*/
QList<ViewsOfMol> MolGroup::addIfUnique(const MolGroup &molgroup)
{
    QList<ViewsOfMol> added_views;

    if (molgroup.isEmpty())
        return added_views;
        
    MolGroupPvt &dref = *d;
    
    foreach (const MolNumUInt &molviewidx,
             molgroup.d->molviewidx_to_num)
    {
        MolNum molnum = molviewidx.get<0>();
        quint32 i = molviewidx.get<1>();
    
        PartialMolecule molview = molgroup[molnum][Index(i)];
        
        if (dref.molecules.addIfUnique(molview))
            added_views.append(molview);
    }
    
    if (not added_views.isEmpty())
        dref.incrementMajor();
        
    return added_views;
}

/** Synonym for MolGroup::addIfUnique(molview) */
bool MolGroup::unite(const MoleculeView &molview)
{
    return this->addIfUnique(molview);
}

/** Synonym for MolGroup::addIfUnique(molviews) */
ViewsOfMol MolGroup::unite(const ViewsOfMol &molviews)
{
    return this->addIfUnique(molviews);
}

/** Synonym for MolGroup::addIfUnique(molecules) */
QList<ViewsOfMol> MolGroup::unite(const Molecules &molecules)
{
    return this->addIfUnique(molecules);
}

/** Synonym for MolGroup::addIfUnique(molgroup). The 
    function MolGroup::addIfUnique(molgroup.molecules()) is
    quicker if you don't care about the order in which
    the views are added. */
QList<ViewsOfMol> MolGroup::unite(const MolGroup &molgroup)
{
    return this->addIfUnique(molgroup);
}

bool MolGroup::_pvt_remove(const MoleculeView &molview)
{
    AtomSelection selected_atoms = molview.selection();

    if (selected_atoms.isEmpty())
        return false;

    MolNum molnum = molview.data().number();

    Molecules::const_iterator it = d.constData()->molecules.find(molnum);
    
    if (it == d.constData()->molecules.end())
        return false;
        
    int viewidx = it->indexOf(selected_atoms);
    
    if (viewidx == -1)
        //this view is not present in this group
        return false;
    
    //the view is present - remove it!    
    MolGroupPvt &dref = *d;
    
    //remove all of the views of this molecule...
    ViewsOfMol molviews = dref.molecules.remove(molview.data().number());
    
    //remove the specified view from this set...
    molviews.removeAt(viewidx);
    
    //if there are any views left then add them back
    //to the collection of molecules
    if (not molviews.isEmpty())
    {
        dref.molecules.add(molviews);
        
        //the molecule has only been partially removed,
        //so remove this view from the molview index
        QMutableVectorIterator< MolNumUInt > it(dref.molviewidx_to_num);
        
        while (it.hasNext())
        {
            MolNumUInt &molviewidx = it.next();
            
            if (molviewidx.get<0>() == molnum)
            {
                if (molviewidx.get<1>() == (uint)viewidx)
                    it.remove();
                else if (molviewidx.get<1>() > (uint)viewidx)
                    molviewidx = MolNumUInt(molnum,molviewidx.get<1>()-1);
            }
        }
    }
    else
    {
        //this molecule has been completely removed...
        //remove it completely from the index
        dref.molidx_to_num.remove(molnum);
        
        QMutableVectorIterator< MolNumUInt > it(dref.molviewidx_to_num);
        
        while (it.hasNext())
        {
            const MolNumUInt &molviewidx = it.next();
        
            if (molviewidx.get<0>() == molnum)
                it.remove();
        }
    }
    
    return true;
}

/** Remove the view of the molecule in 'molview' from this set.
    This only removes the first such view from the set, and 
    returns whether or not any view was removed */
bool MolGroup::remove(const MoleculeView &molview)
{
    if (this->_pvt_remove(molview))
    {
        d->incrementMajor();
        return true;
    }
    else
        return false;
}

ViewsOfMol MolGroup::_pvt_remove(const ViewsOfMol &molviews)
{
    if (molviews.isEmpty())
        return molviews;
        
    int nviews = molviews.nViews();
    
    QList<AtomSelection> removed_views;
    
    for (int i=0; i<nviews; ++i)
    {
        PartialMolecule view = molviews.at(i);
    
        if (this->_pvt_remove(view))
        {
            removed_views.append(view.selection());
        }
    }
    
    if (removed_views.isEmpty())
        return ViewsOfMol();
    else
        return ViewsOfMol(molviews.data(), removed_views);
}

/** Remove all of the views of the molecule in 'molviews' from this
    set. This only removes the first such view of any duplicates
    from this set, and returns the views that were removed */
ViewsOfMol MolGroup::remove(const ViewsOfMol &molviews)
{
    ViewsOfMol removed_views = this->_pvt_remove(molviews);
    
    if (not removed_views.isEmpty())
        d->incrementMajor();
        
    return removed_views;
}

/** Remove all of the molecules listed in 'molecules' from this set. 
    This only removes the first of any duplicated views in this set.
    This returns the views/molecules that were successfully removed. */
QList<ViewsOfMol> MolGroup::remove(const Molecules &molecules)
{
    QList<ViewsOfMol> removed_mols;
    
    for (Molecules::const_iterator it = molecules.begin();
         it != molecules.end();
         ++it)
    {
        ViewsOfMol removed_views = this->_pvt_remove(*it);
        
        if (not removed_views.isEmpty())
            removed_mols.append(removed_views);
    }
    
    if (not removed_mols.isEmpty())
        d->incrementMajor();
    
    return removed_mols;
}

/** Remove all of the molecules from the group 'molgroup' from this set.
    This only removes the first of any duplicated views in this set.
    This returns the views/molecules that were sucessfully removed. */
QList<ViewsOfMol> MolGroup::remove(const MolGroup &molgroup)
{
    return this->remove(molgroup.molecules());
}

bool MolGroup::_pvt_removeAll(const MoleculeView &molview)
{
    //just keep removing it until the view has gone completely!
    bool removed_a_view = false;
    
    while (this->_pvt_remove(molview))
    {
        removed_a_view = true;
    }
    
    return removed_a_view;
}

/** Remove all copies of the view of the molecule in 'molview' from this
    group. This removes all copies if this view is duplicated in this
    group, and returns whether or not any views were removed. */
bool MolGroup::removeAll(const MoleculeView &molview)
{
    if (this->_pvt_removeAll(molview))
    {
        d->incrementMajor();
        return true;
    }
    else
        return false;
}

ViewsOfMol MolGroup::_pvt_removeAll(const ViewsOfMol &molviews)
{
    if (molviews.isEmpty())
        return molviews;
        
    QList<AtomSelection> removed_views;
    
    int nviews = molviews.nViews();
    
    for (int i=0; i<nviews; ++i)
    {
        PartialMolecule view = molviews.at(i);
        
        if (this->_pvt_removeAll(view))
            removed_views.append(view.selection());
    }
    
    if (removed_views.isEmpty())
        return ViewsOfMol();
    else
        return ViewsOfMol(molviews.data(), removed_views);
}

/** Remove all copies of all of the views of the molecule in 'molviews'.
    This removes all copies of any duplicated views in this group,
    and returns the views that were successfully removed. */
ViewsOfMol MolGroup::removeAll(const ViewsOfMol &molviews)
{
    ViewsOfMol removed_views = this->_pvt_remove(molviews);
    
    if (not removed_views.isEmpty())
        d->incrementMajor();
        
    return removed_views;
}

/** Remove all copies of all of the views of the molecules in 'molecules'.
    This removes all copies of any duplicated views in this group.
    This returns the molecules/views that were removed. */
QList<ViewsOfMol> MolGroup::removeAll(const Molecules &molecules)
{
    QList<ViewsOfMol> removed_mols;
    
    for (Molecules::const_iterator it = molecules.begin();
         it != molecules.end();
         ++it)
    {
        ViewsOfMol removed_views = this->_pvt_removeAll(*it);
        
        if (not removed_views.isEmpty())
            removed_mols.append(removed_views);
    }
    
    if (not removed_mols.isEmpty())
        d->incrementMajor();
    
    return removed_mols;
}

/** Remove all copies of all of the views of the molecules in the
    group 'molgroup'. This removes all copies of any duplicated 
    views in this group. This returns the molecules/views that
    were removed
*/
QList<ViewsOfMol> MolGroup::removeAll(const MolGroup &molgroup)
{
    return this->removeAll(molgroup.molecules());
}

ViewsOfMol MolGroup::_pvt_remove(MolNum molnum)
{
    if (not this->contains(molnum))
        return ViewsOfMol();

    MolGroupPvt &dref = *d;

    //remove the molecule
    ViewsOfMol removed_views = dref.molecules.remove(molnum);
    
    //now remove it from the index
    dref.molidx_to_num.remove(molnum);
    
    QMutableVectorIterator< MolNumUInt > it(dref.molviewidx_to_num);
    
    while (it.hasNext())
    {
        const MolNumUInt &molviewidx = it.next();
        
        if (molviewidx.get<0>() == molnum)
            it.remove();
    }
    
    return removed_views;
}

/** Completely remove all views of the molecule with number 'molnum'
    from this group. This returns the views that were removed */
ViewsOfMol MolGroup::remove(MolNum molnum)
{
    ViewsOfMol removed_views = this->_pvt_remove(molnum);
    
    if (not removed_views.isEmpty())
        d->incrementMajor();
        
    return removed_views;
}

/** Remove all views of the molecules whose numbers are in 'molnums'.
    This returns the views that were removed. */
QList<ViewsOfMol> MolGroup::remove(const QSet<MolNum> &molnums)
{
    QList<ViewsOfMol> removed_mols;

    foreach (MolNum molnum, molnums)
    {
        ViewsOfMol removed_views = this->_pvt_remove(molnum);
        
        if (not removed_views.isEmpty())
            removed_mols.append(removed_views);
    }
    
    if (not removed_mols.isEmpty())
        d->incrementMajor();
    
    return removed_mols;
}

/** Remove all of the molecules from this group */
void MolGroup::removeAll()
{
    if (not this->isEmpty())
    {
        MolGroupPvt &dref = *d;
        
        dref.molecules.clear();
        dref.molidx_to_num.clear();
        dref.molviewidx_to_num.clear();
        
        dref.incrementMajor();
    }
}

/** Update this group so that the molecule in this group whose 
    data is in 'moldata' is also at the same version as 'moldata'.
    
    This does nothing if there is no such molecule in this 
    group, or if it is already at this version, and this returns
    whether or not this changes the group. */
bool MolGroup::update(const MoleculeData &moldata)
{
    MolGroupPvt &dref = *d;
    
    if (dref.molecules.update(moldata))
    {
        dref.incrementMinor();
        return true;
    }
    else
        return false;
}

/** Update this group so that the molecule in this group that
    is also viewed in 'molview' is updated to the same
    version as 'molview'.
    
    This does nothing if there is no such molecule in this 
    group, or if it is already at this version, and this returns
    whether or not this changes the group. */
bool MolGroup::update(const MoleculeView &molview)
{
    return this->update(molview.data());
}

/** Update this group so that the contained molecules have the 
    same versions as the molecules in 'molecules'. This does
    nothing if none of these molecules are in this group, or
    if they are already at the same versions. This returns
    the list of molecules that were changed by this update. */
QList<Molecule> MolGroup::update(const Molecules &molecules)
{
    MolGroupPvt &dref = *d;
    
    QList<Molecule> changed_mols = dref.molecules.update(molecules);
    
    if (not changed_mols.isEmpty())
        dref.incrementMinor();
        
    return changed_mols;
}

/** Update this group so that it has the same version of molecules
    as those in 'molgroup'. This does nothing if this group
    doesn't contain any of the molecules in 'molgroup', or
    if it already has the molecules at the same version. This
    returns the list of molecules that were changed by this
    update
*/
QList<Molecule> MolGroup::update(const MolGroup &molgroup)
{
    return this->update(molgroup.molecules());
}

void MolGroup::_pvt_setContents(const Molecules &molecules)
{
    MolGroupPvt &dref = *d;
    
    //set the molecules
    dref.molecules = molecules;
    
    //resize the two indexes
    dref.molidx_to_num.resize(molecules.nMolecules());
    dref.molviewidx_to_num.resize(molecules.nViews());

    MolNum *molidx_to_num_array = dref.molidx_to_num.data();
    MolNumUInt *molviewidx_to_num_array = dref.molviewidx_to_num.data();
    
    int imol = 0;
    int iview = 0;
    
    //index the molecules and views...
    for (Molecules::const_iterator it = molecules.begin();
         it != molecules.end();
         ++it)
    {
        const ViewsOfMol &molviews = *it;
        
        //index the molecule
        molidx_to_num_array[imol] = it.key();
        ++imol;
        
        //now index its views
        int nviews = molviews.nViews();
        
        for (int i=0; i < nviews; ++i)
        {
            molviewidx_to_num_array[iview+i] = MolNumUInt(it.key(),i);
        }
        
        iview += nviews;
    }
    
    dref.incrementMajor();
}

/** Set the contents of this group to 'molecules'. This clears
    any existing contents of this group. */
bool MolGroup::setContents(const Molecules &molecules)
{
    if (molecules.isEmpty())
    {
        if (this->isEmpty())
            return false;
        else
        {
            this->removeAll();
            return true;
        }
    }
    else if (this->isEmpty())
    {
        this->_pvt_setContents(molecules);
        return true;
    }

    const Molecules &oldmols = this->molecules();

    if (oldmols == molecules)
        //there is nothing to do
        return false;
        
    //see if the actual list of molecules has changed, or whether
    //it is just their versions...
    if (oldmols.nMolecules() == molecules.nMolecules())
    {
        bool different = false;
    
        //loop over all molecules and views and check that the same
        //molecules are in both sets, with the same views
        for (Molecules::const_iterator it = oldmols.begin();
             it != oldmols.end();
             ++it)
        {
            if (not molecules.contains(it.key()))
            {
                different = true;
                break;
            }
        
            const ViewsOfMol &oldviews = *it;
            const ViewsOfMol &newviews = molecules.molecule(it.key());
            
            int nviews = oldviews.nViews();
            
            if (nviews != newviews.nViews())
            {
                different = true;
                break;
            }
            
            for (int i=0; i<nviews; ++i)
            {
                if (oldviews.selection(i) != newviews.selection(i))
                {
                    different = true;
                    break;
                }
            }
            
            if (different)
                break;
        }
        
        if (not different)
        {
            //ok - the same molecules and views are in the two sets.
            //All that's changed are the versions :-)
            //Just update the molecules and increment the minor version 
            //counter
            d->molecules = molecules;
            d->incrementMinor();
            return true;
        }
    }
    
    //ok, the two sets of molecules contain different molecules and/or views.
    //We'll just have to reindex them completely.
    this->_pvt_setContents(molecules);
    return true;
}

/** Set the contents of this group so that it only contains the 
    view 'molview'. This clears any existing contents of this group */
bool MolGroup::setContents(const MoleculeView &molview)
{
    return this->setContents( Molecules(molview) );
}

/** Set the contents of this group so that it only contains the 
    views of the molecule in 'molviews'. This clears any existing
    contents of this group. */
bool MolGroup::setContents(const ViewsOfMol &molviews)
{
    return this->setContents( Molecules(molviews) );
}

/** Set the contents of this group so that it equals that
    of the group 'molgroup'. This sets the contents and
    also preserves the same order of molecules/views as
    in 'molgroup'
*/
bool MolGroup::setContents(const MolGroup &molgroup)
{
    //if the other group has the same ID number then we are reverting
    //this group back to another version
    if (this->number() == molgroup.number())
    {
        bool changed = (this->majorVersion() != molgroup.majorVersion() or
                        this->minorVersion() != molgroup.minorVersion());

        d = molgroup.d;
        
        return changed;
    }
        
    if (d.constData()->molecules == molgroup.d->molecules and
        d.constData()->molidx_to_num == molgroup.d->molidx_to_num and
        d.constData()->molviewidx_to_num == molgroup.d->molviewidx_to_num)
    {
        //the contents are exactly the same!
        return false;
    }
    
    MolGroupPvt &dref = *d;
    
    dref.molecules = molgroup.d->molecules;
    dref.molidx_to_num = molgroup.d->molidx_to_num;
    dref.molviewidx_to_num = molgroup.d->molviewidx_to_num;
    
    dref.incrementMajor();
    
    return true;
}
