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

#include "SireCAS/qhash_sirecas.h"

#include "ffthreadworker.h"
#include "ffcalculator.h"

#include "SireMol/molecule.h"

#include "SireError/errors.h"

using namespace SireFF;
using namespace SireCluster;

/** Constructor */
FFThreadWorker::FFThreadWorker(FFCalculatorBase *calculator)
               : FFWorkerBase(), ThreadWorker(),
                 ffcalculator(calculator)
{
    if (not calculator)
        throw SireError::program_bug( ::QObject::tr(
                "Cannot create an FFThreadWorker with a null FFCalculator!"),
                    CODELOC );
}

/** Destructor */
FFThreadWorker::~FFThreadWorker()
{}

/** Return a copy of the forcefield being calculated. This
    will block until the processor is idle */
ForceField FFThreadWorker::forcefield() const
{
    //wait until any running calculation has finished
    const_cast<FFThreadWorker*>(this)->_pvt_waitUntilReady();

    return ffcalculator->forcefield();
}

/** Set the forcefield to be calculated, returning whether or not
    the energy needs to be recalculated

    \throw SireError::incompatible_error
*/
bool FFThreadWorker::_pvt_setForceField(const ForceField &forcefield)
{
    //the FFProcessor has already waited until we are idle...
    return ffcalculator->setForceField(forcefield);
}

/** Add the molecule 'molecule', returning whether or not the
    energy now needs to be recalculated */
bool FFThreadWorker::_pvt_add(const Molecule &molecule, const ParameterMap &map)
{
    //the FFProcessor has already waited until we are idle...
    return ffcalculator->add(molecule, map);
}

/** Add the residue 'residue', returning whether or not the
    energy now needs to be recalculated */
bool FFThreadWorker::_pvt_add(const Residue &residue, const ParameterMap &map)
{
    //the FFProcessor has already waited until we are idle...
    return ffcalculator->add(residue, map);
}

/** Change the molecule 'molecule', returning whether or not the
    energy now needs to be recalculated */
bool FFThreadWorker::_pvt_change(const Molecule &molecule)
{
    //the FFProcessor has already waited until we are idle...
    return ffcalculator->change(molecule);
}

/** Change the residue 'residue', returning whether or not the
    energy now needs to be recalculated */
bool FFThreadWorker::_pvt_change(const Residue &residue)
{
    //the FFProcessor has already waited until we are idle...
    return ffcalculator->change(residue);
}

/** Remove the molecule 'molecule', returning whether or not the
    energy now needs to be recalculated */
bool FFThreadWorker::_pvt_remove(const Molecule &molecule)
{
    //the FFProcessor has already waited until we are idle...
    return ffcalculator->remove(molecule);
}

/** Remove the residue 'residue', returning whether or not the
    energy now needs to be recalculated */
bool FFThreadWorker::_pvt_remove(const Residue &residue)
{
    //the FFProcessor has already waited until we are idle...
    return ffcalculator->remove(residue);
}

/** Replace 'oldmol' with 'newmol', returning whether or not the
    energy now needs to be recalculated */
bool FFThreadWorker::_pvt_replace(const Molecule &oldmol, 
                                  const Molecule &newmol, const ParameterMap &map)
{
    //the FFProcessor has already waited until we are idle...
    return ffcalculator->replace(oldmol, newmol, map);
}

/** Recalculate the energy in the background */
void FFThreadWorker::_pvt_recalculateEnergy()
{
    //wait until the last calculation has finished
    ThreadWorker::runBG();
}

/** Recalculate the energy in the foreground */
void FFThreadWorker::_pvt_recalculateEnergyFG()
{
    ThreadWorker::runFG();
}

/** Wait until the last calculation has finished */
void FFThreadWorker::_pvt_waitUntilReady()
{
    ThreadWorker::waitUntilReady();
}

/** Get the energies from the forcefield */
double FFThreadWorker::_pvt_getEnergies(Values &nrg_components)
{
    //the FFProcessor has already waited until we are idle...
    return ffcalculator->getEnergies(nrg_components);
}

/** Perform the actual energy recalculation */
void FFThreadWorker::calculate()
{
    ffcalculator->calculateEnergy();
}
