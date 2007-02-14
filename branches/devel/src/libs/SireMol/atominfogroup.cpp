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

#include "qhash_siremol.h"

#include <QSet>
#include <QVector>

#include "atominfogroup.h"
#include "atomid.h"
#include "atom.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<AtomInfoGroup> r_aigroup;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const AtomInfoGroup &aigroup)
{
    writeHeader(ds, r_aigroup, 1);

    SharedDataStream(ds) << aigroup.atms;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AtomInfoGroup &aigroup)
{
    VersionID v = readHeader(ds, r_aigroup);

    if (v == 1)
    {
        SharedDataStream(ds) >> aigroup.atms;
    }
    else
        throw version_error(v, "1", r_aigroup, CODELOC);

    return ds;
}

/** Null constructor */
AtomInfoGroup::AtomInfoGroup()
{}

/** Construct an AtomInfoGroup that holds the info for 'size' atoms. These
    will be default constructed. */
AtomInfoGroup::AtomInfoGroup(int size)
              : atms(size)
{}

/** Construct an AtomInfoGroup that holds the info for 'size' atoms, all
    of which will be equal to 'value' */
AtomInfoGroup::AtomInfoGroup(int size, const AtomInfo &value)
              : atms(size, value)
{}

/** Construct an AtomInfoGroup that holds a copy of 'atoms' */
AtomInfoGroup::AtomInfoGroup(const QVector<AtomInfo> &atoms)
              : atms(atoms)
{}

/** Construct from an array of Atoms */
AtomInfoGroup::AtomInfoGroup(const QVector<Atom> &atoms)
{
    int nats = atoms.count();
    if (nats > 0)
    {
        atms.reserve(atoms.count());

        AtomInfo *atmsdata = atms.data();
        const Atom *atomdata = atoms.constData();

        for (int i=0; i<nats; ++i)
            atmsdata[i] = atomdata[i];
    }
}

/** Copy constructor */
AtomInfoGroup::AtomInfoGroup(const AtomInfoGroup &other)
              : atms(other.atms)
{}

/** Destructor */
AtomInfoGroup::~AtomInfoGroup()
{}

/** Assignment operator */
AtomInfoGroup& AtomInfoGroup::operator=(const AtomInfoGroup &other)
{
    atms = other.atms;
    return *this;
}

/** Assign from a vector of AtomInfo objects */
AtomInfoGroup& AtomInfoGroup::operator=(const QVector<AtomInfo> &atominfos)
{
    atms = atominfos;
    return *this;
}

/** Comparison operator */
bool AtomInfoGroup::operator==(const AtomInfoGroup &other) const
{
    return atms == other.atms;
}

/** Comparison operator */
bool AtomInfoGroup::operator!=(const AtomInfoGroup &other) const
{
    return atms != other.atms;
}

/** Assert that the index 'i' is valid

    \throw SireError::invalid_index
*/
void AtomInfoGroup::assertAtomExists(AtomID i) const
{
    if (i >= atms.count())
        throw SireError::invalid_index( QObject::tr(
                "Invalid index in AtomInfoGroup: %1 when nAtoms() == %2")
                    .arg(i).arg(atms.count()), CODELOC );
}

/** Return the AtomInfo at index 'i'

    \throw SireError::invalid_index
*/
const AtomInfo& AtomInfoGroup::at(AtomID i) const
{
    assertAtomExists(i);
    return atms.constData()[i];
}

/** Return the AtomInfo at index 'i'

    \throw SireError::invalid_index
*/
const AtomInfo& AtomInfoGroup::operator[](AtomID i) const
{
    return this->at(i);
}

/** Return the AtomInfo for the atom at index 'i'

    \throw SireError::invalid_index
*/
const AtomInfo& AtomInfoGroup::atom(AtomID i) const
{
    return this->at(i);
}

/** Return the index of the atom with AtomIndex 'atom'

    \throw SireMol::missing_atom
*/
AtomID AtomInfoGroup::indexOf(const AtomIndex &atom) const
{
    int nats = atms.count();

    const AtomInfo *atomarray = atms.constData();

    //slow, linear search!
    for (AtomID i(0); i<nats; ++i)
    {
        if (atom == atomarray[i])
            return i;
    }

    throw SireMol::missing_atom( QObject::tr(
            "There is no atom \"%1\" in this AtomInfoGroup!")
                .arg(atom.toString()), CODELOC );

    return AtomID(0);
}

/** Return an array of all of the AtomInfos in this group, in the
    order that they appear in this group */
QVector<AtomInfo> AtomInfoGroup::atoms() const
{
    return atms;
}

/** Return the AtomInfos for the atoms whose indexes are in 'idxs'

    \throw SireError::invalid_index
*/
QHash<AtomID,AtomInfo> AtomInfoGroup::atoms( const QSet<AtomID> &idxs ) const
{
    QHash<AtomID,AtomInfo> atmhash;
    atmhash.reserve(idxs.count());

    for (QSet<AtomID>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        atmhash.insert( *it, this->atom(*it) );
    }

    return atmhash;
}

/** Return a string representation of this group */
QString AtomInfoGroup::toString() const
{
    return QObject::tr("AtomInfoGroup: nAtoms() == %1").arg(nAtoms());
}

/** Return the number of atoms in this group */
int AtomInfoGroup::nAtoms() const
{
    return atms.count();
}

/** Return the number of atoms in this group that are in residue 'resnum' */
int AtomInfoGroup::nAtoms(ResNum resnum) const
{
    int nresatms = 0;

    for (QVector<AtomInfo>::const_iterator it = atms.begin();
         it != atms.end();
         ++it)
    {
        if (it->resNum() == resnum)
            ++nresatms;
    }

    return nresatms;
}

/** Return whether or not this group contains an atom with index 'atomid' */
bool AtomInfoGroup::contains(AtomID atomid) const
{
    return atomid < nAtoms();
}

/** Return whether or not this group is empty (has no atoms) */
bool AtomInfoGroup::isEmpty() const
{
    return atms.isEmpty();
}

/** Return a raw pointer to the array of AtomInfo objects */
const AtomInfo* AtomInfoGroup::constData() const
{
    return atms.constData();
}

/** Return a vector of all of the numbers of residues that have atoms
    in this group */
QVector<ResNum> AtomInfoGroup::residueNumbers() const
{
    QSet<ResNum> resset;
    QVector<ResNum> resnums;

    for (QVector<AtomInfo>::const_iterator it = atms.begin();
         it != atms.end();
         ++it)
    {
        ResNum resnum = it->resNum();

        if (not resset.contains(resnum))
        {
            resnums.append(resnum);
            resset.insert(resnum);
        }
    }

    return resnums;
}
