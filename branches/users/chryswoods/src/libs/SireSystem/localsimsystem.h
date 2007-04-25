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

#ifndef SIRESYSTEM_LOCALSIMSYSTEM_H
#define SIRESYSTEM_LOCALSIMSYSTEM_H

#include "simsystem.h"

#include "systemdata.h"
#include "systemmonitors.h"

#include "SireFF/forcefields.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{

class CheckPoint;

/** This is a simulation system that performs the entire simulation
    in the local thread.
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT LocalSimSystem : public SimSystem
{
public:
    LocalSimSystem(const CheckPoint &checkpoint);
    
    ~LocalSimSystem();
    
    void rollback(const CheckPoint &checkpoint);
    
private:
    /** The metadata for this system */
    SystemData local_sysdata;
    
    /** The monitors that monitor properties of this system */
    SystemMonitors local_monitors;
    
    /** The forcefields that describe the potential
        energy surface */
    ForceFields local_ffields;
};

}

SIRE_END_HEADER

#endif
