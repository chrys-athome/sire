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

#ifndef SIRESYSTEM_DISTANCECOMPONENT_H
#define SIRESYSTEM_DISTANCECOMPONENT_H

#include "geometrycomponent.h"

#include "SireFF/point.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class DistanceComponent;
}

QDataStream& operator<<(QDataStream&, const SireSystem::DistanceComponent&);
QDataStream& operator>>(QDataStream&, SireSystem::DistanceComponent&);

namespace SireSystem
{

/** This is a constraint that constrains a symbol to equal the
    value of an expression that involves a distance between atoms
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT DistanceComponent
         : public SireBase::ConcreteProperty<DistanceComponent,GeometryComponent>
{

friend QDataStream& ::operator<<(QDataStream&, const DistanceComponent&);
friend QDataStream& ::operator>>(QDataStream&, DistanceComponent&);

public:
    DistanceComponent();
    DistanceComponent(const SireCAS::Symbol &constrained_symbol,
                      const SireFF::PointRef &point0, const SireFF::PointRef &point1);
    DistanceComponent(const SireCAS::Symbol &constrained_symbol,
                      const SireFF::PointRef &point0, const SireFF::PointRef &point1,
                      const SireCAS::Expression &geometry_expression);
                      
    DistanceComponent(const DistanceComponent &other);
    
    ~DistanceComponent();
    
    DistanceComponent& operator=(const DistanceComponent &other);
    
    bool operator==(const DistanceComponent &other) const;
    bool operator!=(const DistanceComponent &other) const;
    
    static const char* typeName();
    
    QString toString() const;
    
    void setSpace(const SireVol::Space &space);
    
    const SireFF::Point& point(int i) const;
    
    const SireFF::Point& point0() const;
    const SireFF::Point& point1() const;
    
    int nPoints() const;
    
    static const SireCAS::Symbol& r();
    
protected:
    bool wouldChange(const System &system, const SireCAS::Values &values) const;
    SireCAS::Values getValues(const System &system);

private:
    double getDistance() const;

    /** The two points between which the distance is calculated */
    SireFF::PointPtr p0, p1;
    
    /** Whether or not the points are within the same molecule */
    bool intra_molecule_points;
};

}

Q_DECLARE_METATYPE( SireSystem::DistanceComponent )

SIRE_EXPOSE_CLASS( SireSystem::DistanceComponent )

SIRE_END_HEADER

#endif
