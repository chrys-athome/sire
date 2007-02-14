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

#include "dihedral.h"
#include "SireMol/errors.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<Dihedral> r_dihedral;

/** Serialise a dihedral to a datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Dihedral &dih)
{
    writeHeader(ds, r_dihedral, 1) << dih.atoms[0] << dih.atoms[1]
                                   << dih.atoms[2] << dih.atoms[3];
    return ds;
}

/** Deserialise a dihedral from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Dihedral &dih)
{
    VersionID v = readHeader(ds, r_dihedral);

    if (v == 1)
    {
        ds >> dih.atoms[0] >> dih.atoms[1] >> dih.atoms[2] >> dih.atoms[3];
    }
    else
        throw version_error(v, "1", r_dihedral, CODELOC);

    return ds;
}

/** Hash a dihedral */
uint SIREMOL_EXPORT qHash(const Dihedral &dih)
{
    //hash each of the atoms in turn...
    uint atm0 = qHash(dih[0]);
    uint atm1 = qHash(dih[1]);
    uint atm2 = qHash(dih[2]);
    uint atm3 = qHash(dih[3]);

    //assume 32bit uint
    //give 8 bits to the hash of each atom...
    return ( atm0 << 24 ) | ( (atm1<<16) & 0x00FF0000 )
               | ( (atm2<<8) & 0x0000FF00 ) | ( atm3 & 0x000000FF );
}

/** Construct a null Dihedral */
Dihedral::Dihedral()
{}

/** Construct a Dihedral between these four AtomIndexes. The atoms must not be the same,
    or else an exception will be thrown. Note that the Dihedral will sort the atoms
    so that the Dihedral between atom0-atom1-atom2-atom3 will be the same as the Dihedral
    between atom3-atom2-atom1-atom0.
*/
Dihedral::Dihedral(const AtomIndex &atom0, const AtomIndex &atom1,
                   const AtomIndex &atom2, const AtomIndex &atom3)
{
    this->create(atom0,atom1,atom2,atom3);
}

/** Construct a Dihedral from a tuple of four AtomIndex objects */
Dihedral::Dihedral( const tuple<AtomIndex,AtomIndex,AtomIndex,AtomIndex> &tuple )
{
    this->create( tuple.get<0>(), tuple.get<1>(), tuple.get<2>(), tuple.get<3>() );
}

/** Construct a dihedral between the four atoms called atom0-atom1-atom2-atom3 in the residue with
    residue number 'resnum' */
Dihedral::Dihedral(const QString &atom0, const QString &atom1,
                   const QString &atom2, const QString &atom3, ResNum resnum)
{
    this->create( AtomIndex(atom0, resnum), AtomIndex(atom1, resnum),
                  AtomIndex(atom2, resnum), AtomIndex(atom3, resnum) );
}

/** Construct a dihedral between the atoms with the specified names in the specified
    residues */
Dihedral::Dihedral(const QString &atom0, ResNum res0, const QString &atom1, ResNum res1,
                   const QString &atom2, ResNum res2, const QString &atom3, ResNum res3)
{
    this->create( AtomIndex(atom0,res0), AtomIndex(atom1,res1),
                  AtomIndex(atom2,res2), AtomIndex(atom3,res3) );
}

/** Copy constructor */
Dihedral::Dihedral(const Dihedral &other)
{
    operator=(other);
}

/** Destructor */
Dihedral::~Dihedral()
{}

/** Construct the Dihedral between these four AtomIndexes. The atoms must not be the same,
    or else an exception will be thrown. Note that the Dihedral will sort the atoms
    so that the Dihedral between atom0-atom1-atom2-atom3 will be the same as the Dihedral
    between atom3-atom2-atom1-atom0.

    \throw SireMol::duplicate_atoms
*/
void Dihedral::create(const AtomIndex &atom0, const AtomIndex &atom1,
                      const AtomIndex &atom2, const AtomIndex &atom3)
{
    if (atom0 == atom1  or  atom0 == atom2  or  atom0 == atom3)
    {
        throw SireMol::duplicate_atom(QObject::tr(
                    "Cannot create a Dihedral between the same atoms! %1-%2-%3-%4")
                          .arg(atom0.toString(),atom1.toString())
                          .arg(atom2.toString(),atom3.toString()), CODELOC);
    }

    if (atom0 < atom3)
    {
        atoms[0] = atom0;
        atoms[1] = atom1;
        atoms[2] = atom2;
        atoms[3] = atom3;
    }
    else
    {
        atoms[0] = atom3;
        atoms[1] = atom2;
        atoms[2] = atom1;
        atoms[3] = atom0;
    }
}

/** Test for equality */
bool Dihedral::operator==(const Dihedral &other) const
{
    return atoms[0] == other.atoms[0] and atoms[1] == other.atoms[1] and
           atoms[2] == other.atoms[2] and atoms[3] == other.atoms[3];
}

/** Test for inequality */
bool Dihedral::operator!=(const Dihedral &other) const
{
    return not operator==(other);
}

/** Assignment operator */
const Dihedral& Dihedral::operator=(const Dihedral &other)
{
    for (int i=0; i<4; ++i)
        atoms[i] = other.atoms[i];

    return *this;
}

/** A Dihedral is greater than another if atm0 of the dihedral is greater than
    atm0 of the other dihedral. If both atm0s are equal, then the test is on atm1,
    then on atm2, then on atm3 */
bool Dihedral::operator>(const Dihedral &other) const
{
    return (atoms[0] > other.atoms[0])
             or ( atoms[0] == other.atoms[0] and atoms[1] > other.atoms[1] )
              or (atoms[0] == other.atoms[0] and atoms[1] == other.atoms[1]
                  and atoms[2] > other.atoms[2])

            or (atoms[0] == other.atoms[0] and atoms[1] == other.atoms[1]
                and atoms[2] == other.atoms[2]
                    and atoms[3] > other.atoms[3]);
}

/** A Dihedral is greater than another if atm0 of the dihedral is greater than
    atm0 of the other dihedral. If both atm0s are equal, then the test is on atm1,
    then on atm2, then on atm3 */
bool Dihedral::operator>=(const Dihedral &other) const
{
    return (atoms[0] > other.atoms[0])
             or ( atoms[0] == other.atoms[0] and atoms[1] > other.atoms[1] )
              or (atoms[0] == other.atoms[0] and atoms[1] == other.atoms[1]
                  and atoms[2] > other.atoms[2])

            or (atoms[0] == other.atoms[0] and atoms[1] == other.atoms[1]
                and atoms[2] == other.atoms[2]
                    and atoms[3] >= other.atoms[3]);
}

/** A Dihedral is less than another if atm0 of the dihedral is less than
    atm0 of the other dihedral. If both atm0s are equal, then the test is on atm1,
    then on atm2, then on atm3 */
bool Dihedral::operator<(const Dihedral &other) const
{
    return not operator>=(other);
}

/** A Dihedral is less than another if atm0 of the dihedral is less than
    atm0 of the other dihedral. If both atm0s are equal, then the test is on atm1,
    then on atm2, then on atm3s */
bool Dihedral::operator<=(const Dihedral &other) const
{
    return not operator>(other);
}

/** Return a string representation of the Dihedral */
QString Dihedral::toString() const
{
    return QObject::tr("%1-%2-%3-%4")
        .arg(atoms[0].toString(),atoms[1].toString(),
             atoms[2].toString(),atoms[3].toString());
}
