/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "idmolatom.h"
#include "moleculeinfo.h"

#include "cgatomid.h"
#include "cgnumatomid.h"
#include "resnumatomid.h"
#include "residatomid.h"
#include "atomindex.h"

using namespace SireMol;

/** Construct an index from a CGAtomID */
IDMolAtom::IDMolAtom(const CGAtomID &cgatomid)
          : indextype(IDMolAtom::CGIDATOMID)
{
    d.cgatomid = &cgatomid;
}

/** Construct an index from a CGNumAtomID */
IDMolAtom::IDMolAtom(const CGNumAtomID &cgatomid)
          : indextype(IDMolAtom::CGNUMATOMID)
{
    d.cgnumatomid = &cgatomid;
}

/** Construct an index from a ResNumAtomID */
IDMolAtom::IDMolAtom(const ResNumAtomID &resatomid)
          : indextype(IDMolAtom::RESNUMATOMID)
{
    d.resnumatomid = &resatomid;
}

/** Construct an index from a ResIDAtomID */
IDMolAtom::IDMolAtom(const ResIDAtomID &resatomid)
          : indextype(IDMolAtom::RESIDATOMID)
{
    d.residatomid = &resatomid;
}

/** Construct an index from an AtomIndex */
IDMolAtom::IDMolAtom(const AtomIndex &atomindex)
          : indextype(IDMolAtom::ATOMINDEX)
{
    d.atomindex = &atomindex;
}

/** Construct an index from an AtomID */
IDMolAtom::IDMolAtom(const AtomID &atomid)
          : indextype(IDMolAtom::ATOMID)
{
    d.atomid = &atomid;
}
    
/** Copy constructor */
IDMolAtom::IDMolAtom(const IDMolAtom &other)
          : indextype(other.indextype)
{}
    
/** Destructor */
IDMolAtom::~IDMolAtom()
{}
   
/** Return the equivalent CGAtomID index for the index held in this
    IDMolAtom in the passed MoleculeInfo.
    
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
const CGAtomID& IDMolAtom::index(const MoleculeInfo &molinfo) const
{
    switch( indextype )
    {
        case CGIDATOMID:
            return *(d.cgatomid);
        case CGNUMATOMID:
            return molinfo.at( *(d.cgnumatomid) );
        case RESNUMATOMID:
            return molinfo.at( *(d.resnumatomid) );
        case RESIDATOMID:
            return molinfo.at( *(d.residatomid) );
        case ATOMINDEX:
            return molinfo.at( *(d.atomindex) );
        case ATOMID:
            return molinfo.at( *(d.atomid) );
    }

    return *(d.cgatomid);
}

/** Return whether or not 'mol' contains the referenced atom */
bool IDMolAtom::contains(const MoleculeInfo &molinfo) const
{
    switch( indextype )
    {
        case CGIDATOMID:
            return molinfo.contains( *(d.cgatomid) );
        case CGNUMATOMID:
            return molinfo.contains( *(d.cgnumatomid) );
        case RESNUMATOMID:
            return molinfo.contains( *(d.resnumatomid) );
        case RESIDATOMID:
            return molinfo.contains( *(d.residatomid) );
        case ATOMINDEX:
            return molinfo.contains( *(d.atomindex) );
        case ATOMID:
            return molinfo.contains( *(d.atomid) );
    }
    
    return false;
}
