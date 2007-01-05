
#include "atomtype.h"

#include <QDebug>

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;

AtomType AtomType::wild_type(QString::null, Element(0));
AtomType AtomType::dummy_type("DM", Element(0));

static const RegisterMetaType<AtomType> r_atomtype;

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const AtomType &typ)
{
    writeHeader(ds, r_atomtype, 1) << typ.id << typ.elmnt;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, AtomType &typ)
{
    VersionID v = readHeader(ds, r_atomtype);

    if (v == 1)
    {
        ds >> typ.id >> typ.elmnt;
    }
    else
        throw version_error(v, "1", r_atomtype, CODELOC);

    return ds;
}

/** Construct an empty AtomType */
AtomType::AtomType()
{}

/** Construct an AtomType from the provided information. */
AtomType::AtomType(const QString &typ, const Element &el) : id(typ.simplified()), elmnt(el)
{}

/** Construct an AtomType when only the description is known. The element
    is left as a dummy */
AtomType::AtomType(const QString &typ) : id(typ.simplified()), elmnt(0)
{}

/** Construct an AtomType that only contains information about the element */
AtomType::AtomType(const Element &element) : id(QString::null), elmnt(element)
{}

/** Construct from the AtomParameter */
AtomType::AtomType(const AtomParameter<AtomType> &atomtype)
         : id(atomtype.id), elmnt(atomtype.elmnt)
{}

/** Copy constructor */
AtomType::AtomType(const AtomType &other) : id(other.id), elmnt(other.elmnt)
{}

/** Destructor */
AtomType::~AtomType()
{}

/** Return a dummy AtomType parameter */
AtomType AtomType::dummy()
{
    return dummy_type;
}

/** Return an AtomType that represents a wildcard (useful for matching dihedral or other
    intramolecular parameters) */
AtomType AtomType::wild()
{
    return wild_type;
}

/** Return an AtomType that represents any atomtype of the specified element */
AtomType AtomType::wild(const Element &element)
{
    return AtomType( QString::null, element );
}

/** Return a string representation of this AtomType */
QString AtomType::toString() const
{
    return QString("{%1: %2}").arg(id).arg(elmnt.symbol());
}

/** Return whether or not this is a wild AtomType */
bool AtomType::isWild() const
{
    return this->operator==( wild() );
}

/** Return whether or not this is a wild AtomType of element 'element' */
bool AtomType::isWild(const Element &element) const
{
    return this->operator==( wild(element) );
}

/** Return whether this is a wild type (specifies only the element, e.g. carbon) */
bool AtomType::isWildType() const
{
    return id.isNull();
}

/** Return whether this is a wild element match (specifies only the type ID,
    not the element, which is wild) */
bool AtomType::isWildElement() const
{
    return elmnt.nProtons() == 0;
}

/** Return whether or not this is a dummy AtomType */
bool AtomType::isDummy() const
{
    return this->operator==( AtomType::dummy() );
}
