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

#include "volumemove.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireStream;

/** The actual NPT Monte Carlo test */
bool VolumeMove::nptTest(double new_nrg, double old_nrg,
                         double new_volume, double old_volume)
{
}

/** Perform a volume Monte Carlo move */
void VolumeMove::move(SimSystem &system)
{
    //get the old energy and old space
    double old_nrg = energy(system);
    Space old_space = system.space();

    //save the old configuration
    CheckPoint checkpoint = system.checkPoint();

    try
    {
        //change the volume
        Space new_space = volchanger.change(old_space, _pvt_generator());

        //change any molecules that need to be mapped
        Molecules mapped_mols = mapfunc.map(system, new_space);

        //now change the system
        system.setSpace(new_space, mapped_mols);

        //get the new energy
        double new_nrg = energy(system);

        //use the NPT MC test
        if ( not nptTest(new_nrg, old_nrg,
                         new_space.volume(), old_space.volume()) )
        {
            //go back to the old system
            rollBack(system, checkpoint);
        }
    }
    catch(...)
    {
        rollBack(system, checkpoint);
        throw;
    }
}
