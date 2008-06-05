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

#ifndef SQUIRE_MOLPROCALCULATOR_H
#define SQUIRE_MOLPROCALCULATOR_H

#include <QFileInfo>
#include <QDir>

#include <boost/shared_ptr.hpp>

#include "SireFF/ffcalculator.h"

#include "molproff.h"

SIRE_BEGIN_HEADER

namespace Squire
{

class MolproSession;

using SireFF::ForceField;

/** This is the calculator used to calculate energies and forces
    via an external Molpro process.

    This is the main driver class behind the Molpro energy
    evaluation. The hierarchy of Molpro classes is as follows;

    MolproFF : The forcefield that uses Molpro to evaluate the QM/MM energy.
               Can evaluate the Molpro energy using a one-shot Molpro process

    MolproCalculator : Calculator that can be used to allow MolproFF to 
                       evaluate the energy using a single Molpro process

    MolproProcessor : Processor that can run MolproFF derived forcefields
                      by using MolproCalculator - a single MolproProcessor
                      starts up a single MolproCalculator in a background
                      thread, thus allowing a background thread to use
                      a single running Molpro process to calculate all
                      of the energies.

    MolproSession : An individual connection to a single running instance
                    of Molpro - this class provides the interface to the
                    running Molpro program that is used by MolproCalculator
                    and MolproFF to evaluate the QM/MM energies and forces.

    With this design, you will only need to override MolproFF if you wish
    to change the type of Molpro calculation.

    @author Christopher Woods
*/
class SQUIRE_EXPORT MolproCalculator : public SireFF::FFLocalCalculator
{
public:
    MolproCalculator(const ForceField &forcefield);

    ~MolproCalculator();

    double getEnergies(Values &values);
    void calculateEnergy();

    bool setForceField(const ForceField &forcefield);
    ForceField forceField();

private:
    void restartMolpro();

    /** The total energy of the forcefield */
    double total_nrg;

    /** The values of the energy components */
    Values nrg_components;

    /** Shared pointer to the Molpro session that is
        used to calculate the energies */
    boost::shared_ptr<MolproSession> molpro_session;
};

}

SIRE_END_HEADER

#endif
