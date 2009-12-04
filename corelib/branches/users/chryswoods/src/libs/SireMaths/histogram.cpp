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

#include "Siren/errors.h"
#include "Siren/stream.h"
#include "Siren/streamqt.h"

using namespace SireMaths;
using namespace SireID;
using namespace Siren;

///////////
/////////// Implementation of HistogramBin
///////////

static const RegisterObject<HistogramBin> r_histbin;

/** Null constructor */
HistogramBin::HistogramBin() 
             : Implements<HistogramBin,Object>(), minval(0), maxval(0)
{}

/** Construct a bin that contains the values that match
    minval <= value < maxval */
HistogramBin::HistogramBin(double min, double max)
             : Implements<HistogramBin,Object>(), minval(min), maxval(max)
{
    if (minval > maxval)
    {
        qSwap(minval, maxval);
    }
}

/** Copy constructor */
HistogramBin::HistogramBin(const HistogramBin &other)
             : Implements<HistogramBin,Object>(other),
               minval(other.minval), maxval(other.maxval)
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

uint HistogramBin::hashCode() const
{
    return qHash( HistogramBin::typeName() ) 
            + qHash(minval) + qHash(maxval);
}

void HistogramBin::stream(Stream &s)
{
    s.assertVersion<HistogramBin>(1);
    
    Schema schema = s.item<HistogramBin>();
    
    schema.data("minimum") & minval;
    schema.data("maximum") & maxval;
}

///////////
/////////// Implementation of HistogramRange
///////////

static const RegisterObject<HistogramRange> r_histrange;

/** Null constructor */
HistogramRange::HistogramRange() 
               : Implements<HistogramRange,Object>(),
                 minval(0), invwidth(1), nbins(0)
{}

/** Construct a histogram range from min <= value < max, using 'nbins' bins */
HistogramRange::HistogramRange(double min, double max, int numbins)
               : Implements<HistogramRange,Object>(), nbins(numbins)
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
               : Implements<HistogramRange,Object>()
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
               : Implements<HistogramRange,Object>(other),
                 minval(other.minval), invwidth(other.invwidth), nbins(other.nbins)
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

uint HistogramRange::hashCode() const
{
    return qHash( HistogramRange::typeName() ) + 
             qHash(minval) + qHash(nbins);
}

void HistogramRange::stream(Stream &s)
{
    s.assertVersion<HistogramRange>(1);
    
    Schema schema = s.item<HistogramRange>();
    
    schema.data("minimum") & minval;
    schema.data("inv_width") & invwidth;
    schema.data("nbins") & nbins;
}

///////////
/////////// Implementation of Histogram
///////////

static const RegisterObject<Histogram> r_histogram;

/** Null constructor */
Histogram::Histogram() 
          : Implements<Histogram,Object>(),
            Interfaces<Histogram,Mutable>()
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
          : Implements<Histogram,Object>(), 
            Interfaces<Histogram,Mutable>(),
            r(min, max, nbins)
{
    binvals = ::create(r);
}

/** Construct a histogram that can hold values from min <= value < max,
    using a bin width of 'binwidth' */
Histogram::Histogram(double min, double max, double binwidth)
          : Implements<Histogram,Object>(), 
            Interfaces<Histogram,Mutable>(),
            r(min, max, binwidth)
{
    binvals = ::create(r);
}

/** Construct a histogram using the passed range */
Histogram::Histogram(const HistogramRange &range)
          : Implements<Histogram,Object>(), r(range), binvals( ::create(range) )
{}

/** Construct a histogram using the passed range and values 

    \throw SireError::incompatible_error
*/
Histogram::Histogram(const HistogramRange &range,
                     const QVector<double> &values)
          : Implements<Histogram,Object>(), 
            Interfaces<Histogram,Mutable>(),
            r(range), binvals(values)
{
    if (range.nBins() != values.count())
        throw Siren::incompatible_error( QObject::tr(
            "The histogram is incompatible with the array of passed values "
            "because the number of values (%1) is not equal to the number "
            "of histogram bins (%2).")
                .arg(binvals.count()).arg(range.nBins()), CODELOC );
}

/** Copy constructor */
Histogram::Histogram(const Histogram &other)
          : Implements<Histogram,Object>(other),
            Interfaces<Histogram,Mutable>(), 
            r(other.r), binvals(other.binvals)
{}

/** Destructor */
Histogram::~Histogram()
{}

/** Copy assignment operator */
Histogram& Histogram::operator=(const Histogram &other)
{
    binvals = other.binvals;
    r = other.r;
    return *this;
}

/** Comparison operator */
bool Histogram::operator==(const Histogram &other) const
{
    return r == other.r and binvals == other.binvals;
}

/** Comparison operator */
bool Histogram::operator!=(const Histogram &other) const
{
    return r != other.r or binvals != other.binvals;
}

/** Return the value of the ith bin

    \throw SireError::invalid_index
*/
double Histogram::operator[](int i) const
{
    i = Index(i).map( r.nBins() );

    return binvals.constData()[i];
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

/** Accumulate the value 'value' (with a weight of 'weight') */
void Histogram::accumulate(double value, double weight)
{
    int idx = r.bin(value);
    
    if (idx != -1)
        binvals[idx] += weight;
}

/** Accumulate the value 'value' (with a weight of 1.0) */
void Histogram::accumulate(double value)
{
    this->accumulate(value, 1);
}

/** Accumulte the values in the passed histogram into this one */
void Histogram::accumulate(const Histogram &other)
{
    if ( r == other.r )
    {
        //we have the same range - just add the values
        for (int i=0; i<r.nBins(); ++i)
        {
            binvals[i] += other.binvals.at(i);
        }
        
        return;
    }

    for (int i=0; i<other.r.nBins(); ++i)
    {
        int min_idx = r.bin( other.r.minimum() );
        int max_idx = r.bin( other.r.maximum() );
        
        if (min_idx == -1)
        {
            if (max_idx == -1)
                continue;
                
            //put all of the values into the max bin
            min_idx = 0;
        }
        else if (max_idx == -1)
        {
            max_idx = r.nBins() - 1;
        }
        
        double val = other.binvals.at(i) / double(max_idx - min_idx + 1);
        
        for (int j=min_idx; j<=max_idx; ++j)
        {
            this->binvals[j] += val;
        }
    }
}

/** Return a string representation */
QString Histogram::toString() const
{
    return QObject::tr("Histogram[ %1 <= x < %2 : nBins() == %3 ]")
                    .arg( r.minimum() ).arg( r.maximum() ).arg( r.nBins() );
}

uint Histogram::hashCode() const
{
    return qHash( Histogram::typeName() ) + qHash(r);
}

void Histogram::stream(Stream &s)
{
    s.assertVersion<Histogram>(1);
    
    Schema schema = s.item<Histogram>();
    
    schema.data("range") & r;
    schema.data("data") & binvals;
    
    Object::stream( schema.base() );
}
