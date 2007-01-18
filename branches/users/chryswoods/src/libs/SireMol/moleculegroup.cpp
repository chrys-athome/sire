
#include "SireMol/qhash_siremol.h"

#include "moleculegroup.h"

#include "moleculeversion.h"
#include "SireMol/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireMol::detail;

using namespace SireStream;
using namespace SireBase;

///////////
/////////// Implementation of MoleculeGroupPvt
///////////

Incremint MoleculeGroupPvt::molgroup_incremint;

static const RegisterMetaType<MoleculeGroupPvt> r_molgrouppvt;

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
                             
        molgrouppvt.reindex();
    }
    else
        throw version_error(v, "1", r_molgrouppvt, CODELOC);

    return ds;
}

/** Null constructor */
MoleculeGroupPvt::MoleculeGroupPvt()
                 : QSharedData(), id_and_version(&molgroup_incremint)
{}

/** Construct a named, empty group */
MoleculeGroupPvt::MoleculeGroupPvt(const QString &name)
                 : QSharedData(), nme(name), id_and_version(&molgroup_incremint)
{}

/** Reindex this group */
void MoleculeGroupPvt::reindex()
{
    idx.clear();

    int nmols = mols.count();

    if (nmols > 0)
    {
        idx.reserve(nmols);

        const Molecule *molarray = mols.constData();

        for (int i=0; i<nmols; ++i)
        {
            idx.insert(molarray[i].ID(), i);
        }
    }
}

/** Construct a named group that contains 'molecules'

    \throw SireMol::duplicate_molecule
*/
MoleculeGroupPvt::MoleculeGroupPvt(const QString &name,
                                   const QVector<Molecule> &molecules)
                 : QSharedData(),
                   mols(molecules),
                   nme(name),
                   id_and_version(&molgroup_incremint)
{
    this->reindex();
}

/** Copy constructor */
MoleculeGroupPvt::MoleculeGroupPvt(const MoleculeGroupPvt &other)
                  : QSharedData(),
                    mols(other.mols),
                    idx(other.idx),
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
    idx = other.idx;
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

/** Return the index */
const QHash<MoleculeID,int>& MoleculeGroupPvt::index() const
{
    return idx;
}

/** Return the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
const Molecule& MoleculeGroupPvt::molecule(MoleculeID molid) const
{
    QHash<MoleculeID,int>::const_iterator it = idx.find(molid);

    if (it == idx.end())
        throw SireMol::missing_molecule( QObject::tr(
                "There is no molecule with ID == %1 in the group "
                "\"%2\" (%3)")
                    .arg(molid).arg(this->name(), id_and_version.toString()),
                        CODELOC );

    return mols.constData()[it.value()];
}

/** Add the molecule 'molecule' to this group. This throws an  
    exception if this molecule already exists in this group.
     
    \throw SireMol::duplicate_molecule
*/
void MoleculeGroupPvt::add(const Molecule &molecule)
{
    QHash<MoleculeID,int>::const_iterator it = idx.constFind(molecule.ID());

    if (it == idx.constEnd())
    {
        //this is a new molecule
        idx.insert(molecule.ID(), mols.count());
        mols.append(molecule);

        id_and_version.incrementMajor();
    }
    else
    {
        //this replaces an existing copy of the molecule
        mols.data()[it.value()] = molecule;

        id_and_version.incrementMinor();
    }
}

/** Change the molecule 'molecule' - this does nothing if this
    molecule is not in this group */
void MoleculeGroupPvt::change(const Molecule &molecule)
{
    QHash<MoleculeID,int>::const_iterator it = idx.constFind(molecule.ID());

    if (it != idx.constEnd())
    {
        //replace the existing molecule (if it has indeed changed)
        if (molecule.version() != mols.constData()[it.value()].version())
        {
            mols.data()[it.value()] = molecule;
            id_and_version.incrementMinor();
        }
    }
}

/** Remove the molecule 'molecule' - this does nothing if this
    molecule is not in this group */
void MoleculeGroupPvt::remove(const Molecule &molecule)
{
    MoleculeID molid = molecule.ID();

    QHash<MoleculeID,int>::const_iterator it = idx.constFind(molid);

    if ( it != idx.constEnd() )
    {
        int index = it.value();

        //remove the molecule from this list
        mols.remove(index);

        //remove the molecule from the index
        idx.remove(molid);

        //reindex the remaining molecules
        for (QHash<MoleculeID,int>::iterator it = idx.begin();
             it != idx.end();
             ++it)
        {
            if (it.value() > index)
                it.value() = it.value() - 1;
        }

        id_and_version.incrementMajor();
    }
}

/** Add a whole load of molecules to this group - this will 
    throw an exception if any of the molecules already exist
    in this group.
     
    \throw SireMol::duplicate_molecule
*/
void MoleculeGroupPvt::add(const QVector<Molecule> &molecules)
{
    //are there any duplicates?
    int n = molecules.count();
    const Molecule *molecules_array = 

    bool changed = false;
    bool added = false;

    int nmax = mols.count() + molecules.count();

    for (QVector<Molecule>::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        QHash<MoleculeID,int>::const_iterator it2 = idx.constFind(it->ID());

        if (it2 == idx.constEnd())
        {
            //this is a new molecule - assume that they all are
            mols.reserve(nmax);
            mols.append(*it);

            added = true;
        }
        else
        {
            //replace an existing molecule?
            int index = it2.value();

            if (mols.constData()[index].ID() != it->ID())
            {
                mols.data()[index] = *it;
                changed = true;
            }
        }
    }

    if (added)
    {
        this->reindex();
        id_and_version.incrementMajor();
    }
    else if (changed)
        id_and_version.incrementMinor();
}

/** Change a whole load of molecules */
void MoleculeGroupPvt::change(const QVector<Molecule> &molecules)
{
    bool changed = false;

    for (QVector<Molecule>::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        //change the molecule
        QHash<MoleculeID,int>::const_iterator it2 = idx.constFind(it->ID());

        if (it2 != idx.constEnd())
        {
            mols.data()[it2.value()] = *it;
            changed = true;
        }
    }

    if (changed)
        id_and_version.incrementMinor();
}

/** Remove a whole load of molecules */
void MoleculeGroupPvt::remove(const QVector<Molecule> &molecules)
{
    bool removed = false;

    for (QVector<Molecule>::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        //remove the molecule
        QHash<MoleculeID,int>::const_iterator it2 = idx.constFind(it->ID());

        if (it2 != idx.constEnd())
        {
            mols.remove(it2.value());
            removed = true;
        }
    }

    if (removed)
    {
        this->reindex();
        id_and_version.incrementMajor();
    }
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

static QSharedDataPointer<MoleculeGroupPvt> shared_null( new MoleculeGroupPvt() );

/** Null constructor */
MoleculeGroup::MoleculeGroup() : d(shared_null)
{}

/** Construct an empty, named group */
MoleculeGroup::MoleculeGroup(const QString &name)
              : d( new MoleculeGroupPvt(name) )
{}

/** Construct a named group that contains the passed molecules */
MoleculeGroup::MoleculeGroup(const QString &name,
                             const QVector<Molecule> &molecules)
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

/** Add the molecule 'molecule' to this group. This replaces any existing
    copy of the molecule */
void MoleculeGroup::add(const Molecule &molecule)
{
    if (not this->contains(molecule) or
        this->molecule(molecule.ID()).version() != molecule.version())
    {
        d->add(molecule);
    }
}

/** Change the molecule 'molecule' - this does nothing if this
    molecule is not in this group */
void MoleculeGroup::change(const Molecule &molecule)
{
    if (this->contains(molecule) and
        this->molecule(molecule.ID()).version() != molecule.version())
    {
        d->change(molecule);
    }
}

/** Remove the molecule 'molecule' - this does nothing if this
    molecule is not in this group */
void MoleculeGroup::remove(const Molecule &molecule)
{
    if (this->contains(molecule))
    {
        d->remove(molecule);
    }
}

/** Add a whole load of molecules to this group - this will replace
    any existing copies of a molecule with the copy from
    'molecules' */
void MoleculeGroup::add(const QVector<Molecule> &molecules)
{
    d->add(molecules);
}

/** Change a whole load of molecules */
void MoleculeGroup::change(const QVector<Molecule> &molecules)
{
    for (QVector<Molecule>::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        if (this->contains(*it) and
            this->molecule(it->ID()).version() != it->version())
        {
            d->change(molecules);
            return;
        }
    }
}

/** Remove a whole load of molecules */
void MoleculeGroup::remove(const QVector<Molecule> &molecules)
{
    for (QVector<Molecule>::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        if (this->contains(*it))
        {
            d->remove(molecules);
            return;
        }
    }
}

/** Rename this group - this also changes the ID number of the group */
void MoleculeGroup::rename(const QString &newname)
{
    if (this->name() != newname)
        d->rename(newname);
}
