/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#ifndef SIREMATHS_WHAM_H
#define SIREMATHS_WHAM_H

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class WHAM;
}

QDataStream& operator<<(QDataStream&, const SireMaths::WHAM&);
QDataStream& operator>>(QDataStream&, SireMaths::WHAM&);

namespace SireMaths
{

/** This class applies the WHAM equations
    (weighted histogram analysis method) to unbias
    and combine together the results of a related
    set of umbrella sampling trajectories.
    
    The equations and method are based on that
    described in;
    
    J CHEM PHYS 111:17 P8048, 1999
    
    @author Christopher Woods
*/
class SIREMATHS_EXPORT WHAM
{

friend QDataStream& ::operator<<(QDataStream&, const WHAM&);
friend QDataStream& ::operator>>(QDataStream&, WHAM&);

public:
    WHAM();
    WHAM(const Symbol &coordinate, const Symbol &umbrella);
    WHAM(const Symbol &coordinate, const Symbol &umbrella,
         SireUnits::Dimension::Temperature temperature);
    
    WHAM(const QVector<Symbol> &coordinates, const Symbol &umbrella);
    WHAM(const QVector<Symbol> &coordinates, const Symbol &umbrella,
         SireUnits::Dimension::Temperature temperature);
    
    WHAM(const QSet<Symbol> &coordinates, const Symbol &umbrella);
    WHAM(const QSet<Symbol> &coordinates, const Symbol &umbrella,
         SireUnits::Dimension::Temperature temperature);
         
    WHAM(const WHAM &other);
    
    ~WHAM();
    
    static const char* typeName();
    const char* what() const;
    
    WHAM* clone() const;
    
    WHAM& operator=(const WHAM &other);
    
    bool operator==(const WHAM &other) const;
    bool operator!=(const WHAM &other) const;
    
    WHAM& operator+=(const WHAM &other);
    WHAM operator+(const WHAM &other) const;
    
    int nReactionCoordinates() const;
    
    const Symbol& reactionCoordinate() const;
    const QVector<Symbol>& reactionCoordinates() const;
    
    bool isEmpty() const;
    
    int nTrajectories() const;
    quint64 nSteps(int i) const;
    
    SireUnits::Dimension::Temperature temperature() const;
    
    QVector<double> coordinateTrajectory() const;
    QVector<double> coordinateTrajectory(const Symbol &coordinate) const;
    QVector<double> umbrellaTrajectory() const;
    
    QVector<double> coordinateTrajectory(int i) const;
    QVector<double> coordinateTrajectory(const Symbol &coordinate, int i) const;
    QVector<double> umbrellaTrajectory(int i) const;
    
    WHAM& add(const QVector<double> &coord_values,
              const QVector<double> &umbrella_values,
              bool new_trajectory=false);

    WHAM& add(double coord_value, double umbrella_value,
              bool new_trajectory=false);

    WHAM& add(const QHash< Symbol,QVector<double> > &coord_values,
              const QVector<double> &umbrella_values,
              bool new_trajectory=false);

    WHAM& add(const Values &coord_values, double umbrella_value,
              bool new_trajectory=false);

    WHAM& add(const WHAM &other);

    QVector< QPair<QVector<double>,double> > solve(
                                    const QHash<Symbol,HistogramRange> &range) const;
    
    QVector< QPair<double,double> > solve(const HistogramRange &range) const;

private:
    /** The trajectories for each reaction coordinate */
    QVector< QHash< Symbol,QMap<quint64,double> > > coords_values;
    
    /** The value of the umbrella potential at each step
        for each of the trajectories */
    QVector< QMap<quint64,double> > umbrella_values;
    
    /** The total number of steps in each trajectory */
    QVector<quint64> nsteps;
    
    /** The symbols representing each of the reaction coordinates */
    QVector<Symbol> reaction_coords;
    
    /** The temperature at which the simulations were run */
    SireUnits::Dimension::Temperature temp;
};

}

Q_DECLARE_METATYPE( SireMaths::WHAM )

SIRE_EXPOSE_CLASS( SireMaths::WHAM )

SIRE_END_HEADER

#endif
