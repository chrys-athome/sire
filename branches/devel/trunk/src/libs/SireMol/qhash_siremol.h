#ifndef SIREMOL_QHASH_SIREMOL_H
#define SIREMOL_QHASH_SIREMOL_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

/** The name lookup rules for argument dependent function lookups is different
    in gcc-4.1. This is a pain, as it means that all "uint qHash(const T &val)"
    functions *must* be declared before QHash is included. This file contains 
    the declarations of all of the SireMol qHash functions. Ensure that you include
    this file before including any QHash, QSet or SireBase::Set includes... */
    
namespace SireMol
{
class AtomIndex;

class Bond;
class Angle;
class Dihedral;
class Improper;

class EditMol;
class ResidueID;
class MolCutGroupID;

class IDBase;

class CGAtomID;
class ResAtomID;
class ResIndexID;
class MolCGAtomID;
class MolResAtomID;
class MolResIndexID;
class GroupIndexID;
class MolGroupIndexID;

}

uint qHash(const SireMol::AtomIndex&);
uint qHash(const SireMol::Bond&);
uint qHash(const SireMol::Angle&);
uint qHash(const SireMol::Dihedral&);
uint qHash(const SireMol::Improper&);
uint qHash(const SireMol::EditMol&);
uint qHash(const SireMol::ResidueID&);
uint qHash(const SireMol::MolCutGroupID&);
uint qHash(const SireMol::IDBase&);
uint qHash(const SireMol::CGAtomID&);
uint qHash(const SireMol::ResAtomID&);
uint qHash(const SireMol::ResIndexID&);
uint qHash(const SireMol::MolCGAtomID&);
uint qHash(const SireMol::MolResAtomID&);
uint qHash(const SireMol::MolResIndexID&);
uint qHash(const SireMol::GroupIndexID&);
uint qHash(const SireMol::MolGroupIndexID&);


SIRE_END_HEADER

#endif
