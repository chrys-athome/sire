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

#ifndef SIREMATHS_VECTOR_H
#define SIREMATHS_VECTOR_H

class QDataStream;
class QString;

#include <QObject>

#include <boost/tuple/tuple.hpp>

#include "SireMaths/errors.h"

#include "maths.h"

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Vector;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMaths::Vector&);
QDataStream& operator>>(QDataStream&, SireMaths::Vector&);

namespace SireMaths
{

class Quaternion;
class Matrix;

using boost::tuple;

using SireUnits::Dimension::Angle;

/**
This is a simple 3D vector.

A vector is implemented as a very simple collection of 3 doubles. It should
thus be very efficient in terms of speed and size as there is no array. This
does mean that the .x(), .y() and .z() functions provide faster access
than the [] operator.

There is a length/distance and a fastLength/fastDistance. The normal
version uses the standard library's sqrt function, while the fast versions
use a numerical approximation that is quite accurate (to within 1-2%).

These have been benchmarked on my development system using the 'testSpeed'
static function. The results are;

100,000,000 evaluations of vectors (1.0,2.0,3.0), (3.0,2.0,1.0)

distance took on average 4498 ms
distance2 took on average 148 ms
fastDistance took on average 350 ms

This shows that you should normally use 'distance2' for distance checking,
with fastDistance if you are not concerned with total accuracy.

The error with fastDistance has an oscillating value, with the error having
a maximum overestimate of 1*10-5, and maximum underestimate of 0.00065 between
the distances of 0.0 and 150.0

Within this range the percentage error also oscillates, with the oscillations
having a random frequency, but almost constant amplitude of 0.0005%. See the
graph in techdocs/fastLengthError.png and the script techdocs/fastLengthError.py

@author Christopher Woods
*/

class Quaternion;

class SIREMATHS_EXPORT Vector
{

friend QDataStream& ::operator<<(QDataStream&, const Vector&);
friend QDataStream& ::operator>>(QDataStream&, Vector&);

public:
    typedef double value_type;

    Vector( double val=0.0 );
    Vector( double xpos, double ypos, double zpos );
    Vector( const tuple<double,double,double> &pos );
    
    Vector( const QString &str );
    
    Vector(const Vector &other);
    
    ~Vector();

    double x() const;
    double y() const;
    double z() const;

    double r() const;
    double g() const;
    double b() const;

    const Vector& operator=(const Vector &other);

    bool operator==(const Vector &p1) const;
    bool operator!=(const Vector &p1) const;
    
    const Vector& operator+=(const Vector &other);
    const Vector& operator-=(const Vector &other);
    const Vector& operator*=(const double &other);
    const Vector& operator/=(const double &other);
    Vector operator-() const;

    void set(double x, double y, double z);
    void setX(double x);
    void setY(double y);
    void setZ(double z);
    void setR(double x);
    void setG(double y);
    void setB(double z);

    void set(int i, const double &v);

    const double& operator[](unsigned int i) const;

    unsigned int count() const;
    const double& at(unsigned int i) const;

    double manhattanLength() const;
    
    double length() const;
    double length2() const;
    
    double invLength() const;
    double invLength2() const;
    
    Vector normalise() const;

    bool isZero() const;

    QString toString() const;

    static Vector fromString(const QString &str);

    static double dot(const Vector &v0, const Vector &v1);
    static Vector cross(const Vector &v0, const Vector &v1);

    void setMax(const Vector &other);
    void setMin(const Vector &other);

    Vector max(const Vector &other) const;
    Vector min(const Vector &other) const;

    Angle bearing() const;
    Angle bearingXY(const Vector &v) const;
    Angle bearingXZ(const Vector &v) const;
    Angle bearingYZ(const Vector &v) const;

    Matrix metricTensor() const;

    static double distance2(const Vector &v1, const Vector &v2);
    static double distance(const Vector &v1, const Vector &v2);
    
    static double invDistance(const Vector &v1, const Vector &v2);
    static double invDistance2(const Vector &v1, const Vector &v2);

    static Angle angle(const Vector &v0, const Vector &v1);
    static Angle angle(const Vector &v0, const Vector &v1, const Vector &v2);

    static Angle dihedral(const Vector &v0, const Vector &v1,
                          const Vector &v2, const Vector &v3);

    static Vector generate(double dst, const Vector &v1, const Angle &ang,
                           const Vector &v2, const Angle &dih, const Vector &v3);

    friend const Vector SireMaths::operator+(const Vector &p1, const Vector &p2);
    friend const Vector SireMaths::operator-(const Vector &p1, const Vector &p2);
    friend const Vector SireMaths::operator*(const Vector &p1, double c);
    friend const Vector SireMaths::operator*(double c, const Vector &p1);
    friend const Vector SireMaths::operator/(const Vector &p1, double c);
    friend const Quaternion SireMaths::operator*(const Vector &p1, const Quaternion &p2);
    friend const Quaternion SireMaths::operator*(const Quaternion &p1, const Vector &p2);
    friend const Vector SireMaths::operator*(const Matrix &m, const Vector &p);

protected:
    double sc[3];
};

/** Copy constructor */
inline Vector::Vector(const Vector& other)
{
    qMemCopy(sc, other.sc, 3*sizeof(double));
}

/** Copy assignment operator */
inline const Vector& Vector::operator=(const Vector &other)
{
    qMemCopy(sc, other.sc, 3*sizeof(double));

    return *this;
}

/** Comparison operator */
inline bool Vector::operator==(const Vector &other) const
{
    return &other == this or
           (sc[0] == other.sc[0] and sc[1] == other.sc[1] and
            sc[2] == other.sc[2]);
}

/** Comparison operator */
inline bool Vector::operator!=(const Vector &other) const
{
    return &other != this and
           (sc[0] != other.sc[0] or sc[1] != other.sc[1] or
            sc[2] != other.sc[2]);
}

/** Return the x component of the vector */
inline double Vector::x() const
{
    return sc[0];
}

/** Return the y component of the vector */
inline double Vector::y() const
{
    return sc[1];
}

/** Return the z component of the vector */
inline double Vector::z() const
{
    return sc[2];
}

/** Return the distance squared between two vectors */
inline double Vector::distance2(const Vector &v1, const Vector &v2)
{
    return pow_2(v1.sc[0]-v2.sc[0]) +
           pow_2(v1.sc[1]-v2.sc[1]) +
           pow_2(v1.sc[2]-v2.sc[2]);
}

/** Return the distance between two vectors */
inline double Vector::distance(const Vector &v1, const Vector &v2)
{
    return std::sqrt( pow_2(v1.sc[0]-v2.sc[0]) +
                      pow_2(v1.sc[1]-v2.sc[1]) +
                      pow_2(v1.sc[2]-v2.sc[2]) );
}

/** Return the 1 / distance between two vectors */
inline double Vector::invDistance(const Vector &v1, const Vector &v2)
{
    double dist = pow_2(v1.sc[0]-v2.sc[0]) +
                  pow_2(v1.sc[1]-v2.sc[1]) +
                  pow_2(v1.sc[2]-v2.sc[2]);

    return double(1.0) / std::sqrt(dist);
}

/** Return 1 / distance2 between two vectors */
inline double Vector::invDistance2(const Vector &v1, const Vector &v2)
{
    return double(1.0) / ( pow_2(v1.sc[0]-v2.sc[0]) +
                           pow_2(v1.sc[1]-v2.sc[1]) +
                           pow_2(v1.sc[2]-v2.sc[2]) );
}

/** Access the elements of the array via an index operator */
inline const double& Vector::operator[](unsigned int i) const
{
    return sc[i%3];
}

/** Return the size of the Vector (always 3 - unless you disagree
    with me that we should be living in a 3-dimensional space!) */
inline unsigned int Vector::count() const
{
    return 3;
}

/** Access elements by index */
inline const double& Vector::at(unsigned int i) const
{
    return this->operator[](i);
}

/** Return the length of the vector */
inline double Vector::length() const
{
    return sqrt( pow_2(sc[0]) + pow_2(sc[1]) + pow_2(sc[2]) );
}

/** Return the length^2 of the vector */
inline double Vector::length2() const
{
    return pow_2(sc[0]) + pow_2(sc[1]) + pow_2(sc[2]);
}

/** Return the inverse of the length of the vector */
inline double Vector::invLength() const
{
    return double(1) / sqrt( pow_2(sc[0]) + pow_2(sc[1]) + pow_2(sc[2]) );
}

/** Return the inverse length squared */
inline double Vector::invLength2() const
{
    return double(1) / ( pow_2(sc[0]) + pow_2(sc[1]) + pow_2(sc[2]) );
}

/** Return whether or not this is a zero length vector */
inline bool Vector::isZero() const
{
    return SireMaths::isZero(sc[0]) and SireMaths::isZero(sc[1]) and SireMaths::isZero(sc[2]);
}

/** Return a normalised form of the vector */
inline Vector Vector::normalise() const
{
    double l = length2();

    if (SireMaths::isZero(l))
        throw SireMaths::math_error(QObject::tr(
            "Cannot normalise a zero vector, %1").arg(this->toString()),CODELOC);

    l = double(1) / sqrt(l);
    return Vector(sc[0]*l,sc[1]*l,sc[2]*l);
}

/** Return the dot product of v0 and v1 */
inline double Vector::dot(const Vector &v0, const Vector &v1)
{
    return (v0.sc[0]*v1.sc[0] + v0.sc[1]*v1.sc[1] + v0.sc[2]*v1.sc[2]);
}

/** Set this Vector so that it has the maximum x/y/z components out of
    this and 'other' (e.g. this->x = max(this->x(),other.x() etc.) */
inline void Vector::setMax(const Vector &other)
{
    for (int i=0; i<3; i++)
        sc[i] = SIRE_MAX( other.sc[i], sc[i] );
}

/** Set this Vector so that it has the minimum x/y/z components */
inline void Vector::setMin(const Vector &other)
{
    for (int i=0; i<3; i++)
        sc[i] = SIRE_MIN( other.sc[i], sc[i] );
}

/** Return a vector that has the maximum x/y/z components out of this
    and 'other' */
inline Vector Vector::max(const Vector &other) const
{
    Vector v(*this);
    v.setMax(other);
    return v;
}

/** Return a vector that has the minimum components */
inline Vector Vector::min(const Vector &other) const
{
    Vector v(*this);
    v.setMin(other);
    return v;
}

}

Q_DECLARE_METATYPE(SireMaths::Vector)
Q_DECLARE_TYPEINFO(SireMaths::Vector, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif

