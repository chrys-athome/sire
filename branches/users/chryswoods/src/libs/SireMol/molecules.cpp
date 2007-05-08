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

#include "molecules.h"
#include "atomselector.h"

#include "SireMol/errors.h"

#include "SireBase/version.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<Molecules> r_mols;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Molecules &mols)
{
    writeHeader(ds, r_mols, 1);

    SharedDataStream sds(ds);

    sds << mols.mols;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Molecules &mols)
{
    VersionID v = readHeader(ds, r_mols);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> mols.mols;
    }
    else
        throw version_error(v, "1", r_mols, CODELOC);

    return ds;
}

/** Null constructor */
Molecules::Molecules()
{}

/** Construct a container that holds only 'molecule' */
Molecules::Molecules(const PartialMolecule &molecule)
{
    if (not molecule.selectedAtoms().isEmpty())
        mols.insert(molecule.ID(), molecule);
}

/** Construct a container that holds only 'molecule' */
Molecules::Molecules(const MolDataView &molecule)
{
    PartialMolecule mol;

    if (not mol.selectedAtoms().isEmpty())
        mols.insert(mol.ID(), mol);
}

static void fixKey(MoleculeID molid, QHash<MoleculeID,PartialMolecule> &mols)
{
    PartialMolecule mol = mols.take(molid);

    qDebug() << CODELOC;
    qDebug() << "Disagreement with key:" << molid << "vs." << mol.ID();

    if (not mol.selectedAtoms().isEmpty())
    {
        QHash<MoleculeID,PartialMolecule>::const_iterator it = mols.constFind(mol.ID());

        if (it != mols.constEnd())
        {
            //make sure that this molecule has the right ID!
            if (it.key() != it->ID())
                fixKey(it.key(), mols);

            it = mols.constFind(mol.ID());

            if (it != mols.constEnd())
            {
                BOOST_ASSERT(it.key() == it->ID());
                mols.insert(mol.ID(), it->selection().add(mol.selectedAtoms()));
                return;
            }
        }

        mols.insert(mol.ID(), mol);
        return;
    }
}

/** Construct a container that holds all of the molecules in 'molecules' */
Molecules::Molecules(const QHash<MoleculeID,PartialMolecule> &molecules)
          : mols(molecules)
{
    for (QHash<MoleculeID,PartialMolecule>::const_iterator it = molecules.begin();
         it != molecules.end();
         ++it)
    {
        if (it.key() != it->ID())
        {
            //disagreement with the ID number and its key in the hash!
            // Remove it and add it back with it's right key!
            fixKey(it.key(), mols);
        }
        else if (it->selectedAtoms().isEmpty())
        {
            //there are no atoms selected in this molecule - remove
            //it from the container
            mols.remove(it.key());
        }
    }
}

/** Copy constructor */
Molecules::Molecules(const Molecules &other)
          : mols(other.mols)
{}

/** Destructor */
Molecules::~Molecules()
{}

/** Return the IDs of all of the molecules in this container */
QSet<MoleculeID> Molecules::moleculeIDs() const
{
    return mols.keys().toSet();
}

/** Return whether this contains all of the atoms of any version
    of the molecule 'molecule' */
bool Molecules::contains(const PartialMolecule &molecule) const
{
    Molecules::const_iterator it = mols.find(molecule.ID());

    if (it != mols.end())
        return it->selectedAtoms().contains(molecule.selectedAtoms());
    else
        return false;
}

/** Ensure that there is at least space for 'count' molecules
    in this container */
void Molecules::reserve(int count)
{
    mols.reserve(count);
}

/** Add the molecule 'molecule' to this container - if it already
    exists then it will be changed to the new version, and
    will have the atom selection from 'molecule' added to it */
bool Molecules::add(const PartialMolecule &molecule)
{
    if (molecule.selectedAtoms().isEmpty())
        return false;

    Molecules::const_iterator it = mols.constFind(molecule.ID());

    if (it == mols.constEnd())
    {
        //the molecule is not in this container - add it
        mols.insert(molecule.ID(), molecule);
        return true;
    }
    else
    {
        if ( it->version() != molecule.version() or
             not it->selectedAtoms().contains(molecule.selectedAtoms()) )
        {
            mols[molecule.ID()] = it->change(molecule)
                                     .selection().add(molecule.selectedAtoms());

            return true;
        }
        else
            return false;
    }
}

/** Add lots of molecules to this container */
bool Molecules::add(const Molecules &other)
{
    bool changed = false;

    for (Molecules::const_iterator it = other.begin();
         it != other.end();
         ++it)
    {
        bool this_changed = this->add(*it);
        changed = changed or this_changed;
    }

    return changed;
}

/** Append a molecule onto the end of this container - this is a
    synonym for 'add' */
bool Molecules::append(const PartialMolecule &molecule)
{
    return this->add(molecule);
}

/** Append lots of molecules onto the end of this container - this is a
    synonym for 'add' */
bool Molecules::append(const Molecules &molecules)
{
    return this->add(molecules);
}

/** Change the molecule in this container to be the same version as 'molecule'

    This does nothing if this molecule is not in this container
*/
bool Molecules::change(const PartialMolecule &molecule)
{
    Molecules::const_iterator it = mols.constFind(molecule.ID());

    if (it != mols.constEnd() and it->version() != molecule.version())
    {
        mols[molecule.ID()] = it->change(molecule);
        return true;
    }
    else
        return false;
}

/** Change lots of molecules */
bool Molecules::change(const Molecules &molecules)
{
    bool changed = false;

    for (Molecules::const_iterator it = molecules.begin();
         it != molecules.end();
         ++it)
    {
        bool this_changed = this->change(*it);
        changed = changed or this_changed;
    }

    return changed;
}

/** Remove the molecule 'molecule' from this container - this removes the
    atoms selected in 'molecule' from the existing copy of any version
    of that molecule that is in this container - this does nothing if these
    atoms or the molecule are not in this container */
bool Molecules::remove(const PartialMolecule &molecule)
{
    Molecules::const_iterator it = mols.constFind(molecule.ID());

    if ( it != mols.constEnd() and
         it->selectedAtoms().intersects(molecule.selectedAtoms()) )
    {
        PartialMolecule &mol = mols[molecule.ID()];

        mol = mol.selection().remove(molecule.selectedAtoms());

        if (mol.selectedAtoms().isEmpty())
            mols.remove(mol.ID());

        return true;
    }
    else
        return false;
}

/** Remove lots of molecules */
bool Molecules::remove(const Molecules &molecules)
{
    bool changed = false;

    for (Molecules::const_iterator it = molecules.begin();
         it != molecules.end();
         ++it)
    {
        bool this_changed = this->remove(*it);
        changed = changed or this_changed;
    }

    return false;
}

/** Completely remove all of the molecule with ID == molid from this container.
    This does nothing if this molecule is not in this container */
bool Molecules::remove(MoleculeID molid)
{
    if (mols.contains(molid))
    {
        mols.remove(molid);
        return true;
    }
    else
        return false;
}

/** Completely remove lots of molecules */
bool Molecules::remove(const QSet<MoleculeID> &molids)
{
    bool changed = false;

    foreach (MoleculeID molid, molids)
    {
        bool this_changed = this->remove(molid);
        changed = changed or this_changed;
    }

    return changed;
}

/** Assert that this container contains at least one atom of
    any version of the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
void Molecules::assertContains(MoleculeID molid) const
{
    if (not this->contains(molid))
        throw SireMol::missing_molecule( QObject::tr(
             "This container does not contain any part of the molecule "
             "with ID == %1.").arg(molid), CODELOC );
}

/** Convienient operator used to access this container like a dictionary
    (molecules indexed by their ID number)

    \throw SireMol::missing_molecule
*/
const PartialMolecule& Molecules::operator[](MoleculeID molid) const
{
    return this->molecule(molid);
}

/** Add the contents of 'other' to this container - where a molecule
    exists in both containers with different versions, the version
    from 'other' will be used. */
Molecules& Molecules::operator+=(const Molecules &other)
{
    for (Molecules::const_iterator it = other.begin();
         it != other.end();
         ++it)
    {
        this->add(*it);
    }

    return *this;
}

/** Remove all parts of all molecules in other from this container */
Molecules& Molecules::operator-=(const Molecules &other)
{
    for (Molecules::const_iterator it = other.begin();
         it != other.end();
         ++it)
    {
        this->remove(*it);
    }

    return *this;
}

/** Add two containers together */
Molecules Molecules::operator+(const Molecules &other) const
{
    Molecules ret(*this);
    ret += other;
    return ret;
}

/** Subtract 'other' from this container */
Molecules Molecules::operator-(const Molecules &other) const
{
    Molecules ret(*this);
    ret -= other;
    return ret;
}

/** Return the union of this container with other - same as
    *this + other */
Molecules Molecules::unite(const Molecules &other) const
{
    return *this + other;
}

/** Return the difference between this container and other,
    same as *this - other */
Molecules Molecules::subtract(const Molecules &other) const
{
    return *this - other;
}

/** Return the intersection of these two containers - this
    returns only the atoms that are in both containers, with
    the same version as in this container */
Molecules Molecules::intersection(const Molecules &other) const
{
    Molecules same_mols;

    for (Molecules::const_iterator it = this->begin();
         it != this->end();
         ++it)
    {
        if (other.contains(it.key()))
        {
            PartialMolecule same_mol = it->selection().intersect(
                                              other[it.key()].selectedAtoms());

            if (not same_mol.selectedAtoms().isEmpty())
                same_mols.add(same_mol);
        }
    }

    return same_mols;
}

/** Return whether there is any atom from any version of any molecule
    in this container which is also in the other container */
bool Molecules::intersects(const Molecules &other) const
{
    for (Molecules::const_iterator it = this->begin();
         it != this->end();
         ++it)
    {
        if ( other.contains(it.key()) and
             other[it.key()].selectedAtoms().intersects(it->selectedAtoms()) )
        {
            return true;
        }
    }

    return false;
}
