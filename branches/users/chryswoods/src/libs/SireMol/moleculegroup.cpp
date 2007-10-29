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

#include "moleculegroup.h"

#include "molecules.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireMol::detail;

using namespace SireStream;
using namespace SireBase;

///////////
/////////// Implementation of MoleculeGroupPvt
///////////

namespace SireMol
{

namespace detail
{

/** This is the private implementation of MoleculeGroup - this
    allows the MoleculeGroup class to be implicitly shared.

    @author Christopher Woods
*/
class MoleculeGroupPvt : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeGroupPvt&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeGroupPvt&);

public:
    MoleculeGroupPvt();
    MoleculeGroupPvt(const QString &name);

    MoleculeGroupPvt(const QString &name, const Molecules &molecules);

    MoleculeGroupPvt(const MoleculeGroupPvt &other);

    ~MoleculeGroupPvt();

    MoleculeGroupPvt& operator=(const MoleculeGroupPvt &other);

    /** The actual molecules in the group */
    Molecules molecules;
    
    /** The numbers of the molecules, in the order they appear
         in this group */
    QVector<MolNum> molidx_to_num;
    
    /** The molviewids of the views, in the order they appear
        in this group */
    QVector<MolNumViewIdx> molviewidx_to_num;
    
    /** The name of this group */
    MGName name;
    
    /** The number of this group */
    MGNum number;
    
    /** The major version number - this changes whenever
        views are added or removed from this group */
    quint64 major_version;
    
    /** The minor version number - this changes whenever
        molecules in the group are changed */
    quint64 minor_version;
};

} //end of namespace detail

} //end of namespace SireMol

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, 
                                       const MoleculeGroupPvt &molgrouppvt)
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
                                       MoleculeGroupPvt &molgrouppvt)
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

static Incremint mgnum_incremint(0);

static MGNum getNewIDNum()
{
    return MGNum( mgnum_incremint.increment() );
}

static QSharedDataPointer<MoleculeGroupPvt> shared_null;

static const QSharedDataPointer<MoleculeGroupPvt>& getSharedNull()
{
    if (shared_null.constData() == 0)
        shared_null = new MoleculeGroupPvt();
        
    return shared_null;
}

/** Construct an empty, unnamed MoleculeGroupPvt */
MoleculeGroupPvt::MoleculeGroupPvt() 
                 : QSharedData(),
                   name("unnamed"),
                   number( getNewIDNum() )
{}

/** Construct an empty, named MoleculeGroupPvt */
MoleculeGroupPvt::MoleculeGroupPvt(const QString &nme)
                 : QSharedData(),
                   name(nme),
                   number( getNewIDNum() )
{}

/** Construct a named group that contains the molecules in 'molecules' */
MoleculeGroupPvt::MoleculeGroupPvt(const QString &nme, 
                                   const Molecules &molecules)
                 : QSharedData(),
                   name(nme),
                   number( getNewIDNum() )
{
    this->add(molecules);
}

/** Copy constructor */
MoleculeGroupPvt::MoleculeGroupPvt(const MoleculeGroupPvt &other)
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
MoleculeGroupPvt::~MoleculeGroupPvt()
{}

/** Copy assignment operator */
MoleculeGroupPvt& MoleculeGroupPvt::operator=(const MoleculeGroupPvt &other)
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

///////////
/////////// Implementation of MoleculeGroup
///////////

/** Construct an unnamed, empty MoleculeGroup */
MoleculeGroup::MoleculeGroup()
              : d( getSharedNull() )
{}

/** Construct an unnamed MoleculeGroup that contains the 
    molecules 'molecules' */
MoleculeGroup::MoleculeGroup(const Molecules &molecules)
              : d( new MoleculeGroupPvt(molecules) )
{}

/** Construct an empty MoleculeGroup called 'name' */
MoleculeGroup::MoleculeGroup(const QString &name)
              : d( new MoleculeGroupPvt(name) )
{}

/** Construct a MoleculeGroup called 'name' that contains
    the molecules 'molecules' */
MoleculeGroup::MoleculeGroup(const QString &name, const Molecules &molecules)
              : d( new MoleculeGroupPvt(name, molecules) )
{}

/** Construct a MoleculeGroup called 'name' that contains the same
    contents as 'other' */
MoleculeGroup::MoleculeGroup(const QString &name, const MoleculeGroup &other)
{
    if (name == other->name())
        d = other.d;
    else
    {
        d = new MoleculeGroup(name, other);
    }
}

/** Copy constructor */
MoleculeGroup::MoleculeGroup(const MoleculeGroup &other)
              : d(other.d)
{}

/** Destructor */
MoleculeGroup::~MoleculeGroup()
{}

/** Copy assignment operator */
MoleculeGroup& MoleculeGroup::operator=(const MoleculeGroup &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MoleculeGroup::operator==(const MoleculeGroup &other) const
{
    return d == other.d or *d == *(other.d);
}

/** Comparison operator */
bool MoleculeGroup::operator!=(const MoleculeGroup &other) const
{
    return d != other.d or *d != *(other.d);
}

/** Return the views of the molecule with number 'molnum'

    \throw SireMol::missing_molecule
*/
const ViewsOfMol& MoleculeGroup::operator[](MolNum molnum) const
{
    return d->molecules[molnum];
}

/** Return the view of the molecule at view 'molviewid'

    \throw SireMol::missing_molecule
    \throw SireError::invalid_index
*/
PartialMolecule MoleculeGroup::operator[](const MolViewID &molviewid) const
{
    return d->molecules[molviewid];
}

/** Return the views of the molecule with number 'molnum'

    \throw SireMol::missing_molecule
*/
const ViewsOfMol& MoleculeGroup::at(MolNum molnum) const
{
    return d->molecules.at(molnum);
}

/** Return the view of the molecule at view 'molviewid'

    \throw SireMol::missing_molecule
    \throw SireError::invalid_index
*/
PartialMolecule MoleculeGroup::at(const MolViewID &molviewid) const
{
    return d->molecules.at(molviewid);
}

/** Return the view at index 'viewidx' of the molecule at 
    number 'molnum'
    
    \throw SireMol::missing_molecule
    \throw SireError::invalid_index
*/
PartialMolecule MoleculeGroup::at(MolNum molnum, Index viewidx) const
{
    return d->molecules.at(molnum, viewidx);
}

/** Return all the views of the ith molecule in this group

    \throw SireError::invalid_index
*/
const ViewsOfMol& MoleculeGroup::moleculeAt(Index i) const
{
    const QVector<MolNum> &molidx_to_num = d->molidx_to_num;

    return d->molecules.at( molidx_to_num.constData()[i.map(molidx_to_num.count())] );
}

/** Return the ith view in this group */
PartialMolecule MoleculeGroup::viewAt(Index i) const
{
    const QVector<MolNum> &molviewidx_to_num = d->molviewidx_to_num;
    
    return d->molecules.at( molviewidx_to_num.constData()
                               [i.map(molviewidx_to_num.count())] );
}

/** Return the number of molecules in this group */
int MoleculeGroup::nMolecules() const
{
    return d->molidx_to_num.count();
}

/** Return the number of views in this group */
int MoleculeGroup::nViews() const
{
    return d->molviewidx_to_num.count();
}

/** Return the number of views of the molecule at number 
    'molnum' in this group 
    
    \throw SireMol::missing_molecule
*/
int MoleculeGroup::nViews(MolNum molnum) const
{
    return this->at(molnum).nViews();
}

/** Return the number of views in the ith molecule in this 
    group
    
    \throw SireError::invalid_index
*/
int MoleculeGroup::nViews(Index i) const
{
    return this->moleculeAt(i).nViews();
}

/** Return whether this group is empty */
bool MoleculeGroup::isEmpty() const
{
    return d->molidx_to_num.isEmpty();
}

/** Return all of the molecules in the group */
const Molecules& MoleculeGroup::molecules() const
{
    return d->molecules;
}

/** Return the name of this molecule group */
const MGName& MoleculeGroup::name() const
{
    return d->name;
}

/** Return the number of this molecule group */
MGNum MoleculeGroup::number() const
{
    return d->number;
}

/** Return the major version number of this group - the
    major version number changes whenever views are added
    or removed from this group */
quint64 MoleculeGroup::majorVersion() const
{
    return d->major_version;
}

/** Return the minor version number of this group - the
    minor version number changes whenever any of the
    molecules in this group are changed */
quint64 MoleculeGroup::minorVersion() const
{
    return d->minor_version;
}

/** Add the view 'molview' to this group - this will add the 
    view as a duplicate if it already exists in this group */
void MoleculeGroup::add(const MoleculeView &molview)
{
    d->add(molview);
}

/** Update the view(s) of the molecule so that the view(s) in this
    set have the same version of the molecule as in 'molview'.
    This does nothing if a view of this molecule is not in this group */
void MoleculeGroup::update(const MoleculeView &molview)
{
    d->update(molview.data());
}

/** Remove the view, 'molview', from this group. This only
    removes this specific view, if it exists in this group,
    and does nothing if this view does not exist in this group.
    Note that this removes *all* copies of this view from
    this group, if it exists multiple times */
void MoleculeGroup::remove(const MoleculeView &molview)
{
    d->remove(molview);
    d->reindex();
}

/** Add all of the views 'molviews' to this group - this
    duplicates any views that already exist in this group */
void MoleculeGroup::add(const ViewsOfMol &molviews)
{
    d->add(molviews);
}

/** Remove all of the views in 'molviews' from this group
      - this removes only the exact views that are in 
        'molviews', and removes all copies of any 
        duplicated views */
void MoleculeGroup::remove(const ViewsOfMol &molviews)
{
    d->remove(molviews);
    d->reindex();
}

/** Add all of the molecules in 'molecules' into this group.
    This duplicates any views that already exist in this group */
void MoleculeGroup::add(const Molecules &molecules)
{
    d->add(molecules);
}

/** Update all of the molecules in this group so that they
    have the same version as the molecules in 'molecules' */
void MoleculeGroup::update(const Molecules &molecules)
{
    d->update(molecules);
}

/** Remove all of the views of the molecules that are in
    'molecules' from this group - this removes only
    those views that exactly match 'molecules', and
    removes all copies of duplicated views */
void MoleculeGroup::remove(const Molecules &molecules)
{
    d->remove(molecules);
    d->reindex();
}

/** Add all of the molecules in the group 'molgroup' to 
    this group - this duplicates any views that are already
    in this group */
void MoleculeGroup::add(const MoleculeGroup &molgroup)
{
    if (this->isEmpty())
        d = molgroup.d;
    else
        d->add(molgroup);
}

/** Update all of the molecules in this group so that 
    they have the same versions as the molecules in  
    the group 'molgroup' */
void MoleculeGroup::update(const MoleculeGroup &molgroup)
{
    d->update( molgroup.molecules() );
}

/** Remove all of the molecule views that are in the
    group 'molgroup' from this group */
void MoleculeGroup::remove(const MoleculeGroup &molgroup)
{
    d->remove(molgroup);
    d->reindex();
}

/** Update the molecule whose data is in 'moldata' to 
    the same version as 'moldata' */
void MoleculeGroup::update(const MoleculeData &moldata)
{
    d->update(moldata);
}

/** Completely remove the molecule with number 'molnum'
    from this group - this does nothing if there is no
    such molecule in this group */
void MoleculeGroup::remove(MolNum molnum)
{
    d->remove(molnum);
    d->reindex();
}

/** Remove all of the molecules whose numbers are in 'molnums' */
void MoleculeGroup::remove(const QSet<MolNum> &molnums)
{
    foreach (MolNum molnum, molnums)
    {
        d->remove(molnum);
    }
    
    d->reindex();
}

/** Completely remove all of the molecules from this group */
void MoleculeGroup::removeAll()
{
    d->removeAll();
}

/** Set this group to contain only the view 'molview' */
void MoleculeGroup::setContents(const MoleculeView &molview)
{
    d->setContents( Molecules(molview) );
}

/** Set this group to contain only the views in 'molviews' */
void MoleculeGroup::setContents(const ViewsOfMol &molviews)
{
    d->setContents( Molecules(molviews) );
}

/** Set this group to contain only the molecules in 'molecules' */
void MoleculeGroup::setContents(const Molecules &molecules)
{
    d->setContents(molecules);
}

/** Set this group to contain only the molecules in 'molgroup' */
void MoleculeGroup::setContents(const MoleculeGroup &molgroup)
{
    d = MoleculeGroup(this->name(), molgroup);
}
