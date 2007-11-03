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
#include "mgname.h"
#include "mgnum.h"
#include "partialmolecule.h"

//// would like to get rid of these include files...///
#include "mover.hpp"
#include "editor.hpp"
#include "selector.hpp"

#include "molecule.h"
#include "segment.h"
#include "chain.h"
#include "residue.h"
#include "cutgroup.h"
#include "atom.h"

///////////////////////////////////////////////////////

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireMol::detail;

using namespace SireStream;
using namespace SireBase;

using boost::tuple;

QDataStream& operator<<(QDataStream &ds, const tuple<MolNum,Index> &viewidx)
{
    ds << viewidx.get<0>() << viewidx.get<1>();
    return ds;
}

QDataStream& operator>>(QDataStream &ds, tuple<MolNum,Index> &viewidx)
{
    ds >> viewidx.get<0>() >> viewidx.get<1>();
    
    return ds;
}

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

public:
    MoleculeGroupPvt();
    MoleculeGroupPvt(const QString &name);

    MoleculeGroupPvt(const QString &name, const Molecules &molecules);

    MoleculeGroupPvt(const QString &name, const MoleculeGroupPvt &other);

    MoleculeGroupPvt(const MoleculeGroupPvt &other);

    ~MoleculeGroupPvt();

    MoleculeGroupPvt& operator=(const MoleculeGroupPvt &other);

    bool operator==(const MoleculeGroupPvt &other) const;
    bool operator!=(const MoleculeGroupPvt &other) const;
    
    void add(const MoleculeView &view);
    void add(const ViewsOfMol &views);
    void add(const Molecules &molecules);
    void add(const MoleculeGroupPvt &molgroup);
    
    bool update(const MoleculeData &moldata);
    
    bool remove(const MoleculeView &view);
    bool remove(const ViewsOfMol &views);
    bool remove(const Molecules &molecules);
    
    bool remove(MolNum molnum);
    
    bool removeAll();

    bool setContents(const Molecules &molecules);
    bool setContents(const MoleculeGroupPvt &molgroup);

    void incrementMajor();
    void incrementMinor();

    /** The actual molecules in the group */
    Molecules molecules;
    
    /** The numbers of the molecules, in the order they appear
         in this group */
    QVector<MolNum> molidx_to_num;
    
    /** The molviewids of the views, in the order they appear
        in this group */
    QVector< tuple<MolNum,Index> > molviewidx_to_num;
    
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
                   number( MGNum::getUniqueNumber() ),
                   major_version(0),
                   minor_version(0)
{}

/** Construct an empty, named MoleculeGroupPvt */
MoleculeGroupPvt::MoleculeGroupPvt(const QString &nme)
                 : QSharedData(),
                   name(nme),
                   number( MGNum::getUniqueNumber() ),
                   major_version(0),
                   minor_version(0)
{}

/** Construct a named group that contains the molecules in 'molecules' */
MoleculeGroupPvt::MoleculeGroupPvt(const QString &nme, 
                                   const Molecules &molecules)
                 : QSharedData(),
                   name(nme),
                   number( MGNum::getUniqueNumber() ),
                   major_version(1),
                   minor_version(0)
{
    this->add(molecules);
}

/** Construct a named group that contains the same molecules as 'other' */
MoleculeGroupPvt::MoleculeGroupPvt(const QString &nme, 
                                   const MoleculeGroupPvt &other)
                 : QSharedData(),
                   molecules(other.molecules),
                   molidx_to_num(other.molidx_to_num),
                   molviewidx_to_num(other.molviewidx_to_num),
                   name(nme),
                   number( MGNum::getUniqueNumber() ),
                   major_version(1),
                   minor_version(0)
{}

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

bool MoleculeGroupPvt::operator==(const MoleculeGroupPvt &other) const
{
    return number == other.number and
           major_version == other.major_version and
           minor_version == other.minor_version;
}

bool MoleculeGroupPvt::operator!=(const MoleculeGroupPvt &other) const
{
    return number != other.number or
           major_version != other.major_version or
           minor_version != other.minor_version;
}

void MoleculeGroupPvt::add(const MoleculeView &view)
{
    MolNum molnum = view.data().number();

    if (molecules.contains(molnum))
    {
        molviewidx_to_num.append( tuple<MolNum,Index>(molnum,
                                           Index(molecules.nViews(molnum))) );
    }
    else
    {
        molidx_to_num.append(molnum);
        molviewidx_to_num.append( tuple<MolNum,Index>(molnum,Index(0)) );
    }
    
    molecules = molecules.add(view);
}

void MoleculeGroupPvt::add(const ViewsOfMol &views)
{
    MolNum molnum = views.data().number();
    
    if (molecules.contains(molnum))
    {
        int nviews = molecules.nViews(molnum);
    
        for (Index i(nviews); i<nviews + views.count(); ++i)
        {
            molviewidx_to_num.append( tuple<MolNum,Index>(molnum,i) );
        }
    }
    else
    {
        molidx_to_num.append(molnum);

        for (Index i(0); i<views.count(); ++i)
        {
            molviewidx_to_num.append( tuple<MolNum,Index>(molnum,i) );
        }
    }
    
    molecules = molecules.add(views);
}

void MoleculeGroupPvt::add(const Molecules &molecules)
{
    for (Molecules::const_iterator it = molecules.begin();
         it != molecules.end();
         ++it)
    {
        this->add(*it);
    }
}

void MoleculeGroupPvt::add(const MoleculeGroupPvt &molgroup)
{
    if (molecules.isEmpty())
    {
        this->setContents(molgroup);
        return;
    }
}

bool MoleculeGroupPvt::update(const MoleculeData &moldata)
{
    Molecules::const_iterator it = molecules.find(moldata.number());
    
    if (it == molecules.end())
        return false;
        
    if (it->data().version() == moldata.version())
        return false;
        
    molecules = molecules.update(moldata);
    return true;
}

template<class T0, class T1>
bool operator==(const tuple<T0,T1> &t0, const tuple<T0,T1> &t1)
{
    return boost::tuples::get<0>(t0) == boost::tuples::get<0>(t1) and 
           boost::tuples::get<1>(t0) == boost::tuples::get<1>(t1);
}

template<class T>
void remove_all(QVector<T> &vec, const T &value)
{
    QMutableVectorIterator<T> it(vec);
    
    while (it.hasNext())
    {
        if (it.next() == value)
            it.remove();
    }
}

bool MoleculeGroupPvt::remove(const MoleculeView &view)
{
    Molecules::const_iterator it = molecules.find(view.data().number());
    
    if (it == molecules.end())
        return false;
        
    AtomSelection selection = view.selection();
        
    QList<int> idxs = it->indiciesOf(selection);
    
    if (idxs.isEmpty())
        return false;
        
    //now update the indexes...
    foreach (int idx, idxs)
    {
        remove_all( molviewidx_to_num, tuple<MolNum,Index>(view.data().number(),
                                                           Index(idx)) );
    }
    
    if (idxs.count() == it->nViews())
    {
        //we have remove all of the views of this molecule
        remove_all(molidx_to_num, view.data().number());
    }

    molecules = molecules.remove(view);
    
    return true;
}

bool MoleculeGroupPvt::remove(MolNum molnum)
{
    if (not molecules.contains(molnum))
        return false;
        
    molecules = molecules.remove(molnum);
    
    //remove the indexes...
    remove_all(molidx_to_num, molnum);
    
    QMutableVectorIterator< tuple<MolNum,Index> > it(molviewidx_to_num);
    
    while (it.hasNext())
    {
        if (it.next().get<0>() == molnum)
            it.remove();
    }
    
    return true;
}

bool MoleculeGroupPvt::remove(const ViewsOfMol &views)
{
    Molecules::const_iterator it = molecules.find(views.number());
    
    if (it == molecules.end())
        return false;
        
    if (*it == views)
    {
        //we are removing the whole molecule
        return this->remove(views.number());
    }
        
    bool removesome = false;
        
    foreach (const AtomSelection &selection, views.selections())
    {
        QList<int> idxs = it->indiciesOf(selection);
        
        if (not idxs.isEmpty())
        {
            foreach (int idx, idxs)
            {
                remove_all( molviewidx_to_num, tuple<MolNum,Index>(views.number(),
                                                                   Index(idx)) );
            }
            
            if (idxs.count() == it->nViews())
                remove_all(molidx_to_num,views.number());
                
            removesome = true;
        }
    }
    
    molecules = molecules.remove(views);
    
    return removesome;
}

bool MoleculeGroupPvt::remove(const Molecules &mols)
{
    bool changed = false;
    
    for (Molecules::const_iterator it = mols.begin();
         it != mols.end();
         ++it)
    {
        bool this_changed = this->remove(*it);
        
        changed = changed or this_changed;
    }
    
    return changed;
}

bool MoleculeGroupPvt::removeAll()
{
    if (molecules.isEmpty())
        return false;
    else
    {
        molecules = molecules.removeAll();
        molidx_to_num.clear();
        molviewidx_to_num.clear();
        
        return true;
    }
}

bool MoleculeGroupPvt::setContents(const Molecules &mols)
{
    if (molecules == mols)
        return false;

    this->removeAll();
    
    this->add(mols);
    
    return true;
}

bool MoleculeGroupPvt::setContents(const MoleculeGroupPvt &other)
{
    if (this == &other or molecules == other.molecules)
        return false;
        
    molecules = other.molecules;
    molidx_to_num = other.molidx_to_num;
    molviewidx_to_num = other.molviewidx_to_num;
    
    return true;
}

void MoleculeGroupPvt::incrementMajor()
{
    throw SireError::incomplete_code("Unimplemented", CODELOC);
}

void MoleculeGroupPvt::incrementMinor()
{
    throw SireError::incomplete_code("Unimplemented", CODELOC);
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
              : d( new MoleculeGroupPvt("unnamed", molecules) )
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
    contents as 'other' - note that this will create a new MoleculeGroup,
    with a new ID number */
MoleculeGroup::MoleculeGroup(const QString &name, const MoleculeGroup &other)
              : d( new MoleculeGroupPvt(name) )
{
    d->setContents( *(other.d) );
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
PartialMolecule MoleculeGroup::operator[](const tuple<MolNum,Index> &viewidx) const
{
    return d->molecules[viewidx];
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
PartialMolecule MoleculeGroup::at(const tuple<MolNum,Index> &viewidx) const
{
    return d->molecules.at(viewidx);
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
    const QVector< tuple<MolNum,Index> > &molviewidx_to_num = d->molviewidx_to_num;
    
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
    d->incrementMajor();
}

/** Update the view(s) of the molecule so that the view(s) in this
    set have the same version of the molecule as in 'molview'.
    This does nothing if a view of this molecule is not in this group */
void MoleculeGroup::update(const MoleculeView &molview)
{
    if (d->update(molview.data()))
        d->incrementMinor();
}

/** Remove the view, 'molview', from this group. This only
    removes this specific view, if it exists in this group,
    and does nothing if this view does not exist in this group.
    Note that this removes *all* copies of this view from
    this group, if it exists multiple times */
void MoleculeGroup::remove(const MoleculeView &molview)
{
    if (d->remove(molview))
        d->incrementMajor();
}

/** Add all of the views 'molviews' to this group - this
    duplicates any views that already exist in this group */
void MoleculeGroup::add(const ViewsOfMol &molviews)
{
    d->add(molviews);
    d->incrementMajor();
}

/** Remove all of the views in 'molviews' from this group
      - this removes only the exact views that are in 
        'molviews', and removes all copies of any 
        duplicated views */
void MoleculeGroup::remove(const ViewsOfMol &molviews)
{
    if (d->remove(molviews))
        d->incrementMajor();
}

/** Add all of the molecules in 'molecules' into this group.
    This duplicates any views that already exist in this group */
void MoleculeGroup::add(const Molecules &molecules)
{
    d->add(molecules);
    d->incrementMajor();
}

/** Update the molecule whose data is in 'moldata' to 
    the same version as 'moldata' */
void MoleculeGroup::update(const MoleculeData &moldata)
{
    if (d->update(moldata))
        d->incrementMinor();
}

/** Update all of the molecules in this group so that they
    have the same version as the molecules in 'molecules' */
void MoleculeGroup::update(const Molecules &molecules)
{
    bool changed = false;
    
    for (Molecules::const_iterator it = molecules.begin();
         it != molecules.end();
         ++it)
    {
        bool this_changed = d->update(it->data());
        changed = changed or this_changed;
    }
    
    if (changed)
        d->incrementMinor();
}

/** Remove all of the views of the molecules that are in
    'molecules' from this group - this removes only
    those views that exactly match 'molecules', and
    removes all copies of duplicated views */
void MoleculeGroup::remove(const Molecules &molecules)
{
    if (d->remove(molecules))
        d->incrementMajor();
}

/** Add all of the molecules in the group 'molgroup' to 
    this group - this duplicates any views that are already
    in this group */
void MoleculeGroup::add(const MoleculeGroup &molgroup)
{
    d->add( *(molgroup.d) );
    d->incrementMajor();
}

/** Update all of the molecules in this group so that 
    they have the same versions as the molecules in  
    the group 'molgroup' */
void MoleculeGroup::update(const MoleculeGroup &molgroup)
{
    this->update(molgroup.molecules());
}

/** Remove all of the molecule views that are in the
    group 'molgroup' from this group */
void MoleculeGroup::remove(const MoleculeGroup &molgroup)
{
    if (d->remove(molgroup.molecules()))
        d->incrementMajor();
}

/** Completely remove the molecule with number 'molnum'
    from this group - this does nothing if there is no
    such molecule in this group */
void MoleculeGroup::remove(MolNum molnum)
{
    if (d->remove(molnum))
        d->incrementMajor();
}

/** Remove all of the molecules whose numbers are in 'molnums' */
void MoleculeGroup::remove(const QSet<MolNum> &molnums)
{
    bool changed = false;

    foreach (MolNum molnum, molnums)
    {
        bool this_changed = d->remove(molnum);
        changed = changed or this_changed;
    }
    
    if (changed)
        d->incrementMajor();
}

/** Completely remove all of the molecules from this group */
void MoleculeGroup::removeAll()
{
    if (d->removeAll())
        d->incrementMajor();
}

/** Set this group to contain only the view 'molview' */
void MoleculeGroup::setContents(const MoleculeView &molview)
{
    if (d->setContents( Molecules(molview) ))
        d->incrementMajor();
}

/** Set this group to contain only the views in 'molviews' */
void MoleculeGroup::setContents(const ViewsOfMol &molviews)
{
    if (d->setContents( Molecules(molviews) ))
        d->incrementMajor();
}

/** Set this group to contain only the molecules in 'molecules' */
void MoleculeGroup::setContents(const Molecules &molecules)
{
    if (d->setContents(molecules))
        d->incrementMajor();
}

/** Set this group to contain only the molecules in 'molgroup' */
void MoleculeGroup::setContents(const MoleculeGroup &molgroup)
{
    if (d->setContents( *(molgroup.d) ))
        d->incrementMajor();
}
