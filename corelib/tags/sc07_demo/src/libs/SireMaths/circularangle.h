/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMATHS_ANGLE_H
#define SIREMATHS_ANGLE_H

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{

/** This class maps an angle onto a circle, so that it 
    is constrained to lie between 0 and 360 degrees.
 
    @author Christopher Woods
*/
class CircularAngle
{
public:
    CircularAngle();
    
    CircularAngle(SireUnits::Dimension::Angle angle);
    
    CircularAngle(double x, double y);
    
    CircularAngle(const CircularAngle &other);
    
    ~CircularAngle();
    
    CircularAngle& operator=(CircularAngle angle);
    CircularAngle& operator=(SireUnits::Dimension::Angle angle);
    
    bool isWithin(CircularAngle angle, SireUnits::Dimension::Angle delta) const;
    
    CircularAngle& operator+=(SireUnits::Dimension::Angle angle);
    CircularAngle& operator-=(SireUnits::Dimension::Angle angle);
    
    CircularAngle& operator+=(CircularAngle angle);
    CircularAngle& operator-=(CircularAngle angle);
    
    CircularAngle& operator*=(double val);
    CircularAngle& operator/=(double val);
    
    CircularAngle operator+(SireUnits::Dimension::Angle angle) const;
    CircularAngle operator-(SireUnits::Dimension::Angle angle) const;
    
    CircularAngle operator+(CircularAngle angle) const;
    CircularAngle operator-(CircularAngle angle) const;
    
    CircularAngle operator*(double val) const;
    CircularAngle operator/(double val) const;
    
    SireUnits::Dimension::Angle small() const;
    SireUnits::Dimension::Angle large() const;
    
    SireUnits::Dimension::Angle positive() const;
    SireUnits::Dimension::Angle negative() const;
    
    SireUnits::Dimension::Angle 
    smallDifference(SireUnits::Dimension::Angle angle) const;
    
    SireUnits::Dimension::Angle 
    largeDifference(SireUnits::Dimension::Angle angle) const;
    
    bool operator>(SireUnits::Dimension::Angle angle) const;
    bool operator>=(SireUnits::Dimension::Angle angle) const;
    bool operator<(SireUnits::Dimension::Angle angle) const;
    bool operator<=(SireUnits::Dimension::Angle angle) const;
    
    bool operator>(CircularAngle angle) const;
    bool operator>=(CircularAngle angle) const;
    bool operator<(CircularAngle angle) const;
    bool operator<=(CircularAngle angle) const;
    
    bool operator==(SireUnits::Dimension::Angle angle) const;
    bool operator!=(SireUnits::Dimension::Angle angle) const;
    
    bool operator==(CircularAngle angle) const;
    bool operator!=(CircularAngle angle) const;
    
    SireUnits::Dimension::Time time() const;
    
    double x() const;
    double y() const;
    
protected:
    static double fixAngle();
    
    /** The angle in radians */
    double rads;
};

}

Q_DECLARE_TYPEINFO(SireMaths::CircularAngle, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
