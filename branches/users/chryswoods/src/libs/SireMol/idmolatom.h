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

#ifndef SIREMOL_IDMOLATOM_H
#define SIREMOL_IDMOLATOM_H

#include "idtypes.h"
#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

class MoleculeInfo;

/** This small and simple class is used to provide a single
    argument for Molecule functions that expect an Atom
    identification (e.g. anything specifying or returning
    the property of a single atom). Using this class,
    the Molecule class can expose only a single function,
    thereby preventing an explosion of over-ridden functions.

    @author Christopher Woods
*/
class SIREMOL_EXPORT IDMolAtom
{
public:
    IDMolAtom(const CGAtomID &cgatomid);
    IDMolAtom(const CGNumAtomID &cgatomid);
    IDMolAtom(const ResNumAtomID &resatomid);
    IDMolAtom(const ResIDAtomID &resatomid);
    IDMolAtom(const AtomIndex &atomindex);
    IDMolAtom(const AtomID &atomid);

    IDMolAtom(const IDMolAtom &other);

    ~IDMolAtom();

    const CGAtomID& index(const MoleculeInfo &molinfo) const;

    bool contains(const MoleculeInfo &molinfo) const;

private:

    enum IndexType { CGIDATOMID    = 0,
                     CGNUMATOMID   = 1,
                     RESNUMATOMID  = 2,
                     RESIDATOMID   = 3,
                     ATOMINDEX     = 4,
                     ATOMID        = 5
                   };

    /** The type of index held */
    IndexType indextype;

    /** Pointer to the index
        (cannot use the actual object as classes
         with non-default constructors or destructors
         cannot be placed in a union!) */
    union
    {
        const CGAtomID *cgatomid;
        const CGNumAtomID *cgnumatomid;
        const ResNumAtomID *resnumatomid;
        const ResIDAtomID *residatomid;
        const AtomIndex *atomindex;
        const AtomID *atomid;
    } d;
};

}

SIRE_END_HEADER

#endif
