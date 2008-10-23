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

#include <cmath>

#include "accumulator.h"
#include "SireMaths/maths.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

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

/** Accumulate many values */
void Accumulator::accumulate(const QVector<double> &values)
{
    foreach (double value, values)
    {
        this->accumulate(value);
    }
}

/** Accumulate many values */
void Accumulator::accumulate(const QList<double> &values)
{
    foreach (double value, values)
    {
        this->accumulate(value);
    }
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
    return *this;
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
///////// Implementation of NullAccumulator
/////////

static const RegisterMetaType<NullAccumulator> r_null;

/** Serialise to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const NullAccumulator &null)
{
    writeHeader(ds, r_null, 1);
    
    ds << static_cast<const Accumulator&>(null);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, NullAccumulator &null)
{
    VersionID v = readHeader(ds, r_null);
    
    if (v == 1)
    {
        ds >> static_cast<Accumulator&>(null);
    }
    else
        throw version_error(v, "1", r_null, CODELOC);
        
    return ds;
}

/** Construct an empty average */
NullAccumulator::NullAccumulator() 
                : ConcreteProperty<NullAccumulator,Accumulator>()
{}

/** Copy constructor */
NullAccumulator::NullAccumulator(const NullAccumulator &other)
        : ConcreteProperty<NullAccumulator,Accumulator>(other)
{}

/** Destructor */
NullAccumulator::~NullAccumulator()
{}

/** Copy assignment operator */
NullAccumulator& NullAccumulator::operator=(const NullAccumulator &other)
{
    Accumulator::operator=(other);
    return *this;
}

/** Comparison operator */
bool NullAccumulator::operator==(const NullAccumulator &other) const
{
    return true;
}

/** Comparison operator */
bool NullAccumulator::operator!=(const NullAccumulator &other) const
{
    return false;
}

/** Accumulate the passed value onto the average */
void NullAccumulator::accumulate(double)
{}

static SharedPolyPointer<NullAccumulator> shared_null;

const NullAccumulator& Accumulator::null()
{
    if (shared_null.constData() == 0)
    {
        QMutexLocker lkr( SireBase::globalLock() );
        
        if (shared_null.constData() == 0)
            shared_null = new NullAccumulator();
    }
    
    return *(shared_null.constData());
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
        ds >> avg.avgval >> static_cast<Accumulator&>(avg);
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
    return this->average();
}

/////////
///////// Implementation of AverageAndStddev
/////////

static const RegisterMetaType<AverageAndStddev> r_avgstddev;

/** Serialise to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, 
                                         const AverageAndStddev &avgstddev)
{
    writeHeader(ds, r_avgstddev, 1);
    
    ds << avgstddev.avgval2 << static_cast<const Average&>(avgstddev);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds,
                                         AverageAndStddev &avgstddev)
{
    VersionID v = readHeader(ds, r_avgstddev);
    
    if (v == 1)
    {
        ds >> avgstddev.avgval2 >> static_cast<Average&>(avgstddev);
    }
    else
        throw version_error( v, "1", r_avgstddev, CODELOC );
        
    return ds;
}

/** Construct an empty average */
AverageAndStddev::AverageAndStddev()
                 : ConcreteProperty<AverageAndStddev,Average>(),
                   avgval2(0)
{}

/** Copy constructor */
AverageAndStddev::AverageAndStddev(const AverageAndStddev &other)
                 : ConcreteProperty<AverageAndStddev,Average>(other),
                   avgval2(other.avgval2)
{}

/** Destructor */
AverageAndStddev::~AverageAndStddev()
{}

/** Copy assignment operator */
AverageAndStddev& AverageAndStddev::operator=(const AverageAndStddev &other)
{
    if (this != &other)
    {
        avgval2 = other.avgval2;
        Average::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool AverageAndStddev::operator==(const AverageAndStddev &other) const
{
    return avgval2 == other.avgval2 and
           Average::operator==(other);
}

/** Comparison operator */
bool AverageAndStddev::operator!=(const AverageAndStddev &other) const
{
    return not this->operator==(other);
}

/** Accumulate the average and standard deviation */
void AverageAndStddev::accumulate(double value)
{
    double nsteps = this->nSamples() + 1;
    
    //calculate the average of the squares as
    // average2 = ((n-1)/n) * average2 + (1/n) * value * value
    
    double big_ratio = (nsteps - 1) / nsteps;
    double small_ratio = 1.0 / nsteps;
    
    avgval2 = (big_ratio * avgval2) + (small_ratio * value * value);

    Average::accumulate(value);
}

/** Return the standard deviation of the average
    (calculated as the sqrt of the mean of the squares minus
     the square of the mean) */
double AverageAndStddev::stddev() const
{
    return std::sqrt( avgval2 - pow_2(this->average()) );
}

/** Return the standard deviation of the average
    (calculated as the sqrt of the mean of the squares minus
     the square of the mean) */
double AverageAndStddev::standardDeviation() const
{
    return this->stddev();
}

/** Return the mean average of the squares */
double AverageAndStddev::meanOfSquares() const
{
    return avgval2;
}

/////////
///////// Implementation of ExpAverage
/////////

static const RegisterMetaType<ExpAverage> r_expavg;

/** Serialise to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const ExpAverage &expavg)
{
    writeHeader(ds, r_expavg, 1);
    
    ds << expavg.avgval << expavg.sclfac
       << static_cast<const Accumulator&>(expavg);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, ExpAverage &expavg)
{
    VersionID v = readHeader(ds, r_expavg);
    
    if (v == 1)
    {
        ds >> expavg.avgval >> expavg.sclfac
           >> static_cast<Accumulator&>(expavg);
    }
    else
        throw version_error(v, "1", r_expavg, CODELOC);
        
    return ds;
}

/** Construct an empty average using the passed scale factor.

    \throw SireError::invalid_arg
*/
ExpAverage::ExpAverage(double scale_factor) 
           : ConcreteProperty<ExpAverage,Accumulator>(), 
             avgval(0), sclfac(scale_factor)
{
    if (scale_factor == 0)
        throw SireError::invalid_arg( QObject::tr(
            "You cannot construct an exponential average using a scale "
            "factor of zero."), CODELOC );
            
    //take the inverse of the scale factor
    sclfac = 1.0 / sclfac;
}

/** Copy constructor */
ExpAverage::ExpAverage(const ExpAverage &other)
        : ConcreteProperty<ExpAverage,Accumulator>(other), 
          avgval(other.avgval), sclfac(other.sclfac)
{}

/** Destructor */
ExpAverage::~ExpAverage()
{}

/** Copy assignment operator */
ExpAverage& ExpAverage::operator=(const ExpAverage &other)
{
    if (this != &other)
    {
        avgval = other.avgval;
        sclfac = other.sclfac;
        Accumulator::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool ExpAverage::operator==(const ExpAverage &other) const
{
    return avgval == other.avgval and
           sclfac == other.sclfac and
           Accumulator::operator==(other);
}

/** Comparison operator */
bool ExpAverage::operator!=(const ExpAverage &other) const
{
    return not this->operator==(other);
}

/** Accumulate the passed value onto the average */
void ExpAverage::accumulate(double value)
{
    double expvalue = std::exp( sclfac * value );

    double nsteps = this->nSamples() + 1;
    
    //calculate the average as
    // average = ((n-1)/n) * average + (1/n) * expvalue
    
    double big_ratio = (nsteps - 1) / nsteps;
    double small_ratio = 1.0 / nsteps;
    
    avgval = (big_ratio * avgval) + (small_ratio * expvalue);
    Accumulator::accumulate(value);
}

/** Return the average value */
double ExpAverage::average() const
{
    return ( 1.0 / sclfac ) * std::log(avgval);
}

/** Allow automatic casting to a double to retrieve the average value */
ExpAverage::operator double() const
{
    return this->average();
}

/** Internal function used to return the scale factor */
double ExpAverage::scaleFactor() const
{
    return 1.0 / sclfac;
}

/////////
///////// Implementation of Median
/////////

static const RegisterMetaType<Median> r_median;

/** Serialise to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const Median &median)
{
    writeHeader(ds, r_median, 1);
    
    ds << median.minval << median.maxval
       << static_cast<const Accumulator&>(median);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, Median &median)
{
    VersionID v = readHeader(ds, r_median);
    
    if (v == 1)
    {
        ds >> median.minval >> median.maxval
           >> static_cast<Accumulator&>(median);
    }
    else
        throw version_error(v, "1", r_median, CODELOC);
        
    return ds;
}

/** Construct an empty average */
Median::Median() 
       : ConcreteProperty<Median,Accumulator>(), 
         minval(  std::numeric_limits<double>::max() ),
         maxval( -std::numeric_limits<double>::max() )
{}

/** Copy constructor */
Median::Median(const Median &other)
        : ConcreteProperty<Median,Accumulator>(other), 
          minval(other.minval), maxval(other.maxval)
{}

/** Destructor */
Median::~Median()
{}

/** Copy assignment operator */
Median& Median::operator=(const Median &other)
{
    if (this != &other)
    {
        minval = other.minval;
        maxval = other.maxval;
        Accumulator::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool Median::operator==(const Median &other) const
{
    return minval == other.minval and
           maxval == other.maxval and
           Accumulator::operator==(other);
}

/** Comparison operator */
bool Median::operator!=(const Median &other) const
{
    return not this->operator==(other);
}

/** Accumulate the passed value onto the average */
void Median::accumulate(double value)
{
    if (value < minval)
        minval = value;
        
    if (value > maxval)
        maxval = value;

    Accumulator::accumulate(value);
}

/** Return the median value */
double Median::median() const
{
    return (0.5 * maxval) + (0.5 * minval);   // the sum of maxval and minval
                                              // could overflow
}

/** Allow automatic casting to a double to retrieve the average value */
Median::operator double() const
{
    return this->median();
}

/** Return the maximum value */
double Median::max() const
{
    return maxval;
}

/** Return the maximum value */
double Median::maximum() const
{
    return this->max();
}

/** Return the minimum value */
double Median::min() const
{
    return minval;
}

/** Return the minimum value */
double Median::minimum() const
{
    return this->min();
}