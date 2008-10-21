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

#ifndef SIREMATHS_ACCUMULATOR_H
#define SIREMATHS_ACCUMULATOR_H

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Accumulator;
class NullAccumulator;
class Average;
class AverageAndStddev;
class ExpAverage;
class Median;
}

QDataStream& operator<<(QDataStream&, const SireMaths::Accumulator&);
QDataStream& operator>>(QDataStream&, SireMaths::Accumulator&);

QDataStream& operator<<(QDataStream&, const SireMaths::NullAccumulator&);
QDataStream& operator>>(QDataStream&, SireMaths::NullAccumulator&);

QDataStream& operator<<(QDataStream&, const SireMaths::Average&);
QDataStream& operator>>(QDataStream&, SireMaths::Average&);

QDataStream& operator<<(QDataStream&, const SireMaths::AverageAndStddev&);
QDataStream& operator>>(QDataStream&, SireMaths::AverageAndStddev&);

QDataStream& operator<<(QDataStream&, const SireMaths::ExpAverage&);
QDataStream& operator>>(QDataStream&, SireMaths::ExpAverage&);

QDataStream& operator<<(QDataStream&, const SireMaths::Median&);
QDataStream& operator>>(QDataStream&, SireMaths::Median&);

namespace SireMaths
{

/** This is the base class of all Accumulators - these are objects
    that can accumulate values and calculate properties of that
    collection of values - e.g. they could accumulate the values
    so that the mean average could be collected, or so that the
    maximum and minimum values could be determined etc.
    
    @author Christopher Woods
*/
class SIREMATHS_EXPORT Accumulator : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Accumulator&);
friend QDataStream& ::operator>>(QDataStream&, Accumulator&);

public:
    Accumulator();
    
    Accumulator(const Accumulator &other);
    
    virtual ~Accumulator();
    
    static const char* typeName()
    {
        return "SireMaths::Accumulator";
    }
    
    virtual Accumulator* clone() const=0;
    
    virtual int nSamples() const;
    
    virtual void accumulate(double value);

    virtual void accumulate(const QVector<double> &values);
    virtual void accumulate(const QList<double> &values);
    
    virtual void clear();
    
    static const NullAccumulator& null();
    
protected:
    Accumulator& operator=(const Accumulator &other);
    
    bool operator==(const Accumulator &other) const;
    bool operator!=(const Accumulator &other) const;

private:
    /** The number of values that have been accumulated */
    quint32 nvalues;
};

/** This is the null accumulator that doesn't accumulate anything

    @author Christopher Woods
*/
class SIREMATHS_EXPORT NullAccumulator
            : public SireBase::ConcreteProperty<NullAccumulator,Accumulator>
{

friend QDataStream& ::operator<<(QDataStream&, const NullAccumulator&);
friend QDataStream& ::operator>>(QDataStream&, NullAccumulator&);

public:
    NullAccumulator();
    
    NullAccumulator(const NullAccumulator &other);
    
    ~NullAccumulator();
    
    NullAccumulator& operator=(const NullAccumulator &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<NullAccumulator>() );
    }
    
    NullAccumulator* clone() const
    {
        return new NullAccumulator(*this);
    }
    
    bool operator==(const NullAccumulator &other) const;
    bool operator!=(const NullAccumulator &other) const;
    
    void accumulate(double value);
};

/** This class is used to accumulate the mean average of a collection
    of values

    @author Christopher Woods
*/
class SIREMATHS_EXPORT Average
            : public SireBase::ConcreteProperty<Average,Accumulator>
{

friend QDataStream& ::operator<<(QDataStream&, const Average&);
friend QDataStream& ::operator>>(QDataStream&, Average&);

public:
    Average();
    
    Average(const Average &other);
    
    ~Average();
    
    Average& operator=(const Average &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Average>() );
    }
    
    Average* clone() const
    {
        return new Average(*this);
    }
    
    bool operator==(const Average &other) const;
    bool operator!=(const Average &other) const;
    
    void accumulate(double value);

    double average() const;

    operator double() const;

private:
    /** The current average value */
    double avgval;
};

/** This class is used to accumulate the mean average 
    and standard deviation of a collection of values

    @author Christopher Woods
*/
class SIREMATHS_EXPORT AverageAndStddev 
         : public SireBase::ConcreteProperty<AverageAndStddev,Average>
{

friend QDataStream& ::operator<<(QDataStream&, const AverageAndStddev&);
friend QDataStream& ::operator>>(QDataStream&, AverageAndStddev&);

public:
    AverageAndStddev();
    
    AverageAndStddev(const AverageAndStddev &other);
    
    ~AverageAndStddev();
    
    AverageAndStddev& operator=(const AverageAndStddev &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<AverageAndStddev>() );
    }

    AverageAndStddev* clone() const
    {
        return new AverageAndStddev(*this);
    }
    
    bool operator==(const AverageAndStddev &other) const;
    bool operator!=(const AverageAndStddev &other) const;
    
    void accumulate(double value);

    double stddev() const;
    double standardDeviation() const;

    double meanOfSquares() const;
    
private:
    /** The current average of the squares */
    double avgval2;
};

/** This class is used to accumulate the exponential average 
    of a collection of values - this is the average formed
    as;
    
    avg =  scale * ln( Mean of e^(value/scale) )
    
    It is used to weight the average, e.g. if scale = -kT
    then this Boltzmann weights the average

    @author Christopher Woods
*/
class SIREMATHS_EXPORT ExpAverage
            : public SireBase::ConcreteProperty<ExpAverage,Accumulator>
{

friend QDataStream& ::operator<<(QDataStream&, const ExpAverage&);
friend QDataStream& ::operator>>(QDataStream&, ExpAverage&);

public:
    ExpAverage(double scale=1);
    
    ExpAverage(const ExpAverage &other);
    
    ~ExpAverage();
    
    ExpAverage& operator=(const ExpAverage &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<ExpAverage>() );
    }
    
    ExpAverage* clone() const
    {
        return new ExpAverage(*this);
    }
    
    bool operator==(const ExpAverage &other) const;
    bool operator!=(const ExpAverage &other) const;
    
    void accumulate(double value);

    double average() const;

    operator double() const;

private:
    /** The intermediate in the average calculation */
    double avgval;
    
    /** The scaling factor */
    double sclfac;
};

/** This class is used to calculate the maximum, minimum and median
    of a collection of values
    
    @author Christopher Woods
*/
class SIREMATHS_EXPORT Median
            : public SireBase::ConcreteProperty<Median,Accumulator>
{

friend QDataStream& ::operator<<(QDataStream&, const Median&);
friend QDataStream& ::operator>>(QDataStream&, Median&);

public:
    Median();
    
    Median(const Median &other);
    
    ~Median();
    
    Median& operator=(const Median &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Median>() );
    }
    
    Median* clone() const
    {
        return new Median(*this);
    }
    
    bool operator==(const Median &other) const;
    bool operator!=(const Median &other) const;
    
    void accumulate(double value);

    double max() const;
    double maximum() const;
    
    double median() const;
    
    double min() const;
    double minimum() const;

    operator double() const;

private:
    /** The current minimum value */
    double minval;
    
    /** The current maximum value */
    double maxval;
};

typedef SireBase::PropPtr<Accumulator> AccumulatorPtr;

}

Q_DECLARE_METATYPE( SireMaths::NullAccumulator )
Q_DECLARE_METATYPE( SireMaths::Average )
Q_DECLARE_METATYPE( SireMaths::AverageAndStddev )
Q_DECLARE_METATYPE( SireMaths::ExpAverage )
Q_DECLARE_METATYPE( SireMaths::Median )

SIRE_EXPOSE_CLASS( SireMaths::Accumulator )
SIRE_EXPOSE_CLASS( SireMaths::NullAccumulator )
SIRE_EXPOSE_CLASS( SireMaths::Average )
SIRE_EXPOSE_CLASS( SireMaths::AverageAndStddev )
SIRE_EXPOSE_CLASS( SireMaths::ExpAverage )
SIRE_EXPOSE_CLASS( SireMaths::Median )

SIRE_EXPOSE_PROPERTY( SireMaths::AccumulatorPtr, SireMaths::Accumulator )

SIRE_END_HEADER

#endif
