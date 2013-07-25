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

#ifndef SOIREE_TI_H
#define SOIREE_TI_H

#include "SireMaths/freeenergyaverage.h"

#include "SireUnits/dimensions.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace Soiree
{
class TI;
class Gradients;
class DataPoint;
class PMF;
}

QDataStream& operator<<(QDataStream&, const Soiree::TI&);
QDataStream& operator>>(QDataStream&, Soiree::TI&);

QDataStream& operator<<(QDataStream&, const Soiree::Gradients&);
QDataStream& operator>>(QDataStream&, Soiree::Gradients&);

QDataStream& operator<<(QDataStream&, const Soiree::DataPoint&);
QDataStream& operator>>(QDataStream&, Soiree::DataPoint&);

QDataStream& operator<<(QDataStream&, const Soiree::PMF&);
QDataStream& operator>>(QDataStream&, Soiree::PMF&);

namespace Soiree
{

using SireMaths::FreeEnergyAverage;
using SireUnits::Dimension::MolarEnergy;

/** This class represents a single datapoint on an x,y graph. The point
    has associated errors (small and large) on both the x and y axes
    
    @author Christopher Woods
*/
class SOIREE_EXPORT DataPoint
{

friend QDataStream& ::operator<<(QDataStream&, const DataPoint&);
friend QDataStream& ::operator>>(QDataStream&, DataPoint&);

public:
    DataPoint();
    DataPoint(double x, double y);
    DataPoint(double x, double y,
              double xerror, double yerror);
    DataPoint(double x, double y,
              double xminerror, double yminerror,
              double xmaxerror, double ymaxerror);
    
    DataPoint(const DataPoint &other);
    
    ~DataPoint();
    
    DataPoint& operator=(const DataPoint &other);
    
    bool operator==(const DataPoint &other) const;
    bool operator!=(const DataPoint &other) const;
    
    const char* what() const;
    static const char* typeName();
   
    QString toString() const;
    
    double x() const;
    double y() const;
    
    double xError() const;
    double yError() const;
    
    double xMinError() const;
    double yMinError() const;
    
    double xMaxError() const;
    double yMaxError() const;
    
    bool hasError() const;
    bool hasErrorRange() const;
    
    bool hasXError() const;
    bool hasYError() const;
    
    bool hasXErrorRange() const;
    bool hasYErrorRange() const;
    
    bool equalWithinError(const DataPoint &other) const;
    bool equalWithinMinError(const DataPoint &other) const;
    bool equalWithinMaxError(const DataPoint &other) const;
    
private:
    /** The individual values of the data point */
    double _x, _y, _xminerr, _yminerr, _xmaxerr, _ymaxerr;
};

/** This class contains the complete potential of mean force
    that has been created by integrating the TI gradients
    
    @author Christopher Woods
*/
class SOIREE_EXPORT PMF : public SireBase::ConcreteProperty<PMF,SireBase::Property>
{

friend QDataStream& ::operator<<(QDataStream&, const PMF&);
friend QDataStream& ::operator>>(QDataStream&, PMF&);

public:
    PMF();
    PMF(int order);
    
    PMF(double min_x, double max_x);
    PMF(double min_x, double max_x, int order);
    
    PMF(const PMF &other);
    
    ~PMF();
    
    PMF& operator=(const PMF &other);
    
    bool operator==(const PMF &other) const;
    bool operator!=(const PMF &other) const;
    
    const char* what() const;
    static const char* typeName();

    QString toString() const;

    QVector<DataPoint> values() const;

    QVector<DataPoint> gradients() const;
    QVector<DataPoint> smoothedGradients() const;

    void setOrder(qint32 order);
    void setRange(double min_x, double max_x);

    void setGradients(const QVector<DataPoint> &gradients);
    
    int order() const;
    
    double rangeMin() const;
    double rangeMax() const;
    
    double integral() const;
    double quadrature() const;

private:
    void recalculate();

    /** The raw x,y points of the PMF gradients, complete with errors */
    QVector<DataPoint> grads;
    
    /** The smoothed x,y gradients, with errors */
    QVector<DataPoint> smoothed_grads;
    
    /** The smoothed x,y points of the PMF, with errors */
    QVector<DataPoint> vals;
    
    /** The minimum value of the range of the PMF */
    double range_min;
    
    /** The maximum value of the range of the PMF */
    double range_max;
    
    /** The free energy from quadrature */
    double quad_value;
    
    /** The number of polynomials to use to fit the gradients */
    qint32 npoly;
};

/** This class contains the free energy gradients from a TI simulation

    @author Christopher Woods
*/
class SOIREE_EXPORT Gradients
        : public SireBase::ConcreteProperty<Gradients,SireBase::Property>
{

friend QDataStream& ::operator<<(QDataStream&, const Gradients&);
friend QDataStream& ::operator>>(QDataStream&, Gradients&);

public:
    Gradients();
    Gradients(const QMap<double,FreeEnergyAverage> &gradients);
    Gradients(const QMap<double,FreeEnergyAverage> &gradients,
              double delta_lambda);
    Gradients(const QMap<double,FreeEnergyAverage> &forwards,
              const QMap<double,FreeEnergyAverage> &backwards,
              double delta_lambda);
    
    Gradients(const Gradients &other);
    
    ~Gradients();
    
    Gradients& operator=(const Gradients &other);
    
    bool operator==(const Gradients &other) const;
    bool operator!=(const Gradients &other) const;

    MolarEnergy operator[](double lam) const;

    static const char* typeName();
    
    const char* what() const;
    
    bool isEmpty() const;
    
    static Gradients merge(const QList<Gradients> &gradients);

    QList<double> lambdaValues() const;
    QList<double> keys() const;

    int nLambdaValues() const;
    qint64 nSamples() const;

    double deltaLambda() const;
    
    QVector<DataPoint> values() const;
    
    QVector<DataPoint> forwardsValues() const;
    QVector<DataPoint> backwardsValues() const;

    QMap<double,FreeEnergyAverage> forwardsData() const;
    QMap<double,FreeEnergyAverage> backwardsData() const;

    MolarEnergy forwards(double lam) const;
    MolarEnergy backwards(double lam) const;
    MolarEnergy gradient(double lam) const;

    PMF integrate() const;
    PMF integrate(int order) const;
    
    PMF integrate(double range_min, double range_max) const;
    PMF integrate(double range_min, double range_max, int order) const;
    

private:
    /** The forwards values */
    QMap<double,FreeEnergyAverage> fwds;

    /** The backwards values */
    QMap<double,FreeEnergyAverage> bwds;

    /** The value of delta lambda */
    double delta_lam;
};

/** This class is used to analyse the free energies that are
    calculated during a thermodynamic integration simulation
    
    @author Christopher Woods
*/
class SOIREE_EXPORT TI : public SireBase::ConcreteProperty<TI,SireBase::Property>
{

friend QDataStream& ::operator<<(QDataStream&, const TI&);
friend QDataStream& ::operator>>(QDataStream&, TI&);

public:
    TI();
    TI(const Gradients &gradients);
    TI(const QList<Gradients> &gradients);
    
    TI(const TI &other);
    
    ~TI();
    
    TI& operator=(const TI &other);
    
    bool operator==(const TI &other) const;
    bool operator!=(const TI &other) const;
    
    const char* what() const;
    static const char* typeName();
    
    QString toString() const;
    
    void add(const QMap<double,FreeEnergyAverage> &gradients);
    void add(const QMap<double,FreeEnergyAverage> &gradients,
             double delta_lambda);
    
    void add(const QMap<double,FreeEnergyAverage> &forwards,
             const QMap<double,FreeEnergyAverage> &backwards,
             double delta_lambda);
    
    void add(const Gradients &gradients);
    
    int nIterations() const;
    int nLambdaValues() const;
    qint64 nSamples() const;
    
    int count() const;
    int size() const;
    
    QList<double> lambdaValues() const;
    
    Gradients operator[](int i) const;
    Gradients at(int i) const;
    
    QList<Gradients> gradients() const;
    
    void set(int i, const QMap<double,FreeEnergyAverage> &gradients);
    void set(int i, const QMap<double,FreeEnergyAverage> &gradients,
             double delta_lambda);
    
    void set(int i, const QMap<double,FreeEnergyAverage> &forwards,
                    const QMap<double,FreeEnergyAverage> &backwards,
                    double delta_lambda);

    void set(int i, const Gradients &gradients);
    
    Gradients merge(int start, int end);
    Gradients merge(QList<int> indicies);
    
    QList<Gradients> rollingAverage(int niterations) const;
    
    void removeAt(int i);
    void removeRange(int start, int end);
    
    void clear();
    
private:
    /** The set of gradients for each iteration */
    QList<Gradients> grads;
};

}

Q_DECLARE_METATYPE( Soiree::Gradients )
Q_DECLARE_METATYPE( Soiree::TI )
Q_DECLARE_METATYPE( Soiree::DataPoint )
Q_DECLARE_METATYPE( Soiree::PMF )

SIRE_EXPOSE_CLASS( Soiree::Gradients )
SIRE_EXPOSE_CLASS( Soiree::TI )
SIRE_EXPOSE_CLASS( Soiree::DataPoint )
SIRE_EXPOSE_CLASS( Soiree::PMF )

SIRE_END_HEADER

#endif

