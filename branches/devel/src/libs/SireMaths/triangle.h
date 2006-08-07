#ifndef SIREMATHS_TRIANGLE_H
#define SIREMATHS_TRIANGLE_H

#include "vector.h"
#include "line.h"
#include "angle.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Triangle;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMaths::Triangle&);
QDataStream& operator>>(QDataStream&, SireMaths::Triangle&);

namespace SireMaths
{

/**
This class represents a triangle in three-dimensional space. (or three points)
 
@author Christopher Woods
*/
class SIREMATHS_EXPORT Triangle
{

friend QDataStream& ::operator<<(QDataStream&, const Triangle&);
friend QDataStream& ::operator>>(QDataStream&, Triangle&);

public:
    Triangle();
    Triangle(const Vector &point0, const Vector &point1, 
             const Vector &point2);
    
    Triangle(const Triangle &other);
    
    ~Triangle();

    QString toString() const;

    Angle angle() const;
    
    Angle angle0() const;
    Angle angle1() const;
    Angle angle2() const;
    
    Vector vector() const;

    Line line0() const;
    Line line1() const;
    Line line2() const;

    Vector vector0() const;
    Vector vector1() const;
    Vector vector2() const;

    int count() const;
    
    const Vector& point(int i) const;
    const Vector& operator[](int i) const;
    const Vector& at(int i) const;

private:

    /** The three points that make up the triangle */
    Vector points[3];
};

/** Return the number of elements in this triangle (3) */
inline int Triangle::count() const
{
    return 3;
}

/** Return the i'th point */
inline const Vector& Triangle::point(int i) const
{
    return points[ i % 3 ];
}

/** Return the i'th point */
inline const Vector& Triangle::at(int i) const
{
    return point(i);
}

/** Return the i'th point */
inline const Vector& Triangle::operator[](int i) const
{
    return point(i);
}

/** Return the line of the triangle opposite point0 (from point 1 -> 2) */
inline Line Triangle::line0() const
{
    return Line(points[1], points[2]);
}

/** Return the line of the triangle opposite point1 (from point 2 -> 0) */
inline Line Triangle::line1() const
{
    return Line(points[2], points[0]);
}

/** Return the line of the triangle opposite point2 (from point 0 -> 1) */
inline Line Triangle::line2() const
{
    return Line(points[0], points[1]);
}

/** Return the vector representing length0 */
inline Vector Triangle::vector0() const
{
    return line0().vector();
}

/** Return the vector representing length0 */
inline Vector Triangle::vector1() const
{
    return line1().vector();
}

/** Return the vector representing length0 */
inline Vector Triangle::vector2() const
{
    return line2().vector();
}

/** Return the vector that is perpendicular to the plane of this triangle */
inline Vector Triangle::vector() const
{
    return Vector::cross( vector0(), vector2() );
}

/** Return the angle at point0 (i.e. the angle point2-point0-point1) (< 180 degrees) */
inline Angle Triangle::angle0() const
{
    return Vector::angle( points[2]-points[0], points[1]-points[0] );
}

/** Return the angle at point1 (i.e. the angle point0-point1-point2) (< 180 degrees) */
inline Angle Triangle::angle1() const
{
    return Vector::angle( points[0]-points[1], points[2]-points[1] );
}

/** Return the angle at point2 (i.e. the angle point1-point2-point0) (< 180 degrees) */
inline Angle Triangle::angle2() const
{
    return Vector::angle( points[1]-points[2], points[0]-points[2] );
}

/** Convienience function - when using this with atomic angles, the most interesting
    angle is the one on point1. This function is therefore a synonym for angle1 */
inline Angle Triangle::angle() const
{
    return this->angle1();
}

}

Q_DECLARE_METATYPE(SireMaths::Triangle)
Q_DECLARE_TYPEINFO(SireMaths::Triangle, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
