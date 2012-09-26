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
    writeHeader(ds, r_avg, 1);
    
    ds << static_cast<const ExpAverage&>(avg);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, FreeEnergyAverage &avg)
{
    VersionID v = readHeader(ds, r_avg);
    
    if (v == 1)
    {
        ds >> static_cast<ExpAverage&>(avg);
    }
    else
        throw version_error(v, "1", r_avg, CODELOC);
        
    return ds;
}

/** Constructor - this defaults to accumulating the average
    at room temperature (25 C) */
FreeEnergyAverage::FreeEnergyAverage()
                  : ConcreteProperty<FreeEnergyAverage,ExpAverage>(
                        -k_boltz * double(25*celsius) )
{}

/** Construct an accumulator to accumulate the free energy average
    at the specified temperature */
FreeEnergyAverage::FreeEnergyAverage(const Temperature &temperature)
                  : ConcreteProperty<FreeEnergyAverage,ExpAverage>(
                        -k_boltz * temperature )
{}

/** Copy constructor */
FreeEnergyAverage::FreeEnergyAverage(const FreeEnergyAverage &other)
                  : ConcreteProperty<FreeEnergyAverage,ExpAverage>(other)
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

/** Return the temperature at which the free energy average
    is being accumulated */
Temperature FreeEnergyAverage::temperature() const
{
    return Temperature( -(ExpAverage::scaleFactor()) / k_boltz );
}

const char* FreeEnergyAverage::typeName()
{
    return QMetaType::typeName( qMetaTypeId<FreeEnergyAverage>() );
}
