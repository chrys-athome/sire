/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIREMOL_ATOMMATCHER_H
#define SIREMOL_ATOMMATCHER_H

#include "sireglobal.h"

namespace SireMol
{

/** Class that will eventually be used to match atoms from 
    on molecule to atoms in another. This is used, for
    example, when aligning two molecules against one another.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomMatcher
{
public:
    AtomMatcher();
    AtomMatcher(const AtomMatcher &other);
    
    ~AtomMatcher();
    
    AtomMatcher& operator=(const AtomMatcher &other);
};

}

SIRE_EXPOSE_CLASS( SireMol::AtomMatcher )

#endif
