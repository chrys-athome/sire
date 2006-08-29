/**
  * @file
  *
  * C++ Interface: AtomInfo
  *
  * Description: 
  * Interface to the AtomInfo class
  * 
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SIREMOL_ATOMINFO_H
#define SIREMOL_ATOMINFO_H

#include "SireMol/atomnum.h"
#include "SireMol/atomindex.h"
#include "SireMol/elementdb.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomInfo;
}

QDataStream& operator<<(QDataStream&, const SireMol::AtomInfo&);
QDataStream& operator>>(QDataStream&, SireMol::AtomInfo&);

uint qHash(const SireMol::AtomInfo&);

namespace SireMol
{

/**
An AtomInfo contains all of the information about an Atom, except for its coordinates. This allows the storage of the coordinates of the Atom to be separated from the storage of the rest of its information. This is useful, as a typical simulation will involve only the manipulation of the coordinates of the atom (as the molecules are moved). Separating the coordinates can thus allow more efficient update schemes which will not involve the copying of the invariant atom info data. Additionally, storing the coordinates of the atom together increases the possibility that a molecules coordinates are together on a single page of memory, and can thus be accessed efficiently.

@author Christopher Woods
*/
class SIREMOL_EXPORT AtomInfo : public AtomIndex, public Element
{

friend QDataStream& ::operator<<(QDataStream&, const AtomInfo&);
friend QDataStream& ::operator>>(QDataStream&, AtomInfo&);

public:
    AtomInfo();
    AtomInfo(const AtomInfo &other);
    AtomInfo(const AtomIndex &atomindex, const Element &element);
    AtomInfo(const AtomInfo &other, ResNum resnum);
    AtomInfo(AtomNum atmnum, const AtomIndex &atomindex, const Element &element);
    AtomInfo(AtomNum atmnum, const AtomInfo &other);
    AtomInfo(AtomNum atmnum, const AtomInfo &other, ResNum resnum);
    
    AtomInfo(const QString &name);
    AtomInfo(const QString &name, const QString &element);
    
    AtomInfo(const QString &name, ResNum resnum);
    AtomInfo(const QString &name, ResNum resnum, const QString &element);
    
    AtomInfo(const AtomIndex &atm);
    
    AtomInfo(const QString &name, const AtomInfo &other);
    
    AtomInfo(AtomNum num, const QString &name);
    AtomInfo(AtomNum num, const QString &name, const QString &element);

    ~AtomInfo();
    
    AtomInfo& operator=(const AtomInfo &other);

    bool operator==(const AtomIndex &other) const;
    bool operator!=(const AtomIndex &other) const;

    bool operator==(const AtomInfo &other) const;
    bool operator!=(const AtomInfo &other) const;

    const AtomIndex& index() const;
    AtomIndex& index();
           
    const Element& element() const;
    Element& element();
    
    const AtomInfo& info() const;
    AtomInfo& info();
    
    AtomNum number() const;
    AtomNum atomNum() const;

    QString name() const;

    QString toString() const;

private:
    AtomNum atmnum;
};

/** Return whether two AtomInfo objects are equal */
inline bool AtomInfo::operator==(const AtomInfo &other) const
{
    return ( Element::operator==(other) and
             AtomIndex::operator==(other) and
             atmnum == other.atmnum );
}

/** Test for inequality */
inline bool AtomInfo::operator!=(const AtomInfo &other) const
{
    return ( AtomIndex::operator!=(other) or
             Element::operator!=(other) or
             atmnum != other.atmnum );
}   

/** Return whether two atoms are equal - this is a convenience function that says that
    two atoms are equal if they have the same AtomIndex. You could explicitly use
    this function via atom.index() == other */
inline bool AtomInfo::operator==(const AtomIndex &other) const
{
    return AtomIndex::operator==(other);
}

/** Test for inequality - equivalent to atom.index() != other */
inline bool AtomInfo::operator!=(const AtomIndex &other) const
{
    return AtomIndex::operator!=(other);
}

/** Copy one AtomInfo to another (deep copy, though quick as no heap allocation) */
inline AtomInfo& AtomInfo::operator=(const AtomInfo &other)
{
    Element::operator=(other);
    AtomIndex::operator=(other);

    atmnum = other.atmnum;

    return *this;
}

/** Cast this AtomInfo as an AtomIndex */
inline const AtomIndex& AtomInfo::index() const
{
    return static_cast<const AtomIndex&>(*this);
}

/** Cast this AtomInfo as an Element */
inline const Element& AtomInfo::element() const
{
    return static_cast<const Element&>(*this);
}

/** Cast this AtomInfo as an AtomIndex */
inline AtomIndex& AtomInfo::index()
{
    return static_cast<AtomIndex&>(*this);
}

/** Cast this AtomInfo as an Element */
inline Element& AtomInfo::element()
{
    return static_cast<Element&>(*this);
}

/** Return the name of the atom (required to resolve ambiguity) */
inline QString AtomInfo::name() const
{
    return AtomIndex::name();
}

/** Return the number of this atom. This number may not necesserily have been set (in which
    case it will be 0), and it may not be unique within a molecule. The atom number is not
    really used for anything within this program, and its only purpose is so that the 
    atom number that is read in from the initial coordinate file can be retained throughout
    the lifetime of this atom, and can then be written out when this atom is written back
    out to an output coordinate file. As much as possible you should avoid writing code
    that relies on the atom number to identify the atom. There are a few cases where this is
    not possible (e.g. the PSF file reader, SireIO::PSF), and in those cases you should 
    mention that what you are doing is not massively safe...! */
inline AtomNum AtomInfo::number() const
{
    return atmnum;
}

/** Synonym for Atom::number() */
inline AtomNum AtomInfo::atomNum() const
{
    return atmnum;
}

}

/** Hash an AtomInfo object */
inline uint qHash(const SireMol::AtomInfo &atom)
{
    return qHash(atom.index());
}

Q_DECLARE_METATYPE(SireMol::AtomInfo)

SIRE_END_HEADER

#endif
