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

#ifndef SIREMOL_ATOMID_H
#define SIREMOL_ATOMID_H

#include "SireID/id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

class MoleculeInfo;
class ResID;
class SegID;
class CGID;

class CGAtomIdx;

/** This is the base class of all identifiers that are used 
    to identify an atom

    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomID : public SireID::ID
{
public:
    AtomID() : SireID::ID()
    {}

    AtomID(const AtomID &other) : SireID::ID(other)
    {}

    ~AtomID()
    {}

    virtual AtomID* clone() const=0;

    /** Map this ID back to the CGAtomIdx of the atom in the molecule, 
        using the passed MoleculeInfo to do the mapping */
    virtual CGAtomIdx map(const MoleculeInfo &molinfo) const=0;
};

/** This is the base class of all identifiers that are
    used to identify an Atom *only* - i.e. IDs that refer to
    the atom in a group, but don't contain information about the
    group, so AtomIdx, AtomNum and AtomName, but not 
    CGAtomIdx or any of the other AtomIDCombo IDs 
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomOnlyID : public AtomID
{
public:
    AtomOnlyID() : AtomID()
    {}
    
    AtomOnlyID(const AtomOnlyID &other) : AtomID(other)
    {}
    
    ~AtomOnlyID()
    {}
    
    using AtomID::operator=;
    
    virtual AtomOnlyID* clone() const=0;
    
    /** Map this ID to the CGAtomIdx of the atom in residue identified by
        'resid' in the molecule described by 'molinfo' */
    virtual CGAtomIdx map(const MoleculeInfo &molinfo, const ResID &resid) const=0;

    /** Map this ID to the CGAtomIdx of the atom in CutGroup identified by
        'cgid' in the molecule described by 'molinfo' */
    virtual CGAtomIdx map(const MoleculeInfo &molinfo, const CGID &cgid) const=0;

    /** Map this ID to the CGAtomIdx of the atom in segment identified by
        'segid' in the molecule described by 'molinfo' */
    virtual CGAtomIdx map(const MoleculeInfo &molinfo, const SegID &segid) const=0;
    
};

}

SIRE_END_HEADER

#endif
