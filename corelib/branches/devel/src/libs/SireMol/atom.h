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

#ifndef SIREMOL_ATOM_H
#define SIREMOL_ATOM_H

#include "atominfo.h"
#include "SireMaths/vector.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Atom;
}

QDataStream& operator<<(QDataStream&, const SireMol::Atom&);
QDataStream& operator>>(QDataStream&, SireMol::Atom&);

namespace SireMol
{

/**
Atom is a holds all of the data for a single atom. An Atom is designed for very fast, primarily read-only access. Atoms are grouped together into CutGroups, with the CutGroups storing the atoms in very fast AtomArrays.

The Atom is formed as the child of AtomIndex, Element and Vector. This is so that an Atom is implicitly casted to an AtomIndex, Element or Vector whenever it needs to use this functionality (e.g. it will cast to be a vector for geometry functions, to an AtomIndex when trying to locate it in a molecule, or as an Element whenever querying the element database). The functions 'index()', 'element()' and 'vector()' perform a static cast to explicitly cast the Atom into one of its base types.

The atom also holds an 'atom number'. This number is only used to provide an identifying number to the atom, and will normally come from the coordinate file that was used to generate the coordinates of the atom (e.g. this will correspond to the number in the second column of the PDB file 'ATOM' or 'HETATM' lines). This atom number is in general *not used* within this program, and is only provided so that the atom number read in from the initial coordinates file can be returned when the atom is written out. There are a few small bits of code that use the atom number, e.g. the PSF file reader, which uses the atom number to assign parameters. In general, it is not safe to use the atom number to identify an atom, as it is not guaranteed to be unique within a molecule (unlike the AtomIndex), and there is also no guarantee that the atom numbers are contiguous or consistent within a molecule.

Because the atom number is not important to this code, it is *not* used when checking if two atoms are equal, or to set the order of atoms (e.g. Atom::operator<, Atom::operator== Atom::operator>= etc. ignore the atom number when performing the comparisons). If you wish to make comparisons that include the atom number, then you must do so explicitly, e.g.

I personally do not like atom numbers, as they can lead to obscure errors when setting up simulations. Also, if the atom number is used to identify the atom, then a lot of work is involved to change the atom numbers, e.g. if some atoms are added or deleted. This is particularly hard when the atom number is used to specify the connectivity of the molecule, or to provide the zmatrix - in these cases changing the atom numbers involves changing the entire connectivity list and zmatrix - something that is very error-prone to do by hand.

\code
if (atom0 == atom1 and atom0.number() == atom1.number())
{
    //atom0 and atom1 are equal!
}

if ( atom0 > atom1 or (atom0 == atom1 and atom0.number() >= atom1.number()) )
{
    //atom0 is greater or equal to atom1
}

if ( atom0 < atom1 or (atom0 == atom1 and atom0.number() < atom1.number()) )
{
    //atom0 is less than atom1
}
\endcode

@author Christopher Woods
*/
class SIREMOL_EXPORT Atom : public AtomInfo, public SireMaths::Vector
{

friend QDataStream& ::operator<<(QDataStream&, const Atom&);
friend QDataStream& ::operator>>(QDataStream&, Atom&);

public:
    Atom();
    Atom(const Atom &other);
    Atom(const AtomIndex &atomindex, const Element &element, const Vector &coords);
    Atom(const Atom &other, ResNum resnum);
    Atom(AtomNum atmnum, const AtomIndex &atomindex, const Element &element, const Vector &coords);
    Atom(AtomNum atmnum, const Atom &other);
    Atom(AtomNum atmnum, const Atom &other, ResNum resnum);

    Atom(const QString &name);
    Atom(const QString &name, const QString &element);

    Atom(const QString &name, ResNum resnum);
    Atom(const QString &name, ResNum resnum, const QString &element);

    Atom(const AtomIndex &atm);

    Atom(const QString &name, const Vector &coords);
    Atom(const QString &name, const QString &element, const Vector &coords);
    Atom(const QString &name, const Atom &other);

    Atom(AtomNum num, const QString &name);
    Atom(AtomNum num, const QString &name, const QString &element);

    Atom(AtomNum num, const QString &name, const Vector &coords);
    Atom(AtomNum num, const QString &name, const QString &element, const Vector &coords);

    Atom(const AtomInfo &atominfo, const Vector &coords);

    ~Atom();

    Atom& operator=(const Atom &other);
    Atom& operator=(const Vector &other);

    bool operator==(const Atom &other) const;
    bool operator!=(const Atom &other) const;

    const AtomInfo& info() const;
    AtomInfo& info();

    const Vector& vector() const;
    Vector& vector();

    QString toString() const;

    static bool withinBondRadii(const Atom &atm0, const Atom &atm1, double err=0.0);
};

/** Return whether two atoms are equal */
inline bool Atom::operator==(const Atom &other) const
{
    return ( AtomInfo::operator==(other) and
             Vector::operator==(other) );
}

/** Test for inequality */
inline bool Atom::operator!=(const Atom &other) const
{
    return ( AtomInfo::operator!=(other) or
             Vector::operator!=(other) );
}

/** Just copy the Vector part of the atom - this is a convenience function that
    allows an atom to be moved (alternatively, you can modify the vector part of
    this atom via the reference returned by Atom::vector) */
inline Atom& Atom::operator=(const Vector &other)
{
    Vector::operator=(other);
    return *this;
}

/** Copy one Atom to another (deep copy, though quick as no heap allocation) */
inline Atom& Atom::operator=(const Atom &other)
{
    AtomInfo::operator=(other);
    Vector::operator=(other);

    return *this;
}

/** Cast this Atom as an AtomInfo */
inline const AtomInfo& Atom::info() const
{
    return static_cast<const AtomInfo&>(*this);
}

/** Cast this Atom as a Vector */
inline const SireMaths::Vector& Atom::vector() const
{
    return static_cast<const SireMaths::Vector&>(*this);
}

/** Cast this Atom as an AtomInfo */
inline AtomInfo& Atom::info()
{
    return static_cast<AtomInfo&>(*this);
}

/** Cast this Atom as a Vector */
inline SireMaths::Vector& Atom::vector()
{
    return static_cast<SireMaths::Vector&>(*this);
}

/** Return a string representation of this Atom (required
    to remove ambiguity) */
inline QString Atom::toString() const
{
    return AtomInfo::toString();
}

}

Q_DECLARE_METATYPE(SireMol::Atom)

SIRE_END_HEADER

#endif
