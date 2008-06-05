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

#ifndef SIREMOL_RESID_H
#define SIREMOL_RESID_H

#include "id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This is the ID type for a residue (index into an array of residues)
    Be careful not to confuse this with ResNum, which is the identifying
    number given to the residue by the user, and used internally to
    identify the residue
*/
class SIREMOL_EXPORT ResID : public IDBase
{

public:
    ResID() : IDBase()
    {}

    explicit ResID(quint32 id) : IDBase(id)
    {}

    ResID(const ResID &other) : IDBase(other)
    {}

    ~ResID()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::ResID, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
