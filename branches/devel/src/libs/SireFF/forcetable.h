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

#ifndef SIREFF_FORCETABLE_H
#define SIREFF_FORCETABLE_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

/**
A ForceTable is a workspace within which all of the forces acting on a molecule may be stored. A ForceTable is used as storing the forces requires lots of memory, and continually creating a deleting such large amouts of memory would be inefficient. Also, using a ForceTable allows for forces to be accumalated directly, rather than requiring intermediate storage space for the individual components.

@author Christopher Woods

*/
class SIREFF_EXPORT ForceTable
{
public:
    ForceTable();
    ~ForceTable();

};

}

SIRE_END_HEADER

#endif
