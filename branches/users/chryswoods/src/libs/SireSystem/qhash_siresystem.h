#ifndef SIRESYSTEM_QHASH_SIRESYSTEM_H
#define SIRESYSTEM_QHASH_SIRESYSTEM_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class FFExpression;
}

uint qHash(const SireSystem::FFExpression &ffexpression);

SIRE_END_HEADER

#endif
