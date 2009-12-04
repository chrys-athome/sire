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

using namespace SireMaths;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace Siren;

static const RegisterObject<FreeEnergyAverage> r_avg;

/** Constructor - this defaults to accumulating the average
    at room temperature (25 C) */
FreeEnergyAverage::FreeEnergyAverage()
                  : Implements<FreeEnergyAverage,ExpAverage>(
                        -k_boltz * double(25*celsius) )
{}

/** Construct an accumulator to accumulate the free energy average
    at the specified temperature */
FreeEnergyAverage::FreeEnergyAverage(const Temperature &temperature)
                  : Implements<FreeEnergyAverage,ExpAverage>(
                        -k_boltz * temperature.to(kelvin) )
{}

/** Copy constructor */
FreeEnergyAverage::FreeEnergyAverage(const FreeEnergyAverage &other)
                  : Implements<FreeEnergyAverage,ExpAverage>(other)
{}

/** Destructor */
FreeEnergyAverage::~FreeEnergyAverage()
{}

/** Copy assignment operator */
FreeEnergyAverage& FreeEnergyAverage::operator=(const FreeEnergyAverage &other)
{
    ExpAverage::operator=(other);
    return *this;
}

/** Comparison operator */
bool FreeEnergyAverage::operator==(const FreeEnergyAverage &other) const
{
    return ExpAverage::operator==(other);
}

/** Comparison operator */
bool FreeEnergyAverage::operator!=(const FreeEnergyAverage &other) const
{
    return ExpAverage::operator!=(other);
}

QString FreeEnergyAverage::toString() const
{
    return QObject::tr(
            "FreeEnergyAverage( value() = %1, nSamples() = %2, temperature() = %3 °C )")
                .arg(this->value()).arg(this->nSamples())
                .arg(this->temperature().to(celsius));
}

void FreeEnergyAverage::stream(Stream &s)
{
    s.assertVersion<FreeEnergyAverage>(1);
    
    Schema schema = s.item<FreeEnergyAverage>();
    
    ExpAverage::stream( schema.base() );
}

uint FreeEnergyAverage::hashCode() const
{
    return qHash(FreeEnergyAverage::typeName()) + ExpAverage::hashCode();
}

/** Return the temperature at which the free energy average
    is being accumulated */
Temperature FreeEnergyAverage::temperature() const
{
    return Temperature( -(ExpAverage::scaleFactor()) / k_boltz );
}
