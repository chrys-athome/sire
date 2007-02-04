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

#include "SireMol/qhash_siremol.h"
#include "SireCAS/qhash_sirecas.h"

#include "molprocalculator.h"
#include "molprosession.h"

#include <QTextStream>
#include <QFileInfo>
#include <QDateTime>
#include <QUuid>

#ifdef Q_OS_UNIX
//Possible Unix-only header file
#include <unistd.h>
#endif

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

#include "SireError/errors.h"

using namespace Squire;
using namespace SireFF;
using namespace SireMol;


//////////
////////// Implementation of MolproCalculator
//////////

/** Construct a calculator that evaluates the forcefield 'forcefield'
    using the molpro executable called 'molproexe'. Note that
    this calculator is only compatible with forcefields that are
    derived from MolproFF.

    \throw SireError::invalid_cast
    \throw SireError::process_error
*/
MolproCalculator::MolproCalculator(const ForceField &forcefield,
                                   const QFileInfo &molproexe,
                                   const QDir &tmpdir)
                 : FFCalculatorBase(),
                   molpro_exe(molproexe),
                   temp_dir(tmpdir)
{
    //its at this point that we ensure that the molpro executable exists and
    //is in the path (we also resolve it in the path)
    if ( not (molpro_exe.exists() and molpro_exe.isExecutable()) )
    {
        throw SireError::process_error( QObject::tr(
                  "Could not find the Molpro executable \"%1\" - please "
                  "ensure that this file exists and is executable.")
                      .arg(molpro_exe.absoluteFilePath()), CODELOC );
    }

    //set the forcefield
    MolproCalculator::setForceField(forcefield);
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
    if ( not molpro_session or molpro_session->incompatibleWith(*molproff) )
    {
        //kill the old session (do this first to prevent having two
        //molpro jobs running simultaneously - think of the memory!)
        molpro_session.reset();

        //start a new session
        molpro_session.reset( new MolproSession(*molproff) );
    }

    nrg_components = molproff->recalculateEnergy(*molpro_session);
    total_nrg = nrg_components.value(molproff->components().total());
}

/** Move the molecule 'molecule' and return whether the energy of
    the forcefield needs to be recalculated */
bool MolproCalculator::change(const Molecule &molecule)
{
    return molproff->change(molecule);
}

/** Move the residue 'residue' and return whether the energy of
    the forcefield now needs to be recalculated */
bool MolproCalculator::change(const Residue &residue)
{
    return molproff->change(residue);
}

/** Move the atom 'atom' and return whether the energy of 
    the forcefield now needs to be recalculated */
bool MolproCalculator::change(const NewAtom &atom)
{
    return molproff->change(atom);
}

bool MolproCalculator::add(const Molecule&, const ParameterMap&)
{
    return false;
}

bool MolproCalculator::add(const Residue&, const ParameterMap&)
{
    return false;
}

bool MolproCalculator::add(const NewAtom&, const ParameterMap&)
{
    return false;
}

/** Remove the molecule 'molecule', returning whether or not this
    changes this forcefield */
bool MolproCalculator::remove(const Molecule &molecule)
{
    return molproff->remove(molecule);
}

bool MolproCalculator::remove(const Residue&)
{
    return false;
}

bool MolproCalculator::replace(const Molecule&, const Molecule&, const ParameterMap&)
{
    return false;
}

/** Set the forcefield for this calculator - this calculator is
    only compatible with forcefields that are derived from MolproFF.
    Return whether the energy of the forcefield needs to be
    recalculated.

    \throw SireError::invalid_cast
*/
bool MolproCalculator::setForceField(const ForceField &forcefield)
{
    molproff = forcefield;

    if (molproff->isDirty())
        return true;
    else
    {
        nrg_components = molproff->energies();
        total_nrg = nrg_components.value(molproff->components().total());

        return false;
    }
}

/** Return a copy of the forcefield being evaluated */
ForceField MolproCalculator::forcefield() const
{
    return ForceField(molproff);
}
