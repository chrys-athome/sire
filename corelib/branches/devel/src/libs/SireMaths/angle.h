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

#include "maths.h"

#include "sireglobal.h"

class QString;

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Angle;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMaths::Angle&);
QDataStream& operator>>(QDataStream&, SireMaths::Angle&);

namespace SireMaths
{

/**
 
 This is an angle. This enables code to pass 'angles' around, without having to worry about what 
 units are used (degrees/radians etc). The angle is constrained to lie between -360 and 360 degrees
 (-2pi and 2pi radians), and is represented internally in radians. There is an implicit conversion
 defined that will convert angles to doubles, and doubles to angles, with the conversion being
 performed in radians. Positive angles are clockwise, negative angles are anticlockwise.
 
@author Christopher Woods
*/
class SIREMATHS_EXPORT Angle
{

friend QDataStream& ::operator<<(QDataStream&, const Angle&);
friend QDataStream& ::operator>>(QDataStream&, Angle&);

public:
    Angle();
    Angle(double rad);
    Angle(double x, double y);
    ~Angle();

    const double& toRadians() const;
    double toDegrees() const;
    void setDegrees(double theta);
    void setRadians(double theta);
    bool isWithin(const Angle &ang, const Angle &delta) const;

    bool isZero() const;

    QString toString() const;
    
    Angle& operator+=(const Angle &a);
    Angle& operator-=(const Angle &a);
    Angle& operator-=(double c);
    Angle& operator*=(double c);
    Angle& operator/=(double c);
    
    Angle small() const;
    Angle large() const;
    
    Angle positive() const;
    Angle negative() const;
    
    Angle smallDifference(const Angle &ang) const;
    Angle largeDifference(const Angle &ang) const;
    
    bool operator>(const Angle &other) const;
    bool operator>=(const Angle &other) const;
    bool operator<(const Angle &other) const;
    bool operator<=(const Angle &other) const;
    
    static Angle degrees(double deg);
    
protected:
    void fixAngle();
    
    /** The angle in radians */
    double rads;
};

/** Return the angle in radians */
inline const double& Angle::toRadians() const
{
    return rads;
}

/** Return whether or not this angle is equal to zero */
inline bool Angle::isZero() const
{
    return SireMaths::isZero(rads);
}

/** Value comparisons. Note that for the purpose of comparisons, the angle is
    constrained to lie between 0 and 360, and 359 > 0, 0 < 1 */
inline bool Angle::operator<(const Angle &ang) const
{
    return rads < ang.rads;
}

/** Value comparisons. Note that for the purpose of comparisons, the angle is
    constrained to lie between 0 and 360, and 359 > 0, 0 < 1 */
inline bool Angle::operator<=(const Angle &ang) const
{
    return rads <= ang.rads;
}

/** Value comparisons. Note that for the purpose of comparisons, the angle is
    constrained to lie between 0 and 360, and 359 > 0, 0 < 1 */
inline bool Angle::operator>(const Angle &ang) const
{
    return rads > ang.rads;
}

/** Value comparisons. Note that for the purpose of comparisons, the angle is
    constrained to lie between 0 and 360, and 359 > 0, 0 < 1 */
inline bool Angle::operator>=(const Angle &ang) const
{
    return rads >= ang.rads;
}

/** Return if two angles are equal */
inline bool operator==(const Angle &a1, const Angle &a2)
{
    return a1.toRadians() == a2.toRadians();
}

/** Return if two angles are not equal */
inline bool operator!=(const Angle &a1, const Angle &a2)
{
    return a1.toRadians() != a2.toRadians();
}

/** Add two angles */
inline Angle operator+(const Angle &a1, const Angle &a2)
{
    return Angle(a1.toRadians() + a2.toRadians());
}

/** Add a double and an angle (a2 assumed to be in radians) */
inline Angle operator+(const Angle &a1, double a2)
{
    return Angle(a1.toRadians() + a2);
}

/** Add a double and an angle (a2 assumed to be in radians) */
inline Angle operator+(double a2, const Angle &a1)
{
    return Angle(a1.toRadians() + a2);
}

/** Add a double and an angle (a2 assumed to be in radians) */
inline Angle operator+(const Angle &a1, int a2)
{
    return Angle(a1.toRadians() + a2);
}

/** Add a double and an angle (a2 assumed to be in radians) */
inline Angle operator+(int a2, const Angle &a1)
{
    return Angle(a1.toRadians() + a2);
}

/** Negate an angle */
inline Angle operator-(const Angle &ang)
{
    return Angle(-ang.toRadians());
}

/** Subtract one angle from another */
inline Angle operator-(const Angle &a1, const Angle &a2)
{
    return Angle(a1.toRadians() - a2.toRadians());
}

/** Subtract a double and an angle (a2 assumed to be in radians) */
inline Angle operator-(const Angle &a1, double a2)
{
    return Angle(a1.toRadians() - a2);
}

/** Subtract a double and an angle (a2 assumed to be in radians) */
inline Angle operator-(double a2, const Angle &a1)
{
    return Angle(a1.toRadians() - a2);
}

/** Subtract a double and an angle (a2 assumed to be in radians) */
inline Angle operator-(const Angle &a1, int a2)
{
    return Angle(a1.toRadians() - a2);
}

/** Subtract a double and an angle (a2 assumed to be in radians) */
inline Angle operator-(int a2, const Angle &a1)
{
    return Angle(a1.toRadians() - a2);
}

/** Multiply an angle by a constant */
inline Angle operator*(const Angle &a, double c)
{
    return Angle(a.toRadians() * c);
}

/** Multiply an angle by a constant */
inline Angle operator*(double c, const Angle &a)
{
    return Angle(a.toRadians() * c);
}

/** Multiply an angle by a constant */
inline Angle operator*(const Angle &a, int c)
{
    return Angle(a.toRadians() * c);
}

/** Multiply an angle by a constant */
inline Angle operator*(int c, const Angle &a)
{
    return Angle(a.toRadians() * c);
}

/** Divide an angle by a constant */
inline Angle operator/(const Angle &a, double c)
{
    if (c != 0)
        return Angle(a.toRadians() / c);
    else
        return a;
}

/** Divide an angle by an integer */
inline Angle operator/(const Angle &a, int c)
{
    if (c != 0)
        return Angle(a.toRadians() / c);
    else
        return a;
}

/** Double overloads to resolve ambiguities */
inline bool operator==(const double &val, const Angle &ang)
{
    return val == ang.toRadians();
}

/** Double overloads to resolve ambiguities */
inline bool operator==(const Angle &ang, const double &val)
{
    return ang.toRadians() == val;
}

/** Double overloads to resolve ambiguities */
inline bool operator!=(const double &val, const Angle &ang)
{
    return val != ang.toRadians();
}

/** Double overloads to resolve ambiguities */
inline bool operator!=(const Angle &ang, const double &val)
{
    return ang.toRadians() != val;
}

}

Q_DECLARE_METATYPE(SireMaths::Angle)
Q_DECLARE_TYPEINFO(SireMaths::Angle, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
