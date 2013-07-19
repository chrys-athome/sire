/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2013  Christopher Woods
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

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

#ifndef SOIREE_TI_H
#define SOIREE_TI_H

#include "SireBase/property.h"
#include "SireMaths/histogram.h"

#include "SireUnits/temperature.h"
#include "SireUnits/dimensions.h"

#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER
/*
namespace Soiree
{
class TI;
class Gradient;
class Gradients;
class PMFValue;
class PMF;
}

namespace SireMaths
{
class FreeEnergyAverage;
}

QDataStream& operator<<(QDataStream&, const Soiree::TI&);
QDataStream& operator>>(QDataStream&, Soiree::TI&);

QDataStream& operator<<(QDataStream&, const Soiree::Gradient&);
QDataStream& operator>>(QDataStream&, Soiree::Gradient&);

QDataStream& operator<<(QDataStream&, const Soiree::Gradients&);
QDataStream& operator>>(QDataStream&, Soiree::Gradients&);

QDataStream& operator<<(QDataStream&, const Soiree::PMFValue&);
QDataStream& operator>>(QDataStream&, Soiree::PMFValue&);

QDataStream& operator<<(QDataStream&, const Soiree::PMF&);
QDataStream& operator>>(QDataStream&, Soiree::PMF&);*/

namespace Soiree
{

using SireUnits::Dimension::MolarEnergy;
using SireUnits::Dimension::Temperature;

/** This class represents a single Gradient value in a TI simulation

    @author Christopher Woods
*/
/*class SOIREE_EXPORT Gradient
{

friend QDataStream& ::operator<<(QDataStream&, const Gradient&);
friend QDataStream& ::operator>>(QDataStream&, Gradient&);

public:
    Gradient();
    Gradient(MolarEnergy value);
    Gradient(MolarEnergy forwards_value, MolarEnergy backwards_value);
    
    Gradient(MolarEnergy value, const Temperature &temperature);
    Gradient(MolarEnergy forwards_value, MolarEnergy backwards_value,
             const Temperature &temperature);

    Gradient(const SireMaths::FreeEnergyAverage &dg_fwds,
             const SireMaths::FreeEnergyAverage &dg_bwds,
             double delta);
    
    static Gradient combine(const QList<Gradient> &gradients);
    static Gradient combine(const Gradient &grad0, const Gradient &grad1);
    
    Gradient(const Gradient &other);
    
    ~Gradient();
    
    Gradient& operator=(const Gradient &other);
    
    bool operator==(const Gradient &other) const;
    bool operator!=(const Gradient &other) const;

    const char* what() const;
    static const char* typeName();
    
    QString toString() const;
    
    MolarEnergy average() const;
    
    MolarEnergy forwardsAverage() const;
    MolarEnergy backwardsAverage() const;
    
    MolarEnergy expAverage() const;
    
    MolarEnergy forwardsExpAverage() const;
    MolarEnergy backwardsExpAverage() const;
    
    MolarEnergy standardDeviation() const;
    
    MolarEnergy forwardsDeviation() const;
    MolarEnergy backwardsDeviation() const;
    
    MolarEnergy skew() const;
    
    MolarEnergy forwardsSkew() const;
    MolarEnergy backwardsSkew() const;
    
    Temperature temperature() const;
    
    quint64 nSamples() const;
    
private:
    /** The forwards mean average gradient value 
    double mean_fwds;
    
    /** The backwards mean average gradient value 
    double mean_bwds;
    
    /** The forwards exponential average gradient value 
    double exp_fwds;
    
    /** The backwards exponential average gradient value 
    double exp_bwds;
    
    /** The standard deviation on the forwards gradient 
    double stdev_fwds;
    
    /** The standard deviation on the backwards gradient 
    double stdev_bwds;
    
    /** The statistical skew on the forwards gradient 
    double skew_fwds;
    
    /** The statistical skew on the backwards gradient 
    double skew_bwds;

    /** The value of kT for the average. This is used when
        calculating the exponential average 
    double kt;
    
    /** The number of samples that make up this gradient 
    quint64 nsamples;
};*/

/** This class represents a set of thermodynamic integration gradients

    @author Christopher Woods
*/
/*class SOIREE_EXPORT Gradients : public SireBase::ConcreteProperty<Gradients,SireBase::Property>
{

friend QDataStream& ::operator<<(QDataStream&, const Gradients&);
friend QDataStream& ::operator>>(QDataStream&, Gradients&);

public:
    Gradients();
    Gradients(const QHash<double,Gradient> &gradients);
    
    Gradients(const Gradients &other);
    
    ~Gradients();
    
    Gradients& operator=(const Gradients &other);
    
    bool operator==(const Gradients &other) const;
    bool operator!=(const Gradients &other) const;
    
    const char* what() const;
    static const char* typeName();

    void set(double lam, const Gradient &gradient);

    int count() const;
    int size() const;
    
    Gradient operator[](double lam) const;
    
    Gradients discardEndPoints() const;
    
    QList<double> lambdaValues() const;
    QHash<double,Gradient> gradients() const;
    
    PMF integrate() const;

private:
    /** All of the gradients /
    QHash<double,Gradient> grads;
};*/

/** This class represents an individual value in the PMF 

    @author Christopher Woods
*/
/*class SOIREE_EXPORT PMFValue
{

friend QDataStream& ::operator<<(QDataStream&, const PMFValue&);
friend QDataStream& ::operator>>(QDataStream&, PMFValue&);

public:
    PMFValue();
    PMFValue(MolarEnergy value);
    PMFValue(MolarEnergy value, MolarEnergy error);
    PMFValue(MolarEnergy value, MolarEnergy small_error, MolarEnergy big_error);
    
    PMFValue(const PMFValue &other);
    
    ~PMFValue();
    
    PMFValue& operator=(const PMFValue &other);
    
    bool operator==(const PMFValue &other) const;
    bool operator!=(const PMFValue &other) const;
    
    const char* what() const;
    static const char* typeName();
    
    QString toString() const;
    
    MolarEnergy value() const;
    
    MolarEnergy error() const;
    
    MolarEnergy lowError() const;
    MolarEnergy highError() const;
    
private:
    /** The value of the PMF 
    double val;
    
    /** The error, if supplied 
    boost::shared_ptr<double> err;
    
    /** The smallest error, if supplied 
    boost::shared_ptr<double> low_err;

    /** The highest error, if supplied 
    boost::shared_ptr<double> hi_err;
};*/

/** This class represents a potential of mean force (PMF).

    @author Christopher Woods
*/
/*class SOIREE_EXPORT PMF : public SireBase::ConcreteProperty<PMF,SireBase::Property>
{

friend QDataStream& ::operator<<(QDataStream&, const PMF&);
friend QDataStream& ::operator>>(QDataStream&, PMF&);

public:
    PMF();
    
    PMF(const PMF &other);
    
    ~PMF();
    
    PMF& operator=(const PMF &other);
    
    bool operator==(const PMF &other) const;
    bool operator!=(const PMF &other) const;
    
    const char* what() const;
    static const char* typeName();
    
    QString toString() const;
    
private:
    /** The gradients of the PMF 
    QHash<double,Gradient> grads;
};*/
    

/** This class provides functions for the storage and analysis of 
    free energy gradients calculated during a TI simulation
    
    @author Christopher Woods
*/
/*class SOIREE_EXPORT TI : public SireBase::ConcreteProperty<TI,SireBase::Property>
{

friend QDataStream& ::operator<<(QDataStream&, const TI&);
friend QDataStream& ::operator>>(QDataStream&, TI&);

public:
    TI();
    TI(const QList<Gradients> &gradients);
    
    TI(const TI &other);
    
    ~TI();
    
    TI& operator=(const TI &other);
    
    bool operator==(const TI &other) const;
    bool operator!=(const TI &other) const;
    
    const char* what() const;
    static const char* typeName();
    
    QString toString() const;

    QString freeEnergyReport() const;
    
    SireUnits::Dimension::MolarEnergy freeEnergy() const;
    SireUnits::Dimension::MolarEnergy freeEnergyError() const;

    void add( const Gradients &gradients );
    
    void removeAt(int iteration);
    void removeRange(int start_iteration, int end_iteration);
    
    void clear();
    
    int count() const;
    int size() const;
    
    QList<double> lambdaValues() const;
    
    Gradients getGradients(int iteration) const;
    
    Gradients getAverageGradients(int niterations) const;
    Gradients getAverageGradients(int start_iteration, int end_iteration) const;
    
    Gradients getExpAverageGradients(int niterations,
                                     const SireUnits::Dimension::TempBase &temperature) const;
    Gradients getExpAverageGradients(int start_iteration, int end_iteration,
                                     const SireUnits::Dimension::TempBase &temperature) const;
    
    SireMaths::Histogram histogramGradients(double lam) const;
    SireMaths::Histogram histogramGradients(double lam, int niterations) const;
    SireMaths::Histogram histogramGradients(double lam,
                                            int start_iteration, int end_iteration) const;

    PMF integrate() const;

private:
    /** All of the gradients 
    QList<Gradients> grads;
};

}*/

SIRE_END_HEADER

#endif //ifndef SIRE_SKIP_INLINE_FUNCTIONS

#endif

