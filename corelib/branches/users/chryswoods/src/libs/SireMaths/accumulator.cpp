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

#include "Siren/objref.h"
#include "Siren/errors.h"
#include "Siren/stream.h"
#include "Siren/streamqt.h"

#include <QDebug>

using namespace SireMaths;
using namespace Siren;

/////////
///////// Implementation of Accumulator
/////////

static const RegisterObject<Accumulator> r_accum( VIRTUAL_CLASS );

/** Constructor */
Accumulator::Accumulator() 
            : Extends<Accumulator,Object>(),
              Interfaces<Accumulator,Mutable>(), nvalues(0)
{}

/** Copy constructor */
Accumulator::Accumulator(const Accumulator &other)
            : Extends<Accumulator,Object>(other),
              Interfaces<Accumulator,Mutable>(), nvalues(other.nvalues)
{}

/** Destructor */
Accumulator::~Accumulator()
{}

QString Accumulator::typeName()
{
    return "Siren::Accumulator";
}

ObjRef Accumulator::saveState() const
{
    return this->clone();
}

void Accumulator::restoreState(const Object &object)
{
    this->copy(object);
}

void Accumulator::stream(Stream &s)
{
    s.assertVersion<Accumulator>(1);
    
    Schema schema = s.item<Accumulator>();
    
    schema.data("nvalues") & nvalues;
    
    Object::stream( schema.base() );
}

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
    Object::operator=(other);
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

uint Accumulator::hashCode() const
{
    return qHash(this->what()) + qHash(this->value()) 
             + qHash(this->nSamples());
}

/** Return the value of this accumulator - this meaning
    of the value depends on the type, e.g. Average would
    return the mean average value, Median would return
    the median value etc. */
double Accumulator::value() const
{
    return this->operator double();
}

/////////
///////// Implementation of Average
/////////

static const RegisterObject<Average> r_avg;

/** Construct an empty average */
Average::Average() : Implements<Average,Accumulator>(), avgval(0)
{}

/** Copy constructor */
Average::Average(const Average &other)
        : Implements<Average,Accumulator>(other), avgval(other.avgval)
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

QString Average::toString() const
{
    return QObject::tr("Average( value() = %1, nSamples() = %2 )")
                .arg(this->value()).arg(this->nSamples());
}

void Average::stream(Stream &s)
{
    s.assertVersion<Average>(1);
    
    Schema schema = s.item<Average>();
    
    schema.data("average") & avgval;
    
    Accumulator::stream( schema.base() );
}

/** Completely clear the statistics in this accumulator */
void Average::clear()
{
    avgval = 0;
    Accumulator::clear();
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

static const RegisterObject<AverageAndStddev> r_avgstddev;

/** Construct an empty average */
AverageAndStddev::AverageAndStddev()
                 : Implements<AverageAndStddev,Average>(),
                   avgval2(0)
{}

/** Copy constructor */
AverageAndStddev::AverageAndStddev(const AverageAndStddev &other)
                 : Implements<AverageAndStddev,Average>(other),
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

/** Completely clear the statistics in this accumulator */
void AverageAndStddev::clear()
{
    avgval2 = 0;
    Average::clear();
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

QString AverageAndStddev::toString() const
{
    return QObject::tr("AverageAndStdDev( value() = %1, stddev() = %2, nSamples() = %3 )")
                .arg(this->value()).arg(this->stddev()).arg(this->nSamples());
}

void AverageAndStddev::stream(Stream &s)
{
    s.assertVersion<AverageAndStddev>(1);
    
    Schema schema = s.item<AverageAndStddev>();
    
    schema.data("average^2") & avgval2;
    
    Average::stream( schema.base() );
}

/////////
///////// Implementation of ExpAverage
/////////

static const RegisterObject<ExpAverage> r_expavg;

/** Construct an empty average using the passed scale factor.

    \throw SireError::invalid_arg
*/
ExpAverage::ExpAverage(double scale_factor) 
           : Implements<ExpAverage,Accumulator>(), 
             avgval(0), sclfac(scale_factor)
{
    if (scale_factor == 0)
        throw Siren::invalid_arg( QObject::tr(
            "You cannot construct an exponential average using a scale "
            "factor of zero."), CODELOC );
            
    //take the inverse of the scale factor
    sclfac = 1.0 / sclfac;
}

/** Copy constructor */
ExpAverage::ExpAverage(const ExpAverage &other)
        : Implements<ExpAverage,Accumulator>(other), 
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

/** Completely clear the statistics in this accumulator */
void ExpAverage::clear()
{
    avgval = 0;
    Accumulator::clear();
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

QString ExpAverage::toString() const
{
    return QObject::tr("ExpAverage( value() = %1, nSamples() = %2 )")
                .arg(this->value()).arg(this->nSamples());
}

void ExpAverage::stream(Stream &s)
{
    s.assertVersion<ExpAverage>(1);
    
    Schema schema = s.item<ExpAverage>();
    
    schema.data("average") & avgval;
    schema.data("scale_factor") & sclfac;
    
    Accumulator::stream( schema.base() );
}

/////////
///////// Implementation of Median
/////////

static const RegisterObject<Median> r_median;

/** Construct an empty average */
Median::Median() 
       : Implements<Median,Accumulator>(), 
         minval(  std::numeric_limits<double>::max() ),
         maxval( -std::numeric_limits<double>::max() )
{}

/** Copy constructor */
Median::Median(const Median &other)
        : Implements<Median,Accumulator>(other), 
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

/** Completely clear the statistics in this accumulator */
void Median::clear()
{
    minval = 0;
    maxval = 0;
    Accumulator::clear();
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

QString Median::toString() const
{
    return QObject::tr("Median( value() = %1, nSamples() = %2 )")
                .arg(this->value()).arg(this->nSamples());
}

void Median::stream(Stream &s)
{
    s.assertVersion<Median>(1);
    
    Schema schema = s.item<Median>();
    
    schema.data("minimum") & minval;
    schema.data("maximum") & maxval;
    
    Accumulator::stream( schema.base() );
}

/////////
///////// Implementation of RecordValues
/////////

static const RegisterObject<RecordValues> r_recval;

/** Construct an empty average */
RecordValues::RecordValues() : Implements<RecordValues,Accumulator>()
{}

/** Copy constructor */
RecordValues::RecordValues(const RecordValues &other)
             : Implements<RecordValues,Accumulator>(other), 
               vals(other.vals)
{}

/** Destructor */
RecordValues::~RecordValues()
{}

/** Copy assignment operator */
RecordValues& RecordValues::operator=(const RecordValues &other)
{
    if (this != &other)
    {
        vals = other.vals;
        Accumulator::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool RecordValues::operator==(const RecordValues &other) const
{
    return vals == other.vals and
           Accumulator::operator==(other);
}

/** Comparison operator */
bool RecordValues::operator!=(const RecordValues &other) const
{
    return not this->operator==(other);
}

/** Completely clear the statistics in this accumulator */
void RecordValues::clear()
{
    vals.clear();
    Accumulator::clear();
}

/** Accumulate the passed value onto the average */
void RecordValues::accumulate(double value)
{
    vals.append(value);

    Accumulator::accumulate(value);
}

/** Return the number of recorded values */
int RecordValues::count() const
{
    return vals.count();
}

/** Return the number of recorded values */
int RecordValues::size() const
{
    return this->count();
}

/** Return the number of recorded values */
int RecordValues::nValues() const
{
    return this->count();
}

/** Return the maximum value */
double RecordValues::max() const
{
    int nvals = this->count();

    if (nvals == 0)
        return 0;
        
    double maxval = -(std::numeric_limits<double>::max());
    
    const double *vals_array = vals.constData();
    
    for (int i=0; i<nvals; ++i)
    {
        if (vals_array[i] > maxval)
            maxval = vals_array[i];
    }
    
    return maxval;
}

/** Return the maximum value */
double RecordValues::maximum() const
{
    return this->max();
}

/** Return the minimum value */
double RecordValues::min() const
{
    int nvals = this->count();

    if (nvals == 0)
        return 0;
        
    double minval = std::numeric_limits<double>::max();
    
    const double *vals_array = vals.constData();
    
    for (int i=0; i<nvals; ++i)
    {
        if (vals_array[i] < minval)
            minval = vals_array[i];
    }
    
    return minval;
}

/** Return the minimum value */
double RecordValues::minimum() const
{
    return this->min();
}

/** Return the sum of all of the values */
double RecordValues::sum() const
{
    int nvals = vals.count();
    const double *vals_array = vals.constData();
    
    double sum = 0;
    
    for (int i=0; i<nvals; ++i)
    {
        sum += vals_array[i];
    }
    
    return sum;
}

/** Return the sum of the square of all of the values */
double RecordValues::sum2() const
{
    int nvals = vals.count();
    const double *vals_array = vals.constData();
    
    double sum2 = 0;
    
    for (int i=0; i<nvals; ++i)
    {
        sum2 += pow_2( vals_array[i] );
    }
    
    return sum2;
}

/** Return the median value */
double RecordValues::median() const
{
    return (0.5 * min()) + (0.5 * max());   // the sum of maxval and minval
                                            // could overflow
}

/** Return the mean value */
double RecordValues::mean() const
{
    if (this->count() == 0)
        return 0;
        
    else
        return this->sum() / this->count();
}

/** Return the mean of the square values */
double RecordValues::meanOfSquares() const
{
    if (this->count() == 0)
        return 0;
        
    else
        return this->sum2() / this->count();
}

/** Return the standard deviation of the values */
double RecordValues::standardDeviation() const
{
    return std::sqrt( this->meanOfSquares() - pow_2(this->mean()) );
}

/** Return the standard deviation of the values */
double RecordValues::stddev() const
{
    return this->standardDeviation();
}

/** Allow automatic casting to a double to retrieve the mean average value */
RecordValues::operator double() const
{
    return this->mean();
}

/** Return the array of all accumulated values */
QVector<double> RecordValues::values() const
{
    return vals;
}

QString RecordValues::toString() const
{
    return QObject::tr("RecordValues( value() = %1, nSamples() = %2 )")
                .arg(this->value()).arg(this->nSamples());
}

void RecordValues::stream(Stream &s)
{
    s.assertVersion<RecordValues>(1);
    
    Schema schema = s.item<RecordValues>();
    
    schema.data("values") & vals;
    
    Accumulator::stream( schema.base() );
}
