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
using SireCAS::Values;

/** This is the calculator used to calculate energies and forces
    via an external Molpro process.

    This is the main driver class behind the Molpro energy
    evaluation. The hierarchy of Molpro classes is as follows;

    MolproFF : The forcefield that uses Molpro to evaluate the QM/MM energy

    MolproProcessor : Processor that can run MolproFF derived forcefields.
                      A MolproFF derived forcefield must be placed on a
                      MolproProcessor so that it can be evaluated.

    MolproCalculator : Calculator that is used by MolproProcessor to
                       actually evaluate the MolproFF derived forcefield.

    MolproSession : An individual connection to a single running instance
                    of Molpro - this class provides the interface to the
                    running Molpro program that is used by MolproCalculator
                    and MolproFF to evaluate the QM/MM energies and forces.

    With this design, you will only need to override MolproFF if you wish
    to change the type of Molpro calculation.

    @author Christopher Woods
*/
class SQUIRE_EXPORT MolproCalculator : public SireFF::FFCalculatorBase
{
public:
    MolproCalculator(const ForceField &forcefield,
                     const QFileInfo &molpro_exe = QFileInfo("molpro"),
                     const QDir &temp_dir = QDir::temp());

    ~MolproCalculator();

    double getEnergies(Values &values);

    const Molecule& molecule(MoleculeID molid) const;

    void calculateEnergy();

    bool change(const Molecule &molecule);
    bool change(const Residue &residue);
    bool change(const NewAtom &atom);

    bool add(const Molecule &molecule, const ParameterMap &map = ParameterMap());
    bool add(const Residue &residue, const ParameterMap &map = ParameterMap());
    bool add(const NewAtom &atom, const ParameterMap &map = ParameterMap());

    bool remove(const Molecule &molecule);
    bool remove(const Residue &residue);
    bool remove(const NewAtom &atom);
    
    bool replace(const Molecule &oldmol, const Molecule &newmol,
                 const ParameterMap &map = ParameterMap());

    bool setForceField(const ForceField &forcefield);

    ForceField forcefield() const;

private:

    void restartMolpro();

    /** The Molpro forcefield being evaluated */
    SireBase::SharedPolyPointer<MolproFF> molproff;

    /** The path to the molpro executable */
    QFileInfo molpro_exe;

    /** The directory that will be used to run the calculation (tmpdir) */
    QDir temp_dir;

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
