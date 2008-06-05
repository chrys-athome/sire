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

#ifndef SPIER_MATERIALSTATE_H
#define SPIER_MATERIALSTATE_H

#include "materialptr.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/**
This maintains the material state of the viewer (the loaded material, texture and shader states, abstracted into the Material class). The current material can be set, or can also be pushed and popped from a stack.
 
@author Christopher Woods
*/
class SPIER_EXPORT MaterialState
{
public:
    MaterialState();
    ~MaterialState();

    void bind(const MaterialPtr &material);
    void clear();

private:
    /** The currently bound material */
    MaterialPtr current_material;
    
    /** Whether or not to force the next material to bind */
    bool forcebind;
};

}

SIRE_END_HEADER

#endif
