#ifndef SIREMM_QHASH_SIREMM_H
#define SIREMM_QHASH_SIREMM_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

/** The name lookup rules for argument dependent function lookups is different
    in gcc-4.1. This is a pain, as it means that all "uint qHash(const T &val)"
    functions *must* be declared before QHash is included. This file contains 
    the declarations of all of the SireMM qHash functions. Ensure that you include
    this file before including any QHash, QSet or SireBase::Set includes... */
    
namespace SireMM
{
class BondResID;
class BondAtomID;
class AngleResID;
class AngleAtomID;
class DihedralResID;
class DihedralAtomID;
}

uint qHash(const SireMM::BondResID&);
uint qHash(const SireMM::BondAtomID&);
uint qHash(const SireMM::AngleResID&);
uint qHash(const SireMM::AngleAtomID&);
uint qHash(const SireMM::DihedralResID&);
uint qHash(const SireMM::DihedralAtomID&);


SIRE_END_HEADER

#endif
