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
    be moved by a Monte Carlo move
    
    MoleculeGroup objects can also be named, so you could
    create groups like "solvent", "protein", "ligands",
    "ions" etc. Each MoleculeGroup in a simulations system
    is placed into a single MoleculeGroups object, which
    can then let you search for atoms or molecules using
    these group names, e.g. GroupName("solvent") + AtomName("O00")
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

    bool isEmpty() const;
    int count() const;
    int nMolecules() const;

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
