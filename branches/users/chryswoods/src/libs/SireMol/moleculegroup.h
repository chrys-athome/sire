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

#ifndef SIREMOL_MOLECULEGROUP_H
#define SIREMOL_MOLECULEGROUP_H

#include <QSharedDataPointer>

#include <boost/tuple/tuple.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeGroup;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeGroup&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeGroup&);

namespace SireID
{
class Index;
}

namespace SireMol
{

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

namespace detail
{
class MoleculeGroupPvt;
}

/** This class is used to group views of molecules together. 
    Underneath, it used the Molecules class to hold views
    of molecules (including multiple views of molecules).
    However it supplements the data in Molecules with 
    additional meta-information, such as indexing of
    the molecules (so you can quicky choose the third
    molecule in the group), indexing of the views
    (so you can quickly choose the tenth view in
    the group), and also versioning (so you can tell
    if the composition of the group has changed,
    or if any of the members have changed), and also
    an ID number that lets you keep track of the group.
    
    The primary purpose of a MoleculeGroup is to allow you
    to construct groups of molecule views that can be 
    used by the program, e.g. a group of views that will
    be moved by a Monte Carlo move, or the group of
    atoms that are in particular forcefield (or a sub-group
    of that forcefield)
    
    MoleculeGroup objects can also be named, so you could
    create groups like "solvent", "protein", "ligands",
    "ions" etc. Each MoleculeGroup in a simulations system
    is placed into a single MoleculeGroups object, which
    can then let you search for atoms or molecules using
    these group names, e.g. MGName("solvent") + AtomName("O00")
    would select all of the atoms called "O00" in all of 
    the molecules in the group(s) called "solvent"

    @author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeGroup
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeGroup&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeGroup&);

public:
    MoleculeGroup();

    MoleculeGroup(const Molecules &molecules);

    MoleculeGroup(const QString &name);

    MoleculeGroup(const QString &name, const Molecules &molecules);
    MoleculeGroup(const QString &name, const MoleculeGroup &other);

    MoleculeGroup(const MoleculeGroup &other);

    ~MoleculeGroup();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MoleculeGroup>() );
    }

    const char* what() const
    {
        return MoleculeGroup::typeName();
    }

    MoleculeGroup* clone() const
    {
        return new MoleculeGroup(*this);
    }

    MoleculeGroup& operator=(const MoleculeGroup &other);

    bool operator==(const MoleculeGroup &other) const;
    bool operator!=(const MoleculeGroup &other) const;

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

    void add(const MoleculeView &molview);
    void update(const MoleculeView &molview);
    void remove(const MoleculeView &molview);

    void add(const ViewsOfMol &molviews);
    void remove(const ViewsOfMol &molviews);

    void add(const Molecules &molecules);
    void update(const Molecules &molecules);
    void remove(const Molecules &molecules);

    void add(const MoleculeGroup &molgroup);
    void update(const MoleculeGroup &molgroup);
    void remove(const MoleculeGroup &molgroup);

    void update(const MoleculeData &moldata);
    void remove(MolNum molnum);

    void remove(const QSet<MolNum> &molnums);

    void removeAll();
    
    void setContents(const MoleculeView &molview);
    void setContents(const ViewsOfMol &molviews);
    void setContents(const Molecules &molecules);
    void setContents(const MoleculeGroup &molgroup);

private:
    /** Implicit pointer to the data of this object */
    QSharedDataPointer<detail::MoleculeGroupPvt> d;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeGroup);

SIRE_END_HEADER

#endif
