#ifndef SIREDB_ATOMTYPE_H
#define SIREDB_ATOMTYPE_H

#include <QString>

#include "SireMol/elementdb.h"

#include "sireglobal.h"

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
