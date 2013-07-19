/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008-2013  Christopher Woods
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

#include "histogram.h"

#include "SireMaths/maths.h"

#include "SireID/index.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMaths;
using namespace SireID;
using namespace SireUnits::Dimension;
using namespace SireBase;
using namespace SireStream;

///////////
/////////// Implementation of HistogramBin
///////////

/** Serialise to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds,
                                         const HistogramBin &bin)
{
    ds << bin.minimum() << bin.maximum();
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, HistogramBin &bin)
{
    ds >> bin.minval >> bin.maxval;
    return ds;
}

/** Null constructor */
HistogramBin::HistogramBin() : minval(0), maxval(0)
{}

/** Construct a bin that contains the values that match
    minval <= value < maxval */
HistogramBin::HistogramBin(double min, double max)
             : minval(min), maxval(max)
{
    if (minval > maxval)
    {
        qSwap(minval, maxval);
    }
}

/** Copy constructor */
HistogramBin::HistogramBin(const HistogramBin &other)
             : minval(other.minval), maxval(other.maxval)
{}

/** Copy assignment operator */
HistogramBin& HistogramBin::operator=(const HistogramBin &other)
{
    minval = other.minval;
    maxval = other.maxval;
    return *this;
}

/** Comparison operator */
bool HistogramBin::operator==(const HistogramBin &other) const
{
    return minval == other.minval and maxval == other.maxval;
}

/** Comparison operator */
bool HistogramBin::operator!=(const HistogramBin &other) const
{
    return minval != other.minval or maxval != other.maxval;
}

/** Return the minimum value of the bin */
double HistogramBin::minimum() const
{
    return minval;
}

/** Return the value at the middle of the bin */
double HistogramBin::middle() const
{
    return 0.5*maxval + 0.5*minval;
}

/** Return the maximum value of the bin */
double HistogramBin::maximum() const
{
    return maxval;
}

/** Return a string representation */
QString HistogramBin::toString() const
{
    return QObject::tr("Bin[ %1 <= x < %2 ]")
                    .arg(minval).arg(maxval);
}

///////////
/////////// Implementation of HistogramValue
///////////

/** Serialise to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds,
                                         const HistogramValue &value)
{
    ds << value.val << static_cast<const HistogramBin&>(value);
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds,
                                         HistogramValue &value)
{
    ds >> value.val >> static_cast<HistogramBin&>(value);
    return ds;
}

/** Null constructor */
HistogramValue::HistogramValue() : HistogramBin(), val(0)
{}

/** Construct the value for the bin 'bin' equal to 'value' */
HistogramValue::HistogramValue(const HistogramBin &bin, double value)
               : HistogramBin(bin), val(value)
{}

/** Copy constructor */
HistogramValue::HistogramValue(const HistogramValue &other)
               : HistogramBin(other), val(other.val)
{}

/** Destructor */
HistogramValue::~HistogramValue()
{}

/** Copy assignment operator */
HistogramValue& HistogramValue::operator=(const HistogramValue &other)
{
    val = other.val;
    HistogramBin::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool HistogramValue::operator==(const HistogramValue &other) const
{
    return val == other.val and HistogramBin::operator==(other);
}

/** Comparison operator */
bool HistogramValue::operator!=(const HistogramValue &other) const
{
    return val != other.val or HistogramBin::operator!=(other);
}

/** Return the value of the bin */
double HistogramValue::value() const
{
    return val;
}

/** Return a string representation */
QString HistogramValue::toString() const
{
    return QObject::tr("Bin[ %1 <= x < %2 ] == %3")
                    .arg( minimum() ).arg( maximum() ).arg(val);
}

///////////
/////////// Implementation of Histogram
///////////

static const RegisterMetaType<Histogram> r_histogram;

/** Serialise to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const Histogram &histogram)
{
    writeHeader(ds, r_histogram, 2);
    
    SharedDataStream sds(ds);
    
    sds << histogram.binvals << histogram.binwidth
        << histogram.avgval << histogram.avgval2
        << histogram.sum_of_bins
        << static_cast<const Property&>(histogram);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, Histogram &histogram)
{
    VersionID v = readHeader(ds, r_histogram);
    
    if (v == 2)
    {
        SharedDataStream sds(ds);

        sds >> histogram.binvals >> histogram.binwidth
            >> histogram.avgval >> histogram.avgval2
            >> histogram.sum_of_bins
            >> static_cast<Property&>(histogram);
    }
    else
        throw version_error( v, "2", r_histogram, CODELOC );
        
    return ds;
}

/** Construct an empty histogram with a bin spacing of 1.0 */
Histogram::Histogram()
          : ConcreteProperty<Histogram,Property>(),
            binwidth(1), avgval(0), avgval2(0), sum_of_bins(0)
{}

/** Construct an empty histogram with specified bin width */
Histogram::Histogram(double width)
          : ConcreteProperty<Histogram,Property>(),
            binwidth(width), avgval(0), avgval2(0), sum_of_bins(0)
{
    if (width <= 0)
        width = 0.1;
    else if (width > 1e20)
        width = 1e20;
}

/** Construct a histogram of specified bin width, and populating it with 
    the passed values (which are all assumed to have weight "1") */
Histogram::Histogram(double width, const QVector<double> &values)
          : ConcreteProperty<Histogram,Property>(),
            binwidth(width), avgval(0), avgval2(0), sum_of_bins(0)
{
    if (width <= 0)
        width = 0.1;
    else if (width > 1e20)
        width = 1e20;

    this->operator+=(values);
}

/** Copy constructor */
Histogram::Histogram(const Histogram &other)
          : ConcreteProperty<Histogram,Property>(other),
            binvals(other.binvals), binwidth(other.binwidth),
            avgval(other.avgval), avgval2(other.avgval2),
            sum_of_bins(other.sum_of_bins)
{}

/** Destructor */
Histogram::~Histogram()
{}

/** Copy assignment operator */
Histogram& Histogram::operator=(const Histogram &other)
{
    if (this != &other)
    {
        binvals = other.binvals;
        binwidth = other.binwidth;
        avgval = other.avgval;
        avgval2 = other.avgval2;
        sum_of_bins = other.sum_of_bins;
    }
    
    return *this;
}

const char* Histogram::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Histogram>() );
}

const char* Histogram::what() const
{
    return Histogram::typeName();
}

Histogram* Histogram::clone() const
{
    return new Histogram(*this);
}

/** Comparison operator */
bool Histogram::operator==(const Histogram &other) const
{
    return this == &other or
           (binvals == other.binvals and binwidth == other.binwidth and
            avgval == other.avgval and sum_of_bins == other.sum_of_bins and
            avgval2 == other.avgval2);
}

/** Comparison operator */
bool Histogram::operator!=(const Histogram &other) const
{
    return not Histogram::operator==(other);
}

/** Add the contects of the passed histogram into this histogram */
Histogram& Histogram::operator+=(const Histogram &other)
{
    this->accumulate(other);
    return *this;
}

/** Add the passed value into this histogram */
Histogram& Histogram::operator+=(double value)
{
    this->accumulate(value);
    return *this;
}

/** Add the passed array of values onto this histogram */
Histogram& Histogram::operator+=(const QVector<double> &values)
{
    this->accumulate(values);
    return *this;
}

/** Return the accumulation of the two passed histograms. Note that the
    returned histogram will have a bin width that is equal to the smallest
    bin width of this or other */
Histogram Histogram::operator+(const Histogram &other) const
{
    if (binwidth <= other.binwidth)
    {
        Histogram ret(*this);
        ret += other;
        return ret;
    }
    else
    {
        Histogram ret(other);
        ret += *this;
        return ret;
    }
}

/** Return the histogram that is a copy of this, but on which 'value' has 
    been added */
Histogram Histogram::operator+(double value) const
{
    Histogram ret(*this);
    ret += value;
    return ret;
}

/** Return the histogram that is a copy of this, but on which the passed
    values have been added */
Histogram Histogram::operator+(const QVector<double> &values) const
{
    Histogram ret(*this);
    ret += values;
    return ret;
}

/** Return a string representation of this histogram */
QString Histogram::toString() const
{
    return QObject::tr("Histogram( binWidth() => %1, mean() => %2, sumOfBins() => %3 )")
                .arg(binWidth()).arg(mean()).arg(sumOfBins());
}

/** Return the ith bin in the histogram */
HistogramValue Histogram::operator[](int i) const
{
    i = Index(i).map(binvals.count());
    return values().at(i);
}

/** Return the ith bin in the histogram */
HistogramValue Histogram::at(int i) const
{
    return this->operator[](i);
}

/** Return the number of bins in the histogram */
int Histogram::count() const
{
    return binvals.count();
}

/** Return the number of bins in the histogram */
int Histogram::size() const
{
    return this->count();
}

/** Return the set of all bins and values in the histogram. The bins
    will be returned in numerical order */
QVector<HistogramValue> Histogram::values() const
{
    if (binvals.isEmpty())
        return QVector<HistogramValue>();

    QList<qint64> bins = binvals.keys();
    qSort(bins);
    
    QVector<HistogramValue> vals(binvals.count());
    HistogramValue *val = vals.data();
    
    foreach (qint64 bin, bins)
    {
        *val = HistogramValue( HistogramBin(bin*binwidth, (bin+1)*binwidth),
                               binvals[bin] );
        
        val += 1;
    }
    
    return vals;
}

/** Return the idealised normal distribution for the values in the histogram,
    based on the current mean and standard deviation, and the sum of weights */
QVector<HistogramValue> Histogram::normalDistribution() const
{
    if (binvals.isEmpty())
        return QVector<HistogramValue>();
    
    QList<qint64> bins = binvals.keys();
    qSort(bins);
    
    QVector<HistogramValue> vals(binvals.count());
    HistogramValue *val = vals.data();
    
    const double avg = this->mean();
    const double stdev = this->standardDeviation();
    const double denom = 1.0 / (2*stdev*stdev);
    double norm = 0;
    int nnorm = 0;

    foreach (qint64 bin, bins)
    {
        double x = (bin+0.5)*binwidth;
        
        if (std::abs( avg - x ) < stdev)
        {
            norm += binvals.value(bin) / std::exp( -pow_2(x-avg) * denom );
            nnorm += 1;
        }
    }

    if (nnorm > 0)
    {
        norm /= nnorm;
    }
    else
    {
        foreach (qint64 bin, bins)
        {
            double x = (bin+0.5)*binwidth;
            norm += binvals.value(bin) / std::exp( -pow_2(x-avg) * denom );
        }
        
        norm /= binvals.count();
    }
    
    foreach (qint64 bin, bins)
    {
        double x = (bin+0.5)*binwidth;
        
        *val = HistogramValue( HistogramBin(bin*binwidth, (bin+1)*binwidth),
                               norm * std::exp( -pow_2(x-avg) * denom ) );
        
        val += 1;
    }

    return vals;
}

/** Accumulate 'value' onto the histogram */
void Histogram::accumulate(double value)
{
    this->accumulate(value, 1.0);
}

/** Accumulate 'value' with the passed 'weight' onto the histogram */
void Histogram::accumulate(double value, double weight)
{
    //we cannot add negative weight to the histogram
    if (weight <= 0)
        return;
    
    //first, calculate the average of the
    if (sum_of_bins == 0)
    {
        avgval = value;
        avgval2 = value*value;
        sum_of_bins = weight;
    }
    else
    {
        const double bigratio = sum_of_bins / (sum_of_bins + weight);
        const double smallratio = 1.0 - bigratio;
    
        avgval = bigratio*avgval + smallratio*value;
        avgval2 = bigratio*avgval2 + smallratio*value*value;
        sum_of_bins += weight;
    }
    
    //now histogram the data
    qint64 bin = qint64(value / binwidth);
    
    binvals.insert(bin, binvals.value(bin,0) + weight);
}

/** Accumulate the passed values onto this histogram */
void Histogram::accumulate(const QVector<double> &values)
{
    foreach (double value, values)
    {
        this->accumulate(value, 1);
    }
}

/** Accumulate the data from the passed histogram onto this histogram */
void Histogram::accumulate(const Histogram &other)
{
    Histogram resized = other.resize( this->binWidth() );
    
    for (QHash<qint64,double>::const_iterator it = resized.binvals.constBegin();
         it != resized.binvals.constEnd();
         ++it)
    {
        this->accumulate( (it.key() + 0.5)*resized.binwidth, it.value() );
    }
}

/** Add 'value' onto the histogram */
void Histogram::add(double value)
{
    this->accumulate(value);
}

/** Add the passed values on this histogram */
void Histogram::add(const QVector<double> &values)
{
    this->accumulate(values);
}

/** Add 'value' with the passed 'weight' onto the histogram */
void Histogram::add(double value, double weight)
{
    this->accumulate(value, weight);
}

/** Add the passed histogram onto this histogram. This will match the
    bin width of the passed histogram to this histogram */
void Histogram::add(const Histogram &other)
{
    this->accumulate(other);
}

/** Return the sum of the weights over all of the bins */
double Histogram::sumOfBins() const
{
    return sum_of_bins;
}

/** Return the width of the bins */
double Histogram::binWidth() const
{
    return binwidth;
}

/** Return the mean average of all values added to the histogram. This
    is calculated exactly from the added data */
double Histogram::mean() const
{
    return avgval;
}

/** Return the standard deviation of all values added to the histogram.
    This is calculated exactly from the added data */
double Histogram::standardDeviation() const
{
    if (binvals.isEmpty())
        return 0;

    return std::sqrt(avgval2 - (avgval*avgval));
}

/** Return the median of all values added to the histogram. This is
    estimated based on the actual histogram of added data */
double Histogram::median() const
{
    if (binvals.isEmpty())
        return 0;

    double sum = 0;
    const double half_full = 0.5 * sumOfBins();
    
    QList<qint64> bins = binvals.keys();
    qSort(bins);
    
    foreach (qint64 bin, bins)
    {
        sum += binvals[bin];
        
        if (sum > half_full)
            return (bin+0.5)*binwidth;
    }
    
    throw SireError::program_bug( QObject::tr(
            "It should not be possible to reach here...!"), CODELOC );
    
    return 0;
}

/** Return the mode of all values added to the histogram. This is 
    estimated based on the actual histogram of added data */
double Histogram::mode() const
{
    if (binvals.isEmpty())
        return 0;
    
    double maxval = 0;
    qint64 maxbin = 0;
    
    for (QHash<qint64,double>::const_iterator it = binvals.constBegin();
         it != binvals.constEnd();
         ++it)
    {
        if (it.value() > maxval)
        {
            maxval = it.value();
            maxbin = it.key();
        }
    }
    
    return (maxbin+0.5) * binwidth;
}

/** Return the highest value in the histogram */
double Histogram::maximumValue() const
{
    if (binvals.isEmpty())
        return 0;
    
    QList<qint64> bins = binvals.keys();
    qSort(bins);
    
    return (bins.last() + 1) * binwidth;
}

/** Return the lowest values in the histogram */
double Histogram::minimumValue() const
{
    if (binvals.isEmpty())
        return 0;
    
    QList<qint64> bins = binvals.keys();
    qSort(bins);
    
    return bins.first() * binwidth;
}

/** Return the range for the data in the histogram */
double Histogram::range() const
{
    if (binvals.isEmpty())
        return 0;
    
    QList<qint64> bins = binvals.keys();
    qSort(bins);
    
    return (bins.last() - bins.first() + 1) * binwidth;
}

/** Return a normalised version of this histogram */
Histogram Histogram::normalise() const
{
    if (binvals.isEmpty())
        return Histogram();

    else if (sum_of_bins == 1)
        return *this;

    Histogram ret(*this);
    
    foreach (qint64 bin, ret.binvals.keys())
    {
        ret.binvals.insert(bin, ret.binvals.value(bin) / sum_of_bins);
    }
    
    ret.sum_of_bins = 1;
    
    return ret;
}

/** Return a resized copy of this histogram with the passed new binwidth */
Histogram Histogram::resize(double width) const
{
    if (width <= 0)
        width = 0.1;

    if (width == binwidth)
        return *this;

    Histogram ret;
    ret.binwidth = width;
    ret.avgval = avgval;
    ret.avgval2 = avgval2;
    ret.sum_of_bins = sum_of_bins;
    
    for (QHash<qint64,double>::const_iterator it = binvals.constBegin();
         it != binvals.constEnd();
         ++it)
    {
        double weight = it.value();
    
        double old_minval = it.key() * binwidth;
        double old_maxval = old_minval + binwidth;

        qint64 bin = qint64(old_minval / width);

        while (weight > 0)
        {
            double new_maxval = (bin+1)*width;
        
            if (new_maxval < old_maxval)
            {
                double partial_weight = weight * (new_maxval - old_minval) / binwidth;
                ret.binvals.insert( bin, ret.binvals.value(bin,0) + partial_weight );
                weight -= partial_weight;
                old_minval = new_maxval;
            }
            else
            {
                ret.binvals.insert( bin, ret.binvals.value(bin,0) + weight );
                weight = 0;
            }
            
            bin += 1;
        }
    }
    
    return ret;
}
