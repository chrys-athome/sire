/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2012  Christopher Woods
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

#ifndef SIRESYSTEM_ENERGYMONITOR_H
#define SIRESYSTEM_ENERGYMONITOR_H

#include "SireSystem/systemmonitor.h"
#include "SireMol/moleculegroup.h"
#include "SireMaths/accumulator.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class EnergyMonitor;
}

QDataStream& operator<<(QDataStream&, const SireSystem::EnergyMonitor&);
QDataStream& operator>>(QDataStream&, SireSystem::EnergyMonitor&);

namespace SireSystem
{

using SireMol::MoleculeGroup;
using SireMaths::Accumulator;

/** This monitor is used to monitor the energy of interaction between
    molecule views in two groups. The coulomb and LJ energy of each pair
    of molecule views in the two groups is calculated and averaged
    using the contained accumulator
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT EnergyMonitor
        : public SireBase::ConcreteProperty<EnergyMonitor,SystemMonitor>
{

friend QDataStream& ::operator<<(QDataStream&, const EnergyMonitor&);
friend QDataStream& ::operator>>(QDataStream&, EnergyMonitor&);

public:
    EnergyMonitor();
    EnergyMonitor(const MoleculeGroup &group0, 
                  const MoleculeGroup &group1);
                  
    EnergyMonitor(const MoleculeGroup &group0, 
                  const MoleculeGroup &group1,
                  const SireMaths::Accumulator &accum);

    EnergyMonitor(const EnergyMonitor &other);
    
    ~EnergyMonitor();
    
    EnergyMonitor& operator=(const EnergyMonitor &other);
    
    bool operator==(const EnergyMonitor &other) const;
    bool operator!=(const EnergyMonitor &other) const;
    
    static const char* typeName();
    
    void clearStatistics();
    
    void monitor(System &system);

private:
    /** The two molecule groups that contain the molecule views
        between which energies will be calculated */
    SireMol::MolGroupPtr grp0, grp1;
    
    /** The accumulator used to accumulate the energy values */
    SireMaths::AccumulatorPtr accum;
};

}

Q_DECLARE_METATYPE( SireSystem::EnergyMonitor )

SIRE_EXPOSE_CLASS( SireSystem::EnergyMonitor )

SIRE_END_HEADER

#endif
