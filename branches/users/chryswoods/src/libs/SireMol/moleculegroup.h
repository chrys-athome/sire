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

#include <QVector>
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

namespace detail
{

/** This is the private implementation of MoleculeGroup - this
    allows the MoleculeGroup class to be implicitly shared.

    @author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeGroupPvt : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeGroupPvt&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeGroupPvt&);

public:
    MoleculeGroupPvt();
    MoleculeGroupPvt(const QString &name);

    MoleculeGroupPvt(const QString &name,
                     const QVector<PartialMolecule> &molecules);

    MoleculeGroupPvt(const MoleculeGroupPvt &other);

    ~MoleculeGroupPvt();

    MoleculeGroupPvt& operator=(const MoleculeGroupPvt &other);

    bool operator==(const MoleculeGroupPvt &other) const;
    bool operator!=(const MoleculeGroupPvt &other) const;

    const QVector<PartialMolecule>& molecules() const;
    const QHash<MoleculeID,int>& index() const;

    QSet<MoleculeID> moleculeIDs() const;

    const PartialMolecule& molecule(MoleculeID molid) const;

    const QString& name() const;

    MoleculeGroupID ID() const;
    const Version& version() const;

    bool add(const PartialMolecule &molecule);
    bool change(const PartialMolecule &molecule);
    bool remove(const PartialMolecule &molecule);

    bool remove(MoleculeID molid);
    bool remove(const QSet<MoleculeID> &molids);

    bool add(const QVector<PartialMolecule> &molecules);
    bool change(const QVector<PartialMolecule> &molecules);
    bool remove(const QVector<PartialMolecule> &molecules);

    bool change(const QHash<MoleculeID,PartialMolecule> &molecules);

    void rename(const QString &newname);

    bool refersTo(MoleculeID molid) const;

    bool contains(const PartialMolecule &molecule) const;

private:
    /** Incremint used to increment that ID number */
    static SireBase::Incremint molgroup_incremint;

    void reindex();

    /** The array of all of the molecules in this group */
    QVector<PartialMolecule> mols;

    /** Hash indexing the molecules in the array, allowing
        rapid lookup by MoleculeID */
    QHash<MoleculeID, int> idx;

    /** The name of this group */
    QString nme;

    /** The ID and version number of this group */
    SireBase::IDMajMinVersion id_and_version;
};

/** Return the name of the group */
inline const QString& MoleculeGroupPvt::name() const
{
    return nme;
}

/** Return the ID of the group */
inline MoleculeGroupID MoleculeGroupPvt::ID() const
{
    return MoleculeGroupID(id_and_version.ID());
}

/** Return the version of the group */
inline const Version& MoleculeGroupPvt::version() const
{
    return id_and_version.version();
}

/** Return the array of all molecules in the group */
inline const QVector<PartialMolecule>& MoleculeGroupPvt::molecules() const
{
    return mols;
}

/** Return whether or not we contain the molecule 'molecule' */
inline bool MoleculeGroupPvt::refersTo(MoleculeID molid) const
{
    return idx.contains(molid);
}

} //end of namespace detail

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
    MoleculeGroup(const QString &name,
                  const QVector<PartialMolecule> &molecules);

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

    const QVector<PartialMolecule>& molecules() const;

    QSet<MoleculeID> moleculeIDs() const;

    int count() const;

    bool add(const PartialMolecule &molecule);
    bool change(const PartialMolecule &molecule);
    bool remove(const PartialMolecule &molecule);

    bool remove(MoleculeID molid);

    bool add(const QVector<PartialMolecule> &molecules);
    bool change(const QVector<PartialMolecule> &molecules);
    bool remove(const QVector<PartialMolecule> &molecules);

    bool remove(const QSet<MoleculeID> &molids);

    bool change(const QHash<MoleculeID,PartialMolecule> &molecules);

    void rename(const QString &newname);

    bool refersTo(MoleculeID molid) const;

    bool contains(const PartialMolecule &molecule) const;

private:
    /** Implicit pointer to the data of this object */
    QSharedDataPointer<detail::MoleculeGroupPvt> d;
};

/** Return the name of the group */
inline const QString& MoleculeGroup::name() const
{
    return d->name();
}

/** Return the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
inline const PartialMolecule& MoleculeGroup::operator[](MoleculeID molid) const
{
    return d->molecule(molid);
}

/** Return the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
inline const PartialMolecule& MoleculeGroup::at(MoleculeID molid) const
{
    return d->molecule(molid);
}

/** Return the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
inline const PartialMolecule& MoleculeGroup::molecule(MoleculeID molid) const
{
    return d->molecule(molid);
}

/** Return the ID number of this group */
inline MoleculeGroupID MoleculeGroup::ID() const
{
    return d->ID();
}

/** Return the version of this group */
inline const Version& MoleculeGroup::version() const
{
    return d->version();
}

/** Return the array of all molecules in this group */
inline const QVector<PartialMolecule>& MoleculeGroup::molecules() const
{
    return d->molecules();
}

/** Return whether or not this group contains any part
    of the molecule with ID == molid */
inline bool MoleculeGroup::refersTo(MoleculeID molid) const
{
    return d->refersTo(molid);
}

/** Return whether or not this group contains all of
    any version of the molecule 'molecule' */
inline bool MoleculeGroup::contains(const PartialMolecule &molecule) const
{
    return d->contains(molecule);
}

/** Return the number of molecules in the group */
inline int MoleculeGroup::count() const
{
    return molecules().count();
}

}

Q_DECLARE_METATYPE(SireMol::detail::MoleculeGroupPvt);
Q_DECLARE_METATYPE(SireMol::MoleculeGroup);

SIRE_END_HEADER

#endif
