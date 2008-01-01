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

#include "molgroup.h"

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

////////////
//////////// Implementation of MolGroupPvt
////////////

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
    QVector< tuple<MolNum,quint32> > molviewidx_to_num;
    
    MGName name;
    MGNum number;
    
    quint64 major_version;
    quint64 minor_version;
};

}

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
              major_version(1),
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

void MolGroupPvt::add(const MoleculeView &view)
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

void MolGroupPvt::add(const ViewsOfMol &views)
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

void MolGroupPvt::add(const Molecules &molecules)
{
    for (Molecules::const_iterator it = molecules.begin();
         it != molecules.end();
         ++it)
    {
        this->add(*it);
    }
}

void MolGroupPvt::add(const MolGroupPvt &molgroup)
{
    if (molecules.isEmpty())
    {
        this->setContents(molgroup);
        return;
    }
}

void MolGroupPvt::update(const MoleculeData &moldata)
{
    Molecules::const_iterator it = molecules.find(moldata.number());
    
    if (it == molecules.end() or it->data().version() == moldata.version())
        return;
        
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

bool MolGroupPvt::remove(const MoleculeView &view)
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

bool MolGroupPvt::remove(MolNum molnum)
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

bool MolGroupPvt::remove(const ViewsOfMol &views)
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

bool MolGroupPvt::remove(const Molecules &mols)
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

bool MolGroupPvt::removeAll()
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

bool MolGroupPvt::setContents(const Molecules &mols)
{
    if (molecules == mols)
        return false;

    this->removeAll();
    
    this->add(mols);
    
    return true;
}

bool MolGroupPvt::setContents(const MolGroupPvt &other)
{
    if (this == &other or molecules == other.molecules)
        return false;
        
    molecules = other.molecules;
    molidx_to_num = other.molidx_to_num;
    molviewidx_to_num = other.molviewidx_to_num;
    
    return true;
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
    if (name == other.name)
    {
        d = other.d;
    }
    else
        d = new MolGroupPvt(name, other);
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

/** Add the view of the molecule in 'molview' to this group. 
    This adds the view as a duplicate if it already exists 
    in this group */
void MolGroup::add(const MoleculeView &molview)
{
    if (molview.isEmpty())
        return;

    MolNum molnum = molview.number();

    MolGroupPvt &dref = *d;

    if (not dref.molecules.contains(molnum))
        dref.molidx_to_num.append(molnum);
    
    dref.molecules.add(molview);
   
    dref.molviewidx_to_num.append( tuple<MolNum,quint32>(molnum,
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
        dref.molviewidx_to_num.append( tuple<MolNum,quint32>(molnum,i) );
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
            molidx_to_num.append(molnum);
            
        quint32 nviews = dref.molecules.nViews(molnum);
        quint32 n_newviews = it->nViews();
        
        for (quint32 i=nviews; i<nviews + n_newviews; ++i)
        {
            molviewidx_to_num.append( tuple<MolNum,quint32>(molnum,i) );
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
    
    foreach (const tuple<MolNum,quint32> &molviewidx,
             molgroup.d->molviewidx_to_num)
    {
        MolNum molnum = molviewidx.get<0>();
    
        dref.molviewidx_to_num.append( tuple<MolNum,quint32>( molnum,
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
    if (molview.isEmpty())
        return false;

    MolNum molnum = molview.number();

    MolGroupPvt &dref = *d;
    
    bool hasmol = dref.molecules.contains(molnum);
    
    if (dref.molecules.addIfUnique(molview))
    {
        //the view has been added!
        if (not hasmol)
            dref.molidx_to_num.append(molnum);
            
        dref.molviewidx_to_num.append( tuple<MolNum,quint32>(molnum,
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
            molidx_to_num.append(molnum);
            
        quint32 nviews = dref.molecules.nViews(molnum);
        quint32 nadded = added_views.nViews();
        
        for (quint32 i=nviews-nadded-1; i<nviews; ++i)
        {
            molviewidx_to_num.append( tuple<MolNum,quint32>(molnum,i) );
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
                dref.molviewidx_to_num.append( tuple<MolNum,quint32>(molnum,i) );
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
    
    foreach (const tuple<MolNum,quint32> &molviewidx,
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

/** Remove the view of the molecule in 'molview' from this set.
    This only removes the first such view from the set, and 
    returns whether or not any view was removed */
bool MolGroup::remove(const MoleculeView &molview)
{
    if (molview.isEmpty())
        return false;

    MolNum molnum = molview.number();

    Molecules::const_iterator it = d.constData()->molecules.find(molnum);
    
    if (it == d.constData()->molecules.end())
        return false;
        
    int viewidx = it->indexOf(molview.selectedAtoms());
    
    if (viewidx == -1)
        //this view is not present in this group
        return false;
    
    //the view is present - remove it!    
    MolGroupPvt &dref = *d;
    
    //remove all of the views of this molecule...
    ViewsOfMol molviews = dref.molecules.remove(molview.number());
    
    //remove the specified view from this set...
    molviews.removeAt(viewidx);
    
    //if there are any views left then add them back
    //to the collection of molecules
    if (not molviews.isEmpty())
    {
        dref.molecules.add(molviews);
        
        //the molecule has only been partially removed,
        //so remove this view from the molview index
        ...
    }
    else
    {
        //this molecule has been completely removed...
        //remove it completely from the index
    }
}

ViewsOfMol MolGroup::remove(const ViewsOfMol &molviews);
QList<ViewsOfMol> MolGroup::remove(const Molecules &molecules);
QList<ViewsOfMol> MolGroup::remove(const MolGroup &molgroup);

bool MolGroup::removeAll(const MoleculeView &molview);
ViewsOfMol MolGroup::removeAll(const ViewsOfMol &molviews);
QList<ViewsOfMol> MolGroup::removeAll(const Molecules &molecules);
QList<ViewsOfMol> MolGroup::removeAll(const MolGroup &molgroup);

bool MolGroup::remove(MolNum molnum);
QSet<MolNum> MolGroup::remove(const QSet<MolNum> &molnums);

void MolGroup::removeAll();

bool MolGroup::update(const MoleculeData &moldata);
bool MolGroup::update(const MoleculeView &molview);

QList<Molecule> MolGroup::update(const Molecules &molecules);
QList<Molecule> MolGroup::update(const MolGroup &molgroup);

bool MolGroup::setContents(const MoleculeView &molview);
bool MolGroup::setContents(const ViewsOfMol &molviews);
bool MolGroup::setContents(const Molecules &molecules);
bool MolGroup::setContents(const MolGroup &molgroup);
