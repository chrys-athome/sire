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

#include "SireMol/qhash_siremol.h"

#include "moleculegroup.h"
#include "atomselector.h"

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

using SireBase::Version;

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

    bool operator==(const MoleculeGroupPvt &other) const;
    bool operator!=(const MoleculeGroupPvt &other) const;

    const Molecules& molecules() const;

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

    bool add(const Molecules &molecules);
    bool change(const Molecules &molecules);
    bool remove(const Molecules &molecules);

    void rename(const QString &newname);

    bool refersTo(MoleculeID molid) const;

    bool contains(const PartialMolecule &molecule) const;

private:
    /** Incremint used to increment that ID number */
    static SireBase::Incremint molgroup_incremint;

    /** All of the molecules in this group */
    Molecules mols;

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
inline const Molecules& MoleculeGroupPvt::molecules() const
{
    return mols;
}

/** Return whether or not we contain the molecule 'molecule' */
inline bool MoleculeGroupPvt::refersTo(MoleculeID molid) const
{
    return mols.contains(molid);
}

} //end of namespace detail

} //end of namespace SireMol

Incremint MoleculeGroupPvt::molgroup_incremint;

static const RegisterMetaType<MoleculeGroupPvt> r_molgrouppvt( MAGIC_ONLY,
                                            "SireMol::detail::MoleculeGroupPvt" );

/** Serialise to a binary data stream */
QDataStream SIRESTREAM_EXPORT &operator<<(QDataStream &ds,
                                          const MoleculeGroupPvt &molgrouppvt)
{
    writeHeader(ds, r_molgrouppvt, 1);

    SharedDataStream(ds) << molgrouppvt.mols
                         << molgrouppvt.nme
                         << molgrouppvt.id_and_version;

    //no need to stream the index as this can be reconstructed

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIRESTREAM_EXPORT &operator>>(QDataStream &ds,
                                          MoleculeGroupPvt &molgrouppvt)
{
    VersionID v = readHeader(ds, r_molgrouppvt);

    if (v == 1)
    {
        SharedDataStream(ds) >> molgrouppvt.mols
                             >> molgrouppvt.nme
                             >> molgrouppvt.id_and_version;
    }
    else
        throw version_error(v, "1", r_molgrouppvt, CODELOC);

    return ds;
}

/** Null constructor */
MoleculeGroupPvt::MoleculeGroupPvt()
                 : QSharedData(), id_and_version(&molgroup_incremint)
{
    id_and_version.incrementID();
}

/** Construct a named, empty group */
MoleculeGroupPvt::MoleculeGroupPvt(const QString &name)
                 : QSharedData(), nme(name), id_and_version(&molgroup_incremint)
{
    id_and_version.incrementID();
}

/** Construct a named group that contains 'molecules'

    \throw SireMol::duplicate_molecule
*/
MoleculeGroupPvt::MoleculeGroupPvt(const QString &name,
                                   const Molecules &molecules)
                 : QSharedData(),
                   mols(molecules),
                   nme(name),
                   id_and_version(&molgroup_incremint)
{
    id_and_version.incrementID();
}

/** Copy constructor */
MoleculeGroupPvt::MoleculeGroupPvt(const MoleculeGroupPvt &other)
                  : QSharedData(),
                    mols(other.mols),
                    nme(other.nme),
                    id_and_version(other.id_and_version)
{}

/** Destructor */
MoleculeGroupPvt::~MoleculeGroupPvt()
{}

/** Assignment operator */
MoleculeGroupPvt& MoleculeGroupPvt::operator=(const MoleculeGroupPvt &other)
{
    mols = other.mols;
    nme = other.nme;
    id_and_version = other.id_and_version;

    return *this;
}

/** Comparison operator - two groups are equal if they have the same
    ID and version */
bool MoleculeGroupPvt::operator==(const MoleculeGroupPvt &other) const
{
    return this == &other or
           (this->ID() == other.ID() and
            this->version() == other.version());
}

/** Comparison operator - two groups are different if they have
    different ID numbers or versions */
bool MoleculeGroupPvt::operator!=(const MoleculeGroupPvt &other) const
{
    return this != &other and
           (this->ID() != other.ID() or
            this->version() != other.version());
}

/** Return the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
const PartialMolecule& MoleculeGroupPvt::molecule(MoleculeID molid) const
{
    return mols[molid];
}

/** Return whether or not this group contains all of any version
    of the molecule 'molecule' */
bool MoleculeGroupPvt::contains(const PartialMolecule &molecule) const
{
    return mols.contains(molecule);
}

/** Return the set of IDs of all of the molecules in this group */
QSet<MoleculeID> MoleculeGroupPvt::moleculeIDs() const
{
    return mols.moleculeIDs();
}

/** Add the molecule 'molecule' to this group.
    This updates any current part of the molecule
    already in this group to the same version as
    'molecule' and adds any atoms in 'molecule'
    that are not already in the group.
*/
bool MoleculeGroupPvt::add(const PartialMolecule &molecule)
{
    if (mols.add(molecule))
    {
        id_and_version.incrementMajor();
        return true;
    }
    else
        return false;
}

/** Change the molecule 'molecule' - this does nothing if this
    molecule is not in this group */
bool MoleculeGroupPvt::change(const PartialMolecule &molecule)
{
    if (mols.change(molecule))
    {
        id_and_version.incrementMinor();
        return true;
    }
    else
        return false;
}

/** Completely remove the molecule with ID == molid */
bool MoleculeGroupPvt::remove(MoleculeID molid)
{
    if (mols.remove(molid))
    {
        id_and_version.incrementMajor();
        return true;
    }
    else
        return false;
}

/** Remove the molecule 'molecule' - this does nothing if this
    molecule is not in this group (this only removes the
    parts of the molecule in 'molecule' */
bool MoleculeGroupPvt::remove(const PartialMolecule &molecule)
{
    if (mols.remove(molecule))
    {
        id_and_version.incrementMajor();
        return true;
    }
    else
        return false;
}

/** Add a whole load of molecules to this group */
bool MoleculeGroupPvt::add(const Molecules &molecules)
{
    if (mols.add(molecules))
    {
        id_and_version.incrementMajor();
        return true;
    }
    else
        return false;
}

/** Change a whole load of molecules */
bool MoleculeGroupPvt::change(const Molecules &molecules)
{
    if (mols.change(molecules))
    {
        id_and_version.incrementMinor();
        return true;
    }
    else
        return false;
}

/** Remove all molecules whose IDs are in 'molids' */
bool MoleculeGroupPvt::remove(const QSet<MoleculeID> &molids)
{
    if (mols.remove(molids))
    {
        id_and_version.incrementMajor();
        return true;
    }
    else
        return false;
}

/** Remove a whole load of molecules */
bool MoleculeGroupPvt::remove(const Molecules &molecules)
{
    if (mols.remove(molecules))
    {
        id_and_version.incrementMajor();
        return true;
    }
    else
        return false;
}

/** Rename this group - this also changes the ID number of the group */
void MoleculeGroupPvt::rename(const QString &newname)
{
    if (nme != newname)
    {
        nme = newname;
        id_and_version.incrementID();
    }
}

///////////
/////////// Implementation of MoleculeGroup
///////////

static const RegisterMetaType<MoleculeGroup> r_group;

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MoleculeGroup &group)
{
    writeHeader(ds, r_group, 1);

    SharedDataStream(ds) << group.d;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MoleculeGroup &group)
{
    VersionID v = readHeader(ds, r_group);

    if (v == 1)
    {
        SharedDataStream(ds) >> group.d;
    }
    else
        throw version_error(v, "1", r_group, CODELOC);

    return ds;
}

/** Null constructor */
MoleculeGroup::MoleculeGroup() : d(new MoleculeGroupPvt())
{}

/** Construct an empty, named group */
MoleculeGroup::MoleculeGroup(const QString &name)
              : d( new MoleculeGroupPvt(name) )
{}

/** Construct a named group that contains the passed molecules */
MoleculeGroup::MoleculeGroup(const QString &name,
                             const Molecules &molecules)
              : d( new MoleculeGroupPvt(name,molecules) )
{}

/** Copy constructor */
MoleculeGroup::MoleculeGroup(const MoleculeGroup &other)
              : d(other.d)
{}

/** Destructor */
MoleculeGroup::~MoleculeGroup()
{}

/** Copy assignment */
MoleculeGroup& MoleculeGroup::operator=(const MoleculeGroup &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator - two groups are equal if they have the same
    ID and version */
bool MoleculeGroup::operator==(const MoleculeGroup &other) const
{
    return d == other.d or *d == *(other.d);
}

/** Comparison operator - two groups are different if they have
    different ID numbers or versions */
bool MoleculeGroup::operator!=(const MoleculeGroup &other) const
{
    return d != other.d and *d != *(other.d);
}

/** Return the name of the group */
const QString& MoleculeGroup::name() const
{
    return d->name();
}

/** Return the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
const PartialMolecule& MoleculeGroup::operator[](MoleculeID molid) const
{
    return d->molecule(molid);
}

/** Return the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
const PartialMolecule& MoleculeGroup::at(MoleculeID molid) const
{
    return d->molecule(molid);
}

/** Return the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
const PartialMolecule& MoleculeGroup::molecule(MoleculeID molid) const
{
    return d->molecule(molid);
}

/** Return the ID number of this group */
MoleculeGroupID MoleculeGroup::ID() const
{
    return d->ID();
}

/** Return the version of this group */
const Version& MoleculeGroup::version() const
{
    return d->version();
}

/** Return the array of all molecules in this group */
const Molecules& MoleculeGroup::molecules() const
{
    return d->molecules();
}

/** Return whether or not this group contains any part
    of the molecule with ID == molid */
bool MoleculeGroup::refersTo(MoleculeID molid) const
{
    return d->refersTo(molid);
}

/** Return whether or not this group contains all of
    any version of the molecule 'molecule' */
bool MoleculeGroup::contains(const PartialMolecule &molecule) const
{
    return d->contains(molecule);
}

/** Return whether this group is empty */
bool MoleculeGroup::isEmpty() const
{
    return molecules().isEmpty();
}

/** Return the number of molecules in the group */
int MoleculeGroup::count() const
{
    return molecules().count();
}

/** Return the number of molecules in the group */
int MoleculeGroup::nMolecules() const
{
    return molecules().count();
}

/** Add the molecule 'molecule' to this group. */
bool MoleculeGroup::add(const PartialMolecule &molecule)
{
    return d->add(molecule);
}

/** Change the molecule 'molecule' - this does nothing if this
    molecule is not in this group */
bool MoleculeGroup::change(const PartialMolecule &molecule)
{
    if (this->refersTo(molecule.ID()) and
        this->molecule(molecule.ID()).version() != molecule.version())
    {
        return d->change(molecule);
    }
    else
        return false;
}

/** Remove the molecule 'molecule' - this does nothing if this
    molecule is not in this group */
bool MoleculeGroup::remove(const PartialMolecule &molecule)
{
    if (this->refersTo(molecule.ID()))
    {
        return d->remove(molecule);
    }
    else
        return false;
}

/** Add a whole load of molecules to this group - this will replace
    any existing copies of a molecule with the copy from
    'molecules' */
bool MoleculeGroup::add(const Molecules &molecules)
{
    return d->add(molecules);
}

/** Change a whole load of molecules */
bool MoleculeGroup::change(const Molecules &molecules)
{
    return d->change(molecules);
}

/** Remove a whole load of molecules */
bool MoleculeGroup::remove(const Molecules &molecules)
{
    return d->remove(molecules);
}

/** Remove all parts of the molecule whose ID == molid */
bool MoleculeGroup::remove(MoleculeID molid)
{
    return d->remove(molid);
}

/** Remove all parts of all molecules whose IDs are in 'molids' */
bool MoleculeGroup::remove(const QSet<MoleculeID> &molids)
{
    return d->remove(molids);
}

/** Rename this group - this also changes the ID number of the group */
void MoleculeGroup::rename(const QString &newname)
{
    if (this->name() != newname)
        d->rename(newname);
}

/** Return the set of IDs of all of the molecules in this group */
QSet<MoleculeID> MoleculeGroup::moleculeIDs() const
{
    return d->moleculeIDs();
}
