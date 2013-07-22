/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "freeenergyaverage.h"

#include "SireUnits/units.h"

#include "SireStream/datastream.h"

using namespace SireMaths;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<FreeEnergyAverage> r_avg;

/** Serialise to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds,
                                         const FreeEnergyAverage &avg)
{
    writeHeader(ds, r_avg, 2);
    
    SharedDataStream sds(ds);
    
    sds << avg.hist << static_cast<const ExpAverage&>(avg);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, FreeEnergyAverage &avg)
{
    VersionID v = readHeader(ds, r_avg);
    
    if (v == 2)
    {
        SharedDataStream sds(ds);
        
        sds >> avg.hist >> static_cast<ExpAverage&>(avg);
    }
    else if (v == 1)
    {
        ds >> static_cast<ExpAverage&>(avg);
        avg.hist = Histogram();
    }
    else
        throw version_error(v, "1", r_avg, CODELOC);
        
    return ds;
}

/** Constructor - this defaults to accumulating the average
    at room temperature (25 C) and collects statistics about the
    free energy using a histogram of bin width 0.5 kcal mol-1 */
FreeEnergyAverage::FreeEnergyAverage()
                  : ConcreteProperty<FreeEnergyAverage,ExpAverage>(
                        -k_boltz * double(25*celsius) ), hist(0.5)
{}

/** Construct an accumulator to accumulate the free energy average
    at the specified temperature, and to collect statistics about
    the free energy using a histogram of bin width 0.5 kcal mol-1 */
FreeEnergyAverage::FreeEnergyAverage(const Temperature &temperature)
                  : ConcreteProperty<FreeEnergyAverage,ExpAverage>(
                        -k_boltz * temperature ), hist(0.5)
{}

/** Constructor - this defaults to accumulating the average
    at room temperature (25 C) and collects statistics about the
    free energy using a histogram of the passed bin width */
FreeEnergyAverage::FreeEnergyAverage(const MolarEnergy &binwidth)
                  : ConcreteProperty<FreeEnergyAverage,ExpAverage>(
                        -k_boltz * double(25*celsius) ), hist(binwidth.value())
{}

/** Construct an accumulator to accumulate the free energy average
    at the specified temperature, and to collect statistics about
    the free energy using a histogram of passed bin width */
FreeEnergyAverage::FreeEnergyAverage(const Temperature &temperature,
                                     const MolarEnergy &binwidth)
                  : ConcreteProperty<FreeEnergyAverage,ExpAverage>(
                        -k_boltz * temperature ), hist(binwidth.value())
{}

/** Copy constructor */
FreeEnergyAverage::FreeEnergyAverage(const FreeEnergyAverage &other)
                  : ConcreteProperty<FreeEnergyAverage,ExpAverage>(other),
                    hist(other.hist)
{}

/** Destructor */
FreeEnergyAverage::~FreeEnergyAverage()
{}

/** Copy assignment operator */
FreeEnergyAverage& FreeEnergyAverage::operator=(const FreeEnergyAverage &other)
{
    if (this != &other)
    {
        hist = other.hist;
        ExpAverage::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool FreeEnergyAverage::operator==(const FreeEnergyAverage &other) const
{
    return this == &other or
           (hist == other.hist and ExpAverage::operator==(other));
}

/** Comparison operator */
bool FreeEnergyAverage::operator!=(const FreeEnergyAverage &other) const
{
    return not operator==(other);
}

/** Return the temperature at which the free energy average
    is being accumulated */
Temperature FreeEnergyAverage::temperature() const
{
    return Temperature( -(ExpAverage::scaleFactor()) / k_boltz );
}

/** Return the histogram of energies */
const Histogram& FreeEnergyAverage::histogram() const
{
    return hist;
}

QString FreeEnergyAverage::toString() const
{
    return QObject::tr("FreeEnergyAverage( dG = %1 kcal mol-1, average = %2 kcal mol-1 "
                       "stderr = %3 kcal mol-1, "
                       "skew = %4 kcal mol-1, nSamples = %5 )")
                            .arg(this->average())
                            .arg(histogram().mean())
                            .arg(histogram().standardDeviation())
                            .arg(histogram().skew())
                            .arg(nSamples());
}

const char* FreeEnergyAverage::typeName()
{
    return QMetaType::typeName( qMetaTypeId<FreeEnergyAverage>() );
}

/** Clear all data from the accumulator */
void FreeEnergyAverage::clear()
{
    hist = Histogram();
    ExpAverage::clear();
}

/** Accumulate the passed energy difference onto the free energy average */
void FreeEnergyAverage::accumulate(double value)
{
    ExpAverage::accumulate(value);
    hist.accumulate(value);
}
