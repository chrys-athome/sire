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

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace Soiree
{
class TI;
}

QDataStream& operator<<(QDataStream&, const Soiree::TI&);
QDataStream& operator>>(QDataStream&, Soiree::TI&);

namespace Soiree
{

using SireMaths::FreeEnergyAverage;

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
    
    int nIterations() const;
    int nLambdaValues() const;
    qint64 nSamples() const;
    
    int count() const;
    int size() const;
    
    QList<double> lambdaValues() const;
    
    boost::tuple< QMap<double,FreeEnergyAverage>,
                  QMap<double,FreeEnergyAverage>,
                  double > operator[](int i) const;
    
    boost::tuple< QMap<double,FreeEnergyAverage>,
                  QMap<double,FreeEnergyAverage>,
                  double > at(int i) const;
    
    void set(int i, const QMap<double,FreeEnergyAverage> &gradients);
    void set(int i, const QMap<double,FreeEnergyAverage> &gradients,
             double delta_lambda);
    
    void set(int i, const QMap<double,FreeEnergyAverage> &forwards,
                    const QMap<double,FreeEnergyAverage> &backwards,
                    double delta_lambda);
    
    void removeAt(int i);
    void removeRange(int start, int end);
    
private:
    /** The set of delta lambdas for each iteration */
    QList<double> delta_lams;

    /** The forwards free energy gradients for each iteration */
    QList< QMap<double,FreeEnergyAverage> > fwds_grads;
    
    /** The backwards free energy gradients for each iteration */
    QList< QMap<double,FreeEnergyAverage> > bwds_grads;
};

}

Q_DECLARE_METATYPE( Soiree::TI )

SIRE_EXPOSE_CLASS( Soiree::TI )

SIRE_END_HEADER

#endif

