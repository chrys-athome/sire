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

#include "SireUnits/dimensions.h"

#include "tostring.h"

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

    QString toString() const;

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

    QString toString() const;

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

    QString toString() const;

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

    Histogram(const HistogramRange &range,
              const QVector<double> &values);
    
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
    
    QString toString() const;
    
    HistogramValue operator[](int i) const;
    
    const double* data() const;
    const double* constData() const;
    
    void accumulate(double value);
    void accumulate(double value, double weight);
    
    void accumulate(const Histogram &other);

private:
    /** The values in each of the bins */
    QVector<double> binvals;
};

/** This class represents a single histogram bin (of type T) */
template<class T>
class SIREMATHS_EXPORT HistogramBinT : public HistogramBin
{
public:
    HistogramBinT();
    HistogramBinT(T minval, T maxval);
    HistogramBinT(const HistogramBin &bin);
    
    HistogramBinT(const HistogramBinT<T> &other);
    
    ~HistogramBinT();
    
    HistogramBinT<T>& operator=(const HistogramBinT<T> &other);

    bool operator==(const HistogramBinT<T> &other) const;
    bool operator!=(const HistogramBinT<T> &other) const;
    
    T minimum() const;
    T middle() const;
    T maximum() const;

    QString toString() const;
};

/** This class represents a single histogram bin with its associated value */
template<class T>
class SIREMATHS_EXPORT HistogramValueT : public HistogramValue
{
public:
    HistogramValueT();
    HistogramValueT(const HistogramBinT<T> &bin, double value);
    
    HistogramValueT(const HistogramValue &other);
    HistogramValueT(const HistogramValueT<T> &other);
    
    ~HistogramValueT();
    
    HistogramValueT<T>& operator=(const HistogramValueT<T> &other);

    bool operator==(const HistogramValueT<T> &other) const;
    bool operator!=(const HistogramValueT<T> &other) const;
    
    T minimum() const;
    T middle() const;
    T maximum() const;
    
    QString toString() const;
};

/** This class represents a histogram range 
    (minimum, maximum, number of bins, bin width)
    
    @author Christopher Woods
*/
template<class T>
class SIREMATHS_EXPORT HistogramRangeT : public HistogramRange
{
public:
    HistogramRangeT();
    HistogramRangeT(T min, T max, int nbins=101);
    HistogramRangeT(T min, T max, T binwidth);
    
    HistogramRangeT(const HistogramRange &other);
    HistogramRangeT(const HistogramRangeT &other);
    
    ~HistogramRangeT();

    HistogramRangeT<T>& operator=(const HistogramRangeT<T> &other);

    bool operator==(const HistogramRangeT<T> &other) const;
    bool operator!=(const HistogramRangeT<T> &other) const;
    
    HistogramBinT<T> operator[](int i) const;
    
    T maximum() const;
    T middle() const;
    T minimum() const;

    T binWidth() const;

    int bin(T value) const;

    QString toString() const;
};

/** This class holds a simple one-dimensional (x,y) histogram of values.

    @author Christopher Woods
*/
template<class T>
class SIREMATHS_EXPORT HistogramT : public Histogram
{
public:
    HistogramT();
    
    HistogramT(T min, T max, int nbins=101);
    HistogramT(T min, T max, T binwidth);

    HistogramT(const HistogramRangeT<T> &range);

    HistogramT(const HistogramRangeT<T> &range,
               const QVector<double> &values);
    
    HistogramT(const Histogram &other);
    HistogramT(const HistogramT<T> &other);
    
    ~HistogramT();
    
    HistogramT<T>& operator=(const HistogramT<T> &other);

    bool operator==(const HistogramT<T> &other) const;
    bool operator!=(const HistogramT<T> &other) const;
    
    QString toString() const;
    
    HistogramValueT<T> operator[](int i) const;
    
    T maximum() const;
    T middle() const;
    T minimum() const;

    T binWidth() const;

    int bin(T value) const;
    
    void accumulate(T value);
    void accumulate(T value, double weight);
    
    void accumulate(const HistogramT<T> &other);
};

////////
//////// Implementation of HistogramBinT
////////

/** Constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramBinT<T>::HistogramBinT() : HistogramBin()
{}

/** Construct the bin from minval <= value < maxval */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramBinT<T>::HistogramBinT(T minval, T maxval)
                 : HistogramBin(minval, maxval)
{}

/** Copy construct from a non-T bin */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramBinT<T>::HistogramBinT(const HistogramBin &bin)
                 : HistogramBin(bin)
{}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramBinT<T>::HistogramBinT(const HistogramBinT<T> &other)
                 : HistogramBin(other)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramBinT<T>::~HistogramBinT()
{}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramBinT<T>& HistogramBinT<T>::operator=(const HistogramBinT<T> &other)
{
    HistogramBin::operator=(other);
    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool HistogramBinT<T>::operator==(const HistogramBinT<T> &other) const
{
    return HistogramBin::operator==(other);
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool HistogramBinT<T>::operator!=(const HistogramBinT<T> &other) const
{
    return HistogramBin::operator!=(other);
}

/** Return the minimum value */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T HistogramBinT<T>::minimum() const
{
    return T( HistogramBin::minimum() );
}

/** Return the value at the middle of the bin */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T HistogramBinT<T>::middle() const
{
    return T( HistogramBin::middle() );
}

/** Return the maximum value */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T HistogramBinT<T>::maximum() const
{
    return T( HistogramBin::maximum() );
}

/** Return a string representation of this bin */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QString HistogramBinT<T>::toString() const
{
    return QObject::tr("Bin[ %1 <= x < %2 ]")
                .arg( Sire::toString( this->minimum() ),
                      Sire::toString( this->maximum() ) );
}

////////
//////// Implementation of HistogramValueT
////////

/** Constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramValueT<T>::HistogramValueT() : HistogramValue()
{}

/** Construct from the passed bin and value */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramValueT<T>::HistogramValueT(const HistogramBinT<T> &bin, double value)
                   : HistogramValue(bin, value)
{}

/** Copy construct from a non-T value */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramValueT<T>::HistogramValueT(const HistogramValue &other)
                   : HistogramValue(other)
{}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramValueT<T>::HistogramValueT(const HistogramValueT<T> &other)
                   : HistogramValue(other)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramValueT<T>::~HistogramValueT()
{}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramValueT<T>& HistogramValueT<T>::operator=(const HistogramValueT<T> &other)
{
    HistogramValue::operator=(other);
    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool HistogramValueT<T>::operator==(const HistogramValueT<T> &other) const
{
    return HistogramValue::operator==(other);
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool HistogramValueT<T>::operator!=(const HistogramValueT<T> &other) const
{
    return HistogramValue::operator!=(other);
}

/** Return the minimum value */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T HistogramValueT<T>::minimum() const
{
    return T( HistogramValue::minimum() );
}

/** Return the value at the middle of the bin */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T HistogramValueT<T>::middle() const
{
    return T( HistogramValue::middle() );
}

/** Return the maximum value */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T HistogramValueT<T>::maximum() const
{
    return T( HistogramValue::maximum() );
}

/** Return a string representation of this value */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QString HistogramValueT<T>::toString() const
{
    return QObject::tr("Bin[ %1 <= x < %2 ] == %3")
                    .arg( Sire::toString(this->minimum()),
                          Sire::toString(this->maximum()) )
                    .arg(this->value());
}

////////
//////// Implementation of HistogramRangeT
////////

/** Constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramRangeT<T>::HistogramRangeT() : HistogramRange()
{}

/** Construct for min <= x < max, using 'nbins' bins */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramRangeT<T>::HistogramRangeT(T min, T max, int nbins)
                   : HistogramRange(min, max, nbins)
{}

/** Construct for min <= x < max using a binwidth of 'binwidth' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramRangeT<T>::HistogramRangeT(T min, T max, T binwidth)
                   : HistogramRange(min, max, binwidth)
{}

/** Copy construct from a non-T HistogramRange */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramRangeT<T>::HistogramRangeT(const HistogramRange &other)
                   : HistogramRange(other)
{}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramRangeT<T>::HistogramRangeT(const HistogramRangeT &other)
                   : HistogramRange(other)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramRangeT<T>::~HistogramRangeT()
{}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramRangeT<T>& HistogramRangeT<T>::operator=(const HistogramRangeT<T> &other)
{
    HistogramRange::operator=(other);
    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool HistogramRangeT<T>::operator==(const HistogramRangeT<T> &other) const
{
    return HistogramRange::operator==(other);
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool HistogramRangeT<T>::operator!=(const HistogramRangeT<T> &other) const
{
    return HistogramRange::operator!=(other);
}

/** Return the 'ith' bin

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramBinT<T> HistogramRangeT<T>::operator[](int i) const
{
    return HistogramBinT<T>( HistogramRange::operator[](i) );
}

/** Return the maximum value */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T HistogramRangeT<T>::maximum() const
{
    return T( HistogramRange::maximum() );
}

/** Return the middle of the histogram */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T HistogramRangeT<T>::middle() const
{
    return T( HistogramRange::middle() );
}

/** Return the minimum value of the histogram */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T HistogramRangeT<T>::minimum() const
{
    return T( HistogramRange::minimum() );
}

/** Return the bin width */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T HistogramRangeT<T>::binWidth() const
{
    return T( HistogramRange::binWidth() );
}

/** Return the index of the bin for the value 'value' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int HistogramRangeT<T>::bin(T value) const
{
    return HistogramRange::bin(value);
}

/** Return a string representation of this range */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QString HistogramRangeT<T>::toString() const
{
    return QObject::tr("Range[ %1 <= x < %2 : nBins() == %3 ]")
                    .arg( Sire::toString(this->minimum()),
                          Sire::toString(this->maximum()) )
                    .arg(this->nBins());
}

////////
//////// Implementation of HistogramT
////////

/** Constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramT<T>::HistogramT() : Histogram()
{}

/** Construct a histogram from min <= x < max, using 'nbins' bins */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramT<T>::HistogramT(T min, T max, int nbins)
              : Histogram(min, max, nbins)
{}

/** Construct a histogram from min <= x < max using a bin width of 'binwidth' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramT<T>::HistogramT(T min, T max, T binwidth)
              : Histogram(min, max, binwidth)
{}

/** Construct a histogram using the passed range */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramT<T>::HistogramT(const HistogramRangeT<T> &range)
              : Histogram(range)
{}

/** Construct a histogram using the passed range and initialising
    using the passed values
    
    \throw SireError::incompatible_error
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramT<T>::HistogramT(const HistogramRangeT<T> &range,
                          const QVector<double> &values)
              : Histogram(range, values)
{}

/** Copy construct from a non-T histogram */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramT<T>::HistogramT(const Histogram &other)
              : Histogram(other)
{}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramT<T>::HistogramT(const HistogramT<T> &other)
              : Histogram(other)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramT<T>::~HistogramT()
{}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramT<T>& HistogramT<T>::operator=(const HistogramT<T> &other)
{
    Histogram::operator=(other);
    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool HistogramT<T>::operator==(const HistogramT<T> &other) const
{
    return Histogram::operator==(other);
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool HistogramT<T>::operator!=(const HistogramT<T> &other) const
{
    return Histogram::operator!=(other);
}

/** Return a string representation of this histogram */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QString HistogramT<T>::toString() const
{
    return QObject::tr("Histogram[ %1 <= x < %2 : nBins() == %3 ]")
                    .arg( Sire::toString(this->minimum()),
                          Sire::toString(this->maximum()) )
                    .arg(this->nBins());
}

/** Return the value of the 'ith' bin

    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
HistogramValueT<T> HistogramT<T>::operator[](int i) const
{
    return HistogramValueT<T>( Histogram::operator[](i) );
}

/** Return the maximum value */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T HistogramT<T>::maximum() const
{
    return T( Histogram::maximum() );
}

/** Return the middle of the histogram */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T HistogramT<T>::middle() const
{
    return T( Histogram::middle() );
}

/** Return the minimum value of the histogram */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T HistogramT<T>::minimum() const
{
    return T( Histogram::minimum() );
}

/** Return the bin width */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T HistogramT<T>::binWidth() const
{
    return T( Histogram::binWidth() );
}

/** Return the index of the bin for the value 'value' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int HistogramT<T>::bin(T value) const
{
    return Histogram::bin(value);
}

/** Add the value 'value' to this histogram (with weight=1) */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void HistogramT<T>::accumulate(T value)
{
    Histogram::accumulate(value);
}

/** Add the value 'value' to this histogram (with weight='weight') */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void HistogramT<T>::accumulate(T value, double weight)
{
    Histogram::accumulate(value, weight);
}

/** Add the contents of the other histogram onto this one */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void HistogramT<T>::accumulate(const HistogramT<T> &other)
{
    Histogram::accumulate(other);
}

typedef HistogramT<SireUnits::Dimension::Length> LengthHistogram;
typedef HistogramBinT<SireUnits::Dimension::Length> LengthHistogramBin;
typedef HistogramValueT<SireUnits::Dimension::Length> LengthHistogramValue;
typedef HistogramRangeT<SireUnits::Dimension::Length> LengthHistogramRange;

typedef HistogramT<SireUnits::Dimension::Energy> EnergyHistogram;
typedef HistogramBinT<SireUnits::Dimension::Energy> EnergyHistogramBin;
typedef HistogramValueT<SireUnits::Dimension::Energy> EnergyHistogramValue;
typedef HistogramRangeT<SireUnits::Dimension::Energy> EnergyHistogramRange;

}

Q_DECLARE_METATYPE( SireMaths::Histogram )

SIRE_EXPOSE_CLASS( SireMaths::HistogramBin )
SIRE_EXPOSE_CLASS( SireMaths::HistogramValue )
SIRE_EXPOSE_CLASS( SireMaths::HistogramRange )
SIRE_EXPOSE_CLASS( SireMaths::Histogram )

SIRE_EXPOSE_ALIAS( 
        (HistogramT<SireUnits::Dimension::PhysUnit<0, 1, 0, 0, 0, 0, 0> >),
        SireMaths::LengthHistogram )
SIRE_EXPOSE_ALIAS( 
        (HistogramBinT<SireUnits::Dimension::PhysUnit<0, 1, 0, 0, 0, 0, 0> >),
        SireMaths::LengthHistogramBin )
SIRE_EXPOSE_ALIAS( 
        (HistogramValueT<SireUnits::Dimension::PhysUnit<0, 1, 0, 0, 0, 0, 0> >),
        SireMaths::LengthHistogramValue )
SIRE_EXPOSE_ALIAS( 
        (HistogramRangeT<SireUnits::Dimension::PhysUnit<0, 1, 0, 0, 0, 0, 0> >),
        SireMaths::LengthHistogramRange )

SIRE_EXPOSE_ALIAS( 
        (HistogramT<SireUnits::Dimension::PhysUnit<1, 2, -2, 0, 0, 0, 0> >),
        SireMaths::EnergyHistogram )
SIRE_EXPOSE_ALIAS( 
        (HistogramBinT<SireUnits::Dimension::PhysUnit<1, 2, -2, 0, 0, 0, 0> >),
        SireMaths::EnergyHistogramBin )
SIRE_EXPOSE_ALIAS( 
        (HistogramValueT<SireUnits::Dimension::PhysUnit<1, 2, -2, 0, 0, 0, 0> >),
        SireMaths::EnergyHistogramValue )
SIRE_EXPOSE_ALIAS( 
        (HistogramRangeT<SireUnits::Dimension::PhysUnit<1, 2, -2, 0, 0, 0, 0> >),
        SireMaths::EnergyHistogramRange )

#ifdef SIRE_INSTANTIATE_TEMPLATES
template class SireMaths::HistogramT<SireUnits::Dimension::Length>;
template class SireMaths::HistogramBinT<SireUnits::Dimension::Length>;
template class SireMaths::HistogramValueT<SireUnits::Dimension::Length>;
template class SireMaths::HistogramRangeT<SireUnits::Dimension::Length>;

template class SireMaths::HistogramT<SireUnits::Dimension::Energy>;
template class SireMaths::HistogramBinT<SireUnits::Dimension::Energy>;
template class SireMaths::HistogramValueT<SireUnits::Dimension::Energy>;
template class SireMaths::HistogramRangeT<SireUnits::Dimension::Energy>;
#endif


SIRE_END_HEADER

#endif
