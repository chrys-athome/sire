/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#include <QList>
#include <QHash>
#include <QSharedDataPointer>

#include "moleculegroupid.h"

#include "partialmolecule.h"
#include "moleculeid.h"

#include "SireBase/idmajminversion.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

namespace detail
{
class MoleculeGroupPvt;
}

class MoleculeGroup;
}

QDataStream& operator<<(QDataStream&, const SireMol::detail::MoleculeGroupPvt&);
QDataStream& operator>>(QDataStream&, SireMol::detail::MoleculeGroupPvt&);

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeGroup&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeGroup&);

namespace SireMol
{

using SireBase::Version;

class Molecules;

namespace detail
{
class MoleculeGroupPvt;
}

/** This class holds a group of molecules. The group is versioned,
    and holds the molecules in a manner that allows efficient linear
    access and indexing (by MoleculeID)

    @author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeGroup
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeGroup&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeGroup&);

public:
    MoleculeGroup();

    MoleculeGroup(const QString &name);

    MoleculeGroup(const QString &name, const Molecules &molecules);

    MoleculeGroup(const MoleculeGroup &other);

    ~MoleculeGroup();

    MoleculeGroup& operator=(const MoleculeGroup &other);

    bool operator==(const MoleculeGroup &other) const;
    bool operator!=(const MoleculeGroup &other) const;

    const PartialMolecule& operator[](MoleculeID molid) const;

    const PartialMolecule& at(MoleculeID molid) const;

    const PartialMolecule& molecule(MoleculeID molid) const;

    const QString& name() const;

    MoleculeGroupID ID() const;
    const Version& version() const;

    const Molecules& molecules() const;

    QSet<MoleculeID> moleculeIDs() const;

    int count() const;

    bool add(const PartialMolecule &molecule);
    bool change(const PartialMolecule &molecule);
    bool remove(const PartialMolecule &molecule);

    bool remove(MoleculeID molid);

    bool add(const Molecules &molecules);
    bool change(const Molecules &molecules);
    bool remove(const Molecules &molecules);

    bool remove(const QSet<MoleculeID> &molids);

    void rename(const QString &newname);

    bool refersTo(MoleculeID molid) const;

    bool contains(const PartialMolecule &molecule) const;

private:
    /** Implicit pointer to the data of this object */
    QSharedDataPointer<detail::MoleculeGroupPvt> d;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeGroup);

SIRE_END_HEADER

#endif
