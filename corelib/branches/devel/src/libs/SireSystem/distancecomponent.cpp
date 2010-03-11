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

#include "distancecomponent.h"

#include "SireVol/space.h"

#include "SireSystem/system.h"

#include "SireID/index.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireFF;
using namespace SireCAS;
using namespace SireVol;
using namespace SireID;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<DistanceComponent> r_distcomp;

QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const DistanceComponent &distcomp)
{
    writeHeader(ds, r_distcomp, 1);
    
    SharedDataStream sds(ds);
    
    sds << distcomp.p0 << distcomp.p1
        << static_cast<const GeometryComponent&>(distcomp);
        
    return ds;
}

QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, DistanceComponent &distcomp)
{
    VersionID v = readHeader(ds, r_distcomp);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> distcomp.p0 >> distcomp.p1
            >> static_cast<GeometryComponent&>(distcomp);
            
        distcomp.intra_molecule_points = Point::areIntraMoleculePoints(
                                                        distcomp.p0, distcomp.p1 );
    }
    else
        throw version_error(v, "1", r_distcomp, CODELOC);
        
    return ds;
}

/** Null constructor */
DistanceComponent::DistanceComponent() 
                  : ConcreteProperty<DistanceComponent,GeometryComponent>(),
                    intra_molecule_points(true)
{}

Q_GLOBAL_STATIC_WITH_ARGS( Symbol, getRSymbol, ("r") );

/** Return the symbol that represents the distance between the
    two points ("r") */
const Symbol& DistanceComponent::r()
{
    return *(getRSymbol());
}

/** Construct to set the value of 'constrained_symbol' equal to the 
    distance between the two points 'point0' and 'point1' */
DistanceComponent::DistanceComponent(const Symbol &constrained_symbol,
                                     const PointRef &point0, const PointRef &point1)
       : ConcreteProperty<DistanceComponent,GeometryComponent>(constrained_symbol,
                                                               DistanceComponent::r()),
         p0(point0), p1(point1)
{
    intra_molecule_points = Point::areIntraMoleculePoints(p0,p1);
}

/** Construct to set the value of 'constrained_symbol' equal to the 
    expression based on the distance between the two points
    'point0' and 'point1' */
DistanceComponent::DistanceComponent(const Symbol &constrained_symbol,
                                     const PointRef &point0, const PointRef &point1,
                                     const Expression &geometry_expression)
       : ConcreteProperty<DistanceComponent,GeometryComponent>(constrained_symbol,
                                                               geometry_expression),
         p0(point0), p1(point1)
{
    intra_molecule_points = Point::areIntraMoleculePoints(p0,p1);
}
  
/** Copy constructor */
DistanceComponent::DistanceComponent(const DistanceComponent &other)
                  : ConcreteProperty<DistanceComponent,GeometryComponent>(other),
                    p0(other.p0), p1(other.p1),
                    intra_molecule_points(other.intra_molecule_points)
{}

/** Destructor */
DistanceComponent::~DistanceComponent()
{}

/** Copy assignment operator */
DistanceComponent& DistanceComponent::operator=(const DistanceComponent &other)
{
    if (this != &other)
    {
        GeometryComponent::operator=(other);
        p0 = other.p0;
        p1 = other.p1;
        intra_molecule_points = other.intra_molecule_points;
    }
    
    return *this;
}

/** Comparison operator */
bool DistanceComponent::operator==(const DistanceComponent &other) const
{
    return this != &other or
           (p0 == other.p0 and p1 == other.p1 and 
            GeometryComponent::operator==(other));
}

/** Comparison operator */
bool DistanceComponent::operator!=(const DistanceComponent &other) const
{
    return not DistanceComponent::operator==(other);
}

const char* DistanceComponent::typeName()
{
    return QMetaType::typeName( qMetaTypeId<DistanceComponent>() );
}

QString DistanceComponent::toString() const
{
    return QObject::tr("DistanceComponent( %1, %3-%4, %2 )")
                .arg(component().toString(), expression().toString() )
                .arg(p0.read().toString(), p1.read().toString());
}

/** Set the space used by the points and distance calculation */
void DistanceComponent::setSpace(const Space &new_space)
{
    if (space().equals(new_space))
        return;

    DistanceComponent old_state(*this);
    
    try
    {
        p0.edit().setSpace(new_space);
        p1.edit().setSpace(new_space);
        GeometryComponent::setSpace(new_space);
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }
}

/** Return the ith point

    \throw SireError::invalid_index
*/
const Point& DistanceComponent::point(int i) const
{
    i = Index(i).map( nPoints() );
    
    if (i == 0)
        return p0.read();
    else
        return p1.read();
}

/** Return the first point between which the distance is calculated */
const Point& DistanceComponent::point0() const
{
    return p0.read();
}

/** Return the second point between which the distance is calculated */
const Point& DistanceComponent::point1() const
{
    return p1.read();
}

/** Return the number of points (2) */
int DistanceComponent::nPoints() const
{
    return 2;
}

double DistanceComponent::getDistance() const
{
    if (intra_molecule_points)
        return Vector::distance(p0.read().point(), p1.read().point());
    else
        return space().calcDist(p0.read().point(), p1.read().point());
}

bool DistanceComponent::wouldChange(const System &system, const Values &values) const
{
    if (p0.read().wouldUpdate(system) or p1.read().wouldUpdate(system))
    {
        DistanceComponent n(*this);
        return values != n.getValues(system);
    }
    else
        return values[r()] != getDistance();
}

Values DistanceComponent::getValues(const System &system)
{
    if (p0.read().wouldUpdate(system))
        p0.edit().update(system);

    if (p1.read().wouldUpdate(system))
        p1.edit().update(system);
        
    return r() == getDistance();
}
