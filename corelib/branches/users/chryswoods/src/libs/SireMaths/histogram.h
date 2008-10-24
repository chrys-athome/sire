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

#ifndef SIREMATHS_HISTOGRAM_H
#define SIREMATHS_HISTOGRAM_H

#include <QVector>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class HistogramBin;
class HistogramValue;
class HistogramRange;
class Histogram;
}

QDataStream& operator<<(QDataStream&, const SireMaths::HistogramBin&);
QDataStream& operator>>(QDataStream&, SireMaths::HistogramBin&);

QDataStream& operator<<(QDataStream&, const SireMaths::HistogramValue&);
QDataStream& operator>>(QDataStream&, SireMaths::HistogramValue&);

QDataStream& operator<<(QDataStream&, const SireMaths::HistogramRange&);
QDataStream& operator>>(QDataStream&, SireMaths::HistogramRange&);

QDataStream& operator<<(QDataStream&, const SireMaths::Histogram&);
QDataStream& operator>>(QDataStream&, SireMaths::Histogram&);

namespace SireMaths
{

/** This class represents a single histogram bin */
class SIREMATHS_EXPORT HistogramBin
{

friend QDataStream& ::operator<<(QDataStream&, const HistogramBin&);
friend QDataStream& ::operator>>(QDataStream&, HistogramBin&);

public:
    HistogramBin();
    HistogramBin(double minval, double maxval);
    
    HistogramBin(const HistogramBin &other);
    
    HistogramBin& operator=(const HistogramBin &other);
    
    bool operator==(const HistogramBin &other) const;
    bool operator!=(const HistogramBin &other) const;
    
    double minimum() const;
    double middle() const;
    double maximum() const;

private:
    /** The minimum value of the bin */
    double minval;
    
    /** The maximum value of the bin */
    double maxval;
};

/** This class represents a single histogram bin with its associated value */
class SIREMATHS_EXPORT HistogramValue : public HistogramBin
{

friend QDataStream& ::operator<<(QDataStream&, const HistogramValue&);
friend QDataStream& ::operator>>(QDataStream&, HistogramValue&);

public:
    HistogramValue();
    HistogramValue(const HistogramBin &bin, double value);
    
    HistogramValue(const HistogramValue &other);
    
    ~HistogramValue();
    
    HistogramValue& operator=(const HistogramValue &other);
    
    bool operator==(const HistogramValue &other) const;
    bool operator!=(const HistogramValue &other) const;
    
    double value() const;

private:
    /** The actual value in the bin */
    double val;
};

/** This class represents a histogram range 
    (minimum, maximum, number of bins, bin width)
    
    @author Christopher Woods
*/
class SIREMATHS_EXPORT HistogramRange
{

friend QDataStream& ::operator<<(QDataStream&, const HistogramRange&);
friend QDataStream& ::operator>>(QDataStream&, HistogramRange&);

public:
    HistogramRange();
    HistogramRange(double min, double max, int nbins=101);
    HistogramRange(double min, double max, double binwidth);
    
    HistogramRange(const HistogramRange &other);
    
    ~HistogramRange();
    
    HistogramRange& operator=(const HistogramRange &other);
    
    bool operator==(const HistogramRange &other) const;
    bool operator!=(const HistogramRange &other) const;
    
    HistogramBin operator[](int i) const;
    
    double maximum() const;
    double middle() const;
    double minimum() const;
    
    int nBins() const;
    int count() const;

    double binWidth() const;

    int bin(double value) const;

private:
    /** Minimum bin value - data must have a value greater
        than or equal to this value to be included in the histogram */
    double minval;
    
    /** The width of each bin */
    double invwidth;
    
    /** The number of bins in the histogram */
    quint32 nbins;
};

/** This class holds a simple one-dimensional (x,y) histogram of values.

    @author Christopher Woods
*/
class SIREMATHS_EXPORT Histogram : public HistogramRange
{

friend QDataStream& ::operator<<(QDataStream&, const Histogram&);
friend QDataStream& ::operator>>(QDataStream&, Histogram&);

public:
    Histogram();
    
    Histogram(double min, double max, int nbins=101);
    Histogram(double min, double max, double binwidth);

    Histogram(const HistogramRange &range);
    
    Histogram(const Histogram &other);
    
    ~Histogram();
    
    Histogram& operator=(const Histogram &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Histogram>() );
    }
    
    const char* what() const
    {
        return Histogram::typeName();
    }
    
    Histogram* clone() const
    {
        return new Histogram(*this);
    }

    bool operator==(const Histogram &other) const;
    bool operator!=(const Histogram &other) const;
    
    HistogramValue operator[](int i) const;
    
    const double* data() const;
    const double* constData() const;
    
    void accumulate(double value);
    void accumulate(double value, double weight);

private:
    /** The values in each of the bins */
    QVector<double> binvals;
};

}

Q_DECLARE_METATYPE( SireMaths::Histogram )

SIRE_EXPOSE_CLASS( SireMaths::Histogram )

SIRE_END_HEADER

#endif
