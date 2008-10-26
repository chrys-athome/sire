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

#include "histogram.h"

#include "SireID/index.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMaths;
using namespace SireID;
using namespace SireUnits::Dimension;
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
/////////// Implementation of HistogramRange
///////////

/** Serialise to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds,
                                         const HistogramRange &range)
{
    ds << range.minval << range.invwidth << range.nbins;
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, 
                                         HistogramRange &range)
{
    ds >> range.minval >> range.invwidth >> range.nbins;
    return ds;
}

/** Null constructor */
HistogramRange::HistogramRange() : minval(0), invwidth(1), nbins(0)
{}

/** Construct a histogram range from min <= value < max, using 'nbins' bins */
HistogramRange::HistogramRange(double min, double max, int numbins)
               : nbins(numbins)
{
    if (min > max)
    {
        qSwap(min, max);
    }
    
    minval = min;
 
    if (nbins <= 0)
    {
        nbins = 0;
        invwidth = 1;
    }
    else
    {
        invwidth = double(nbins) / (max - min);
    }
}

/** Construct a histogram range from min <= value < max, using a bin width
    of 'binwidth' */
HistogramRange::HistogramRange(double min, double max, double binwidth)
{
    if (min > max)
    {
        qSwap(min, max);
    }
    
    minval = min;
    
    if (binwidth <= 0)
    {
        nbins = 0;
        invwidth = 1;
    }
    else
    {
        invwidth = 1.0 / binwidth;
        nbins = 0.5 + ((max - min) * invwidth);
    }
}

/** Copy constructor */
HistogramRange::HistogramRange(const HistogramRange &other)
               : minval(other.minval), invwidth(other.invwidth), nbins(other.nbins)
{}

/** Destructor */
HistogramRange::~HistogramRange()
{}

/** Copy assignment operator */
HistogramRange& HistogramRange::operator=(const HistogramRange &other)
{
    minval = other.minval;
    invwidth = other.invwidth;
    nbins = other.nbins;
    return *this;
}

/** Comparison operator */
bool HistogramRange::operator==(const HistogramRange &other) const
{
    return minval == other.minval and invwidth == other.invwidth
               and nbins == other.nbins;
}

/** Comparison operator */
bool HistogramRange::operator!=(const HistogramRange &other) const
{
    return minval != other.minval or invwidth != other.invwidth or
           nbins != other.nbins;
}

/** Return the 'ith' bin

    \throw SireError::invalid_index
*/
HistogramBin HistogramRange::operator[](int i) const
{
    i = Index(i).map(nbins);
    
    double binwidth = 1.0 / invwidth;
    
    double min = minval + (i * binwidth);
    
    return HistogramBin(min, min + binwidth);
}

/** Return the maximum value of the histogram */
double HistogramRange::maximum() const
{
    return minval + (double(nbins) / invwidth);
}

/** Return the minimum value of the histogram */
double HistogramRange::minimum() const
{
    return minval;
}

/** Return the middle value of the histogram */
double HistogramRange::middle() const
{
    return 0.5*minimum() + 0.5*maximum();
}

/** Return the number of bins in the histogram */
int HistogramRange::nBins() const
{
    return nbins;
}

/** Return the number of bins in the histogram */
int HistogramRange::count() const
{
    return nBins();
}

/** Return the width of each bin in the histogram */
double HistogramRange::binWidth() const
{
    return 1.0 / invwidth;
}

/** Return the index of the bin that contains the value 'value'. 
    This returns '-1' if this value does not fit in this histogram */
int HistogramRange::bin(double value) const
{
    int idx = int( (value - minval) * invwidth );

    if (idx < 0 or idx >= int(nbins))
        return -1;
        
    return idx;
}

/** Return a string representation */
QString HistogramRange::toString() const
{
    return QObject::tr("Range[ %1 <= x < %2 : nBins() == %3 ]")
                    .arg( minimum() ).arg( maximum() ).arg(nBins());
}

///////////
/////////// Implementation of Histogram
///////////

static const RegisterMetaType<Histogram> r_histogram;

/** Serialise to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const Histogram &histogram)
{
    writeHeader(ds, r_histogram, 1);
    
    SharedDataStream sds(ds);
    
    sds << histogram.binvals << static_cast<const HistogramRange&>(histogram);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, Histogram &histogram)
{
    VersionID v = readHeader(ds, r_histogram);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> histogram.binvals >> static_cast<HistogramRange&>(histogram);
    }
    else
        throw version_error( v, "1", r_histogram, CODELOC );
        
    return ds;
}

/** Null constructor */
Histogram::Histogram() : HistogramRange()
{}

static QVector<double> create(const HistogramRange &range)
{
    if (range.nBins() > 0)
    {
        QVector<double> bins(range.nBins(), 0.0);
        bins.squeeze();
        return bins;
    }
    else
        return QVector<double>();
}

/** Construct a histogram that can hold values from min <= value < max, 
    using 'nbins' bins */
Histogram::Histogram(double min, double max, int nbins)
          : HistogramRange(min, max, nbins)
{
    binvals = ::create(*this);
}

/** Construct a histogram that can hold values from min <= value < max,
    using a bin width of 'binwidth' */
Histogram::Histogram(double min, double max, double binwidth)
          : HistogramRange(min, max, binwidth)
{
    binvals = ::create(*this);
}

/** Construct a histogram using the passed range */
Histogram::Histogram(const HistogramRange &range)
          : HistogramRange(range), binvals( ::create(range) )
{}

/** Construct a histogram using the passed range and values 

    \throw SireError::incompatible_error
*/
Histogram::Histogram(const HistogramRange &range,
                     const QVector<double> &values)
          : HistogramRange(range), binvals(values)
{
    if (range.nBins() != values.count())
        throw SireError::incompatible_error( QObject::tr(
            "The histogram is incompatible with the array of passed values "
            "because the number of values (%1) is not equal to the number "
            "of histogram bins (%2).")
                .arg(binvals.count()).arg(range.nBins()), CODELOC );
}

/** Copy constructor */
Histogram::Histogram(const Histogram &other)
          : HistogramRange(other), binvals(other.binvals)
{}

/** Destructor */
Histogram::~Histogram()
{}

/** Copy assignment operator */
Histogram& Histogram::operator=(const Histogram &other)
{
    binvals = other.binvals;
    HistogramRange::operator=(other);
    return *this;
}

/** Comparison operator */
bool Histogram::operator==(const Histogram &other) const
{
    return HistogramRange::operator==(other) and binvals == other.binvals;
}

/** Comparison operator */
bool Histogram::operator!=(const Histogram &other) const
{
    return HistogramRange::operator!=(other) or binvals != other.binvals;
}

/** Return the value of the ith bin

    \throw SireError::invalid_index
*/
HistogramValue Histogram::operator[](int i) const
{
    i = Index(i).map( this->nBins() );

    return HistogramValue( HistogramRange::operator[](i),
                           binvals.constData()[i] );
}

/** Return a raw pointer to the values in this histogram */
const double* Histogram::data() const
{
    return binvals.constData();
}

/** Return a raw pointer to the values in this histogram */
const double* Histogram::constData() const
{
    return binvals.constData();
}

/** Assumulate the value 'value' (with a weight of 'weight') */
void Histogram::accumulate(double value, double weight)
{
    int idx = this->bin(value);
    
    if (idx != -1)
        binvals[idx] += weight;
}

/** Assumulate the value 'value' (with a weight of 1.0) */
void Histogram::accumulate(double value)
{
    this->accumulate(value, 1);
}

/** Return a string representation */
QString Histogram::toString() const
{
    return QObject::tr("Histogram[ %1 <= x < %2 : nBins() == %3 ]")
                    .arg( minimum() ).arg( maximum() ).arg( nBins() );
}

//// Fully instantiate the Length and Energy histogram classes

template class HistogramT<Length>;
template class HistogramBinT<Length>;
template class HistogramValueT<Length>;
template class HistogramRangeT<Length>;

template class HistogramT<Energy>;
template class HistogramBinT<Energy>;
template class HistogramValueT<Energy>;
template class HistogramRangeT<Energy>;
