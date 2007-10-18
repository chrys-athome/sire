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

#include <cmath>

#include "monitorenergy.h"
#include "querysystem.h"

#include "SireStream/datastream.h"

#include <QDebug>

using namespace SireSystem;
using namespace SireStream;

static const RegisterMetaType<MonitorEnergy> r_monnrg;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const MonitorEnergy &monnrg)
{
    writeHeader(ds, r_monnrg, 1)
        << monnrg.cmpnt << monnrg.avgnrg << monnrg.avgnrg2
        << static_cast<const SystemMonitorBase&>(monnrg);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, MonitorEnergy &monnrg)
{
    VersionID v = readHeader(ds, r_monnrg);

    if (v == 1)
    {
        ds >> monnrg.cmpnt >> monnrg.avgnrg >> monnrg.avgnrg2
           >> static_cast<SystemMonitorBase&>(monnrg);
    }
    else
        throw version_error(v, "1", r_monnrg, CODELOC);

    return ds;
}

/** Null constructor */
MonitorEnergy::MonitorEnergy()
              : SystemMonitorBase(),
                avgnrg(0), avgnrg2(0)
{}

/** Construct a monitor that monitors the average energy of the
    component 'component' */
MonitorEnergy::MonitorEnergy(const Symbol &component)
              : SystemMonitorBase(),
                cmpnt(component),
                avgnrg(0), avgnrg2(0)
{}

/** Copy constructor */
MonitorEnergy::MonitorEnergy(const MonitorEnergy &other)
              : SystemMonitorBase(other),
                cmpnt(other.cmpnt),
                avgnrg(other.avgnrg),
                avgnrg2(other.avgnrg2)
{}

/** Destructor */
MonitorEnergy::~MonitorEnergy()
{}

/** Copy assignment operator */
MonitorEnergy& MonitorEnergy::operator=(const MonitorEnergy &other)
{
    if (this != &other)
    {
        SystemMonitorBase::operator=(other);

        cmpnt = other.cmpnt;
        avgnrg = other.avgnrg;
        avgnrg2 = other.avgnrg2;
    }

    return *this;
}

/** Comparison function */
bool MonitorEnergy::_pvt_isEqual(const PropertyBase &other) const
{
    BOOST_ASSERT( other.isA<MonitorEnergy>() );

    const MonitorEnergy &mon = other.asA<MonitorEnergy>();

    return this == &other or
            (cmpnt == mon.cmpnt and nUpdates() == mon.nUpdates() and
             avgnrg == mon.avgnrg and avgnrg2 == mon.avgnrg2);
}

/** Add the passed value onto the average and standard deviation */
void MonitorEnergy::addToAverage(double val)
{
//   averages calculated as a running value, based on the equation
//
//       newAverage = (oldAverage * nsteps-1/nsteps) + newValue/nsteps
//
//   in this way, we avoid large running totals which have to be divided
//   (and could be subject to error), and we are always dealing with numbers
//   that should be well handled as doubles (since nsteps-1/nsteps will
//   lie between 0.5 and 1, and 1/nsteps will lie between 0 and 0.5.
//   Also, 1/nsteps = 1 - (nsteps-1/nsteps)

    //nsteps has already been incremented
    double nsteps = this->nUpdates();

    BOOST_ASSERT(nsteps > 0);

    double bigratio = (nsteps-1) / nsteps;
    double smallratio = val * (1 - bigratio);

    //calculate the average, and the average of the square
    avgnrg = (avgnrg * bigratio) + smallratio;
    avgnrg2 = (avgnrg2 * bigratio) + (val * smallratio);
}

/** Monitor the passed system - calculate the energy of the component
    for this configuration of the system and add it onto the average

    \throw SireFF::missing_component
*/
void MonitorEnergy::monitor(QuerySystem &system)
{
    SystemMonitorBase::monitor(system);

    double nrg = system.energy(cmpnt);

    this->addToAverage(nrg);
}

/** Return the current value of this monitor (the average energy) */
double MonitorEnergy::value() const
{
    return avgnrg;
}

/** Completely clear the statistics of this monitor */
void MonitorEnergy::clear()
{
    SystemMonitorBase::clear();
    avgnrg = 0;
    avgnrg2 = 0;
}

/** Assert that this monitor is compatible with the passed system

    \throw SireFF::missing_component
*/
void MonitorEnergy::assertCompatibleWith(const QuerySystem &system) const
{
    system.assertContains(cmpnt);
}

/** Return the average energy */
double MonitorEnergy::average() const
{
    return avgnrg;
}

/** Return the standard deviation of the energy */
double MonitorEnergy::stdDev() const
{
    //standard deviation is sqrt( [mean of the squares] - [square of the mean] )
    return std::sqrt( avgnrg2 - (avgnrg*avgnrg) );
}

/** Return the energy component that this is monitoring */
const Symbol& MonitorEnergy::energyComponent() const
{
    return cmpnt;
}
