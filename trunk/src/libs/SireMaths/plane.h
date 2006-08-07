#ifndef SIREMATHS_PLANE_H
#define SIREMATHS_PLANE_H

#include "vector.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Plane;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMaths::Plane&);
QDataStream& operator>>(QDataStream&, SireMaths::Plane&);

namespace SireMaths
{

/**
This class represents an infinite plane, represented by a normal vector perpendicular to the plane, and the distance from the origin along that normal vector.
 
@author Christopher Woods
*/
class SIREMATHS_EXPORT Plane
{

friend QDataStream& ::operator<<(QDataStream&, const Plane&);
friend QDataStream& ::operator>>(QDataStream&, Plane&);

public:
    Plane();
    Plane(const Vector &normal, const double &distance);
    Plane(const double &a, const double &b, const double &c, const double &d);
    Plane(const Vector &normal, const Vector &contains_point);
    Plane(const Plane &other);
    ~Plane();

    const Vector& normal() const;
    const double& distanceFromOrigin() const;
    
    double a() const;
    double b() const;
    double c() const;
    double d() const;

    double distance(const Vector &point) const;

private:

    Vector norm;
    double dist;

};

/** Return the (normalised) normal vector to the plane */
inline const Vector& Plane::normal() const
{
    return norm;
}

/** Return the distance of the plane from the origin along the normal vector */
inline const double& Plane::distanceFromOrigin() const
{
    return dist;
}

/** Return the 'a' component of the plane equation "ax + by + cz + d = 0" */
inline double Plane::a() const
{
    return norm.x();
}

/** Return the 'b' component of the plane equation "ax + by + cz + d = 0" */
inline double Plane::b() const
{
    return norm.y();
}

/** Return the 'c' component of the plane equation "ax + by + cz + d = 0" */
inline double Plane::c() const
{
    return norm.z();
}

/** Return the 'd' component of the plane equation "ax + by + cz + d = 0" */
inline double Plane::d() const
{
    return dist;
}

/** Return the shortest (signed) distance from the plane to the point 'point' */
inline double Plane::distance(const Vector &point) const
{
    return Vector::dot(norm, point) + dist;
}

}

Q_DECLARE_METATYPE(SireMaths::Plane)
Q_DECLARE_TYPEINFO(SireMaths::Plane, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
