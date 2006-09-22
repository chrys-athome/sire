/**
  * @file
  *
  * C++ Implementation: AtomInfoGroup
  *
  * Description: Implementation of AtomInfoGroup
  *
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "atominfogroup.h"
#include "atomid.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<AtomInfoGroup> r_aigroup("SireMol::AtomInfoGroup");

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const AtomInfoGroup &aigroup)
{
    writeHeader(ds, r_aigroup, 0);
    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AtomInfoGroup &aigroup)
{
    VersionID v = readHeader(ds, r_aigroup);

    if (v != 0)
        throw version_error(v, "0", r_aigroup, CODELOC);

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

/** Ensure that the index 'i' is valid

    \throw SireError::invalid_index
*/
void AtomInfoGroup::checkAtom(AtomID id) const
{
    if (id >= atms.count())
        throw SireError::invalid_index( QObject::tr(
                "Invalid index in AtomInfoGroup: %1 when count() == %2")
                    .arg(id).arg(atms.count()), CODELOC );
}

/** Return the AtomInfo at index 'i'

    \throw SireError::invalid_index
*/
const AtomInfo& AtomInfoGroup::at(AtomID i) const
{
    checkAtom(i);
    return atms.constData()[i];
}

/** Return the AtomInfo at index 'i' */
const AtomInfo& AtomInfoGroup::operator[](AtomID i) const
{
    checkAtom(i);
    return atms.constData()[i];
}

/** Return whether or not this is a null AtomInfoGroup */
bool AtomInfoGroup::isNull() const
{
    return atms.isEmpty();
}

/** Return the number of AtomInfo objects in this group */
int AtomInfoGroup::count() const
{
    return atms.count();
}

/** Return the number of AtomInfo objects in this group */
int AtomInfoGroup::size() const
{
    return atms.count();
}

/** Return a raw pointer to the array of AtomInfo objects */
const AtomInfo* AtomInfoGroup::constData() const
{
    return atms.constData();
}
