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

#include "SireSystem/energymonitor.h"
#include "SireSystem/system.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireMol;
using namespace SireMaths;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<EnergyMonitor> r_nrgmonitor;

QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, 
                                          const EnergyMonitor &nrgmonitor)
{
    writeHeader(ds, r_nrgmonitor, 1);
    
    SharedDataStream sds(ds);
    
    sds << nrgmonitor.grp0 << nrgmonitor.grp1 << nrgmonitor.accum
        << static_cast<const SystemMonitor&>(nrgmonitor);
        
    return ds;
}

QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds,
                                          EnergyMonitor &nrgmonitor)
{
    VersionID v = readHeader(ds, r_nrgmonitor);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> nrgmonitor.grp0 >> nrgmonitor.grp1 >> nrgmonitor.accum
            >> static_cast<SystemMonitor&>(nrgmonitor);
    }
    else
        throw version_error(v, "1", r_nrgmonitor, CODELOC);
        
    return ds;
}

/** Null constructor */
EnergyMonitor::EnergyMonitor() : ConcreteProperty<EnergyMonitor,SystemMonitor>()
{}

/** Construct to monitor the energies between all pairs of molecule views in the
    two passed groups. This will accumulate the average and standard deviation
    of each of the energies */
EnergyMonitor::EnergyMonitor(const MoleculeGroup &group0, 
                             const MoleculeGroup &group1)
              : ConcreteProperty<EnergyMonitor,SystemMonitor>(),
                grp0(group0), grp1(group1), accum( AverageAndStddev() )
{}
            
/** Construct to monitor the energies between all pairs of molecule views in 
    the two passed groups, accumulating the energies using the passed
    accumulator */
EnergyMonitor::EnergyMonitor(const MoleculeGroup &group0, 
                             const MoleculeGroup &group1,
                             const SireMaths::Accumulator &accumulator)
              : ConcreteProperty<EnergyMonitor,SystemMonitor>(),
                grp0(group0), grp1(group1), accum(accumulator)
{}

/** Copy constructor */
EnergyMonitor::EnergyMonitor(const EnergyMonitor &other)
              : ConcreteProperty<EnergyMonitor,SystemMonitor>(other),
                grp0(other.grp0), grp1(other.grp1), accum(other.accum)
{}

/** Destructor */
EnergyMonitor::~EnergyMonitor()
{}

/** Copy assignment operator */
EnergyMonitor& EnergyMonitor::operator=(const EnergyMonitor &other)
{
    if (this != &other)
    {
        grp0 = other.grp0;
        grp1 = other.grp1;
        accum = other.accum;
        SystemMonitor::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool EnergyMonitor::operator==(const EnergyMonitor &other) const
{
    return this == &other or
           (grp0 == other.grp0 and grp1 == other.grp1 and
            accum == other.accum and SystemMonitor::operator==(other));
}

/** Comparison operator */
bool EnergyMonitor::operator!=(const EnergyMonitor &other) const
{
    return not EnergyMonitor::operator==(other);
}

/** Return the typename of the class */
const char* EnergyMonitor::typeName()
{
    return QMetaType::typeName( qMetaTypeId<EnergyMonitor>() );
}

/** Clear all statistics */
void EnergyMonitor::clearStatistics()
{}

/** Accumulate energies from the passed system */
void EnergyMonitor::monitor(System &system)
{}
