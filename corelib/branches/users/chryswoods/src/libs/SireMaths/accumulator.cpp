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

#include <QMutex>

#include "accumulator.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMaths;
using namespace SireBase;
using namespace SireStream;

/////////
///////// Implementation of Accumulator
/////////

static const RegisterMetaType<Accumulator> r_accum( MAGIC_ONLY,
                                                    Accumulator::typeName() );
                                                    
/** Serialise to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const Accumulator &accum)
{
    writeHeader(ds, r_accum, 1);
    
    ds << accum.nvalues << static_cast<const Property&>(accum);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, Accumulator &accum)
{
    VersionID v = readHeader(ds, r_accum);
    
    if (v == 1)
    {
        ds >> accum.nvalues >> static_cast<Property&>(accum);
    }
    else
        throw version_error(v, "1", r_accum, CODELOC);
        
    return ds;
}

/** Constructor */
Accumulator::Accumulator() : Property(), nvalues(0)
{}

/** Copy constructor */
Accumulator::Accumulator(const Accumulator &other)
            : Property(other), nvalues(other.nvalues)
{}

/** Destructor */
Accumulator::~Accumulator()
{}

/** Return the number of values that have been sampled */
int Accumulator::nSamples() const
{
    return int(nvalues);
}

/** Accumulate the value 'value' onto the sample */
void Accumulator::accumulate(double)
{
    ++nvalues;
}

/** Completely clear the statistics in this accumulator */
void Accumulator::clear()
{
    nvalues = 0;
}

/** Internal copy assignment operator */
Accumulator& Accumulator::operator=(const Accumulator &other)
{
    nvalues = other.nvalues;
    Property::operator=(other);
}

/** Internal comparison operator */
bool Accumulator::operator==(const Accumulator &other) const
{
    return nvalues == other.nvalues;
}

/** Internal comparison operator */
bool Accumulator::operator!=(const Accumulator &other) const
{
    return nvalues != other.nvalues;
}

/////////
///////// Implementation of Average
/////////

static const RegisterMetaType<Average> r_avg;

/** Serialise to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const Average &avg)
{
    writeHeader(ds, r_avg, 1);
    
    ds << avg.avgval << static_cast<const Accumulator&>(avg);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, Average &avg)
{
    VersionID v = readHeader(ds, r_avg);
    
    if (v == 1)
    {
        ds >> avg.avgval >> static_cast<const Accumulator&>(avg);
    }
    else
        throw version_error(v, "1", r_avg, CODELOC);
        
    return ds;
}

/** Construct an empty average */
Average::Average() : ConcreteProperty<Average,Accumulator>(), avgval(0)
{}

/** Copy constructor */
Average::Average(const Average &other)
        : ConcreteProperty<Average,Accumulator>(other), avgval(other.avgval)
{}

/** Destructor */
Average::~Average()
{}

/** Copy assignment operator */
Average& Average::operator=(const Average &other)
{
    if (this != &other)
    {
        avgval = other.avgval;
        Accumulator::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool Average::operator==(const Average &other) const
{
    return avgval == other.avgval and
           Accumulator::operator==(other);
}

/** Comparison operator */
bool Average::operator!=(const Average &other) const
{
    return not this->operator==(other);
}

/** Accumulate the passed value onto the average */
void Average::accumulate(double value)
{
    double nsteps = this->nSamples() + 1;
    
    //calculate the average as
    // average = ((n-1)/n) * average + (1/n) * value
    
    double big_ratio = (nsteps - 1) / nsteps;
    double small_ratio = 1.0 / nsteps;
    
    avgval = (big_ratio * avgval) + (small_ratio * value);
    Accumulator::accumulate(value);
}

/** Return the average value */
double Average::average() const
{
    return avgval;
}

/** Allow automatic casting to a double to retrieve the average value */
Average::operator double() const
{
    return avgval;
}
