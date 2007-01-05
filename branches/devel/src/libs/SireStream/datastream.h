#ifndef SIRESTREAM_DATASTREAM_H
#define SIRESTREAM_DATASTREAM_H

// This file contains functions and definitions that
// will (hopefully!) make writing binary streaming functions
// more easy and more robust.
//
// @author Christopher Woods

#include "SireStream/version_error.h"

class QDataStream;

namespace SireStream
{

QDataStream& writeHeader(QDataStream &ds, const RegisterMetaTypeBase &r_type,
                         VersionID version);

QDataStream& writeHeader(QDataStream &ds, MagicID magicid, VersionID version);

VersionID readHeader(QDataStream &ds, const RegisterMetaTypeBase &r_type);

VersionID readHeader(QDataStream &ds, MagicID magicid, const char *type_name);

}

#endif
