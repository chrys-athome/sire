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

#ifndef SIREMOL_ATOMNUM_H
#define SIREMOL_ATOMNUM_H

#include "id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This ID number is used to identify an atom by the user-supplied
    atom number (this is typically the number assigned to the
    atom from the PDB or other coordinate file)

    Be careful not to confuse this with AtomID, which is the
    index of the atom in the residue or CutGroup (e.g. the
    fifth atom in the residue would have AtomID '4' but has
    whatever AtomNum the user supplied.

    Each atom can be given an identifying number in a molecule, much like how every
    residue is given an identifying number (ResNum). However, the atom number doesn't
    have the same importance, as the atoms are identified by name. The atom number
    may sometimes be used for parametisation, or by file readers to identify bonds.

    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomNum : public IDBase
{

public:
    AtomNum() : IDBase()
    {}

    explicit AtomNum(quint32 id) : IDBase(id)
    {}

    AtomNum(const AtomNum &other) : IDBase(other)
    {}

    ~AtomNum()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::AtomNum, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
