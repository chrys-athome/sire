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

#include "id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This ID number is used to identify an Atom via its index.
    (e.g. the fifth atom in a residue)

    Be careful not to confuse this with AtomNum, which is a
    user-supplied ID number for an atom (AtomID is to
    AtomNum as ResID is to ResNum)

    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomID : public IDBase
{

public:
    AtomID() : IDBase()
    {}

    explicit AtomID(quint32 id) : IDBase(id)
    {}

    AtomID(const AtomID &other) : IDBase(other)
    {}

    ~AtomID()
    {}
};

}

/** This is a movable type */
Q_DECLARE_TYPEINFO(SireMol::AtomID, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
