/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SIREMM_DISTANCERESTRAINT_H
#define SIREMM_DISTANCERESTRAINT_H

#include "point.h"
#include "points.h"

#include "restraint.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class DistanceRestraint;
class DoubleDistanceRestraint;
class TripleDistanceRestraint;
}

namespace SireMM
{

class DistanceRestraints : 
    DistanceRestraints(const PointsRef &points0, const PointsRef &points1)
    

/** This is a restraint that operates on the distance between 
    two SireMM::Point objects (e.g. two atoms in a molecule,
    a point in space and the center of a molecule, the
    center of geometry of one molecule with the center of 
    mass of the solvent)
    
    @author Christopher Woods
*/
class SIREMM_EXPORT DistanceRestraint 
        : public SireBase::ConcreteProperty<DistanceRestraint,Restraint3D>
{

friend QDataStream& ::operator<<(QDataStream&, const DistanceRestraint&);
friend QDataStream& ::operator>>(QDataStream&, DistanceRestraint&);

public:
    DistanceRestraint();
    
    DistanceRestraint(const PointRef &point0, const PointRef &point1,
                      const Expression &restraint);

    DistanceRestraint(const DistanceRestraint &other);
    
    ~DistanceRestraint();
    
    DistanceRestraint& operator=(const DistanceRestraint &other);
    
    bool operator==(const DistanceRestraint &other) const;
    bool operator!=(const DistanceRestraint &other) const;
    
    static const char* typeName();
    
    const Point& point0() const;
    const Point& point1() const;
    
    SireUnits::Dimension::MolarEnergy energy() const;

    void update(const MoleculeData &moldata, 
                const PropertyMap &map = PropertyMap());
                
    void update(const Molecules &molecules,
                const PropertyMap &map = PropertyMap());
};



}

SIRE_END_HEADER

#endif
