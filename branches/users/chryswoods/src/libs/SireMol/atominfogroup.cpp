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

AtomInfoGroup::AtomInfoGroup()
{}


AtomInfoGroup::~AtomInfoGroup()
{}
