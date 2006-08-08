#ifndef SIRECAS_QHASH_SIRECAS_H
#define SIRECAS_QHASH_SIRECAS_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

/** The name lookup rules for argument dependent function lookups is different
    in gcc-4.1. This is a pain, as it means that all "uint qHash(const T &val)"
    functions *must* be declared before QHash is included. This file contains 
    the declarations of all of the SireCAS qHash functions. Ensure that you include
    this file before including any QHash, QSet or SireBase::Set includes... */
    
namespace SireCAS
{
class Symbol;
class Function;
class FunctionSignature;
class ExpressionBase;
class Expression;
}

uint qHash(const SireCAS::Symbol&);
uint qHash(const SireCAS::Function&);
uint qHash(const SireCAS::FunctionSignature&);
uint qHash(const SireCAS::ExpressionBase&);
uint qHash(const SireCAS::Expression&);

SIRE_END_HEADER

#endif
