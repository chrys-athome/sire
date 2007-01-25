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

#include <QDataStream>

#include "bond.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<Bond> r_bond;

/** Serialise a Bond to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Bond &bond)
{
    writeHeader(ds, r_bond, 1) << bond.atoms[0] << bond.atoms[1];

    return ds;
}

/** Deserialise a Bond from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Bond &bond)
{
    VersionID v = readHeader(ds, r_bond);

    if (v == 1)
    {
        ds >> bond.atoms[0] >> bond.atoms[1];
    }
    else
        throw version_error(v, "1", r_bond, CODELOC);

    return ds;
}

/** Construct a null Bond */
Bond::Bond()
{}

/** Construct a Bond between these two AtomIndexes. The atoms must not be the same,
    or else an exception will be thrown. Note that the Bond will sort the atoms
    so that the Bond between atom0-atom1 will be the same as the Bond
    between atom1-atom0.
*/
Bond::Bond(const AtomIndex &atom0, const AtomIndex &atom1)
{
    this->create(atom0,atom1);
}

/** Construct a Bond between the two AtomIndexes held in the tuple object */
Bond::Bond(const tuple<AtomIndex,AtomIndex> &atoms)
{
    this->create( atoms.get<0>(), atoms.get<1>() );
}

/** Construct a bond between two atoms called 'atom0' and 'atom1' in residue with
    residue number 'resnum' */
Bond::Bond(const QString &atom0, const QString &atom1, ResNum resnum)
{
    this->create( AtomIndex(atom0, resnum), AtomIndex(atom1, resnum) );
}

/** Construct a bond between atoms AtomIndex(nam0,res0) and AtomIndex(nam1,res1) */
Bond::Bond(const QString &nam0, ResNum res0, const QString &nam1, ResNum res1)
{
    this->create( AtomIndex(nam0,res0), AtomIndex(nam1,res1) );
}

/** Copy constructor */
Bond::Bond(const Bond &other)
{
    operator=(other);
}

/** Destructor */
Bond::~Bond()
{}

/** Return a string representation of the Bond */
QString Bond::toString() const
{
    return QObject::tr("%1-%2").arg(atoms[0].toString(),atoms[1].toString());
}

/** Return the atom in the bond that is in the residue with number 'resnum'.
    If both atoms in the bond are in this residue then only the first will
    be returned.

    \throw SireMol::missing_residue
*/
const AtomIndex& Bond::operator[](ResNum resnum) const
{
    if ( atom0().resNum() == resnum )
        return atom0();
    else if ( atom1().resNum() != resnum )
        throw SireMol::missing_residue( QObject::tr(
                "The bond %1 does not contain any atoms from the residue "
                "with number \"%2\".")
                    .arg(toString()).arg(resnum), CODELOC );

    return atom1();
}

/** Return the atom in the bond that is in the residue with number 'resnum'.
    If both atoms in the bond are in this residue then only the first will
    be returned.

    \throw SireMol::missing_residue
*/
const AtomIndex& Bond::at(ResNum resnum) const
{
    return this->operator[](resnum);
}

/** Return the atom in the bond that is in the residue with number 'resnum'.
    If both atoms in the bond are in this residue then only the first will
    be returned.

    \throw SireMol::missing_residue
*/
const AtomIndex& Bond::atom(ResNum resnum) const
{
    return this->operator[](resnum);
}

/** Construct the Bond between these two AtomIndexes. The atoms must not be the same,
    or else an exception will be thrown. Note that the Bond will sort the atoms
    so that the Bond between atom0-atom1 will be the same as the Bond
    between atom1-atom0.

    \throw SireMol::duplicate_atom
*/
void Bond::create(const AtomIndex &atom0, const AtomIndex &atom1)
{
    if (atom0 < atom1)
    {
        atoms[0] = atom0;
        atoms[1] = atom1;
    }
    else if (atom0 > atom1)
    {
        atoms[0] = atom1;
        atoms[1] = atom0;
    }
    else
    {
        throw SireMol::duplicate_atom(QObject::tr(
                    "Cannot create a Bond between the same atoms! %1-%2")
                          .arg(atom0.toString(),atom1.toString()), CODELOC);
    }
}

/** Return the index of the atom which is not 'atm' (e.g. return the other atom!)
    This throws an exception if 'atm' is not in this Bond. */
const AtomIndex& Bond::other(const AtomIndex &atm) const
{
    if (atm == atoms[0])
        return atoms[1];
    else if (atm == atoms[1])
        return atoms[0];
    else
        throw SireMol::missing_atom(QObject::tr(
                "There is not atom %1 in this Bond (%2)")
                      .arg(atm.toString(), this->toString()), CODELOC);
}

/** Return the atomindex of the atom that is in the other residue to 'res'. This
    only works properly if this is an inter-residue bond, and 'res' is the residue
    of one of the atoms in this bond. */
const AtomIndex& Bond::other(ResNum res) const
{
    if (atoms[0].resNum() == res)
        return atoms[1];
    else
        return atoms[0];
}

/** Return the residue number of the other residue in an inter-residue bond. This
    returns the same residue number if this is an intra-residue bond */
ResNum Bond::otherRes(ResNum resnum) const
{
    if (atoms[0].resNum() == resnum)
        return atoms[1].resNum();
    else
        return atoms[0].resNum();
}

/** Return a copy of this bond where the residue number 'oldnum' has been
    changed to 'newnum' */
Bond Bond::renumber(ResNum oldnum, ResNum newnum) const
{
    ResNum res0 = atoms[0].resNum();
    if (res0 == oldnum)
        res0 = newnum;

    ResNum res1 = atoms[1].resNum();
    if (res1 == oldnum)
        res1 = newnum;

    return Bond( AtomIndex(atoms[0].name(),res0),
                 AtomIndex(atoms[1].name(),res1) );
}

/** Test for equality */
bool Bond::operator==(const Bond &other) const
{
    return atoms[0] == other.atoms[0] and atoms[1] == other.atoms[1];
}

/** Test for inequality */
bool Bond::operator!=(const Bond &other) const
{
    return not operator==(other);
}

/** Assignment operator */
const Bond& Bond::operator=(const Bond &other)
{
    for (int i=0; i<2; ++i)
        atoms[i] = other.atoms[i];

    return *this;
}

/** A Bond is greater than another if atm0 of the bond is greater than
    atm0 of the other bond. If both atm0s are equal, then the test is on atm1 */
bool Bond::operator>(const Bond &other) const
{
    return (atoms[0] > other.atoms[0])
            or ( atoms[0] == other.atoms[0] and atoms[1] > other.atoms[1] );
}

/** A Bond is greater than another if atm0 of the bond is greater than
    atm0 of the other bond. If both atm0s are equal, then the test is on atm1 */
bool Bond::operator>=(const Bond &other) const
{
    return (atoms[0] > other.atoms[0])
            or ( atoms[0] == other.atoms[0] and atoms[1] >= other.atoms[1] );
}

/** A Bond is less than another if atm0 of the bond is less than
    atm0 of the other bond. If both atm0s are equal, then the test is on atm1 */
bool Bond::operator<(const Bond &other) const
{
    return not operator>=(other);
}

/** A Bond is less than another if atm0 of the bond is less than
    atm0 of the other bond. If both atm0s are equal, then the test is on atm1 */
bool Bond::operator<=(const Bond &other) const
{
    return not operator>(other);
}
