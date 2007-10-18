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

#ifndef SIREMOL_RESNUM_H
#define SIREMOL_RESNUM_H

#include "id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This ID number is used to identify a residue by the user-supplied
    residue number (this is typically the number assigned to the
    residue from the PDB or other coordinate file)

    Be careful not to confuse this with ResID, which is the
    index of the residue in the molecule or group (e.g. the
    fifth residue in the molecule would have ResID '4' but has
    whatever ResNum the user supplied.

    @author Christopher Woods
*/
class SIREMOL_EXPORT ResNum : public IDBase
{

public:
    ResNum() : IDBase()
    {}

    explicit ResNum(quint32 id) : IDBase(id)
    {}

    ResNum(const ResNum &other) : IDBase(other)
    {}

    ~ResNum()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::ResNum, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
