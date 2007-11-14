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

/** This is the pure virtual base class of all MoleculeGroup type
    objects (e.g. MoleculeGroup, MolGroups and FFBase)
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT MolGroup : public PropertyBase
{

friend QDataStream& ::operator<<(QDataStream&, const MolGroup&);
friend QDataStream& ::operator>>(QDataStream&, MolGroup&);

public:
    ~MolGroup();
    
    static const char* typeName()
    {
        return "SireMol::MolGroup";
    }

    virtual MolGroup* clone() const=0;
    
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

protected:
    MolGroup();

    MolGroup(const Molecules &molecules);

    MolGroup(const QString &name);
    
    MolGroup(const QString &name, const Molecules &molecules);
    MolGroup(const QString &name, const MolGroup &molgroup);
    
    MolGroup(const MolGroup &other);

    MolGroup& operator=(const MolGroup &other);

    const detail::MolGroupPvt& data() const;

    detail::MolGroupPvt& edit();

private:
    /** Implicit pointer to the data of this object */
    QSharedDataPointer<detail::MolGroupPvt> d;
};


}

#endif
