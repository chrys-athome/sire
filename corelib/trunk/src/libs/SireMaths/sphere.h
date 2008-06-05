#ifndef SIREMATHS_SPHERE_H
#define SIREMATHS_SPHERE_H

#include "vector.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Sphere;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMaths::Sphere&);
QDataStream& operator>>(QDataStream&, SireMaths::Sphere&);

namespace SireMaths
{

/**
This class is a mathematical representation of a sphere.
 
@author Christopher Woods
*/
class SIREMATHS_EXPORT Sphere
{

friend QDataStream& ::operator<<(QDataStream&, const Sphere&);
friend QDataStream& ::operator>>(QDataStream&, Sphere&);

public:
    Sphere();
    Sphere(const double &radius);
    Sphere(const Vector &position, const double &radius);
    Sphere(const Sphere &other);
    
    ~Sphere();

    bool operator==(const Sphere &other) const;
    bool operator!=(const Sphere &other) const;

    const Vector& position() const;
    const Vector& center() const;
    double radius() const;

    void setPosition(const Vector &position);
    void setCenter(const Vector &center);
    void setRadius(double radius);

private:

    /** The location of the center of the sphere */
    Vector _center;

    /** The radius of the sphere */
    double _radius;
};

/** Return the position of the center of the sphere */
inline const Vector& Sphere::position() const
{
    return _center;
}

/** Return the position of the center of the sphere */
inline const Vector& Sphere::center() const
{
    return _center;
}

/** Return the radius of the sphere */
inline double Sphere::radius() const
{
    return _radius;
}

}

Q_DECLARE_METATYPE(SireMaths::Sphere)
Q_DECLARE_TYPEINFO(SireMaths::Sphere, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
