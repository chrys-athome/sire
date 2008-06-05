/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

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
class AtomInfo;

class Bond;
class Angle;
class Dihedral;
class Improper;

class ResidueID;
class MolCutGroupID;

class IDBase;

class CGAtomID;
class ResNumAtomID;
class ResIDAtomID;
class MolCGAtomID;
class MolResAtomID;
class MolResIndexID;
class GroupIndexID;
class MolGroupIndexID;

class NewAtom;
class Residue;

class EditMol;
class Molecule;

class AtomSelection;

}

uint qHash(const SireMol::AtomSelection&);
uint qHash(const SireMol::AtomIndex&);
uint qHash(const SireMol::AtomInfo&);
uint qHash(const SireMol::Bond&);
uint qHash(const SireMol::Angle&);
uint qHash(const SireMol::Dihedral&);
uint qHash(const SireMol::Improper&);
uint qHash(const SireMol::ResidueID&);
uint qHash(const SireMol::MolCutGroupID&);
uint qHash(const SireMol::IDBase&);
uint qHash(const SireMol::CGAtomID&);
uint qHash(const SireMol::ResNumAtomID&);
uint qHash(const SireMol::ResIDAtomID&);
uint qHash(const SireMol::MolCGAtomID&);
uint qHash(const SireMol::MolResAtomID&);
uint qHash(const SireMol::MolResIndexID&);
uint qHash(const SireMol::GroupIndexID&);
uint qHash(const SireMol::MolGroupIndexID&);
uint qHash(const SireMol::EditMol&);
uint qHash(const SireMol::Molecule&);
uint qHash(const SireMol::Residue&);
uint qHash(const SireMol::NewAtom&);

SIRE_END_HEADER

#endif
