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

#include <QVector>

#include "Siren/object.h"
#include "Siren/objptr.hpp"
#include "Siren/mutable.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{

/** This is the base class of all Accumulators - these are objects
    that can accumulate values and calculate properties of that
    collection of values - e.g. they could accumulate the values
    so that the mean average could be collected, or so that the
    maximum and minimum values could be determined etc.
    
    @author Christopher Woods
*/
class SIREMATHS_EXPORT Accumulator 
        : public Siren::Extends<Accumulator,Siren::Object>,
          public Siren::Interfaces<Accumulator,Siren::Mutable>
{
public:
    Accumulator();
    
    Accumulator(const Accumulator &other);
    
    virtual ~Accumulator();
    
    ///////////////////////////
    // Extends Siren::Object //
    ///////////////////////////
    static QString typeName();
    uint hashCode() const;
    void stream(Siren::Stream &s);

    ///////////////////////////////
    // Interfaces Siren::Mutable //
    ///////////////////////////////
    Siren::ObjRef saveState() const;
    void restoreState(const Siren::Object &object);
    
    ////////////////////////////
    // SireMaths::Accumulator //
    ////////////////////////////
    
    virtual int nSamples() const;
    
    virtual void accumulate(double value);

    virtual void accumulate(const QVector<double> &values);
    virtual void accumulate(const QList<double> &values);
    
    virtual void clear();
    
    virtual operator double() const=0;
    
    virtual double value() const;
    
protected:
    Accumulator& operator=(const Accumulator &other);
    
    bool operator==(const Accumulator &other) const;
    bool operator!=(const Accumulator &other) const;

    friend class Siren::Extends<Accumulator,Object>;
    static QStringList listInterfaces()
    {
        return Siren::Interfaces<Accumulator,Siren::Mutable>::listInterfaces();
    }

private:
    /** The number of values that have been accumulated */
    quint32 nvalues;
};

/** This class is used to accumulate the mean average of a collection
    of values

    @author Christopher Woods
*/
class SIREMATHS_EXPORT Average : public Siren::Implements<Average,Accumulator>
{

public:
    Average();
    Average(const Average &other);
    
    ~Average();
    
    Average& operator=(const Average &other);
    
    bool operator==(const Average &other) const;
    bool operator!=(const Average &other) const;

    //////////////////////////////
    // Implements Siren::Object //
    //////////////////////////////
    QString toString() const;
    void stream(Siren::Stream &s);
    
    ////////////////////////////
    // Implements Accumulator //
    ////////////////////////////
    
    void clear();
    
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
         : public Siren::Implements<AverageAndStddev,Average>
{
public:
    AverageAndStddev();
    
    AverageAndStddev(const AverageAndStddev &other);
    
    ~AverageAndStddev();
    
    AverageAndStddev& operator=(const AverageAndStddev &other);
    
    bool operator==(const AverageAndStddev &other) const;
    bool operator!=(const AverageAndStddev &other) const;

    //////////////////////////////
    // Implements Siren::Object //
    //////////////////////////////
    QString toString() const;
    void stream(Siren::Stream &s);

    ////////////////////////////
    // Implements Accumulator //
    ////////////////////////////
    
    void clear();
    
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
class SIREMATHS_EXPORT ExpAverage : public Siren::Implements<ExpAverage,Accumulator>
{
public:
    ExpAverage(double scale=1);
    
    ExpAverage(const ExpAverage &other);
    
    ~ExpAverage();
    
    ExpAverage& operator=(const ExpAverage &other);
    
    bool operator==(const ExpAverage &other) const;
    bool operator!=(const ExpAverage &other) const;

    //////////////////////////////
    // Implements Siren::Object //
    //////////////////////////////
    QString toString() const;
    void stream(Siren::Stream &s);

    ////////////////////////////
    // Implements Accumulator //
    ////////////////////////////
    
    void clear();
    
    void accumulate(double value);

    double average() const;

    operator double() const;

protected:
    double scaleFactor() const;

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
class SIREMATHS_EXPORT Median : public Siren::Implements<Median,Accumulator>
{
public:
    Median();
    
    Median(const Median &other);
    
    ~Median();
    
    Median& operator=(const Median &other);
    
    bool operator==(const Median &other) const;
    bool operator!=(const Median &other) const;

    //////////////////////////////
    // Implements Siren::Object //
    //////////////////////////////
    QString toString() const;
    void stream(Siren::Stream &s);

    ////////////////////////////
    // Implements Accumulator //
    ////////////////////////////
    
    void clear();
    
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

/** This class is used to collect a record of all of the values.
    This allows you to extract the values and calculate whatever
    statistical property you wish in post-production
    
    @author Christopher Woods
*/
class SIREMATHS_EXPORT RecordValues : public Siren::Implements<RecordValues,Accumulator>
{
public:
    RecordValues();
    
    RecordValues(const RecordValues &other);
    
    ~RecordValues();
    
    RecordValues& operator=(const RecordValues &other);
    
    bool operator==(const RecordValues &other) const;
    bool operator!=(const RecordValues &other) const;

    //////////////////////////////
    // Implements Siren::Object //
    //////////////////////////////
    QString toString() const;
    void stream(Siren::Stream &s);

    ////////////////////////////
    // Implements Accumulator //
    ////////////////////////////
    
    void clear();
    
    void accumulate(double value);

    double max() const;
    double maximum() const;
    
    double median() const;
    double mean() const;
    
    double min() const;
    double minimum() const;

    double sum() const;
    double sum2() const;
    
    double meanOfSquares() const;
    
    double standardDeviation() const;
    double stddev() const;

    int count() const;
    int size() const;
    int nValues() const;

    operator double() const;

    QVector<double> values() const;

private:
    /** The record of all values */
    QVector<double> vals;
};

typedef Siren::ObjPtr<Accumulator> AccumulatorPtr;

}

Q_DECLARE_METATYPE( SireMaths::Average )
Q_DECLARE_METATYPE( SireMaths::AverageAndStddev )
Q_DECLARE_METATYPE( SireMaths::ExpAverage )
Q_DECLARE_METATYPE( SireMaths::Median )
Q_DECLARE_METATYPE( SireMaths::RecordValues )

SIRE_EXPOSE_CLASS( SireMaths::Accumulator )
SIRE_EXPOSE_CLASS( SireMaths::Average )
SIRE_EXPOSE_CLASS( SireMaths::AverageAndStddev )
SIRE_EXPOSE_CLASS( SireMaths::ExpAverage )
SIRE_EXPOSE_CLASS( SireMaths::Median )
SIRE_EXPOSE_CLASS( SireMaths::RecordValues )

SIRE_EXPOSE_OBJECT_PTR( SireMaths::AccumulatorPtr, SireMaths::Accumulator )

SIRE_END_HEADER

#endif
