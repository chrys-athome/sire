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
}

QDataStream& operator<<(QDataStream&, const Soiree::TI&);
QDataStream& operator>>(QDataStream&, Soiree::TI&);

QDataStream& operator<<(QDataStream&, const Soiree::Gradients&);
QDataStream& operator>>(QDataStream&, Soiree::Gradients&);

namespace Soiree
{

using SireMaths::FreeEnergyAverage;
using SireUnits::Dimension::MolarEnergy;

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
    
    QMap<double,FreeEnergyAverage> forwardsData() const;
    QMap<double,FreeEnergyAverage> backwardsData() const;

    MolarEnergy forwards(double lam) const;
    MolarEnergy backwards(double lam) const;
    MolarEnergy gradient(double lam) const;

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

SIRE_EXPOSE_CLASS( Soiree::Gradients )
SIRE_EXPOSE_CLASS( Soiree::TI )

SIRE_END_HEADER

#endif

