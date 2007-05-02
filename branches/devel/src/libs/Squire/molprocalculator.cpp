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

#include "molprocalculator.h"
#include "molprosession.h"

#include "SireError/errors.h"

using namespace Squire;
using namespace SireFF;

/** Construct a calculator that evaluates the forcefield 'forcefield'
    using the molpro executable called 'molproexe'. Note that
    this calculator is only compatible with forcefields that are
    derived from MolproFF.

    \throw SireError::invalid_cast
    \throw SireError::process_error
*/
MolproCalculator::MolproCalculator(const ForceField &ffield)
                 : FFLocalCalculator(ffield)
{
    //set the forcefield
    MolproCalculator::setForceField(ffield);
}

/** Destructor */
MolproCalculator::~MolproCalculator()
{}

/** Return the value of the total energy of the forcefield, as well as
    the energy components */
double MolproCalculator::getEnergies(Values &values)
{
    values = nrg_components;
    return total_nrg;
}

/** Tell the molpro forcefield to recalculate its energy */
void MolproCalculator::calculateEnergy()
{
    BOOST_ASSERT( ffield.isA<MolproFF>() );

    MolproFF &molproff = ffield.asA<MolproFF>();

    if ( not molpro_session or molpro_session->incompatibleWith(molproff) )
    {
        //kill the old session (do this first to prevent having two
        //molpro jobs running simultaneously - think of the memory!)
        molpro_session.reset();

        //start a new session
        molpro_session.reset( new MolproSession(molproff) );
    }

    nrg_components = molproff.recalculateEnergy(*molpro_session);
    total_nrg = nrg_components.value(molproff.components().total());
}

/** Set the forcefield for this calculator - this calculator is
    only compatible with forcefields that are derived from MolproFF.
    Return whether the energy of the forcefield needs to be
    recalculated.

    \throw SireError::invalid_cast
*/
bool MolproCalculator::setForceField(const ForceField &forcefield)
{
    if (not forcefield.isA<MolproFF>())
        throw SireError::invalid_cast( QObject::tr(
            "A MolproCalculator can only be used to evaluate forcefields "
            "that are derived from Squire::MolproFF. You cannot place "
            "a %1 on it!")
                .arg(forcefield.what()), CODELOC );

    ffield = forcefield;
    
    if (ffield.isDirty())
        return true;
    else
    {
        nrg_components = ffield.energies();
        total_nrg = nrg_components.value(ffield.components().total());

        return false;
    }
}

/** Return a copy of the forcefield being evaluated */
ForceField MolproCalculator::forceField()
{
    return ffield;
}
