#ifndef PYPLUSPLUS_SQUIRE_HEADERS_H
#define PYPLUSPLUS_SQUIRE_HEADERS_H

#ifdef SKIP_BROKEN_GCCXML_PARTS
#define _BUILD_MOLPRO_
#endif

#include "SireMol/qhash_siremol.h"
#include "SireFF/qhash_sireff.h"

#ifdef _BUILD_MOLPRO_
#include "Squire/molprocalculator.h"
#include "Squire/molproff.h"
#include "Squire/molproprocessor.h"
#include "Squire/molprosession.h"
#endif // end of '#ifdef _BUILD_MOLPRO_'

#endif
