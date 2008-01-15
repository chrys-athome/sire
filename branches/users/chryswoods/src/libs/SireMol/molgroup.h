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

#ifndef SIREMOL_MOLGROUP_H
#define SIREMOL_MOLGROUP_H

#include <QSharedDataPointer>
#include <QList>

#include <boost/tuple/tuple.hpp>

#include "SireBase/property.h"

#include "molecules.h"

namespace SireMol
{
class MolGroup;
}

QDataStream& operator<<(QDataStream&, const SireMol::MolGroup&);
QDataStream& operator>>(QDataStream&, SireMol::MolGroup&);

namespace SireID
{
class Index;
}

namespace SireMol
{

namespace detail
{
class MolGroupPvt;
}

using SireID::Index;

class MoleculeData;
class Molecules;
class Molecule;
class MoleculeView;
class PartialMolecule;
class ViewsOfMol;

class MolNum;
class MolNumViewIdx;

class MGName;
class MGNum;

using SireBase::ConcreteProperty;
using SireBase::PropertyBase;

/** This is the virtual base class of all MoleculeGroup type
    objects (e.g. MoleculeGroup, MolGroups and FFBase)
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT MolGroup : public ConcreteProperty<MolGroup,PropertyBase>
{

friend QDataStream& ::operator<<(QDataStream&, const MolGroup&);
friend QDataStream& ::operator>>(QDataStream&, MolGroup&);

public:

    typedef Molecules::const_iterator const_iterator;
    typedef Molecules::iterator iterator;

    MolGroup();

    MolGroup(const Molecules &molecules);

    MolGroup(const QString &name);

    MolGroup(const QString &name, const Molecules &molecules);
    MolGroup(const QString &name, const MolGroup &other);

    MolGroup(const MolGroup &other);

    virtual ~MolGroup();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MolGroup>() );
    }

    virtual const char* what() const
    {
        return MolGroup::typeName();
    }

    virtual MolGroup* clone() const
    {
        return new MolGroup(*this);
    }

    virtual bool operator==(const MolGroup &other) const;
    virtual bool operator!=(const MolGroup &other) const;
    
    const ViewsOfMol& operator[](MolNum molnum) const;
    const ViewsOfMol& operator[](MolIdx molidx) const;
    const ViewsOfMol& operator[](const MolName &molname) const;
    const ViewsOfMol& operator[](const MolID &molid) const;
    
    PartialMolecule operator[](const boost::tuple<MolNum,Index> &viewidx) const;
    PartialMolecule operator[](const boost::tuple<MolIdentifier,Index> &viewidx) const;
    
    MolGroup& operator+=(const Molecules &molecules);
    MolGroup& operator-=(const Molecules &molecules);
    
    const ViewsOfMol& at(MolNum molnum) const;
    const ViewsOfMol& at(MolIdx molidx) const;
    const ViewsOfMol& at(const MolName &molname) const;
    const ViewsOfMol& at(const MolID &molid) const;
    
    PartialMolecule at(const boost::tuple<MolNum,Index> &viewidx) const;
    PartialMolecule at(const boost::tuple<MolIdentifier,Index> &viewidx) const;
    
    PartialMolecule at(MolNum molnum, int viewidx) const;
    PartialMolecule at(const MolID &molid, int viewidx) const;
    
    const ViewsOfMol& moleculeAt(int idx) const;
    PartialMolecule viewAt(int idx) const;

    const ViewsOfMol& molecule(MolNum molnum) const;
    const ViewsOfMol& molecule(MolIdx molidx) const;
    const ViewsOfMol& molecule(const MolName &molname) const;
    const ViewsOfMol& molecule(const MolID &molid) const;

    Molecules molecules(const MolID &molid) const;

    MolNum getMoleculeNumber(MolNum molnum) const;
    MolNum getMoleculeNumber(MolIdx molidx) const;
    MolNum getMoleculeNumber(const MolName &molname) const;
    MolNum getMoleculeNumber(const MolID &molid) const;

    QList<MolNum> map(MolNum molnum) const;
    QList<MolNum> map(MolIdx molidx) const;
    QList<MolNum> map(const MolName &molname) const;
    QList<MolNum> map(const MolID &molid) const;

    bool contains(MolNum molnum) const;
    bool contains(MolIdx molidx) const;
    bool contains(const MolName &molname) const;
    bool contains(const MolID &molid) const;
    
    bool contains(const MoleculeView &molview) const;
    bool contains(const ViewsOfMol &molviews) const;
    bool contains(const Molecules &molecules) const;
    bool contains(const MolGroup &molgroup) const;
    
    bool intersects(const MoleculeView &molview) const;
    bool intersects(const Molecules &other) const;
    bool intersects(const MolGroup &molgroup) const;
    
    int nMolecules() const;
    
    int nViews() const;

    int nViews(MolNum molnum) const;
    int nViews(const MolID &molid) const;

    int nViews(Index idx) const;

    bool isEmpty() const;
    
    const Molecules& molecules() const;

    const ViewsOfMol& first() const;
    const ViewsOfMol& last() const;
    
    const ViewsOfMol& front() const;
    const ViewsOfMol& back() const;

    const_iterator begin() const;
    const_iterator end() const;

    const_iterator constBegin() const;
    const_iterator constEnd() const;

    const_iterator find(MolNum molnum) const;
    const_iterator constFind(MolNum molnum) const;

    const_iterator find(const MolID &molid) const;
    const_iterator constFind(const MolID &molid) const;

    QSet<MolNum> molNums() const;
    QSet<MolName> molNames() const;

    void assertContains(MolNum molnum) const;
    void assertContains(const MolName &molname) const;

    const MGName& name() const;
    MGNum number() const;
    
    quint64 majorVersion() const;
    quint64 minorVersion() const;

    virtual void add(const MoleculeView &molview);
    virtual void add(const ViewsOfMol &molviews);
    virtual void add(const Molecules &molecules);
    virtual void add(const MolGroup &molgroup);
    
    virtual bool addIfUnique(const MoleculeView &molview);
    virtual ViewsOfMol addIfUnique(const ViewsOfMol &molviews);
    virtual QList<ViewsOfMol> addIfUnique(const Molecules &molecules);
    virtual QList<ViewsOfMol> addIfUnique(const MolGroup &molgroup);
    
    bool unite(const MoleculeView &molview);
    ViewsOfMol unite(const ViewsOfMol &molviews);
    QList<ViewsOfMol> unite(const Molecules &molecules);
    QList<ViewsOfMol> unite(const MolGroup &molgroup);
    
    virtual bool remove(const MoleculeView &molview);
    virtual ViewsOfMol remove(const ViewsOfMol &molviews);
    virtual QList<ViewsOfMol> remove(const Molecules &molecules);
    virtual QList<ViewsOfMol> remove(const MolGroup &molgroup);
    
    virtual bool removeAll(const MoleculeView &molview);
    virtual ViewsOfMol removeAll(const ViewsOfMol &molviews);
    virtual QList<ViewsOfMol> removeAll(const Molecules &molecules);
    virtual QList<ViewsOfMol> removeAll(const MolGroup &molgroup);

    virtual ViewsOfMol remove(MolNum molnum);
    virtual QList<ViewsOfMol> remove(const QSet<MolNum> &molnums);

    virtual void removeAll();

    virtual bool update(const MoleculeData &moldata);
    bool update(const MoleculeView &molview);
    
    virtual QList<Molecule> update(const Molecules &molecules);
    virtual QList<Molecule> update(const MolGroup &molgroup);
    
    virtual bool setContents(const MoleculeView &molview);
    virtual bool setContents(const ViewsOfMol &molviews);
    virtual bool setContents(const Molecules &molecules);
    virtual bool setContents(const MolGroup &molgroup);

private:
    bool _pvt_remove(const MoleculeView &molview);
    ViewsOfMol _pvt_remove(const ViewsOfMol &molviews);

    ViewsOfMol _pvt_remove(MolNum molnum);
    
    bool _pvt_removeAll(const MoleculeView &molview);
    ViewsOfMol _pvt_removeAll(const ViewsOfMol &molviews);

    void _pvt_setContents(const Molecules &molecules);

    /** Implicitly shared pointer to the contents and index
        of this group */
    QSharedDataPointer<detail::MolGroupPvt> d;
};

}

Q_DECLARE_METATYPE(SireMol::MolGroup);

#endif
