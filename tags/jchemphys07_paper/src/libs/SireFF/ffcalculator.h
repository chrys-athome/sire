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

#ifndef SIREFF_FFCALCULATOR_H
#define SIREFF_FFCALCULATOR_H

#include "ffcalculatorbase.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

/** This is the base class of calculators that store and use
    a local copy of the forcefield

    @author Christopher Woods
*/
class SIREFF_EXPORT FFLocalCalculator : public FFCalculatorBase
{
public:
    FFLocalCalculator();
    FFLocalCalculator(const ForceField &ffield);

    ~FFLocalCalculator();

    bool setProperty(const QString &name, const Property &property);
    Property getProperty(const QString &name);
    bool containsProperty(const QString &name);

    QHash<QString,Property> properties();

    void mustNowRecalculateFromScratch();

    bool change(const PartialMolecule &molecule);
    bool change(const Molecules &molecules);

    bool add(const PartialMolecule &molecule,
             const ParameterMap &map = ParameterMap());
    bool add(const Molecules &molecules,
             const ParameterMap &map = ParameterMap());

    bool addTo(const FFBase::Group &group,
               const PartialMolecule &molecule,
               const ParameterMap &map = ParameterMap());
    bool addTo(const FFBase::Group &group,
               const Molecules &molecules,
               const ParameterMap &map = ParameterMap());

    bool remove(const PartialMolecule &molecule);
    bool remove(const Molecules &molecules);

    bool removeFrom(const FFBase::Group &group,
                    const PartialMolecule &molecule);
    bool removeFrom(const FFBase::Group &group,
                    const Molecules &molecules);

    bool contains(const PartialMolecule &molecule);
    bool contains(const PartialMolecule &molecule,
                  const FFBase::Group &group);

    bool refersTo(MoleculeID molid);
    bool refersTo(MoleculeID molid, const FFBase::Group &group);

    QSet<FFBase::Group> groupsReferringTo(MoleculeID molid);

    QSet<MoleculeID> moleculeIDs();
    QSet<MoleculeID> moleculeIDs(const FFBase::Group &group);

    PartialMolecule molecule(MoleculeID molid);
    PartialMolecule molecule(MoleculeID molid, const FFBase::Group &group);

    Molecules molecules();
    Molecules molecules(const QSet<MoleculeID> &molids);
    Molecules molecules(const FFBase::Group &group);

    Molecules contents(const FFBase::Group &group);
    Molecules contents();

    bool isDirty();
    bool isClean();

    ForceFieldID ID();
    Version version();

    void assertContains(const FFComponent &component);

protected:
    /** The forcefield being evaluated */
    ForceField ffield;
};

/** This is the default, and most simple, FFCalculator. This will
    work with any forcefield that can be used from the command line
    (pretty much all of them).

    @author Christopher Woods
*/
class SIREFF_EXPORT FFCalculator : public FFLocalCalculator
{
public:
    FFCalculator();
    FFCalculator(const ForceField &forcefield);

    ~FFCalculator();

    double getEnergies(Values &values);
    void calculateEnergy();

    bool setForceField(const ForceField &forcefield);
    ForceField forceField();

private:
    /** The total energy of the forcefield at the last calculation */
    double total_nrg;

    /** The values of the energy components at the last evaluation */
    Values nrg_components;
};

}

SIRE_END_HEADER

#endif
