/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "distvector.h"

#include "SireUnits/units.h"
#include "SireStream/datastream.h"

#include "SireError/errors.h"

#include <QString>
#include <QRegExp>

#include <cmath>

using namespace SireMaths;
using namespace SireUnits;
using namespace SireStream;

static const RegisterMetaType<DistVector> r_distvector;

/** Serialise to a binary data stream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, 
                                         const SireMaths::DistVector &vec)
{
    writeHeader(ds, r_vector, 1) << vec.vec.sc[0] << vec.vec.sc[1]
                                 << vec.vec.sc[2] << vec.vec.sc[3];

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, 
                                         SireMaths::DistVector &vec)
{
    VersionID v = readHeader(ds, r_vector);

    if (v == 1)
    {
        ds >> vec.vec.sc[0] >> vec.vec.sc[1] >> vec.vec.sc[2] >> vec.vec.sc[3];
    }
    else
        throw version_error(v, "1", r_vector, CODELOC);

    return ds;
}

/** Create the zero vector */
DistVector::DistVector()
{
    for (int i=0; i<4; i++)
        vec.sc[i] = 0;
}

/** Create from the passed vector */
DistVector::DistVector( const Vector &vec )
{
    double dist = vec.length();
    
    if (SireMaths::isZero(dist))
    {
        for (int i=0; i<4; ++i)
            vec.sc[i] = 0;
    }
    else
    {
        vec.sc[3] = dist;
        dist = 1 / dist;
        
        vec.sc[0] = vec.x() * dist;
        vec.sc[1] = vec.y() * dist;
        vec.sc[2] = vec.z() * dist;
    }
}

/** Construct a DistVector from the QString representation returned by 'toString()' 

    \throw SireError::invalid_arg
*/
DistVector DistVector::fromString(const QString &str)
{
    return DistVector( Vector::fromString(str) );
}

/** Destructor */
DistVector::~DistVector()
{}


/** Copy constructor */
inline Vector::Vector(const Vector& other)
{
    qMemCopy(sc, other.sc, 4*sizeof(double));
}

/** Copy assignment operator */
inline const Vector& Vector::operator=(const Vector &other)
{
    qMemCopy(sc, other.sc, 4*sizeof(double));

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

/** Return the length of the vector */
inline double Vector::length() const
{
    return std::sqrt( pow_2(sc[0]) + pow_2(sc[1]) + pow_2(sc[2]) );
}

/** Return the length^2 of the vector */
inline double Vector::length2() const
{
    return pow_2(sc[0]) + pow_2(sc[1]) + pow_2(sc[2]);
}

/** Return the inverse of the length of the vector */
inline double Vector::invLength() const
{
    return double(1) / std::sqrt( pow_2(sc[0]) + pow_2(sc[1]) + pow_2(sc[2]) );
}

/** Return the inverse length squared */
inline double Vector::invLength2() const
{
    return double(1) / ( pow_2(sc[0]) + pow_2(sc[1]) + pow_2(sc[2]) );
}

/** Return the distance squared between two vectors */
inline double Vector::distance2(const Vector &v1, const Vector &v2)
{
    return pow_2(v2.sc[0]-v1.sc[0]) + pow_2(v2.sc[1]-v1.sc[1]) +
           pow_2(v2.sc[2]-v1.sc[2]);
}

/** Return the distance between two vectors */
inline double Vector::distance(const Vector &v1, const Vector &v2)
{
    return std::sqrt( pow_2(v2.sc[0]-v1.sc[0]) + pow_2(v2.sc[1]-v1.sc[1]) +
                      pow_2(v2.sc[2]-v1.sc[2]) );
}

/** Return the 1 / distance between two vectors */
inline double Vector::invDistance(const Vector &v1, const Vector &v2)
{
    return double(1.0) / std::sqrt( pow_2(v1.sc[0]-v2.sc[0]) +
                                    pow_2(v1.sc[1]-v2.sc[1]) +
                                    pow_2(v1.sc[2]-v2.sc[2]) );
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

/** Return a QString representation of the vector */
QString DistVector::toString() const
{
    return QObject::tr("( %1, %2, %3 )").arg(x()).arg(y()).arg(z());
}

/** Return the components via rgb (limited between 0 and 1) */
double DistVector::r() const
{
    return std::max(0.0, std::min(1.0,x()));
}

/** Return the components via rgb (limited between 0 and 1) */
double DistVector::g() const
{
    return std::max(0.0, std::min(1.0,y()));
}

/** Return the components via rgb (limited between 0 and 1) */
double DistVector::b() const
{
    return std::max(0.0, std::min(1.0,z()));
}

/** Return the direction of this vector */
const Vector& DistVector::direction() const
{
    return vec;
}

/** Return the magnitude of this vector */
double DistVector::magnitude() const
{
    return vec.sc[3];
} 

/** Return the bearing of this vector against (0,1,0) (north) on the xy plane */
Angle DistVector::bearing() const
{
    return this->direction().bearing();
}

/** Return the bearing of this vector against 'v' on the xy plane */
Angle DistVector::bearingXY(const DistVector &v) const
{
    return this->direction().bearingXY( v.direction() );
}

/** Return the bearing of this vector against 'v' on the xz plane */
Angle DistVector::bearingXZ(const DistVector &v) const
{
    return this->direction().bearingXZ( v.direction() );
}

/** Return the bearing of this vector against 'v' on the yz plane */
Angle DistVector::bearingYZ(const DistVector &v) const
{
    return this->direction().bearingYZ( v.direction() );
}

/** Return the angle between vectors 'v0' and 'v1' - this is the smallest
    angle, and will always lie between 0 and 180 degrees */
Angle DistVector::angle(const DistVector &v0, const DistVector &v1)
{
    return Vector::angle( v0.direction(), v1.direction() );
}

/** Return the angle between v0-v1-v2 (treating the vectors as points in space) */
Angle DistVector::angle(const DistVector &v0, const DistVector &v1, const DistVector &v2)
{
    return Vector::angle( v0, v1, v2 );
}

/** Return the dihedral angle between v0-v1-v2-v3 (treating the vectors as points) */
Angle DistVector::dihedral(const DistVector &v0, const DistVector &v1, const DistVector &v2, const DistVector &v3)
{
    return Vector::dihedral( v0, v1, v2, v3 );
}
    
/** Generate a vector, v0, that has distance 'dst' v0-v1, angle 'ang' v0-v1-v2,
    and dihedral 'dih' v0-v1-v2-v3 */
DistVector DistVector::generate(double dst, const DistVector &v1, const Angle &ang, 
                                const DistVector &v2,
                                const Angle &dih, const DistVector &v3)
{
    return DistVector( Vector::generate(dst, v1, ang, v2, dih, v3) );
}

 /** Return the cross product of v0 and v1 */
DistVector DistVector::cross(const DistVector &v0, const DistVector &v1)
{
    return Vector::cross( v0.direction(), v1.direction() );
}

/** Return the manhattan length of the vector */
double DistVector::manhattanLength() const
{
    return Vector(*this).manhattanLength();
}

/** Return the metric tensor of a vector, i.e.
          
    | y*y + z*z,    -x*y    -x*z      |
    |    -y*x,   x*x + z*z  -y*z      |
    |    -z*x       -z*y    x*x + y*y |

*/
Matrix DistVector::metricTensor() const
{
    return Vector(*this).metricTensor();
}

/** Increment, decrement, negate etc. */
const DistVector& DistVector::operator+=(const DistVector &other)
{
    return this->operator=( Vector(*this) + Vector(other) );
}

/** Increment, decrement, negate etc. */
const DistVector& DistVector::operator-=(const DistVector &other)
{
    return this->operator=( Vector(*this) - Vector(other) );
}

/** Increment, decrement, negate etc. */
const DistVector& DistVector::operator*=(const double &val)
{
    sc[3] *= val;
    return *this;
}

/** Increment, decrement, negate etc. */
const DistVector& DistVector::operator/=(const double &val)
{
    if ( SireMaths::isZero(val) )
        throw SireMaths::math_error(QObject::tr(
            "Cannot divide a vector by zero! %1 / 0 is a error!").arg(this->toString()),CODELOC);
    
    sc[3] /= val;
    return *this;
}

/** Increment, decrement, negate etc. */
DistVector DistVector::operator-() const
{
    DistVector ret(*this);
    ret.sc[3] = -sc[3];
    return ret;
}
