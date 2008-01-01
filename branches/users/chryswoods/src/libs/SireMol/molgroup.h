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

#include <boost/tuple/tuple.hpp>

#include "SireBase/property.h"

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
class MoleculeView;
class PartialMolecule;
class ViewsOfMol;

class MolNum;
class MolNumViewIdx;

class MGName;
class MGNum;

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
    PartialMolecule operator[](const boost::tuple<MolNum,Index> &viewidx) const;
    
    const ViewsOfMol& at(MolNum molnum) const;
    PartialMolecule at(const boost::tuple<MolNum,Index> &viewidx) const;
    PartialMolecule at(MolNum molnum, Index viewidx) const;
    
    const ViewsOfMol& moleculeAt(Index idx) const;
    PartialMolecule viewAt(Index idx) const;
    
    int nMolecules() const;
    
    int nViews() const;
    int nViews(MolNum molnum) const;
    int nViews(Index idx) const;

    bool isEmpty() const;
    
    const Molecules& molecules() const;

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

    virtual bool remove(MolNum molnum);
    virtual QSet<MolNum> remove(const QSet<MolNum> &molnums);

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
    /** Implicitly shared pointer to the contents and index
        of this group */
    QSharedDataPointer<detail::MolGroupPvt> d;
};


}

#endif
