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

#include "wham.h"

#include "datastream.h"
#include "shareddatastream.h"

using namespace SireMaths;
using namespace SireUnits::Dimension;
using namespace SireUnits;
using namespace SireStream;

static const RegisterMetaType<WHAM> r_wham;

QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const WHAM &wham)
{
    writeHeader(ds, r_wham, 1);
    
    SharedDataStream sds(ds);
    
    sds << wham.coords_values << wham.umbrella_values
        << wham.nsteps << wham.reaction_coords
        << wham.temp.to(kelvin);
        
    return ds;
}

QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, WHAM &wham)
{
    VersionID v = readHeader(ds, r_wham);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        double temp;
        
        sds >> wham.coords_values >> wham.umbrella_values
            >> wham.nsteps >> wham.reaction_coords
            >> temp;
            
        wham.temp = temp*kelvin;
    }
    else
        throw version_error(v, "1", r_wham, CODELOC);
        
    return ds;
}

WHAM::WHAM();
WHAM::WHAM(const Symbol &coordinate, const Symbol &umbrella);
WHAM::WHAM(const Symbol &coordinate, const Symbol &umbrella,
     SireUnits::Dimension::Temperature temperature);

WHAM::WHAM(const QVector<Symbol> &coordinates, const Symbol &umbrella);
WHAM::WHAM(const QVector<Symbol> &coordinates, const Symbol &umbrella,
     SireUnits::Dimension::Temperature temperature);

WHAM::WHAM(const QSet<Symbol> &coordinates, const Symbol &umbrella);
WHAM::WHAM(const QSet<Symbol> &coordinates, const Symbol &umbrella,
     SireUnits::Dimension::Temperature temperature);
     
WHAM::WHAM(const WHAM &other);

WHAM::~WHAM();

const char* WHAM::typeName();
const char* WHAM::what() const;

WHAM* WHAM::clone() const;

WHAM& WHAM::operator=(const WHAM &other);

bool WHAM::operator==(const WHAM &other) const;
bool WHAM::operator!=(const WHAM &other) const;

WHAM& WHAM::operator+=(const WHAM &other);
WHAM WHAM::operator+(const WHAM &other) const;

int WHAM::nReactionCoordinates() const;

const Symbol& WHAM::reactionCoordinate() const;
const QVector<Symbol>& WHAM::reactionCoordinates() const;

bool WHAM::isEmpty() const;

int WHAM::nTrajectories() const;
quint64 WHAM::nSteps(int i) const;

Temperature WHAM::temperature() const;

QVector<double> WHAM::coordinateTrajectory() const;
QVector<double> WHAM::coordinateTrajectory(const Symbol &coordinate) const;
QVector<double> WHAM::umbrellaTrajectory() const;

QVector<double> WHAM::coordinateTrajectory(int i) const;
QVector<double> WHAM::coordinateTrajectory(const Symbol &coordinate, int i) const;
QVector<double> WHAM::umbrellaTrajectory(int i) const;

WHAM& WHAM::add(const QVector<double> &coord_values,
          const QVector<double> &umbrella_values,
          bool new_trajectory=false);

WHAM& WHAM::add(double coord_value, double umbrella_value,
          bool new_trajectory=false);

WHAM& WHAM::add(const QHash< Symbol,QVector<double> > &coord_values,
          const QVector<double> &umbrella_values,
          bool new_trajectory=false);

WHAM& WHAM::add(const Values &coord_values, double umbrella_value,
          bool new_trajectory=false);

WHAM& WHAM::add(const WHAM &other);

QVector< QPair<QVector<double>,double> > WHAM::solve(
                                const QHash<Symbol,HistogramRange> &range) const;

QVector< QPair<double,double> > WHAM::solve(const HistogramRange &range) const;
