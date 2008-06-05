#ifndef SIRESTREAM_GETMAGIC_H
#define SIRESTREAM_GETMAGIC_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireStream
{

/** Typedef a uint type that is used to check that the binary
    datastream holds an object of the right type */
typedef quint32 MagicID;

MagicID getMagic(const char *type_name);

}

SIRE_END_HEADER

#endif
