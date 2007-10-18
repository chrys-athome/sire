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

#ifndef SIREDB_ATOMTYPE_H
#define SIREDB_ATOMTYPE_H

#include <QString>

#include "SireMol/elementdb.h"

#include "atomparameter.hpp"

SIRE_BEGIN_HEADER

namespace SireDB
{
class AtomType;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireDB::AtomType&);
QDataStream& operator>>(QDataStream&, SireDB::AtomType&);

namespace SireDB
{

using SireMol::Element;

/**
An AtomType represents type of atom (textual code giving the type of atom, e.g. two letter code for Amber type forcefields). The atom types are used by many forcefields to assign the intramolecular bonded terms and also to identify the type of element of the atom.

@author Christopher Woods
*/
class SIREDB_EXPORT AtomType
{

friend QDataStream& ::operator<<(QDataStream&, const AtomType&);
friend QDataStream& ::operator>>(QDataStream&, AtomType&);

public:
    AtomType();
    AtomType(const QString &id);
    AtomType(const QString &id, const Element &element);
    AtomType(const Element &element);

    AtomType(const AtomParameter<AtomType> &atomtype);

    AtomType(const AtomType &other);

    ~AtomType();

    QString ID() const;
    const Element& element() const;

    bool isDummy() const;
    bool isWild() const;
    bool isWild(const Element &element) const;

    bool isWildType() const;
    bool isWildElement() const;

    QString toString() const;

    bool operator==(const AtomType &other) const;
    bool operator!=(const AtomType &other) const;

    static AtomType dummy();
    static AtomType wild();
    static AtomType wild(const Element &element);

protected:

    /** Global wild type */
    static AtomType wild_type;
    /** Global dummy type */
    static AtomType dummy_type;

    /** The atom type ID (e.g "CA" or "H3"). This is
        null for wild atom types */
    QString id;

    /** The element - this is a zero element for wild
        atom types */
    Element elmnt;
};

/** Compare two AtomTypes */
inline bool AtomType::operator==(const AtomType &other) const
{
    return elmnt == other.elmnt and id == other.id;
}

/** Compare two AtomTypes */
inline bool AtomType::operator!=(const AtomType &other) const
{
    return not operator==(other);
}

/** Return the identifying ID associated with this type (e.g. the 2-letter AMBER code) */
inline QString AtomType::ID() const
{
    return id;
}

/** Return the element represented by this AtomType */
inline const Element& AtomType::element() const
{
    return elmnt;
}

}

/** Provide a function to hash an AtomType */
inline unsigned int qHash(const SireDB::AtomType &atomtype)
{
    return qHash(atomtype.ID()) + atomtype.element().nProtons();
}

Q_DECLARE_METATYPE(SireDB::AtomType)

SIRE_END_HEADER

#endif
