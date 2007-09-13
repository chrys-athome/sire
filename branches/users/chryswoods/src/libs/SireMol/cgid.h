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

#ifndef SIREMOL_CGID_H
#define SIREMOL_CGID_H

#include "SireID/id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

class CGIdx;
class MoleculeInfo;

/** This is the base class of all identifiers that are used 
    to identify a CutGroup

    @author Christopher Woods
*/
class SIREMOL_EXPORT CGID : public SireID::ID
{

public:
    CGID() : SireID::ID()
    {}

    CGID(const CGID &other) : SireID::ID(other)
    {}

    ~CGID()
    {}

    virtual CGID* clone() const=0;

    /** Map this ID back to the CGIdx of the CutGroup
        within the molecule described by the info in 'molinfo' */
    virtual CGIdx map(const MoleculeInfo &molinfo) const=0;
};

}

SIRE_END_HEADER

#endif
